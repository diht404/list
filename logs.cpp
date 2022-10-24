#include "logs.h"

FILE *LIST_LOG_FILE = stderr;

void setLogFile(const char *filename)
{
    if (filename == nullptr)
        return;

    FILE *fp = fopen(filename, "w");
    if (fp == nullptr)
        return;

    LIST_LOG_FILE = fp;
    setvbuf(LIST_LOG_FILE, nullptr, _IONBF, 0);
}

void closeLogFile()
{
    if (LIST_LOG_FILE != nullptr)
        fclose(LIST_LOG_FILE);
}

size_t listDump(List *list)
{
    if (list == nullptr)
        return LIST_CANT_ALLOCATE_MEMORY;

    if (list->data == nullptr)
        return LIST_DATA_IS_NULLPTR;

    logError(listVerifier(list), LIST_LOG_FILE);

    fprintf(LIST_LOG_FILE, "DATA: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printElem_t(LIST_LOG_FILE, list->data[i].value);
    }
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "NEXT: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(LIST_LOG_FILE, list->data[i].next);
    }
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "IND:  ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(LIST_LOG_FILE, i);
    }
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "PREV: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(LIST_LOG_FILE, list->data[i].prev);
    }
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "HEAD: ");
    printSize_t(LIST_LOG_FILE, list->head);
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "TAIL: ");
    printSize_t(LIST_LOG_FILE, list->tail);
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "FREE: ");
    printSize_t(LIST_LOG_FILE, list->free);
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "SIZE: ");
    printSize_t(LIST_LOG_FILE, list->size);
    fprintf(LIST_LOG_FILE, "\n");

    fprintf(LIST_LOG_FILE, "CAPACITY: ");
    printSize_t(LIST_LOG_FILE, list->size);
    fprintf(LIST_LOG_FILE, "\n");
    return LIST_NO_ERRORS;
}

size_t listVerifier(List *list)
{
    if (list == nullptr)
        return LIST_IS_NULLPTR;

    if (list->data == nullptr)
        return LIST_DATA_IS_NULLPTR;

    if (list->size > list->capacity)
        return LIST_SIZE_MORE_THAN_CAPACITY;

    if (!list->alive)
        return LIST_NOT_ALIVE;

    size_t position = list->free;
    size_t num_verified = 0;
    do
    {
        if (list->data[position].alive)
            return LIST_INTERSECT_ALIVE_AND_DEAD_ARRAYS;
        position = list->data[position].next;
        num_verified++;
    }
    while (position == list->free and num_verified < list->size);

    return LIST_NO_ERRORS;
}

void logError(size_t error, FILE *fp)
{
    if (!error)
        return;

    switch (error)
    {
        case LIST_IS_NULLPTR:
        {
            fprintf(fp, "List is nullptr.\n");
            break;
        }
        case LIST_DATA_NOT_NULLPTR:
        {
            fprintf(fp,
                    "Tried to run constructor. List data is not nullptr.\n");
            break;
        }
        case LIST_DATA_IS_NULLPTR:
        {
            fprintf(fp,
                    "List is nullptr.\n");
            break;
        }
        case LIST_CANT_ALLOCATE_MEMORY:
        {
            fprintf(fp,
                    "Can't allocate memory for list data.\n");
            break;
        }
        case LIST_TRIED_TO_INSERT_AFTER_DEAD_ELEMENT:
        {
            fprintf(fp,
                    "Tried to insert to list after dead element.\n");
            break;
        }
        case LIST_TRIED_TO_INSERT_BEFORE_DEAD_ELEMENT:
        {
            fprintf(fp,
                    "Tried to insert to list before dead element.\n");
            break;
        }
        case LIST_TRIED_TO_POP_FROM_EMPTY_LIST:
        {
            fprintf(fp,
                    "Tried to pop from empty list.\n");
            break;
        }
        case LIST_INTERSECT_ALIVE_AND_DEAD_ARRAYS:
        {
            fprintf(fp,
                    "List arrays of dead and alive elements are intersected.\n");
            break;
        }
        case LIST_SIZE_MORE_THAN_CAPACITY:
        {
            fprintf(fp,
                    "List size more than capacity.\n");
            break;
        }
        case LIST_ALREADY_ALIVE:
        {
            fprintf(fp,
                    "Tried to construct list. List already alive.\n");
            break;
        }
        case LIST_NOT_ALIVE:
        {
            fprintf(fp,
                    "List not alive. Can't work with list.\n");
            break;
        }
        default:
        {
            fprintf(fp,
                    "Unknown error code: %zu.\n", error);
            break;
        }
    }
}

void printElem_t(FILE *fp, Val_t elem)
{
    fprintf(fp, "%2d ", elem);
}

void printSize_t(FILE *fp, size_t value)
{
    fprintf(fp, "%2zu ", value);
}