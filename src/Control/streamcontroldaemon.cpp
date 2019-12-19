//
// Created by James on 12/2/2019.
//

#include "streamcontroldaemon.hpp"
#include "math.cpp"

namespace Control {
    StreamControlDaemon::StreamControlDaemon(ROBOT_ID id, uint16_t videoPort, uint16_t controlPort, bool start)
    : bot(id), videoPort(videoPort), controlPort(controlPort) {
        running = std::make_shared<bool>(start);
        if (start) __workerThread = std::thread(workerMain, bot, videoPort, controlPort, running);
    }

    StreamControlDaemon::~StreamControlDaemon() {
        *running = false;
        if (__workerThread.joinable()) __workerThread.join();
    }

    auto StreamControlDaemon::workerMain(ROBOT_ID bot, uint16_t videoPort, uint16_t controlPort,
            const std::shared_ptr<bool>& active) -> void {
        using namespace boost::asio;
        using ip::tcp;
        using ip::udp;

        boost::asio::io_service service;

        tcp::acceptor controlAcceptor(service, tcp::endpoint(tcp::v4(), REMOTE_CONTROL_PORT));
        tcp::socket controlSocket(service);
        controlAcceptor.accept(controlSocket);

        while (*active) {
            GAME_CONTROLLER_BULK controlData = socketRead(controlSocket);
            Control::updateRobotState(bot, );
        }
    }

    auto StreamControlDaemon::socketRead(boost::asio::ip::tcp::socket& socket) -> GAME_CONTROLLER_BULK {
        using namespace boost::asio;
        using ip::tcp;


    }

    auto StreamControlDaemon::socketWrite(boost::asio::ip::tcp::socket &socket,
            const STREAM_CONTROL_DATA& data) -> void {
        using namespace boost::asio;
        using ip::udp;


    }

    auto StreamControlDaemon::start() -> bool {
        if (*running) return false;

        *running = true;
        __workerThread = std::thread(workerMain, this->bot, this->videoPort, this->controlPort, running);

        return true;
    }

    auto StreamControlDaemon::stop() -> void {
        *running = false;
    }

    auto StreamControlDaemon::isRunning() -> bool {
        return *running;
    }
}
