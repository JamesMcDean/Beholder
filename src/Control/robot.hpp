//
// Created by James on 11/7/2019.
//

#ifndef BEHOLDER_ROBOT_HPP
#define BEHOLDER_ROBOT_HPP

#include <string>
#include <wiringSerial.h>

#include "config.hpp"

#ifdef DEBUG
#include <OIS/OISJoyStick.h>
#endif

#include "robotContol.h"

namespace Control {
    int findRobot();
    void updateRobotState(int port, ROBOT_CONTROL_BULK state);
}

#endif //BEHOLDER_ROBOT_HPP
