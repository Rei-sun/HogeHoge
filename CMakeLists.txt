cmake_minimum_required(VERSION 3.10)

project(Main)

message(${PROJECT_SOURCE_DIR})

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Library)

add_subdirectory(Application)
add_subdirectory(Library)