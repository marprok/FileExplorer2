project(FileExplorer2)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -g)
include_directories(
    inc
)
set(SOURCES
    src/terminal_window.cc
    src/scene.cc
    src/File.cc
    src/Directory.cc
    src/query_manager.cc
    src/fs_node.cc
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

#file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} "main.cc" ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
