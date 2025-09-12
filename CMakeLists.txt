cmake_minimum_required(VERSION 3.11)
project(cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# tq_cache -------------------------------------------------

add_executable(tq_cache src/main_2q_cache.cc)

target_include_directories(tq_cache
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ideal_cache ----------------------------------------------

add_executable(ideal_cache src/main_ideal_cache.cc)

target_include_directories(ideal_cache
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# tests ----------------------------------------------------

add_executable(tests tests/tests.cc)

target_include_directories(tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

include(CTest)
enable_testing()
add_test(NAME all_tests COMMAND tests)
