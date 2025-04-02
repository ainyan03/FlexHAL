#pragma once

#include <cstdarg> // for va_list

// Forward declare LogProxy to allow declaring the extern Log instance.
namespace flexhal { namespace utils { namespace logger { class LogProxy; } } }

namespace flexhal {
namespace utils {
namespace logger {

/**
 * @brief Defines the logging levels.
 */
enum class LogLevel {
    NONE,    ///< No logging output
    ERROR,   ///< Only errors
    WARN,    ///< Errors and warnings
    INFO,    ///< Errors, warnings, and info
    DEBUG,   ///< Errors, warnings, info, and debug
    VERBOSE  ///< All log levels
};

/**
 * @brief Interface for logger implementations.
 *
 * Defines the common operations for logging messages.
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
    // or by a global mechanism if needed.
};

// --- Global Logger Management Declarations ---

// Pointer to the currently active logger instance (defined in .inl)
extern ILogger* _active_logger;

// Default log level (defined in .inl)
extern LogLevel _default_log_level;

/**
 * @brief Sets the global logger instance.
 * @param logger Pointer to the ILogger implementation to use.
 */
void setLogger(ILogger* logger); // Definition in .inl

/**
 * @brief Sets the global minimum log level.
 * @param level The minimum log level.
 */
void setLogLevel(LogLevel level); // Definition in .inl

/**
 * @brief Gets the current global minimum log level.
 * @return The current log level.
 */
LogLevel getLogLevel(); // Definition in .inl

// --- Global Log Proxy Instance Declaration ---

// Global instance of the log proxy (defined in .inl)
// Use this like: flexhal::utils::logger::Log.info("TAG", "Message %d", value);
extern LogProxy Log;

// Include the LogProxy class definition *after* declaring the extern Log instance
#include "LogProxy.hpp"


} // namespace logger
} // namespace utils
} // namespace flexhal
