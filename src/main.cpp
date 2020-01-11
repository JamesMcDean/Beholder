//
// Created by James on 10/26/2019.
//

#include "main.hpp"

auto main(int argc, char* argv[]) -> int {
    // Process arguments into an easy to use state
    auto arguments = compileArguments(argc, argv);
    int pResult = processArguments(&arguments);
    if (pResult != 0) {
        return pResult;
    }



    return 0;
}

auto printVersion() -> void {
    printf("Beholder v%d.%d - (C) 2019-2020 James Hamilton McRoberts IV\n",
           Config::VERSION_MAJOR, Config::VERSION_MINOR);
    printf("Released under the GNU GPLv3 license.\n");
}

auto printHelp() -> void {
    // Version
    printVersion();

    printf("\n");

    // Diagnostic
    printf("-h --help\t");
    printf("Prints this help message.\n");

    printf("-v --version\t");
    printf("Prints the version chunk seen at the beginning of this message.\n");

    printf("-V --verbose\t");
    printf("Makes the output more verbose.\n");

    printf("\n");

    // Network
    printf("-S --serial-path [string PATH]\t");
    printf("The path that the serial connection exists on.\n");

    printf("-B --baud [int baud]\t");
    printf("The baud rate used to communicate over the serial connection with.\n");

    printf("   --serial-response-time [uint MILLIS]\t");
    printf("The time in milliseconds to wait for the response message from the robot serial port.\n");

    printf("   --serial-state-char [char STATE]\t");
    printf("The char to use when sending a robot state. (default: 'S')\n");

    printf("   --serial-test-char [char TEST]\t");
    printf("The optional char to use when testing for a response.\n");

    printf("   --control-port [uint16_t PORT_NUM]\t");
    printf("Sets the TCP port to use for the control data.\n");

    printf("   --stream-port [uint16_t PORT_NUM]\t");
    printf("Sets the UDP port to use for the stream payloads.\n");

    printf("\n");

    // Robot
    printf("-s --speed [float SPEED]\t");
    printf("Sets the speed at which the value of any control parameter is changed per second.\n");

    printf("-f --fire-threshold [float[0.0, 1.0] THRESH]\t");
    printf("Sets the threshold that must be crossed for the gun to fire (zero being always firing).\n");

    printf("\n");
}

auto __argumentCrash(const std::string& argument) -> void {
    fprintf(stderr, "Invalid argument: %s\nHalting.\n", argument.c_str());
    exit(-1);
}

auto compileArguments(int argc, char* argv[]) -> struct inputs {
    struct inputs result = {nullptr};

    for (int index = 1; index < argc; index++) {
        std::string argument = argv[index];

        // Worded arguments
        if (argument.length() > 2 && argument.substr(0, 2) == "--") {
            std::string fArg = argument.substr(2, argument.length() - 2);

            if (fArg == "help") {
                printHelp();
            }
            else if (fArg == "version") {
                printVersion();
            }
            else if (fArg == "verbose") {
                Config::verbose(true);
            }
            else if (index < argc - 1) {
                std::string next = argv[index + 1];

                try {
                    if (fArg == "serial-path") {
                        *(result.serialPath) = next;
                    } else if (fArg == "baud") {
                        *(result.baud) = std::stoi(next);
                    } else if (fArg == "serial-response-time") {
                        *(result.responseTime) = (uint) std::stoul(next);
                    } else if (fArg == "serial-state-char") {
                        *(result.stateChar) = next[0];
                    } else if (fArg == "serial-test-char") {
                        *(result.testChar) = next[1];
                    } else if (fArg == "control-port") {
                        *(result.controlPort) = (uint16_t) std::stoul(next);
                    } else if (fArg == "stream-port") {
                        *(result.streamPort) = (uint16_t) std::stoul(next);
                    } else if (fArg == "speed") {
                        *(result.speed) = std::stof(next);
                    } else if (fArg == "fire-threshold") {
                        *(result.threshold) = std::stof(next);
                    }
                }
                catch (std::invalid_argument& ex) {
                    std::string argMsg = argument;
                    argMsg += " ";
                    argMsg += next;

                    __argumentCrash(argMsg);
                }

                index += 1;
            }
            else {
                __argumentCrash(argv[index]);
            }
        }
        // char arguments
        else if (argument.length() > 1 && argument[0] == '-') {
            for (int jndex = 1; jndex < argument.length(); jndex++) {
                char flag = argument[jndex];
                bool extra = false;

                // Compoundable flags
                switch (flag) {
                    case 'h': {
                        printHelp();
                        break;
                    }
                    case 'v': {
                        printVersion();
                        break;
                    }
                    case 'V': {
                        Config::verbose(true);
                        break;
                    }
                    default: {
                        extra = true;
                        break;
                    }
                }

                // Crash if not enough arguments
                if (extra && index < argc - 1) {
                    __argumentCrash(argument);
                }

                // Non-compoundable flags
                std::string next = argv[index + 1];
                switch (flag) {
                    case 'S': {
                        *(result.serialPath) = next;
                        break;
                    }
                    case 'B': {
                        *(result.baud) = std::stoi(next);
                    }
                    case 's': {
                        *(result.speed) = std::stof(next);
                        break;
                    }
                    case 'f': {
                        *(result.threshold) = std::stof(next);
                        break;
                    }
                    // Skips
                    case 'h':
                    case 'v':
                    case 'V': {
                        break;
                    }
                    // Invalid
                    default: {
                        __argumentCrash(argument);
                    }
                }
            }
        }
        // Invalid
        else {
            __argumentCrash(argument);
        }
    }

    return result;
}

auto processArguments(struct inputs* arguments) -> int {
    // Handle serial specifications
    if (arguments->serialPath == nullptr ^ arguments->baud == nullptr) {
        fprintf(stderr, "Both the path for the serial device and the baud rate must be specified.\n");
        return EXIT_FAILURE;
    }
    else if (arguments->serialPath == nullptr && arguments->baud == nullptr) {
        fprintf(stdout, "Finding serial ports...\n");

        auto serialPorts = Control::findPotentialSerialPorts();
        if (serialPorts == nullptr) {
            fprintf(stderr, "/dev/ could not be opened.");
            return EXIT_FAILURE;
        }

        if (serialPorts->empty()) {
            fprintf(stderr, "No serial ports could be found/opened.");
            return EXIT_FAILURE;
        }

        fprintf(stdout, "*");
        for (auto port : *serialPorts) {
            fprintf(stdout, "\t%s <-> %d\n", std::get<0>(port).c_str(), std::get<1>(port));
        }

        auto port = serialPorts->at(0);
        *arguments->serialPath = std::get<0>(port);
        *arguments->baud = std::get<1>(port);
    }

    // Others
    if (arguments->responseTime == nullptr) *arguments->responseTime = Config::SERIAL_RESPONSE_TIME_MILLIS;
    if (arguments->stateChar == nullptr) *arguments->stateChar = Config::SERIAL_STATE_CHAR;
    if (arguments->testChar == nullptr) *arguments->testChar = Config::SERIAL_TEST_CHAR;
    if (arguments->controlPort == nullptr) *arguments->controlPort = Config::REMOTE_CONTROL_PORT;
    if (arguments->streamPort == nullptr) *arguments->streamPort = Config::REMOTE_STREAM_PORT;
    if (arguments->speed == nullptr) *arguments->speed = Config::ROBOT_SPEED_PER_SEC;
    if (arguments->threshold == nullptr) *arguments->threshold = Config::TRIGGER_FIRING_THRESHOLD;

    return 0;
}