
#include "countryHashTable.h"
#include <bits/stdc++.h>
#include "countryHashTable.h"
#include "TreeNode.h"
#include "AVLTree.h"
#include "Bucket.h"
#include "PatientRecord.h"
#include "utils.h"
#include "Records_list.h"

countryHashTable::countryHashTable(int chs_num_entries)
{
    cht_num_entries = chs_num_entries;

    bucket_array = new Bucket*[cht_num_entries];

    for(int i = 0; i<cht_num_entries; i++)
    {
        bucket_array[i] = nullptr;
    }
}

countryHashTable::~countryHashTable()
{
    for(int i = 0; i<cht_num_entries; i++)
    {
        delete bucket_array[i];   //free every bucket
    }
    delete bucket_array;
}

void countryHashTable::ageRanges(int k, string country, string disease, string date1, string date2)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    int ages_20 = 0;
    int ages_40 = 0;
    int ages_60 = 0;
    int ages_60plus = 0;
    int sort_array[4];

    for(int i = 0; i < this->cht_num_entries; i++)  //search the whole hash table
    {
        Bucket* it = bucket_array[i];

        while(it != nullptr)
        {
            for(int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if(it->tree_array[j] != nullptr)
                {
                    TreeNode* N = it->tree_array[j]->getRoot();

                    if(N->patientRecord->getCountry() == country)   //found the tree of this country
                    {
                        ages_20 = it->tree_array[j]->countAges(N, 0, 20);
                        ages_40 = it->tree_array[j]->countAges(N, 21, 40);
                        ages_60 = it->tree_array[j]->countAges(N, 41, 60);
                        ages_60plus = it->tree_array[j]->countAges(N, 60, 100);

                        int a1=ages_20;
                        int a2=ages_40;
                        int a3=ages_60;
                        int a4=ages_60plus;

                        int sum = ages_20 + ages_40 + ages_60 + ages_60plus;

                        sort_array[0] = ages_20;
                        sort_array[1] = ages_40;
                        sort_array[2] = ages_60;
                        sort_array[3] = ages_60plus;

                        int s = sizeof(sort_array) / sizeof(sort_array[0]);
                        sort(sort_array, sort_array + s); //sort the array of numbers

                        for (int z = 0; z < k; z++)
                        {
                            if(sort_array[z] == a1)
                            {
                                cout<<"0-20: "<<(sort_array[z]/sum) * 100 <<"%"<<endl;
                            }
                            else if(sort_array[z] == a2)
                            {
                                cout<<"21-40: "<<(sort_array[z]/sum) * 100 <<"%"<<endl;
                            }
                            else if(sort_array[z] == a3)
                            {
                                cout<<"41-60: "<<(sort_array[z]/sum) * 100 <<"%"<<endl;
                            }
                            else if(sort_array[z] == a4)
                            {
                                cout<<"60+: "<<(sort_array[z]/sum) * 100 <<"%"<<endl;
                            }

                        }

                            //cout<<disease<<" "<<counter<<endl;

                        return;
                    }
                }
            }
            it = it->next_bucket;
        }
    }


}

bool countryHashTable::treeExists(AVLTree* tree, PatientRecord *record)
{
    TreeNode* tree_node = tree->getRoot();    //i only need to check the disease of the root's record
    string check_disease = tree_node->patientRecord->getCountry();

    if(check_disease == record->getCountry())
    {
        return true; //tree for this disease exists
    }

    return false;
}

void countryHashTable::insertRecord(PatientRecord *record, int bucket_size)
{
    int key = hashFunction(record->getCountry(), this->cht_num_entries);    //generate key for new record

    if(this->bucket_array[key] == nullptr)     //case: first record with this key
    {
        auto* new_bucket = initializeBucket(bucket_size);   //create new Bucket
        new_bucket->next_bucket = nullptr;

        this->bucket_array[key] = new_bucket;

        auto* new_tree = new AVLTree;
        new_bucket->tree_array[0] = new_tree;   //create first tree for this bucket
        new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));  //insert record to tree

        new_bucket->current_record = 1; //first record for this bucket

    }

    else if(this->bucket_array[key] != nullptr)  //else I need to check if tree already exists
    {
        Bucket* check_bucket = this->bucket_array[key];
        while(check_bucket != nullptr)  //check every bucket in position key
        {
            int i = 0;

            while(check_bucket->tree_array[i] != nullptr && i < check_bucket->current_record)  //a bucket is only created to create a new tree so there is no chance of a null array of trees
            {
                AVLTree* check_tree = check_bucket->tree_array[i];

                if(treeExists(check_tree, record))   //insert record at current tree
                {
                    check_tree->setRoot(check_tree->insertTreeNode(check_tree->getRoot(), record));

                    return;
                }
                i++;
            }
            check_bucket = check_bucket->next_bucket;
        }
        check_bucket = this->bucket_array[key];
        while(check_bucket->next_bucket != nullptr)  //check every bucket in position key
        {
            check_bucket = check_bucket->next_bucket;
        }

        //after this while check_bucket is the last one of the bucket array in position key

        if(check_bucket->current_record == check_bucket->max_record)  //then bucket is full
        {
            auto* new_bucket = initializeBucket(bucket_size);   //create new Bucket
            new_bucket->next_bucket = nullptr;

            check_bucket->next_bucket = new_bucket;

            auto* new_tree = new AVLTree;
            new_bucket->tree_array[0] = new_tree;   //create first tree for this bucket
            new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));  //insert record to tree

            new_bucket->current_record = 1; //first record for this bucket
        }
        else   //bucket not full, there is space for new tree in its tree array
        {
            auto* new_tree = new AVLTree;
            check_bucket->tree_array[(check_bucket->current_record)] = new_tree;

            new_tree->setRoot(new_tree->insertTreeNode(new_tree->getRoot(), record));
            check_bucket->current_record++;
        }
    }

}

void countryHashTable::frequency(string virus, string country, string date1, string date2)
{
    date1 = ReverseDateFormat(date1);
    date2 = ReverseDateFormat(date2);

    for(int i = 0; i < this->cht_num_entries; i++)  //search the whole hash table
    {
        Bucket* it = bucket_array[i];

        while(it != nullptr)
        {
            for(int j = 0; j <= it->current_record; j++)  //check every tree in this bucket
            {
                if(it->tree_array[j] != nullptr)
                {
                    TreeNode* N = it->tree_array[j]->getRoot();

                    if(N->patientRecord->getCountry() == country)   //found the tree of this country
                    {
                        int counter = it->tree_array[j]->countRecordsForCountry(N, virus, date1, date2);  //count the record between the 2 dates for this virus

                        cout<<virus<<" "<<counter<<endl;

                        return;
                    }
                }
            }
            it = it->next_bucket;
        }
    }
}





