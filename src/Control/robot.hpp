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

namespace Control {
    typedef int32_t ROBOT_ID;

    auto clean(const char* responseChar) -> std::vector<ROBOT_ID>;
    auto connect(const std::string& devicePath, int baud) -> ROBOT_ID;
    auto updateRobotState(int port, ROBOT_CONTROL_BULK state) -> void;
}

#endif //BEHOLDER_ROBOT_HPP
