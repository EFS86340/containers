cmake_minimum_required(VERSION 3.7)
project(list)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES library.cpp library.h map.h)
add_library(list ${SOURCE_FILES})