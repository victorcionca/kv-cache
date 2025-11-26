---
geometry:
- margin=1in
author:
- Victor Cionca
title: ''
---

A basic key-value cache for on disk data that

* stores the data on multiple file on disk, somewhat like sharding
* provides a network socket for serving data to clients based on key
* the data is cached in memory for fast access in a hash map

# High level design

* keys are strings of a max length looking like paths, e.g. /first/second/third
* data is binary blob
* a hash map is used to store the key-value pairs
    * keys are hashed
    * buckets for the same hash contain linked lists of k-v pointers
    * k-v data is stored elsewhere, to reduce memory use (see memcached)
* a 2-linked list is used to implement LRU policy
    * the recently accessed nodes are laced at the start of the list

# Iterations

1. 

* doubly-linked list for LRU
* hash map
    * hash function
        * Stroustrup function
        * start with unsigned integer = 0
        * shift left 1 bit and XOR with value of next character
        * repeat for all characters
        * shift should be circular

# Evaluation

* basic testing
    * create map with smaller capacity
    * generate a number of put requests to get some data in but not to full capacity
    * generate a number of random put/get requests and observe the LRU
* performance testing
    * fill in the table with lots of data; put requests
        * should be more than the size of the cache
    * start issuing get requests for random keys
    
