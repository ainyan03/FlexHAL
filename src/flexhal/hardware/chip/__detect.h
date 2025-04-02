#pragma once

#include "../__detect.h"

// チップ階層の検出マクロ
#ifndef FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF
 #if defined(ESP_PLATFORM) || defined(ESP32) || defined(ESP8266)
  #define FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF 1
 #else
  #define FLEXHAL_DETECT_HARDWARE_CHIP_ESPRESSIF 0
 #endif
#endif
