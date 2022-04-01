#ifndef EDA_TESTS
#define EDA_TESTS


#include <stdint.h>
#include <string.h>


/**
 * @brief Prints the operations and respective executons of a job.
 * 
 * @param job Job to print the values.
 */
void test_print_job_operations(job_t* job);

/**
 * @brief Adds an operations with some executions into a job.
 * 
 * @param job Job to add the operation into.
 * @param value_multiplier Multiplies with the value added executions, to allow this functions to be called multiple times.
 */
void test_job_add_operation(job_t* job, uint32_t value_multiplier);

/**
 * @brief Removes an operation from a job.
 * 
 * @param job Job to remove the operation from.
 * @param operation_index Index of the operation to be removed.
 */
void test_job_remove_operaton(job_t* job, uint32_t operation_index);

/**
 * @brief Replaces an execution of an operation in a job.
 * 
 * @param job Job to replce the execution.
 * @param operation_index Index of the operation in the job.
 * @param execution_index Index of the execution in the operation.
 * @param execution Value of the execution to replace.
 */
void test_job_set_operaton_execution(job_t* job, uint32_t operation_index, uint32_t execution_index, machine_execution_t execution);

#endif