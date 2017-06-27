cmake_minimum_required(VERSION 3.8)
project(plugin_common)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/lib/tinyxml2/tinyxml2.cpp)

add_library(plugin_common STATIC ${SOURCE_FILES})