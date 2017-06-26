cmake_minimum_required(VERSION 3.8)
project(api)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories("D:/msys64/mingw64/include/python2.7")
link_directories("D:/msys64/mingw64/bin")
include_directories(src/api)

set(COMMON_SOURCE_FILES plugin_api.cpp python_plugin_module.cpp dynamic_library_module.cpp)

add_library(${PROJECT_NAME} STATIC ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -lpython2.7.dll)