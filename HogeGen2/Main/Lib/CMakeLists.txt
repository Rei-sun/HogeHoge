project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
    HogeHogeSerial/HogeHogeSerial.cpp
    ModuleManagerMain/ModuleManagerMain.cpp
    Modules/EncoderModuleMain.cpp
    Modules/MotorModuleMain.cpp
    Modules/SensorModuleMain.cpp
    Modules/SolenoidModuleMain.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${COM_DIR}/
    ${COM_DIR}/nlohmann/
    ${COM_DIR}/Module/
    ${COM_DIR}/ModuleManager/
    ${COM_DIR}/Socket/
    ${LIB_DIR}/
    ${LIB_DIR}/IPCommunication/
    ${LIB_DIR}/Timer/
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/HogeHogeSerial/
    ${LIB_DIR}/Modules/
    ${LIB_DIR}/ModuleManagerMain/
    ${LIB_DIR}/Interface/
    ${LIB_DIR}/ConfigFileLoader/
)