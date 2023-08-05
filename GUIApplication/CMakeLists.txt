cmake_minimum_required(VERSION 3.10)

project(GUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Library)
set(APP_DIR ${PROJECT_SOURCE_DIR}/Application)

add_subdirectory(Application)
add_subdirectory(Library)