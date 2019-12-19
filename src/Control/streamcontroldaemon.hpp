//
// Created by James on 12/2/2019.
//

#ifndef BEHOLDER_STREAM_CONTROL_DAEMON_HPP
#define BEHOLDER_STREAM_CONTROL_DAEMON_HPP

#include <thread>
#include <cstdlib>
#include <map>
#include <iostream>

#include "control.hpp"
#include "streamcontrol.hpp"
#include "config.hpp"
#include "gamecontroller.h"

#include <boost/asio.hpp>

namespace Control {
    class StreamControlDaemon {
    private:
        std::thread __workerThread;
        std::shared_ptr<bool> running;

        static auto workerMain(ROBOT_ID bot, uint16_t videoPort, uint16_t controlPort,
                const std::shared_ptr<bool>& active) -> void;

        static auto socketRead(boost::asio::ip::tcp::socket& socket) -> GAME_CONTROLLER_BULK;
        static auto socketWrite(boost::asio::ip::tcp::socket& socket, const STREAM_CONTROL_DATA& data) -> void;
    protected:
        const uint16_t videoPort;
        const uint16_t controlPort;
        const ROBOT_ID bot;

    public:
        /**
         * Creates the daemon.
         * @param id The id of the robot to send commands to.
         * @param port The networking port to use for the stream communication.
         * @param start Start the daemon on construction.
         */
        explicit StreamControlDaemon(ROBOT_ID id, uint16_t videoPort = REMOTE_VIDEO_PORT,
                uint16_t controlPort = REMOTE_CONTROL_PORT, bool start = false);
        ~StreamControlDaemon();

        /**
         * Starts the daemon if not started already.
         * @return If the thread started on this method call.
         */
        auto start() -> bool;

        /**
         * Stops the daemon.
         */
        auto stop() -> void;

        /**
         * Checks if the daemon is running.
         * @return If the daemon is running.
         */
        auto isRunning() -> bool;
    };
}

#endif //BEHOLDER_STREAM_CONTROL_DAEMON_HPP
