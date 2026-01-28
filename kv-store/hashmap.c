#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

/* Initialise a map and the associated LRU */
Map *create_map() {
    Map *new_map = malloc(sizeof(Map));
    new_map->table = malloc(sizeof(List *)*MAP_BUCKETS);
    new_map->lru = create_list();
    // Initialise the table buckets
    for (int i=0;i<MAP_BUCKETS;i++)
        new_map->table[i] = create_list();

    new_map->num_entries = 0;
    return new_map;
}

Entry* new_entry(const char *key, unsigned char *value, int value_length) {
    // TODO copy the key and value locally in some memory bank
    Entry *e = malloc(sizeof(Entry));
    e->key = strdup(key);
    e->value = strndup((const char*)value, value_length);
    e->lru_entry = NULL;

    return e;
}

LRUEntry* new_lru_entry(List *l, Node *n) {
    LRUEntry *new_e = malloc(sizeof(LRUEntry));
    new_e->list = l;
    new_e->node = n;

    return new_e;
}


/* Stroustrup hash function */
static unsigned int stroustrup_hash(const char *key) {
    unsigned int val = 0;

    for (int i=0;key[i]!=0;i++){
        // Circular shift left 1 bit
        val = (val << 1) | (val >> (sizeof(val)*CHAR_BIT - 1));
        // XOR with key[i]
        val ^= key[i];
    }

    return val;
}

static unsigned int djb2_hash(const char *key) {
    unsigned long hash = 5381;
    int c;

    while (c= *key++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
#define STR     1
#define DJB2    2
#define MURMUR  3

#ifndef HASH
#define HASH STR
#endif

#if HASH == MURMUR
#include "murmur.h"
#endif

static unsigned int hash_function(const char *key){
#if HASH == STR
    return stroustrup_hash(key);
#elif HASH == DJB2
    return djb2_hash(key);
#elif HASH == MURMUR
    return murmur3_32(key, strlen(key), 1);
#endif
}

int mystrcmp(const char *s1, const char *s2){
    while (1){
        int res = ((*s1==0) || (*s1 != *s2));
        if (__builtin_expect((res),0)) break;
        ++s1; ++s2;
    }

    return (*s1-*s2);
}

/* Compare keys for two entries. Return 1 if same, 0 if diff */
int cmp(void *entry1_v, void *entry2_v) {
    char *key1 = ((Entry*) entry1_v)->key;
    char *key2 = ((Entry*) entry2_v)->key;

    //while (*key1 && !(*key1 ^ *key2)){
    //    key1++; key2++;
    //}
    //
    //return *key1 == *key2;
    return !strcmp(key1, key2);
}

Entry *insert(Map *map, const char *key, unsigned char *value, int value_length) {
    // Find the bucket
    unsigned int bucket_id = hash_function(key) % MAP_BUCKETS;
    List *bucket = map->table[bucket_id];
    Entry *e = new_entry(key, value, value_length);
    // New key, append to this bucket
    // check capacity, evict if necessary
    if (map_is_full(map)) {
        // Map is full, we must evict the LRU
        LRUEntry *lru_e = (LRUEntry*)map->lru->tail->data;
        // Free the map node data
        free(lru_e->node->data);
        delete_node(lru_e->list, lru_e->node);
        delete_node(map->lru, map->lru->tail);
        // Free the LRU entry
        free(lru_e);
        map->num_entries --;
    }
    Node *new_node = insert_tail(bucket, (void*)e);
    map->num_entries ++;
    // Update LRU, insert new entry at the head
    LRUEntry *lru_e = new_lru_entry(bucket, new_node);
    Node *lru_n = insert_head(map->lru, (void*)lru_e);
    // Connect new lru node to map entry for fast lookup
    e->lru_entry = lru_n;

    return e;
}

Entry *put(Map *map, const char *key, unsigned char *value, int value_length) {
    // Find the bucket
    unsigned int bucket_id = hash_function(key) % MAP_BUCKETS;
    List *bucket = map->table[bucket_id];

    Entry *e = new_entry(key, value, value_length);
    Node *entry_node = find_node(bucket, e, cmp);
    if (!entry_node) {
        // New key, append to this bucket
        // check capacity, evict if necessary
        if (map_is_full(map)) {
            // Map is full, we must evict the LRU
            LRUEntry *lru_e = (LRUEntry*)map->lru->tail->data;
            // Free the map node data
            free(lru_e->node->data);
            delete_node(lru_e->list, lru_e->node);
            delete_node(map->lru, map->lru->tail);
            // Free the LRU entry
            free(lru_e);
            map->num_entries --;
        }
        Node *new_node = insert_tail(bucket, (void*)e);
        map->num_entries ++;
        // Update LRU, insert new entry at the head
        LRUEntry *lru_e = new_lru_entry(bucket, new_node);
        Node *lru_n = insert_head(map->lru, (void*)lru_e);
        // Connect new lru node to map entry for fast lookup
        e->lru_entry = lru_n;
    }else{
        // The key exists, just update its contents
        // First get the LRU node from the existing entry
        Node *lru_n = ((Entry*)entry_node->data)->lru_entry;
        if (!lru_n){
            // TODO panic
            free(e);
            return NULL;
        }
        e->lru_entry = lru_n;
        node_replace_data(entry_node, e);  // TODO check error
        // Move the LRU node to the front of the list
        move_to_head(map->lru, lru_n);
    }

    return e;
}

Entry *get(Map *map, const char *key) {
    // Find the bucket
    unsigned int bucket_id = hash_function(key) % MAP_BUCKETS;
    List *bucket = map->table[bucket_id];

    Entry tmp = {key, NULL};
    Node *entry_node = find_node(bucket, &tmp, cmp);
    if (!entry_node) return NULL;
    Entry *e = (Entry*)entry_node->data;
    if (!e) return NULL;

    // Move the node to the front of the LRU list
    move_to_head(map->lru, e->lru_entry);

    return e;
}

int map_is_full(Map *map) {
    return map->num_entries == MAP_CAPACITY;
}

static void print_lru_entry(void *lru_data) {
    LRUEntry *lru_e = (LRUEntry*)lru_data;
    Entry *e = (Entry*)lru_e->node->data;
    printf("%s->", e->key);
}

void print_lru(Map *map) {
    print_list(map->lru, print_lru_entry);
    printf("NULL\n");
}
