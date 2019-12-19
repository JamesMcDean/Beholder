//
// Created by James on 12/7/2019.
//

#ifndef BEHOLDER_ROBOT_HPP
#define BEHOLDER_ROBOT_HPP

#include <vector>
#include <string>

#include "robotcontol.h"
#include "gamecontroller.h"
#include "streamcontrol.hpp"

#include "connectionexceptions.hpp"
#include "camera.hpp"

#include <wiringSerial.h>
#include <opencv2/opencv.hpp>

#include "config.hpp"

namespace Control {
    class Robot {
    private:
        int __fd;
        std::unique_ptr<char> __responseChar = nullptr;

        auto __init(const std::string& devicePath, int baud, std::vector<std::shared_ptr<Vision::Camera>> cameras,
                char responseChar, bool useResponseChar) -> void;

        auto __updateState() -> void;

    protected:
        std::vector<std::shared_ptr<Vision::Camera>> _cameras;
        STREAM_CONTROL_DATA _streamData; // Row major
        GAME_CONTROLLER_BULK _controlState;
        ROBOT_CONTROL_BULK _robotState;
        uint8_t* _previousFrame;

    public:
        Robot(const std::string& devicePath, int baud, const std::shared_ptr<Vision::Camera>& camera);
        Robot(const std::string& devicePath, int baud, const std::vector<std::shared_ptr<Vision::Camera>>& cameras);

        Robot(const std::string& devicePath, int baud,
                const std::shared_ptr<Vision::Camera>& camera, char responseChar);
        Robot(const std::string& devicePath, int baud,
                const std::vector<std::shared_ptr<Vision::Camera>>& cameras, char responseChar);

        ~Robot();

        auto isConnected() -> bool;
        auto getStreamData() -> STREAM_CONTROL_DATA;
        auto getStreamFrame() -> uint8_t*;
        auto getCameraFrame(size_t index) -> cv::Mat;
        auto updateControllerState(const GAME_CONTROLLER_BULK& data) -> void;
    };
}

#endif //BEHOLDER_ROBOT_HPP
