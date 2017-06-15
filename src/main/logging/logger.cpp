#include "logger.hpp"
#include "logger-output.hpp"
#include <algorithm>
#include <functional>

using namespace std::placeholders;

namespace lang {
    namespace logging {
        namespace {
            void loggerBroadcast(
                    std::vector<std::unique_ptr<LoggerOutput>> &outputs,
                    LogLevel level,
                    std::string message
            ) {
                LogEntry entry = {
                        level,
                        move(message)};
                for_each(outputs.begin(), outputs.end(), std::bind(&LoggerOutput::write, _1, entry));
            }
        }

        void Logger::addOutput(std::unique_ptr<LoggerOutput> output) {
            outputs.emplace_back(move(output));
        }

        Logger &Logger::info(std::string message) {
            loggerBroadcast(outputs, LogLevel::Info, move(message));
            return *this;
        }

        Logger &Logger::warning(std::string message) {
            loggerBroadcast(outputs, LogLevel::Warning, move(message));
            return *this;
        }

        Logger &Logger::error(std::string message) {
            loggerBroadcast(outputs, LogLevel::Error, move(message));
            return *this;
        }
    }
}
