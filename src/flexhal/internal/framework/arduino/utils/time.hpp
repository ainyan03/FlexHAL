#pragma once

#include <Arduino.h>
#include "flexhal/base/status.hpp"

#ifndef FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME
#define FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME flexhal::internal::framework::arduino::utils::time
#endif

namespace flexhal::internal::framework::arduino::utils::time {

  inline base::error_t delay_ms(uint32_t ms) {
    ::delay(ms);
    return to_error(base::status::ok);
  }

  inline base::error_t delay_us(uint32_t us) {
    ::delayMicroseconds(us);
    return to_error(base::status::ok);
  }

  inline uint32_t millis() {
    return ::millis();
  }

  inline uint32_t micros() {
    return ::micros();
  }

} // namespace flexhal::internal::framework::arduino::utils::time
