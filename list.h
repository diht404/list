#ifndef LIST__LIST_H
#define LIST__LIST_H

#include "stdio.h"
#include "stdlib.h"

typedef int Val_t;

const Val_t POISONED_VALUE = -7;

struct Elem_t
{
    Val_t value = POISONED_VALUE;
    size_t prev = 0;
    size_t next = 0;
    bool alive = false;
};

struct List
{
    Elem_t *data = nullptr;
    size_t head = 0;
    size_t tail = 0;
    size_t free = 0;
    size_t size = 0;
    size_t capacity = 0;
    bool alive = false;
};

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
    POSITION_IS_NULLPTR                      = 12,
};

#endif //LIST__LIST_H
