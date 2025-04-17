#include "hash_table.h"
#include <stdbool.h>
#include <stdlib.h>
#include "hash_node.h"

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

hash_table_t*
ht_create(int ht_size) {
    hash_table_t* ht = calloc(1, sizeof(*ht));
    if (!ht) {
        return NULL;
    }

    ht->table = calloc(ht_size, sizeof(ht->table));
    if (!ht->table) {
        free(ht);
        return NULL;
    }

    ht->num_buckets = ht_size;
    ht->num_items = 0;
    return ht;
}

void
ht_clear(hash_table_t* ht) {
    if (!ht) {
        return;
    }

    for (int i = 0; i < ht->num_buckets; i++) {
        hash_node_t* node = ht->table[i];

        while (node) {
            hash_node_t* next = node->next;
            free(node);
            node = next;
        }
        ht->table[i] = NULL;
    }

    ht->num_items = 0;
}

hash_table_t*
ht_destroy(hash_table_t* ht) {
    if (!ht) {
        return NULL;
    }

    ht_clear(ht);
    free(ht->table);
    free(ht);
    return NULL;
}

hash_node_t*
ht_get_node(const hash_table_t* ht, int key) {
    // Check for null table *and* zero buckets before proceeding
    if (!ht || ht->num_buckets <= 0) {
        return NULL;
    }

    int idx = calc_hash_code(key, ht->num_buckets);
    // Now it's safe to access ht->table[idx] because num_buckets > 0
    for (hash_node_t* n = ht->table[idx]; n; n = n->next) {
        if (n->key == key) {
            return n;
        }
    }

    return NULL;
}

bool
ht_contains(const hash_table_t* ht, int key) {
    return ht && ht->num_buckets > 0 // avoid 0‑length buckets
           && ht_get_node(ht, key);
}

bool
ht_remove(hash_table_t* ht, int key) {
    if (!ht || ht->num_buckets <= 0) {
        return false;
    }

    int idx = calc_hash_code(key, ht->num_buckets);
    hash_node_t* n = ht->table[idx];
    while (n && n->key != key) {
        n = n->next;
    }
    if (!n) {
        return false;
    }

    // unlink n
    if (n->prev) {
        n->prev->next = n->next;
    } else {
        // n was head of bucket
        ht->table[idx] = n->next;
    }
    if (n->next) {
        n->next->prev = n->prev;
    }

    free(n);
    ht->num_items--;
    return true;
}

bool
ht_add(hash_table_t* ht, int key, hash_node_t* node) {
    if (!ht || ht->num_buckets <= 0 || !node) {
        return false;
    }

    // ensure new node doesnt have bad links
    node->key = key;
    node->next = NULL;
    node->prev = NULL;

    if (ht_contains(ht, key)) {
        ht_remove(ht, key); // replace, not append, dupes
    }

    int idx = calc_hash_code(key, ht->num_buckets);
    if (ht->table[idx]) {
        // find tail
        hash_node_t* tail = ht->table[idx];
        while (!hn_is_tail(tail)) {
            tail = tail->next;
        }

        hn_insert_after(tail, node);

    } else {
        ht->table[idx] = node;
    }

    ht->num_items++;
    return true;
}
