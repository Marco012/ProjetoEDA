#include <stdio.h>
#include "utils.h"
#include "job.h"
#include "list.h"
#include "tests.h"


#define FILTER_MIN 0b00000111
#define FILTER_MAX 0b00001111


typedef struct {
	uint32_t operation_id;
	machine_execution_t execution;
} operation_execution_t;


void get_job_finish_filtered_operations(job_t* job, list_t* out_operation_executions, uint8_t filter) {
	list_t* operations = &(job->operations);
	operation_execution_t operation_execution;
	uint8_t i = 1;

	*out_operation_executions = list_init(NULL);

	LIST_START_ITERATION(operations, operation_t, operation) {
		list_t* executions = &(operation->executions);
		operation_execution.operation_id = i;
		bool hasExecution = false;

		LIST_START_ITERATION(executions, machine_execution_t, execution) {
			if (!hasExecution || ((execution->duration < operation_execution.execution.duration && filter == FILTER_MIN) || (execution->duration > operation_execution.execution.duration && filter == FILTER_MAX)))
				operation_execution.execution = *execution;

			hasExecution = true;
		}
		LIST_END_ITERATION;

		if (hasExecution)
			list_push(out_operation_executions, &operation_execution, sizeof(operation_execution_t));

		i++;
	}
	LIST_END_ITERATION;
}


void print_job_finish_min_time(job_t* job) {
	list_t operations;
	uint32_t total = 0;

	get_job_finish_filtered_operations(job, &operations, FILTER_MIN);
	printf("> Minimum Operations\n");

	LIST_START_ITERATION((&operations), operation_execution_t, operation) {
		printf("Operation %u: machine: %u duration: %u\n", operation->operation_id, operation->execution.machine, operation->execution.duration);
		total += operation->execution.duration;
	}
	LIST_END_ITERATION;

	printf("Total duration: %u\n\n", total);
	list_clear(&operations);
}


void print_job_finish_max_time(job_t* job) {
	list_t operations;
	uint32_t total = 0;

	get_job_finish_filtered_operations(job, &operations, FILTER_MAX);
	printf("> Maximum Operations\n");

	LIST_START_ITERATION((&operations), operation_execution_t, operation) {
		printf("Operation %u: machine: %u duration: %u\n", operation->operation_id, operation->execution.machine, operation->execution.duration);
		total += operation->execution.duration;
	}
	LIST_END_ITERATION;

	printf("Total duration: %u\n\n", total);
	list_clear(&operations);
}


void print_job_finish_operation_average_time(job_t* job, uint32_t operation_index) {
	uint32_t count = 0;
	uint32_t total_duration = 0;
	operation_t* operation = list_get(&(job->operations), operation_index);

	printf("> Average time of operation %u\n", operation_index + 1);

	if (operation == NULL)
	{
		printf("Invalid operation index!\n");
		return;
	}

	LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
		total_duration += execution->duration;
		count++;
	}
	LIST_END_ITERATION;

	printf("Average duration of %u possibilities: %u\n\n", count, total_duration / count);
}


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
