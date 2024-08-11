#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



// Define the size of the hash table
#define TABLE_SIZE 10

// Node structure for the linked list in each hash table bucket
typedef struct Node {
    char* key;
    char* value;
    struct Node* next;
} Node;

// Hash table structure
typedef struct HashTable {
    Node* table[TABLE_SIZE];
} HashTable;




// Function to create a new hash table
HashTable* create_table(void);

// Function to insert a key-value pair into the hash table
void insert(HashTable* hashTable, const char* key, char* value);

// Function to search for a value by key in the hash table
char* search(HashTable* hashTable, const char* key);

// Function to delete a key-value pair from the hash table
void delete_node(HashTable* hashTable, const char* key);

// Function to free the memory used by the hash table
void free_table(HashTable* hashTable);

#endif // HASHTABLE_H
