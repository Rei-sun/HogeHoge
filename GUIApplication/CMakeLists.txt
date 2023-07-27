cmake_minimum_required(VERSION 3.10)

project(GUIApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Library)

add_subdirectory(Application)
add_subdirectory(Library)