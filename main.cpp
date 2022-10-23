#include "list.h"

int main()
{
    List list = {};
    ctorList(&list, 16);
    listPush(&list, 69);
    listPush(&list, 13);
    listPush(&list, 31);
    listPush(&list, 26);
    listPush(&list, 42);
    listInsertAfter(&list, 2, 99);
    listInsertBefore(&list, 3, 66);
    listDump(&list);
    listPopTail(&list);
    listDump(&list);
    listPopHead(&list);
    listDump(&list);
    dtorList(&list);
    return 0;
}