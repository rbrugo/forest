cmake_minimum_required(VERSION 3.5)
project(avl_tree VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fconcepts")

add_library(avl_tree INTERFACE)

include_directories(avl_tree INTERFACE include/)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    enable_testing()
    add_subdirectory(test)
endif()
