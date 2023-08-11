project(Module)

find_package(Qt5Widgets REQUIRED)

add_library( ${PROJECT_NAME} STATIC
    Module/Module.cpp
    Module/ModuleManager.cpp
    DeviceValue/DeviceValue.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/Module/
    ${LIB_DIR}/DeviceValue/
)

target_link_libraries( ${PROJECT_NAME}
    Qt5::Widgets
)