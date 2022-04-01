#include <stdio.h>
#include "utils.h"
#include "job.h"
#include "job_utils.h"
#include "list.h"
#include "tests.h"


void job_tests(job_t* job) {
	printf("Added 3 operations.\n");
	test_job_add_operation(job, 1);
	test_job_add_operation(job, 2);
	test_job_add_operation(job, 3);
	test_print_job_operations(job);

	printf("Changed the value of the 2nd execution in the operation in the middle.\n");
	machine_execution_t execution = { .machine = 88, .duration = 99 };
	test_job_set_operaton_execution(job, 1, 1, execution);
	test_print_job_operations(job);

	printf("Removed the operation in the middle.\n");
	test_job_remove_operaton(job, 1);
	test_print_job_operations(job);

}


int main(void) {
	job_t job = job_init();

	job_tests(&job);

	job_load_file(&job, "job.csv");

	print_job_finish_min_time(&job);
	print_job_finish_max_time(&job);
	print_job_finish_operation_average_time(&job, 0);

	test_print_job_operations(&job);

	job_save_file(&job, "out.csv");

	job_clear(&job);

	return 0;
}
