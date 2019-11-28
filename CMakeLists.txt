cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(Beholder)

project(Beholder)

set(CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake"
        ${CMAKE_MODULE_PATH})

add_subdirectory(cmake)
add_subdirectory(docs)
add_subdirectory(extern)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(tests)