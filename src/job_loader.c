#include <job_loader.h>
#include "cJSON.h"


void jobs_import_json(char* path) {
	jobs_clear();

	FILE* ptr = fopen(path, "r"); // read only 
	char ch;
	char data[100000];

	int i = 0;
	do {
		ch = fgetc(ptr);
		data[i] = ch;

		// Checking if character is not EOF.
		// If it is EOF stop eading.
		i++;
	} while (ch != EOF);

	cJSON* monitor_json = cJSON_Parse(data);
	const cJSON* jobJson = NULL;


	cJSON_ArrayForEach(jobJson, monitor_json) {

		cJSON* operationsJson = cJSON_GetObjectItemCaseSensitive(jobJson, "job");
		cJSON* job_name = cJSON_GetObjectItemCaseSensitive(jobJson, "name");

		job_t job = job_init(job_name == NULL ? NULL : job_name->valuestring);

		cJSON* operationJson = NULL;
		cJSON_ArrayForEach(operationJson, operationsJson) {

			cJSON* machinesJson = cJSON_GetObjectItemCaseSensitive(operationJson, "operations");
			cJSON* name = cJSON_GetObjectItemCaseSensitive(operationJson, "name");
			cJSON* machineJSON = NULL;

			operation_t* operation = job_new_operation(&job, name == NULL ? NULL : name->valuestring);

			cJSON_ArrayForEach(machineJSON, machinesJson) {
				cJSON* machine = cJSON_GetObjectItemCaseSensitive(machineJSON, "machine");
				cJSON* duration = cJSON_GetObjectItemCaseSensitive(machineJSON, "duration");

				machine_execution_t execution;
				execution.duration = duration->valueint;
				execution.machine = machine->valueint - 1;


				operation_add_execution(operation, execution);
			}
		}

		jobs_insert(&job);
	}

	cJSON_Delete(monitor_json);
}


void jobs_export_json(char* path) {
	list_t* jobs = jobs_get_all();
	cJSON* json_jobs = cJSON_CreateArray();

	LIST_START_ITERATION(jobs, job_t, job) {
		cJSON* json_job = cJSON_CreateObject();
		cJSON* json_job_operations = cJSON_AddArrayToObject(json_job, "job");

		if (strlen(job->name) > 0)
			cJSON_AddItemToObject(json_job, "name", cJSON_CreateString(job->name));

		LIST_START_ITERATION((&job->operations), operation_t, operation) {
			cJSON* json_operation = cJSON_CreateObject();
			cJSON* json_operation_machines = cJSON_AddArrayToObject(json_operation, "operations");

			if (strlen(operation->name) > 0)
				cJSON_AddItemToObject(json_operation, "name", cJSON_CreateString(operation->name));

			LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
				cJSON* json_machine = cJSON_CreateObject();
				cJSON_AddItemToObject(json_machine, "machine", cJSON_CreateNumber(execution->machine + 1));
				cJSON_AddItemToObject(json_machine, "duration", cJSON_CreateNumber(execution->duration + 1));

				cJSON_AddItemToArray(json_operation_machines, json_machine);

			}
			LIST_END_ITERATION;
			cJSON_AddItemToArray(json_job_operations, json_operation);

		}
		LIST_END_ITERATION;
		
		cJSON_AddItemToArray(json_jobs, json_job);
	}
	LIST_END_ITERATION;

	char* result = cJSON_Print(json_jobs);

	FILE* file = fopen(path, "w");
	fprintf(file, "%s", result);
	fclose(file);

	free(result);
	cJSON_Delete(json_jobs);
}