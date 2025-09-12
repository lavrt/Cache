# Cache Algorithms Implementation

This project provides C++ implementations of two cache replacement algorithms: the 2Q Cache and the Ideal Cache. The 2Q Cache is an efficient approximation of the LRU-2 algorithm, designed to handle both recency and frequency in page accesses. The Ideal Cache is an optimal (but impractical for real-time use) algorithm that assumes knowledge of future requests to make eviction decisions.

## Features

- ##### 2Q Cache:
    - Uses three queues: A1_in (recently accessed), Am (frequently accessed), and A1_out (ghost queue for evicted from A1_in).
    - Configurable ratio for Am queue size (default 0.5).
    - Supports generic key and value types (default key is int).
- ##### Ideal Cache:
    - Evicts the page that will be requested furthest in the future (Belady's optimal algorithm).
    - Requires the full sequence of requests in advance.
    - Supports generic key and value types (default key is int).
- ##### Testing:
    - Tests for both caches with expected hit/miss sequences.
    - Comparative tests to show hit rates between 2Q and Ideal caches.

## Requirements
- C++20 or later.
- Compiler: GCC or Clang.
- No external dependencies.
