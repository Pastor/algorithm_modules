cmake_minimum_required(VERSION 3.8)
project(algorithm)

set(CMAKE_CXX_STANDARD 17)

include_directories("D:/msys64/mingw64/include/python2.7")
link_directories("D:/msys64/mingw64/bin")

include_directories(src)
include_directories(lib/googletest/googletest/include)
add_subdirectory(lib/googletest/googletest)


set(COMMON_SOURCE_FILES src/plugin_module.cpp src/python_plugin_module.cpp)
set(ALGORITHM_SOURCE_FILES src/main.cpp)
set(TESTS_SOURCE_FILES tests/python_plugin_test.cpp tests/module_context_test.cpp)

add_executable(${PROJECT_NAME} ${ALGORITHM_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -lpython2.7.dll)
add_executable(${PROJECT_NAME}_tests ${TESTS_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main -lpython2.7.dll)


file(COPY
        ${CMAKE_SOURCE_DIR}/scripts/plugin.01.py
        DESTINATION
        ${CMAKE_BINARY_DIR}/scripts)

#Start environment
#PYTHONHOME=D:/msys64/mingw64/include
#PYTHONPATH=D:/msys64/mingw64/lib/python2.7