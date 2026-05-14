cmake_minimum_required(VERSION 3.10)

project(TMPS_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Все cpp файлы проекта

set(SOURCES

    src/main.cpp

    src/server/server.cpp

    src/handlers/handlers.cpp

    src/algorithms/vigenere/vigenere.cpp
    src/algorithms/md5/md5.cpp
    src/algorithms/secant/secant.cpp
    src/algorithms/graph_cycle/graph_cycle.cpp

    src/utils/json.cpp

    src/database/database.cpp
)

add_executable(server ${SOURCES})

# Подключение заголовков

target_include_directories(server PRIVATE

    src/server
    src/handlers

    src/algorithms/vigenere
    src/algorithms/md5
    src/algorithms/secant
    src/algorithms/graph_cycle

    src/utils

    src/database
)

# Windows sockets

if(WIN32)
    target_link_libraries(server ws2_32)
endif()# Docs branch adjustment
