#ifndef LOGGING_LOGGER_OUTPUT_HPP
#define LOGGING_LOGGER_OUTPUT_HPP

#include <string>

namespace lang {
    namespace logging {
        enum class LogLevel {
            Info,
            Warning,
            Error,
        };

        struct LogEntry {
            LogLevel level;
            std::string message;
        };

        struct LoggerOutput {
            virtual ~LoggerOutput() {}

            virtual void write(const LogEntry &entry) = 0;
        };
    }
}

#endif // LOGGING_LOGGER_OUTPUT_HPP
