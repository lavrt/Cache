#include <iostream>

#include "cache.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {
    caches::TwoQCache<int> c{6};

    int m = 0;
    while (std::cin >> m) {
        std::cout << (c.LookupUpdate(m, SlowGetPage) ? "hit" : "miss") << "\n";
        c.PrintCache();
    }
    
    return 0;
}
