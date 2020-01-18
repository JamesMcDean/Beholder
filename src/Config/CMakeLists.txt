project(Config)

set(SERIAL_RESPONSE_TIME "100" CACHE STRING
        "The millisecond delay to wait before checking for a serial response.")
set(SERIAL_STATE_CHAR "S" CACHE STRING
        "The char defining a state being sent.")
set(SERIAL_TEST_CHAR "T" CACHE STRING
        "The char defining a test message being sent.")

set(REMOTE_CONTROL_PORT "5455" CACHE STRING
        "The port that handles the control data sent from the control. (TCP)")
set(REMOTE_STREAM_PORT "5455" CACHE STRING
        "The port that handles the stream data being sent back to the control. (TCP/UDP)")

set(ROBOT_SPEED_PER_SECOND "1" CACHE STRING
        "The amount that should be moved per second in the range of [-1.0, 1.0] when receiving movement input.")
set(TRIGGER_FIRING_THRESHOLD "0.5" CACHE STRING
        "The point at which the trigger depression means fire ([0.0, 1.0]).")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cconfig.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/cconfig.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(Config config.hpp ${CMAKE_CURRENT_BINARY_DIR}/cconfig.hpp varconfig.hpp varconfig.cpp)
