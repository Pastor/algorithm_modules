cmake_minimum_required(VERSION 3.8)
project(db_context)

find_package(PostgreSQL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_definitions(-DEXPORT_LIBRARY)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

set(POSTGRESQL_SOURCE_FILES postgresql_context.cpp sql_context.cpp)
set(SQLITE_SOURCE_FILES sqlite_context.cpp sql_context.cpp)

add_library(postgresql_context SHARED ${POSTGRESQL_SOURCE_FILES})
add_library(sqlite_context SHARED ${SQLITE_SOURCE_FILES})
target_link_libraries(postgresql_context api ${PostgreSQL_LIBRARIES})
target_link_libraries(sqlite_context api sqlite)