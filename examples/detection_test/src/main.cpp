#ifdef ARDUINO
#include <Arduino.h>
#else // --- Native Environment ---
#include <stdio.h> // For printf (might still be needed if logger fails?)
#include <unistd.h> // For sleep
#include <cstdio> // For printf in this example
#include <cstdlib> // For EXIT_SUCCESS

// Include LogProxy first, as it defines the class and includes ILogger.hpp (which declares Log)
#include "flexhal/utils/logger/LogProxy.hpp"     // Include LogProxy (Log instance and definition)
// #include "flexhal/utils/logger/ILogger.hpp"      // No longer needed directly, included by LogProxy.hpp
#include "flexhal/platform/native/logger/PrintfLogger.hpp" // Include the native logger implementation

// Define a tag for our logs
#define LOG_TAG "DetectionTest"

// Create a static logger instance for the native environment
static flexhal::platform::native::logger::PrintfLogger native_logger;

#endif // ARDUINO

#include <FlexHAL.h> // Include FlexHAL core *after* potential platform specifics

// Detect Arduino environment (PlatformIO usually defines this)
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #define FLEXHAL_DETECT_FRAMEWORK_ARDUINO
#else
  #define FLEXHAL_DETECT_PLATFORM_NATIVE // Assume native if not Arduino
  // Note: Framework and RTOS detection would be more complex in reality
  #define FLEXHAL_DETECT_FRAMEWORK_NONE
  #define FLEXHAL_DETECT_RTOS_NONE
#endif

void setup() {
#ifndef ARDUINO // In Arduino, Serial setup is often done, but we initialize logging here
  std::printf("---- FlexHAL Detection Test (Native) ----\n");
  // Set the global logger and level
  flexhal::utils::logger::setLogger(&native_logger);
  flexhal::utils::logger::setLogLevel(flexhal::utils::logger::LogLevel::VERBOSE); // Set desired log level
  flexhal::utils::logger::Log.info(LOG_TAG, "Logger initialized. Starting setup...");
#else
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect (needed for some boards)
  Serial.println("---- FlexHAL Detection Test (Arduino) ----");
  flexhal::utils::Log.debug("FlexHAL", "Serial Initialized."); // Example log
#endif

  flexhal::utils::logger::Log.info(LOG_TAG, "Performing environment detection checks...");

  // Example Usage of Detection Macros
#ifdef FLEXHAL_DETECT_PLATFORM_NATIVE
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: Native detected.");
#elif defined(FLEXHAL_DETECT_PLATFORM_ESP32)
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: ESP32 detected.");
#elif defined(FLEXHAL_DETECT_PLATFORM_RP2040)
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: RP2040 detected.");
#else
  flexhal::utils::logger::Log.warn(LOG_TAG, "Platform: Unknown or not detected!");
#endif

#ifdef FLEXHAL_DETECT_FRAMEWORK_ARDUINO
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: Arduino detected.");
#elif defined(FLEXHAL_DETECT_FRAMEWORK_ESP_IDF)
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: ESP-IDF detected.");
#elif defined(FLEXHAL_DETECT_FRAMEWORK_NONE)
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: None or Generic detected.");
#else
  flexhal::utils::logger::Log.warn(LOG_TAG, "Framework: Unknown or not detected!");
#endif

#ifdef FLEXHAL_DETECT_RTOS_FREERTOS
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: FreeRTOS detected.");
#elif defined(FLEXHAL_DETECT_RTOS_ZEPHYR)
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: Zephyr detected.");
#else
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: None or Generic detected.");
#endif

  flexhal::utils::logger::Log.info(LOG_TAG, "Detection checks complete.");
  flexhal::utils::logger::Log.debug(LOG_TAG, "Setup finished."); // Example debug message
  std::printf("----------------------------------------\n");
}

void loop() {
#ifndef ARDUINO
    flexhal::utils::logger::Log.verbose(LOG_TAG, "Loop iteration started."); // Example verbose message
    flexhal::utils::logger::Log.error(LOG_TAG, "Simulating an error condition!"); // Example error message
    std::printf("Native loop: Sleeping for 1 second...\n");
    sleep(1); // Sleep for 1 second (Unix-like systems)
    flexhal::utils::logger::Log.info(LOG_TAG, "Loop iteration finished.");
#else
    Serial.println("Arduino loop: Delaying for 1 second...");
    delay(1000);
#endif
}

#ifndef ARDUINO // Non-Arduino platforms need a main function
int main() {
  setup();
  // Loop 10 times then exit
  for (int i = 0; i < 10; ++i) {
    loop();
  }
  flexhal::utils::logger::Log.info(LOG_TAG, "Native program finished."); // Log the finish
  return 0;
}
#endif
