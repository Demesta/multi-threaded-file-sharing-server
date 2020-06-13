#include "Bucket.h"
#include <stdlib.h>

Bucket* initializeBucket(int bucket_size)
{
    auto* new_bucket = new Bucket;
    int max = (bucket_size -  sizeof(Bucket) ) / sizeof(AVLTree*);
    new_bucket->max_record = max;

    new_bucket->tree_array = new AVLTree*[max];

    for(int i = 0; i<max; i++)
    {
        new_bucket->tree_array[i] = nullptr;
    }

    new_bucket->next_bucket = nullptr;

    new_bucket->current_record = 0;

    return new_bucket;
}