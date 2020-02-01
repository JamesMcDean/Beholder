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

    auto Camera::capture() -> void {
        device >> rawImage;

        if (!calibrated()) {
            return;
        }

        // TODO - Calibration math
    }

    auto Camera::capture(uint delayMillis) -> void {
        if (!delayFinished) delayedCapture.join();

        delayedCapture = std::thread(
                [this, delayMillis]() {
            this->delayFinished = false;

            usleep(delayMillis);
            this->capture();

            this->delayFinished = true;
        });

        delayedCapture.detach();
    }

    auto Camera::read() -> cv::Mat {
        return calibrated() ? calibratedImage : rawImage;
    }

    auto Camera::calibrated() -> bool {
        return !settingsPath.empty();
    }
}