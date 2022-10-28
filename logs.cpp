#include "logs.h"

FILE *LIST_LOG_FILE = stderr;
const char *LIST_GRAPH_LOG_FILENAME = "logs";

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
    while (num_verified < list->capacity - list->size);

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

void graphDump(List *list)
{
    char filename[128] = "";
    char photo_name[128] = "";

    sprintf(filename,
            "%s_v.%zu.html",
            LIST_GRAPH_LOG_FILENAME,
            LIST_GRAPH_LOG_VERSION);
    sprintf(photo_name,
            "%s_v.%zu.jpg",
            LIST_GRAPH_LOG_FILENAME,
            LIST_GRAPH_LOG_VERSION);

    FILE *fp = fopen(filename, "w");
    if (fp == nullptr)
        return;

    fprintf(fp, "<pre>\n");
    listDump(list, fp);

    createGraph(list, photo_name);

    fprintf(fp, "<img height=200 src=%s />", photo_name);

    fclose(fp);
    LIST_GRAPH_LOG_VERSION++;
}

void createGraph(List *list, const char *filename)
{

    FILE *fp = fopen(GRAPH_FILENAME, "w");
    fprintf(fp, "digraph LIST {\n"
                "    rankdir=LR;\n");

    // create dummy node
    fprintf(fp, "    node_%zu[shape=\"record\", \n"
                "        color=%s, \n"
                "        style=\"rounded, filled\", \n"
                "        label=\"DUMMY ELEMENT | \n"
                "            VALUE = %d | \n"
                "            {{TAIL | %zu} | {INDEX | %zu} | {HEAD | %zu}}\"\n"
                "    ]\n", 0,
            PURPLE_COLOR,
            list->data[0].value,
            list->data[0].prev,
            0,
            list->data[0].next);

    // create head, tail, free nodes
    fprintf(fp, "    head[shape=\"record\", "
                "        color=%s,"
                "        style=\"rounded, filled\","
                "        label = \""
                "HEAD\"];\n", BLUE_COLOR);
    fprintf(fp, "    tail[shape=\"record\", "
                "        color=%s,"
                "        style=\"rounded, filled\","
                "        label = \""
                "TAIL\"];\n", BLUE_COLOR);
    fprintf(fp, "    free[shape=\"record\", "
                "        color=%s,"
                "        style=\"rounded, filled\","
                "        label = \""
                "FREE\"];\n", BLUE_COLOR);

    // create data nodes
    for (size_t i = 1; i < list->capacity; i++)
    {
        fprintf(fp,
                "    node_%zu[shape=\"record\", \n"
                "        color=%s, \n"
                "        style=\"rounded, filled\", \n"
                "        label=\"\n"
                "            VALUE = %d |\n"
                "            {{PREV | %zu} | {INDEX | %zu} | {NEXT | %zu}}\"]\n",
                i,
                list->data[i].alive ? GREEN_COLOR : RED_COLOR,
                list->data[i].value,
                list->data[i].prev,
                i,
                list->data[i].next);
    }

    // lines from head, tail, free to their nodes
    fprintf(fp, "    head->node_%zu;\n", list->data[0].next);
    fprintf(fp, "    tail->node_%zu;\n", list->data[0].prev);
    fprintf(fp, "    free->node_%zu;\n", list->free);

    //lines from dummy node to head, tail, free
    fprintf(fp, "    node_0->head;\n");
    fprintf(fp, "    node_0->tail;\n");
    fprintf(fp, "    node_0->free;\n");


    // group data nodes
    fprintf(fp,
            "    subgraph cluster_data{style=filled;color=%s\n",
            LIGHT_GREEN_COLOR);
    fprintf(fp, "        head;\n");
    size_t head = list->data[0].next;
    size_t counter = 0;
    while (counter < list->size - 1)
    {
        fprintf(fp, "        node_%zu;\n", head);
        fprintf(fp,
                "    node_%zu->node_%zu;\n",
                head,
                list->data[head].next);
        head = list->data[head].next;
        counter++;
    }

    size_t tail = list->data[0].prev;
    counter = 0;
    while (counter < list->size - 1)
    {
        fprintf(fp, "        node_%zu;\n", tail);
        fprintf(fp,
                "    node_%zu->node_%zu[style=dashed];\n",
                tail,
                list->data[tail].prev);
        tail = list->data[tail].prev;
        counter++;
    }

    fprintf(fp, "        tail;\n");
    fprintf(fp, "    }\n");

    //group free node
    fprintf(fp,
            "    subgraph cluster_free{style=filled;color=%s\n",
            LIGHT_RED_COLOR);
    fprintf(fp, "        free;\n");
    size_t free_elem = list->free;
    for (size_t i = 0; i < list->capacity - list->size; i++)
    {
        fprintf(fp, "        node_%zu;\n", free_elem);
        fprintf(fp,
                "    node_%zu->node_%zu;\n",
                free_elem,
                list->data[free_elem].next);
        free_elem = list->data[free_elem].next;
    }
    for (size_t i = 0; i < list->capacity - list->size; i++)
    {
        fprintf(fp, "        node_%zu;\n", free_elem);
        fprintf(fp,
                "    node_%zu->node_%zu[style=dashed];\n",
                free_elem,
                list->data[free_elem].prev);
        free_elem = list->data[free_elem].prev;
    }
    fprintf(fp, "    }\n");

    // close graph with }
    fprintf(fp, "}\n");

    char command[128] = "";
    fclose(fp);
    sprintf(command, "dot %s -T jpg -o %s", GRAPH_FILENAME, filename);
    system(command);
//    sprintf(command, "rm %s", GRAPH_FILENAME);
//    system(command);
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