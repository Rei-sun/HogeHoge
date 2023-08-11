cmake_minimum_required(VERSION 3.10)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(${PROJECT_SOURCE_DIR})

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Library)

add_subdirectory(Application)
add_subdirectory(Library)