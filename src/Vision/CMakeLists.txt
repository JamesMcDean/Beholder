project(Vision)

include_directories("../Config")

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
else()
    message(FATAL_ERROR "Boost libraries are required to build.")
endif()

add_library(Vision camera.hpp camera.cpp visiontools.hpp visiontools.cpp)

if (Boost_FOUND)
    target_link_libraries(Control
            PUBLIC ${Boost_LIBRARIES})
endif()

target_link_libraries(Vision
        PUBLIC libopencv_core.so
        PUBLIC libopencv_video.so
        PUBLIC libopencv_videoio.so
        PUBLIC libopencv_stereo.so
        PUBLIC libopencv_calib3d.so
        PUBLIC libpthread.so
        PUBLIC libboost_system.so)