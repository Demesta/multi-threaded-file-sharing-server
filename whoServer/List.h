#ifndef SYSPRO2_RECORDS_LIST_H
#define SYSPRO2_RECORDS_LIST_H

#include <string>

class ArrayList {
 public:
    ArrayList(size_t initial_capacity);
    ~ArrayList();
    size_t size();
    void push(void* value_ptr);
    void* get(int index);
};




#endif //SYSPRO2_RECORDS_LIST_H