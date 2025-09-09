#include <iostream>

#include "cache.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {
    TwoQCache<int> c(10);

    for (size_t i = 0; i != 5; ++i) {
        std::cout << c.LookupUpdate(i, SlowGetPage) << "\n";
    }
    c.PrintCache();
    std::cout << c.LookupUpdate(4, SlowGetPage) << "\n";
    c.PrintCache();
    return 0;
}
