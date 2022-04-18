#include <utils.h>
#include <job.h>
#include <job_utils.h>
#include <list.h>
#include <tests.h>
#include <views.h>

#include <stdio.h>
#include <inttypes.h>
#include <gui/gui.h>


#define MENU_WIDTH 200


void ui_draw(window_t* window) {

	// Draws the main menu.
	gui_start_menu();
	{
		gui_draw_title_centered("Menu");
		gui_draw_spacing();

		// Draws and handles the jobs button.
		if (gui_draw_button_fill(VIEW_TITLE_JOBS))
			view_open_jobs();

		// Draws and handles the min time button.
		if (gui_draw_button_fill(VIEW_TITLE_MIN_TIME)) {
			job_t* job = list_get(jobs_get_all(), 0);
			gui_open_view(VIEW_TITLE_MIN_TIME, job);
		}

		// Draws and handles the max time button.
		if (gui_draw_button_fill(VIEW_TITLE_MAX_TIME)) {
			job_t* job = list_get(jobs_get_all(), 0);
			gui_open_view(VIEW_TITLE_MAX_TIME, job);
		}

		gui_draw_spacing();
		gui_draw_spacing();

		if (gui_draw_button_fill("Quit"))
			window_close(window);

		gui_draw_spacing();
	}

	gui_end_menu(MENU_WIDTH);
}


void open_main_window() {
	window_t* window;

	if (!window_init()) {
		printf("Error initiating rendering context. Aborting.");
		return -1;
	}
	
	window = window_create();

	if (window == NULL) {
		printf("Error opening window. Aborting.");
		return -1;
	}

	while (window_render(window, ui_draw));

	window_destroy(window);
}


int main(void) {

	// Register all the views being used.
	view_register_jobs();
	view_register_job();
	view_register_max_time();
	view_register_min_time();

	job_t job = job_init();
	job_load_file(&job, "job.csv");
	jobs_insert(&job);

	open_main_window();

	job_clear(&job);

	return 0;
}
