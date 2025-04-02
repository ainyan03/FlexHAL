#ifdef ARDUINO
#include <Arduino.h>
#else // --- Native Environment ---
#include <stdio.h> // For printf (might still be needed if logger fails?)
#include <unistd.h> // For sleep
#include <cstdio> // For printf in this example
#include <cstdlib> // For EXIT_SUCCESS
#include "flexhal/utils/logger/ILogger.hpp" // Provides LogLevel, setLogger, Log proxy
#include "flexhal/utils/logger/LogProxy.hpp" // Include LogProxy (Log instance)
#include "flexhal/platform/native/logger/PrintfLogger.hpp" // The logger we will use

// Define a tag for our logs
#define LOG_TAG "DetectionTest"

// Create a static logger instance for the native environment
static flexhal::platform::native::logger::PrintfLogger native_logger;

#endif // ARDUINO

#include <FlexHAL.h> // Include FlexHAL core *after* potential platform specifics

void setup() {
#ifdef ARDUINO
  Serial.begin(115200);
  delay(1000); // Wait for Serial monitor
  // Arduino uses SerialLogger by default (or needs its own setup)
  // For now, we can just use Serial directly or set up SerialLogger later
  Serial.println("FlexHAL Setup (Arduino)");
  flexhal::utils::Log.info("FlexHAL", "Setup complete (Arduino)."); // Example log

#else // --- Native Environment ---
  // --- Logger Initialization ---
  // 1. Create an instance of the logger implementation we want to use.
  //    Making it static ensures it persists after setup() exits.
  static flexhal::platform::native::logger::PrintfLogger my_logger;

  // 2. Set the created logger as the global active logger.
  flexhal::utils::logger::setLogger(&my_logger);

  // 3. Set the desired global log level. Let's see everything for testing.
  flexhal::utils::logger::setLogLevel(flexhal::utils::logger::LogLevel::VERBOSE);

  // --- Now we can use the logger! ---
  flexhal::utils::logger::Log.info(LOG_TAG, "Logger initialized. Starting setup...");

  // --- Original Setup Code (example) ---
  // Use printf for comparison or for parts not using the FlexHAL logger yet
  std::printf("--- FlexHAL Environment Detection Test ---\n");
  flexhal::utils::logger::Log.info(LOG_TAG, "Performing environment detection checks...");

  // Check Hardware Platform
#if FLEXHAL_DETECT_PLATFORM_NATIVE
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: Native detected.");
#elif FLEXHAL_DETECT_PLATFORM_ESP32
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: ESP32 detected.");
#elif FLEXHAL_DETECT_PLATFORM_STM32
  flexhal::utils::logger::Log.info(LOG_TAG, "Platform: STM32 detected.");
#else
  flexhal::utils::logger::Log.warn(LOG_TAG, "Platform: Unknown or not detected!");
#endif

  // Check Framework
#if FLEXHAL_DETECT_FRAMEWORK_ARDUINO
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: Arduino detected.");
#elif FLEXHAL_DETECT_FRAMEWORK_ESP_IDF
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: ESP-IDF detected.");
#elif FLEXHAL_DETECT_FRAMEWORK_STM32CUBE
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: STM32Cube detected.");
#else
  flexhal::utils::logger::Log.info(LOG_TAG, "Framework: None or Generic detected.");
#endif

  // Check RTOS
#if FLEXHAL_DETECT_RTOS_FREERTOS
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: FreeRTOS detected.");
#elif FLEXHAL_DETECT_RTOS_ZEPHYR
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: Zephyr detected.");
#else
  flexhal::utils::logger::Log.info(LOG_TAG, "RTOS: None or Generic detected.");
#endif

  flexhal::utils::logger::Log.info(LOG_TAG, "Detection checks complete.");
  flexhal::utils::logger::Log.debug(LOG_TAG, "Setup finished."); // Example debug message
  std::printf("----------------------------------------\n");
#endif
  // Minimal setup content for now
}

void loop() {
#ifdef ARDUINO
  // Arduino loop
  Serial.println("Looping... (Arduino)");
  flexhal::utils::Log.debug("FlexHAL", "Looping... (Arduino)"); // Example log
  delay(1000);
#else // --- Native Environment ---
  // Keep the loop empty for this simple detection test
  // We could add periodic logging here if needed.
  // flexhal::utils::logger::Log.verbose(LOG_TAG, "Loop running..."); // Example verbose

  // Delay or yield if necessary for the platform
#if FLEXHAL_DETECT_FRAMEWORK_ARDUINO
  delay(1000); // Arduino standard delay
#elif defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
  // Simple sleep for native platforms
  #include <thread> // Requires C++11
  #include <chrono>
  std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
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
