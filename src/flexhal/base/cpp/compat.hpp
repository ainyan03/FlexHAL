#pragma once

namespace flexhal {
namespace base {
namespace cpp {

// C++のバージョンチェック
#if defined(__cplusplus)
  #if __cplusplus >= 201703L
    #define FLEXHAL_INTERNAL_CPP17
  #endif
  #if __cplusplus >= 202002L
    #define FLEXHAL_INTERNAL_CPP20
  #endif
#endif

// [[nodiscard]] の互換性対応
#ifdef FLEXHAL_INTERNAL_CPP17
  #define FLEXHAL_INTERNAL_NODISCARD [[nodiscard]]
#else
  #define FLEXHAL_INTERNAL_NODISCARD
#endif

// C++17 以降の機能を使用するかどうかのフラグ
#ifdef FLEXHAL_INTERNAL_CPP17
  #define FLEXHAL_INTERNAL_USE_CPP17_FEATURES
#endif


} // namespace cpp
} // namespace base
} // namespace flexhal

