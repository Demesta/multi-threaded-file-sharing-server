#ifndef SYSPRO2_DIR_LIST_H
#define SYSPRO2_DIR_LIST_H

#include <string>
using namespace std;

class Dir_countries;

typedef struct dirListNode
{
    Dir_countries* item;
    struct dirListNode* nextNode;
}DirListNode;

void insertListNode(Dir_countries*, DirListNode*&);
void printList(DirListNode *);


#endif //SYSPRO2_DIR_LIST_H
