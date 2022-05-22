#include <utils.h>
#include <job.h>
#include <job_utils.h>
#include <list.h>
#include <tests.h>
#include <views.h>
#include <job_loader.h>

#include <stdio.h>
#include <inttypes.h>
#include <gui/gui.h>


#define MENU_WIDTH 200


void ui_draw(window_t* window) {

	char file_path[256];

	// Draws the main menu.
	gui_start_menu();
	{
		gui_draw_title_centered("Menu");
		gui_draw_spacing();

		// Draws and handles the jobs button.
		if (gui_draw_button_fill(VIEW_TITLE_JOBS))
			view_open_jobs();

		if (gui_draw_button_fill("Import")) {
			gui_open_load_file_dialog("DIALOG_IMPORT", ICON_FA_FOLDER_OPEN " Load from a json file", ".json");
		}

		if (gui_draw_button_fill("Export")) {
			gui_open_save_file_dialog("DIALOG_EXPORT", ICON_FA_SAVE " Export to a json file", ".json");
		}

		// Draws and handles the escalation button.
		if (gui_draw_button_fill(VIEW_TITLE_ESCALATION)) {
			view_open_escalation();
		}

		gui_draw_spacing();
		gui_draw_spacing();

		if (gui_draw_button_fill("Quit"))
			window_close(window);

		gui_draw_spacing();
	}

	gui_end_menu(MENU_WIDTH);

	// Renders the import dialog. And when the dialog is closed, loads the jobs data.
	if (gui_render_file_dialog("DIALOG_IMPORT", file_path))
		jobs_import_json(file_path);

	// Renders the export dialog. And when the dialog is closed, saves the jobs data.
	if (gui_render_file_dialog("DIALOG_EXPORT", file_path))
		jobs_export_json(file_path);
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

	jobs_init();

	// Register all the views being used.
	view_register_jobs();
	view_register_job();
	view_register_max_time();
	view_register_min_time();
	view_register_escalation();

	//job_t job = job_init();
	//job_load_file(&job, "job.csv");
	//jobs_insert(&job);

	open_main_window();

	return 0;
}
