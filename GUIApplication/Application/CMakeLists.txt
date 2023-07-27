find_package(Qt5Widgets REQUIRED)

add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${LIB_DIR}/Socket/
    ${LIB_DIR}/Module/
)

target_link_libraries( ${PROJECT_NAME}
    Qt5::Widgets
)