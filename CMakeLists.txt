cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
project(Beholder)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)

add_executable(Beholder main.cpp)
target_link_libraries(Beholder Control Vision Movement)