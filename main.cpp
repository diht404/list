#include "list_utils.h"
#include "logs.h"

int main()
{
    clearGraphLogFile();
//    setLogFile("logs.txt");
    List list = {};
    listCtor(&list);
    listDump(&list, nullptr);
    listPush(&list, 67);
    listPush(&list, 13);
    listPush(&list, 31);
    graphDump(&list);
    listPush(&list, 26);
    listPushFront(&list, 42);
    graphDump(&list);
    listPushFront(&list, 32);
    graphDump(&list);
    listPopHead(&list);
    graphDump(&list);
    listPopTail(&list);
    graphDump(&list);
    listDtor(&list);
    closeLogFile();
    return 0;
}