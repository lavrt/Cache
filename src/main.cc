#include <iostream>

#include "cache.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {
    // caches::TwoQCache<int> c{6};

    // int m = 0;
    // while (std::cin >> m) {
    //     std::cout << (c.LookupUpdate(m, SlowGetPage) ? "hit" : "miss") << "\n";
    //     c.PrintCache();
    // }

    std::vector<int> req {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    caches::IdealCache<int> idl{3, req};
    
    for (int n : req) {
        std::cout << n << ": " << (idl.LookupUpdate(n, SlowGetPage) ? "hit" : "miss") << "\n";
        idl.PrintCache();
    }

    return 0;
}
