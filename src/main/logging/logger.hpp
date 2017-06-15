#ifndef LOGGING_LOGGER_HPP
#define LOGGING_LOGGER_HPP

#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include "logger-output.hpp"

namespace lang {
    namespace logging {
        struct LoggerOutput;

        class Logger {
            std::vector<std::unique_ptr<LoggerOutput>> outputs;

        public:
            void addOutput(std::unique_ptr<LoggerOutput> output);

            Logger &info(std::string message);

            Logger &warning(std::string message);

            Logger &error(std::string message);
        };
    }
}

#endif // LOGGING_LOGGER_HPP
