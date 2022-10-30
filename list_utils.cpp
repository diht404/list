#include "list_utils.h"

size_t listCtor(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->alive)
        return LIST_ALREADY_ALIVE;

    list->dummy = (Elem_t *) calloc(1, sizeof(list->dummy[0]));

    CHECK_NULLPTR_ERROR(list->dummy, LIST_DATA_IS_NULLPTR)

    listHead(list) = nullptr;
    listTail(list) = nullptr;

    list->dummy->value = 0;
    list->size = 0;
    list->alive = true;

    return LIST_NO_ERRORS;
}

size_t listDtor(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(list->dummy, LIST_DATA_IS_NULLPTR)

    Elem_t *position = listHead(list);
    Elem_t *next_position = nullptr;
    for (size_t i = 0; i < list->size; i++)
    {
        next_position = position->next;
        free(position);
        position = next_position;
    }
    free(list->dummy);

    list->alive = false;

    return LIST_NO_ERRORS;
}

size_t listPushFront(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return listPushEmpty(list, value);

    Elem_t *new_element = (Elem_t *) calloc(1, sizeof(new_element[0]));
    CHECK_NULLPTR_ERROR(new_element, LIST_CANT_ALLOCATE_MEMORY)

    new_element->value = value;
    new_element->prev = listTail(list);
    new_element->next = listHead(list);

    listHead(list)->prev = new_element;
    listTail(list)->next = new_element;

    listHead(list) = new_element;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listPushBack(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return listPushEmpty(list, value);

    Elem_t *new_element = (Elem_t *) calloc(1, sizeof(Elem_t));
    CHECK_NULLPTR_ERROR(new_element, LIST_CANT_ALLOCATE_MEMORY)

    new_element->value = value;
    new_element->prev = listTail(list);
    new_element->next = listHead(list);

    listHead(list)->prev = new_element;
    listTail(list)->next = new_element;

    listTail(list) = new_element;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listPush(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return listPushEmpty(list, value);

    return listPushBack(list, value);
}

size_t listPushEmpty(List *list, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    Elem_t *new_element = (Elem_t *) calloc(1, sizeof(Elem_t));
    CHECK_NULLPTR_ERROR(new_element, LIST_CANT_ALLOCATE_MEMORY)

    new_element->value = value;
    new_element->prev = new_element;
    new_element->next = new_element;

    listHead(list) = new_element;
    listTail(list) = new_element;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listInsertBefore(List *list, Elem_t *position, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    Elem_t *position_to_insert_after = position->prev;

    return listInsertAfter(list, position_to_insert_after, value);
}

size_t listInsertAfter(List *list, Elem_t *position, Val_t value)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (position == listTail(list))
        return listPushBack(list, value);

    Elem_t *new_element = (Elem_t *) calloc(1, sizeof(new_element[0]));
    CHECK_NULLPTR_ERROR(new_element, LIST_CANT_ALLOCATE_MEMORY)

    new_element->value = value;
    new_element->next = position->next;
    new_element->prev = position;

    position->next = new_element;
    position->next->prev = new_element;

    list->size++;

    return LIST_NO_ERRORS;
}

size_t listPop(List *list, Elem_t *position)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (position == listTail(list))
        return listPopTail(list);

    if (position == listHead(list))
        return listPopHead(list);

    position->prev->next = position->next;
    position->next->prev = position->prev;

    free(position);
    list->size--;

    return LIST_NO_ERRORS;
}

size_t listPopTail(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    Elem_t *tail = listTail(list);

    tail->prev->next = listHead(list);
    tail->next->prev = tail->prev;

    listTail(list) = tail->prev;

    free(tail);

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listPopHead(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (list->size == 0)
        return LIST_TRIED_TO_POP_FROM_EMPTY_LIST;

    Elem_t *head = listHead(list);

    head->next->prev = listTail(list);
    listTail(list)->next = head->next;

    listHead(list) = head->next;

    free(head);

    list->size--;

    return LIST_NO_ERRORS;
}

size_t listFirstOccurrence(List *list,
                           Val_t value,
                           Elem_t **position,
                           bool *success)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(position, LIST_POSITION_IS_NULLPTR)

    size_t num_checked = 0;
    Elem_t *check_position = listHead(list);

    while (num_checked < list->size)
    {
        if (check_position->value == value)
        {
            *position = check_position;
            *success = true;
            return LIST_NO_ERRORS;
        }
        check_position = check_position->next;
        num_checked++;
    }
    *success = false;
    return LIST_NO_ERRORS;
}

size_t listElemByIndex(List *list,
                       size_t index,
                       Elem_t **position)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(position, LIST_POSITION_IS_NULLPTR)

    Elem_t *check_position = listHead(list);

    for (size_t i = 0; i < index % list->size; i++)
    {
        check_position = check_position->next;
    }
    *position = check_position;

    return LIST_NO_ERRORS;
}

Elem_t *listFront(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return nullptr;
    }

    return listHead(list);
}

Elem_t *listBack(List *list, size_t *error)
{
    if (list == nullptr)
    {
        if (error)
            *error = LIST_IS_NULLPTR;
        return nullptr;
    }

    return listTail(list);
}
