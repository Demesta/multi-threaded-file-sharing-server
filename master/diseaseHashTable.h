
#ifndef SYSPRO2_DISEASEHASHTABLE_H
#define SYSPRO2_DISEASEHASHTABLE_H

#include "HashTable.h"
#include <vector>
typedef struct bucket Bucket;
class AVLTree;

class diseaseHashTable: public HashTable
{
private:
    int dht_num_entries;
    Bucket**  bucket_array;

public:
    diseaseHashTable(int);
    ~diseaseHashTable();
    void insertRecord(PatientRecord*, int) override;
    bool treeExists(AVLTree*, PatientRecord*);
    void numCurrentPatients(string);
    void everyDiseaseRecords();  //prints number of records for each disease
    void recordsBetweenDates(string, string);
    void frequency(string, string, string);
    int getDhtNumEntries() const;
    Bucket **getBucketArray() const;
    void statistics(int socket);



};


#endif //SYSPRO2_DISEASEHASHTABLE_H
