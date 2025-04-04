// /Users/lovyan/ainyan/FlexHAL/src/flexhal/internal/framework/arduino/hal/gpio/ArduinoGpio.hpp
#pragma once

// #include "flexhal/internal/config.hpp" // Use src-relative path
#include "../../__detect.h"               // Relative path to parent's detect header

#if FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO == 1

#include "flexhal/hal/gpio.hpp"      // Use src-relative path
#include "flexhal/hal/gpio/IGpio.hpp" // Use src-relative path
#include "flexhal/hal/gpio/IPin.hpp"  // Use src-relative path (for IPin return type)
#include "ArduinoPin.hpp"             // Relative path for sibling header
#include <cstdint>                    
#include <vector>                     
#include <memory>                     

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

class ArduinoGpio : public flexhal::hal::gpio::IGpio {
public:
    ArduinoGpio(); 

    flexhal::hal::gpio::IPin* getPin(uint32_t pin_index) override; 
    size_t getPinCount() const override;

private:
    std::vector<std::unique_ptr<ArduinoPin>> _pins;
    uint32_t _pin_count = 0; 
};

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

// Implementation Section
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

#include <Arduino.h> // Needed for NUM_DIGITAL_PINS

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

ArduinoGpio::ArduinoGpio() {
#ifdef NUM_DIGITAL_PINS
    _pins.resize(NUM_DIGITAL_PINS); // Resize vector to hold potential pins
#else
    // Fallback or error if NUM_DIGITAL_PINS is not defined?
    // For now, assume it's defined in Arduino environments.
    // You might want to add a static_assert or #error here.
#endif
}

size_t ArduinoGpio::getPinCount() const {
#ifdef NUM_DIGITAL_PINS
    return NUM_DIGITAL_PINS;
#else
    return 0; // Or handle error
#endif
}

flexhal::hal::gpio::IPin* ArduinoGpio::getPin(uint32_t pin_index)
{
    // Check if pin_index is valid
#ifdef NUM_DIGITAL_PINS
    if (pin_index >= NUM_DIGITAL_PINS) {
        return nullptr; // Index out of bounds
    }
#else
    // If NUM_DIGITAL_PINS isn't defined, we cannot validate the index easily.
    // Maybe check against _pins.size() if it was resized some other way?
    if (pin_index >= _pins.size()) { 
        return nullptr;
    }
#endif

    // Lazy initialization: Create the ArduinoPin instance if it doesn't exist yet
    if (!_pins[pin_index]) {
        _pins[pin_index] = std::make_unique<ArduinoPin>(pin_index); 
    }

    // Return the raw pointer managed by unique_ptr
    return _pins[pin_index].get();
}

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION

#endif // FLEXHAL_DETECT_INTERNAL_FRAMEWORK_ARDUINO == 1
