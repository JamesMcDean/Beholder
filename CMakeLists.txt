cmake_minimum_required(VERSION 3.1)

project(Beholder)
set(Beholder_VERSION_MAJOR 0)
set(Beholder_VERSION_MINOR 1)
configure_file(
        "${PROJECT_SOURCE_DIR}/stdinc.hpp"
        "${PROJECT_BINARY_DIR}/stdinc.hpp"
)
include_directories("${PROJECT_BINARY_DIR}")
include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)

add_executable(Beholder main.cpp)
target_link_libraries(Beholder Control Vision Movement)