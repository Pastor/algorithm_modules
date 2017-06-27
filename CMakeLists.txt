cmake_minimum_required(VERSION 3.8)
project(algorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (MSVC)
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /SUBSYSTEM:CONSOLE")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CMAKE_CFLAGS "${CMAKE_CFLAGS} /MDd")
        add_compile_options(/MDd)
    else ()
        add_compile_options(/MT)
    endif ()
    add_compile_options(/wd4267)
    add_compile_options(/wd4251)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wp,-w")
    set(CMAKE_CFLAGS "${CMAKE_CFLAGS} -pedantic -Wp,-w")
endif ()

include_directories(src)
include_directories(src/plugin_api)
include_directories(src/plugin_manager)
include_directories(src/plugin_context)
include_directories(src/plugin_python)
include_directories(src/plugin_common)
include_directories(lib/sqlite)
include_directories(lib/tinyxml2)
include_directories(lib/googletest/googletest/include)
add_subdirectory(lib/googletest/googletest)
add_subdirectory(lib/sqlite)
add_subdirectory(src/plugin_api)
add_subdirectory(src/plugin_manager)
add_subdirectory(src/plugin_context)
add_subdirectory(src/plugin_python)
add_subdirectory(src/plugin_common)

set(ALGORITHM_SOURCE_FILES src/plugin_app/main.cpp)
set(TESTS_SOURCE_FILES
        tests/python_plugin_test.cpp
        tests/module_context_test.cpp
        tests/dynamic_module_test.cpp
        tests/plugin_spec_test.cpp
        tests/postgresql_context_test.cpp
        tests/sqlite_context_test.cpp
        tests/module_manager_test.cpp)

add_executable(${PROJECT_NAME} ${ALGORITHM_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} plugin_api plugin_manager plugin_common)

add_executable(${PROJECT_NAME}_tests ${TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main
        plugin_api plugin_python plugin_manager postgresql_context sqlite_context plugin_common)

add_library(dynamic_module_library SHARED tests/dynamic_module_library.cpp)
target_link_libraries(dynamic_module_library plugin_api)

file(COPY
        ${CMAKE_SOURCE_DIR}/tests_data/plugin.01.py
        DESTINATION
        ${CMAKE_BINARY_DIR}/tests_data)
file(COPY
        ${CMAKE_SOURCE_DIR}/tests_data/configuration.xml
        DESTINATION
        ${CMAKE_BINARY_DIR}/tests_data)
#Start environment
#PYTHONHOME=D:/msys64/mingw64/include
#PYTHONPATH=D:/msys64/mingw64/lib/python2.7