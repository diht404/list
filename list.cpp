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

    return LIST_NO_ERRORS;
}

size_t resizeList(List *list)
{
    Elem_t *new_data = (Elem_t *) realloc(list->data,
                                          list->capacity * 2);

    if (new_data == nullptr)
    {
        return LIST_CANT_ALLOCATE_MEMORY;
    }

    list->data = new_data;
    list->capacity *= 2;

    return LIST_NO_ERRORS;
}

size_t dtorList(List *list)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    free(list->data);
    return LIST_NO_ERRORS;
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
        return listPushEmpty(list, value);
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
    list->data[list->free].alive = true;
    // set new tree
    list->tail = list->free;
    // update free index
    list->free = next_free_index;
    list->size += 1;

    return LIST_NO_ERRORS;
}

size_t listPushEmpty(List *list, Val_t value)
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
    list->data[list->free].alive = true;
    // move free
    list->free = next_free_index;

    list->size++;
    return LIST_NO_ERRORS;
}

size_t listFront(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return 0;
    }

    return list->head;
}

size_t listBack(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return 0;
    }

    return list->tail;
}

size_t listInsertAfter(List *list, size_t position, Val_t value)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT;

    if (position == list->tail)
        return listPush(list, value);

    list->data[list->free].value = value;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;
    size_t next_free_index = list->data[list->free].next;

    // get prev next indexes
    size_t prev_index = position;
    size_t next_index = list->data[position].next;

    // update prev next free elems
    list->data[prev_free_index].next = next_free_index;
    list->data[next_free_index].prev = prev_free_index;

    // update prev next elems
    list->data[next_index].prev = list->free;
    list->data[position].next = list->free;

    // set new elem params
    list->data[list->free].next = next_index;
    list->data[list->free].prev = prev_index;
    list->data[list->free].alive = true;
    // move free
    list->free = next_free_index;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listInsertBefore(List *list, size_t position, Val_t value)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT;

    size_t position_to_insert_after = list->data[position].prev;

    return listInsertAfter(list, position_to_insert_after, value);
}

size_t listPop(List *list, size_t position)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT;

    if (position == list->tail)
        return listPopTail(list);

    if (position == list->head)
        return listPopHead(list);

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;

    // get prev next indexes
    size_t prev_index = list->data[position].prev;
    size_t next_index = list->data[position].next;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[position].alive = false;
    list->data[position].value = POISONED_VALUE;
    list->data[position].prev = prev_free_index;
    list->data[prev_free_index].next = position;
    list->data[position].next = list->free;

    list->free = position;

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listPopTail(List *list)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;

    // get prev next indexes
    size_t prev_index = list->data[list->tail].prev;
    size_t next_index = list->head;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[list->tail].alive = false;
    list->data[list->tail].value = POISONED_VALUE;
    list->data[list->tail].prev = prev_free_index;
    list->data[prev_free_index].next = list->tail;
    list->data[list->tail].next = list->free;

    list->free = list->tail;
    list->tail = prev_index;

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listPopHead(List *list)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;

    // get prev next indexes
    size_t prev_index = list->tail;
    size_t next_index = list->data[list->head].next;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[list->head].alive = false;
    list->data[list->head].value = POISONED_VALUE;
    list->data[list->head].prev = prev_free_index;
    list->data[prev_free_index].next = list->head;
    list->data[list->head].next = list->free;

    list->free = list->head;
    list->head = next_index;

    list->size--;

    return LIST_NO_ERRORS;
}

