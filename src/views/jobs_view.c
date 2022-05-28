#include <views.h>

#define POPUP_TITLE_DELETE_JOB "Delete Job"
#define POPUP_TITLE_EDIT_JOB "Edit Job"
#define STRING_JOB_TITLE(str, index) sprintf(str, "Job %d", index)


int selected_index;
char temp_name[JOB_NAME_LENGTH];
job_t* editing_job;


static void render_popup_edit_job() {
	operation_t new_operation;

	if (gui_begin_popup(POPUP_TITLE_EDIT_JOB)) {
		gui_draw_text("Name:");
		gui_draw_input_string("##NAME", temp_name, OPERATION_NAME_LENGTH);

		gui_columns(3);
		if (gui_draw_button_fill("Ok"))
		{
			sprintf(editing_job->name, "%s", temp_name);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("Cancel"))
			gui_close_popup();
		gui_next_column();
		if (gui_draw_button_fill("Reset")) {
			editing_job->name[0] = 0;
			gui_close_popup();
		}

		gui_end_popup();
	}
}


static void render_popup_delete_job() {
	if (gui_begin_popup(POPUP_TITLE_DELETE_JOB)) {
		gui_draw_text("Are you sure you want to delete it?");

		gui_columns(2);
		if (gui_draw_button_fill("Yes"))
		{
			jobs_remove(selected_index);
			gui_close_popup();
		}
		gui_next_column();
		if (gui_draw_button_fill("No"))
			gui_close_popup();

		gui_end_popup();
	}
}


void view_open_jobs() {
	gui_open_view(VIEW_TITLE_JOBS, NULL);
}


static void view_render(view_t* view, void* data, void* param) {
	list_t* jobs = jobs_get_all();
	int i = 1;

	if (gui_draw_button_fill(ICON_FA_PLUS)) {
		job_t new_job = job_init();
		jobs_insert(&new_job);
	}

	char temp[32];

	LIST_START_ITERATION(jobs, job_t, job) {
		
		// Draws and handles the Delete Job button.
		sprintf(temp, ICON_FA_TRASH "##%d", i);
		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_DELETE_JOB);
			selected_index = i - 1;
		}


		MARGIN_X_3();
		gui_sameline();
		sprintf(temp, ICON_FA_EDIT "##%d", i);
		if (gui_draw_button(temp)) {
			gui_open_popup(POPUP_TITLE_EDIT_JOB);
			selected_index = i - 1;
			editing_job = job;
			sprintf(temp_name, "%s", job->name);
		}

		gui_sameline();
		MARGIN_X(105);


		if (strlen(job->name) == 0)
			STRING_JOB_TITLE(temp, i);
		else
			sprintf(temp, "%s", job->name);

		if (gui_draw_button_fill(temp))
			view_open_job(job, i - 1);

		i++;
	}
	LIST_END_ITERATION;

	render_popup_delete_job();
	render_popup_edit_job();
}


void view_register_jobs() {
	gui_register_view(VIEW_TITLE_JOBS, NULL, view_render, NULL);
}