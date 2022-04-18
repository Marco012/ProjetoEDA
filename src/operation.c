#include "operation.h"


static bool operation_has_machine(operation_t* operation, uint16_t machine) {
	LIST_START_ITERATION((&operation->executions), machine_execution_t, execution) {
		if (execution->machine == machine)
			return true;
	}
	LIST_END_ITERATION;

	return false;
}


bool operation_add_execution(operation_t* operation, machine_execution_t execution) {
	if (operation_has_machine(operation, execution.machine))
		return false;

	list_push(&operation->executions, &execution, sizeof(execution));
	return true;
}


bool operation_remove_execution(operation_t* operation, uint32_t index) {
	return list_remove(&operation->executions, index);
}