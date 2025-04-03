#include <FlexHAL.h> // Just include the header

// Empty setup function (required by Arduino framework)
void setup() {
  // Nothing needed here for a minimal build test
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
