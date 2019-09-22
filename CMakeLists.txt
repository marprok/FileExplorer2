project(FileExplorer)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    inc
)
set(SOURCES
    src/TWindow.cc
    src/Scene.cc
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

#file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
