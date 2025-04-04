#include <gtest/gtest.h>

__attribute__((weak))
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
  
#if defined (ARDUINO)
#include <Arduino.h>

void setup() {
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}
void loop() {
    delay(1000);
}

#endif
