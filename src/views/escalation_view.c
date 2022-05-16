#include <views.h>


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
int machines_count = 8;

list_t machines_list;


static void* view_opening(view_t* view, void* param) {

	list_t* jobs = jobs_get_all();
	machines_list = list_init(NULL);

	list_t temp_jobs = list_init(NULL);

	jobs_count = 0;
	LIST_START_ITERATION(jobs, job_t, job) {
		int operation_id = 0;
		job_id_t temp_job = { .job = job_init(), .id = jobs_count };
		LIST_START_ITERATION((&job->operations), operation_t, operation) {
			operation_t* temp_operation = job_new_operation(&temp_job.job);
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				operation_info_t operation_info = (operation_info_t){
								.duration = execution->duration,
								.job = jobs_count,
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
		jobs_count++;
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

		for (int i = 0; i < machines_count; i++) {

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
				machines[i] = fastest_operation;
				int job_id = 0;
				LIST_START_ITERATION((&temp_jobs), job_id_t, job) {
					if (job->id == fastest_operation.job)
					{
						list_push(&machines_list, &fastest_operation, sizeof(fastest_operation));
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

	ending = cur_duration;

	free(machines);

	return NULL;
}


static void view_render(view_t* view, void* param, void* data) {
	list_t* operations = (list_t*)data;
	uint32_t total = 0;
	float cursor_x, cursor_y;
	float window_x, window_y;

	gui_draw_text("Total duration: %d\n\n", ending);

	int block_height = 40;
	int start_y = 60;
	int margin_x = 80;
	float width = gui_get_window_width() - 80 - 5;

	for (int i = 0; i < 8; i++) {
		gui_set_cursor_pos(5, start_y + block_height * i + 10);
		gui_draw_text("Machine %d", i + 1);
	}

	gui_get_window_pos(&window_x, &window_y);
	gui_get_cursor_pos(&cursor_x, &cursor_y);

	cursor_x -= window_x;
	cursor_y -= window_y;

	operation_info_t* focused_operation = NULL;

	LIST_START_ITERATION((&machines_list), operation_info_t, operation) {
		float x = operation->end - operation->duration;
		gui_set_cursor_pos(0, 0);

		float min_x = x * width / ending + margin_x;
		float min_y = start_y + block_height * operation->machine;
		float max_x = margin_x + (x + operation->duration) * width / ending;
		float max_y = start_y + block_height * (operation->machine + 1);

		if (((min_x > cursor_x && max_x < cursor_x) || (min_x < cursor_x && max_x > cursor_x)) &&
			((min_y > cursor_y && max_y < cursor_y) || (min_y < cursor_y && max_y > cursor_y))) {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / jobs_count, 54, 50, 255);
					focused_operation = operation;
		}
		else {
			gui_draw_rect_hsv(min_x, min_y, max_x, max_y, operation->job * 360 / jobs_count, 54, 34, 255);
		}

		gui_set_cursor_pos(x * width / ending + margin_x + 5, start_y + (block_height * operation->machine) + 1);
		gui_draw_text("Job %d \nOp %d", operation->job + 1, operation->operation + 1);
	}
	LIST_END_ITERATION;

	if (focused_operation != NULL) {
		gui_set_cursor_pos(cursor_x, cursor_y + 40);
		gui_draw_rect_rgb(15, -35, 120, 5, 0, 0, 0, 0.6f);
		gui_set_cursor_pos(cursor_x + 20, cursor_y + 8);
		gui_draw_text("Start time: %d\nDuration: %d", focused_operation->end - focused_operation->duration, focused_operation->duration);

	}

	for (int i = 0; i < 32; i += 2) {
		gui_set_cursor_pos(0, 0);
		gui_draw_line(margin_x + i * width / ending, start_y, margin_x + i * width / ending, start_y + block_height * machines_count, 1.0f, 1, 1, 1, 0.1f);
	}
}


static void view_closing(view_t* view, void* param, void* data) {
	//list_t* operations = (list_t*)data;

	//list_clear(operations);
	//free(operations);
}


void view_register_escalation() {
	gui_register_view(VIEW_TITLE_ESCALATION, view_opening, view_render, view_closing);
}