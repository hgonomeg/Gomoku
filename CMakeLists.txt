cmake_minimum_required(VERSION 3.1)
enable_language(CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(TGUI REQUIRED)
#find_package(rang REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

project(Gomoku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${SFML_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(gomoku

    src/board.cpp
    src/game_controller.cpp
    src/game_window.cpp
    src/gui_mode.cpp
    src/hotseat.cpp
    src/main.cpp
    src/resource_manager.cpp
    src/server_connection.cpp
    src/server_mode.cpp
    src/server.cpp
)

target_link_libraries(gomoku sfml-graphics sfml-network tgui Threads::Threads)

