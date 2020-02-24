//
// Created by James on 10/26/2019.
//

#ifndef BEHOLDER_CAMERA_HPP
#define BEHOLDER_CAMERA_HPP

#include <string>
#include <thread>
#include <iostream>
#include <cstdint>
#include <exception>
#include <fstream>
#include <sstream>
#include <memory>

#include <unistd.h>
#include <sys/time.h>

#include <opencv2/opencv.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Vision {
    /**
     * The types of identification for cameras.
     */
    enum CameraIDType {
        INDEX_ID,
        PATH_ID,
    };

    /**
     * The camera lens parameters defined at:
     * https://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html
     */
    struct lensParameters {
        double focalLengthX;
        double focalLengthY;
        double centerX;
        double centerY;
    };

    /**
     * The distortion coefficients defined at:
     * https://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html
     */
    struct distortionCoefficients {
        double k1;
        double k2;
        double p1;
        double p2;
        double k3;
    };

    /**
     * Settings for initializing and calibrating a camera.
     */
    struct cameraSettings {
        enum CameraIDType idType;
        std::string id;
        bool parseError;

        double xyz[3]; // Millimeters
        double rpt[3]; // Degrees

        bool calibrated;

        struct lensParameters lensParams;
        struct distortionCoefficients distortion;
    };

    /**
     * Parses a JSON file containing the definitions for the cameras used.
     * @param filepath The path where the JSON file is located.
     * @return A vector of all of the parsed cameraSettings, or nullptr if the file doesn't
     * appear to be valid.
     */
    auto parseCameraSettings(const std::string& filepath) ->
    std::shared_ptr<std::vector<struct cameraSettings>>;

    class Camera {
    private:
        cv::VideoCapture __device;
        cv::Mat __rawImage = cv::Mat();

        bool __calibrated = false;
        cv::Mat __cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        cv::Mat __distortionMatrix = cv::Mat(1, 5, CV_64F);
        cv::Mat __calibratedImage = cv::Mat();

        bool __delayFinished = true;
        std::unique_ptr<std::thread> __delayedCapture;

    protected:
        /**
         * Applies settings to the private values.
         * @param settings The settings struct to define the camera.
         * @return If the settings succeeded in being applied.
         */
        auto _applySettings(const cameraSettings* settings) -> void;

    public:
        /**
         * Constructs a Camera object based off of the device index and a path to the settings.
         * @param deviceIndex The device index which the camera sit at.
         * @param settingsPath The path to the settings for the camera.
         */
        Camera(int deviceIndex, const std::string& settingsPath);

        /**
         * Constructs a Camera object based off of the device path and a path to the settings.
         * @param devicePath The path to the device which the camera sits at.
         * @param settingsPath The path to the settings for the camera.
         */
        Camera(const std::string& devicePath, const std::string& settingsPath);

        /**
         * Constructs a Camera object purely off of the provided settings struct.
         * @param settings The settings for the camera, containing all parameters for the camera.
         */
        explicit Camera(const struct cameraSettings& settings);
        ~Camera();

        /**
         * Captures an image as fast as possible.
         */
        auto capture() -> void;

        /**
         * Captures an image after a certain delay. If the previous capture delay has not triggered
         * this must first wait for the previous capture. The total delay time is returned which can
         * be compared to the input delay to see if the camera fired at the appropriate time.
         * @param delayMillis The amount of time to wait to capture the image.
         * @returns The actual wait time in milliseconds.
         */
        auto capture(uint64_t delayMillis) -> uint64_t;

        /**
         * Gets the most recently captured image, calibrated if possible.
         * @return The most recently captured raw image.
         */
        auto read() -> cv::Mat;

        /**
         * Returns if the camera has set calibration parameters.
         * @return If the camera has set calibration parameters.
         */
        auto calibrated() -> bool;
    };
}

#endif //BEHOLDER_CAMERA_HPP
