#include "flexhal/utils/time.hpp"
#include <gtest/gtest.h>

namespace flexhal::test::time {

// delay_msのテスト実装
inline bool test_delay_ms(uint32_t delay_time = 100, double tolerance = 0.1) {
  auto start = flexhal::utils::time::millis();
  auto result = flexhal::utils::time::delay_ms(delay_time);
  auto elapsed = flexhal::utils::time::millis() - start;

  // 遅延時間が期待範囲内かチェック
  return (result == to_error(base::status::ok)) &&
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
  return (result == to_error(base::status::ok)) &&
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
inline bool test_millis_micros_consistency() {
  auto start_millis = flexhal::utils::time::millis();
  auto start_micros = flexhal::utils::time::micros();
  flexhal::utils::time::delay_ms(100);
  auto end_millis = flexhal::utils::time::millis();
  auto end_micros = flexhal::utils::time::micros();

  auto elapsed_millis = end_millis - start_millis;
  auto elapsed_micros = end_micros - start_micros;

  // マイクロ秒を1000で割ってミリ秒に変換し、誤差が10%以内かチェック
  auto micros_to_millis = elapsed_micros / 1000;
  return std::abs(static_cast<int64_t>(micros_to_millis - elapsed_millis)) <= (elapsed_millis * 0.1);
}

} // namespace flexhal::test::time

TEST(TimeTest, DelayMs) {
  EXPECT_TRUE(flexhal::test::time::test_delay_ms());
}

TEST(TimeTest, DelayUs) {
  EXPECT_TRUE(flexhal::test::time::test_delay_us());
}

TEST(TimeTest, MillisProgression) {
  EXPECT_TRUE(flexhal::test::time::test_millis_progression());
}

TEST(TimeTest, MicrosProgression) {
  EXPECT_TRUE(flexhal::test::time::test_micros_progression());
}

TEST(TimeTest, MillisMicrosConsistency) {
  EXPECT_TRUE(flexhal::test::time::test_millis_micros_consistency());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
