#pragma once

#include <cstdarg> // for va_list
#include "flexhal/utils/logger/ILogger.hpp" // Include the ILogger interface

namespace flexhal {
namespace platform {
namespace native {
namespace logger {

/**
 * @brief Logger implementation for native platforms using printf.
 *
 * Outputs log messages to standard output.
 */
class PrintfLogger final : public flexhal::utils::logger::ILogger {
public:
    /**
     * @brief Default constructor.
     */
    PrintfLogger() = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~PrintfLogger() noexcept override = default;

    /**
     * @brief Logs a message using printf.
     *
     * @param level The log level.
     * @param tag The log tag.
     * @param format The format string.
     * @param args The variable arguments.
     */
    void log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, std::va_list args) override;

    // PrintfLogger specific methods (if any) can go here
    // For example, maybe setting a global level for this specific logger?
    // void setLevel(flexhal::utils::logger::LogLevel level) { /* ... */ }
    // flexhal::utils::logger::LogLevel getLevel() const { /* ... */ }

    // Delete copy constructor and assignment operator
    PrintfLogger(const PrintfLogger&) = delete;
    PrintfLogger& operator=(const PrintfLogger&) = delete;

private:
    // Member variables specific to PrintfLogger (if any)
    // For example, its own log level threshold
    // flexhal::utils::logger::LogLevel _level = flexhal::utils::logger::LogLevel::INFO;
};

} // namespace logger
} // namespace native
} // namespace platform
} // namespace flexhal
