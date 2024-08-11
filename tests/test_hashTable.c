#include "../include/hashTable.h"
#include <assert.h>

void test_insert_and_search(void) {
    HashTable* hashTable = create_table();

    insert(hashTable, "key1", "100");
    insert(hashTable, "key2", "200");
    insert(hashTable, "key3", "300");

    assert(strcmp(search(hashTable, "key1"), "100") == 0);
    assert(strcmp(search(hashTable, "key2"), "200") == 0);
    assert(strcmp(search(hashTable, "key3"), "300") == 0);

    free_table(hashTable);
    printf("test_insert_and_search passed!\n");
}

void test_search_non_existent_key(void) {
    HashTable* hashTable = create_table();

    insert(hashTable, "key1", "100");

    assert(search(hashTable, "key2") == NULL); // Should return NULL for non-existent key

    free_table(hashTable);
    printf("test_search_non_existent_key passed!\n");
}

void test_update_existing_key(void) {
    HashTable* hashTable = create_table();

    insert(hashTable, "key1", "100");
    assert(strcmp(search(hashTable, "key1"), "100") == 0);

    insert(hashTable, "key1", "500"); // Update value
    assert(strcmp(search(hashTable, "key1"), "500") == 0);

    free_table(hashTable);
    printf("test_update_existing_key passed!\n");
}

void test_delete_key(void) {
    HashTable* hashTable = create_table();

    insert(hashTable, "key1", "100");
    insert(hashTable, "key2", "200");

    delete_node(hashTable, "key1");
    assert(search(hashTable, "key1") == NULL); // Should return NULL after deletion

    assert(strcmp(search(hashTable, "key2"), "200") == 0); // Ensure other keys are unaffected

    free_table(hashTable);
    printf("test_delete_key passed!\n");
}

void test_insert_multiple_keys_with_same_hash(void) {
    HashTable* hashTable = create_table();

    // Insert keys that would hash to the same index (assuming a simple hash function)
    insert(hashTable, "a", "asdf");
    insert(hashTable, "k", "wert"); // Assuming 'a' and 'k' hash to the same index

    assert(strcmp(search(hashTable, "a"), "asdf") == 0);
    assert(strcmp(search(hashTable, "k"), "wert") == 0);

    free_table(hashTable);
    printf("test_insert_multiple_keys_with_same_hash passed!\n");
}

int main(void) {
    test_insert_and_search();
    test_search_non_existent_key();
    test_update_existing_key();
    test_delete_key();
    test_insert_multiple_keys_with_same_hash();

    return 0;
}
