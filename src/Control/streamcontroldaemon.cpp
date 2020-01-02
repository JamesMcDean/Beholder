//
// Created by James on 12/2/2019.
//

#include "streamcontroldaemon.hpp"

namespace Control {
    StreamControlDaemon::StreamControlDaemon(const std::shared_ptr<Robot>& bot, uint16_t streamPort, uint16_t controlPort, bool start)
    : _bot(bot), _streamPort(streamPort), _controlPort(controlPort) {
        __running = std::make_shared<bool>(start);
        if (start) __workerThread = std::thread(__workerMain, bot, streamPort, controlPort,
                __running, _stopReason, _stopException);
    }

    StreamControlDaemon::~StreamControlDaemon() {
        *__running = false;
        if (__workerThread.joinable()) __workerThread.join();
    }

    auto StreamControlDaemon::__workerMain(const std::shared_ptr<Robot>& bot, uint16_t videoPort, uint16_t controlPort,
                                           const std::shared_ptr<bool>& active, const std::shared_ptr<std::string>& stopMsg,
                                           const std::shared_ptr<std::exception>& errPtr) -> void {
        using namespace boost::asio;
        using ip::tcp;
        using ip::udp;

        // Running data
        GAME_CONTROLLER_BULK controlData;
        boost::system::error_code err;

        try {
            // Set up connections
            boost::asio::io_service service;
            tcp::socket controlSocket(service);
            tcp::acceptor controlAcceptor(service, tcp::endpoint(tcp::v4(), controlPort));

            udp::socket streamFrameSocket(service, udp::endpoint(udp::v4(), videoPort));
            udp::endpoint streamEndpoint;
            {
                boost::array<uint8_t, 1> rcvBuf{};
                streamFrameSocket.receive_from(buffer(rcvBuf), streamEndpoint);
            }

            // Exchange data
            while (*active && !err) {
                auto controlResult = __controlRead(controlSocket);
                controlData = std::get<0>(controlResult);
                err = std::get<1>(controlResult);
                if (err || !*active) break;
                bot->updateControllerState(controlData);

                err = __controlWrite(controlSocket, bot->getStreamData());
                if (err || !*active) break;
                err = __videoWrite(streamFrameSocket, streamEndpoint, bot->getStreamData(), bot->getStreamFrame());
            }
        }
        catch (std::exception& ex) {
            *active = false;

            *stopMsg = "Exception: ";
            *stopMsg += ex.what();
            *errPtr = ex;
        }

        if (err) {
            *active = false;

            *stopMsg = err.message();
            //             MESSAGE                 ' '  (   ERROR_VALUE                            )  '\0'
            char msgBuffer[err.message().length() + 1 + 1 + std::to_string(err.value()).length() + 1 + 1];
            sprintf(msgBuffer, "%s (%d)", err.message().c_str(), err.value());
            *errPtr = std::runtime_error(msgBuffer); // NOLINT(cppcoreguidelines-slicing)
        }
    }

    auto StreamControlDaemon::__controlRead(boost::asio::ip::tcp::socket& socket) -> std::tuple<GAME_CONTROLLER_BULK, boost::system::error_code> {
        GAME_CONTROLLER_BULK result;
        using namespace boost::asio;

        boost::system::error_code err;
        read(socket, buffer(&result, sizeof(GAME_CONTROLLER_BULK)), err);
        return std::make_tuple(result, err);
    }

    auto StreamControlDaemon::__controlWrite(boost::asio::ip::tcp::socket& socket,
                                             const STREAM_CONTROL_DATA& control) -> boost::system::error_code {
        using namespace boost::asio;

        boost::system::error_code err;
        write(socket, buffer(&control, sizeof(GAME_CONTROLLER_BULK)), err);
        return err;
    }

    auto StreamControlDaemon::__videoWrite(boost::asio::ip::udp::socket& socket,
            boost::asio::ip::udp::endpoint& endpoint,
            const STREAM_CONTROL_DATA& control,
            const uint8_t* frame) -> boost::system::error_code {
        using namespace boost::asio;

        auto frameBuffer = buffer(frame, control.frameLength);
        boost::system::error_code err;
        socket.send_to(frameBuffer, endpoint, 0, err);

        return err;
    }

    auto StreamControlDaemon::start() noexcept -> bool {
        if (*__running) return false;

        *__running = true;
        __workerThread = std::thread(__workerMain, _bot, _streamPort, _controlPort,
                __running, _stopReason, _stopException);

        return true;
    }

    auto StreamControlDaemon::stop() noexcept -> void {
        *__running = false;
        *_stopReason = "Stopped by server.";
    }

    auto StreamControlDaemon::isRunning() noexcept -> bool {
        return *__running;
    }

    auto StreamControlDaemon::stopReason() noexcept -> std::string {
        return *_stopReason;
    }
}
