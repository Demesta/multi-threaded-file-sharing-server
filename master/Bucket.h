//
// Created by demesta on 26/5/20.
//

#ifndef SYSPRO2_BUCKET_H
#define SYSPRO2_BUCKET_H

class AVLTree;

typedef struct bucket
{
    int max_record;
    int current_record;
    AVLTree** tree_array;
    bucket *next_bucket;
} Bucket;

Bucket* initializeBucket(int);


#endif //SYSPRO2_BUCKET_H
