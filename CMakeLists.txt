cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
project(Beholder)

include_directories("/usr/local/include")

set(CMAKE_MODULE_PATH
        "${PROJECT_SOURCE_DIR}/cmake"
        ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcompare-debug-second -O3")

add_subdirectory(cmake)
add_subdirectory(docs)
add_subdirectory(extern)
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(tests)