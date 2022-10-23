#ifndef LIST__LOGS_H
#define LIST__LOGS_H

#include "list.h"

void setLogFile(const char *filename);

void closeLogFile();

size_t listDump(List *list);

size_t listVerifier(List *list);

void logError(size_t error, FILE *fp);

void printElem_t(FILE *fp, Val_t elem);

void printNextPrev(FILE *fp, size_t value);

#endif //LIST__LOGS_H
