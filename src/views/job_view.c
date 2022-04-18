#include <views.h>


#define POPUP_TITLE_DELETE_OPERATON "Delete Operaton"
#define POPUP_TITLE_DELETE_EXECUTION "Delete Execution"
#define POPUP_TITLE_ADD_EXECUTION "Add Execution"
#define POPUP_TITLE_EDIT_EXECUTION "Edit Execution"
#define POPUP_TITLE_EXECUTION_MACHINE_REPEATED "Repeated Machine"
#define STRING_OPERATION_TITLE(str, index) sprintf(str, "Operation %d", index + 1)


operation_t* selected_operation;
int selected_index; // Used to store the id of operation and execution when deleting or editing.

machine_execution_t editing_execution;
bool open_popup_machine_repeated = false; // Needed because ImGui cannot show popups in popups ):


void view_open_job(job_t* job) {
	gui_open_view(VIEW_TITLE_JOB, job);
}


static void view_opening(view_t* view, void* param) {
	job_t* job = (job_t*)param;
}


static void render_popup_delete_operation(job_t* job) {
	if (gui_begin_popup(POPUP_TITLE_DELETE_OPERATON)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Yes"))
		{
			job_remove_operation(job, selected_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("No"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void render_popup_delete_execution() {
	if (gui_begin_popup(POPUP_TITLE_DELETE_EXECUTION)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Yes"))
		{
			operation_remove_execution(selected_operation, selected_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("No"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void render_popup_add_or_edit_execution() {
	if (gui_begin_popup(POPUP_TITLE_ADD_EXECUTION) || (gui_begin_popup(POPUP_TITLE_EDIT_EXECUTION))) {
		
		gui_draw_text("%-20s", "Machine:");
		gui_sameline();
		gui_draw_input_uint16("##Machine", &(editing_execution.machine));
		gui_draw_text("%-20s", "Duration:");
		gui_sameline();
		gui_draw_input_uint16("##Duration", &(editing_execution.duration));

		gui_columns(2);
		if (gui_draw_button_fill("Save"))
		{
			gui_close_popup();

			if (selected_index == -1) {
				if (!operation_add_execution(selected_operation, editing_execution))
				{
					gui_close_popup();
					open_popup_machine_repeated = true;
				}
			}
			else {
				if (!operation_set_execution(selected_operation, selected_index, editing_execution))
				{
					gui_close_popup();
					open_popup_machine_repeated = true;
				}
			}

		}

		gui_next_column();
		if (gui_draw_button_fill("Cancel"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void render_popup_execution_machine_repeated() {
	if (!gui_begin_popup(POPUP_TITLE_EXECUTION_MACHINE_REPEATED))
		return;

	gui_draw_text("This operation already contains an execution on this machine.");

	if (gui_draw_button_fill("Ok"))
		gui_close_popup();

	gui_end_popup();
}


static void view_render(view_t* view, void* param, void* data) {
	job_t* job = (job_t*)param;
	const char temp[32];
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t operation_total_duration = 0;

	if (gui_draw_button_fill("Save to file 'out.csv'"))
		job_save_file(job, "out.csv");

	// Draws and handles the Add Operation into job button.
	gui_draw_title("Operations:");
	if (gui_draw_button_fill(ICON_FA_PLUS)) {
		job_new_operation(job);
	}

	gui_draw_spacing();
	gui_draw_spacing();

	LIST_START_ITERATION((&job->operations), operation_t, operation) {

		// Draws and handles the Delete Operation button.
		sprintf(temp, ICON_FA_TRASH "##%d", i);
		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_DELETE_OPERATON);
			selected_index = i;
		}
		gui_sameline();

		STRING_OPERATION_TITLE(temp, i);

		// Draws the collapsing header of the operation.
		MARGIN_X_3();
		if (gui_draw_collapsing_header(temp)) {

			MARGIN_X_4();

			// Draws the button to add a new execution into the operation.
			sprintf(temp, ICON_FA_PLUS "##%d", i);
			if (gui_draw_button_fill(temp)) {
				editing_execution.duration = 0;
				editing_execution.machine = 0;
				selected_operation = operation;
				selected_index = -1;
				gui_open_popup(POPUP_TITLE_ADD_EXECUTION);
			}

			gui_draw_spacing();
			gui_draw_spacing();

			j = 0;
			operation_total_duration = 0;
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				sprintf(temp, "Execution %d##%d%d", j + 1, i, execution->machine);

				// Draws the collapsing header of the execution in the operation.
				MARGIN_X_4();
				if (gui_draw_collapsing_header(temp)) {
					MARGIN_X_5();
					gui_draw_text("Machine: %d", execution->machine);
					MARGIN_X_5();
					gui_draw_text("Duration: %ds", execution->duration);
					MARGIN_X_5();

					// Draws the button to edit the execution in the operation.
					sprintf(temp, ICON_FA_EDIT "##%d%d", i, j);
					if (gui_draw_button(temp)) {
						editing_execution.machine = execution->machine;
						editing_execution.duration = execution->duration;
						selected_index = j;
						selected_operation = operation;
						gui_open_popup(POPUP_TITLE_EDIT_EXECUTION);
					}
					gui_sameline();

					// Draws the button to remove the execution from the operation.
					sprintf(temp, ICON_FA_TRASH "##%d%d", i, j);
					if (gui_draw_button(temp)) {
						selected_index = j;
						selected_operation = operation;
						gui_open_popup(POPUP_TITLE_DELETE_EXECUTION);
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

	// This is used to show a popup when an edited or added execution already exists with the same machine.
	if (open_popup_machine_repeated) {
		gui_open_popup(POPUP_TITLE_EXECUTION_MACHINE_REPEATED);
		open_popup_machine_repeated = false;
	}

	render_popup_delete_operation(job);
	render_popup_delete_execution();
	render_popup_add_or_edit_execution();
	render_popup_execution_machine_repeated();
}


void view_register_job() {
	gui_register_view(VIEW_TITLE_JOB, view_opening, view_render, NULL);
}