//
// Created by James on 12/7/2019.
//

#ifndef BEHOLDER_ROBOT_HPP
#define BEHOLDER_ROBOT_HPP

#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>

#include "robotcontol.h"
#include "gamecontroller.h"
#include "streamcontrol.hpp"

#include "camera.hpp"

#include <wiringSerial.h>
#include <opencv2/opencv.hpp>

#include "config.hpp"

namespace Control {
    /**
     * Looks through the /dev/ directory in the local system and finds potential serial ports.
     *  note: Looking at baud rates {B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200}.
     * @return A vector of paths contained in strings or nullptr if /dev/ could not be opened.
     */
    auto findPotentialSerialPorts() -> std::shared_ptr<std::vector<std::tuple<std::string, speed_t>>>;

    class Robot {
    private:
        int __fd;
        std::unique_ptr<char> __responseChar = nullptr;

        /**
         * The backend for the constructors.
         * @param devicePath The path that the serial device exists at to communicate with the robot.
         * @param baud The baud rate of the serial connection.
         * @param cameras The cameras to bind to the robot controller.
         * @param responseChar The char to check for after sending a test message.
         * @param useResponseChar If the class should check for a specific response char when testing the connection.
         */
        auto __init(const std::string& devicePath, int baud, std::vector<std::shared_ptr<Vision::Camera>> cameras,
                char responseChar, bool useResponseChar) -> void;

        /**
         * Gets the current milliseconds since January 1st, 1970.
         * @return The milliseconds since January 1st, 1970.
         */
        static auto __currentTime() noexcept -> std::chrono::milliseconds;

        /**
         * Sends the state contained in _robotState to the robot over serial.
         */
        auto __updateState() noexcept -> void;

    protected:
        std::vector<std::shared_ptr<Vision::Camera>> _cameras;

        STREAM_CONTROL_DATA _streamData;
        uint8_t* _previousFrame;

        GAME_CONTROLLER_BULK _controllerState = {0};
        ROBOT_CONTROL_BULK _robotState = {0};
        std::chrono::milliseconds _lastUpdate = __currentTime();

    public:
        /**
         * Constructs the robot with only one camera and no response char defined.
         * @param devicePath The path to the serial device in *nix.
         * @param baud The baud rate to communicate over serial with.
         * @param camera The shared reference to the camera provided for the robot.
         */
        Robot(const std::string& devicePath, int baud, const std::shared_ptr<Vision::Camera>& camera);

        /**
         * Constructs the robot with multiple cameras and no response char defined.
         * @param devicePath The path to the serial device in *nix.
         * @param baud The baud rate to communicate over serial with.
         * @param cameras A vector of shared pointers to the cameras provided for the robot.
         */
        Robot(const std::string& devicePath, int baud, const std::vector<std::shared_ptr<Vision::Camera>>& cameras);

        /**
         * Constructs the robot with only one camera and a response char defined.
         * @param devicePath The path to the serial device in *nix.
         * @param baud The baud rate to communicate over serial with.
         * @param camera The shared reference to the camera provided for the robot.
         * @param responseChar The char to listen for when sending the test message.
         */
        Robot(const std::string& devicePath, int baud,
                const std::shared_ptr<Vision::Camera>& camera, char responseChar);

        /**
         * Constructs the robot with multiple cameras and a response char defined.
         * @param devicePath The path to the serial device in *nix.
         * @param baud The baud rate to communicate over serial with.
         * @param cameras A vector of shared pointers to the cameras provided for the robot.
         * @param responseChar The char to listen for when sending the test message.
         */
        Robot(const std::string& devicePath, int baud,
                const std::vector<std::shared_ptr<Vision::Camera>>& cameras, char responseChar);

        /**
         * Deconstructs and disconnects from the robot.
         */
        ~Robot();

        /**
         * Checks if the robot is connected to the serial connection optionally using the responseChar provided
         * during construction.
         * @return If the robot is connected.
         */
        auto isConnected() noexcept -> bool;

        /**
         * Get the data to send over the stream.
         * @return The data to send over the stream.
         */
        auto getStreamData() noexcept -> STREAM_CONTROL_DATA;

        /**
         * Gets the raw frame of the stream to send over the stream.
         * @return The raw frame of the stream.
         */
        auto getStreamFrame() noexcept -> uint8_t*;

        /**
         * Gets the most recently captured frame from the camera at the index provided.
         * @param index The index of the camera (from the camera vector) to query.
         * @return The OpenCV matrix defining the frame from the camera.
         */
        auto getCameraFrame(size_t index) -> cv::Mat;

        /**
         * Uses the controller state provided (mixed with the most recent update state and time) to control the
         * absolute rotation of the robot's guns along with the other directly mapped states.
         * @param data The controller data to use for the update.
         */
        auto updateControllerState(const GAME_CONTROLLER_BULK& data) -> void;
    };
}

#endif //BEHOLDER_ROBOT_HPP
