#ifndef EDA_JOB
#define EDA_JOB


#include "operation.h"
#include "list.h"


typedef struct {
	list_t operations;
} job_t;


job_t job_new();

operation_t* job_new_operation(job_t* job);

void job_remove_operation(job_t* job, int index);

void job_set_operation(job_t* job, int index, operation_t operation);

bool job_load_file(job_t* job, const char* file_name);

void job_save_file(job_t* job, const char* file_name);

#endif