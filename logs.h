#ifndef LIST__LOGS_H
#define LIST__LOGS_H

#include "list.h"

const char *const GRAPH_FILENAME    = "list_graph";
const char *const PURPLE_COLOR      = "\"#a66be8\"";
const char *const BLUE_COLOR        = "\"#6bcce8\"";
const char *const GREEN_COLOR       = "\"#6be871\"";
const char *const LIGHT_GREEN_COLOR = "\"#caf2d4\"";
const char *const RED_COLOR         = "\"#e87b6b\"";
const char *const LIGHT_RED_COLOR   = "\"#f2d3ca\"";

/**
 * @brief sets log file
 *
 * @param filename name of loge file
 */
void setLogFile(const char *filename);

/**
 * @brief closes log file
 */
void closeLogFile();

/**
 * @brief dumps list to log file
 *
 * @param list list to dump
 * @return error code
 */
size_t listDump(List *list, FILE *fp, bool color_output = false);

/**
 * @brief verifies list
 *
 * @param list list to verify
 * @return error code
 */
size_t listVerifier(List *list);

/**
 * @brief logs error code to file
 *
 * @param error error to log
 * @param fp file to log
 */
void logError(size_t error, FILE *fp);

/**
 * @brief creates graphical dump
 *
 * @param list list to dump
 */
void graphDump(List *list);

/**
 * @brief creates graph of list
 *
 * @param list list with graph
 * @param filename name of graph
 */
void createGraph(List *list, const char *filename);

/**
 * @brief prints element to file
 *
 * @param fp file to print
 * @param elem element to print
 */
void printElem_t(FILE *fp, Val_t elem);

/**
 * @brief prints size_t value to file with specific format
 *
 * @param fp file to print
 * @param value value to print
 */
void printSize_t(FILE *fp, size_t value);

#endif //LIST__LOGS_H
