#include "../include/hashTable.h"
#include "unity.h"

void setUp() 
{

}

void tearDown()
{

}


void test_insert_and_search(void) {
    hash_table_t* hashTable = create_table();

    insert(hashTable, "key1", "100");
    insert(hashTable, "key2", "200");
    insert(hashTable, "key3", "300");

    TEST_ASSERT_EQUAL_STRING("100", search(hashTable, "key1"));
    TEST_ASSERT_EQUAL_STRING("200", search(hashTable, "key2"));
    TEST_ASSERT_EQUAL_STRING("300", search(hashTable, "key3"));

    free_table(hashTable);
}

void test_search_non_existent_key(void) {
    hash_table_t* hashTable = create_table();

    insert(hashTable, "key1", "100");

    TEST_ASSERT_NULL(search(hashTable, "key2")); // Should return NULL for non-existent key

    free_table(hashTable);
}

void test_update_existing_key(void) {
    hash_table_t* hashTable = create_table();

    insert(hashTable, "key1", "100");
    TEST_ASSERT_EQUAL_STRING("100", search(hashTable, "key1"));

    insert(hashTable, "key1", "500"); // Update value
    TEST_ASSERT_EQUAL_STRING("500", search(hashTable, "key1"));

    free_table(hashTable);
}

void test_delete_key(void) {
    hash_table_t* hashTable = create_table();

    insert(hashTable, "key1", "100");
    insert(hashTable, "key2", "200");

    delete_node(hashTable, "key1");
    TEST_ASSERT_NULL(search(hashTable, "key1")); // Should return NULL after deletion

    TEST_ASSERT_EQUAL_STRING("200", search(hashTable, "key2")); // Ensure other keys are unaffected

    free_table(hashTable);
}

void test_insert_multiple_keys_with_same_hash(void) {
    hash_table_t* hashTable = create_table();

    // Insert keys that would hash to the same index (assuming a simple hash function)
    insert(hashTable, "a", "asdf");
    insert(hashTable, "k", "wert"); // Assuming 'a' and 'k' hash to the same index

    TEST_ASSERT_EQUAL_STRING("asdf", search(hashTable, "a"));
    TEST_ASSERT_EQUAL_STRING("wert", search(hashTable, "k"));

    free_table(hashTable);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_insert_and_search);
    RUN_TEST(test_search_non_existent_key);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_delete_key);
    RUN_TEST(test_insert_multiple_keys_with_same_hash);

    return UNITY_END();
}
