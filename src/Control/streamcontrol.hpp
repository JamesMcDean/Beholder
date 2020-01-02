//
// Created by James on 12/2/2019.
//

#ifndef BEHOLDER_REMOTE_CONTROL_HPP
#define BEHOLDER_REMOTE_CONTROL_HPP

#include <cstdint>
#include "robotcontol.h"

#define STREAM_WIDTH 1280
#define STREAM_HEIGHT 720
#define STREAM_COLOR_LENGTH 3

/**
 *  Holds the frame to draw on the controlling device along with feedback.
 */
typedef struct {
    float rumbleLevel; // [0.0, 1.0]
    uint32_t rumbleTime; // Milliseconds
    ROBOT_CONTROL_BULK robotControlData; // Current robot positioning

    size_t frameLength;
} STREAM_CONTROL_DATA;

#endif //BEHOLDER_REMOTE_CONTROL_HPP
