#ifndef SYSPRO2_RECORDS_LIST_H
#define SYSPRO2_RECORDS_LIST_H

#include <string>
using namespace std;

class PatientRecord;

typedef struct listNode
{
    PatientRecord* item;
    struct listNode* nextNode;
}ListNode;

void insertListNode(PatientRecord*, ListNode*&);
void insertExitDate(string, ListNode*&);
void printList(ListNode *);
bool idExists(PatientRecord*, ListNode*);




#endif //SYSPRO2_RECORDS_LIST_H
