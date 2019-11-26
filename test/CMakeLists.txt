cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)

include_directories(third_party)

add_executable(avl_test avl_test.cpp)
add_executable(bst_test bst_test.cpp)

include(CTest)

add_test(binary_search_tree bst_test)
add_test(avl_tree avl_test)
