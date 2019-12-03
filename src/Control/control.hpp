//
// Created by James on 11/7/2019.
//

#ifndef BEHOLDER_CONTROL_HPP
#define BEHOLDER_CONTROL_HPP

#include <wiringSerial.h>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <thread>

#include "config.hpp"
#include "robotcontol.h"

/**
 * Methods for getting data from control sources and to robot sources.
 */
namespace Control {
    /**
     * The poorly hashed ID of a robot.
     */
    typedef int32_t ROBOT_ID;

    /**
     * The poorly hashed ID of a control loop.
     */
    typedef int32_t LOOP_ID;

    /**
     * Removes all dead connections and returns them as a vector.
     * @param responseChar The char to respond to (optional).
     * @return The ID's of the connections removed.
     */
    auto clean(const char* responseChar) -> std::vector<ROBOT_ID>;

    /**
     * Connects to a device and returns the ID.
     * @param devicePath The system device path of the serial connection.
     * @param baud The baud rate.
     * @return The ROBOT_ID used in other functions in this namespace.
     */
    auto connect(const std::string& devicePath, int baud) -> ROBOT_ID;

    /**
     * Sends a state to the robot specified.
     * @param id The ID of the robot to send the state to.
     * @param state The state of the body and guns of the robot to update to.
     */
    auto updateRobotState(ROBOT_ID id, ROBOT_CONTROL_BULK state) -> void;

    /**
     * Checks if a robot is registered.
     * @param cleanFirst Whether to run the clean() method prior to checking the registration map.
     * @return If the robot is in the registration map.
     */
    auto isRobotRegistered(ROBOT_ID id, bool cleanFirst = false) -> bool;
}

#endif //BEHOLDER_CONTROL_HPP
