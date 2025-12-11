#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#define FNV_offset_basis 0xcbf29ce484222325
#define FNV_prime FNV_prime

typedef struct {
    char **keys;
    void **values;

    int entry_count;
} HashTableEntry;

typedef struct {
    int bucket_size;
    int current_filled_count;

    char **keys;
    HashTableEntry *entries;
} HashTable;

HashTable new_hash_table();
void set_hash_table(HashTable *hash_table, char *key, void *value);
void *get_hash_table(HashTable *hash_table, char *key);

#endif
