#include <views.h>


#define POPUP_TITLE_DELETE_OPERATON "Delete Operaton"
#define POPUP_TITLE_EDIT_OPERATON "Edit Operaton"
#define POPUP_TITLE_DELETE_EXECUTION "Delete Execution"
#define POPUP_TITLE_ADD_EXECUTION "Add Execution"
#define POPUP_TITLE_EDIT_EXECUTION "Edit Execution"
#define POPUP_TITLE_EXECUTION_MACHINE_REPEATED "Repeated Machine"
#define POPUP_TITLE_EXECUTION_MACHINE_INVALID "Invalid Machine"
#define STRING_OPERATION_TITLE(str, index) sprintf(str, "Operation %d", index + 1)


typedef enum {
	EDIT_ERROR_NONE, EDIT_ERROR_REPEATED, EDIT_ERROR_INVALID
} operation_edit_error_t;


typedef struct {
	job_t* job;
	int id;
} job_info_t;


static int g_operations_count = 0;

operation_t* selected_operation;
int selected_index; // Used to store the id of operation and execution when deleting or editing.

machine_execution_t editing_execution;
operation_edit_error_t operation_edit_error = EDIT_ERROR_NONE; // Needed because I was having trouble showing popups in popups ):



void view_open_job(job_t* job, int id) {
	job_info_t* job_info = malloc(sizeof(job_info_t));
	job_info->job = job;
	job_info->id = id;

	gui_open_view(VIEW_TITLE_JOB, job_info);
}


static void view_opening(view_t* view, void* param) { }


static void view_closing(view_t* view, void* param, void* data) {
	free(param);
}



static void clone_operation(operation_t* dst, operation_t* src) {
	memcpy(dst->name, src->name, OPERATION_NAME_LENGTH);
	dst->executions = list_init(NULL);

	LIST_START_ITERATION((&src->executions), machine_execution_t, execution) {
		operation_add_execution(dst, *execution);
	}
	LIST_END_ITERATION;
}


char temp_name[OPERATION_NAME_LENGTH];

static void render_popup_edit_operation(job_t* job) {
	operation_t new_operation;

	if (gui_begin_popup(POPUP_TITLE_EDIT_OPERATON)) {
		gui_draw_text("Name:");
		gui_draw_input_string("##NAME", temp_name, OPERATION_NAME_LENGTH);

		if (selected_index > 0) {
			if (gui_draw_button(ICON_FA_ARROW_UP)) {
				// Clone the current operation and inserts into the job.
				clone_operation(&new_operation, selected_operation);
				job_insert_operation(job, selected_index - 1, &new_operation);
				job_remove_operation(job, selected_index + 1);
				gui_close_popup();
			}
		}

		if (selected_index < g_operations_count - 1) {
			if (gui_draw_button(ICON_FA_ARROW_DOWN)) {
				// Clone the current operation and inserts into the job.
				clone_operation(&new_operation, selected_operation);
				job_insert_operation(job, selected_index + 2, &new_operation);
				job_remove_operation(job, selected_index);
				gui_close_popup();
			}
		}

		gui_columns(3);
		if (gui_draw_button_fill("Ok"))
		{
			sprintf(selected_operation->name, "%s", temp_name);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("Cancel"))
			gui_close_popup();
		gui_next_column();
		if (gui_draw_button_fill("Reset")) {
			selected_operation->name[0] = 0;
			gui_close_popup();
		}

		gui_end_popup();
	}
}



static void render_popup_delete_operation(job_t* job) {
	if (gui_begin_popup(POPUP_TITLE_DELETE_OPERATON)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Ok"))
		{
			job_remove_operation(job, selected_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("Cacel"))
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

			if (editing_execution.machine < 1)
			{
				gui_close_popup();
				operation_edit_error = EDIT_ERROR_INVALID;
				gui_end_popup();
				return;
			}

			if (selected_index == -1) {
				editing_execution.machine--;
				if (!operation_add_execution(selected_operation, editing_execution))
				{
					gui_close_popup();
					operation_edit_error = EDIT_ERROR_REPEATED;
				}
				editing_execution.machine++;
			}
			else {
				editing_execution.machine--;
				if (!operation_set_execution(selected_operation, selected_index, editing_execution))
				{
					gui_close_popup();
					operation_edit_error = EDIT_ERROR_REPEATED;
				}
				editing_execution.machine++;
			}

		}

		gui_next_column();
		if (gui_draw_button_fill("Cancel"))
			gui_close_popup();

		gui_end_popup();
	}
}


static void render_popup_execution_machine_error() {
	if (gui_begin_popup(POPUP_TITLE_EXECUTION_MACHINE_REPEATED))
		gui_draw_text("This operation already contains an execution on this machine.");
	else if (gui_begin_popup(POPUP_TITLE_EXECUTION_MACHINE_INVALID))
		gui_draw_text("Machine with an invalid id. It must be greater than 0.");
	else
		return;

	if (gui_draw_button_fill("Ok"))
		gui_close_popup();

	gui_end_popup();
}


static void view_render(view_t* view, void* param, void* data) {
	job_info_t* job_info = (job_info_t*)param;
	job_t* job = job_info->job;
	int job_id = job_info->id;
	const char temp[32];
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t operation_total_duration = 0;

	sprintf(temp, "Job %d", job_id + 1);

	// Draws and handles the Add Operation into job button.
	gui_draw_title(temp);

	gui_draw_spacing();

	// Draws and handles the min time button.
	if (gui_draw_button_fill(VIEW_TITLE_MIN_TIME)) {
		gui_open_view(VIEW_TITLE_MIN_TIME, job);
	}

	// Draws and handles the max time button.
	if (gui_draw_button_fill(VIEW_TITLE_MAX_TIME)) {
		gui_open_view(VIEW_TITLE_MAX_TIME, job);
	}

	gui_draw_spacing();
	gui_draw_spacing();

	if (gui_draw_button_fill(ICON_FA_PLUS)) {
		job_new_operation(job, NULL);
	}

	gui_draw_spacing();

	g_operations_count = 0;
	LIST_START_ITERATION((&job->operations), operation_t, operation) {

		// Draws and handles the Delete Operation button.
		sprintf(temp, ICON_FA_TRASH "##%d", i);
		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_DELETE_OPERATON);
			selected_index = i;
		}
		gui_sameline();

		MARGIN_X_3();
		gui_sameline();
		sprintf(temp, ICON_FA_EDIT "##%d", i);
		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_EDIT_OPERATON);
			selected_index = i;
			selected_operation = operation;
			sprintf(temp_name, "%s", operation->name);
		}

		if (strlen(operation->name) == 0)
			STRING_OPERATION_TITLE(temp, i);
		else
			sprintf(temp, "%s##%d", operation->name, i);

		gui_sameline();
		// Draws the collapsing header of the operation.
		MARGIN_X(110);
		if (gui_draw_collapsing_header(temp)) {

			MARGIN_X_5();

			// Draws the button to add a new execution into the operation.
			sprintf(temp, ICON_FA_PLUS "##%d", i);
			if (gui_draw_button_fill(temp)) {
				editing_execution.duration = 1;
				editing_execution.machine = 1;
				selected_operation = operation;
				selected_index = -1;
				gui_open_popup(POPUP_TITLE_ADD_EXECUTION);
			}

			gui_draw_spacing();
			gui_draw_spacing();

			j = 0;
			operation_total_duration = 0;
			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				sprintf(temp, "Execution %d##%d%d", j + 1, i, execution->machine + 1);

				// Draws the collapsing header of the execution in the operation.
				MARGIN_X_5();
				if (gui_draw_collapsing_header(temp)) {
					MARGIN_X_6();
					gui_draw_text("Machine: %d", execution->machine + 1);
					MARGIN_X_6();
					gui_draw_text("Duration: %ds", execution->duration);
					MARGIN_X_6();

					// Draws the button to edit the execution in the operation.
					sprintf(temp, ICON_FA_EDIT "##%d%d", i, j);
					if (gui_draw_button(temp)) {
						editing_execution.machine = execution->machine + 1;
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
		g_operations_count++;
	}
	LIST_END_ITERATION;

	// This is used to show a popup when an edited or added execution already exists with the same machine.
	switch (operation_edit_error) {
	case EDIT_ERROR_REPEATED:
		gui_open_popup(POPUP_TITLE_EXECUTION_MACHINE_REPEATED);
		break;
	case EDIT_ERROR_INVALID:
		gui_open_popup(POPUP_TITLE_EXECUTION_MACHINE_INVALID);
		break;
	}

	operation_edit_error = EDIT_ERROR_NONE;

	render_popup_delete_operation(job);
	render_popup_edit_operation(job);
	render_popup_delete_execution();
	render_popup_add_or_edit_execution();
	render_popup_execution_machine_error();
}


void view_register_job() {
	gui_register_view(VIEW_TITLE_JOB, view_opening, view_render, view_closing);
}