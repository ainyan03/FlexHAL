#include <FlexHAL.h> // Just include the header

// テスト用の関数
flexhal::base::error_t test_error() {
  return flexhal::base::to_error(flexhal::base::status::ok);
}

// Empty setup function (required by Arduino framework)
void setup() {
  // エラーコードのテスト
  auto result = test_error();
  if (flexhal::base::is_ok(result)) {
    // OK
  }
}

// Empty loop function (required by Arduino framework)
void loop() {
  // Nothing needed here for a minimal build test
}

#ifndef ARDUINO // Provide main() for native execution to link successfully
int main() {
  // Minimal main for native build test
  return 0;
}
#endif
