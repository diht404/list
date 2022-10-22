#ifndef LIST__LIST_H
#define LIST__LIST_H

#include "stdio.h"
#include <cstdlib>

typedef int Val_t;

const Val_t POISONED_VALUE = -7;

struct Elem_t
{
    Val_t value = POISONED_VALUE;
    size_t prev =  0;
    size_t next =  0;
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

enum ERRORS
{
    NO_ERRORS             = 0,
    LIST_IS_NULLPTR       = 1,
    LIST_DATA_NOT_NULLPTR = 2,
    CANT_ALLOCATE_MEMORY  = 3,
};

size_t ctorList(List *list, size_t size = 1);

size_t listDump(List *list);

void printElem_t(Val_t elem);

void printNextPrev(size_t value);

size_t resizeList(List *list);

size_t dtorList(List *list);

size_t listPushFront(List *list, Val_t value);

size_t listPushBack(List *list, Val_t value);

size_t listPush(List *list, Val_t value);

size_t listSetEmpty(List *list, Val_t value);
#endif //LIST__LIST_H
