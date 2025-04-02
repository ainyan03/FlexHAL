#include "PrintfLogger.hpp"
#include <cstdio>  // For vprintf, fflush, printf, snprintf, vsnprintf
#include <cstdarg> // For va_list

namespace flexhal {
namespace platform {
namespace native {
namespace logger {

// Helper function to get log level prefix string
const char* getLogLevelPrefix(flexhal::utils::logger::LogLevel level) {
    // Cast LogLevel to int for switch compatibility
    switch (static_cast<int>(level)) {
        case static_cast<int>(flexhal::utils::logger::LogLevel::ERROR):   return "[E]";
        case static_cast<int>(flexhal::utils::logger::LogLevel::WARN):    return "[W]";
        case static_cast<int>(flexhal::utils::logger::LogLevel::INFO):    return "[I]";
        case static_cast<int>(flexhal::utils::logger::LogLevel::DEBUG):   return "[D]";
        case static_cast<int>(flexhal::utils::logger::LogLevel::VERBOSE): return "[V]";
        // case LogLevel::NONE: should not ideally reach here
        default:                                                          return "[?]"; // Or empty string for unknown/NONE
    }
}

PrintfLogger::PrintfLogger() {
    // Constructor implementation (if needed in the future)
    // Currently, nothing specific is required for initialization.
}

void PrintfLogger::log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, std::va_list args) {
    // Note: Global level filtering should happen before calling this log function
    // (e.g., in the LogProxy or global log function).
    // PrintfLogger itself doesn't filter by level here, it just formats and prints.

    // Format the message with level and tag prefix
    // Using snprintf and vsnprintf for safer buffer handling.
    char buffer[256]; // Be mindful of potential buffer overflows with long messages/tags
    int prefix_len = 0;

    // Add log level prefix and tag
    prefix_len = std::snprintf(buffer, sizeof(buffer), "%s [%s] ", getLogLevelPrefix(level), tag ? tag : ""); // Handle null tag

    // Check for snprintf errors or truncation
    if (prefix_len < 0 || static_cast<size_t>(prefix_len) >= sizeof(buffer)) {
        // If prefix alone failed or filled buffer, fallback or log error?
        // Fallback: Just print the raw message without prefix
        std::vprintf(format, args);
        std::printf("\n"); // Add newline as vprintf doesn't
        std::fflush(stdout);
        return; // Stop processing this message
    }

    // Append the actual log message using vsnprintf
    // Pass the remaining buffer size and the pointer to the end of the prefix
    std::vsnprintf(buffer + prefix_len, sizeof(buffer) - prefix_len, format, args);
    // Ensure null termination even if vsnprintf truncated the message
    buffer[sizeof(buffer) - 1] = '\0';

    // Print the complete formatted string to standard output
    std::printf("%s\n", buffer);
    std::fflush(stdout); // Ensure the message is printed immediately for native console
}

} // namespace logger
} // namespace native
} // namespace platform
} // namespace flexhal
