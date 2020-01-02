project(Control)

include_directories("../Vision")
add_library(Control gamecontroller.h robotcontol.h ../config.hpp.in streamcontrol.hpp streamcontroldaemon.cpp streamcontroldaemon.hpp robot.cpp robot.hpp)