#include <stdio.h>
#include "utils.h"
#include "job.h"
#include "list.h"
#include "tests.h"


void test_print_job_operations(job_t* job) {
	uint32_t i = 1;
	LIST_START_ITERATION((&job->operations), operation_t, operation) {
		printf("> Operation: %u\n", i);
		list_t* executions = &(operation->executions);
		LIST_START_ITERATION(executions, machine_execution_t, execution) {
			printf("\tmachine: %u duration: %u\n", execution->machine, execution->duration);
		}
		LIST_END_ITERATION;
		i++;
	}
	LIST_END_ITERATION;

	printf("\n");
}


void test_job_add_operation(job_t* job, uint32_t value_multiplier) {
	operation_t* operation = job_new_operation(job, NULL);

	machine_execution_t execution;
	execution.duration = 10 * value_multiplier;
	execution.machine = 1 * value_multiplier;

	operation_add_execution(operation, execution);

	execution.duration = 6 * value_multiplier;
	execution.machine = 2 * value_multiplier;
	operation_add_execution(operation, execution);

	execution.duration = 17 * value_multiplier;
	execution.machine = 1 * value_multiplier;
	operation_add_execution(operation, execution);
}


void test_job_remove_operaton(job_t* job, uint32_t operation_index) {
	list_remove(&job->operations, operation_index);
}


void test_job_set_operaton_execution(job_t* job, uint32_t operation_index, uint32_t execution_index, machine_execution_t execution) {
	list_t* executions = list_get(&job->operations, operation_index);

	if (executions != NULL) {
		list_remove(executions, execution_index);
		list_insert(executions, &execution, sizeof(execution), execution_index, false);
	}
}


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