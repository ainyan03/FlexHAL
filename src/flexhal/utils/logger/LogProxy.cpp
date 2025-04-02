#include "LogProxy.hpp"
#include "ILogger.hpp" // Include ILogger for LogLevel definition and potentially others

namespace flexhal {
namespace utils {
namespace logger {

// Define the global logger pointer (initially null)
ILogger* _active_logger = nullptr;

// Define the default log level (e.g., INFO)
LogLevel _default_log_level = LogLevel::INFO;

// Define the global LogProxy instance
LogProxy Log; // This calls the default constructor of LogProxy

// --- Function Definitions ---

void setLogger(ILogger* logger) {
    _active_logger = logger;
}

void setLogLevel(LogLevel level) {
    _default_log_level = level;
}

// LogProxy methods like log_internal are typically defined inline in the header
// or could be moved here if they were more complex and not performance-critical.
// For now, we assume they remain in LogProxy.hpp.

} // namespace logger
} // namespace utils
} // namespace flexhal
