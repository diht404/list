#ifndef LIST__LIST_H
#define LIST__LIST_H

#include "stdio.h"
#include <cstdlib>

typedef int Val_t;

const Val_t POISONED_VALUE = -7;

struct Elem_t
{
    Val_t value = POISONED_VALUE;
    size_t prev = 0;
    size_t next = 0;
    bool alive = 0;
};

struct List
{
    Elem_t *data = nullptr;
    size_t head = 0;
    size_t tail = 0;
    size_t free = 0;
    size_t size = 0;
    size_t capacity = 0;
};

enum LIST_ERRORS
{
    LIST_NO_ERRORS = 0,
    LIST_IS_NULLPTR = 1,
    LIST_DATA_NOT_NULLPTR = 2,
    LIST_DATA_IS_NULLPTR = 3,
    LIST_CANT_ALLOCATE_MEMORY = 4,
    LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT = 5,
    LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT = 6,
    LIST_TRIED_TO_POP_FROM_EMPTY_LIST = 7,
    LIST_INTERSECT_ALIVE_AND_DEAD_ARRAYS = 8,
    LIST_SIZE_MORE_THAN_CAPACITY = 9,
};

/**
 * @brief constructor for list
 *
 * @param list list to construct
 * @param size size of array of list
 * @return error code
 */
size_t ctorList(List *list, size_t size = 1);

/**
 * @brief resizes list to x2 capacity
 *
 * @param list list to resize
 * @return error code
 */
size_t resizeList(List *list);

/**
 * @brief destructs list
 *
 * @param list list to destruction
 * @return error code
 */
size_t dtorList(List *list);

/**
 * @brief pushes value to list
 *
 * @param list list to push
 * @param value value to push
 * @return error code
 */
size_t listPush(List *list, Val_t value);

/**
 * @brief pushes value to from of list
 *
 * @param list list to push
 * @param value value to push
 * @return error code
 */
size_t listPushFront(List *list, Val_t value);

/**
 * @brief pushes value to back of list
 *
 * @param list list to push
 * @param value value to push
 * @return error code
 */
size_t listPushBack(List *list, Val_t value);

/**
 * @brief pushes to empty list
 *
 * @param list list to push
 * @param value value to push
 * @return error code
 */
size_t listPushEmpty(List *list, Val_t value);

/**
 * @brief pop from list
 *
 * @param list list to pop
 * @param position position to pop
 * @return error code
 */
size_t listPop(List *list, size_t position);

/**
 * @brief pop from tail of list
 *
 * @param list list to pop
 * @return error code
 */
size_t listPopTail(List *list);

/**
 * @brief pop from head of list
 *
 * @param list list to pop
 * @return error code
 */
size_t listPopHead(List *list);

/**
 * @brief returns front position
 *
 * @param list list to pop
 * @param error error code
 * @return front position
 */
size_t listFront(List *list, size_t *error);

/**
 * @brief returns back position
 *
 * @param list list to pop
 * @param error error code
 * @return back position
 */
size_t listBack(List *list, size_t *error);

/**
 * @brief inserts value to list after position
 *
 * @param list list to insert
 * @param position position to insert
 * @param value value to insert
 * @return error code
 */
size_t listInsertAfter(List *list, size_t position, Val_t value);

/**
 * @brief inserts value to list before position
 *
 * @param list list to insert
 * @param position position to insert
 * @param value value to insert
 * @return error code
 */
size_t listInsertBefore(List *list, size_t position, Val_t value);

#endif //LIST__LIST_H
