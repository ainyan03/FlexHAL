#pragma once

#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO
// Arduino 環境かどうかの判定例
 #if defined(ARDUINO)
  #define FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO 1
 #endif
#endif
 
#ifndef FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO 
 #define FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO 0
#endif
