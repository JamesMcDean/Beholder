//
// Created by James on 12/7/2019.
//

#include "robot.hpp"

namespace Control {
    auto Robot::__init(const std::string& devicePath, int baud, std::vector<std::shared_ptr<Vision::Camera>> cameras,
                       char responseChar, bool useResponseChar) -> void {
        __fd = serialOpen(devicePath.c_str(), baud);
        if (__fd < 0) throw Exceptions::SerialException((std::string) "Could not open serial for "
                                                        + "the device at \"" + devicePath + "\" with the speed of "
                                                        + std::to_string(baud) + " baud.");

        _cameras = cameras;
        if (useResponseChar) __responseChar = responseChar;
    }

    auto Robot::__updateState() -> void {
        char* structTape = static_cast<char*>(static_cast<void*>(&_robotState));
        serialPuts(__fd, structTape);
        delete[] structTape;
    }

    Robot::Robot(const std::string& devicePath, int baud, const std::shared_ptr<Vision::Camera>& camera) {
        auto cameras = std::vector<std::shared_ptr<Vision::Camera>>();
        cameras.emplace_back(camera);

        __init(devicePath, baud, cameras, '\0', false);
    }

    Robot::Robot(const std::string& devicePath, int baud, const std::vector<std::shared_ptr<Vision::Camera>>& cameras) {
        __init(devicePath, baud, cameras, '\0', false);
    }

    Robot::Robot(const std::string& devicePath, int baud,
            const std::shared_ptr<Vision::Camera>& camera, char responseChar) {
        auto cameras = std::vector<std::shared_ptr<Vision::Camera>>();
        cameras.emplace_back(camera);

        __init(devicePath, baud, cameras, responseChar, true);
    }

    Robot::Robot(const std::string& devicePath, int baud,
            const std::vector<std::shared_ptr<Vision::Camera>>& cameras, char responseChar) {
        __init(devicePath, baud, cameras, responseChar, true);
    }

    Robot::~Robot() {
        serialFlush(__fd);
        serialClose(__fd);
    }

    auto Robot::isConnected() -> bool {
        // TEST
        serialPutchar(__fd, SERIAL_TEST_CHAR);
        serialPuts(__fd, "OK");
        std::this_thread::sleep_for(std::chrono::milliseconds(SERIAL_WAIT_TIME_MILLIS));

        // ACK
        if (serialDataAvail(__fd) && (__responseChar == nullptr || serialGetchar(__fd) == *__responseChar)) {
            serialFlush(__fd);
            return true;
        }

        return false;
    }

    auto Robot::getStreamData() -> STREAM_CONTROL_DATA {
        return _streamData;
    }

    auto Robot::getStreamFrame() -> uint8_t* {
        return _previousFrame;
    }

    auto Robot::getCameraFrame(size_t index) -> cv::Mat {
        std::shared_ptr<Vision::Camera> target = _cameras[index];
        return target->read();
    }

    auto Robot::updateControllerState(const GAME_CONTROLLER_BULK& data) -> void {
        
    }
}