#pragma once

#include <cstdint>
#include <Arduino.h> // Include Arduino core library
#include <memory>     // For std::unique_ptr if needed later, maybe not here

#include "flexhal/hal/gpio/IPin.hpp"
#include "flexhal/hal/gpio/IPort.hpp" // Include IPort for getPort()
#include "flexhal/hal/gpio.hpp"    // Include gpio.hpp for PinMode, PinConfig etc.

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

class ArduinoPin : public flexhal::hal::gpio::IPin {
public:
    // Constructor needs the parent Port and the pin index within that port
    explicit ArduinoPin(flexhal::hal::gpio::IPort& port, uint32_t pin_index);

    // --- IPin Interface Implementation ---

    // Get the parent Port interface
    flexhal::hal::gpio::IPort& getPort() override;
    const flexhal::hal::gpio::IPort& getPort() const override;

    // Get the pin's index within its port
    uint32_t getPinIndex() const override;

    // Configure pin mode (Input, Output, Pullups, etc.) using PinMode enum
    flexhal::base::status setMode(flexhal::hal::gpio::PinMode mode) override;

    // Configure pin using the detailed PinConfig struct (IPin requires this)
    flexhal::base::status setConfig(const flexhal::hal::gpio::PinConfig& config) override;

    // Digital I/O
    flexhal::base::status digitalWrite(bool level) override; // Updated parameter to bool
    int digitalRead() const override;             // Updated return type to int

    // Analog I/O (PWM/DAC/ADC) - Keep existing signatures if they match IPin
    flexhal::base::status analogWrite(uint32_t value) override; // Keep uint32_t if IPin uses it
    int analogRead() const override;                  // Keep int if IPin uses it

    // --- End IPin Interface ---

private:
    flexhal::hal::gpio::IPort& _port; // Reference to the parent port
    uint32_t _pin_index;              // Pin index within the port
};

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
#ifndef FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPIN_IPP
#define FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPIN_IPP

#include <Arduino.h> // Make sure Arduino API is available for implementation

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// Constructor Implementation
inline ArduinoPin::ArduinoPin(flexhal::hal::gpio::IPort& port, uint32_t pin_index)
    : _port(port), _pin_index(pin_index) /*, _pin_number(pin_index) */ // Initialize members. Map index to Arduino pin number if necessary.
{
    // Initialization logic if needed, e.g., map logical index to physical Arduino pin number
    // For now, assume pin_index directly corresponds to Arduino pin number for simplicity.
}

// --- IPin Implementation ---

inline flexhal::hal::gpio::IPort& ArduinoPin::getPort() {
    return _port;
}

inline const flexhal::hal::gpio::IPort& ArduinoPin::getPort() const {
    return _port;
}

inline uint32_t ArduinoPin::getPinIndex() const {
    return _pin_index; // Return the stored pin index
}

inline flexhal::base::status ArduinoPin::setMode(flexhal::hal::gpio::PinMode mode) {
    // Map FlexHAL PinMode to Arduino pinMode() arguments
    switch (mode) {
        case flexhal::hal::gpio::PinMode::Input:
            pinMode(_pin_index, INPUT); // Assuming _pin_index is the Arduino pin number
            break;
        case flexhal::hal::gpio::PinMode::InputPullup:
            pinMode(_pin_index, INPUT_PULLUP);
            break;
        case flexhal::hal::gpio::PinMode::InputPulldown:
            // Arduino doesn't have a standard INPUT_PULLDOWN define for all boards.
            // May require board-specific code or return unsupported.
             pinMode(_pin_index, INPUT); // Default to INPUT
            // return flexhal::base::status::unsupported;
            break;
        case flexhal::hal::gpio::PinMode::Output:
            pinMode(_pin_index, OUTPUT);
            break;
        case flexhal::hal::gpio::PinMode::OutputOpenDrain:
             pinMode(_pin_index, OUTPUT); // Default to OUTPUT
            // Requires board-specific handling, might not be universally available.
            // return flexhal::base::status::unsupported;
            break;
        // Handle other modes like Analog, Pwm if necessary, or return unsupported
        default:
            return flexhal::base::status::unsupported;
    }
    return flexhal::base::status::ok; // Return OK on success
}

// Implementation for setConfig (handles pull modes)
inline flexhal::base::status ArduinoPin::setConfig(const flexhal::hal::gpio::PinConfig& config) {
    // Based on the pull mode, call the appropriate pinMode()
    // Note: This assumes the pin is intended to be used as an input when pulls are active.
    switch (config.pull) {
        case flexhal::hal::gpio::PinPull::Up:
            pinMode(_pin_index, INPUT_PULLUP);
            return flexhal::base::status::ok;

        case flexhal::hal::gpio::PinPull::Down:
#if defined(INPUT_PULLDOWN) // Check if the Arduino core defines INPUT_PULLDOWN
            pinMode(_pin_index, INPUT_PULLDOWN);
            return flexhal::base::status::ok;
#else
            // This Arduino core doesn't define INPUT_PULLDOWN, so unsupported.
            return flexhal::base::status::unsupported;
#endif

        case flexhal::hal::gpio::PinPull::None:
            // Setting pull to None. If the pin is an input, set it to plain INPUT.
            // If it's an output, this call might not have a direct effect on pull state,
            // but calling pinMode(INPUT) is the standard way to disable pulls.
            // We might need getMode() to be smarter here, but let's default to INPUT.
            pinMode(_pin_index, INPUT); 
            return flexhal::base::status::ok;

        default:
            // Unknown pull mode setting
            return flexhal::base::status::unsupported;
    }
}

inline flexhal::base::status ArduinoPin::digitalWrite(bool level) {
    ::digitalWrite(_pin_index, level ? HIGH : LOW); // Use global namespace ::digitalWrite
    return flexhal::base::status::ok; // Assume success
}

inline int ArduinoPin::digitalRead() const {
    return ::digitalRead(_pin_index); // Use global namespace ::digitalRead. Returns HIGH (1) or LOW (0).
}

inline flexhal::base::status ArduinoPin::analogWrite(uint32_t value) {
    // Arduino analogWrite usually takes an int (0-255 for PWM).
    // Need to consider the range of value (uint32_t) and map/clamp it.
    // Also, check if the pin supports PWM/DAC on the target Arduino board.
    // For simplicity, casting for now. May need proper range mapping.
    ::analogWrite(_pin_index, static_cast<int>(value)); // Standard Arduino analogWrite
    return flexhal::base::status::ok; // Assume success, though pin capability check is needed
}

inline int ArduinoPin::analogRead() const {
    return ::analogRead(_pin_index); // Use global namespace ::analogRead
}

// --- End IPin Implementation ---


} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPIN_IPP
#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
