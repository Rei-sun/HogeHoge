project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
    HogeHogeSerial/HogeHogeSerial.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/HogeHogeSerial/
    ${LIB_DIR}/Timer/
)