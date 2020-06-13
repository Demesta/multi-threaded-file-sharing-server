
#include "HashTable.h"
#include "PatientRecord.h"  //TODO na to bgalw
#include <iostream>  //TODO kai auto
#include <string>
#include <cstring>
#include "Records_list.h"

using namespace std;

HashTable::HashTable()
{

}

int HashTable::hashFunction(string s, int entries)   //unsigned long?
{
    int x = 0;
    int n = s.length();
    char char_array[n + 1];

    strcpy(char_array, s.c_str());

    for (int i = 0; i < n; i++)
        x =  x + char_array[i];

    int hash = 5381;
    int a=33;

    hash=(a*hash + x) % entries;

    return hash;
}

void HashTable::initializeHashTable(ListNode *L, int bucket_size)
{
    ListNode* N = L;
    while(N != nullptr)
    {
        this->insertRecord(N->item, bucket_size);
        N = N->nextNode;
    }
}
