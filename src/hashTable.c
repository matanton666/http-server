#include "include/hashTable.h"

// Hash function to generate index
unsigned int hash(const char* key) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

// Create a new node
Node* create_node(const char* key, char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Insert key-value pair into hash table
void insert(HashTable* hashTable, const char* key, char* value) {
    unsigned int index = hash(key);
    Node* newNode = create_node(key, value);
    Node* head = hashTable->table[index];

    if (head == NULL) {
        hashTable->table[index] = newNode;
    } else {
        Node* current = head;
        Node* prev = NULL;
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                current->value = value; // Update value if key already exists
                free(newNode->key);
                free(newNode);
                return;
            }
            prev = current;
            current = current->next;
        }
        prev->next = newNode;
    }
}

// Search for a key in the hash table
char* search(HashTable* hashTable, const char* key) {
    unsigned int index = hash(key);
    Node* current = hashTable->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

// Remove a key-value pair from the hash table
void delete_node(HashTable* hashTable, const char* key) {
    unsigned int index = hash(key);
    Node* current = hashTable->table[index];
    Node* prev = NULL;

    while (current != NULL && strcmp(current->key, key) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return; // Key not found
    }

    if (prev == NULL) {
        hashTable->table[index] = current->next;
    } else {
        prev->next = current->next;
    }

    free(current->key);
    free(current);
}

// Initialize hash table
HashTable* create_table() {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

// Free memory of the hash table
void free_table(HashTable* hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = hashTable->table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(hashTable);
}

