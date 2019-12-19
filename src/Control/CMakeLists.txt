project(Control)

include_directories("../Vision" "../Exceptions")
add_library(Control gamecontroller.h robotcontol.h control.hpp control.cpp ../config.hpp.in streamcontrol.hpp streamcontroldaemon.cpp streamcontroldaemon.hpp math.cpp robot.cpp robot.hpp)