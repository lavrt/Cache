#include "tests.hpp"

#include <iostream>
#include <cassert>

#include "cache.hpp"

namespace colors {
    const std::string kRed = "\033[31m";
    const std::string kGreen = "\033[32m";
    const std::string kReset = "\033[0m";
};

// static ------------------------------------------------------------------------------------------

static const std::vector<Test> kTwoQCacheTests {
    {"2q_test_1", 4, {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4},  {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1}},
    {"2q_test_2", 8, {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4},  {0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1}},
    {"2q_test_3", 6, {1, 2, 3, 1, 4, 2, 5, 3, 4, 5},        {0, 0, 0, 1, 0, 1, 0, 1, 1, 1}},
    {"2q_test_4", 2, {1, 1, 1, 1, 1},                       {0, 1, 1, 1, 1}},
    {"2q_test_5", 5, {1, 2, 3, 1, 2, 4, 5, 1, 2, 3},        {0, 0, 0, 1, 1, 0, 0, 1, 1, 1}},
};

static const std::vector<Test> kIdealCacheTests {
    {"ideal_test_1", 3, {1, 2, 3, 1, 4, 2, 5, 3, 4, 5},    {0, 0, 0, 1, 0, 1, 0, 1, 1, 1}},
    {"ideal_test_2", 2, {1, 2, 1, 3, 2, 4, 1, 5},          {0, 0, 1, 0, 1, 0, 0, 0}},
    {"ideal_test_3", 4, {1, 2, 3, 4, 1, 2, 5, 1, 2, 3},    {0, 0, 0, 0, 1, 1, 0, 1, 1, 1}},
    {"ideal_test_4", 1, {1, 1, 1, 1, 1},                   {0, 1, 1, 1, 1}},
    {"ideal_test_5", 3, {1, 2, 3, 4, 5, 1, 2, 3},          {0, 0, 0, 0, 0, 1, 1, 0}},
};

static void RunTwoQCahceTests();
static void RunIdealCacheTests();

// global ------------------------------------------------------------------------------------------

void RunTests() {
    std::cout << "Running cache tests\n";
    std::cout << "=======================================\n";

    RunTwoQCahceTests();
    RunIdealCacheTests();
    
    std::cout << "=======================================\n";
    std::cout << "Testing complete\n";
}

// static ------------------------------------------------------------------------------------------

static void RunTwoQCahceTests() {
    std::cout << "Running 2Q cache tests\n";
    
    for (const Test& test : kTwoQCacheTests) {
        assert(test.requests_.size() == test.expected_hits_.size());

        caches::TwoQCache<int> cache{test.cache_size_};

        for (size_t i = 0; i != test.requests_.size(); ++i) {
            bool actual_hit = cache.LookupUpdate(test.requests_[i], [](int x) { return x; });
            bool expected_hit = test.expected_hits_[i];

            if (actual_hit != expected_hit) {
                throw std::runtime_error(colors::kRed + "Test " + test.test_name_ +
                                         ": Request " + std::to_string(i + 1) + 
                                         " (key=" + std::to_string(test.requests_[i]) + 
                                         "): expected " + (expected_hit ? "HIT" : "MISS") + 
                                         ", got " + (actual_hit ? "HIT" : "MISS") + colors::kReset);
            }
        }
    }

    std::cout << colors::kGreen << "2Q cache tests passed successfully\n" << colors::kReset;
}

static void RunIdealCacheTests() {
    std::cout << "Running ideal cache tests\n";

    for (const Test& test : kIdealCacheTests) {
        assert(test.requests_.size() == test.expected_hits_.size());

        caches::IdealCache<int> cache{test.cache_size_, test.requests_};

        for (size_t i = 0; i != test.requests_.size(); ++i) {
            bool actual_hit = cache.LookupUpdate(test.requests_[i], [](int x) { return x; });
            bool expected_hit = test.expected_hits_[i];

            if (actual_hit != expected_hit) {
                throw std::runtime_error(colors::kRed + "Test " + test.test_name_ +
                                         ": Request " + std::to_string(i + 1) + 
                                         " (key=" + std::to_string(test.requests_[i]) + 
                                         "): expected " + (expected_hit ? "HIT" : "MISS") + 
                                         ", got " + (actual_hit ? "HIT" : "MISS") + colors::kReset);
            }
        }
    }

    std::cout << colors::kGreen << "Ideal cache tests passed successfully\n" << colors::kReset;
}
