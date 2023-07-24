cmake_minimum_required(VERSION 3.10)

project(GUIApp)

find_package(Qt5Widgets REQUIRED)

add_executable( ${PROJECT_NAME}
    Application/main.cpp
)

target_link_libraries( ${PROJECT_NAME}
    Qt5::Widgets
)