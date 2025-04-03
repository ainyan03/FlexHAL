#pragma once

#include <cstdarg> // For std::va_list

// Forward declare LogProxy - needed even if globals are moved, 
// because LogProxy might use LogLevel or ILogger concepts internally.
// Although, if LogProxy ONLY needs LogLevel/ILogger, including this header is enough.
// Let's keep it clean for now.
// namespace flexhal { namespace utils { namespace logger { class LogProxy; } } } // No longer needed here

namespace flexhal {
namespace utils {
namespace logger {

// --- Log Level Enum ---
/**
 * @brief Defines the logging levels.
 */
enum class LogLevel {
    NONE = 0, ///< No logging
    ERROR,    ///< Critical errors
    WARN,     ///< Warnings
    INFO,     ///< Informational messages
    DEBUG,    ///< Debug messages
    VERBOSE   ///< Verbose debug messages
};

// --- Logger Interface ---
/**
 * @brief Interface for logger implementations.
 *
 * Concrete logger implementations (e.g., SerialLogger, PrintfLogger)
 * should inherit from this class and implement the log method.
 */
class ILogger {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ILogger() = default;

    /**
     * @brief Logs a message with a specific level and tag using variadic arguments.
     *
     * @param level The log level of the message.
     * @param tag A tag string to categorize the log message (e.g., component name).
     * @param format The printf-style format string.
     * @param args The variable argument list.
     */
    virtual void log(LogLevel level, const char* tag, const char* format, std::va_list args) = 0;

    // Note: Level management (setLevel/getLevel) is intentionally omitted here.
    // It's assumed to be handled either by the specific implementation
    // or by a global mechanism (now in logger_globals.hpp).
};

// --- Global definitions and declarations moved to logger_globals.hpp ---

// Include LogProxy definition here? No, LogProxy should include ILogger.hpp if needed.
// #include "LogProxy.hpp"

} // namespace logger
} // namespace utils
} // namespace flexhal
