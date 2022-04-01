#ifndef EDA_OPERATION
#define EDA_OPERATION


#include "utils.h"
#include "list.h"


typedef struct {
	uint16_t machine;
	uint16_t duration;
} machine_execution_t;


typedef struct {
	list_t executions;
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


#endif