project(FileExplorer2)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

include_directories(
    inc
)
set(SOURCES
    src/terminal_window.cc
    src/scene.cc
    src/query_manager.cc
    src/inode.cc
	src/log.cc
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} "main.cc" ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
