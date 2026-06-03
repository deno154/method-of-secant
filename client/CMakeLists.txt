cmake_minimum_required(VERSION 3.16)

project(TMPS_Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Network
)

qt_standard_project_setup()

qt_add_executable(client

    main.cpp

    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(client PRIVATE

    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)