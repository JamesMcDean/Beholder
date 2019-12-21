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

        _cameras = std::move(cameras);
        if (useResponseChar) __responseChar = std::make_unique<char>(responseChar);
    }

    auto Robot::__currentTime() -> std::chrono::milliseconds {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
                );
    }

    auto Robot::__updateState() -> void {
        char* structTape = static_cast<char*>(static_cast<void*>(&_robotState));
        serialPutchar(__fd, Config::SERIAL_STATE_CHAR);
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
        serialPutchar(__fd, Config::SERIAL_TEST_CHAR);
        serialPuts(__fd, "OK");
        std::this_thread::sleep_for(std::chrono::milliseconds(Config::SERIAL_RESPONSE_TIME_MILLIS));

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
        // Running data
        const std::chrono::milliseconds updateTime = __currentTime();
        const float durationRaw = (float) updateTime.count() - _lastUpdate.count();
        const float speed = (float) Config::ROBOT_SPEED_PER_SEC / 1000;

        /*
         * Set rotations
         * TODO - Implement targeting.
         *
         * note: Using the previous state of the controller for proper reaction to duration.
         */
        const float bodyRot = _robotState.base_rotation + (speed * durationRaw * _controllerState.rightStickX);
        const float aimPit_p = _robotState.primaryGun_pitch + (speed * durationRaw * _controllerState.rightStickY);
        const float aimPit_s = _robotState.secondaryGun_pitch + (speed * durationRaw * _controllerState.rightStickY);

        _robotState.base_rotation = bodyRot;
        _robotState.primaryGun_pitch = aimPit_p; // TODO
        _robotState.secondaryGun_pitch = aimPit_s; // TODO
        _robotState.primaryGun_rotation = 0; // TODO
        _robotState.secondaryGun_rotation = 0; // TODO

        // Set movement
        _robotState.base_forward = data.leftStickY;
        _robotState.base_strafe = data.leftStickX;

        /*
         * Set firing states
         * note: Using the most recent state of the controller for proper firing response.
         */
        _robotState.firing = data.rightTrigger > Config::TRIGGER_FIRING_THRESHOLD ? FIRING_primary : FIRING_none;
        _robotState.firing |= data.leftTrigger > Config::TRIGGER_FIRING_THRESHOLD ? FIRING_secondary : FIRING_none;

        // Send state
        __updateState();
    }
}