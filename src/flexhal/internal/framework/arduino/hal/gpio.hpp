#pragma once

// Include all headers from the gpio implementation subdirectory
#include "gpio/ArduinoPin.hpp"
#include "gpio/ArduinoPort.hpp"
#include "gpio/ArduinoGpio.hpp"

#include "flexhal/base/status.hpp"      // For base::status
#include "flexhal/hal/gpio.hpp"         // For PinMode etc.
#include <Arduino.h>                  // For Arduino core functions like pinMode, digitalWrite

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// --- Functional API Declarations ---

/**
 * @brief Sets the mode of a specific pin (Arduino implementation).
 *
 * @param pin_number The platform-specific pin number.
 * @param mode The desired pin mode (flexhal::hal::gpio::PinMode).
 * @return flexhal::base::status::ok on success, or an error status if the mode is unsupported.
 */
base::status pin_mode(uint32_t pin_number, flexhal::hal::gpio::PinMode mode);

/**
 * @brief Writes a digital value (HIGH or LOW) to a specific pin (Arduino implementation).
 *
 * @param pin_number The platform-specific pin number.
 * @param level The value to write (true for HIGH, false for LOW).
 * @return flexhal::base::status Always returns ok for Arduino.
 */
base::status digital_write(uint32_t pin_number, bool level);

/**
 * @brief Reads the digital value (HIGH or LOW) from a specific pin (Arduino implementation).
 *
 * @param pin_number The platform-specific pin number.
 * @return 1 (HIGH) or 0 (LOW). Note: Error handling (e.g., for non-digital pins) is not directly supported by Arduino's digitalRead.
 */
int digital_read(uint32_t pin_number);

// Add declarations for analog_write, analog_read, pin_config etc. later

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal


// --- Implementation Section ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
#ifndef FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_IMPL_
#define FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_IMPL_

// Make sure Arduino.h is included for pinMode constants
// No need to include again if already included above top-level namespace
// #include <Arduino.h>
// #include "flexhal/base/status.hpp"
// #include "flexhal/hal/gpio.hpp"


namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

base::status pin_mode(uint32_t pin_number, flexhal::hal::gpio::PinMode mode) {
    int arduino_mode = -1; // Use -1 to indicate invalid/unassigned initially

    switch (mode) {
        case flexhal::hal::gpio::PinMode::Input:
            arduino_mode = INPUT;
            break;
        case flexhal::hal::gpio::PinMode::Output:
            arduino_mode = OUTPUT;
            break;
        case flexhal::hal::gpio::PinMode::InputPullup:
#ifdef INPUT_PULLUP
            arduino_mode = INPUT_PULLUP;
#else
            // Fallback if INPUT_PULLUP is not defined in this Arduino core
            // This usually implies only INPUT is available for pullup setting via pinMode.
            // The actual pullup might need separate digitalWrite or register manipulation.
            // For pinMode itself, falling back to INPUT might be the closest.
            arduino_mode = INPUT;
            // Consider logging a warning or returning a specific status if exact mode isn't supported?
            // For now, map to INPUT as a basic fallback.
#endif
            break;
        case flexhal::hal::gpio::PinMode::InputPulldown:
#ifdef INPUT_PULLDOWN // Check if a specific constant exists (less common)
            arduino_mode = INPUT_PULLDOWN;
#else
            // Standard Arduino pinMode doesn't support INPUT_PULLDOWN directly.
            // Fallback to simple INPUT. Actual pulldown needs separate handling.
            arduino_mode = INPUT;
             // TODO: Decide if returning error is better than INPUT fallback for unsupported pulldown.
             // return base::status::error; // Alternative
#endif
            break;
        case flexhal::hal::gpio::PinMode::OutputOpenDrain:
#ifdef OUTPUT_OPENDRAIN // Check if a specific constant exists (e.g., some ESP cores)
            arduino_mode = OUTPUT_OPENDRAIN;
#else
            // Standard Arduino pinMode doesn't support OUTPUT_OPENDRAIN.
            // Fallback to simple OUTPUT. Actual OD needs specific HW setup/register config.
            arduino_mode = OUTPUT;
            // TODO: Decide if returning error is better than OUTPUT fallback for unsupported open-drain.
            // return base::status::error; // Alternative
#endif
             break;
        case flexhal::hal::gpio::PinMode::Analog:
            // Analog functionality is usually implicit or set by analogRead/Write.
            // Mapping to INPUT for pinMode is a common approach for pins that *can* be analog.
             // Some cores might have an ANALOG constant, check if needed:
#ifdef ANALOG
             // arduino_mode = ANALOG; // If a specific constant exists and is appropriate
#else
             arduino_mode = INPUT; // Default fallback
#endif
            // TODO: Verify if specific cores require different handling for analog via pinMode.
            break;
         case flexhal::hal::gpio::PinMode::Disabled:
             // Treat as INPUT (High-Z) generally. Some platforms might have specific ways
             // to disable a pin (e.g., disconnect from peripheral).
             arduino_mode = INPUT;
             break;
        default:
            // Unknown FlexHAL mode, return error.
            return base::status::error;
    }

    // Check if a valid Arduino mode was determined
    if (arduino_mode != -1) {
        ::pinMode(pin_number, arduino_mode);
        return base::status::ok;
    } else {
        // This path might be taken if a mode explicitly decided to return error above,
        // or if the default case was hit (which already returns error).
        // This specific 'else' might be redundant if all paths set arduino_mode or return early.
        // However, it catches the initial -1 state if somehow no case matched and set a valid mode.
        return base::status::error; // Indicate failure to map mode
    }
}

// digital_write and digital_read implementations remain the same as before
base::status digital_write(uint32_t pin_number, bool level) {
    ::digitalWrite(pin_number, level ? HIGH : LOW);
    return base::status::ok;
}

int digital_read(uint32_t pin_number) {
    return ::digitalRead(pin_number);
}

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_IMPL_
#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
