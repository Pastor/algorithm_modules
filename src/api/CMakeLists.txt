cmake_minimum_required(VERSION 3.8)
project(api)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_LIBRARY)
set(COMMON_SOURCE_FILES plugin_api.cpp python_plugin_module.cpp dynamic_library_module.cpp)

add_library(api STATIC ${COMMON_SOURCE_FILES})