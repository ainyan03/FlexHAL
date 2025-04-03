#pragma once

// ルートレベルのインクルードファイル

// #include "__detect.h" // これはplatform階層内に必要になる。この階層では不要

// Common utilities (always included)
#include "utils/__include.h"

#include "platform/__include.h"  // Platform階層のインクルードファイル

// 以下は不要
// Platform-specific includes (conditionally included based on detection)
#if FLEXHAL_DETECT_HARDWARE
 #include "platform/hardware/__include.h"
#endif

#if FLEXHAL_DETECT_FRAMEWORK
 #include "platform/framework/__include.h"
#endif
