#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

HashTable new_hash_table() {
    HashTable hash_table;
    hash_table.bucket_size = 100;

    hash_table.current_filled_count = 0;
    hash_table.entries = calloc(100, sizeof(HashTableEntry));
    hash_table.keys = calloc(100, sizeof(char*));

    return hash_table;
}

unsigned long hash_string_fnv_1a(char *string) {
    unsigned long hash = 0xcbf29ce484222325;

    int length = strlen(string);

    for( int i = 0; i < length; i++) {
        hash = hash ^ string[i];
        hash *=0x100000001b3;
    }

    return hash;
}

void resize_hash_table(HashTable *hash_table, int new_size) {
    HashTableEntry *values = calloc(
        sizeof(HashTableEntry),
        hash_table->bucket_size
    );

    memcpy(
        values, 
        hash_table->entries, 
        sizeof(HashTableEntry) * hash_table->bucket_size
    );

    free(hash_table->entries);

    int previous_size = hash_table->bucket_size;

    hash_table->entries = calloc(new_size, sizeof(HashTableEntry));
    hash_table->keys = calloc(new_size, sizeof(char*));

    hash_table->current_filled_count = 0;
    hash_table->bucket_size = new_size;

    for(int i = 0; i < previous_size; i++) {
        HashTableEntry hash_table_entry = values[i];
        for(int j = 0; j < hash_table_entry.entry_count; j++) {
            set_hash_table(hash_table,
                hash_table_entry.keys[j],
                hash_table_entry.values[j]
            );
        }
    }

    free(values);
}

void **get_hash_table_ptr(HashTable *hash_table, char *key) {

    unsigned long hashed = hash_string_fnv_1a(key);

    int index = hashed % hash_table->bucket_size;

    HashTableEntry entry = hash_table->entries[index];
    for(int i = 0; i < entry.entry_count; i++) {
        bool same_key = strcmp(entry.keys[i], key) == 0;
        if (same_key) {
            return &entry.values[i];
        }
    }

    return NULL;
}

void set_hash_table(HashTable *hash_table, char *key, void *value) {
    void **value_ptr = get_hash_table_ptr(hash_table, key);
    if (value_ptr != NULL) {
        *value_ptr = value;
    }

    hash_table->current_filled_count++;

    unsigned long hashed = hash_string_fnv_1a(key);

    if (hash_table->bucket_size * 0.8 <= hash_table->current_filled_count) {
        resize_hash_table(hash_table, hash_table->bucket_size + 100);
    }

    int index = hashed % hash_table->bucket_size;

    HashTableEntry *entry = &hash_table->entries[index];

    entry->entry_count++;
    entry->values = realloc(entry->values, sizeof(void*) * entry->entry_count);
    entry->keys = realloc(entry->keys, sizeof(char*) * entry->entry_count);

    entry->values[entry->entry_count-1] = value;
    entry->keys[entry->entry_count-1] = key;
    hash_table->keys[hash_table->current_filled_count-1] = key;
}

void *get_hash_table(HashTable *hash_table, char *key) {

    void **value_ptr = get_hash_table_ptr(hash_table, key);

    if (value_ptr == NULL) {
        return NULL;
    }

    return *value_ptr;
}

