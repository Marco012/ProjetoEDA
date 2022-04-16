#include <stdio.h>
#include "utils.h"
#include "job.h"
#include "job_utils.h"
#include "list.h"
#include "tests.h"
#include <inttypes.h>
#include <gui/gui.h>


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


void userAddOperationMachineToJob(job_t* job) {
	machine_execution_t execution;

	printf("Select an operation: \n");

	uint32_t i = 1;
	LIST_START_ITERATION((&job->operations), operation_t, operation) {
		printf(" > %u\n", i);
		i++;
	}
	LIST_END_ITERATION;

	printf("--New operation--\n");
	printf(" > Machine: ");
	scanf("%" SCNu16, &execution.machine);
	printf(" > Duration: ");
	scanf("%" SCNu16, &execution.duration);
}


void userAddOperationToJob(job_t* job) {
	job_new_operation(job);
}


void ui_draw(window_t* window) {
	float menu_width = 200;

	gui_start_menu();
	{
		gui_draw_title_centered("Menu");
		gui_draw_spacing();
		gui_draw_button("Jobs", menu_width);
		gui_draw_button("Min Time", menu_width);
		gui_draw_button("Max Time", menu_width);
		gui_draw_button("Average Time", menu_width);
		gui_draw_spacing();
		gui_draw_spacing();

		if (gui_draw_button("Quit", menu_width))
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

	test_window();

	job_t job = job_init();

	job_tests(&job);

	job_load_file(&job, "job.csv");

	print_job_finish_min_time(&job);
	print_job_finish_max_time(&job);
	print_job_finish_operation_average_time(&job, 0);

	userAddOperationToJob(&job);
	userAddOperationMachineToJob(&job);

	test_print_job_operations(&job);

	job_save_file(&job, "out.csv");

	job_clear(&job);

	return 0;
}
