cmake_minimum_required(VERSION 3.8)
project(plugin_context)

find_package(PostgreSQL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_CONTEXT_LIBRARY)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

#message(STATUS "PostgreSQL: ${PostgreSQL_LIBRARY}")

set(POSTGRESQL_SOURCE_FILES postgresql_module.cpp sql_module.cpp)
set(SQLITE_SOURCE_FILES sqlite_module.cpp sql_module.cpp)

add_library(postgresql_module SHARED ${POSTGRESQL_SOURCE_FILES})
add_library(sqlite_module SHARED ${SQLITE_SOURCE_FILES})
target_link_libraries(postgresql_module plugin_api ${PostgreSQL_LIBRARIES})
target_link_libraries(sqlite_module plugin_api sqlite)

file(COPY
        ${PostgreSQL_LIBRARY}
        DESTINATION
        ${CMAKE_BINARY_DIR})
file(COPY
        ${PostgreSQL_LIBRARY}/../../bin/libintl-8.dll
        DESTINATION
        ${CMAKE_BINARY_DIR})
file(COPY
        ${PostgreSQL_LIBRARY}/../../bin/iconv.dll
        DESTINATION
        ${CMAKE_BINARY_DIR})
