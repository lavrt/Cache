#include <iostream>

#include "caches.hpp"
#include "tests.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {
    // caches::TwoQCache<int> c{5};

    // std::vector<int> d;
    // std::vector<int> r {1, 2, 3, 1, 2, 4, 5, 1, 2, 3};
    // for (int el : r) {
    //     d.push_back(c.LookupUpdate(el, SlowGetPage));
    //     c.PrintCache();
    // }
    // for (int e : d) {
    //     std::cout << e << ", ";
    // }
    // std::cout << "\n";










    // std::vector<int> req {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    // caches::IdealCache<int> idl{3, req};
    
    // for (int n : req) {
    //     std::cout << n << ": " << (idl.LookupUpdate(n, SlowGetPage) ? "hit" : "miss") << "\n";
    //     idl.PrintCache();
    // }

    RunTests();

    return 0;
}
