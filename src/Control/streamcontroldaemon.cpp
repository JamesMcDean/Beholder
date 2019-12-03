//
// Created by James on 12/2/2019.
//

#include "streamcontroldaemon.hpp"
#include "math.cpp"

namespace Control {
    StreamControlDaemon::StreamControlDaemon(ROBOT_ID id, uint16_t port, bool start) : bot(id), port(port) {
        running = std::make_shared<bool>(start);
        if (*running) workerThread = std::thread(workerMain, this->bot, this->port, running);
    }

    StreamControlDaemon::~StreamControlDaemon() {
        *running = false;
        if (workerThread.joinable()) workerThread.join();
    }

    auto StreamControlDaemon::workerMain(ROBOT_ID bot, uint16_t port, const std::shared_ptr<bool>& active) -> void {
        while (*active) {
            // TODO - Socket logic
        }
    }

    auto StreamControlDaemon::start() -> bool {
        if (*running) return false;

        *running = true;
        workerThread = std::thread(workerMain, this->bot, this->port, running);

        return true;
    }

    auto StreamControlDaemon::stop() -> void {
        *running = false;
    }

    auto StreamControlDaemon::isRunning() -> bool {
        return *running;
    }
}
