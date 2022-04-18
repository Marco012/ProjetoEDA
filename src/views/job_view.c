#include <views.h>


#define POPUP_TITLE_DELETE_OPERATON "Delete Operaton"
#define POPUP_TITLE_DELETE_EXECUTION "Delete Execution"
#define STRING_OPERATION_TITLE(str, index) sprintf(str, "Operation %d", index + 1)


operation_t* selected_operation;
int delete_index;


void view_open_job(job_t* job) {
	gui_open_view(VIEW_TITLE_JOB, job);
}


static void view_opening(view_t* view, void* param) {
	job_t* job = (job_t*)param;
}


static void render_popup_delete_operation(job_t* job) {
	const char temp[32];

	if (gui_begin_popup(POPUP_TITLE_DELETE_OPERATON)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Yes"))
		{
			STRING_OPERATION_TITLE(temp, delete_index);
			job_remove_operation(job, delete_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("No"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void render_popup_delete_execution() {
	const char temp[32];

	if (gui_begin_popup(POPUP_TITLE_DELETE_EXECUTION)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Yes"))
		{
			STRING_OPERATION_TITLE(temp, delete_index);
			operation_remove_execution(selected_operation, delete_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("No"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void view_render(view_t* view, void* param, void* data) {
	job_t* job = (job_t*)param;
	const char temp[32];
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t operation_total_duration = 0;

	gui_draw_title("Operations:");

	if (gui_draw_button_fill(ICON_FA_PLUS)) {
		job_new_operation(job);
	}

	gui_draw_spacing();
	gui_draw_spacing();

	LIST_START_ITERATION((&job->operations), operation_t, operation) {

		sprintf(temp, ICON_FA_TRASH "##%d", i);

		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_DELETE_OPERATON);
			delete_index = i;
		}
		gui_sameline();

		STRING_OPERATION_TITLE(temp, i);

		MARGIN_X_3();
		if (gui_draw_collapsing_header(temp)) {
			j = 0;
			operation_total_duration = 0;
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				sprintf(temp, "Execution %d##%d%d", j + 1, i, execution->machine);

				MARGIN_X_4();
				if (gui_draw_collapsing_header(temp)) {
					MARGIN_X_5();
					gui_draw_text("Machine: %d", execution->machine);
					MARGIN_X_5();
					gui_draw_text("Duration: %ds", execution->duration);
					MARGIN_X_5();
					gui_draw_button(ICON_FA_EDIT);
					gui_sameline();

					sprintf(temp, ICON_FA_TRASH "##%d%d", i, j);

					if (gui_draw_button(temp)) {
						gui_open_popup(POPUP_TITLE_DELETE_EXECUTION);
						delete_index = j;
						selected_operation = operation;
					}

					gui_draw_spacing();
				}

				operation_total_duration += execution->duration;
				j++;
			}
			LIST_END_ITERATION;

			// If the operation has executions, shows the average duration.
			if (j > 0) {
				MARGIN_X_3();
				gui_draw_text("Average duration of %u possibilities: %u\n\n", j, operation_total_duration / j);
			}
		}

		i++;
	}
	LIST_END_ITERATION;

	render_popup_delete_operation(job);
	render_popup_delete_execution();
}


void view_register_job() {
	gui_register_view(VIEW_TITLE_JOB, view_opening, view_render, NULL);
}