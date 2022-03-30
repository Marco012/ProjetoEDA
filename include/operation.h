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


bool operation_add_execution(operation_t* operation, machine_execution_t execution);


#endif