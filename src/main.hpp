//
// Created by james on 1/5/20.
//

#ifndef BEHOLDER_MAIN_HPP
#define BEHOLDER_MAIN_HPP

#include <iostream>
#include <map>
#include <string>
#include <memory>

#include <robot.hpp>
#include <streamcontroldaemon.hpp>
#include <config.hpp>

struct inputs {
    std::shared_ptr<std::string> serialPath;
    std::shared_ptr<int> baud;
    std::shared_ptr<uint> responseTime;
    std::shared_ptr<char> stateChar;
    std::shared_ptr<char> testChar;
    std::shared_ptr<std::string> robotSettingsPath;
    std::shared_ptr<uint16_t> controlPort;
    std::shared_ptr<uint16_t> streamPort;
    std::shared_ptr<float> speed;
    std::shared_ptr<float> threshold;
};

auto printVersion() -> void;
auto printHelp() -> void;
auto compileArguments(int argc, char* argv[]) -> struct inputs;
auto processArguments(struct inputs* arguments) -> int;

#endif //BEHOLDER_MAIN_HPP
