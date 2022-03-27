#include <stdio.h>
#include "utils.h"
#include "job.h"

int main(char* args) {
    list_t jobs = list_new();
    
    job_t job = job_new();
    
    operation_t operation;
    operation.duration = 5;
    operation.machine = 2;
    job_add_operation(&job, operation);

    operation.duration = 10;
    //job_add_operation(&job, operation);

    operation.duration = 6;
    //job_set_operation(&job, 1, operation);

    list_t *operations = &(job.operations);

    LIST_START_ITERATION(operations, operation_t, value) {
        printf("%d", value->duration);
    }
    LIST_END_ITERATION()

}
