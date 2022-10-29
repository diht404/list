#include "list_utils.h"

size_t listCtor(List *list, size_t capacity)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    if (list->data != nullptr)
        return LIST_DATA_NOT_NULLPTR;

    if (list->alive)
        return LIST_ALREADY_ALIVE;

    if (capacity < 2)
        capacity = 2;

    list->data = (Elem_t *) calloc(capacity, sizeof(list->data[0]));

    CHECK_NULLPTR_ERROR(list->data, LIST_DATA_IS_NULLPTR)

    list->capacity = capacity;

    // set head, tail and free
    listTail(list) = list->capacity - 1;
    listHead(list) = 1;
    list->data[0].value = POISONED_VALUE;

    // create circle list
    for (size_t i = 1; i < list->capacity; i++)
    {
        list->data[i].next = 1 + i % (list->capacity - 1);
        list->data[i].prev = (list->capacity + i - 1) % list->capacity;
        list->data[i].value = POISONED_VALUE;
    }
    list->data[1].prev = list->capacity - 1;

    list->size = 1;
    list->free = 1;
    list->alive = true;

    return LIST_NO_ERRORS;
}

size_t listDtor(List *list)
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
        listResizeUp(list);
    }
    if (list->capacity > list->size * 4)
    {
        listResizeDown(list);
    }

    list->data[list->free].value = value;
    // get prev next free indexes
    size_t prev_free_index = list->data[list->free].prev;
    size_t next_free_index = list->data[list->free].next;

    // update prev next free elems
    list->data[prev_free_index].next = next_free_index;
    list->data[next_free_index].prev = prev_free_index;

    // update prev next elems
    list->data[listHead(list)].prev = list->free;
    list->data[listTail(list)].next = list->free;

    // set new elem params
    list->data[list->free].prev = listTail(list);
    list->data[list->free].next = listHead(list);
    list->data[list->free].alive = true;
    // set new free
    listTail(list) = list->free;
    // update free
    list->free = next_free_index;
    list->size += 1;

    return LIST_NO_ERRORS;
}

size_t listInsertBefore(List *list, size_t position, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT;

    if (list->capacity - list->size <= 1)
    {
        listResizeUp(list);
    }
    if (list->capacity > list->size * 4)
    {
        listResizeDown(list);
    }

    size_t position_to_insert_after = list->data[position].prev;

    return listInsertAfter(list, position_to_insert_after, value);
}

size_t listInsertAfter(List *list, size_t position, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT;

    if (list->capacity - list->size <= 1)
    {
        listResizeUp(list);
    }
    if (list->capacity > list->size * 4)
    {
        listResizeDown(list);
    }

    if (position == listTail(list))
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
    // update free
    list->free = next_free_index;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listPop(List *list, size_t position)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->data[position].alive)
        return LIST_TRIED_TO_POP_DEAD_ELEMENT;

    if (position == listTail(list))
        return listPopTail(list);

    if (position == listHead(list))
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

    // update free
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
    size_t prev_index = list->data[listTail(list)].prev;
    size_t next_index = listHead(list);

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[listTail(list)].alive = false;
    list->data[listTail(list)].value = POISONED_VALUE;
    list->data[listTail(list)].prev = prev_free_index;
    list->data[prev_free_index].next = listTail(list);
    list->data[listTail(list)].next = list->free;

    // update free
    list->free = listTail(list);
    // update tail
    listTail(list) = prev_index;

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
    size_t prev_index = listTail(list);
    size_t next_index = list->data[listHead(list)].next;

    // update prev next indexes
    list->data[prev_index].next = next_index;
    list->data[next_index].prev = prev_index;

    // update new free elem
    list->data[listHead(list)].alive = false;
    list->data[listHead(list)].value = POISONED_VALUE;
    list->data[listHead(list)].prev = prev_free_index;
    list->data[prev_free_index].next = listHead(list);
    list->data[listHead(list)].next = list->free;

    list->free = listHead(list);
    listHead(list) = next_index;

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listLinearize(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    Elem_t *new_data =
        (Elem_t *) calloc(list->capacity, sizeof(new_data[0]));

    CHECK_NULLPTR_ERROR(new_data, LIST_CANT_ALLOCATE_MEMORY)

    size_t pointer = listHead(list);

    // copy alive elements to new memory
    for (size_t i = 1; i < list->size; i++)
    {
        new_data[i].next = 1 + i % (list->size - 1);
        new_data[i].prev = (list->size + i - 1) % list->size;
        if (i == 1)
            new_data[i].prev = list->size - 1;
        new_data[i].value = list->data[pointer].value;
        pointer = list->data[pointer].next;
        new_data[i].alive = true;
    }

    // update dummy element
    new_data[0].next = 1;
    new_data[0].prev = list->size - 1;
    new_data[0].value = POISONED_VALUE;

    free(list->data);
    list->data = new_data;

    // set not alive elements
    for (size_t i = list->size; i < list->capacity; i++)
    {
        list->data[i].next = (i + 1) % list->capacity;
        list->data[i].prev = (i - 1) % list->capacity;
        // set next for last free element
        if (i == list->capacity - 1)
            list->data[i].next = list->size;
        // set prev for first free element
        if (i == list->size)
            list->data[i].prev = list->capacity - 1;
        list->data[i].value = POISONED_VALUE;
    }
    // update free
    list->free = list->size;

    return LIST_NO_ERRORS;
}

size_t listResizeUp(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    size_t old_capacity = list->capacity;
    size_t new_capacity = list->capacity * 2;

    Elem_t *new_data = (Elem_t *) realloc(list->data,
                                          sizeof(Elem_t)
                                              * new_capacity);

    CHECK_NULLPTR_ERROR(new_data, LIST_CANT_ALLOCATE_MEMORY)

    list->data = new_data;

    list->capacity = new_capacity;
    // set new free elements
    for (size_t i = old_capacity; i < list->capacity; i++)
    {
        list->data[i].next = (i + 1) % list->capacity;
        list->data[i].prev = (i - 1) % list->capacity;
        list->data[i].value = POISONED_VALUE;
    }
    // get last free index before resize
    size_t prev_free_index = list->data[list->free].prev;

    // update last free element before resize
    list->data[prev_free_index].next = old_capacity;
    // update first free element in additional block
    list->data[old_capacity].prev = prev_free_index;

    // update last free index after resize
    list->data[list->capacity - 1].next = list->free;
    // update last free index before resize
    list->data[list->free].prev = list->capacity - 1;
    return LIST_NO_ERRORS;
}

size_t listResizeDown(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    size_t num_free = 0;
    for (size_t i = list->capacity - 1; i != 0; i--)
    {
        if (list->data[i].alive)
            break;
        else
            num_free++;
    }
    size_t new_capacity = max(list->capacity / 2,
                              list->capacity - num_free + 1);

    // get first element, that will be deleted
    Elem_t first_deleted_element = list->data[new_capacity];

    Elem_t *new_data = (Elem_t *) realloc(list->data,
                                          sizeof(Elem_t)
                                              * new_capacity);

    CHECK_NULLPTR_ERROR(new_data, LIST_CANT_ALLOCATE_MEMORY)

    list->data = new_data;

    // update new last free elemtn
    list->data[first_deleted_element.prev].next = list->free;
    // update free pointer
    list->data[list->free].prev = first_deleted_element.prev;

    list->capacity = new_capacity;

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
    size_t check_position = listHead(list);

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

    size_t check_position = listHead(list);

    for (size_t i = 0; i < index % list->size; i++)
    {
        check_position = list->data[check_position].next;
    }
    *position = check_position;

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

    return listHead(list);
}

size_t listBack(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return 0;
    }

    return listTail(list);
}

size_t max(size_t lhs, size_t rhs)
{
    if (lhs > rhs)
        return lhs;
    return rhs;
}
