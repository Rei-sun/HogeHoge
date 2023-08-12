cmake_minimum_required(VERSION 3.12)

project(GUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Lib)
set(APP_DIR ${PROJECT_SOURCE_DIR}/App)

add_subdirectory(App)
add_subdirectory(Lib)