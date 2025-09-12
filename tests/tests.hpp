#ifndef TESTS_HPP
#define TESTS_HPP

#include <string>
#include <vector>

struct Test {
    std::string test_name_;
    size_t cache_size_;
    std::vector<int> requests_;
    std::vector<int> expected_hits_;
};

void RunTests();

#endif // TESTS_HPP