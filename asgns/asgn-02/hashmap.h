// myfunctions.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* Key-value pairs in the hash table */
typedef struct KeyValue {
  char* key;
  char* value;
  struct KeyValue* next;
} KeyValue;

// Define the structure for the hash table
typedef struct HashTable {
  int table_size; /* total size of the hash table */
  int num_items; /* number of items in the hash table */
  KeyValue** table_items; /* array of pointers to KeyValue */
} HashTable;

/* Function prototypes */
HashTable* create_hash_table(int size);
void insert(HashTable* table, char* key, char* value);
char* get(HashTable* table, char* key);
void remove_key(HashTable* table, char* key);
void free_hash_table(HashTable* table);

#endif