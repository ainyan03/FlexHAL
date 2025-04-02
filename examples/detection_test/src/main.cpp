#include <Arduino.h>
#include <FlexHAL.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("FlexHAL 環境検出テスト");
  Serial.println("=====================");
  
  // ハードウェア検出結果
  Serial.println("ハードウェア検出結果:");
  Serial.print("FLEXHAL_DETECT_HARDWARE: ");
  Serial.println(FLEXHAL_DETECT_HARDWARE);
  
  Serial.print("FLEXHAL_DETECT_HARDWARE_CHIP: ");
  Serial.println(FLEXHAL_DETECT_HARDWARE_CHIP);
  
  Serial.print("FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF: ");
  Serial.println(FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF);
  
  Serial.print("FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF_ESP32: ");
  Serial.println(FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF_ESP32);
  
  Serial.print("FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3: ");
  Serial.println(FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3);
  
  // フレームワーク検出結果
  Serial.println("\nフレームワーク検出結果:");
  Serial.print("FLEXHAL_DETECT_FRAMEWORK: ");
  Serial.println(FLEXHAL_DETECT_FRAMEWORK);
  
  Serial.print("FLEXHAL_DETECT_FRAMEWORK_ARDUINO: ");
  Serial.println(FLEXHAL_DETECT_FRAMEWORK_ARDUINO);
}

void loop() {
  delay(1000);
}
