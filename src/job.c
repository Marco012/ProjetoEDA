#include "job.h"
#include <stdio.h>


job_t job_new() {
    job_t job;
    job.operations = list_new();

    return job;
}


void job_add_operation(job_t *job, operation_t operation) {
    list_push(&job->operations, &operation, sizeof(operation));
}


void job_remove_operation(job_t *job, int index) {
    list_remove(&job->operations, index);
}


void job_set_operation(job_t *job, int index, operation_t operation) {
    if (list_remove(&job->operations, index))
        list_insert(&job->operations, &operation, sizeof(operation_t), index, false);
}


bool job_load_file(job_t *job, const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
        return false;

    operation_t operation;

    while(!feof(file)) { 
        fscanf(file, "%d,%d", &operation.duration, &operation.machine);
        job_add_operation(job, operation);
    }

    fclose(file);

    return true;
}