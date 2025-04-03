#include "tests/test_utils.hpp"
#include "flexhal/base/error.hpp"

using namespace flexhal::base;

// 基本的なエラーコードのテスト
TEST_CASE(error_basic) {
  // 正常値のテスト
  TEST_ASSERT(is_ok(to_error(status::ok)));
  TEST_ASSERT(!is_error(to_error(status::ok)));
  TEST_ASSERT(is_ok(to_error(status::pending)));
  TEST_ASSERT(is_ok(to_error(status::done)));

  // エラー値のテスト
  TEST_ASSERT(is_error(to_error(status::error)));
  TEST_ASSERT(!is_ok(to_error(status::error)));
  TEST_ASSERT(is_error(to_error(status::timeout)));
  TEST_ASSERT(is_error(to_error(status::busy)));

  return {"error_basic", true, "", __FILE__, __LINE__};
}

// 境界値のテスト
TEST_CASE(error_boundary) {
  // 0は正常
  TEST_ASSERT(is_ok(0));
  TEST_ASSERT(!is_error(0));

  // -1はエラー
  TEST_ASSERT(is_error(-1));
  TEST_ASSERT(!is_ok(-1));

  return {"error_boundary", true, "", __FILE__, __LINE__};
}

// 型変換のテスト
TEST_CASE(error_conversion) {
  // statusからerror_tへの変換
  error_t err = to_error(status::timeout);
  TEST_ASSERT(err == to_error(status::timeout));
  TEST_ASSERT(err < 0);

  return {"error_conversion", true, "", __FILE__, __LINE__};
}

// メインエントリーポイント
int main() {
  RUN_TEST(error_basic);
  RUN_TEST(error_boundary);
  RUN_TEST(error_conversion);
  return flexhal::test::print_test_summary() ? 0 : 1;
}
