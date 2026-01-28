#ifndef __HASH_H_
#define __HASH_H_
#include "list.h"

/**
 * The hash map is implemented as a fixed-length array of list heads.
 * The map will also have a capacity, which takes into account the elements 
 * that fall in the buckets.
 * We have a hash function that leads to an index in the array.
 * In addition we have the LRU list that decides which entry in the map
 * should be removed
 */

#define MAP_CAPACITY    16000
#define MAP_BUCKETS     2048

/* Entry in the LRU list; refs the table bucket (list) and the node in the bucket */
typedef struct {
    List *list;
    Node *node;
}LRUEntry;

/* Key-value entries in the table */
typedef struct {
    const char *key;
    unsigned char *value;
    int value_length;
    Node *lru_entry; // pointer to node in LRU list for fast lookup
}Entry;

typedef struct {
    List **table; // The list of buckets
    List *lru;    // The LRU list, each node contains a pointer to a table node
    unsigned int num_entries; // Current number of entries in the table
}Map;

/* Create a new map */
Map *create_map();
Entry *new_entry(const char *key, unsigned char *value, int value_length);
LRUEntry *new_lru_entry(List *l, Node *n);
/*
 * Put a value inside the map corresponding to the key.
 * If it is a new key then insert it in the table.
 * If it already exists then update its value.
 * If the map is full the LRU entry will be evicted.
 */
Entry *put(Map *map, const char *key, unsigned char *value, int value_length);
/*
 * Insert a new key into the map.
 * Does not check the existence of the key. The new entry is appended to
 * the end of the bucket, if there is capacity.
 */
Entry *insert(Map *map, const char *key, unsigned char *value, int value_length);
/* Get a value from the map, if it exists, otherwise return NULL */
Entry *get(Map *map, const char *key);
void destroy_map(Map *map);
/* Checks if a map is full */
int map_is_full(Map *map);
/* Prints the LRU */
void print_lru(Map *map);
#endif
