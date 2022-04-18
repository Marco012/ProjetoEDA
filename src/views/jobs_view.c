#include <views.h>


void view_open_jobs() {
	gui_open_view(VIEW_TITLE_JOBS, NULL);
}


static void view_render(view_t* view, void* data) {
	list_t* jobs = jobs_get_all();
	int i = 1;

	//gui_draw_button_fill(ICON_FA_PLUS);
	//gui_draw_spacing();
	//gui_draw_spacing();

	char name[32];

	LIST_START_ITERATION(jobs, job_t, job) {
		sprintf(name, "Job %d", i);

		//gui_draw_button(ICON_FA_TRASH);
		//gui_sameline();
		//MARGIN_X_3();

		if (gui_draw_button_fill(name)) {
			view_open_job(job);
		}

		i++;
	}
	LIST_END_ITERATION;
}


void view_register_jobs() {
	gui_register_view(VIEW_TITLE_JOBS, NULL, view_render, NULL);
}