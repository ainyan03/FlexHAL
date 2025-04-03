#pragma once

#include "ILogger.hpp" // For LogLevel enum and ILogger interface
#include <cstdarg>     // For std::va_list in LogProxy methods if needed indirectly

// Forward declare LogProxy used by the global Log instance
namespace flexhal { namespace utils { namespace logger { class LogProxy; } } }

namespace flexhal {
namespace utils {
namespace logger {

// --- Global Logger Management Declarations ---

// Pointer to the currently active logger instance
extern ILogger* _active_logger;

// Default log level
extern LogLevel _default_log_level;

/**
 * @brief Sets the global logger instance.
 * @param logger Pointer to the ILogger implementation to use.
 */
void setLogger(ILogger* logger);

/**
 * @brief Sets the global minimum log level.
 * @param level The minimum log level.
 */
void setLogLevel(LogLevel level);

/**
 * @brief Gets the current global minimum log level.
 * @return The current minimum log level.
 */
LogLevel getLogLevel();

// --- Global Log Proxy Instance Declaration ---

// Global instance of the log proxy
// Use this like: flexhal::utils::logger::Log.info("TAG", "Message %d", value);
extern LogProxy Log;

// Implementation details are included below if FLEXHAL_INTERNAL_IMPLEMENTATION is defined

} // namespace logger
} // namespace utils
} // namespace flexhal

// --- Implementation (definitions) ---
// This section is included only once in the entire project (e.g., in FlexHAL.cpp)
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

#include "LogProxy.hpp" // Include LogProxy definition only when implementing

namespace flexhal { namespace utils { namespace logger {

// --- Global Logger Variable Definitions ---
ILogger* _active_logger = nullptr; // Initialize to null, must be set via setLogger
LogLevel _default_log_level = LogLevel::INFO; // Default to INFO level if not changed

// --- Global Log Proxy Instance Definition ---
LogProxy Log; // Define the global Log instance used for Log.info(), etc.

// --- Global Logger Function Definitions ---
void setLogger(ILogger* logger) {
    _active_logger = logger;
}

void setLogLevel(LogLevel level) {
    _default_log_level = level;
}

LogLevel getLogLevel() {
    return _default_log_level;
}

} } } // namespace flexhal::utils::logger

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
