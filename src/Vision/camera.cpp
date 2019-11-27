//
// Created by James on 10/26/2019.
//

#include "camera.hpp"

namespace Vision {
    Camera::Camera(int deviceIndex, const std::string& settingsPath) {
        device = cv::VideoCapture(deviceIndex);
        this->settingsPath = settingsPath;
    }

    Camera::Camera(const std::string& devicePath, const std::string& settingsPath) {
        device = cv::VideoCapture(devicePath);
        this->settingsPath = settingsPath;
    }

    Camera::~Camera() {
        if (!delayFinished) {
            delayedCapture.join();
        }

        device.release();
    }

    void Camera::capture() {
        device >> rawImage;

        if (!isCalibrated()) {
            return;
        }

        // TODO - Calibration math
    }

    void Camera::capture(uint delayMillis) {
        if (!delayFinished) delayedCapture.join();

        delayedCapture = std::thread(
                [this, delayMillis](Camera* parent, uint delay) {
            parent->delayFinished = false;

            usleep(delay);
            parent->capture();

            parent->delayFinished = true;
        });

        delayedCapture.detach();
    }

    cv::Mat Camera::read() {
        return isCalibrated() ? calibratedImage : rawImage;
    }

    bool Camera::isCalibrated() {
        return !settingsPath.empty();
    }
}