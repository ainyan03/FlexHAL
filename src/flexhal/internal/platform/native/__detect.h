#pragma once

#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE
// macOS 環境かどうかの判定例
 #if defined(__MACH__)
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE 1
// Windows 環境かどうかの判定例
 #elif defined(_WIN32)
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE 1
// Linux 環境かどうかの判定例
 #elif defined(__linux)
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE 1
 #endif
#endif
 
#ifndef FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE 
 #define FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE 0
#endif
