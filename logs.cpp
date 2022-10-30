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

void clearGraphLogFile()
{
    char filename[128] = "";

    sprintf(filename,
            "%s.html",
            LIST_GRAPH_LOG_FILENAME);
    FILE *fp = fopen(filename, "w");
    if (fp == nullptr)
        return;
    fclose(fp);
}

void closeLogFile()
{
    if (LIST_LOG_FILE != nullptr)
        fclose(LIST_LOG_FILE);
}

size_t listDump(List *list, FILE *fp, bool color_output)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (fp == nullptr)
        fp = LIST_LOG_FILE;

    logError(listVerifier(list), fp);

    fprintf(fp, "DATA: ");
    Elem_t *position = list->dummy;
    for (size_t i = 1; i <= list->size; i++)
    {
        printElem_t(fp, position->next->value);
        position = position->next;
    }
    fprintf(fp, "\n");

    if (color_output)
        fprintf(fp, "<span style=\"color:green;\">");
    fprintf(fp, "NEXT: ");
    position = listHead(list);
    for (size_t i = 1; i <= list->size; i++)
    {
        printPointer(fp, position->next);
        position = position->next;
    }
    if (color_output)
        fprintf(fp, "</span>");
    fprintf(fp, "\n");

    if (color_output)
        fprintf(fp, "<span style=\"color:blue;\">");
    fprintf(fp, "IND:  ");
    position = listHead(list);
    for (size_t i = 1; i <= list->size; i++)
    {
        printPointer(fp, position);
        position = position->next;
    }
    if (color_output)
        fprintf(fp, "</span>");
    fprintf(fp, "\n");

    if (color_output)
        fprintf(fp, "<span style=\"color:red;\">");
    fprintf(fp, "PREV: ");
    position = listHead(list);
    for (size_t i = 1; i <= list->size; i++)
    {
        printPointer(fp, position->prev);
        position = position->next;
    }
    if (color_output)
        fprintf(fp, "</span>");
    fprintf(fp, "\n");

    fprintf(fp, "HEAD:     ");
    printPointer(fp, list->dummy->next);
    fprintf(fp, "\n");

    fprintf(fp, "TAIL:     ");
    printPointer(fp, list->dummy->prev);
    fprintf(fp, "\n");

    fprintf(fp, "SIZE:     ");
    printSize_t(fp, list->size);
    fprintf(fp, "\n");
    fprintf(fp, "\n");

    return LIST_NO_ERRORS;
}

size_t listVerifier(List *list)
{
    CHECK_NULLPTR_ERROR(list, LIST_IS_NULLPTR)

    if (!list->alive)
        return LIST_NOT_ALIVE;

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
            "%s.html",
            LIST_GRAPH_LOG_FILENAME);

    FILE *fp = fopen(filename, "a");
    if (fp == nullptr)
        return;

    fprintf(fp, "<pre>\n");
    listDump(list, fp, true);

    fprintf(fp, "Dump in logical order.\n");
    sprintf(photo_name,
            "%s_logical_order_v.%zu.jpg",
            LIST_GRAPH_LOG_FILENAME,
            LIST_GRAPH_LOG_VERSION);
    createGraph(list, photo_name);
    fprintf(fp, "<img height=200 src=%s />\n", photo_name);

    fclose(fp);
    LIST_GRAPH_LOG_VERSION++;
}

void createGraph(List *list, const char *filename)
{

    FILE *fp = fopen(GRAPH_FILENAME, "w");
    fprintf(fp, "digraph LIST {\n"
                "    rankdir=LR;\n");

    // create dummy node
    fprintf(fp,
            "    node_%p[shape=\"record\", \n"
            "        color=%s, \n"
            "        style=\"rounded, filled\", \n"
            "        label=\"DUMMY ELEMENT | \n"
            "            VALUE = %d | \n"
            "            {{TAIL | %p} | {INDEX | %p} | {HEAD | %p}}\"\n"
            "    ]\n",
            list->dummy,
            PURPLE_COLOR,
            list->dummy->value,
            list->dummy->prev,
            list->dummy,
            list->dummy->next);

    // create head, tail nodes
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

    // create data nodes
    Elem_t *head = listHead(list);
    size_t counter = 1;
    while (counter <= list->size)
    {
        fprintf(fp,
                "    node_%p[shape=\"record\", \n"
                "        color=%s, \n"
                "        style=\"rounded, filled\", \n"
                "        label=\"\n"
                "            VALUE = %d |\n"
                "            {{PREV | %p} | {INDEX | %p} | {NEXT | %p}}\"]\n",
                head,
                GREEN_COLOR,
                head->value,
                head->prev,
                head,
                head->next);
        head = head->next;
        counter++;
    }

    //lines from dummy node to head, tail
    fprintf(fp,
            "    node_%p->head->node_%p;\n",
            list->dummy,
            listHead(list));
    fprintf(fp,
            "    node_%p->tail->node_%p;\n",
            list->dummy,
            listTail(list));

    // group data nodes
    fprintf(fp,
            "    subgraph cluster_data{style=filled;color=%s\n",
            LIGHT_GREEN_COLOR);
    fprintf(fp, "        head;\n");
    head = listHead(list);
    counter = 0;
    while (counter < list->size - 1)
    {
        fprintf(fp, "        node_%p;\n", head);
        fprintf(fp,
                "    node_%p->node_%p;\n",
                head,
                head->next);
        head = head->next;
        counter++;
    }

    Elem_t *tail = list->dummy->prev;
    counter = 0;
    while (counter < list->size - 1)
    {
        fprintf(fp, "        node_%p;\n", tail);
        fprintf(fp,
                "    node_%p->node_%p[style=dashed];\n",
                tail,
                tail->prev);
        tail = tail->prev;
        counter++;
    }

    fprintf(fp, "        tail;\n");
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

    fprintf(fp, "%16d ", elem);
}

void printSize_t(FILE *fp, size_t value)
{
    if (fp == nullptr)
        return;

    fprintf(fp, "%16zu ", value);
}

void printPointer(FILE *fp, Elem_t *value)
{
    if (fp == nullptr)
        return;

    fprintf(fp, "%16p ", value);
}