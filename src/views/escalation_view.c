#include <views.h>


#define POPUP_FINISHED_MULTITHREAD_ESCALATION "Escalation finished"
#define POPUP_SEARCHING_MULTITHREAD_ESCALATION "Finding best machine sequence"
#define POPUP_MULTITHREAD_ESCALATION_NOT_AVAILABLE "Multithreaded escalation is only available on Windows"

void view_open_escalation() {
	gui_open_view(VIEW_TITLE_ESCALATION, NULL);
}


typedef struct {
	int job;
	int operation;
	int machine;
	int duration;
	int end;
} operation_info_t;


typedef struct {
	job_t job;
	int id;
} job_id_t;


int g_ending_time;
int g_jobs_count = 0;
list_t g_machines;
int g_machines_used_count = 0;
int g_machines_count = 0;
int g_prev_machines_count = 0;
int g_operations_count = 0;
list_t g_machines_list;
int* g_machines_ordering = NULL;

/* Used for searching best machine sequence multi-threaded. */
static int g_finished_machines_thread_count = 0;
static bool g_is_finding_machines = false;
static int g_current_machines_found = 0;


static inline bool machines_push_unique(int machine) {
	if (!machines_contains(machine)) {
		list_push(&g_machines, &machine, sizeof(int));
		return true;
	}

	return false;
}


static inline bool machines_contains(int machine) {
	LIST_START_ITERATION(&g_machines, int, m) {
		if (*m == machine)
			return true;
	}
	LIST_END_ITERATION;

	return false;
}


static int perform_escalation(list_t *machines_list, int* machines_ordering) {
	list_t* jobs = jobs_get_all();

	list_clear(machines_list);

	list_t temp_jobs = list_init(NULL);
	int job_id = 0;

	LIST_START_ITERATION(jobs, job_t, job) {
		int operation_id = 0;
		job_id_t temp_job = { .job = job_init(NULL), .id = job_id };
		LIST_START_ITERATION((&job->operations), operation_t, operation) {
			operation_t* temp_operation = job_new_operation(&temp_job.job, operation->name);
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				operation_info_t operation_info = (operation_info_t){
								.duration = execution->duration,
								.job = job_id,
								.machine = execution->machine,
								.end = execution->duration,
								.operation = operation_id
				};
				list_push(&temp_operation->executions, &operation_info, sizeof(operation_info_t));
			}
			LIST_END_ITERATION;
			operation_id++;
		}
		LIST_END_ITERATION;
		list_push(&temp_jobs, &temp_job, sizeof(temp_job));
		job_id++;
	}
	LIST_END_ITERATION;

	operation_info_t* machines = malloc(sizeof(operation_info_t) * g_machines_used_count);
	operation_info_t empty = {
		.duration = -1, .job = -1, .machine = -1, .operation = -1, .end = -1
	};

	for (int i = 0; i < g_machines_used_count; i++)
		machines[i] = empty;

	int cur_duration = 0;
	bool has_machines_working = true;

	while (has_machines_working) {

		for (int i = 0; i < g_machines_used_count; i++) {

			if (machines[i].end != -1)
			{
				if (machines[i].end == cur_duration)
					machines[i] = empty;
				else
					continue;
			}
		}

		for (int j = 0; j < g_machines_used_count; j++) {
			int i = machines_ordering[j];

			if (machines[j].end != -1)
				continue;

			operation_info_t fastest_operation = empty;

			int job_id = 0;

			LIST_START_ITERATION((&temp_jobs), job_id_t, job) {
				bool skip_job = false;

				// Check if the job has any operation being processed by any machine.
				for (int k = 0; k < g_machines_used_count; k++) {
					if (machines[machines_ordering[k]].job == job->id) {
						skip_job = true;
						break;
					}
				}

				if (skip_job)
					continue;

				LIST_START_ITERATION((&job->job.operations), operation_t, operation) {
					LIST_START_ITERATION((&operation->executions), operation_info_t, execution) {
						if (execution->machine != i)
							continue;

						if (fastest_operation.end == -1 || fastest_operation.duration > execution->duration)
							fastest_operation = (operation_info_t){
								.duration = execution->duration,
								.job = execution->job,
								.machine = execution->machine,
								.end = cur_duration + execution->duration,
								.operation = execution->operation
						};
					}
					LIST_END_ITERATION;
					break;
				}
				LIST_END_ITERATION;
				job_id++;
			}
			LIST_END_ITERATION;

			if (fastest_operation.end != -1) {
				machines[j] = fastest_operation;
				int job_id = 0;
				LIST_START_ITERATION((&temp_jobs), job_id_t, job) {
					if (job->id == fastest_operation.job)
					{
						list_push(machines_list, &fastest_operation, sizeof(fastest_operation));
						job_remove_operation(&job->job, 0);

						if (job->job.operations.first == NULL) {
							list_remove(&temp_jobs, job_id);
							break;
						}
						break;
					}
					job_id++;
				}
				LIST_END_ITERATION;
			}
		}

		int next_ending_machine = -1;
		has_machines_working = false;
		for (int i = 0; i < g_machines_used_count; i++) {
			if (machines[i].end != -1) {
				has_machines_working = true;
				if (next_ending_machine == -1 || machines[i].end < next_ending_machine) {
					next_ending_machine = machines[i].end;
				}
			}
		}

		if (has_machines_working)
			cur_duration = next_ending_machine;
	}

	list_clear(&temp_jobs);
	free(machines);

	return cur_duration;
}


static void perform_fast_escalation() {
	list_t* jobs = jobs_get_all();
	g_machines_used_count = 0;
	g_operations_count = 0;
	g_jobs_count = 0;
	g_machines_count = 0;

	list_clear(&g_machines);

	LIST_START_ITERATION(jobs, job_t, job) {
		LIST_START_ITERATION((&job->operations), operation_t, operation) {
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				if (machines_push_unique(execution->machine))
					g_machines_used_count++;

				if (execution->machine + 1 > g_machines_count)
					g_machines_count = execution->machine + 1;
			}
			LIST_END_ITERATION;
			g_operations_count++;
		}
		LIST_END_ITERATION;
		g_jobs_count++;
	}
	LIST_END_ITERATION;


	if (g_prev_machines_count != g_machines_used_count) {
		if (g_machines_ordering != NULL)
			free(g_machines_ordering);

		g_machines_ordering = malloc(sizeof(int) * g_machines_used_count);

		int i = 0;
		LIST_START_ITERATION(&g_machines, int, machine) {
			g_machines_ordering[i] = *machine;
			i++;
		}
		LIST_END_ITERATION;
	}

	g_prev_machines_count = g_machines_used_count;

	g_ending_time = perform_escalation(&g_machines_list, g_machines_ordering);
}


static void recursive_find_best_machine(int sequence, int i, int* ending, list_t* machines_list, int* ordering, int* best_ordering) {

	if (!g_is_finding_machines)
		return;

	ordering[sequence] = i;

	if (sequence == g_machines_used_count - 1) {
		int local_ending = 0;
		list_t local_machines = list_init(NULL);

		local_ending = perform_escalation(&local_machines, ordering);

		if (local_ending < *ending || *ending == 0) {
			*ending = local_ending;

			memcpy(best_ordering, ordering, sizeof(int) * g_machines_used_count);

			list_clear(machines_list);

			LIST_START_ITERATION((&local_machines), operation_info_t, operation) {
				list_push(machines_list, operation, sizeof(operation_info_t));
			}
			LIST_END_ITERATION;
		}

		list_clear(&local_machines);
		g_current_machines_found++;

		return;
	}

	LIST_START_ITERATION(&g_machines, int, machine) {
			bool contains_machine = false;

			for (int k = 0; k <= sequence; k++)
				if (ordering[k] == *machine)
					contains_machine = true;

			if (!contains_machine)
				recursive_find_best_machine(sequence + 1, *machine, ending, machines_list, ordering, best_ordering);
	}
	LIST_END_ITERATION;
}


typedef struct {
	int i;
} machine_sequence_data_t;

static bool lock = false;


#ifdef _WIN32
// Required for threading.
#include <windows.h>


DWORD WINAPI escalation_thread(LPVOID lpParameter)
{
	machine_sequence_data_t* data = (DWORD*)lpParameter;

	int ending_time = 0;
	list_t machines = list_init(NULL);
	int* ordering = malloc(sizeof(int) * g_machines_used_count);
	int* best_ordering = malloc(sizeof(int) * g_machines_used_count);

	recursive_find_best_machine(0, data->i, &ending_time, &machines, ordering, best_ordering);

	if (g_is_finding_machines)
	{
		while (lock) {
			Sleep(1);
		}
		lock = true;

		if (ending_time < g_ending_time) {
			g_ending_time = ending_time;

			list_clear(&g_machines_list);
			memcpy(g_machines_ordering, best_ordering, sizeof(int) * g_machines_used_count);

			LIST_START_ITERATION((&machines), operation_info_t, operation) {
				list_push(&g_machines_list, operation, sizeof(operation_info_t));
			}
			LIST_END_ITERATION;
		}
		lock = false;

		g_finished_machines_thread_count++;
	}
	
	free(best_ordering);
	free(ordering);
	list_clear(&machines);
	free(data);
}


static void find_best_machine_sequence() {

	g_finished_machines_thread_count = 0;
	g_is_finding_machines = true;
	g_current_machines_found = 0;
	lock = false;

	LIST_START_ITERATION(&g_machines, int, machine) {

		DWORD thread_id;
		HANDLE thread_handle;

		machine_sequence_data_t* data = malloc(sizeof(machine_sequence_data_t));
		data->i = *machine;

		thread_handle = CreateThread(NULL, 0, escalation_thread, data, 0, &thread_id);
	}
	LIST_END_ITERATION;
}

#else

static void find_best_machine_sequence() {
	gui_open_popup(POPUP_MULTITHREAD_ESCALATION_NOT_AVAILABLE);
}

#endif


/**
 * @brief Exports the machines in YAML format.
*/
static void export_machines(char* path) {
	FILE* file = fopen(path, "w");

	list_t* machines = malloc(sizeof(list_t) * g_machines_count);

	for (int i = 0; i < g_machines_count; i++)
		machines[i] = list_init(NULL);
	
	LIST_START_ITERATION((&g_machines_list), operation_info_t, operation) {
		list_push(&machines[operation->machine], operation, sizeof(operation_info_t));
	}
	LIST_END_ITERATION;

	for (int j = 0; j < g_machines_used_count; j++)
	{
		int i = g_machines_ordering[j];
		if (machines[i].first == NULL)
			continue;

		fprintf(file, "- machine %d:\n", i + 1);

		LIST_START_ITERATION((&machines[i]), operation_info_t, operation) {
			fprintf(file, "    - %d:\n        duration: %d\n        job: %d\n        operation: %d\n", operation->end - operation->duration, operation->duration, operation->job + 1, operation->operation + 1);
		}
		LIST_END_ITERATION;
	}
	
	for (int i = 0; i < g_machines_count; i++)
		list_clear(&machines[i]);

	free(machines);
	fclose(file);
}


static void* view_opening(view_t* view, void* param) {
	g_machines = list_init(NULL);
	g_machines_list = list_init(NULL);
	perform_fast_escalation();

	return NULL;
}



static void render_popup_finished_escalation() {

	if (g_machines_used_count == 0)
		return;

	if (g_finished_machines_thread_count == g_machines_used_count) {
		g_is_finding_machines = false;
		gui_close_popup();
		gui_open_popup(POPUP_FINISHED_MULTITHREAD_ESCALATION);
		g_finished_machines_thread_count = 0;
	}

	if (g_is_finding_machines) {
		gui_open_popup(POPUP_SEARCHING_MULTITHREAD_ESCALATION);
	}

	if (gui_begin_popup(POPUP_FINISHED_MULTITHREAD_ESCALATION)) {

		if (gui_draw_button_fill("Ok"))
			gui_close_popup();

		gui_end_popup();
	}
	else if (gui_begin_popup(POPUP_SEARCHING_MULTITHREAD_ESCALATION)) {

		int max = 1;

		for (int i = 1; i <= g_machines_used_count; i++)
			max *= i;

		gui_draw_text("Machine sequences checked: %d/%d", g_current_machines_found, max);
		gui_draw_text("Progress: %.1f%%", (g_current_machines_found / ((float)max)) * 100);

		if (gui_draw_button_fill("Cancel")) {
			g_is_finding_machines = false;
			gui_close_popup();
		}

		gui_end_popup();
	}
	else if (gui_begin_popup(POPUP_MULTITHREAD_ESCALATION_NOT_AVAILABLE)) {
		if (gui_draw_button_fill("Ok"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void view_render(view_t* view, void* param, void* data) {
	list_t* operations = (list_t*)data;
	uint32_t total = 0;
	float cursor_x, cursor_y;
	float window_x, window_y;

	if (gui_draw_button(ICON_FA_REDO)) {
		g_prev_machines_count = 0;
		perform_fast_escalation();
	}
	
	gui_sameline();
	if (gui_draw_button("Find best machines sequence"))
		find_best_machine_sequence();

	gui_sameline();
	if (gui_draw_button("Export"))
		gui_open_save_file_dialog("DIALOG_EXPORT_EXCALATION", ICON_FA_SAVE " Export to a YAML file", ".yaml");

	gui_sameline();
	gui_draw_text("Total duration: %d\t Total operations: %d\n\n", g_ending_time, g_operations_count);


	int block_height = 54;
	int start_y = 60;
	int margin_x = 120;
	float width = gui_get_window_width() - margin_x - 5;
	char temp[32];

	for (int i = 0; i < g_machines_used_count; i++) {
			gui_set_cursor_pos(5, start_y + block_height * i + 10);
			gui_draw_text("Machine %d", g_machines_ordering[i] + 1);
			gui_set_cursor_pos(75, start_y + block_height * i);

			if (i != 0) {
				sprintf(temp, ICON_FA_ARROW_UP "##0%d", i);
				if (gui_draw_button(temp))
				{
					int previous = g_machines_ordering[i - 1];
					g_machines_ordering[i - 1] = g_machines_ordering[i];
					g_machines_ordering[i] = previous;
					perform_fast_escalation();
				}
			}

			gui_set_cursor_pos(75, start_y + block_height * i + 25);

			if (i < g_machines_used_count - 1) {
				sprintf(temp, ICON_FA_ARROW_DOWN "##1%d", i);
				if (gui_draw_button(temp))
				{
					int next = g_machines_ordering[i + 1];
					g_machines_ordering[i + 1] = g_machines_ordering[i];
					g_machines_ordering[i] = next;
					perform_fast_escalation();
				}
			}
		}

	gui_get_window_pos(&window_x, &window_y);
	gui_get_cursor_pos(&cursor_x, &cursor_y);

	cursor_x -= window_x;
	cursor_y -= window_y;

	operation_info_t* focused_operation = NULL;

	int k = 0;

	LIST_START_ITERATION((&g_machines_list), operation_info_t, operation) {
		float x = operation->end - operation->duration;
		gui_set_cursor_pos(0, 0);

		int machine_y = 0;

		for (int j = 0; j < g_machines_used_count; j++)
			if (g_machines_ordering[j] == operation->machine) {
				machine_y = j;
				break;
			}

		float min_x = x * width / g_ending_time + margin_x;
		float min_y = start_y + block_height * machine_y;
		float max_x = margin_x + (x + operation->duration) * width / g_ending_time;
		float max_y = start_y + block_height * (machine_y + 1);

		if (((min_x > cursor_x && max_x < cursor_x) || (min_x < cursor_x && max_x > cursor_x)) &&
			((min_y > cursor_y && max_y < cursor_y) || (min_y < cursor_y && max_y > cursor_y))) {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / g_jobs_count, 54, 50, 255);
					focused_operation = operation;
		}
		else {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / g_jobs_count, 54, 34, 255);
		}

		gui_set_cursor_pos(x * width / g_ending_time + margin_x + 5, start_y + (block_height * machine_y) + 1);
		gui_draw_text("Job %d \nOp %d", operation->job + 1, operation->operation + 1);

		k++;
	}
	LIST_END_ITERATION;
	//printf("%d ", k);

	for (int i = 0; i < g_ending_time; i += 2) {
		gui_set_cursor_pos(0, 0);
		gui_draw_line(margin_x + i * width / g_ending_time, start_y, margin_x + i * width / g_ending_time, start_y + block_height * g_machines_used_count, 1.0f, 1, 1, 1, 0.1f);
	}

	if (focused_operation != NULL) {
		int x = cursor_x;

		if (gui_get_window_width() - cursor_x < 120)
			x -= 120;

		gui_set_cursor_pos(x, cursor_y + 40);
		gui_draw_rect_rgb(15, -65, 120, 5, 0, 0, 0, 0.6f);
		gui_set_cursor_pos(x + 20, cursor_y - 23);
		gui_draw_text("Job: %d\nOperation: %d\nStart time: %d\nDuration: %d", focused_operation->job + 1, focused_operation->operation + 1, focused_operation->end - focused_operation->duration, focused_operation->duration);
	}

	render_popup_finished_escalation();

	char file_path[256];
	if (gui_render_file_dialog("DIALOG_EXPORT_EXCALATION", file_path))
		export_machines(file_path);
}


static void view_closing(view_t* view, void* param, void* data) {
	list_clear(&g_machines_list);
}


void view_register_escalation() {
	gui_register_view(VIEW_TITLE_ESCALATION, view_opening, view_render, view_closing);
}