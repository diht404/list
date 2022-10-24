#include "list_utils.h"
#include "logs.h"

int main()
{
//    setLogFile("logs.txt");
    List list = {};
    ctorList(&list, 1);
    listPush(&list, 67);
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
    size_t position = -7;
    listElemByIndex(&list, 1, &position);
    printf("__%zu__\n", position);
    bool success = false;
    listFirstOccurence(&list, 31, &position, &success);
    if (success)
        printf("{{%zu}}\n", position);
    dtorList(&list);
    closeLogFile();
    return 0;
}