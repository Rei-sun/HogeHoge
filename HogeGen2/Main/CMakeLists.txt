cmake_minimum_required(VERSION 3.10)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(${PROJECT_SOURCE_DIR})

#set(HOGE_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(COM_DIR ${PROJECT_SOURCE_DIR}/../Common)
set(APP_DIR ${PROJECT_SOURCE_DIR}/App)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/Lib)

#add_compile_options(-D HOGE_ROOT ${HOGE_ROOT_DIR})

add_subdirectory(App)
add_subdirectory(Lib)