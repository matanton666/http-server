#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


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

// use regular free because it just points to existing ht and not copying
typedef struct hash_table_iter {
    hash_table_t* ht;
    node_t* curr;
    int idx;
}hash_table_iter;

// returns iterator even if table is empty
// free using regular free()
hash_table_iter* create_iterator(hash_table_t* tbl);

// returns true or false if has next
int has_next(hash_table_iter* iter);

void point_next(hash_table_iter* iter);

// create a new hash table
hash_table_t* create_table(void);

// insert a key-value pair into the hash table
// creates a copy of the strings
void insert(hash_table_t* hashTable, const char* key, const char* value);

// search for a value by key in the hash table
// returns null if not found
char* search(hash_table_t* hashTable, const char* key);

// delete a key-value pair from the hash table
void delete_node(hash_table_t* hashTable, const char* key);

// free the memory used by the hash table
void free_table(hash_table_t* hashTable);


#endif // HASHTABLE_H
