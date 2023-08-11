find_package(Threads REQUIRED)

add_executable( ${PROJECT_NAME}
    main.cpp
    Application/Application.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${LIB_DIR}/
    ${APP_DIR}/Application/
)

target_link_libraries( ${PROJECT_NAME}
    HogeHoge
    ${CMAKE_THREAD_LIBS_INIT}
)