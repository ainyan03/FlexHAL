
#include <FlexHAL.h>
#include <gtest/gtest.h>

namespace flexhal_test {

// delay_msのテスト実装
inline bool test_delay_ms(uint32_t delay_time = 100, double tolerance = 0.1) {
  auto start = flexhal::utils::time::millis();
  auto result = flexhal::utils::time::delay_ms(delay_time);
  auto elapsed = flexhal::utils::time::millis() - start;

  // 遅延時間が期待範囲内かチェック
  return (result == to_error(flexhal::base::status::ok)) &&
         (elapsed >= delay_time) &&
         (elapsed <= delay_time * (1.0 + tolerance));
}

// delay_usのテスト実装
inline bool test_delay_us(uint32_t delay_time = 1000) {
  auto start = flexhal::utils::time::micros();
  auto result = flexhal::utils::time::delay_us(delay_time);
  auto elapsed = flexhal::utils::time::micros() - start;

  // マイクロ秒レベルでは環境による誤差が大きいので、
  // おおよその範囲でチェック（0.5倍～1.5倍）
  return (result == to_error(flexhal::base::status::ok)) &&
         (elapsed >= delay_time) &&
         (elapsed <= delay_time * 1.5);
}

// millis()の進行テスト実装
inline bool test_millis_progression(uint32_t delay_time = 50) {
  auto start = flexhal::utils::time::millis();
  flexhal::utils::time::delay_ms(delay_time);
  auto end = flexhal::utils::time::millis();

  return end > start;
}

// micros()の進行テスト実装
inline bool test_micros_progression(uint32_t delay_time = 1000) {
  auto start = flexhal::utils::time::micros();
  flexhal::utils::time::delay_us(delay_time);
  auto end = flexhal::utils::time::micros();

  return end > start;
}

// millis()とmicros()の一貫性テスト実装
inline bool test_millis_micros_consistency(uint32_t delay_time = 500, double tolerance = 0.2) {
  auto start_millis = flexhal::utils::time::millis();
  auto start_micros = flexhal::utils::time::micros();
  flexhal::utils::time::delay_ms(delay_time);
  auto end_millis = flexhal::utils::time::millis();
  auto end_micros = flexhal::utils::time::micros();

  // 経過時間を浮動小数点で計算
  double elapsed_millis = static_cast<double>(end_millis - start_millis);
  double elapsed_micros = static_cast<double>(end_micros - start_micros) / 1000.0;

  // 誤差の絶対値を計算し、許容範囲内かチェック
  double diff = std::abs(elapsed_micros - elapsed_millis);
  return diff <= (elapsed_millis * tolerance);
}

} // namespace flexhal_test

TEST(TimeTest, DelayMs) {
  EXPECT_TRUE(flexhal_test::test_delay_ms());
}

TEST(TimeTest, DelayUs) {
  EXPECT_TRUE(flexhal_test::test_delay_us());
}

TEST(TimeTest, MillisProgression) {
  EXPECT_TRUE(flexhal_test::test_millis_progression());
}

TEST(TimeTest, MicrosProgression) {
  EXPECT_TRUE(flexhal_test::test_micros_progression());
}

TEST(TimeTest, MillisMicrosConsistency) {
  EXPECT_TRUE(flexhal_test::test_millis_micros_consistency());
}
