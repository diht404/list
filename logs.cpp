#include "logs.h"

FILE *LIST_LOG_FILE = stderr;
const char * LIST_GRAPH_LOG_FILENAME = "logs";

size_t LIST_GRAPH_LOG_VERSION = 0;

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

size_t listDump(List *list, FILE *fp)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(list->data, LIST_DATA_IS_NULLPTR)

    if (fp == nullptr)
        fp = LIST_LOG_FILE;

    logError(listVerifier(list), fp);

    fprintf(fp, "DATA: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printElem_t(fp, list->data[i].value);
    }
    fprintf(fp, "\n");

    fprintf(fp, "NEXT: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(fp, list->data[i].next);
    }
    fprintf(fp, "\n");

    fprintf(fp, "IND:  ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(fp, i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "PREV: ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printSize_t(fp, list->data[i].prev);
    }
    fprintf(fp, "\n");

    fprintf(fp, "HEAD:     ");
    printSize_t(fp, list->data[0].next);
    fprintf(fp, "\n");

    fprintf(fp, "TAIL:     ");
    printSize_t(fp, list->data[0].prev);
    fprintf(fp, "\n");

    fprintf(fp, "FREE:     ");
    printSize_t(fp, list->free);
    fprintf(fp, "\n");

    fprintf(fp, "SIZE:     ");
    printSize_t(fp, list->size);
    fprintf(fp, "\n");

    fprintf(fp, "CAPACITY: ");
    printSize_t(fp, list->capacity);
    fprintf(fp, "\n");
    return LIST_NO_ERRORS;
}

size_t listVerifier(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(list->data, LIST_DATA_IS_NULLPTR)

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

void graphLog(List *list)
{
    char filename[128] = "";
    char photo_name[128] = "";

    sprintf(filename, "%s_v.%zu.html", LIST_GRAPH_LOG_FILENAME, LIST_GRAPH_LOG_VERSION);
    sprintf(photo_name, "%s_v.%zu.jpg", LIST_GRAPH_LOG_FILENAME, LIST_GRAPH_LOG_VERSION);

    FILE *fp = fopen(filename, "w");
    if (fp == nullptr)
        return;

    fprintf(fp, "<pre>\n");
    listDump(list, fp);

    createGraph(list, photo_name);

    fprintf(fp, "<img width=200 src=%s />", photo_name);

    fclose(fp);
    LIST_GRAPH_LOG_VERSION++;
}

void createGraph(List *list, const char *filename)
{
    FILE *fp = fopen("graph", "w");
    fprintf(fp, "digraph LIST {\n");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(fp,
                "    node_%zu[shape=\"rectangle\", label=\""
                "    VALUE = %d, \n"
                "    PREV = %zu, \n"
                "    INDEX = %zu,\n"
                "    NEXT = %zu\"]\n",
                i,
                list->data[i].value,
                list->data[i].prev,
                i,
                list->data[i].next);
    }

    fprintf(fp, "    edge [style=invis, constraint=true];");
    for (size_t i = 0; i < list->capacity - 1; i++)
    {
        fprintf(fp,
                "    node_%zu->node_%zu[style=invis];\n",
                i,
                (i + 1) % list->capacity);
    }

    fprintf(fp, "    node_%zu->node_%zu;", list->data[0].prev, 0);
    fprintf(fp, "    node_%zu->node_%zu;", 0, list->data[0].next);


    fprintf(fp, "    edge [style=solid, constraint=true];");

    for (size_t i = 1; i < list->capacity; i++)
    {
        fprintf(fp,
                "    node_%zu->node_%zu\n",
                i,
                list->data[i].next);
    }
    fprintf(fp, "}\n");

    char command[128] = "";
    fclose(fp);
    sprintf(command, "dot graph -T jpg -o %s", filename);
    system(command);
//    system("rm graph");
}

void printElem_t(FILE *fp, Val_t elem)
{
    if (fp == nullptr)
        return;

    fprintf(fp, "%8d ", elem);
}

void printSize_t(FILE *fp, size_t value)
{
    if (fp == nullptr)
        return;

    fprintf(fp, "%8zu ", value);
}