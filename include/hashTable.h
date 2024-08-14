#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



// Define the size of the hash table
#define TABLE_SIZE 5

// Node structure for the linked list in each hash table bucket
typedef struct node_t {
    char* key;
    char* value;
    struct node_t* next;
} node_t;

// Hash table structure
typedef struct hash_table_t {
    node_t* table[TABLE_SIZE];
} hash_table_t;




// create a new hash table
hash_table_t* create_table(void);

// insert a key-value pair into the hash table
void insert(hash_table_t* hashTable, const char* key, char* value);

// search for a value by key in the hash table
// returns null if not found
char* search(hash_table_t* hashTable, const char* key);

// delete a key-value pair from the hash table
void delete_node(hash_table_t* hashTable, const char* key);

// free the memory used by the hash table
void free_table(hash_table_t* hashTable);

#endif // HASHTABLE_H
