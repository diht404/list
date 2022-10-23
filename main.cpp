#include "list_utils.h"
#include "logs.h"

int main()
{
//    setLogFile("logs.txt");
//    TODO: find first occurrence, search by index
    List list = {};
    ctorList(&list, 1);
    listPush(&list, 69);
    listPush(&list, 13);
    listPush(&list, 31);
    listPush(&list, 26);
    listPush(&list, 42);
    listDump(&list);
    listInsertAfter(&list, 2, 99);
    listInsertBefore(&list, 3, 66);
    listDump(&list);
    listPopTail(&list);
    listDump(&list);
    listPopHead(&list);
    listDump(&list);
    dtorList(&list);
    closeLogFile();
    return 0;
}