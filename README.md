# Cache Algorithms Implementation

This project provides C++ implementations of two cache replacement algorithms: the 2Q Cache and the Ideal Cache. The 2Q Cache is an efficient approximation of the LRU-2 algorithm, designed to handle both recency and frequency in page accesses. The Ideal Cache is an optimal (but impractical for real-time use) algorithm that assumes knowledge of future requests to make eviction decisions.

## Features

- #### 2Q Cache:
    - Uses three queues: A1_in (recently accessed), Am (frequently accessed), and A1_out (ghost queue for evicted from A1_in).
    - Configurable ratio for Am queue size (default 0.5).
    - Supports generic key and value types (default key is int).
- #### Ideal Cache:
    - Evicts the page that will be requested furthest in the future (Belady's optimal algorithm).
    - Requires the full sequence of requests in advance.
    - Supports generic key and value types (default key is int).
- #### Testing:
    - Tests for both caches with expected hit/miss sequences.
    - Comparative tests to show hit rates between 2Q and Ideal caches.

## Requirements
- C++20 or later.
- Compiler: GCC or Clang.
- No external dependencies.

## Project Structure
```
Cache/
├── include/
│ └── caches.hpp
├── src/
| ├── main_2q_cache.cc
│ └── main_ideal_cache.cc
├── tests/
| ├── tests.cc
│ └── tests.hpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Building and Running
1. Configure and Build:
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
2. Run Main Application:
```bash
./build/tq_cache           # to use the 2Q Cache

./build/ideal_cache        # to use the Ideal Cache
```
3. Run Tests 
```bash
./build/tests              # Run tests directly

cd build && ctest          # Basic output

cd build && ctest -V       # Verbose output
```

## Usage
Include ```caches.hpp``` in your project and use the cache classes as follows:

### 2Q Cache Example
```cpp
#include "caches.hpp"

int SlowGetPage(int key) {
    // Simulate slow page fetch
    return key;
}

int main() {
    caches::TwoQCache<int> cache(10, 0.75);         // Capacity 10, Am ratio is 0.75 (default is 0.5)
    bool hit = cache.LookupUpdate(1, SlowGetPage);  // Returns false (miss), inserts
    hit = cache.LookupUpdate(1, SlowGetPage);       // Returns true (hit)
    return 0;
}
```

### Ideal Cache Example
```cpp
#include "caches.hpp"

int SlowGetPage(int key) {
    // Simulate slow page fetch
    return key;
}

int main() {
    std::vector<int> requests = {1, 2, 3, 1, 4};  // future request sequence
    caches::IdealCache<int> cache(3, requests);   // Capacity 3, provide full requests
    bool hit = cache.LookupUpdate(1, SlowGetPage);
    return 0;
}
```

## Algorithm Details

### 2Q Cache
- **A1_in**: FIFO queue for new pages.
- **Am**: LRU queue for frequently accessed pages (promoted from A1_in on hit).
- **A1_out**: Ghost queue to track evicted pages from A1_in; promotes to Am if hit.
- Eviction: From A1_in to A1_out, or from Am if full.
- Goal: Balances recency and frequency without full LRU overhead.

### Ideal Cache
- Assumes perfect knowledge of future requests.
- On eviction, removes the page with the furthest next access (or never accessed).
- Used as a benchmark for cache efficiency.

## Tests

The ```tests.cc``` file includes:
- Predefined test cases for 2Q and Ideal caches with expected hit/miss patterns.
- Comparative runs showing hit percentages for both caches on the same request sequences.

Example output snippet:
```
Running cache tests
=======================================
Running 2Q cache tests
...
2Q cache tests passed successfully
=======================================
Running ideal cache tests
...
Ideal cache tests passed successfully
=======================================
Running comparative tests (cache_capacity = 3)
...
=======================================
Testing complete
```
