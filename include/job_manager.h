#ifndef EDA_JOB_MANAGER
#define EDA_JOB_MANAGER


#include "utils.h"
#include "list.h"
#include "job.h"


/**
 * @brief Inits the job manager system.
*/
void jobs_init();


/**
 * @brief Get the list containing all the jobs.
 * @return The list containing all the jobs. The returned list is not a copy. Avoid making changes to it.
*/
list_t* jobs_get_all();

/**
 * @brief Adds a new job into the jobs list.
 * @param job Job to add.
*/
void jobs_insert(job_t* job);


/**
 * @brief Clears the jobs list.
*/
void jobs_clear();

#endif