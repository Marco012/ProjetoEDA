#include "job.h"
#include <stdio.h>
#include <inttypes.h>


job_t job_new() {
	job_t job;
	job.operations = list_new();

	return job;
}


operation_t* job_new_operation(job_t* job) {
	operation_t operation;
	operation.executions = list_new();
	operation_t* operation_ptr = list_push(&job->operations, &operation, sizeof(operation));
	return operation_ptr;
}


void job_remove_operation(job_t* job, int index) {
	list_remove(&job->operations, index);
}


void job_set_operation(job_t* job, int index, operation_t operation) {
	if (list_remove(&job->operations, index))
		list_insert(&job->operations, &operation, sizeof(operation_t), index, false);
}


bool job_load_file(job_t* job, const char* file_name) {
	FILE* file = fopen(file_name, "r");

	if (file == NULL)
		return false;

	operation_t* operation = job_new_operation(job);

	char buffer[256];

	while (fgets(buffer, 256, file)) {
		if (buffer[0] == '-') {
			operation = job_new_operation(job);
			continue;
		}

		machine_execution_t execution;
		sscanf(buffer, "%" SCNu16 ",%" SCNu16 "\n", &execution.duration, &execution.machine);
		operation_add_execution(operation, execution);
	}

	fclose(file);

	return true;
}


void job_save_file(job_t* job, const char* file_name) {
	FILE* file = fopen(file_name, "w");
	uint32_t i = 1;

	fprintf(file, "Operation,Duration,Machine\n");

	LIST_START_ITERATION((&job->operations), operation_t, operation) {
		list_t* executions = &(operation->executions);

		LIST_START_ITERATION(executions, machine_execution_t, execution) {
			fprintf(file, "%u,%u,%u\n", i, execution->duration, execution->machine);
		}
		LIST_END_ITERATION;

		i++;
	}
	LIST_END_ITERATION;

	fclose(file);
}