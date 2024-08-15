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

void test_iterator() {
    // Test case 1: Empty hash table
    hash_table_t* ht1 = create_table();
    hash_table_iter* iter1 = create_iterator(ht1);
    TEST_ASSERT_FALSE(has_next(iter1));
    free(iter1);
    free_table(ht1);

    // Test case 2: Hash table with a single key-value pair
    hash_table_t* ht2 = create_table();
    insert(ht2, "key1", "value1");
    hash_table_iter* iter2 = create_iterator(ht2);
    TEST_ASSERT_FALSE(has_next(iter2));
    free(iter2);
    free_table(ht2);

    // Test case 3: Hash table with multiple key-value pairs
    hash_table_t* ht3 = create_table();
    insert(ht3, "key1", "value1");
    insert(ht3, "key2", "value2");
    insert(ht3, "key3", "value3");
    hash_table_iter* iter3 = create_iterator(ht3);
    TEST_ASSERT_TRUE(has_next(iter3));
    point_next(iter3);
    TEST_ASSERT_TRUE(has_next(iter3));
    point_next(iter3);
    TEST_ASSERT_FALSE(has_next(iter3));
    free(iter3);
    free_table(ht3);

    // Test case 4: Hash table with collisions
    hash_table_t* ht4 = create_table();
    insert(ht4, "a", "value2");
    insert(ht4, "k", "value2"); // Assuming 'a' and 'k' hash to the same index
    insert(ht4, "r", "value2"); // Assuming 'a' and 'k' hash to the same index
    insert(ht4, "e", "value2"); // Assuming 'a' and 'k' hash to the same index
    insert(ht4, "w", "value2"); // Assuming 'a' and 'k' hash to the same index
    insert(ht4, "q", "value2"); // Assuming 'a' and 'k' hash to the same index
    insert(ht4, "c", "value2"); // Assuming 'a' and 'k' hash to the same index
    hash_table_iter* iter4 = create_iterator(ht4);
    do {
        TEST_ASSERT_EQUAL_STRING("value2", iter4->curr->value);
        point_next(iter4);
    }while (has_next(iter4)); 
    TEST_ASSERT_FALSE(has_next(iter4));
    TEST_ASSERT_EQUAL_STRING("c", iter4->curr->key);
    free(iter4);
    free_table(ht4);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_insert_and_search);
    RUN_TEST(test_search_non_existent_key);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_iterator);
    RUN_TEST(test_insert_multiple_keys_with_same_hash);
    RUN_TEST(test_iterator);

    return UNITY_END();
}
