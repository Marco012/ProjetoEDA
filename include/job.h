#ifndef EDA_JOB
#define EDA_JOB


#include "operation.h"
#include "list.h"


#define JOB_NAME_LENGTH 32


typedef struct {
	list_t operations;
	char name[JOB_NAME_LENGTH];
} job_t;


/**
 * @brief Inits a job by setting its variables to their initial value.
 * 
 * @return job_t Empty job ready to use.
 */
job_t job_init(char* name);

/**
 * @brief Creates a new operation in a job.
 * 
 * @param job Job to create the operation.
 * @param name Designation of the operation.
 * @return operation_t* Created operation.
 */
operation_t* job_new_operation(job_t* job, char* name);

/**
 * @brief Removes an operation from a job on a given index.
 * 
 * @param job Job to remove the operation from.
 * @param index Index of the operation to be removed.
 */
void job_remove_operation(job_t* job, int index);


/**
 * @brief Insert an operation at a specific index of a job.
 * @param job Job to receive the operation.
 * @param index Index to be placed the operation.
 * @param operation Operation to be inserted into the job.
*/
void job_insert_operation(job_t* job, int index, operation_t* operation);


/**
 * @brief Replaces an operation in a job on a given index.
 * 
 * @param job Job to replace the operation.
 * @param index Index of the operation that will be replaced.
 * @param operation Operation to replace.
 */
void job_set_operation(job_t* job, int index, operation_t operation);

/**
 * @brief Loads the operations in a CSV file to a job. 
 * All the previous operations in the job will not be removed.
 * 
 * @param job Job to load the operations into.
 * @param file_name Name of the file to load the operations from.
 * @return true File load successfully.
 * @return false Error loading the file.
 */
bool job_load_file(job_t* job, const char* file_name);

/**
 * @brief Saves all the operations in a job into a CSV file.
 * 
 * @param job Job to save its operations.
 * @param file_name Name of the file to be saved.
 */
void job_save_file(job_t* job, const char* file_name);

/**
 * @brief Clears a job and all his memory used, including its operations.
 * @param job Job to be cleared.
*/
void job_clear(job_t* job);

#endif