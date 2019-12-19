//
// Created by James on 12/8/2019.
//

#ifndef BEHOLDER_CONNECTIONEXCEPTIONS_HPP
#define BEHOLDER_CONNECTIONEXCEPTIONS_HPP

#include <stdexcept>

namespace Exceptions {
    /**
     * An exception thrown during communication over Serial.
     *
     *
     */
    class SerialException : public std::runtime_error {
    public:
        SerialException() : std::runtime_error("Serial issue.") {}

        /**
         * Constructs an exception containing the provided message.
         * @param message The message to contain in the exception.
         */
        explicit SerialException(const std::string& message) : std::runtime_error(message) {}
    };

    /**
     * An exception thrown during communications over a network.
     */
    class NetworkingException : std::runtime_error {
    protected:
        const std::string reason;
    public:
        /**
         * The default constructor.
         */
        NetworkingException() : std::runtime_error("Networking issue.") {}

        /**
         * Constructs an exception containing the provided message.
         * @param message The message to contain in the exception.
         */
        explicit NetworkingException(const std::string& message)
        : std::runtime_error(message), reason(message) {}
    };
}

#endif //BEHOLDER_CONNECTIONEXCEPTIONS_HPP
