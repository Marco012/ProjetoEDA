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
		job_t job = job_init();

		cJSON* operationsJson = cJSON_GetObjectItemCaseSensitive(jobJson, "job");

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
}


void jobs_export_json(char* path) {

}