#include <iostream>
#include <unistd.h>
#include <string.h>
#include "diseaseHashTable.h"
#include "utils.h"
#include "TreeNode.h"
#include "AVLTree.h"
#include "Bucket.h"
#include "PatientRecord.h"
#include "Records_list.h"
#include "sockets.h"

diseaseHashTable::diseaseHashTable(int dhs_num_entries)
{
    dht_num_entries = dhs_num_entries;

    bucket_array = new Bucket *[dht_num_entries];

    for (int i = 0; i < dht_num_entries; i++)
    {
        bucket_array[i] = nullptr;
    }
}

diseaseHashTable::~diseaseHashTable()
{
    for (int i = 0; i < dht_num_entries; i++)
    {
        delete bucket_array[i];   //free every bucket
    }
    delete bucket_array;
}

void diseaseHashTable::statistics(int socket)
{
    for (int i = 0; i < this->dht_num_entries; i++)  //search the whole hash table
    {
        Bucket *it = bucket_array[i];

        while (it != nullptr)   //search every bucket
        {
            for (int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if (it->tree_array[j] != nullptr)
                {

                    TreeNode *N = it->tree_array[j]->getRoot();
                    string disease =
                        N->patientRecord->getDiseaseId();  //will be true for the whole tree as it is a tree based on same disease
                    string country =
                        N->patientRecord->getCountry();  //will be true for the whole tree as the whole table is for one file
                    string date = N->patientRecord->GetDate(); //same

                    int ages_20 = it->tree_array[j]->countAges(N, 0, 20);
                    int ages_40 = it->tree_array[j]->countAges(N, 21, 40);
                    int ages_60 = it->tree_array[j]->countAges(N, 41, 60);
                    int ages_60plus = it->tree_array[j]->countAges(N, 60, 150);


                    socket_write_string(socket, disease);
                    socket_write_int(socket, ages_20);
                    socket_write_int(socket, ages_40);
                    socket_write_int(socket, ages_60);
                    socket_write_int(socket, ages_60plus);

                    cout<<disease<<endl<<ages_20<<endl<<ages_40<<endl<<ages_60<<endl<<ages_60plus<<endl;
                }
            }
            it = it->next_bucket;
        }
    }

    socket_write_string(socket, "/Done");
}

bool diseaseHashTable::treeExists(AVLTree *tree, PatientRecord *record)
{
    TreeNode *tree_node = tree->getRoot();    //i only need to check the disease of the root's record
    string check_disease = tree_node->patientRecord->getDiseaseId();

    if (check_disease == record->getDiseaseId())
    {
        return true; //tree for this disease exists
    }

    return false;
}

void diseaseHashTable::insertRecord(PatientRecord *record, int bucket_size)
{
    int key = hashFunction(record->getDiseaseId(), this->dht_num_entries);    //generate key for new record

    if (this->bucket_array[key] == nullptr)     //case: first record with this key
    {
        auto *new_bucket = initializeBucket(bucket_size);   //create new Bucket

        new_bucket->next_bucket = nullptr;

        this->bucket_array[key] = new_bucket;

        auto *new_tree = new AVLTree;
        new_bucket->tree_array[0] = new_tree;   //create first tree for this bucket
        new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));  //insert record to tree

        new_bucket->current_record = 1; //first record for this bucket

    } else if (this->bucket_array[key] != nullptr)  //else I need to check if tree already exists
    {
        Bucket *check_bucket = this->bucket_array[key];
        while (check_bucket != nullptr)  //check every bucket in position key  //TODO to next bucket
        {
            int i = 0;

            while (check_bucket->tree_array[i] != nullptr && i
                < check_bucket->current_record)  //a bucket is only created to create a new tree so there is no chance of a null array of trees
            {
                AVLTree *check_tree = check_bucket->tree_array[i];

                if (treeExists(check_tree, record))   //insert record at current tree
                {
                    check_tree->setRoot(check_tree->insertTreeNode(check_tree->getRoot(), record));

                    return;
                }
                i++;
            }
            check_bucket = check_bucket->next_bucket;
        }
        check_bucket = this->bucket_array[key];
        while (check_bucket->next_bucket != nullptr)  //check every bucket in position key
        {
            check_bucket = check_bucket->next_bucket;
        }

        //after this while check_bucket is the last one of the bucket array in position key

        if (check_bucket->current_record == check_bucket->max_record)  //then bucket is full
        {
            Bucket *new_bucket = initializeBucket(bucket_size);   //create new Bucket
            new_bucket->next_bucket = nullptr;

            check_bucket->next_bucket = new_bucket;

            AVLTree *new_tree = new AVLTree;
            new_bucket->tree_array[0] = new_tree;   //create first tree for this bucket
            new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));  //insert record to tree

            new_bucket->current_record = 1; //first record for this bucket
        } else   //bucket not full, there is space for new tree in it's tree array
        {
            auto *new_tree = new AVLTree;
            check_bucket->tree_array[(check_bucket->current_record)] = new_tree;

            new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));
            check_bucket->current_record++;
        }
    }

}

//void diseaseHashTable::numCurrentPatients(string s)
//{
//    int length = s.length();
//
//    if(length < 20) //the word numCurrentPatients has 18 letters + 1 for a possible " " that is okay if exists
//    {
//        //then there is no specific disease the users cares about
//
//        for(int i = 0; i < this->dht_num_entries; i++)  //search the whole hash table
//        {
//            Bucket* it = bucket_array[i];
//
//            if(it != nullptr)
//            {
//                for(int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
//                {
//                    if(it->tree_array[j] != nullptr)
//                    {
//                        TreeNode* N = it->tree_array[j]->getRoot();
//                        it->tree_array[j]->printHospitalizedPatients(N);
//                    }
//                }
//                if(it->current_record == it->max_record)  //if bucket full then we check if there is next bucket
//                {
//                    it = it->next_bucket;
//                }
//            }
//        }
//    }
//    else  //we are looking for a specific disease
//    {
//        string disease = getLastWord(s);   //in this case the last word would be the disease
//        int key = hashFunction(disease, this->dht_num_entries);
//
//        if(bucket_array[key] != nullptr) //we only need to search at bucket_array[key]
//        {
//            for(int j = 0; j <= bucket_array[key]->current_record; j++)  //check every tree in this bucket
//            {
//                TreeNode* N = bucket_array[key]->tree_array[j]->getRoot(); //the root's record's disease represents what tree this disease is about
//
//                if(N->patientRecord->getDiseaseId() == disease) //we found the tree of this disease
//                {
//                    int counter = bucket_array[key]->tree_array[j]->countHospitalizedPatients(N);
//                    cout<<disease<<" "<<counter<<endl;
//                    return; //no need to search further
//                }
//            }
//            if(bucket_array[key]->current_record == bucket_array[key]->max_record)  //if bucket full then we check if there is next bucket
//            {
//                bucket_array[key] = bucket_array[key]->next_bucket;
//            }
//        }
//    }
//}

void diseaseHashTable::everyDiseaseRecords()
{
    for (int i = 0; i < this->dht_num_entries; i++)  //search the whole hash table
    {
        Bucket *it = bucket_array[i];

        while (it != nullptr)
        {
            for (int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if (it->tree_array[j] != nullptr)
                {
                    TreeNode *N = it->tree_array[j]->getRoot();
                    string disease = N->patientRecord->getDiseaseId();

                    int num = it->tree_array[j]->getRecordCounter();

                    cout << disease << " " << num << endl;
                }
            }
            it = it->next_bucket;
        }
    }
}

void diseaseHashTable::recordsBetweenDates(string date1, string date2)
{
    //reverse to compare dates correctly
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    for (int i = 0; i < this->dht_num_entries; i++)  //search the whole hash table
    {
        Bucket *it = bucket_array[i];

        while (it != nullptr)
        {
            for (int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if (it->tree_array[j] != nullptr)
                {
                    TreeNode *N = it->tree_array[j]->getRoot();
                    int counter = it->tree_array[j]->countBetweenDates(N, date1, date2);

                    cout << N->patientRecord->getDiseaseId() << " " << counter << endl;
                }
            }
            it = it->next_bucket;
        }
    }
}

void diseaseHashTable::frequency(string virus, string date1, string date2)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    for (int i = 0; i < this->dht_num_entries; i++)  //search the whole hash table
    {
        Bucket *it = bucket_array[i];

        while (it != nullptr)
        {
            for (int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if (it->tree_array[j] != nullptr)
                {
                    TreeNode *N = it->tree_array[j]->getRoot();

                    if (N->patientRecord->getDiseaseId() == virus)   //found the tree of this virus
                    {
                        int counter = it->tree_array[j]->countBetweenDates(N,
                                                                           date1,
                                                                           date2);   //count the record between the 2 dates

                        cout << N->patientRecord->getDiseaseId() << " " << counter << endl;
                    }
                }
            }
            it = it->next_bucket;
        }
    }
}

int diseaseHashTable::getDhtNumEntries() const
{
    return dht_num_entries;
}

Bucket **diseaseHashTable::getBucketArray() const
{
    return bucket_array;
}


