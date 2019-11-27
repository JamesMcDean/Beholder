cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
project(Beholder)

project(Beholder)

set(CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake"
        ${CMAKE_MODULE_PATH})

add_subdirectory(src)