#ifndef LIST__LIST_UTILS_H
#define LIST__LIST_UTILS_H

#include "list.h"

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

/**
 * @brief finds if list first occurrence of value
 *
 * @param list list to find
 * @param value value ot fina
 * @param position index of first occurrence of value
 * @param success true if exist, false if not exist
 * @return error code
 */
size_t listFirstOccurrence(List *list,
                           Val_t value,
                           size_t *position,
                           bool *success);

/**
 * @brief searches element by index
 *
 * @param list list to search
 * @param index index to search in array
 * @param position position of index
 * @return error code
 */
size_t listElemByIndex(List *list,
                       size_t index,
                       size_t *position);

/**
 * @brief linearizes list
 *
 * @param list list to linearize
 * @return error code
 */
size_t listLinearize(List *list);

#endif //LIST__LIST_UTILS_H
