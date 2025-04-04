#pragma once

#include "flexhal/base/error.hpp"

#if 0

#if defined(ARDUINO)
  #include "flexhal/internal/framework/arduino/utils/time.hpp"
  namespace flexhal::utils::time {
    using namespace internal::framework::arduino::utils::time;
  }
#elif defined(FLEXHAL_NATIVE)
  #include "flexhal/internal/platform/native/utils/time.hpp"
  namespace flexhal::utils::time {
    using namespace internal::platform::native::utils::time;
  }
#else
  #error "No time implementation available for this platform"
#endif

#endif
