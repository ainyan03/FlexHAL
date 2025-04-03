#include <FlexHAL.h> // Include the main FlexHAL header

// Define a logging tag for this file
static const char* LOG_TAG = "SimpleExample";

#ifdef ARDUINO
  #include <Arduino.h>
  // Simple logger implementation for Arduino using Serial
  class SimpleSerialLogger : public flexhal::utils::logger::ILogger {
  public:
      void log(flexhal::utils::logger::LogLevel level, const char* tag, const char* format, std::va_list args) override {
          if (Serial) { // Check if Serial is available
              char levelChar = '?';
              switch (level) {
                  case flexhal::utils::logger::LogLevel::ERROR:   levelChar = 'E'; break;
                  case flexhal::utils::logger::LogLevel::WARN:    levelChar = 'W'; break;
                  case flexhal::utils::logger::LogLevel::INFO:    levelChar = 'I'; break;
                  case flexhal::utils::logger::LogLevel::DEBUG:   levelChar = 'D'; break;
                  case flexhal::utils::logger::LogLevel::VERBOSE: levelChar = 'V'; break;
                  default: break;
              }
              char buffer[256]; // Static buffer for log message
              vsnprintf(buffer, sizeof(buffer), format, args);
              Serial.printf("[%c] [%s] %s\n", levelChar, tag, buffer);
          }
      }
  };
  static SimpleSerialLogger platform_logger; // Create an instance

#else // --- Native Environment ---
  #include <cstdio> // For vsnprintf used by PrintfLogger potentially
  #include <cstdlib> // For exit() if needed later
  #include "flexhal/platform/native/logger/PrintfLogger.hpp" // Include the native logger
  static flexhal::platform::native::logger::PrintfLogger platform_logger; // Create an instance
#endif


void setup() {
  #ifdef ARDUINO
    // Initialize Serial communication for Arduino
    Serial.begin(115200);
    // while (!Serial); // Optional: wait for serial connection
  #endif

  // Set the platform-specific logger instance
  flexhal::utils::logger::setLogger(&platform_logger);

  // Set the desired global log level
  flexhal::utils::logger::setLogLevel(flexhal::utils::logger::LogLevel::VERBOSE);

  // Log a setup completion message using FlexHAL logger
  flexhal::utils::logger::Log.info(LOG_TAG, "FlexHAL setup complete.");
}

void loop() {
  // Log a simple message using FlexHAL logger
  flexhal::utils::logger::Log.info(LOG_TAG, "Hello from FlexHAL loop!");

  // No delay/sleep needed for this simple example
}

#ifndef ARDUINO // Provide main() for native execution
int main() {
    setup();
    // Call loop once for simplicity in this native example
    loop();
    return 0; // Exit cleanly
}
#endif
