find_package(Qt5Widgets REQUIRED)
find_package(Threads REQUIRED)

add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/Socket/
    ${LIB_DIR}/Module/
    ${LIB_DIR}/DeviceValue/
    ${APP_DIR}/
)

target_link_libraries( ${PROJECT_NAME}
    Module
    Qt5::Widgets
    ${CMAKE_THREAD_LIBS_INIT}
)