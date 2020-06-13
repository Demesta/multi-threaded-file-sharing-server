
#ifndef SYSPRO2__COUNTRYHASHTABLE_H_
#define SYSPRO2__COUNTRYHASHTABLE_H_

#ifndef INC_1_COUNTRYHASHTABLE_H
#define INC_1_COUNTRYHASHTABLE_H

#include "HashTable.h"
typedef struct bucket Bucket;
class AVLTree;

class countryHashTable: public HashTable
{
 private:
  int cht_num_entries;
  Bucket** bucket_array;

 public:
  countryHashTable(int);
  ~countryHashTable();
  void insertRecord(PatientRecord*, int) override;
  bool treeExists(AVLTree*, PatientRecord*);
  void frequency(string, string, string, string);
  void ageRanges(int, string, string, string, string);

};


#endif //INC_1_COUNTRYHASHTABLE_H

#endif //SYSPRO2__COUNTRYHASHTABLE_H_
