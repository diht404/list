#include "list.h"

size_t ctorList(List *list, size_t capacity)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (list->data != nullptr)
        return LIST_DATA_NOT_NULLPTR;

    list->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));
    list->capacity = capacity;

    for (size_t i = 0; i < list->capacity; i++)
    {
        list->data[i].next = (i + 1) % list->capacity;
        list->data[i].prev = (list->capacity + i - 1) % list->capacity;
        list->data[i].value = POISONED_VALUE;
    }

    list->free = 10;
    list->head = 10;
    list->tail = 10;

    return NO_ERRORS;
}

size_t listDump(List *list)
{
    if (list == nullptr)
    {
        return CANT_ALLOCATE_MEMORY;
    }

    printf("DATA: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printElem_t(list->data[i].value);
    }
    printf("\n");

    printf("NEXT: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printNextPrev(list->data[i].next);
    }
    printf("\n");

    printf("IND:  ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printNextPrev(i);
    }
    printf("\n");

    printf("PREV: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printNextPrev(list->data[i].prev);
    }
    printf("\n");

    printf("HEAD: ");
    printNextPrev(list->head);
    printf("\n");

    printf("TAIL: ");
    printNextPrev(list->tail);
    printf("\n");

    printf("FREE: ");
    printNextPrev(list->free);
    printf("\n");
    return NO_ERRORS;
}

void printElem_t(Val_t elem)
{
    printf("%02d ", elem);
}

void printNextPrev(size_t value)
{
    printf("%02zu ", value);
}

size_t resizeList(List *list)
{
    Elem_t
        *new_data = (Elem_t *) realloc(list->data, list->capacity * 2);

    if (new_data == nullptr)
    {
        return CANT_ALLOCATE_MEMORY;
    }

    list->data = new_data;
    list->capacity *= 2;

    return NO_ERRORS;
}

size_t dtorList(List *list)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    free(list->data);
    return NO_ERRORS;
}

size_t listPushFront(List *list, Val_t value)
{
    return listPush(list, value);
}

size_t listPushBack(List *list, Val_t value)
{
    return listPush(list, value);
}

size_t listPush(List *list, Val_t value)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (list->size == 0)
    {
        return listSetEmpty(list, value);
    }

    list->data[list->free].value = value;
    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;
    size_t next_free_index = list->data[list->free].next;

    // update prev next free elems
    list->data[prev_free_index].next = next_free_index;
    list->data[next_free_index].prev = prev_free_index;

    // update prev next elems
    list->data[list->head].prev = list->free;
    list->data[list->tail].next = list->free;

    // set new elem params
    list->data[list->free].prev = list->tail;
    list->data[list->free].next = list->head;
    // set new tree
    list->tail = list->free;
    // update free index
    list->free = next_free_index;
}

size_t listSetEmpty(List *list, Val_t value)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    list->data[list->free].value = value;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;
    size_t next_free_index = list->data[list->free].next;

    // update prev next free elems
    list->data[prev_free_index].next = next_free_index;
    list->data[next_free_index].prev = prev_free_index;

    // set new elem params
    list->data[list->free].next = list->free;
    list->data[list->free].prev = list->free;
    // move free
    list->free = next_free_index;

    list->size += 1;
    return NO_ERRORS;
}
