#ifndef EDA_LIST
#define EDA_LIST


#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


#define LIST_START_ITERATION(list, type, value_name){                                   \
    list_element_t* _list_element_next = list->first;                                   \
                                                                                        \
    while (_list_element_next != NULL) {                                                \
        type* value_name = (type*) _list_element_next->value;                           \
        _list_element_next = _list_element_next->next;                                                            \


#define LIST_END_ITERATION }}


typedef struct
{
	void* next;
	void* value;
} list_element_t;


typedef struct {
	list_element_t* first;
	list_element_t* last;
	void (*free_fn)(void* value);
} list_t;


list_t list_new();

void* list_push(list_t* list, void* value, uint32_t size);

void* list_get(list_t* list, uint32_t index);

bool list_insert(list_t* list, void* value, uint32_t size, int index, bool force);

bool list_remove(list_t* list, int index);

void list_clear(list_t* list);

void list_print_int(list_t* list);

#endif