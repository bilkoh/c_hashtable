#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash_node.h"
#include "hash_table.h"

static inline int
calc_hash_code(int key, int num_buckets) {
    // guard against bad bucket count
    if (num_buckets <= 0) {
        return 0;
    }
    // C’s % can go negative, so normalize
    int h = key % num_buckets;
    return (h < 0) ? h + num_buckets : h;
}

// Helper function to create a simple hash node
static hash_node_t*
create_test_node(int value) {
    hash_node_t* node = calloc(1, sizeof(*node));
    // In a real scenario, node->data might point to something more complex
    // For testing, we can store the value directly or use a simple struct
    // Let's assume node->data is just an int for simplicity here,
    // although the original hash_node.h might define it differently.
    // If hash_node_t doesn't have a 'value' field, adjust accordingly.
    // Let's assume we store the value in the key itself for this test.
    if (node) {
        node->key = value; // Using key to store a simple value for test
    }
    return node;
}

// Test basic creation and destruction
void
test_ht_create_destroy() {
    printf("Running test_ht_create_destroy...\n");
    hash_table_t* ht = ht_create(10);
    assert(ht != NULL);
    assert(ht->num_buckets == 10);
    assert(ht->num_items == 0);
    assert(ht->table != NULL);
    ht = ht_destroy(ht);
    assert(ht == NULL);

    // Test creation with zero size (should handle gracefully or as defined)
    // Assuming ht_create handles 0 size by allocating, but num_buckets is 0
    // Or adjust based on actual ht_create behavior for ht_size <= 0
    ht = ht_create(0);
    assert(ht != NULL);        // ht_create allocates the struct itself
    assert(ht->table != NULL); // ht_create allocates the table array
    assert(ht->num_buckets == 0);
    assert(ht->num_items == 0);
    ht = ht_destroy(ht);
    assert(ht == NULL);

    // Test creation failure (simulate calloc failure - hard to test directly)
    // We can test destroying NULL
    ht = ht_destroy(NULL);
    assert(ht == NULL);

    printf("test_ht_create_destroy PASSED\n");
}

// Test adding items and checking containment
void
test_ht_add_contains() {
    printf("Running test_ht_add_contains...\n");
    hash_table_t* ht = ht_create(5);
    assert(ht != NULL);

    hash_node_t* node1 = create_test_node(101);
    assert(node1 != NULL);
    bool added = ht_add(ht, 1, node1);
    assert(added == true);
    assert(ht->num_items == 1);
    assert(ht_contains(ht, 1) == true);
    assert(ht_contains(ht, 2) == false);

    hash_node_t* node2 = create_test_node(102);
    assert(node2 != NULL);
    added = ht_add(ht, 6, node2); // Should hash to the same bucket as 1 (6 % 5 = 1)
    assert(added == true);
    assert(ht->num_items == 2);
    assert(ht_contains(ht, 1) == true);
    assert(ht_contains(ht, 6) == true);

    // Check node retrieval
    hash_node_t* retrieved_node = ht_get_node(ht, 1);
    assert(retrieved_node == node1);
    assert(retrieved_node->key == 1); // Key stored in node by ht_add

    retrieved_node = ht_get_node(ht, 6);
    assert(retrieved_node == node2);
    assert(retrieved_node->key == 6);

    retrieved_node = ht_get_node(ht, 99);
    assert(retrieved_node == NULL);

    ht = ht_destroy(ht); // Frees nodes added
    printf("test_ht_add_contains PASSED\n");
}

// Test removing items
void
test_ht_remove() {
    printf("Running test_ht_remove...\n");
    hash_table_t* ht = ht_create(5);
    assert(ht != NULL);

    hash_node_t* node1 = create_test_node(201);
    hash_node_t* node2 = create_test_node(202);
    hash_node_t* node3 = create_test_node(203);
    ht_add(ht, 1, node1);
    ht_add(ht, 6, node2); // Same bucket as 1
    ht_add(ht, 3, node3); // Different bucket

    assert(ht->num_items == 3);
    assert(ht_contains(ht, 1) == true);
    assert(ht_contains(ht, 6) == true);
    assert(ht_contains(ht, 3) == true);

    // Remove head of a chain
    bool removed = ht_remove(ht, 1);
    assert(removed == true);
    assert(ht->num_items == 2);
    assert(ht_contains(ht, 1) == false);
    assert(ht_contains(ht, 6) == true); // node2 should now be head
    assert(ht_get_node(ht, 6) == node2);
    int idx1 = calc_hash_code(1, ht->num_buckets);
    assert(ht->table[idx1] == node2); // node2 is the new head for bucket 1
    assert(node2->prev == NULL);      // node2 should be the head

    // Remove tail of a chain (which is now node2)
    removed = ht_remove(ht, 6);
    assert(removed == true);
    assert(ht->num_items == 1);
    assert(ht_contains(ht, 6) == false);
    assert(ht->table[idx1] == NULL); // Bucket 1 should be empty

    // Remove item in another bucket
    removed = ht_remove(ht, 3);
    assert(removed == true);
    assert(ht->num_items == 0);
    assert(ht_contains(ht, 3) == false);
    int idx3 = calc_hash_code(3, ht->num_buckets);
    assert(ht->table[idx3] == NULL);

    // Try removing non-existent item
    removed = ht_remove(ht, 99);
    assert(removed == false);
    assert(ht->num_items == 0);

    ht = ht_destroy(ht);
    printf("test_ht_remove PASSED\n");
}

// Test clearing the hash table
void
test_ht_clear() {
    printf("Running test_ht_clear...\n");
    hash_table_t* ht = ht_create(3);
    assert(ht != NULL);

    ht_add(ht, 1, create_test_node(301));
    ht_add(ht, 4, create_test_node(304)); // Same bucket as 1
    ht_add(ht, 2, create_test_node(302));

    assert(ht->num_items == 3);
    assert(ht_contains(ht, 1) == true);
    assert(ht_contains(ht, 4) == true);
    assert(ht_contains(ht, 2) == true);

    ht_clear(ht);

    assert(ht->num_items == 0);
    assert(ht_contains(ht, 1) == false);
    assert(ht_contains(ht, 4) == false);
    assert(ht_contains(ht, 2) == false);
    assert(ht_get_node(ht, 1) == NULL);
    assert(ht_get_node(ht, 4) == NULL);
    assert(ht_get_node(ht, 2) == NULL);

    // Check buckets are NULL
    for (int i = 0; i < ht->num_buckets; i++) {
        assert(ht->table[i] == NULL);
    }

    // Add again after clear
    bool added = ht_add(ht, 5, create_test_node(305));
    assert(added == true);
    assert(ht->num_items == 1);
    assert(ht_contains(ht, 5) == true);

    ht = ht_destroy(ht);
    printf("test_ht_clear PASSED\n");
}

// Test adding an item that already exists (should replace)
void
test_ht_add_replace() {
    printf("Running test_ht_add_replace...\n");
    hash_table_t* ht = ht_create(5);
    assert(ht != NULL);

    hash_node_t* node1 = create_test_node(401);
    ht_add(ht, 1, node1);
    assert(ht->num_items == 1);
    assert(ht_get_node(ht, 1) == node1);

    hash_node_t* node2 = create_test_node(402); // New node for the same key
    bool added = ht_add(ht, 1, node2);          // Add with the same key
    assert(added == true);
    assert(ht->num_items == 1); // Count should remain 1 because it's a replacement
    assert(ht_contains(ht, 1) == true);
    assert(ht_get_node(ht, 1) == node2); // Should now point to node2
    // node1 should have been freed by the remove inside add

    // Add another node to check list integrity after replacement
    hash_node_t* node3 = create_test_node(403);
    ht_add(ht, 6, node3); // Same bucket
    assert(ht->num_items == 2);
    assert(ht_contains(ht, 6));
    assert(ht_get_node(ht, 6) == node3);

    // Check the chain for bucket 1 (1 % 5 = 1, 6 % 5 = 1)
    int idx = calc_hash_code(1, ht->num_buckets);
    assert(ht->table[idx] == node2); // Head should be node2 (the replacement)
    assert(node2->next == node3);
    assert(node3->prev == node2);
    assert(node3->next == NULL);

    ht = ht_destroy(ht);
    printf("test_ht_add_replace PASSED\n");
}

// Test edge cases and null inputs
void
test_ht_edge_cases() {
    printf("Running test_ht_edge_cases...\n");
    hash_table_t* ht = ht_create(10);     // Normal HT
    hash_table_t* ht_zero = ht_create(0); // Zero bucket HT
    hash_node_t* node = create_test_node(501);

    // Operations on NULL hash table
    assert(ht_destroy(NULL) == NULL);
    ht_clear(NULL); // Should not crash
    assert(ht_get_node(NULL, 1) == NULL);
    assert(ht_contains(NULL, 1) == false);
    assert(ht_remove(NULL, 1) == false);
    assert(ht_add(NULL, 1, node) == false);

    // Operations on zero-bucket hash table
    assert(ht_zero != NULL);
    assert(ht_zero->num_buckets == 0);
    assert(ht_get_node(ht_zero, 1) == NULL);   // calc_hash_code returns 0, but table size is 0
    assert(ht_contains(ht_zero, 1) == false);  // ht_contains checks num_buckets > 0
    assert(ht_remove(ht_zero, 1) == false);    // ht_remove checks num_buckets <= 0
    assert(ht_add(ht_zero, 1, node) == false); // ht_add checks num_buckets <= 0
    ht_clear(ht_zero);                         // Should handle 0 buckets
    assert(ht_zero->num_items == 0);
    ht_zero = ht_destroy(ht_zero);
    assert(ht_zero == NULL);

    // Operations with NULL node
    assert(ht_add(ht, 1, NULL) == false);
    assert(ht->num_items == 0);

    // Test calc_hash_code directly
    assert(calc_hash_code(5, 10) == 5);
    assert(calc_hash_code(-1, 10) == 9); // Handles negative keys
    assert(calc_hash_code(-11, 10) == 9);
    assert(calc_hash_code(0, 10) == 0);
    assert(calc_hash_code(10, 10) == 0);
    assert(calc_hash_code(5, 0) == 0);  // Handles zero buckets
    assert(calc_hash_code(5, -5) == 0); // Handles negative buckets

    free(node); // Free the node used for NULL tests
    ht = ht_destroy(ht);
    printf("test_ht_edge_cases PASSED\n");
}

int
main() {
    test_ht_create_destroy();
    test_ht_add_contains();
    test_ht_remove();
    test_ht_clear();
    test_ht_add_replace();
    test_ht_edge_cases();

    printf("\nAll hash table tests passed!\n");
    return 0;
}