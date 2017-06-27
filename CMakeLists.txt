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
    find_package(PythonLibs REQUIRED)
    set(PYTHON_LIBRARY "")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -Wp,-w")
    set(CMAKE_CFLAGS "${CMAKE_CFLAGS} -pedantic -Wp,-w")
    set(PYTHON_INCLUDE_DIR D:/msys64/mingw64/include/python2.7)
    set(PYTHON_LIBRARY D:/msys64/mingw64/lib/python2.7)
    set(PYTHON_LIBRARY_RELEASE -lpython2.7.dll)
endif ()
message(STATUS "Python. Include  : ${PYTHON_INCLUDE_DIR}")
message(STATUS "Python. Libs     : ${PYTHON_LIBRARY}")
message(STATUS "Python. Libraries: ${PYTHON_LIBRARIES}")
include_directories(${PYTHON_INCLUDE_DIR})
link_directories(${PYTHON_LIBRARY})

include_directories(src)
include_directories(src/api)
include_directories(src/manager)
include_directories(src/db_context)
include_directories(lib/sqlite)
include_directories(lib/tinyxml2)
include_directories(lib/googletest/googletest/include)
add_subdirectory(lib/googletest/googletest)
add_subdirectory(lib/sqlite)
add_subdirectory(src/api)
add_subdirectory(src/manager)
add_subdirectory(src/db_context)

set(ALGORITHM_SOURCE_FILES src/app/main.cpp)
set(COMMON_SOURCE_FILES    lib/tinyxml2/tinyxml2.cpp)
set(TESTS_SOURCE_FILES
        tests/python_plugin_test.cpp
        tests/module_context_test.cpp
        tests/dynamic_module_test.cpp
        tests/plugin_spec_test.cpp
        tests/postgresql_context_test.cpp
        tests/sqlite_context_test.cpp
        tests/module_manager_test.cpp)

add_executable(${PROJECT_NAME} ${ALGORITHM_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARY_RELEASE} api manager)

add_executable(${PROJECT_NAME}_tests ${TESTS_SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main ${PYTHON_LIBRARY_RELEASE}
        api manager postgresql_context sqlite_context)

add_library(dynamic_module_library SHARED tests/dynamic_module_library.cpp)
target_link_libraries(dynamic_module_library api)

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