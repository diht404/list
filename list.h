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

size_t ctorList(List *list, size_t size = 1);

size_t resizeList(List *list);

size_t dtorList(List *list);

size_t listPush(List *list, Val_t value);

size_t listPushFront(List *list, Val_t value);

size_t listPushBack(List *list, Val_t value);

size_t listPushEmpty(List *list, Val_t value);

size_t listPop(List *list, size_t position);

size_t listPopTail(List *list);

size_t listPopHead(List *list);

size_t listFront(List *list, size_t *error);

size_t listBack(List *list, size_t *error);

size_t listInsertAfter(List *list, size_t position, Val_t value);

size_t listInsertBefore(List *list, size_t position, Val_t value);

#endif //LIST__LIST_H
