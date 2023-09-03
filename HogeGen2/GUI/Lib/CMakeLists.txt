project(Module)

find_package(Qt5Widgets REQUIRED)

add_library( ${PROJECT_NAME} STATIC
    DeviceValue/DeviceValue.cpp
    Modules/EncoderModuleGUI.cpp
    Modules/MotorModuleGUI.cpp
    Modules/SensorModuleGUI.cpp
    Modules/SolenoidModuleGUI.cpp
    ModuleManagerGUI/ModuleManagerGUI.cpp
    MainWindow/MainWindow.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${COM_DIR}/
    ${COM_DIR}/nlohmann/
    ${COM_DIR}/Module/
    ${COM_DIR}/ModuleManager/
    ${COM_DIR}/Socket/
    ${LIB_DIR}/
    ${LIB_DIR}/DeviceValue/
    ${LIB_DIR}/Interface/
    ${LIB_DIR}/IPCommunicationSub/
    ${LIB_DIR}/MainWindow/
    ${LIB_DIR}/ModuleManagerGUI/
    ${LIB_DIR}/Modules/
    ${LIB_DIR}/SubWindow/
)

target_link_libraries( ${PROJECT_NAME}
    Qt5::Widgets
)