#pragma once

#include "flexhal/base/status.hpp"

#ifndef FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME
#include "../../../FlexHAL.hpp"
#endif

#ifdef FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME

namespace flexhal {
  namespace utils {
    namespace time {
      using namespace FLEXHAL_INTERNAL_FLEXHAL_UTILS_TIME;
    }
  }
} 

#else
  #error "utils::time implementation not available for this platform"
#endif
