#include "Records_list.h"
#include "utils.h"
#include "PatientRecord.h"
#include <iostream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

void insertListNode(PatientRecord* item, ListNode *&L)
{
    ListNode* N = new ListNode;           //dhmiourgw enan neo komvo

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

void printList(ListNode *L)
{
    ListNode *N = L;

    while(N != nullptr)
    {
        N->item->printPatientRecord();
        N = N->nextNode;
    }
}

bool idExists(PatientRecord *record, ListNode *L)
{
    ListNode *N = L;

    while(N != nullptr)
    {
        if(N->item->GetRecordId() == record->GetRecordId())
        {
            cout<<"Error! One record already exists\n";
            return true;
        }
        N = N->nextNode;
    }

    return false;
}

//void insertExitDate(string s, ListNode *&L)     //FROM HOMEWORK 1
//{
//    string id;  //extract id and date from user's choice passed through string
//    string exit_date;
//
//    //code to cut string to substrings
//    string token;
//    stringstream tokenize(s);
//    string elements[3];
//
//    int i = 0;
//    while(getline(tokenize, token, ' '))
//    {
//        elements[i] = token;
//        i++;
//    }
//    id = elements[1];
//    exit_date = elements[2];
//
//    while(L != nullptr)  //search every node of list
//    {
//        if(L->item->GetRecordId() == id)  //record has been found
//        {
//            if(ReverseDateFormat(L->item->GetEntryDate()) > ReverseDateFormat(exit_date))   //if dates are not correct
//            {
//                cout<<"Error! Exit date can no be before entry date\n";
//                return;
//            }
//
//            L->item->setExitDate(exit_date);    //set exit date with the one given by the user
//
//            //cout<<"Exit date has been added. The record's data are now the following:\n";
//            //L->item->printPatientRecord();
//            cout<<"Record updated\n";
//            return;
//        }
//        L = L->nextNode;
//    }
//
//    //if code gets this far, the record does not exist
//    cout<<"Not found\n";
//    return;
//}
