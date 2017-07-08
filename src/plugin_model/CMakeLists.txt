cmake_minimum_required(VERSION 3.8)
project(plugin_model)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_MODEL_LIBRARY)
set(SOURCE_FILES plugin_model.cpp)

add_library(plugin_model SHARED ${SOURCE_FILES})
target_link_libraries(plugin_model plugin_api plugin_common plugin_controller)