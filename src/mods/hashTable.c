#include "../../include/hashTable.h"

unsigned int hash(const char* key) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

node_t* create_node(const char* key, const char* value) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = NULL;
    return newNode;
}

void insert(hash_table_t* hashTable, const char* key, const char* value) {
    unsigned int index = hash(key);
    node_t* newNode = create_node(key, value);
    node_t* head = hashTable->table[index];

    if (head == NULL) {
        hashTable->table[index] = newNode;
    } else {
        node_t* current = head;
        node_t* prev = NULL;
        while (current != NULL) { // append to end or update
            if (strcmp(current->key, key) == 0) {
                free(current->value);
                current->value = strdup(value); // Update value if key already exists

                free(newNode->key);
                free(newNode->value);
                free(newNode);
                return;
            }
            prev = current;
            current = current->next;
        }
        prev->next = newNode;
    }
}

char* search(hash_table_t* hashTable, const char* key) {
    unsigned int index = hash(key);
    node_t* current = hashTable->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void delete_node(hash_table_t* hashTable, const char* key) {
    unsigned int index = hash(key);
    node_t* current = hashTable->table[index];
    node_t* prev = NULL;

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

hash_table_t* create_table() {
    hash_table_t* hashTable = (hash_table_t*)malloc(sizeof(hash_table_t));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

void free_table(hash_table_t* hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        node_t* current = hashTable->table[i];
        while (current != NULL) {
            node_t* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value); 
            free(temp);
        }
    }
    free(hashTable);
}




hash_table_iter* create_iterator(hash_table_t* tbl)
{
    if (!tbl) return NULL;

    hash_table_iter* iter = (hash_table_iter*)malloc(sizeof(hash_table_iter));

    iter->ht = tbl;
    iter->idx = 0;
    iter->curr = NULL;

    for (; iter->idx < TABLE_SIZE; iter->idx++) {
        iter->curr = tbl->table[iter->idx];
        if (iter->curr != NULL) break;
    }

    return iter;
}


int has_next(hash_table_iter* iter)
{
    if (iter->curr == NULL) return false;
    if (iter->curr->next != NULL) return true;

    node_t* tmp = NULL;
    for (int i = iter->idx + 1; i < TABLE_SIZE; i++) {
        tmp = iter->ht->table[i];
        if (tmp != NULL) return true;
    }

    return false;
}




void point_next(hash_table_iter* iter)
{
    if (iter->curr == NULL) return;
    if (iter->curr->next != NULL){
        iter->curr = iter->curr->next;
        return;
    }

    for (iter->idx++; iter->idx < TABLE_SIZE; iter->idx++) {
        iter->curr = iter->ht->table[iter->idx];
        if (iter->curr != NULL) break;
    }
}
