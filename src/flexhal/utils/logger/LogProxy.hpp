#pragma once

#include "../logger.hpp" // Needs LogLevel, ILogger*, _active_logger, _default_log_level
#include "ILogger.hpp"

#include <cstdarg>    // For va_start, va_end

namespace flexhal {
namespace utils {
namespace logger {

/**
 * @brief Helper class to provide a convenient logging interface (Log.info(...)).
 *
 * This class uses the globally set logger (_active_logger) and log level (_default_log_level).
 * Access it via the global `Log` instance (declared in ILogger.hpp).
 */
class LogProxy {
private:
    // Helper function to check level and call logger
    // This avoids repeating the check in every public method
    static void log_internal(LogLevel level, const char* tag, const char* format, va_list args) {
        if (_active_logger && level != LogLevel::NONE && level <= _default_log_level) {
             _active_logger->log(level, tag, format, args);
        }
    }

public:
    // Removed the templated log function as va_start doesn't work easily with templates this way.
    // Instead, each public function will handle va_start/va_end.

    void error(const char* tag, const char* format, ...) {
        va_list args_list;
        va_start(args_list, format);
        log_internal(LogLevel::ERROR, tag, format, args_list);
        va_end(args_list);
    }

    void warn(const char* tag, const char* format, ...) {
        va_list args_list;
        va_start(args_list, format);
        log_internal(LogLevel::WARN, tag, format, args_list);
        va_end(args_list);
    }

    void info(const char* tag, const char* format, ...) {
        va_list args_list;
        va_start(args_list, format);
        log_internal(LogLevel::INFO, tag, format, args_list);
        va_end(args_list);
    }

    void debug(const char* tag, const char* format, ...) {
        va_list args_list;
        va_start(args_list, format);
        log_internal(LogLevel::DEBUG, tag, format, args_list);
        va_end(args_list);
    }

    void verbose(const char* tag, const char* format, ...) {
        va_list args_list;
        va_start(args_list, format);
        log_internal(LogLevel::VERBOSE, tag, format, args_list);
        va_end(args_list);
    }
};

} // namespace logger
} // namespace utils
} // namespace flexhal
