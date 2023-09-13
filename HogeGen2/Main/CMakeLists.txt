cmake_minimum_required(VERSION 3.12)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS -rdynamic)

message(${PROJECT_SOURCE_DIR})

set(COM_DIR ${PROJECT_SOURCE_DIR}/../Common)
set(APP_DIR ${PROJECT_SOURCE_DIR}/App)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/Lib)

add_compile_definitions(HOGE_ROOT="${PROJECT_SOURCE_DIR}/../")
add_compile_definitions(HOGE_PATH="${PROJECT_SOURCE_DIR}/")

add_subdirectory(App)
add_subdirectory(Lib)