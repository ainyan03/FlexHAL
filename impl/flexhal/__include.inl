#pragma once

// 実装ファイルのルートレベルインクルード

// 各階層の実装ファイルをインクルード
#if FLEXHAL_DETECT_HARDWARE
 #include "hardware/__include.inl"
#endif
#include "utils/__include.inl"

#if FLEXHAL_DETECT_FRAMEWORK
 #include "framework/__include.inl"
#endif
