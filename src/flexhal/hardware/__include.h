#pragma once

#include "__detect.h"

// ハードウェア階層のインクルードファイル
#if FLEXHAL_DETECT_HARDWARE_CHIP
 #include "chip/__include.h"
#endif
