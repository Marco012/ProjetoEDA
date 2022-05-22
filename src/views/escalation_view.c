#include <views.h>
#include <windows.h>


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


int ending;
int jobs_count = 0;
int machines_count = 0;
int prev_machines_count = 0;
int operations_count = 0;
list_t machines_list;
int* machines_ordering = NULL;


static int perform_escalation(list_t *machines_list, int* machines_ordering) {
	list_t* jobs = jobs_get_all();

	list_clear(machines_list);

	list_t temp_jobs = list_init(NULL);
	int job_id = 0;

	LIST_START_ITERATION(jobs, job_t, job) {
		int operation_id = 0;
		job_id_t temp_job = { .job = job_init(), .id = job_id };
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

	operation_info_t* machines = malloc(sizeof(operation_info_t) * machines_count);
	operation_info_t empty = {
		.duration = -1, .job = -1, .machine = -1, .operation = -1, .end = -1
	};

	for (int i = 0; i < machines_count; i++)
		machines[i] = empty;

	int cur_duration = 0;
	bool has_machines_working = true;

	while (has_machines_working) {

		for (int i = 0; i < machines_count; i++) {

			if (machines[i].end != -1)
			{
				if (machines[i].end == cur_duration)
					machines[i] = empty;
				else
					continue;
			}
		}

		for (int j = 0; j < machines_count; j++) {
			int i = machines_ordering[j];

			if (machines[i].end != -1)
				continue;

			operation_info_t fastest_operation = empty;

			int job_id = 0;

			LIST_START_ITERATION((&temp_jobs), job_id_t, job) {
				bool skip_job = false;

				// Check if the job has any operation being processed by any machine.
				for (int j = 0; j < machines_count; j++) {
					if (machines[j].job == job->id) {
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

						if (fastest_operation.end == -1 || fastest_operation.duration >= execution->duration)
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
				machines[i] = fastest_operation;
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
		for (int i = 0; i < machines_count; i++) {
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
	machines_count = 0;
	operations_count = 0;
	jobs_count = 0;

	LIST_START_ITERATION(jobs, job_t, job) {
		LIST_START_ITERATION((&job->operations), operation_t, operation) {
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				if (execution->machine + 1 > machines_count)
					machines_count = execution->machine + 1;
			}
			LIST_END_ITERATION;
			operations_count++;
		}
		LIST_END_ITERATION;
		jobs_count++;
	}
	LIST_END_ITERATION;


	if (prev_machines_count != machines_count) {
		if (machines_ordering != NULL)
			free(machines_ordering);

		machines_ordering = malloc(sizeof(int) * machines_count);

		for (int i = 0; i < machines_count; i++)
			machines_ordering[i] = i;
	}

	prev_machines_count = machines_count;

	ending = perform_escalation(&machines_list, machines_ordering);
}


static void recursive_find_best_machine(int sequence, int i, int* ending, list_t* machines_list, int* ordering) {

	ordering[sequence] = i;

	if (sequence == machines_count - 1) {
		int local_ending = 0;
		list_t local_machines = list_init(NULL);
		int* local_ordering = malloc(sizeof(int) * machines_count);

		memcpy(local_ordering, ordering, sizeof(int) * machines_count);

		local_ending = perform_escalation(&local_machines, local_ordering);

		if (local_ending < *ending || *ending == 0) {
			*ending = local_ending;

			for (int j = 0; j < machines_count; j++)
				ordering[j] = local_ordering[j];

			LIST_START_ITERATION((&local_machines), operation_info_t, operation) {
				list_push(machines_list, operation, sizeof(operation_info_t));
			}
			LIST_END_ITERATION;
		}

		list_clear(&local_machines);
		free(local_ordering);

		return;
	}

	for (int j = 0; j < machines_count; j++) {

		bool contains_machine = false;

		for (int k = 0; k <= sequence; k++)
			if (ordering[k] == j)
				contains_machine = true;

		if (!contains_machine)
			recursive_find_best_machine(sequence + 1, j, ending, machines_list, ordering);
	}
}


typedef struct {
	int i;
	int ending;
	list_t machines_list;
	int* ordering;
} machine_sequence_data_t;


DWORD WINAPI myThread(LPVOID lpParameter)
{
	machine_sequence_data_t* data = (DWORD*)lpParameter;

	int ending_time = 0;
	list_t machines = list_init(NULL);
	int* ordering = malloc(sizeof(int) * machines_count);

	recursive_find_best_machine(0, data->i, &ending_time, &machines, ordering);

	free(data);

	//recursive_find_best_machine(0, data->i, &data->ending, &data->machines_list, data->ordering);
	printf("finished %d\n", ending_time);
}


static void find_best_machine_sequence() {
	for (int i = 0; i < machines_count; i++)
	{
		int ending_time = 0;
		list_t machines = list_init(NULL);
		int* ordering = malloc(sizeof(int) * machines_count);

		DWORD ThreadId;
		HANDLE ThreadHandle;

		machine_sequence_data_t* data = malloc(sizeof(machine_sequence_data_t));
		data->i = i;
		data->ending = 0;
		data->machines_list = list_init(NULL);
		data->ordering = ordering;

		ThreadHandle = CreateThread(NULL, 0, myThread, data, 0, &ThreadId);
		/* returns the thread identifier */
		//if (ThreadHandle != NULL) {
			/* now wait for the thread to finish */ //WaitForSingleObject(ThreadHandle, INFINITE);
			/* close the thread handle */
			//CloseHandle(ThreadHandle);
			//printf("sum = %d\n", Sum);
		//}

		//recursive_find_best_machine(0, i, &ending_time, &machines, ordering);

		/*
		if (ending_time < ending) {
			ending = ending_time;

			memcpy(machines_ordering, ordering, sizeof(int) * machines_count);

			LIST_START_ITERATION((&machines), operation_info_t, operation) {
				list_push(&machines_list, operation, sizeof(operation_info_t));
			}
			LIST_END_ITERATION;
		}
		*/

		//list_clear(&machines);
		//free(ordering);
	}
}


static void* view_opening(view_t* view, void* param) {
	machines_list = list_init(NULL);
	perform_fast_escalation();

	find_best_machine_sequence();

	return NULL;
}


static void view_render(view_t* view, void* param, void* data) {
	list_t* operations = (list_t*)data;
	uint32_t total = 0;
	float cursor_x, cursor_y;
	float window_x, window_y;

	if (gui_draw_button(ICON_FA_REDO)) {
		prev_machines_count = 0;
		perform_fast_escalation();
	}

	gui_sameline();
	gui_draw_text("Total duration: %d\t Total operations: %d\n\n", ending, operations_count);


	int block_height = 54;
	int start_y = 60;
	int margin_x = 120;
	float width = gui_get_window_width() - margin_x - 5;
	char temp[32];

	for (int i = 0; i < machines_count; i++) {
		gui_set_cursor_pos(5, start_y + block_height * i + 10);
		gui_draw_text("Machine %d", machines_ordering[i] + 1);
		gui_set_cursor_pos(75, start_y + block_height * i);

		if (i != 0) {
			sprintf(temp, ICON_FA_ARROW_UP "##0%d", i);
			if (gui_draw_button(temp))
			{
				int previous = machines_ordering[i - 1];
				machines_ordering[i - 1] = machines_ordering[i];
				machines_ordering[i] = previous;
				perform_fast_escalation();
			}
		}

		gui_set_cursor_pos(75, start_y + block_height * i + 25);

		if (i < machines_count - 1) {
			sprintf(temp, ICON_FA_ARROW_DOWN "##1%d", i);
			if (gui_draw_button(temp))
			{
				int next = machines_ordering[i + 1];
				machines_ordering[i + 1] = machines_ordering[i];
				machines_ordering[i] = next;
				perform_fast_escalation();
			}
		}
	}

	gui_get_window_pos(&window_x, &window_y);
	gui_get_cursor_pos(&cursor_x, &cursor_y);

	cursor_x -= window_x;
	cursor_y -= window_y;

	operation_info_t* focused_operation = NULL;

	LIST_START_ITERATION((&machines_list), operation_info_t, operation) {
		float x = operation->end - operation->duration;
		gui_set_cursor_pos(0, 0);

		int machine_y = 0;

		for (int j = 0; j < machines_count; j++)
			if (machines_ordering[j] == operation->machine) {
				machine_y = j;
				break;
			}

		float min_x = x * width / ending + margin_x;
		float min_y = start_y + block_height * machine_y;
		float max_x = margin_x + (x + operation->duration) * width / ending;
		float max_y = start_y + block_height * (machine_y + 1);

		if (((min_x > cursor_x && max_x < cursor_x) || (min_x < cursor_x && max_x > cursor_x)) &&
			((min_y > cursor_y && max_y < cursor_y) || (min_y < cursor_y && max_y > cursor_y))) {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / jobs_count, 54, 50, 255);
					focused_operation = operation;
		}
		else {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / jobs_count, 54, 34, 255);
		}

		gui_set_cursor_pos(x * width / ending + margin_x + 5, start_y + (block_height * machine_y) + 1);
		gui_draw_text("Job %d \nOp %d", operation->job + 1, operation->operation + 1);
	}
	LIST_END_ITERATION;

	for (int i = 0; i < ending; i += 2) {
		gui_set_cursor_pos(0, 0);
		gui_draw_line(margin_x + i * width / ending, start_y, margin_x + i * width / ending, start_y + block_height * machines_count, 1.0f, 1, 1, 1, 0.1f);
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
}


static void view_closing(view_t* view, void* param, void* data) {
	//list_t* operations = (list_t*)data;

	//list_clear(operations);
	//free(operations);

	list_clear(&machines_list);
}


void view_register_escalation() {
	gui_register_view(VIEW_TITLE_ESCALATION, view_opening, view_render, view_closing);
}