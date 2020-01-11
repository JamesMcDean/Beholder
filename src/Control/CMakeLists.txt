project(Control)

include_directories("../Vision")
include_directories("../Config")

add_library(Control gamecontroller.h robotcontol.h
        streamcontrol.hpp streamcontroldaemon.cpp
        streamcontroldaemon.hpp robot.cpp robot.hpp)

target_link_libraries(Control
        PUBLIC libopencv_core.so
        PUBLIC libwiringPi.so)