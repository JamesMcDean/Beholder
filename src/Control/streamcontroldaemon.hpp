//
// Created by James on 12/2/2019.
//

#ifndef BEHOLDER_STREAM_CONTROL_DAEMON_HPP
#define BEHOLDER_STREAM_CONTROL_DAEMON_HPP

#include <thread>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include "robot.hpp"
#include "streamcontrol.hpp"
#include "config.hpp"
#include "gamecontroller.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace Control {
    class StreamControlDaemon {
    private:
        std::thread __workerThread;
        std::shared_ptr<bool> __running;

        /**
         * The main worker function for the background thread this class makes.
         * @param bot A pointer to the robot that is being sent commands.
         * @param videoPort The UDP port to use when sending the video.
         * @param controlPort The port to send and receive control data over.
         * @param active If the thread is active. As soon as this is set false, the worker exits.
         * @param stopMsg The pointer to the message holding the reason for stopping.
         * @param errPtr The pointer to a potentially thrown exception.
         */
        static auto __workerMain(const std::shared_ptr<Robot>& bot, uint16_t videoPort, uint16_t controlPort,
                                 const std::shared_ptr<bool>& active, const std::shared_ptr<std::string>& stopMsg,
                                 const std::shared_ptr<std::exception>& errPtr) -> void;

        /**
         * Reads the game controller data from the socket.
         * @param socket The socket to read from.
         * @return The game controller data and any resulting boost error code.
         */
        static auto __controlRead(boost::asio::ip::tcp::socket& socket) -> std::tuple<GAME_CONTROLLER_BULK, boost::system::error_code>;

        /**
         * Writes the stream control data into the socket provided.
         * @param socket The socket to write data into.
         * @param control The stream control data to send through the socket.
         * @return Any resulting boost error code.
         */
        static auto __controlWrite(boost::asio::ip::tcp::socket& socket, const STREAM_CONTROL_DATA& control) -> boost::system::error_code;

        /**
         * Writes the frame data into the socket provided.
         * @param socket The socket to write data into.
         * @param endpoint The endpoint to target.
         * @param control The stream control data to use for frame data formatting.
         * @param frame The raw tape of the stream frame.
         * @return Any resulting boost error code.
         */
        static auto __videoWrite(boost::asio::ip::udp::socket& socket,
                boost::asio::ip::udp::endpoint& endpoint,
                const STREAM_CONTROL_DATA& control,
                const uint8_t* frame) -> boost::system::error_code;

    protected:
        const uint16_t _streamPort;
        const uint16_t _controlPort;

        std::shared_ptr<Robot> _bot;

        std::shared_ptr<std::string> _stopReason = std::make_shared<std::string>("Not yet started.");
        std::shared_ptr<std::exception> _stopException;

    public:
        /**
         * Constructs the daemon.
         * @param bot The _bot that should be controlled through this daemon.
         * @param streamPort The port to send the stream data through (UDP).
         * @param controlPort The port to send and receive control data through (TCP).
         * @param start Start the daemon on construction.
         */
        explicit StreamControlDaemon(const std::shared_ptr<Robot>& bot, uint16_t streamPort = Config::REMOTE_STREAM_PORT,
                uint16_t controlPort = Config::REMOTE_CONTROL_PORT, bool start = false);

        /**
         * Deconstructs and stops the daemon.
         */
        ~StreamControlDaemon();

        /**
         * Starts the daemon if not started already.
         * @return If the thread started on this method call.
         */
        auto start() noexcept -> bool;

        /**
         * Stops the daemon.
         */
        auto stop() noexcept -> void;

        /**
         * Checks if the daemon is __running.
         * @return If the daemon is __running.
         */
        auto isRunning() noexcept -> bool;

        /**
         * Gets the reason for the thread stopping.
         * @return The reason for the daemon stopping.
         */
        auto stopReason() noexcept -> std::string;
    };
}

#endif //BEHOLDER_STREAM_CONTROL_DAEMON_HPP
