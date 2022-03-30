#ifndef EDA_TESTS
#define EDA_TESTS


#include <stdint.h>
#include <string.h>


void test_print_job_operations(job_t* job);

void test_job_add_operation(job_t* job, uint32_t value_multiplier);

void test_job_remove_operaton(job_t* job, uint32_t operation_index);

void test_job_set_operaton(job_t* job, uint32_t operation_index, uint32_t execution_index, machine_execution_t execution);

#endif