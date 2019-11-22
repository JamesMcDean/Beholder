//
// Created by James on 10/26/2019.
//

#ifndef BEHOLDER_CAMERA_HPP
#define BEHOLDER_CAMERA_HPP

#include <string>
#include <thread>
#include <iostream>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

namespace Vision {
    class Camera {
    private:
        cv::VideoCapture device;
        std::string settingsPath;
        cv::Mat rawImage = cv::Mat();

        cv::Mat distortionCoefficients = cv::Mat();
        cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
        cv::Mat calibratedImage = cv::Mat();

        bool delayFinished = true;
        std::thread delayedCapture;

    public:
        // Construction
        Camera(int deviceIndex, const std::string& settingsPath);
        Camera(const std::string& devicePath, const std::string& settingsPath);
        ~Camera();

        void capture();
        void capture(uint delayMillis);

        cv::Mat read();

        bool isCalibrated();
    };
}

#endif //BEHOLDER_CAMERA_HPP
