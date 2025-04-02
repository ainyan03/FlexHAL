#include "print_string.hpp"
#include <cstdio> // For vprintf

namespace flexhal {
namespace platform {
namespace native {

void print_string(const char* format, std::va_list args) {
    // Use vprintf to print to standard output
    std::vprintf(format, args);
}

} // namespace native
} // namespace platform
} // namespace flexhal
