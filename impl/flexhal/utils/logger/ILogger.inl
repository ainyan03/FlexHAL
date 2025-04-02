#pragma once

// Provides definitions for global logger variables and functions declared in ILogger.hpp

#include "flexhal/utils/logger/ILogger.hpp" // Include the header with declarations
#include "flexhal/utils/logger/LogProxy.hpp" // Include LogProxy definition for the Log instance

namespace flexhal {
namespace utils {
namespace logger {

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

} // namespace logger
} // namespace utils
} // namespace flexhal
