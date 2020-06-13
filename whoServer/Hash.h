
#ifndef SYSPRO2_HASHTABLE_H
#define SYSPRO2_HASHTABLE_H


#include <string>

using namespace std;

class HashTable
{
 public:
  HashTable(size_t buckets, int (*hash_func)(void*));
  ~HashTable();

  void put(void* key_ptr, void* value_ptr);
  void* get(void* key_ptr);
  void remove(void* key_tr);
};




#endif //SYSPRO2_HASHTABLE_H