project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
    HogeHogeSerial/HogeHogeSerial.cpp
    Module/MotorControlModule/MotorControlModule.cpp
    Module/EncoderModule/EncoderModule.cpp
    Module/SensorModule/SensorModule.cpp
    Module/SolenoidModule/SolenoidModule.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/Timer/
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/HogeHogeSerial/
    ${LIB_DIR}/Module/
    ${LIB_DIR}/Module/BaseModule/
    ${LIB_DIR}/Module/MotorControlModule/
    ${LIB_DIR}/Module/EncoderModule/
    ${LIB_DIR}/Module/SensorModule/
    ${LIB_DIR}/Module/SolenoidModule/
)