#include "list_utils.h"

size_t ctorList(List *list, size_t capacity)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    //head = list->data[0].next
    //tail = list->data[0].prev
    if (list->data != nullptr)
        return LIST_DATA_NOT_NULLPTR;

    if (list->alive)
        return LIST_ALREADY_ALIVE;

    if (capacity < 2)
        capacity = 2;

    list->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));

    CHECK_NULLPTR_ERROR(list->data, LIST_DATA_IS_NULLPTR)

    list->capacity = capacity;

    list->data[0].prev = list->capacity - 1;
    list->data[0].next = 1;
    list->data[0].value = POISONED_VALUE;

    for (size_t i = 1; i < list->capacity; i++)
    {
        list->data[i].next = 1 + (i) % (list->capacity - 1);
        list->data[i].prev = (list->capacity + i - 1) % list->capacity;
        list->data[i].value = POISONED_VALUE;
    }
    list->data[1].prev = list->capacity - 1;

    list->size = 1;
    list->free = 1;
    list->alive = true;

    return LIST_NO_ERRORS;
}

size_t resizeList(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    size_t old_capacity = list->capacity;
    size_t new_capacity = list->capacity * 2;

    Elem_t *new_data = (Elem_t *) realloc(list->data,
                                          sizeof(Elem_t)
                                              * new_capacity);

    CHECK_NULLPTR_ERROR(new_data , LIST_CANT_ALLOCATE_MEMORY)

    list->data = new_data;

    list->capacity = new_capacity;
    for (size_t i = old_capacity; i < new_capacity; i++)
    {
        list->data[i].next = (i + 1) % new_capacity;
        list->data[i].prev = (i - 1) % new_capacity;
        list->data[i].value = POISONED_VALUE;
    }
    size_t prev_free_index = list->data[list->free].prev;

    list->data[prev_free_index].next = old_capacity;
    list->data[old_capacity].prev = prev_free_index;

    list->data[list->capacity - 1].next = list->free;
    list->data[list->free].prev = list->capacity - 1;
    return LIST_NO_ERRORS;
}

size_t dtorList(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    free(list->data);
    list->alive = false;

    return LIST_NO_ERRORS;
}

size_t listPushFront(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    return listPush(list, value);
}

size_t listPushBack(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    return listPush(list, value);
}

size_t listPush(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->capacity - list->size <= 1)
    {
        resizeList(list);
    }

    list->data[list->free].value = value;
    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;
    size_t next_free_index = list->data[list->free].next;

    // update prev next free elems
    list->data[prev_free_index].next = next_free_index;
    list->data[next_free_index].prev = prev_free_index;

    // update prev next elems
    list->data[list->data[0].next].prev = list->free;
    list->data[list->data[0].prev].next = list->free;

    // set new elem params
    list->data[list->free].prev = list->data[0].prev;
    list->data[list->free].next = list->data[0].next;
    list->data[list->free].alive = true;
    // set new tree
    list->data[0].prev = list->free;
    // update free index
    list->free = next_free_index;
    list->size += 1;

    return LIST_NO_ERRORS;
}

size_t listPushEmpty(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

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

    return list->data[0].next;
}

size_t listBack(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return 0;
    }

    return list->data[0].prev;
}

size_t listInsertAfter(List *list, size_t position, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT;

    if (list->capacity - list->size <= 1)
    {
        resizeList(list);
    }

    if (position == list->data[0].prev)
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
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT;

    if (list->capacity - list->size <= 1)
    {
        resizeList(list);
    }

    size_t position_to_insert_after = list->data[position].prev;

    return listInsertAfter(list, position_to_insert_after, value);
}

size_t listPop(List *list, size_t position)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_POP_DEAD_ELEMENT;

    if (position == list->data[0].prev)
        return listPopTail(list);

    if (position == list->data[0].next)
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
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;

    // get prev next indexes
    size_t prev_index = list->data[list->data[0].prev].prev;
    size_t next_index = list->data[0].next;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[list->data[0].prev].alive = false;
    list->data[list->data[0].prev].value = POISONED_VALUE;
    list->data[list->data[0].prev].prev = prev_free_index;
    list->data[prev_free_index].next = list->data[0].prev;
    list->data[list->data[0].prev].next = list->free;

    list->free = list->data[0].prev;
    list->data[0].prev = prev_index;

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listPopHead(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;

    // get prev next indexes
    size_t prev_index = list->data[0].prev;
    size_t next_index = list->data[list->data[0].next].next;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[list->data[0].next].alive = false;
    list->data[list->data[0].next].value = POISONED_VALUE;
    list->data[list->data[0].next].prev = prev_free_index;
    list->data[prev_free_index].next = list->data[0].next;
    list->data[list->data[0].next].next = list->free;

    list->free = list->data[0].next;
    list->data[0].next = next_index;

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listFirstOccurrence(List *list,
                           Val_t value,
                           size_t *position,
                           bool *success)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(position, LIST_POSITION_IS_NULLPTR)

    size_t num_checked = 0;
    size_t check_position = list->data[0].next;

    while (num_checked < list->size)
    {
        if (list->data[check_position].value == value)
        {
            *position = check_position;
            *success = true;
            return LIST_NO_ERRORS;
        }
        check_position = list->data[check_position].next;
        num_checked++;
    }
    *success = false;
    return LIST_NO_ERRORS;
}

size_t listElemByIndex(List *list,
                       size_t index,
                       size_t *position)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(position, LIST_POSITION_IS_NULLPTR)

    size_t num_checked = 0;
    size_t check_position = list->data[0].next;

    for (size_t i = 0; i < index % list->size; i++)
    {
        check_position = list->data[check_position].next;
    }
    *position = check_position;

    return LIST_NO_ERRORS;
}

size_t listLinearize(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    Elem_t *new_data =(Elem_t *)calloc(list->capacity, sizeof(new_data[0]));

    CHECK_NULLPTR_ERROR(new_data, LIST_CANT_ALLOCATE_MEMORY)

    size_t pointer = list->data[0].next;

    for (size_t i = 1; i < list->size; i++)
    {
        new_data[i].next = 1 + i % (list->size - 1);
        new_data[i].prev = (list->size + i - 1) % list->size;
        if (i == 1)
            new_data[i].prev = list->size - 1;
        new_data[i].value = list->data[pointer].value;
        pointer = list->data[pointer].next;
    }

    new_data[0].next = 1;
    new_data[0].prev = list->size - 1;

    free(list->data);
    list->data = new_data;

    for (size_t i = list->size; i < list->capacity; i++)
    {
        list->data[i].next = (i + 1) % list->capacity;
        list->data[i].prev = (i - 1) % list->capacity;
        if (i == list->capacity - 1)
            list->data[i].next = list->size;
        list->data[i].value = POISONED_VALUE;
    }
    list->free = list->size;

    return LIST_NO_ERRORS;
}
