#include "job.h"
#include <stdio.h>
#include <inttypes.h>


static void job_operation_free(void *value) {
	operation_t *operation = (operation_t*) value;
	list_clear(&operation->executions);
	free(operation);
}


job_t job_init() {
	job_t job;
	job.operations = list_init(&job_operation_free);

	return job;
}


operation_t* job_new_operation(job_t* job, char* name) {
	operation_t operation;
	operation.executions = list_init(NULL);

	operation.name[0] = 0;

	if (name != NULL) {
		int length = strlen(name);

		length = length > OPERATION_NAME_LENGTH - 1 ? OPERATION_NAME_LENGTH - 1 : length;
		strncat(operation.name, name, length);
	}

	return list_push(&job->operations, &operation, sizeof(operation));
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

	operation_t* operation = job_new_operation(job, NULL);

	char buffer[256];

	// Go through each line and store it in the buffer.
	while (fgets(buffer, 256, file)) {
		// Check if it's creating a new operation.
		if (buffer[0] == '-') {
			operation = job_new_operation(job, NULL);
			continue;
		}

		// Reads the execution and adds it into the current operation.
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

	// Go through all the operations of the job.
	LIST_START_ITERATION((&job->operations), operation_t, operation) {
		list_t* executions = &(operation->executions);

		// Go through all the executions fo the operation and adds a line to the file.
		LIST_START_ITERATION(executions, machine_execution_t, execution) {
			fprintf(file, "%u,%u,%u\n", i, execution->duration, execution->machine);
		}
		LIST_END_ITERATION;

		i++;
	}
	LIST_END_ITERATION;

	fclose(file);
}


void job_clear(job_t* job) {
	list_clear(&job->operations);
}
