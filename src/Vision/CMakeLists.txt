project(Vision)

include_directories("../Config")

add_library(Vision camera.hpp camera.cpp)

target_link_libraries(Vision
        PUBLIC libopencv_core.so
        PUBLIC libopencv_video.so
        PUBLIC libopencv_videoio.so
        PUBLIC libopencv_stereo.so
        PUBLIC libopencv_calib3d.so
        PUBLIC libwiringPi.so
        PUBLIC libpthread.so)