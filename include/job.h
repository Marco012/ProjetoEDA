#ifndef EDA_JOB
#define EDA_JOB


#include "operation.h"
#include "list.h"


typedef struct {
    list_t operations;
} job_t;


job_t job_new();

void job_add_operation(job_t *job, operation_t operation);

void job_remove_operation(job_t *job, int index);

void job_set_operation(job_t *job, int index, operation_t operation);

#endif