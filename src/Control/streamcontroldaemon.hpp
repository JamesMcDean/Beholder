//
// Created by James on 12/2/2019.
//

#ifndef BEHOLDER_STREAM_CONTROL_DAEMON_HPP
#define BEHOLDER_STREAM_CONTROL_DAEMON_HPP

#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <map>

#include "control.hpp"
#include "streamcontrol.hpp"
#include "config.hpp"

namespace Control {
    class StreamControlDaemon {
    private:
        std::thread workerThread;
        std::shared_ptr<bool> running;

        static auto workerMain(ROBOT_ID bot, uint16_t port, const std::shared_ptr<bool>& active) -> void;
    protected:
        const uint16_t port;
        const ROBOT_ID bot;

    public:
        /**
         * Creates the daemon.
         * @param id The id of the robot to send commands to.
         * @param port The networking port to use for the stream communication.
         * @param start Start the daemon on construction.
         */
        explicit StreamControlDaemon(ROBOT_ID id, uint16_t port = REMOTE_PORT, bool start = false);
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
