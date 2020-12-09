cmake_minimum_required(VERSION 3.1)
enable_language(CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

project(Gomoku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${SFML_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(gomoku)

target_link_libraries(gomoku sfml-graphics sfml-network Threads::Threads)

