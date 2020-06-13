//
// Created by demesta on 26/5/20.
//

#ifndef SYSPRO2_AVLTREE_H
#define SYSPRO2_AVLTREE_H


#include <string>
using namespace std;

class PatientRecord;

typedef struct node TreeNode;

class AVLTree
{
private:
    TreeNode* root ;
    int record_counter; //holds the number of records that this tree has

public:
    AVLTree();
    TreeNode* insertTreeNode(TreeNode*, PatientRecord*);
    TreeNode* rightRotation(TreeNode*);
    TreeNode* leftRotation(TreeNode*);
    int findHeight(TreeNode*);
    int balance(TreeNode*);
    void setRoot(TreeNode *root);
    TreeNode *getRoot() const;
    int getRecordCounter() const;
    int countHospitalizedPatients(TreeNode*);
    void printHospitalizedPatients(TreeNode*);
    int countBetweenDates(TreeNode*, string, string);
    int countRecordsForCountry(TreeNode*, string, string, string);
    int countAgeRange(TreeNode*, string, string, string, int, int);
    int countAges(TreeNode*, int, int);




};


#endif //SYSPRO2_AVLTREE_H
