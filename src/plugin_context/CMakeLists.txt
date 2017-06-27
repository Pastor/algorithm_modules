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

set(POSTGRESQL_SOURCE_FILES postgresql_context.cpp sql_context.cpp)
set(SQLITE_SOURCE_FILES sqlite_context.cpp sql_context.cpp)

add_library(postgresql_context SHARED ${POSTGRESQL_SOURCE_FILES})
add_library(sqlite_context SHARED ${SQLITE_SOURCE_FILES})
target_link_libraries(postgresql_context plugin_api ${PostgreSQL_LIBRARIES})
target_link_libraries(sqlite_context plugin_api sqlite)

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
