#ifndef EDA_OPERATION
#define EDA_OPERATION


#include "utils.h"
#include "list.h"


#define OPERATION_NAME_LENGTH 32


typedef struct {
	uint16_t machine;
	uint16_t duration;
} machine_execution_t;


typedef struct {
	list_t executions;
	char name[OPERATION_NAME_LENGTH];
} operation_t;


/**
 * @brief Adds an execution to an operation if the operation does not contain any other operation with the same machine.
 * 
 * @param operation Operation to add the execution into.
 * @param execution Execution to be added.
 * @return true Execution added successfully into the operation.
 * @return false A machine with the same id already exists in the operation.
 */
bool operation_add_execution(operation_t* operation, machine_execution_t execution);

/**
 * @brief Replaces an execution of an operation at a given index.
 * @param operation Operation to change its execution.
 * @param index Index of the execution to be replaced.
 * @param execution New execution to be replaced into.
 * @return true Execution replaced successfully into the operation.
 * @return false A machine with the same id already exists in the operation.
 */
bool operation_set_execution(operation_t* operation, uint32_t index, machine_execution_t execution);

/**
 * @brief Removes an execution of an operation at a given index.
 * @param operation Operation to remove its execution.
 * @param index Index of the execution to be removed.
 * @return true Execution removed successfully from the operation.
 * @return false Index was out of bounds.
 */
bool operation_remove_execution(operation_t* operation, uint32_t index);


#endif