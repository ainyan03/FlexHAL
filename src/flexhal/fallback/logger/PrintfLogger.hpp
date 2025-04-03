#pragma once

#include "../../utils/logger/ILogger.hpp"
#include <stdio.h> // For printf
#include <stdarg.h> // For va_list

namespace flexhal {
namespace fallback {
namespace logger {

class PrintfLogger : public flexhal::utils::logger::ILogger {
public:
    // Match the ILogger interface for the va_list version
    void log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, va_list args) override;

    // Provide a convenience overload using variadic arguments (not virtual in base)
    void log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, ...);

};

// Function to print a simple string (platform-agnostic fallback)
void print_string(const char* str);

} // namespace logger
} // namespace fallback
} // namespace flexhal

// --- Implementation ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

namespace flexhal {
namespace fallback {
namespace logger {

// Definition for the va_list version (matches declaration)
void PrintfLogger::log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, va_list args) {
    // Simple implementation using printf, ignoring log level and tag for now
    // TODO: Add log level prefix (e.g., "[INFO] ", "[WARN] ")
    // TODO: Consider adding timestamp?
    // TODO: Optionally use the tag?
    // Example: printf("[%s] ", tag); // If you want to print the tag
    vprintf(format, args);
    printf("\n"); // Ensure newline
}

// Definition for the variadic arguments version (matches declaration)
void PrintfLogger::log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    // Call the va_list version, passing the tag (even if unused here)
    log(level, tag, format, args); // Calls the overridden virtual function
    va_end(args);
}


void print_string(const char* str) {
    printf("%s", str);
}

} // namespace logger
} // namespace fallback
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
