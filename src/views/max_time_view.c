#include <views.h>


void view_open_max_time(job_t* job) {
	gui_open_view(VIEW_TITLE_MAX_TIME, job);
}


static void* view_opening(view_t* view, void* param) {
	job_t* job = (job_t*)param;
	list_t* operations = malloc(sizeof(list_t));
	uint32_t total = 0;

	get_job_finish_filtered_operations(job, operations, FILTER_MAX);

	return operations;
}


static void view_render(view_t* view, void* param, void* data) {
	list_t* operations = (list_t *) data;
	uint32_t total = 0;

	LIST_START_ITERATION(operations, operation_execution_t, operation) {
		gui_draw_text("Operation %u: machine: %u duration: %u\n", operation->operation_id, operation->execution.machine, operation->execution.duration);
		total += operation->execution.duration;
	}
	LIST_END_ITERATION;

	gui_draw_text("Total duration: %u\n\n", total);
}


static void view_closing(view_t* view, void* param, void* data) {
	list_t* operations = (list_t*)data;

	list_clear(operations);
	free(operations);
}


void view_register_max_time() {
	gui_register_view(VIEW_TITLE_MAX_TIME, view_opening, view_render, view_closing);
}