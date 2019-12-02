project(Beholder)

option(BUILD_LOCAL_CONTROLLER
        "Builds in the support for a local controller."
        ON)
option(BUILD_REMOTE_CONTROLLER
        "Builds in the support for a network based controller."
        OFF)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)

# Honestly, I hated the main sitting in apps, it didn't make
# sense if I am not even making a library, so, fuck it, it's src
add_executable(Beholder main.cpp config.hpp)
target_link_libraries(Beholder
        PUBLIC Control
        PUBLIC Vision
        PUBLIC Movement)