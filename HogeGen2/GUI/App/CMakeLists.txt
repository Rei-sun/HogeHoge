find_package(Qt5Widgets REQUIRED)
find_package(Threads REQUIRED)

add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/DeviceValue/
    ${LIB_DIR}/Interface/
    ${LIB_DIR}/Modules/
    ${LIB_DIR}/SettingData/
    ${APP_DIR}/
)

target_link_libraries( ${PROJECT_NAME}
    Module
    Qt5::Widgets
    ${CMAKE_THREAD_LIBS_INIT}
)