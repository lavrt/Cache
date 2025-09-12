#include "tests.hpp"

#include <iostream>
#include <cassert>
#include <iomanip>

#include "caches.hpp"

namespace colors {
    const std::string kRed = "\033[31m";
    const std::string kGreen = "\033[32m";
    const std::string kReset = "\033[0m";
    const std::string kBrightBlue = "\033[94m";
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
    {"ideal_test_1", 3, {1, 2, 3, 1, 4, 2, 5, 3, 4, 5},     {0, 0, 0, 1, 0, 1, 0, 1, 1, 1}},
    {"ideal_test_2", 2, {1, 2, 1, 3, 2, 4, 1, 5},           {0, 0, 1, 0, 1, 0, 0, 0}},
    {"ideal_test_3", 4, {1, 2, 3, 4, 1, 2, 5, 1, 2, 3},     {0, 0, 0, 0, 1, 1, 0, 1, 1, 1}},
    {"ideal_test_4", 1, {1, 1, 1, 1, 1},                    {0, 1, 1, 1, 1}},
    {"ideal_test_5", 3, {1, 2, 3, 4, 5, 1, 2, 3},           {0, 0, 0, 0, 0, 1, 1, 0}},
};

static const std::string kLine = "=======================================\n";

static int SlowGetPageForTests(int x);
static void RunTwoQCacheTests();
static void RunIdealCacheTests();
static void RunComparativeTests(size_t cache_capacity);

// global ------------------------------------------------------------------------------------------

void RunTests() {
    std::cout << "Running cache tests\n";
    std::cout << kLine;

    RunTwoQCacheTests();
    RunIdealCacheTests();
    RunComparativeTests(3);
    
    std::cout << "Testing complete\n";
}

// static ------------------------------------------------------------------------------------------

static int SlowGetPageForTests(int x) {
    return x;
}

static void RunTwoQCacheTests() {
    std::cout << "Running 2Q cache tests\n";
    
    for (const Test& test : kTwoQCacheTests) {
        std::cout << test.test_name_ << ": ";

        assert(test.requests_.size() == test.expected_hits_.size());

        caches::TwoQCache<int> cache{test.cache_size_};

        for (size_t i = 0; i != test.requests_.size(); ++i) {
            bool actual_hit = cache.LookupUpdate(test.requests_[i], SlowGetPageForTests);
            bool expected_hit = test.expected_hits_[i];

            if (actual_hit != expected_hit) {
                std::cout << colors::kRed << "failed" << colors::kReset << std::endl;

                throw std::runtime_error(
                    colors::kRed + "Request " + std::to_string(i + 1) + 
                    " (key=" + std::to_string(test.requests_[i]) + 
                    "): expected " + (expected_hit ? "HIT" : "MISS") + 
                    ", got " + (actual_hit ? "HIT" : "MISS") + colors::kReset
                );
            }
        }

        std::cout << colors::kGreen << "passed" << colors::kReset << std::endl;
    }

    std::cout << colors::kGreen << "2Q cache tests passed successfully\n" << colors::kReset;
    std::cout << kLine;
}

static void RunIdealCacheTests() {
    std::cout << "Running ideal cache tests\n";

    for (const Test& test : kIdealCacheTests) {
        std::cout << test.test_name_ << ": ";

        assert(test.requests_.size() == test.expected_hits_.size());

        caches::IdealCache<int> cache{test.cache_size_, test.requests_};

        for (size_t i = 0; i != test.requests_.size(); ++i) {
            bool actual_hit = cache.LookupUpdate(test.requests_[i], SlowGetPageForTests);
            bool expected_hit = test.expected_hits_[i];

            if (actual_hit != expected_hit) {
                std::cout << colors::kRed << "failed" << colors::kReset << std::endl;

                throw std::runtime_error(
                    colors::kRed + "Request " + std::to_string(i + 1) + 
                    " (key=" + std::to_string(test.requests_[i]) + 
                    "): expected " + (expected_hit ? "HIT" : "MISS") + 
                    ", got " + (actual_hit ? "HIT" : "MISS") + colors::kReset
                );
            }
        }

        std::cout << colors::kGreen << "passed" << colors::kReset << std::endl;
    }

    std::cout << colors::kGreen << "Ideal cache tests passed successfully\n" << colors::kReset;
    std::cout << kLine;
}

static void RunComparativeTests(size_t cache_capacity) {
    std::cout << "Running comparative tests (cache_capacity = " << cache_capacity << ")\n";

    size_t counter = 0;
    auto ProcessTests = [&counter, cache_capacity](const std::vector<Test>& tests) {
        for (const Test& test : tests) {
            std::cout << "comparative_test_" << ++counter
                << " (based on " << test.test_name_ << "): ";

            caches::TwoQCache<int> tq_cache{cache_capacity};
            size_t hits_tq = 0;
            for (int request : test.requests_) {
                hits_tq += tq_cache.LookupUpdate(request, SlowGetPageForTests);
            }

            caches::IdealCache<int> id_cache{cache_capacity, test.requests_};
            size_t hits_id = 0;
            for (int request : test.requests_) {
                hits_id += id_cache.LookupUpdate(request, SlowGetPageForTests);
            }

            std::cout << std::fixed << std::setprecision(1)
                << colors::kBrightBlue << "TwoQCache: "
                << colors::kGreen << (100.0 * hits_tq) / test.requests_.size() << "% "
                << colors::kBrightBlue << "IdealCache: "
                << colors::kGreen << (100.0 * hits_id) / test.requests_.size() << "%\n"
                << colors::kReset;
        }
    };
    
    ProcessTests(kIdealCacheTests);
    ProcessTests(kTwoQCacheTests);

    std::cout << kLine;
}
