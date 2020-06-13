#include <iostream>
#include "AVLTree.h"
#include "PatientRecord.h"
#include "TreeNode.h"
#include "utils.h"

using namespace std;

AVLTree::AVLTree()
{
    root = nullptr ;
    record_counter = 0;
}

int AVLTree::findHeight(TreeNode *r)
{
    if(r == nullptr)
    {
        return 0;
    }
    int h1=0, h2=0, h=0;

    if(r->left == nullptr)   //if left child doesnt exist
        h1 = 0;
    else
        h1 = r->left->height;
    if(r->right == nullptr)   //if right child doesnt exist
        h2 = 0;
    else
        h2 = r->right->height;
    if(h1 >= h2)      //keeps the biggest height
        h = h1+1;    //+1 for its own height
    else if(h2 > h1)
        h = h2+1;

    return h;
}

int AVLTree::balance(TreeNode *r)
{
    if(r == nullptr)
    {
        return 0;
    }
    int b = (findHeight(r->left)) - (findHeight(r->right));
    return b;
}

TreeNode * AVLTree::rightRotation(TreeNode *r)
{
    TreeNode *x= nullptr, *temp = nullptr;
    x = r->left;
    temp = x->right;
    //Rotation:
    x->right = r;
    r->left = temp;

    //find new height for r and x
    r->height = findHeight(r);
    x->height = findHeight(x);

    return x;
}

TreeNode * AVLTree::leftRotation(TreeNode *r)
{
    TreeNode *x = nullptr, *temp = nullptr;
    x = r->right;
    temp = x->left;

    //Rotation:
    x->left = r;
    r->right = temp;

    //find new height for r and x
    r->height = findHeight(r);
    x->height = findHeight(x);

    return x;
}

TreeNode* AVLTree::insertTreeNode(TreeNode *r, PatientRecord *record)
{
    if(r == nullptr)  //then new node will be the root
    {
        auto* node = new TreeNode();
        node->id = record->GetDate();
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        node->patientRecord = record;

        this->record_counter++;

        return node;
    }

    if(ReverseDateFormat(record->GetDate()) < ReverseDateFormat(r->id)) //reverse in order to compare the dates
    {
        r->left = insertTreeNode(r->left, record);
    }
    else if(ReverseDateFormat(record->GetDate()) > ReverseDateFormat(r->id))
    {
        r->right = insertTreeNode(r->right, record);
    }
    else   //same dates
    {
        if(record->GetRecordId() == r->patientRecord->GetRecordId())   //same record
        {
            return r;
        }
        else
        {
            r->left = insertTreeNode(r->left, record);
        }
    }

    //update height
    r->height = findHeight(r);

    //check balance:
    int b = balance(r);

    if(b > 1 && ReverseDateFormat(record->GetDate()) < ReverseDateFormat(r->left->id))  //left left
    {
        return rightRotation(r);
    }
    if(b < -1 && ReverseDateFormat(record->GetDate()) > ReverseDateFormat(r->right->id)) //right right
    {
        return leftRotation(r);
    }

    if(b > 1 && ReverseDateFormat(record->GetDate()) > ReverseDateFormat(r->left->id)) //left right
    {
        r->left = leftRotation(r->left);
        return rightRotation(r);
    }

    if(b < -1 && ReverseDateFormat(record->GetDate()) < ReverseDateFormat(r->right->id)) //right left
    {
        r->right = rightRotation(r->right);
        return leftRotation(r);
    }

    return r;

}

int AVLTree::countAges(TreeNode *root, int age1, int age2)
{
    int counter = 0;

    if(root != nullptr)
    {
        counter += countAges(root->left, age1, age2);  //inorder

        if(stoi(root->patientRecord->getAge()) >= age1 && stoi(root->patientRecord->getAge()) <= age2)  //then the patient is of the age we check
        {
            counter++;
        }

        counter += countAges(root->right, age1, age2);
    }

    return counter;

}

//void AVLTree::printHospitalizedPatients(TreeNode* root)
//{
//    if(root != nullptr)
//    {
//        printHospitalizedPatients(root->left);  //inorder
//
//        if(root->patientRecord->GetExitDate() == "-")  //then the patient is still hospitalized
//        {
//            root->patientRecord->printPatientRecord();
//        }
//
//        printHospitalizedPatients(root->right);
//    }
//}

//int AVLTree::countHospitalizedPatients(TreeNode *root)
//{
//    int counter = 0;
//
//    if(root != nullptr)
//    {
//        counter += countHospitalizedPatients(root->left);  //inorder
//
//        if(root->patientRecord->GetExitDate() == "-")  //then the patient is still hospitalized
//        {
//            counter++;
//        }
//
//        counter += countHospitalizedPatients(root->right);
//    }
//
//    return counter;
//
//
//}

int AVLTree::countBetweenDates(TreeNode *root, string date1, string date2)
{
    int counter = 0;

    if(root != nullptr)
    {
        counter += countBetweenDates(root->left, date1, date2);  //inorder
        string record_date = ReverseDateFormat(root->id);

        if(record_date >= date1 && record_date <= date2)  //then the record is between the 2 dates
        {
            counter++;
        }

        counter += countBetweenDates(root->right, date1, date2);
    }

    return counter;
}

int AVLTree::countAgeRange(TreeNode *root, string virus, string date1, string date2, int age1, int age2)
{
    int counter = 0;

    if(root != nullptr)
    {
        counter += countRecordsForCountry(root->left, virus, date1, date2);  //inorder
        string record_date = ReverseDateFormat(root->patientRecord->GetDate());

        if(record_date >= date1 && record_date <= date2)  //then the record is between the 2 dates
        {
            if(root->patientRecord->getDiseaseId() == virus)  //then the record is for this virus
            {
                if(stoi(root->patientRecord->getAge()) >= age1 && stoi(root->patientRecord->getAge()) <= age2)
                {
                    counter++;
                }
            }
        }

        counter += countRecordsForCountry(root->right, virus, date1, date2);
    }

    return counter;

}

int AVLTree::countRecordsForCountry(TreeNode *root, string virus, string date1, string date2)
{
    int counter = 0;

    if(root != nullptr)
    {
        counter += countRecordsForCountry(root->left, virus, date1, date2);  //inorder
        string record_date = ReverseDateFormat(root->patientRecord->GetDate());

        if(record_date >= date1 && record_date <= date2)  //then the record is between the 2 dates
        {
            if(root->patientRecord->getDiseaseId() == virus)  //then the record is for this virus
            {
                counter++;
            }
        }

        counter += countRecordsForCountry(root->right, virus, date1, date2);
    }

    return counter;
}


TreeNode *AVLTree::getRoot() const {
    return root;
}

void AVLTree::setRoot(TreeNode *root) {
    AVLTree::root = root;
}

int AVLTree::getRecordCounter() const
{
    return record_counter;
}