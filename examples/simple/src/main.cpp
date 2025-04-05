#include <FlexHAL.h> // Just include the header

// テスト用の関数
flexhal::base::status test_error() {
  return flexhal::base::status::ok;
}

// Empty setup function (required by Arduino framework)
void setup() {
}

// Empty loop function (required by Arduino framework)
void loop() {
}

#ifndef ARDUINO // Provide main() for native execution to link successfully
__attribute__((weak))
int main() {
  setup();
  for (;;) loop();
  return 0;
}
#endif
