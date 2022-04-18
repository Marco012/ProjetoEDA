#include <utils.h>
#include <job.h>
#include <job_utils.h>
#include <list.h>
#include <tests.h>
#include <views.h>

#include <stdio.h>
#include <inttypes.h>
#include <gui/gui.h>


list_t jobs;


void job_tests(job_t* job) {

	// Add operations into the job.
	test_job_add_operation(job, 1);
	test_job_add_operation(job, 2);
	test_job_add_operation(job, 3);
	printf("Added 3 operations.\n");
	test_print_job_operations(job);

	// Replace an execution of an operation of the job.
	machine_execution_t execution = { .machine = 88, .duration = 99 };
	test_job_set_operaton_execution(job, 1, 1, execution);
	printf("Changed the value of the 2nd execution in the operation in the middle.\n");
	test_print_job_operations(job);

	// Remove an operation from the job.
	test_job_remove_operaton(job, 1);
	printf("Removed the operation in the middle.\n");
	test_print_job_operations(job);

}


void ui_draw(window_t* window) {
	float menu_width = 200;

	gui_start_menu();
	{
		gui_draw_title_centered("Menu");
		gui_draw_spacing();

		if (gui_draw_button_fill(VIEW_TITLE_JOBS))
			view_open_jobs();

		if (gui_draw_button_fill(VIEW_TITLE_MIN_TIME)) {
			job_t* job = list_get(jobs_get_all(), 0);
			gui_open_view(VIEW_TITLE_MIN_TIME, job);
		}

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

	gui_end_menu(menu_width);
}


void test_window() {
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

	view_register_jobs();
	view_register_job();
	view_register_max_time();
	view_register_min_time();

	job_t job = job_init();
	job_load_file(&job, "job.csv");
	jobs_insert(&job);

	test_window();


	job_tests(&job);


	print_job_finish_min_time(&job);
	print_job_finish_max_time(&job);
	print_job_finish_operation_average_time(&job, 0);

	test_print_job_operations(&job);

	//job_save_file(&job, "out.csv");

	job_clear(&job);

	return 0;
}
