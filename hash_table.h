
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include "hash_node.h" // Assuming hash_node.h defines hash_node_t

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct hash_table {
    hash_node_t** table;
    int num_buckets;
    int num_items;
} hash_table_t;

/**
 * @brief Creates a new hash table.
 *
 * Allocates memory for a new hash table structure and its underlying bucket array.
 *
 * @param ht_size The desired number of buckets in the hash table. Must be positive.
 * @return A pointer to the newly created hash table, or NULL if allocation fails or ht_size is invalid.
 */
hash_table_t* ht_create(int ht_size);

/**
 * @brief Removes all items from the hash table.
 *
 * Frees all nodes within the hash table but does not free the table structure itself.
 * Resets the item count to zero.
 *
 * @param ht A pointer to the hash table to clear. If NULL, the function does nothing.
 */
void ht_clear(hash_table_t* ht);

/**
 * @brief Destroys a hash table and frees all associated memory.
 *
 * Clears the hash table (frees all nodes), frees the bucket array, and finally frees
 * the hash table structure itself.
 *
 * @param ht A pointer to the hash table to destroy.
 * @return Always returns NULL, suitable for assigning back to the pointer variable (e.g., ht = ht_destroy(ht);).
 */
hash_table_t* ht_destroy(hash_table_t* ht);

/**
 * @brief Retrieves the hash node associated with a given key.
 *
 * Searches the hash table for a node matching the specified key.
 *
 * @param ht A pointer to the hash table to search (const, as it's not modified).
 * @param key The key of the node to find.
 * @return A pointer to the found hash_node_t, or NULL if the key is not found or the table is invalid.
 */
hash_node_t* ht_get_node(const hash_table_t* ht, int key);

/**
 * @brief Checks if a key exists within the hash table.
 *
 * @param ht A pointer to the hash table to check (const).
 * @param key The key to look for.
 * @return true if the key is found in the table, false otherwise or if the table is invalid.
 */
bool ht_contains(const hash_table_t* ht, int key);

/**
 * @brief Removes a node with the specified key from the hash table.
 *
 * Finds the node associated with the key, unlinks it from the bucket's list,
 * and frees the node's memory. Decrements the item count.
 *
 * @param ht A pointer to the hash table.
 * @param key The key of the node to remove.
 * @return true if the node was found and removed, false otherwise (e.g., key not found, invalid table).
 */
bool ht_remove(hash_table_t* ht, int key);

/**
 * @brief Adds a new node to the hash table with the specified key.
 *
 * Associates the given node with the key and inserts it into the appropriate bucket.
 * If a node with the same key already exists, it is removed first (effectively replacing it).
 * The function takes ownership of the provided `node` if successful.
 * The `node`'s key field will be overwritten with the provided `key`.
 * The `node`'s next/prev pointers will be managed by the hash table.
 *
 * @param ht A pointer to the hash table.
 * @param key The key to associate with the node.
 * @param node A pointer to the hash_node_t to add. Must not be NULL.
 * @return true if the node was successfully added, false otherwise (e.g., invalid table, invalid node).
 */
bool ht_add(hash_table_t* ht, int key, hash_node_t* node);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // HASHTABLE_H