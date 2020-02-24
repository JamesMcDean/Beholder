//
// Created by James on 10/26/2019.
//

#include "camera.hpp"

#define CAMERA_KEY "cameras"

#define ID_KEY "id"
#define ID_TYPE_KEY "idType"
#define ID_TYPE_INDEX "index"
#define ID_TYPE_PATH "path"

#define POSITION_KEY "position"
#define POS_X_KEY "x"
#define POS_Y_KEY "y"
#define POS_Z_KEY "z"

#define ROTATION_KEY "rotation"
#define ROT_RHO_KEY "rho"
#define ROT_PHI_KEY "phi"
#define ROT_THETA_KEY "theta"

#define LENS_KEY "lensParameters"
#define LENS_FOCAL_X_KEY "focalLengthX"
#define LENS_FOCAL_Y_KEY "focalLengthY"
#define LENS_CENTER_X_KEY "centerX"
#define LENS_CENTER_Y_KEY "centerY"

#define DISTORTION_KEY "distortionCoefficients"
#define DIST_K1_KEY "k1"
#define DIST_K2_KEY "k2"
#define DIST_P1_KEY "p1"
#define DIST_P2_KEY "p2"
#define DIST_K3_KEY "k3"

namespace Vision {
    auto parseCameraSettings(const std::string& filepath) ->
    std::shared_ptr<std::vector<struct cameraSettings>> {
        using boost::property_tree::ptree;

        // Parse as JSON
        ptree propertyTree;
        read_json(filepath, propertyTree);

        // Find entry for cameras
        ptree::const_iterator end = propertyTree.end();
        ptree::const_iterator camerasIter;
        for (camerasIter = propertyTree.begin(); camerasIter != end; ++camerasIter) {
            if (camerasIter->first == CAMERA_KEY) break;
        }
        if (camerasIter == end) return nullptr;

        // Convert each entry to a camera
        auto result = std::make_shared<std::vector<struct cameraSettings>>(
                std::vector<struct cameraSettings>());
        end = camerasIter->second.end();
        // Each entry
        for (ptree::const_iterator cameraIter = camerasIter->second.begin(); cameraIter != end;
        ++cameraIter) {
            struct cameraSettings camera = {};

            // Each item
            for (ptree::const_iterator itemIter = cameraIter->second.begin();
            itemIter != cameraIter->second.end(); ++itemIter) {
                auto key = (std::string) itemIter->first;
                auto item = itemIter->second.get_value<std::string>();
                // TODO - Verify this works
                auto subIter = itemIter->second.begin();
                auto subEnd = itemIter->second.end();

                if (key == ID_KEY) {
                    camera.id = item;
                }
                else if (key == ID_TYPE_KEY) {
                    if (item == ID_TYPE_INDEX) {
                        camera.idType = INDEX_ID;
                    }
                    else if (item == ID_TYPE_PATH) {
                        camera.idType = PATH_ID;
                    }
                    else {
                        camera.parseError = true;
                    }
                }
                else if (key == POSITION_KEY) {
                    for (; subIter != subEnd; ++subIter) {
                        auto posKey = (std::string) subIter->first;
                        auto posItem = std::stod(subIter->second.get_value<std::string>());

                        if (posKey == POS_X_KEY) {
                            camera.xyz[0] = posItem;
                        }
                        else if (posKey == POS_Y_KEY) {
                            camera.xyz[1] = posItem;
                        }
                        else if (posKey == POS_Z_KEY) {
                            camera.xyz[2] = posItem;
                        }
                        else {
                            camera.parseError = true;
                        }
                    }
                }
                else if (key == ROTATION_KEY) {
                    for (; subIter != subEnd; ++subIter) {
                        auto rotKey = (std::string) subIter->first;
                        auto rotValue = std::stod(subIter->second.get_value<std::string>());

                        if (rotKey == ROT_RHO_KEY) {
                            camera.rpt[0] = rotValue;
                        }
                        else if (rotKey == ROT_PHI_KEY) {
                            camera.rpt[1] = rotValue;
                        }
                        else if (rotKey == ROT_THETA_KEY) {
                            camera.rpt[2] = rotValue;
                        }
                        else {
                            camera.parseError = true;
                        }
                    }
                }
                else if (key == LENS_KEY) {
                    for (; subIter != subEnd; ++subIter) {
                        auto lenKey = (std::string) subIter->first;
                        auto lenValue = std::stod(subIter->second.get_value<std::string>());

                        if (lenKey == LENS_FOCAL_X_KEY) {
                            camera.lensParams.focalLengthX = lenValue;
                        }
                        else if (lenKey == LENS_FOCAL_Y_KEY) {
                            camera.lensParams.focalLengthY = lenValue;
                        }
                        else if (lenKey == LENS_CENTER_X_KEY) {
                            camera.lensParams.centerX = lenValue;
                        }
                        else if (lenKey == LENS_CENTER_Y_KEY) {
                            camera.lensParams.centerY = lenValue;
                        }
                        else {
                            camera.parseError = true;
                        }
                    }

                    camera.calibrated = true;
                }
                else if (key == DISTORTION_KEY) {
                    for (; subIter != subEnd; ++subIter) {
                        auto distKey = (std::string) subIter->first;
                        auto distValue = std::stod(subIter->second.get_value<std::string>());

                        if (distKey == DIST_K1_KEY) {
                            camera.distortion.k1 = distValue;
                        }
                        else if (distKey == DIST_K2_KEY) {
                            camera.distortion.k2 = distValue;
                        }
                        else if (distKey == DIST_P1_KEY) {
                            camera.distortion.p1 = distValue;
                        }
                        else if (distKey == DIST_P2_KEY) {
                            camera.distortion.p2 = distValue;
                        }
                        else if (distKey == DIST_K3_KEY) {
                            camera.distortion.k3 = distValue;
                        }
                        else {
                            camera.parseError = true;
                        }
                    }

                    camera.calibrated = true;
                }
                else {
                    camera.parseError = true;
                }
            }

            result->push_back(camera);
        }

        return result;
    }

    auto Camera::_applySettings(const cameraSettings* settings) -> void {
        __calibrated = settings->calibrated;
        if (__calibrated) {
            __cameraMatrix.at<double>(0, 0) = settings->lensParams.focalLengthX;
            __cameraMatrix.at<double>(1, 1) = settings->lensParams.focalLengthY;
            __cameraMatrix.at<double>(0, 2) = settings->lensParams.centerX;
            __cameraMatrix.at<double>(1, 2) = settings->lensParams.centerY;
            __cameraMatrix.at<double>(2, 2) = 1.0;

            __distortionMatrix.at<double>(0, 0) = settings->distortion.k1;
            __distortionMatrix.at<double>(0, 1) = settings->distortion.k2;
            __distortionMatrix.at<double>(0, 2) = settings->distortion.p1;
            __distortionMatrix.at<double>(0, 3) = settings->distortion.p2;
            __distortionMatrix.at<double>(0, 4) = settings->distortion.k3;
        }

        switch (settings->idType) {
            case INDEX_ID: {
                __device = cv::VideoCapture(std::stoi(settings->id));
                break;
            }
            case PATH_ID: {
                __device = cv::VideoCapture(settings->id);
                break;
            }
        }
    }

    Camera::Camera(int deviceIndex, const std::string& settingsPath) {
        // Grab settings for the camera
        auto settingsOptions = parseCameraSettings(settingsPath);
        struct cameraSettings* settings = nullptr;
        for (const auto& element : *settingsOptions) {
            if (element.idType != INDEX_ID) continue;
            if (element.id == std::to_string(deviceIndex)) {
                settings = new struct cameraSettings;
                *settings = element;
                break;
            }
        }

        // No settings found, throw
        if (settings == nullptr) {
            throw std::runtime_error("No settings found for the camera at device index ["
            + std::to_string(deviceIndex) +"] with the settings file at\"" + settingsPath + "\"");
        }

        _applySettings(settings);
    }

    Camera::Camera(const std::string& devicePath, const std::string& settingsPath) {
        // Grab settings for the camera
        auto settingsOptions = parseCameraSettings(settingsPath);
        struct cameraSettings* settings = nullptr;
        for (const auto& element : *settingsOptions) {
            if (element.idType != PATH_ID) continue;
            if (element.id == devicePath) {
                settings = new struct cameraSettings;
                *settings = element;
                break;
            }
        }

        // No settings found, throw
        if (settings == nullptr) {
            throw std::runtime_error("No settings found for the camera at path \""
            + devicePath + "\" with the settings file at \"" + settingsPath + "\"");
        }

        _applySettings(settings);
    }

    Camera::Camera(const struct cameraSettings& settings) {
        _applySettings(&settings);
    }

    Camera::~Camera() {
        if (!__delayFinished) {
            __delayedCapture->join();
        }

        __device.release();
    }

    auto Camera::capture() -> void {
        __device >> __rawImage;

        if (!calibrated()) {
            return;
        }

        cv::undistort(__rawImage, __calibratedImage,
                __cameraMatrix, __distortionMatrix);
    }

    auto Camera::capture(uint64_t delayMillis) -> uint64_t {
        // Join delay measurement
        uint64_t joinDuration = 0;
        if (!__delayFinished) {
            // Get start time
            struct timeval timerTransfer = {0};
            gettimeofday(&timerTransfer, nullptr);
            uint64_t start = timerTransfer.tv_sec * 1000 + timerTransfer.tv_usec / 1000;

            // Join
            __delayedCapture->join();

            // Get end time
            gettimeofday(&timerTransfer, nullptr);
            uint64_t end = timerTransfer.tv_sec * 100 + timerTransfer.tv_usec / 1000;
            joinDuration = end - start;

            // Update delay millis
            if (joinDuration > delayMillis) delayMillis = 0;
            else delayMillis -= joinDuration;
        }

        // Delay capture
        __delayedCapture = std::make_unique<std::thread>(
                [this, delayMillis]() {
            this->__delayFinished = false;

            usleep(delayMillis);
            this->capture();

            this->__delayFinished = true;
        });

        // Return the actual delay time
        __delayedCapture->detach();
        return delayMillis + joinDuration;
    }

    auto Camera::read() -> cv::Mat {
        return calibrated() ? __calibratedImage : __rawImage;
    }

    auto Camera::calibrated() -> bool {
        return __calibrated;
    }
}