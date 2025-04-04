// /Users/lovyan/ainyan/FlexHAL/src/flexhal/internal/framework/arduino/hal/gpio/ArduinoPin.hpp
#pragma once

// #include "flexhal/internal/config.hpp" // Use src-relative path // Removed unnecessary include
#include "../../__detect.h"               // Relative path to parent's detect header

#if FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO

#include <Arduino.h>
#include "flexhal/base/error.hpp"    // Use src-relative path
#include "flexhal/hal/gpio.hpp"      // Use src-relative path
#include "flexhal/hal/gpio/IPin.hpp" // Use src-relative path

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

class ArduinoPin : public flexhal::hal::gpio::IPin {
public:
    explicit ArduinoPin(flexhal::hal::gpio::pin_id_t pin_id); 

    flexhal::hal::gpio::pin_id_t getPinNumber() const override;

    base::error_t setConfig(const flexhal::hal::gpio::PinConfig& config) override;
    base::error_t digitalWrite(flexhal::hal::gpio::DigitalValue value) override;
    flexhal::hal::gpio::DigitalValue digitalRead() override;
    base::error_t analogWrite(int value) override;
    int analogRead() override;
    flexhal::hal::gpio::pin_id_t getPinId() const { return _pin_id; }

private:
    flexhal::hal::gpio::pin_id_t _pin_id; 
    flexhal::hal::gpio::PinConfig _current_config; 
};

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

// --- Implementation Section ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

ArduinoPin::ArduinoPin(flexhal::hal::gpio::pin_id_t pin_id)
    : _pin_id(pin_id)
{
    // Initialize _current_config with default (Input, None, Floating)
    // Optionally, could read initial pin state if possible/needed, but unlikely reliable.
}

inline flexhal::hal::gpio::pin_id_t ArduinoPin::getPinNumber() const {
    // Assuming _pin_id directly corresponds to the physical pin number
    // and fits within uint8_t. Add checks if necessary.
    return _pin_id;
}

base::error_t ArduinoPin::setConfig(const flexhal::hal::gpio::PinConfig& config)
{
    uint8_t arduino_mode = INPUT; // Default to INPUT
    bool mode_set = false;

    // Map FlexHAL config to Arduino pinMode constants
    if (config.dir == flexhal::hal::gpio::PinDir::Input) {
        if (config.pull == flexhal::hal::gpio::PinPull::Up) {
            arduino_mode = INPUT_PULLUP;
            mode_set = true;
#ifdef INPUT_PULLDOWN // Check if the feature macro is defined
        } else if (config.pull == flexhal::hal::gpio::PinPull::Down) {
            arduino_mode = INPUT_PULLDOWN;
            mode_set = true;
#endif // INPUT_PULLDOWN
        } else { // No pull or unsupported pull
            arduino_mode = INPUT;
            mode_set = true;
        }
    } else if (config.dir == flexhal::hal::gpio::PinDir::Output) {
        if (config.signal_type == flexhal::hal::gpio::PinSignalType::PushPull) {
            arduino_mode = OUTPUT;
            mode_set = true;
#ifdef OUTPUT_OPEN_DRAIN // Check if the feature macro is defined
        } else if (config.signal_type == flexhal::hal::gpio::PinSignalType::OpenDrain) {
            arduino_mode = OUTPUT_OPEN_DRAIN;
            mode_set = true;
#endif // OUTPUT_OPEN_DRAIN
        }
        // Note: Pull settings are typically ignored for output modes in Arduino
    } else if (config.dir == flexhal::hal::gpio::PinDir::InOut) {
         // InOut might map to INPUT for reading, OUTPUT for writing? 
         // Or specific bidirectional modes if platform supports? Needs clarification.
         // For now, treat like INPUT for pinMode.
         arduino_mode = INPUT;
         mode_set = true; 
    }

    // Handle Analog type - often no specific pinMode needed, but depends on platform ADC/DAC setup.
    // If signal_type is Analog, we might skip pinMode or use a specific one.
    if (config.signal_type == flexhal::hal::gpio::PinSignalType::Analog) {
       // For basic analogRead on many Arduinos, no pinMode is strictly needed.
       // For ESP32, pinMode(pin, ANALOG) exists but isn't always required.
       // Let's assume no action needed for now, can refine later.
       mode_set = true; // Consider it handled
    }
     // Handle Pwm type - typically needs OUTPUT mode set first, handled by analogWrite usually.
    if (config.signal_type == flexhal::hal::gpio::PinSignalType::Pwm) {
        arduino_mode = OUTPUT; // PWM usually requires OUTPUT mode
        mode_set = true;
    }

    if (mode_set) {
        // Check if pin ID is valid for Arduino (simple cast for now)
        // Might need more robust checking based on board limits (NUM_DIGITAL_PINS)
        pinMode(static_cast<uint8_t>(_pin_id), arduino_mode);
        _current_config = config; // Store the applied config
        return base::to_error(base::status::ok);
    } else {
        // Configuration combination not supported or invalid
        // Should PinMode::Disabled be handled explicitly?
        return base::to_error(base::status::unsupported); // Or param error?
    }
}

base::error_t ArduinoPin::digitalWrite(flexhal::hal::gpio::DigitalValue value)
{
    // Map FlexHAL DigitalValue to Arduino HIGH/LOW
    uint8_t arduino_value = (value == flexhal::hal::gpio::DigitalValue::High) ? HIGH : LOW;
    ::digitalWrite(static_cast<uint8_t>(_pin_id), arduino_value);
        return base::to_error(base::status::ok);
}

flexhal::hal::gpio::DigitalValue ArduinoPin::digitalRead()
{
    int arduino_value = ::digitalRead(static_cast<uint8_t>(_pin_id));
    // Map Arduino HIGH/LOW back to FlexHAL DigitalValue
    return (arduino_value == HIGH) ? flexhal::hal::gpio::DigitalValue::High : flexhal::hal::gpio::DigitalValue::Low;
}

base::error_t ArduinoPin::analogWrite(int value)
{
    ::analogWrite(static_cast<uint8_t>(_pin_id), value);
    return base::to_error(base::status::ok);
}

int ArduinoPin::analogRead()
{
    return ::analogRead(static_cast<uint8_t>(_pin_id));
}

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION

#endif // FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO
