#include "list.h"
#include "operation.h"


list_t list_init(void (*free_fn)(void* value)) {
	return (list_t) { .first = NULL, .last = NULL, .free_fn = free_fn };
}


static list_element_t* list_create_element(void* value, uint32_t size) {
	list_element_t* element = malloc(sizeof(list_element_t));
	if (element) {
		element->value = malloc(size);
		memcpy(element->value, value, size);
		element->next = NULL;
	}
	return element;
}


void* list_push(list_t* list, void* value, uint32_t size) {
	list_element_t* element = list_create_element(value, size);

	if (list->first == NULL)
		list->first = element;

	if (list->last != NULL)
		list->last->next = element;

	list->last = element;
	return element->value;
}


void* list_get(list_t* list, uint32_t index) {

	list_element_t* holder = NULL;
	bool stop = index < 0;

	for (uint32_t i = 0; i <= index && !stop; i++) {
		if (holder == NULL)
			holder = list->first;
		else if (holder->next != NULL)
			holder = holder->next;

		// If the list stops here, then we don't need to continue to the next elements. 
		if ((holder == NULL || holder->next == NULL) && i + 1 < index)
			stop = true;
	}

	if (holder != NULL)
		return holder->value;

	return NULL;
}


bool list_insert(list_t* list, void* value, uint32_t size, int index, bool force) {
	list_element_t* element = list_create_element(value, size);
	list_element_t* holder = NULL;
	bool stop = index < 0;

	for (uint32_t i = 0; i < index && !stop; i++) {
		if (holder == NULL)
			holder = list->first;
		else if (holder->next != NULL)
			holder = holder->next;

		// If the list stops here, then we don't need to continue to the next elements. 
		if ((holder == NULL || holder->next == NULL) && i + 1 < index)
			stop = true;
	}

	if (stop && !force) {
		free(element);
		return false;
	}

	if (holder != NULL) {
		element->next = holder->next;

		if (element->next == NULL)
			list->last = element;

		holder->next = element;
	}
	else {    // Enters here when the index is 0.
		if (list->first != NULL)
			element->next = list->first;

		list->first = element;

		if (list->last == NULL)
			list->last = element;
	}

	return true;
}


static void list_free_element(list_t* list, list_element_t* element) {
	if (list->free_fn)
		list->free_fn(element->value);
	else
		free(element->value);

	free(element);
}


bool list_remove(list_t* list, int index) {
	list_element_t* element = list->first;
	list_element_t* previous = NULL;

	if (index < 0)
		return false;

	for (int i = 0; i < index; i++) {
		previous = element;
		element = element->next;
	}

	if (element == NULL)
		return false;

	list_element_t* next = element->next;

	if (element == list->first)
		list->first = next;

	if (element == list->last)
		list->last = previous;

	if (previous != NULL)
		previous->next = next;

	list_free_element(list, element);

	return true;
}


void list_clear(list_t* list) {

	list_element_t* next = list->first;
	list_element_t* temp = NULL;

	while (next != NULL) {
		temp = next->next;

		list_free_element(list, next);
		next = temp;
	}

	list->first = NULL;
	list->last = NULL;
}


void list_print_int(list_t* list) {
	list_element_t* next = list->first;

	while (next != NULL) {
		printf("%d,", *((int32_t*) next->value));
		next = next->next;
	}
}