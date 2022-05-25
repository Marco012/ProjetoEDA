#ifndef EDA_LIST
#define EDA_LIST


#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


/**
 * @brief Foreach-like loop to iterate through a list.
 * 
 * list: The list to iterate
 * type: Raw type of the values in the list.
 * value_name: Name of the variable being created to receive the value on each iteration.
 */
#define LIST_START_ITERATION(list, type, value_name){                                   \
    list_element_t* _list_element_next = (list)->first;                                 \
                                                                                        \
    while (_list_element_next != NULL) {                                                \
        type* value_name = (type*) _list_element_next->value;                           \
        _list_element_next = _list_element_next->next;                                  \


/**
 * @brief Ends a list iteration created by LIST_START_ITERATION.
 * 
 */
#define LIST_END_ITERATION }}


typedef struct
{
	void* next; ///< Pointer to the next element.
	void* value; ///< Pointer to the value of the element.
} list_element_t;


typedef struct {
	list_element_t* first; ///< Pointer to the first element in the list.
	list_element_t* last; ///< Pointer to the last element in the list.
	void (*free_fn)(void* value); ///< Function to free an element.
} list_t;


/**
 * @brief Inits a list by setting its variables to their initial value.
 * Do not call this to clear a list!
 * This does not clear the memory allocated by the list.
 * 
 * @return list_t Empty list ready to be used.
 */
list_t list_init(void (*free_fn)(void* value));

/**
 * @brief Adds a new value to the end of the list.
 * 
 * @param list List to add the value into.
 * @param value Pointer to the value to be added into the list.
 * @param size Size of the value being added.
 * @return void* Pointer to the value allocated.
 */
void* list_push(list_t* list, void* value, uint32_t size);

/**
 * @brief Retrieves a value from the list at a given index.
 * 
 * @param list List to retrieve the value from.
 * @param index Index of the value being retrieved.
 * @return void* Pointer to the value obtained. May be NULL if the value was not found.
 */
void* list_get(list_t* list, uint32_t index);

/**
 * @brief Adds a new value to a given index in the list.
 * 
 * @param list List to add the value into.
 * @param value Pointer to the value to be added into the list.
 * @param size Size of the value being added.
 * @param index Index to add the value in.
 * @param force Forces the list to add the value. If this is false and the index is invalid, the value will not be added.
 * @return true Value inserted successfully.
 * @return false Value could not be inserted.
 */
bool list_insert(list_t* list, void* value, uint32_t size, int index, bool force);

/**
 * @brief Removes a value from the list at given index.
 * 
 * @param list List to remove the value from.
 * @param index Index of the value to be removed.
 * @return true Value removed successfully.
 * @return false Invalid list index.
 */
bool list_remove(list_t* list, int index);

/**
 * @brief Clears the list and deallocates all the memory used.
 * The list can still be used after calling this, without any problem.
 * 
 * @param list List to be cleared.
 */
void list_clear(list_t* list);

/**
 * @brief Helper function to print all the list values as integers.
 * This functions only works if the values are 32 bit integers.
 * 
 * @param list List to print its values.
 */
void list_print_int(list_t* list);

#endif