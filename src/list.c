#include "list.h"



list_t list_new() {
    return (list_t) {.first = NULL, .last = NULL};
}


static void* list_create_element(void *value, uint32_t size) {
    list_element_t *element = malloc(sizeof(list_element_t) + size);
    memcpy(element + sizeof(list_element_t), value, size);
    element->next = NULL;
    return element;
}


void list_push(list_t *list, void *value, uint32_t size) {
    list_element_t *element = list_create_element(value, size);

    if (list->first == NULL)
        list->first = element;

    if (list->last != NULL)
        list->last->next = element;

    list->last = element;
}


bool list_insert(list_t *list, void *value, uint32_t size, int index, bool force) {
    list_element_t *element = list_create_element(value, size);
    list_element_t *holder = NULL;
    bool stop = index < 0;

    for (int i = 0; i < index && !stop; i++) {

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
    } else {    // Enters here when the index is 0.
        if (list->first != NULL)
            element->next = list->first;

        list->first = element;

        if (list->last == NULL)
            list->last = element;
    }

    return true;
}


bool list_remove(list_t *list, int index) {
    list_element_t *element = NULL;
    
    for (int i = 0; i < index; i++) {
        if (element == NULL)
            element = list->first;
        else
            element = element->next;
    }

    if (element == NULL)
        return false;

    if (element->next != NULL) {
        list_element_t *next = element->next;

        if (next == list->last)
            list->last = element;

        if (next->next != NULL) {
            element->next = next->next;
        }
    }

    return true;
}


void list_clear(list_t *list) {
    
    list_element_t *next = list->first;
    list_element_t *temp = NULL;

    while (next != NULL) {
        temp = next->next;
        free(next);
        next = temp;
    }

    list->first = NULL;
    list->last = NULL;
}


void list_print_int(list_t *list) {
    list_element_t *next = list->first;

    while (next != NULL) {
        printf("%d,", *(next + sizeof(list_element_t)));
        next = next->next;
    }
}