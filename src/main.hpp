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
#include <config.hpp>

struct inputs {
    std::unique_ptr<std::string> serialPath;
    std::unique_ptr<int> baud;
    std::unique_ptr<uint> responseTime;
    std::unique_ptr<char> stateChar;
    std::unique_ptr<char> testChar;
    std::unique_ptr<uint16_t> controlPort;
    std::unique_ptr<uint16_t> streamPort;
    std::unique_ptr<float> speed;
    std::unique_ptr<float> threshold;
};

auto printVersion() -> void;
auto printHelp() -> void;
auto compileArguments(int argc, char* argv[]) -> struct inputs;
auto processArguments(struct inputs* arguments) -> int;

#endif //BEHOLDER_MAIN_HPP
