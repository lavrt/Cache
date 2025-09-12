cmake_minimum_required(VERSION 3.11)
project(cache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(SOURCES
    src/main.cc
    tests/tests.cc
)

add_executable(cache ${SOURCES})

target_include_directories(cache
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
