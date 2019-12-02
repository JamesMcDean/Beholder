//
// Created by James on 11/7/2019.
//

#include "robot.hpp"
namespace fs = std::filesystem;

namespace Control {
    std::map<ROBOT_ID, int32_t> _idsToFD;

    auto connect(const std::string& devicePath, int baud) -> ROBOT_ID {
        // Try to connect
        // TODO - Check if this needs root for whatever reason
        int result = serialOpen(devicePath.c_str(), baud);
        if (result < 0) return NULL;

        // Find mediocre id hash
        int32_t id = ((clock() * 29) % (int32_t) (pow(2, 32) - 1)) + 1;
        while (_idsToFD.count(id) || id == 0) id = ((clock() * 29) % (int) (pow(2, 32) - 1)) + 1;

        // Save
        _idsToFD.insert(std::pair<int, int>(id, result));
        return id;
    }

    auto clean(const char* responseChar = nullptr) -> std::vector<ROBOT_ID> {
        // Running data
        std::vector<std::thread> threads;
        std::vector<ROBOT_ID> result;

        // Check for unresponsive serial connections
        for (auto const& [key, val] : _idsToFD) {
            ROBOT_ID id = key;
            auto fd = val;

            std::thread thread([id, fd, &result, responseChar]() {
                serialPuts(fd, "TOK");
                std::this_thread::sleep_for(std::chrono::milliseconds(SERIAL_WAIT_TIME_MILLIS));

                if (serialDataAvail(fd) && (responseChar == nullptr || serialGetchar(fd) == *responseChar)) {
                    serialFlush(fd);
                    std::terminate();
                }

                serialClose(id);
                _idsToFD.erase(id);
                result.push_back(id);
            });
            thread.detach();
            threads.push_back(thread);
        }

        // Wait for worker threads
        for (auto &thread : threads) {
            thread.join();
        }

        // Return dead IDs
        return result;
    }

    auto updateRobotState(ROBOT_ID id, ROBOT_CONTROL_BULK state) -> void {
        auto fd = _idsToFD.at(id);

        char* stateChars = reinterpret_cast<char*>(&state);
        serialPutchar(fd, 'S');
        serialPuts(fd, stateChars);
    }
}
