cmake_minimum_required(VERSION 3.11)
project(cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CACHE_SOURCES
    src/main.cc
)

add_executable(cache ${CACHE_SOURCES})

target_include_directories(cache
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(TESTS_SOURCES
    tests/main_test.cc
    tests/tests.cc
)

add_executable(tests ${TESTS_SOURCES})

target_include_directories(tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

include(CTest)
enable_testing()
add_test(NAME all_tests COMMAND tests)
