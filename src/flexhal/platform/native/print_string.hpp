#ifndef FLEXHAL_PLATFORM_NATIVE_PRINT_STRING_HPP
#define FLEXHAL_PLATFORM_NATIVE_PRINT_STRING_HPP

#include <cstdarg> // For va_list

namespace flexhal {
namespace platform {
namespace native {

/**
 * @brief Native platform implementation for printing a formatted string.
 * 
 * This function uses the standard C library's vprintf to output
 * the formatted string to the standard output.
 * 
 * @param format The format string (printf style).
 * @param args The variable argument list.
 */
void print_string(const char* format, std::va_list args);

} // namespace native
} // namespace platform
} // namespace flexhal

#endif // FLEXHAL_PLATFORM_NATIVE_PRINT_STRING_HPP
