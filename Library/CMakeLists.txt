project(HogeHoge)

add_library( ${PROJECT_NAME} SHARED
    SerialCommunication/SerialCommunication.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/SerialCommunication/
    ${LIB_DIR}/Timer/
)