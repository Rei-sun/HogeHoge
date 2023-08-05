project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
    HogeHogeSerial/HogeHogeSerial.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${COM_DIR}/
    ${COM_DIR}/Module/
    ${COM_DIR}/ModuleManager/
    ${LIB_DIR}/
    ${LIB_DIR}/GUICommunication/
    ${LIB_DIR}/Socket/
    ${LIB_DIR}/Timer/
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/HogeHogeSerial/
)