
#include "Dir_list.h"
#include "Dir_countries.h"
#include <iostream>

using namespace std;

void insertListNode(Dir_countries* item, DirListNode *&L)
{
    DirListNode* N = new DirListNode;           //dhmiourgw enan neo komvo

    if(L == nullptr)   //if first node
    {
        N->item = item;
        N->nextNode = nullptr;
        L = N;   //N is the new first node of the list
    }
    else
    {
        N->item = item;
        N->nextNode = L;         //vazw ton neo komvo na deixnei ekei pou deixnei o prwtos komvos
        L = N;
    }
}

void printList(DirListNode *L)
{
    DirListNode *N = L;

    while(N != nullptr)
    {
        cout<<N->item->getCountryName()<<endl;
        N = N->nextNode;
    }
}