#pragma once

// ルートレベルのインクルードファイル

// Include base definitions and utilities
#if __has_include("base/__include.h")
 #include "base/__include.h"
#endif

// Include platform-specific implementations (hardware, framework, etc.)
#if __has_include("internal/__include.h")
 #include "internal/__include.h"
#endif

// Include common utilities
#if __has_include("utils/__include.h")
 #include "utils/__include.h"
#endif

// Include fallback implementations (always available)
#if __has_include("fallback/__include.h")
 #include "fallback/__include.h"
#endif
