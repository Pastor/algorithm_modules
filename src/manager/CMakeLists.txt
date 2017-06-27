cmake_minimum_required(VERSION 3.8)
project(manager)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SOURCE_FILES plugin_manager.cpp plugin_spec.cpp)

add_library(manager STATIC ${SOURCE_FILES})