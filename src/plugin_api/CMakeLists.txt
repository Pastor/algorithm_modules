cmake_minimum_required(VERSION 3.8)
project(plugin_api)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_API_LIBRARY)
set(COMMON_SOURCE_FILES plugin_api.cpp dynamic_library_module.cpp plugin_spec.cpp system_process_module.cpp)

add_library(plugin_api SHARED ${COMMON_SOURCE_FILES})
target_link_libraries(plugin_api plugin_common)