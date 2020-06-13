
#ifndef SYSPRO2_HASHTABLE_H
#define SYSPRO2_HASHTABLE_H


#include <string>

typedef struct listNode ListNode;
class PatientRecord;

using namespace std;

class HashTable
{
public:
    HashTable();
    virtual void insertRecord(PatientRecord*, int) = 0;
    int hashFunction(string, int);
    void initializeHashTable(ListNode*, int) ;
};




#endif //SYSPRO2_HASHTABLE_H
