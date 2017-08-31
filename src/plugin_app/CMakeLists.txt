cmake_minimum_required(VERSION 3.8)
project(plugin_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(ALGORITHM_SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${ALGORITHM_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} plugin_api plugin_manager plugin_common plugin_model)
