cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

add_executable(avl_test avl_test.cpp)
add_executable(bst_test bst_test.cpp)

include(CTest)
include(Catch)

add_test(binary_search_tree bst_test)
add_test(avl_tree avl_test)
