cmake_minimum_required(VERSION 3.8)
project(algorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(PythonInterp)

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
    add_compile_options(/wd4996)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wp,-w")
    set(CMAKE_CFLAGS "${CMAKE_CFLAGS} -pedantic -Wp,-w")
endif ()
add_definitions(-DPythonInterpretator=${PYTHON_EXECUTABLE})
include_directories(src)
include_directories(src/plugin_api)
include_directories(src/plugin_manager)
include_directories(src/plugin_modules)
include_directories(src/plugin_python)
include_directories(src/plugin_common)
include_directories(src/plugin_model)
include_directories(lib/sqlite)
include_directories(lib/tinyxml2)
include_directories(lib/googletest/googletest/include)
add_subdirectory(lib/googletest/googletest)
add_subdirectory(lib/sqlite)
add_subdirectory(src/plugin_api)
add_subdirectory(src/plugin_manager)
add_subdirectory(src/plugin_modules)
add_subdirectory(src/plugin_python)
add_subdirectory(src/plugin_common)
add_subdirectory(src/plugin_model)
add_subdirectory(src/plugin_app)

set(ALGORITHM_SOURCE_FILES src/plugin_app/main.cpp)
set(TESTS_SOURCE_FILES
        tests/python_plugin_test.cpp
        tests/module_context_test.cpp
        tests/dynamic_module_test.cpp
        tests/plugin_spec_test.cpp
        tests/postgresql_module_test.cpp
        tests/sqlite_module_test.cpp
        tests/system_process_module_test.cpp
        tests/loading_modules_test.cpp
        tests/test_main.cpp
        tests/module_manager_test.cpp)

add_executable(${PROJECT_NAME}_tests ${TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_tests gtest plugin_api plugin_python plugin_manager plugin_common plugin_model)

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
