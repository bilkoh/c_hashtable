#ifndef HASH_NODE_H
#define HASH_NODE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct hash_node {
    int key;
    int hash_code;
    char* full_name;
    char* address;
    char* city;
    char* state;
    char* zip;
    struct hash_node* prev;
    struct hash_node* next;
} hash_node_t;

/**
 * Creates a new hash node
 * @return Pointer to the newly created hash node, or NULL if allocation failed
 */
hash_node_t* hn_create();

/**
 * Destroys a hash node and frees all associated memory
 * @param node Pointer to the hash node to destroy
 * @return Always returns NULL
 */
hash_node_t* hn_destroy(hash_node_t* node);

/**
 * Checks if a node is the head of a linked list
 * @param node The node to check
 * @return true if the node is a head (no previous node), false otherwise
 */
bool hn_is_head(hash_node_t* node);

/**
 * Checks if a node is the tail of a linked list
 * @param node The node to check
 * @return true if the node is a tail (no next node), false otherwise
 */
bool hn_is_tail(hash_node_t* node);

/**
 * Inserts a new node after the specified node
 * @param node The node after which to insert
 * @param new_node The node to insert
 */
void hn_insert_after(hash_node_t* node, hash_node_t* new_node);

/**
 * Inserts a new node before the specified node
 * @param node The node before which to insert
 * @param new_node The node to insert
 */
void hn_insert_before(hash_node_t* node, hash_node_t* new_node);

/**
 * Removes a node from its linked list without freeing it
 * @param node The node to remove
 */
void hn_remove(hash_node_t* node);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HASH_NODE_H */