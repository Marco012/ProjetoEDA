#ifndef EDA_JOB_UTILS
#define EDA_JOB_UTILS


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "operation.h"
#include "list.h"
#include "job.h"


typedef enum {
    FILTER_MIN, FILTER_MAX
} operations_filter_t;


typedef struct {
	uint32_t operation_id;
	machine_execution_t execution;
} operation_execution_t;


/**
 * @brief Get the job finish filtered operations object
 * 
 * @param job Job to get the duration.
 * @param out_operation_executions List to add the data into.
 * @param filter Filter that can be used to get the maximum or minimum duration.
 */
void get_job_finish_filtered_operations(job_t* job, list_t* out_operation_executions, operations_filter_t filter);

/**
 * @brief Prints the minimum duration to finish a job and the used executions.
 * 
 * @param job Job to use.
 */
void print_job_finish_min_time(job_t* job);

/**
 * @brief Prints the maximum duration to finish a job and the used executions.
 * 
 * @param job Job to use.
 */
void print_job_finish_max_time(job_t* job);

/**
 * @brief Prints the average time to complete an operation in a job.
 * 
 * @param job Job to use.
 * @param operation_index Index of the operation in the job. 
 */
void print_job_finish_operation_average_time(job_t* job, uint32_t operation_index);

#endif