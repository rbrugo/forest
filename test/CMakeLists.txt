cmake_minimum_required(VERSION 3.5)

project(avl_test LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

add_executable(avl_test avl_test.cpp)

include(CTest)
include(Catch)
