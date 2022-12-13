cmake_minimum_required(VERSION 3.12)

project(
    lab1
    VERSION 1.0
    DESCRIPTION "C++ Year 2022 Lab1"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)


if (NOT TARGET number)
    add_subdirectory(lib)
endif()
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
