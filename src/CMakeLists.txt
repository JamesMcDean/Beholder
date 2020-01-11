project(Beholder)

include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)
include_directories("Config")
add_subdirectory(Config)

add_executable(Beholder main.cpp main.hpp)
target_link_libraries(Beholder
        PUBLIC Control
        PUBLIC Vision
        PUBLIC Movement
        PUBLIC Config)