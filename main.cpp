#include "list_utils.h"
#include "logs.h"

int main()
{
//    setLogFile("logs.txt");
    List list = {};
    ctorList(&list, 1);
    listDump(&list, nullptr);
    listPush(&list, 67);
    listPush(&list, 13);
    listPush(&list, 31);
    listPush(&list, 26);
    listPush(&list, 42);
    listDump(&list, nullptr);
    listInsertAfter(&list, 2, 99);
    listInsertBefore(&list, 3, 66);
    listDump(&list, nullptr);
    listPopTail(&list);
    listDump(&list, nullptr);
    listPopHead(&list);
    listDump(&list, nullptr);
    size_t position = -7;
    listElemByIndex(&list, 1, &position);
    bool success = false;
    listFirstOccurrence(&list, 31, &position, &success);
    graphDump(&list);
    listLinearize(&list);
    listDump(&list, nullptr);
    graphDump(&list);
    dtorList(&list);
    closeLogFile();
    return 0;
}