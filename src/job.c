#include "job.h"


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