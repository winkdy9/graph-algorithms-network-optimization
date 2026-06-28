#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../lab3/vector/generic.h"
#include "../../lab3/comparators.h"
#include "../../lab3/macros.h"
#include "generic.h"

#define PTR_SIZE sizeof(void*)
#include <stdint.h>

Vector **get_bucket_ptr(HashTable *table, size_t index) {
    return (Vector **)((char *)table->values->data + index * PTR_SIZE);
}

static void *get_bucket_element(Vector *bucket, size_t idx) {
    return getVectorItem(bucket, idx);
}

const uint64_t A = 11400714819323198485ULL;

int HashInt(const void *key) {
    if (!key) {
        RETURN_ERROR("NULL key", 0);
    }
    int ik = *(const int *)key;
    uint64_t k = (uint64_t)(uint32_t)ik;
    uint64_t h = k * A;
    return (int)(h >> 32);
}

int HashString(const void *key) {
    if (!key) {
        RETURN_ERROR("NULL key", 0);
    }

    const unsigned char *s = (const unsigned char *)key;
    uint32_t hash = 5381;

    while (*s) {
        hash = ((hash << 5) + hash) ^ (uint32_t)(*s);
        s++;
    }
    return (int)hash;
}

int HashLL(const void *key) {
    if (!key) return 0;

    uint64_t k = *(const uint64_t*)key;
    uint64_t h = k * A;
    return (int)(h >> 32);
}

int HashL(const void *key) {
    if (!key) return 0;

    long k = *(const long *)key;
    return (int)(k * 2654435761u);
}

HashTable *createHashTable(size_t key_size, size_t val_size) {
    if (key_size == 0 || val_size == 0) {
        RETURN_ERROR("Zero size", NULL);
    }
    
    HashTable *table = malloc(sizeof(HashTable));
    if (!table) {
        RETURN_ERROR("Failed to allocate table", NULL);
    }
    
    table->key_size = key_size;
    table->val_size = val_size;
    table->size = 0;
    table->capacity = TABLE_MIN_SIZE;

    table->values = createVector(PTR_SIZE);
    if (!table->values) {
        free(table);
        RETURN_ERROR("Failed to create vector", NULL);
    }

    void *buf = calloc(table->capacity, PTR_SIZE);
    if (!buf) {
        vectorFree(table->values);
        free(table);
        RETURN_ERROR("Failed to allocate buckets", NULL);
    }
    
    table->values->data = buf;
    table->values->size = table->capacity;
    table->values->capacity = table->capacity;

    return table;
}

static Vector *create_bucket(size_t key_size, size_t val_size) {
    size_t elem_size = key_size + val_size;
    Vector *bucket = createVector(elem_size);
    return bucket;
}

static long find_in_bucket(Vector *bucket, const void *key, size_t key_size, CmpFunc cmp) {
    if (!bucket) {
        return -1;
    }
    for (size_t i = 0; i < bucket->size; ++i) {
        void *elem = get_bucket_element(bucket, i);
        void *key_ptr = elem;
        if (cmp(key_ptr, key)) {
            return (long)i;
        }
    }
    return -1;
}

void setItemHashTable(HashTable *table, void *key, void *data, HashFunc hash, CmpFunc cmp) {
    if (!table || !hash || !cmp || !key || !data) {
        RETURN_ERROR_VOID("NULL input");
    }

    double load_after = (double)(table->size + 1) / (double)table->capacity;
    if (load_after > 0.75) {
        rehashHashTable(table, hash, cmp);
    }
    
    uint64_t hash_val = (uint64_t)hash(key);
    size_t idx = (size_t)(hash_val % table->capacity);

    Vector **bucket_ptr = get_bucket_ptr(table, idx);
    Vector *bucket = *bucket_ptr;

    if (!bucket) {
        bucket = create_bucket(table->key_size, table->val_size);
        if (!bucket) {
            RETURN_ERROR_VOID("Failed to create bucket");
        }
        *bucket_ptr = bucket;
    }

    long pos = find_in_bucket(bucket, key, table->key_size, cmp);
    if (pos >= 0) {
        void *elem = get_bucket_element(bucket, (size_t)pos);
        void *value_ptr = (char *)elem + table->key_size;
        memcpy(value_ptr, data, table->val_size);
        return;
    }

    size_t elem_size = table->key_size + table->val_size;
    void *buf = malloc(elem_size);
    if (!buf) {
        RETURN_ERROR_VOID("Failed to allocate element");
    }
    
    memcpy(buf, key, table->key_size);
    memcpy((char *)buf + table->key_size, data, table->val_size);

    if (appendVectorItem(bucket, buf) != 0) {
        free(buf);
        RETURN_ERROR_VOID("Failed to append to bucket");
    }

    free(buf);
    table->size++;
    
    double load = (double)table->size / (double)table->capacity;
    if (load > 0.75) {
        rehashHashTable(table, hash, cmp);
    }
}

void rehashHashTable(HashTable *table, HashFunc hash, CmpFunc cmp) {
    if (!table || !hash || !cmp) {
        RETURN_ERROR_VOID("NULL input");
    }

    size_t old_capacity = table->capacity;
    size_t new_capacity = old_capacity * 2;
    if (new_capacity == 0) {
        new_capacity = 1;
    }

    Vector *new_vector = createVector(PTR_SIZE);
    if (!new_vector) {
        RETURN_ERROR_VOID("Failed to create new vector");
    }
    
    new_vector->size = new_capacity;
    void *new_data = calloc(new_capacity, PTR_SIZE);
    if (!new_data) {
        vectorFree(new_vector);
        RETURN_ERROR_VOID("Failed to allocate new buckets");
    }
    new_vector->data = new_data;
    new_vector->capacity = new_capacity;

    size_t elem_size = table->key_size + table->val_size;

    for (size_t i = 0; i < old_capacity; ++i) {
        Vector **old_bucket_ptr = get_bucket_ptr(table, i);
        Vector *old_bucket = *old_bucket_ptr;
        if (!old_bucket) continue;

        for (size_t j = 0; j < old_bucket->size; ++j) {
            void *elem = get_bucket_element(old_bucket, j);
            void *key_ptr = elem;

            uint64_t hash_val = (uint64_t)hash(key_ptr);
            size_t new_idx = (size_t)(hash_val % new_capacity);

            Vector **new_bucket_ptr = (Vector **)((char *)new_vector->data + new_idx * PTR_SIZE);
            Vector *new_bucket = *new_bucket_ptr;
            
            if (!new_bucket) {
                new_bucket = create_bucket(table->key_size, table->val_size);
                if (!new_bucket) {
                    for (size_t k = 0; k < new_capacity; ++k) {
                        Vector **bp = (Vector **)((char *)new_vector->data + k * PTR_SIZE);
                        if (*bp) vectorFree(*bp);
                    }
                    free(new_data);
                    vectorFree(new_vector);
                    RETURN_ERROR_VOID("Failed to create new bucket");
                }
                *new_bucket_ptr = new_bucket;
            }

            void *tmp = malloc(elem_size);
            if (!tmp) continue;
            memcpy(tmp, elem, elem_size);

            if (appendVectorItem(new_bucket, tmp) != 0) {
                free(tmp);
            }
        }

        vectorFree(old_bucket);
        *old_bucket_ptr = NULL;
    }
    
    vectorFree(table->values);
    table->values = new_vector;
    table->capacity = new_capacity;
}

void *getItemHashTable(HashTable *table, void *key, HashFunc hash, CmpFunc cmp) {
    if (!table || !hash || !cmp || !key) {
        RETURN_ERROR("NULL input", NULL);
    }

    uint64_t hash_val = (uint64_t)hash(key);
    size_t idx = (size_t)(hash_val % table->capacity);

    Vector **bucket_ptr = get_bucket_ptr(table, idx);
    Vector *bucket = *bucket_ptr;
    if (!bucket) {
        return NULL;
    }

    long pos = find_in_bucket(bucket, key, table->key_size, cmp);
    if (pos < 0) {
        return NULL;
    }

    void *elem = get_bucket_element(bucket, (size_t)pos);
    return (char *)elem + table->key_size;
}

void *popItemHashTable(HashTable *table, void *key, HashFunc hash, CmpFunc cmp) {
    if (!table || !hash || !cmp || !key) {
        RETURN_ERROR("NULL input", NULL);
    }

    uint64_t hash_val = (uint64_t)hash(key);
    size_t idx = (size_t)(hash_val % table->capacity);

    Vector **bucket_ptr = get_bucket_ptr(table, idx);
    Vector *bucket = *bucket_ptr;
    if (!bucket) {
        return NULL;
    }

    long pos = find_in_bucket(bucket, key, table->key_size, cmp);
    if (pos < 0) {
        return NULL;
    }

    void *elem_copy = popVectorItem(bucket, (size_t)pos);
    if (!elem_copy) {
        RETURN_ERROR("Failed to pop item", NULL);
    }

    void *value_copy = malloc(table->val_size);
    if (!value_copy) {
        free(elem_copy);
        RETURN_ERROR("Failed to copy value", NULL);
    }
    memcpy(value_copy, (char *)elem_copy + table->key_size, table->val_size);

    free(elem_copy);
    table->size--;

    return value_copy;
}

unsigned long int getCollisionCount(HashTable *table, HashFunc hash) {
    if (!table || !hash) {
        RETURN_ERROR("NULL input", 0);
    }

    unsigned long collisions = 0;
    for (size_t i = 0; i < table->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(table, i);
        Vector *bucket = *bucket_ptr;
        if (bucket && bucket->size > 1) {
            collisions += (unsigned long)(bucket->size - 1);
        }
    }
    return collisions;
}

void freeHashTable(HashTable *table) {
    if (!table) return;
    
    if (table->values) {
        for (size_t i = 0; i < table->capacity; ++i) {
            Vector **bucket_ptr = get_bucket_ptr(table, i);
            Vector *bucket = *bucket_ptr;
            if (bucket) {
                vectorFree(bucket);
                *bucket_ptr = NULL;
            }
        }
        vectorFree(table->values);
    }
    free(table);
}