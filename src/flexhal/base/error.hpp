#pragma once

#include <stdint.h>    // for int16_t
#include <limits>     // numeric_limits は C++ 固有機能なので残す

#include "cpp/compat.hpp"

namespace flexhal {
namespace base {

// エラーコードの基本型定義
using error_t = int16_t;

// エラーコードの構造:
// int16_t の符号ビットを利用して、負の値はエラー、0以上は正常を表す
enum class status : int16_t {
  // 正常系 (0以上)
  ok          = 0,      // 成功
  pending     = 1,      // 処理中
  done        = 2,      // 完了（追加情報あり）
  // 3-32767 は将来の拡張用に予約

  // エラー系 (負の値)
  error       = -1,     // 一般エラー
  timeout     = -2,     // タイムアウト
  busy        = -3,     // ビジー
  param       = -4,     // パラメータエラー
  not_found   = -5,     // 見つからない
  no_memory   = -6,     // メモリ不足
  io          = -7,     // I/Oエラー
  perm        = -8,     // 権限エラー
  unsupported = -9,     // 未サポート
  // -10 から -32768 は将来の拡張用に予約
};

// キャストヘルパー
constexpr error_t to_error(status s) {
  return static_cast<error_t>(s);
}

// エラー判定関数
FLEXHAL_INTERNAL_NODISCARD constexpr bool is_error(error_t err) {
  return err < 0;
}

FLEXHAL_INTERNAL_NODISCARD constexpr bool is_ok(error_t err) {
  return err >= 0;
}

// 型チェック
static_assert(sizeof(error_t) == 2, "error_t must be 16-bit");
static_assert(std::numeric_limits<error_t>::is_signed, "error_t must be signed");

} // namespace base
} // namespace flexhal
