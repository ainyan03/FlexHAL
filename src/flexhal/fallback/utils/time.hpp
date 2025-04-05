#pragma once

#include <chrono>
#include <thread>
#include "flexhal/base/status.hpp"

#ifndef FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME
#define FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME flexhal::fallback::utils::time
#endif

// namespace flexhal::internal::platform::native::utils::time {
namespace flexhal {
namespace fallback {
namespace utils {
namespace time {

  inline base::status delay_ms(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return base::status::ok;
  }

  inline base::status delay_us(uint32_t us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
    return base::status::ok;
  }

  inline uint32_t millis() {
    return static_cast<uint32_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
      ).count()
    );
  }

  inline uint32_t micros() {
    return static_cast<uint32_t>(
      std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
      ).count()
    );
  }
} // namespace time
} // namespace utils
} // namespace fallback
} // namespace flexhal
