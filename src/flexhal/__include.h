#pragma once

// ルートレベルのインクルードファイル
#include "__detect.h"

// 各階層のインクルード
#if FLEXHAL_DETECT_HARDWARE
 #include "hardware/__include.h"
#endif

#if FLEXHAL_DETECT_FRAMEWORK
 #include "framework/__include.h"
#endif
