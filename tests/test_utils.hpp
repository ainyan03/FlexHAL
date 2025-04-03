#pragma once

#include <stdio.h>

namespace flexhal::test {

// テスト結果を保持する構造体
struct TestResult {
  const char* name;      // テスト名
  bool passed;          // 成功/失敗
  const char* message;  // 失敗時のメッセージ
  const char* file;     // ファイル名
  int line;            // 行番号
};

// テスト結果のリスト
inline int test_count = 0;
inline int failed_count = 0;

// テスト結果の出力
inline void print_test_result(const TestResult& result) {
  test_count++;
  if (result.passed) {
    printf("✅ %s: OK\n", result.name);
  } else {
    failed_count++;
    printf("❌ %s: FAILED\n", result.name);
    printf("   %s:%d: %s\n", result.file, result.line, result.message);
  }
}

// テスト結果のサマリー出力
inline bool print_test_summary() {
  printf("\n=== Test Summary ===\n");
  printf("Total: %d, Failed: %d\n", test_count, failed_count);
  printf("==================\n");
  return failed_count == 0;
}

} // namespace flexhal::test

// テストマクロ
#ifdef FLEXHAL_TESTING
  #define TEST_ASSERT(condition) \
    do { \
      if (!(condition)) { \
        return flexhal::test::TestResult{ \
          __func__, \
          false, \
          #condition, \
          __FILE__, \
          __LINE__ \
        }; \
      } \
    } while (0)

  #define TEST_CASE(name) \
    flexhal::test::TestResult test_##name()

  #define RUN_TEST(name) \
    do { \
      auto result = test_##name(); \
      flexhal::test::print_test_result(result); \
    } while (0)

  #define TEST_MAIN() \
    int main() { \
      return flexhal::test::print_test_summary() ? 0 : 1; \
    }
#else
  #define TEST_ASSERT(condition)
  #define TEST_CASE(name) void test_##name()
  #define RUN_TEST(name)
  #define TEST_MAIN()
#endif
