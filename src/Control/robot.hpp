//
// Created by James on 11/7/2019.
//

#ifndef BEHOLDER_ROBOT_HPP
#define BEHOLDER_ROBOT_HPP

#include <string>
#include <wiringSerial.h>

#include "config.hpp"

#include <OIS/OISJoyStick.h>
#include <wiringSerial.h>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <thread>

#include "robotContol.h"

/**
 * Methods for getting data from control sources and to robot sources.
 */
namespace Control {
    /**
     * The poorly hashed ID of a robot.
     */
    typedef int32_t ROBOT_ID;

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
}

#endif //BEHOLDER_ROBOT_HPP
