project(Beholder)

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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)

add_executable(Beholder main.cpp config.hpp main.hpp)
target_link_libraries(Beholder
        PUBLIC Control
        PUBLIC Vision
        PUBLIC Movement)