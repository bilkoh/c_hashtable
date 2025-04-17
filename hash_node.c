#include "hash_node.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

hash_node_t*
hn_create() {
    // using calloc because it initializes memory to zero
    hash_node_t* node = calloc(1, sizeof(hash_node_t));
    if (!node) {
        return NULL;
    }

    return node;
}

hash_node_t*
hn_destroy(hash_node_t* node) {
    if (node) {
        free(node->full_name);
        free(node->address);
        free(node->city);
        free(node->state);
        free(node->zip);
        free(node);
    }
    return NULL;
}

bool
hn_is_head(hash_node_t* node) {
    return node->prev == NULL;
}

bool
hn_is_tail(hash_node_t* node) {
    return node->next == NULL;
}

void
hn_insert_after(hash_node_t* node, hash_node_t* new_node) {
    if (node && new_node) {
        new_node->next = node->next;
        new_node->prev = node;

        if (node->next) {
            node->next->prev = new_node;
        }

        node->next = new_node;
    }
}

void
hn_insert_before(hash_node_t* node, hash_node_t* new_node) {
    if (node && new_node) {
        new_node->prev = node->prev;
        new_node->next = node;

        if (node->prev) {
            node->prev->next = new_node;
        }

        node->prev = new_node;
    }
}

void
hn_remove(hash_node_t* node) {
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
}
