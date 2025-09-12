#include <iostream>

#include "caches.hpp"

int SlowGetPage(int key) {
    return key;
}

int main() {
    size_t size = 0;
    size_t number_of_elements = 0;
    std::cin >> size >> number_of_elements;

    if (!std::cin.good()) {
        std::cerr << "Input error: expected two integers (size, count)" << std::endl;
        return 1;
    }

    caches::TwoQCache<int> cache{size};

    size_t number_of_hits = 0;
    for (size_t i = 0; i != number_of_elements; ++i) {
        int num = 0;
        std::cin >> num;
        if (!std::cin.good()) {
            std::cerr << "Input error: expected integer at element " << i + 1 << std::endl;
            return 1;
        }
        number_of_hits += cache.LookupUpdate(num, SlowGetPage);
    }
    std::cout << number_of_hits << "\n";

    return 0;
}
