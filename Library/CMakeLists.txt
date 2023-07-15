project(HogeHogeSystem)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/SerialCommunication/
)