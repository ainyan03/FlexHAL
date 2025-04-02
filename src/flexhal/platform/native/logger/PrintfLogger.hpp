#pragma once

#include <cstdarg> // For va_list
#include "flexhal/utils/logger/ILogger.hpp"

namespace flexhal {
namespace platform {
namespace native {
namespace logger {

/**
 * @brief Logger implementation that uses standard printf.
 *
 * This logger directs output to the standard output stream (usually the console)
 * using printf-style formatting.
 */
class PrintfLogger final : public flexhal::utils::logger::ILogger {
public:
    /**
     * @brief Default constructor.
     */
    PrintfLogger();

    /**
     * @brief Default virtual destructor (overrides base class).
     */
    virtual ~PrintfLogger() override = default; // Keep it simple, noexcept should be inherited

    /**
     * @brief Logs a message with the specified level, tag, format, and arguments.
     *
     * @param level The log level (e.g., INFO, WARN, ERROR).
     * @param tag A tag string (often the module name).
     * @param format The printf-style format string.
     * @param args The variable arguments list matching the format string.
     */
    void log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, std::va_list args) override;

private:
    // Prevent copying and assignment
    PrintfLogger(const PrintfLogger&) = delete;
    PrintfLogger& operator=(const PrintfLogger&) = delete;
};

} // namespace logger
} // namespace native
} // namespace platform
} // namespace flexhal
