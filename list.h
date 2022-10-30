#ifndef LIST__LIST_H
#define LIST__LIST_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "limits.h"

typedef int Val_t;

const Val_t POISONED_VALUE = 993;

struct Elem_t
{
    Val_t value = POISONED_VALUE;
    Elem_t *prev = nullptr;
    Elem_t *next = nullptr;
};

struct List
{
    Elem_t *dummy = nullptr;
    size_t size = 0;
    bool alive = false;
};

#define listHead(list) \
    (list)->dummy->next

#define listTail(list) \
    (list)->dummy->prev

enum LIST_ERRORS
{
    LIST_NO_ERRORS                           =  0,
    LIST_IS_NULLPTR                          =  1,
    LIST_DATA_NOT_NULLPTR                    =  2,
    LIST_DATA_IS_NULLPTR                     =  3,
    LIST_CANT_ALLOCATE_MEMORY                =  4,
    LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT  =  5,
    LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT =  6,
    LIST_TRIED_TO_POP_FROM_EMPTY_LIST        =  7,
    LIST_INTERSECT_ALIVE_AND_DEAD_ARRAYS     =  8,
    LIST_SIZE_MORE_THAN_CAPACITY             =  9,
    LIST_ALREADY_ALIVE                       = 10,
    LIST_NOT_ALIVE                           = 11,
    LIST_POSITION_IS_NULLPTR                 = 12,
    LIST_TRIED_TO_POP_DEAD_ELEMENT           = 13,
};

#define CHECK_NULLPTR_ERROR(value, error) \
    {                                     \
        assert(#value && (int)error);     \
        if (value == nullptr)             \
            return error;                 \
    }


#endif //LIST__LIST_H
