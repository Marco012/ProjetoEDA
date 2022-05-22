#include <views.h>

#define POPUP_TITLE_DELETE_JOB "Delete Job"


int selected_index;


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
		gui_sameline();

		MARGIN_X_3();

		sprintf(temp, "Job %d", i);

		if (gui_draw_button_fill(temp))
			view_open_job(job, i - 1);

		i++;
	}
	LIST_END_ITERATION;

	render_popup_delete_job();
}


void view_register_jobs() {
	gui_register_view(VIEW_TITLE_JOBS, NULL, view_render, NULL);
}