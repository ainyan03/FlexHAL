#pragma once

#include <chrono>
#include <thread>
#include "flexhal/base/error.hpp"

namespace flexhal::internal::platform::native::utils::time {

  inline error_t delay_ms(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return to_error(status::ok);
  }

  inline error_t delay_us(uint32_t us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
    return to_error(status::ok);
  }

  inline uint32_t millis() {
    return static_cast&lt;uint32_t&gt;(
      std::chrono::duration_cast&lt;std::chrono::milliseconds&gt;(
        std::chrono::steady_clock::now().time_since_epoch()
      ).count()
    );
  }

  inline uint32_t micros() {
    return static_cast&lt;uint32_t&gt;(
      std::chrono::duration_cast&lt;std::chrono::microseconds&gt;(
        std::chrono::steady_clock::now().time_since_epoch()
      ).count()
    );
  }

} // namespace flexhal::internal::platform::native::utils::time
