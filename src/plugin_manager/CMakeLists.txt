cmake_minimum_required(VERSION 3.8)
project(plugin_manager)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_MANAGER_LIBRARY)
set(SOURCE_FILES plugin_manager.cpp)

add_library(plugin_manager SHARED ${SOURCE_FILES})
target_link_libraries(plugin_manager plugin_api plugin_python plugin_common)