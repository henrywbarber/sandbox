#include <stdio.h>
#include <string.h>
#include "hash_table.h"

// Helper function to print test results
void print_test_header(const char* test_name) {
    printf("\n========================================\n");
    printf("TEST: %s\n", test_name);
    printf("========================================\n");
}

void print_test_result(const char* description, int passed) {
    printf("[%s] %s\n", passed ? "PASS" : "FAIL", description);
}

void print_hash_table_stats(ht_hash_table* ht) {
    printf("\nHash Table Stats:\n");
    printf("  Size: %d\n", ht->size);
    printf("  Count: %d\n", ht->count);
    printf("  Base Size: %d\n", ht->base_size);
    printf("  Load Factor: %.2f%%\n", (ht->count * 100.0) / ht->size);
}

void print_separator() {
    printf("----------------------------------------\n");
}

// Test 1: Basic insertion and retrieval
void test_basic_operations() {
    print_test_header("Basic Insert and Search");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    // Insert some items
    printf("\nInserting items...\n");
    ht_insert(ht, "name", "Alice");
    ht_insert(ht, "age", "30");
    ht_insert(ht, "city", "New York");
    ht_insert(ht, "country", "USA");
    
    print_hash_table_stats(ht);
    
    // Search for items
    printf("\nSearching for items...\n");
    char* name = ht_search(ht, "name");
    print_test_result("Search 'name' = 'Alice'", name != NULL && strcmp(name, "Alice") == 0);
    if (name) printf("  Found: name = %s\n", name);
    
    char* age = ht_search(ht, "age");
    print_test_result("Search 'age' = '30'", age != NULL && strcmp(age, "30") == 0);
    if (age) printf("  Found: age = %s\n", age);
    
    char* city = ht_search(ht, "city");
    print_test_result("Search 'city' = 'New York'", city != NULL && strcmp(city, "New York") == 0);
    if (city) printf("  Found: city = %s\n", city);
    
    // Search for non-existent key
    char* missing = ht_search(ht, "missing_key");
    print_test_result("Search 'missing_key' = NULL", missing == NULL);
    
    ht_del_hash_table(ht);
}

// Test 2: Update existing keys
void test_update_keys() {
    print_test_header("Update Existing Keys");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("\nInserting initial value...\n");
    ht_insert(ht, "status", "pending");
    char* value1 = ht_search(ht, "status");
    printf("  status = %s\n", value1 ? value1 : "NULL");
    print_test_result("Initial value 'pending'", value1 != NULL && strcmp(value1, "pending") == 0);
    
    printf("\nUpdating value...\n");
    ht_insert(ht, "status", "completed");
    char* value2 = ht_search(ht, "status");
    printf("  status = %s\n", value2 ? value2 : "NULL");
    print_test_result("Updated value 'completed'", value2 != NULL && strcmp(value2, "completed") == 0);
    
    print_hash_table_stats(ht);
    print_test_result("Count should remain 1 after update", ht->count == 1);
    
    ht_del_hash_table(ht);
}

// Test 3: Delete operations
void test_delete() {
    print_test_header("Delete Operations");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("\nInserting items...\n");
    ht_insert(ht, "key1", "value1");
    ht_insert(ht, "key2", "value2");
    ht_insert(ht, "key3", "value3");
    
    printf("Before delete:\n");
    print_hash_table_stats(ht);
    
    printf("\nDeleting 'key2'...\n");
    ht_delete(ht, "key2");
    
    printf("After delete:\n");
    print_hash_table_stats(ht);
    
    char* deleted = ht_search(ht, "key2");
    print_test_result("Deleted key returns NULL", deleted == NULL);
    
    char* key1 = ht_search(ht, "key1");
    char* key3 = ht_search(ht, "key3");
    print_test_result("Other keys still accessible", 
                     key1 != NULL && key3 != NULL &&
                     strcmp(key1, "value1") == 0 && strcmp(key3, "value3") == 0);
    
    // Try deleting non-existent key
    printf("\nDeleting non-existent key...\n");
    int count_before = ht->count;
    ht_delete(ht, "nonexistent");
    print_test_result("Delete non-existent doesn't change count", ht->count == count_before);
    
    ht_del_hash_table(ht);
}

// Test 4: Collision handling
void test_collisions() {
    print_test_header("Collision Handling");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("\nInserting multiple items (testing collision resolution)...\n");
    
    // Insert many items to force collisions
    char key[20], value[20];
    int num_items = 20;
    
    for (int i = 0; i < num_items; i++) {
        sprintf(key, "key_%d", i);
        sprintf(value, "value_%d", i);
        ht_insert(ht, key, value);
    }
    
    print_hash_table_stats(ht);
    
    // Verify all items can be retrieved
    printf("\nVerifying all items...\n");
    int all_found = 1;
    for (int i = 0; i < num_items; i++) {
        sprintf(key, "key_%d", i);
        sprintf(value, "value_%d", i);
        char* result = ht_search(ht, key);
        if (result == NULL || strcmp(result, value) != 0) {
            printf("  [FAIL] Missing or incorrect: %s\n", key);
            all_found = 0;
        }
    }
    
    print_test_result("All items retrieved correctly", all_found);
    print_test_result("Count matches insertions", ht->count == num_items);
    
    ht_del_hash_table(ht);
}

// Test 5: Resize behavior
void test_resize() {
    print_test_header("Resize Behavior");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("\nInitial state:\n");
    int initial_size = ht->size;
    print_hash_table_stats(ht);
    
    printf("\nInserting many items to trigger resize up...\n");
    char key[20], value[30];
    int num_items = 50;
    
    for (int i = 0; i < num_items; i++) {
        sprintf(key, "item_%d", i);
        sprintf(value, "This is value %d", i);
        ht_insert(ht, key, value);
        
        if (i == 39) {
            printf("\nAfter 40 insertions:\n");
            print_hash_table_stats(ht);
        }
    }
    
    printf("\nAfter all insertions:\n");
    print_hash_table_stats(ht);
    print_test_result("Table resized up", ht->size > initial_size);
    
    // Verify all items still accessible after resize
    printf("\nVerifying all items after resize...\n");
    int all_found = 1;
    for (int i = 0; i < num_items; i++) {
        sprintf(key, "item_%d", i);
        sprintf(value, "This is value %d", i);
        char* result = ht_search(ht, key);
        if (result == NULL || strcmp(result, value) != 0) {
            all_found = 0;
            break;
        }
    }
    print_test_result("All items accessible after resize", all_found);
    
    ht_del_hash_table(ht);
}

// Test 6: Empty string keys and values
void test_edge_cases() {
    print_test_header("Edge Cases");
    
    ht_hash_table* ht = ht_new();
    if (ht == NULL) {
        printf("Failed to create hash table\n");
        return;
    }
    
    printf("\nTesting empty string value...\n");
    ht_insert(ht, "empty", "");
    char* empty_result = ht_search(ht, "empty");
    print_test_result("Empty string value stored", empty_result != NULL && strcmp(empty_result, "") == 0);
    
    printf("\nTesting long strings...\n");
    char long_key[100] = "this_is_a_very_long_key_name_for_testing_purposes";
    char long_value[200] = "This is a very long value string that contains a lot of text to test how the hash table handles longer strings and memory allocation";
    ht_insert(ht, long_key, long_value);
    char* long_result = ht_search(ht, long_key);
    print_test_result("Long strings handled correctly", 
                     long_result != NULL && strcmp(long_result, long_value) == 0);
    
    print_hash_table_stats(ht);
    
    ht_del_hash_table(ht);
}

int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║   HASH TABLE TEST SUITE                ║\n");
    printf("╔════════════════════════════════════════╗\n");
    
    test_basic_operations();
    print_separator();
    
    test_update_keys();
    print_separator();
    
    test_delete();
    print_separator();
    
    test_collisions();
    print_separator();
    
    test_resize();
    print_separator();
    
    test_edge_cases();
    
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║   ALL TESTS COMPLETED                  ║\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("\n");
    
    return 0;
}