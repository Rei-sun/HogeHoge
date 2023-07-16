project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
    HogeHogeSerial/HogeHogeSerial.cpp
    MotorControlModule/MotorControlModule.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/Timer/
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/HogeHogeSerial/
    ${LIB_DIR}/BaseModule/
    ${LIB_DIR}/MotorControlModule/
)