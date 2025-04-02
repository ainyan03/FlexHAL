#pragma once

#include "../__detect.h"

// フレームワーク階層の検出マクロ
#ifndef FLEXHAL_DETECT_FRAMEWORK_ARDUINO
 #if defined(ARDUINO)
  #define FLEXHAL_DETECT_FRAMEWORK_ARDUINO 1
 #else
  #define FLEXHAL_DETECT_FRAMEWORK_ARDUINO 0
 #endif
#endif
