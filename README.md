---
geometry:
- margin=1in
author:
- Victor Cionca
title: ''
---

Code for the MTU "SOFT8043 Systems performance at scale" module.

Contents

* key value store in several iterations showing optimisations for
    * minimising cache misses
    * maximising user-space to kernel-space throughput
    * maximising network throughput per core
* example code for
    * cache optimisation
    * zero-copy system calls
    * network throughput.


# Optimising for memory access

* data structures used in the k-v store

# Optimising for user-kernel space throughput

* sending data back to the user from the k-v store
    * basic TCP Socket
    * asyncio, zero copy (MSG\_ZEROCOPY)

# Optimising for networking

* multi-threaded execution of the k-v store
