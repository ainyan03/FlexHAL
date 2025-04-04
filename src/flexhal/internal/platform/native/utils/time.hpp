#pragma once

#include <chrono>
#include <thread>
#include "flexhal/base/error.hpp"

namespace flexhal::internal::platform::native::utils::time {

  inline base::error_t delay_ms(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return to_error(base::status::ok);
  }

  inline base::error_t delay_us(uint32_t us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
    return to_error(base::status::ok);
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

} // namespace flexhal::internal::platform::native::utils::time
