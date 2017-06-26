cmake_minimum_required(VERSION 3.8)
project(algorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories("D:/msys64/mingw64/include/python2.7")
link_directories("D:/msys64/mingw64/bin")

include_directories(src)
include_directories(src/api)
include_directories(src/manager)
include_directories(lib/sqlite)
include_directories(lib/googletest/googletest/include)
add_subdirectory(lib/googletest/googletest)
add_subdirectory(lib/sqlite)
add_subdirectory(src/api)
add_subdirectory(src/manager)

set(ALGORITHM_SOURCE_FILES src/app/main.cpp)
set(TESTS_SOURCE_FILES tests/python_plugin_test.cpp tests/module_context_test.cpp tests/dynamic_module_test.cpp tests/module_manager_test.cpp)

add_executable(${PROJECT_NAME} ${ALGORITHM_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -lpython2.7.dll api manager)

add_executable(${PROJECT_NAME}_tests ${TESTS_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main -lpython2.7.dll api manager)

add_library(dynamic_module_library SHARED tests/dynamic_module_library.cpp)
target_link_libraries(dynamic_module_library api)

file(COPY
        ${CMAKE_SOURCE_DIR}/scripts/plugin.01.py
        DESTINATION
        ${CMAKE_BINARY_DIR}/scripts)

#Start environment
#PYTHONHOME=D:/msys64/mingw64/include
#PYTHONPATH=D:/msys64/mingw64/lib/python2.7