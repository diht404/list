#include "list.h"

int main()
{
    List list = {};
    ctorList(&list, 16);
    listPush(&list, 69);
    listPush(&list, 13);
    listPush(&list, 42);
    listDump(&list);
    dtorList(&list);
    return 0;
}