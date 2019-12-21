project(Beholder)

option(SERIAL_RESPONSE_TIME
        "The millisecond delay to wait before checking for a serial response."
        100)
option(SERIAL_STATE_CHAR
        "The char defining a state being sent."
        S)
option(SERIAL_TEST_CHAR
        "The char defining a test message being sent."
        T)

option(REMOTE_CONTROL_PORT
        "The port that handles the control data sent from the control. (TCP)"
        5455)
option(REMOTE_STREAM_PORT
        "The port that handles the stream data being sent back to the control. (TCP/UDP)"
        5456)

option(ROBOT_SPEED_PER_SECOND
        "The amount that should be moved per second in the range of [-1.0, 1.0] when receiving movement input."
        1)

option(TRIGGER_FIRING_THRESHOLD
        "The point at which the trigger depression means fire ([0.0, 1.0])."
        0.5)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("Control")
add_subdirectory(Control)
include_directories("Movement")
add_subdirectory(Movement)
include_directories("Vision")
add_subdirectory(Vision)
include_directories("Exceptions")
add_subdirectory(Exceptions)

# Honestly, I hated the main sitting in apps, it didn't make
# sense if I am not even making a library, so, fuck it, it's src
add_executable(Beholder main.cpp config.hpp)
target_link_libraries(Beholder
        PUBLIC Control
        PUBLIC Vision
        PUBLIC Movement
        PUBLIC Exceptions)