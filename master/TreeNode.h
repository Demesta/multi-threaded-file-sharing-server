//
// Created by demesta on 26/5/20.
//

#ifndef SYSPRO2_TREENODE_H
#define SYSPRO2_TREENODE_H

#include <string>
class PatientRecord;

using namespace std;

typedef int Item;

typedef struct node
{
    struct node *left = nullptr, *right= nullptr;
    Item data;
    int height = 0;
    string id;   //id will be the entry date
    PatientRecord* patientRecord;
} TreeNode;

#endif //SYSPRO2_TREENODE_H
