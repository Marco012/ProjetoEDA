#ifndef EDA_JOB_MANAGER
#define EDA_JOB_MANAGER


#include "utils.h"
#include "list.h"
#include "job.h"


list_t* jobs_get_all();

void jobs_insert(job_t* job);

#endif