add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/SerialCommunication/
)

target_link_libraries( ${PROJECT_NAME}
    HogeHogeSystem
)