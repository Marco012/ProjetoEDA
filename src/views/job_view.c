#include <views.h>

void view_render_jobs(view_t* view, void* data) {
	list_t* jobs = jobs_get_all();
	int i = 1;

	gui_draw_button_fill(ICON_FA_PLUS);
	gui_draw_spacing();
	gui_draw_spacing();

	LIST_START_ITERATION(jobs, job_t, job) {
		char name[32];
		sprintf(name, "%d", i);
		gui_draw_button_fill(name);
		i++;
	}
	LIST_END_ITERATION;
}