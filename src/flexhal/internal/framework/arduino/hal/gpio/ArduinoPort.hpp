#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <cassert> // For assert()

#include "flexhal/hal/gpio/IPort.hpp"
#include "flexhal/hal/gpio/IPin.hpp"

// Forward declare IGpio for the reference in constructor/member
namespace flexhal { namespace hal { namespace gpio { class IGpio; } } }

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// Forward declare ArduinoPin to be included only in implementation section
class ArduinoPin;

// Forward declare ArduinoGpio if needed, or include IGpio reference
class ArduinoGpio; // Assuming this is the concrete type, better use IGpio&

class ArduinoPort : public flexhal::hal::gpio::IPort {
public:
    // Constructor now takes a const reference to the parent GPIO controller
    explicit ArduinoPort(const flexhal::hal::gpio::IGpio& gpio, uint32_t port_index);
    virtual ~ArduinoPort() = default; // Virtual destructor

    // --- IPort Interface Implementation ---

    // Get the parent Gpio interface
    flexhal::hal::gpio::IGpio& getGpio() override;
    const flexhal::hal::gpio::IGpio& getGpio() const override;

    // Get the port's index within the Gpio controller
    uint32_t getPortIndex() const override;

    // Get total number of pins available in this port
    uint32_t getNumberOfPins() const override;

    // Get a specific pin interface by index
    flexhal::hal::gpio::IPin& getPin(uint32_t pin_index) override;
    const flexhal::hal::gpio::IPin& getPin(uint32_t pin_index) const override;

    // Port-level operations (optional, default implementations in IPort return unsupported)
    // base::error_t writePort(uint32_t value, uint32_t mask = 0xFFFFFFFF) override; // Use write() instead
    // uint32_t readPort(uint32_t mask = 0xFFFFFFFF) const override; // Use read() instead
    base::status write(uint32_t value) override; // Implement required write
    uint32_t read() const override;               // Implement required read
    // base::error_t setMode(flexhal::hal::gpio::PinMode mode, uint32_t mask = 0xFFFFFFFF) override;
    // base::error_t setConfig(const flexhal::hal::gpio::PinConfig& config, uint32_t mask = 0xFFFFFFFF) override;

    // --- End IPort Interface ---

private:
    const flexhal::hal::gpio::IGpio& _gpio; // Const reference to the parent Gpio controller
    uint32_t _port_index;
    // Use IPin pointers to store ArduinoPin instances polymorphically.
    // Use mutable for const getPin to allow lazy initialization.
    mutable std::vector<std::unique_ptr<flexhal::hal::gpio::IPin>> _pins;
    mutable bool _pins_initialized = false;

    // Helper for lazy initialization in const version
    void ensurePinsInitialized() const;
};

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal


// --- Implementation Section ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
#ifndef FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPORT_IPP
#define FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPORT_IPP

#include "ArduinoPin.hpp" // Include full definition for make_unique
#include <stdexcept>      // For std::out_of_range
#include <limits>       // For numeric_limits
#include <Arduino.h> // Include Arduino headers only in implementation for NUM_DIGITAL_PINS

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// Constructor
inline ArduinoPort::ArduinoPort(const flexhal::hal::gpio::IGpio& gpio, uint32_t port_index)
    : _gpio(gpio), _port_index(port_index)
{
    // Determine the number of pins for this port.
    // For Arduino, we typically map all digital pins to a single port (port 0).
    // Size _pins vector immediately? Or lazy init in getPin?
    // Let's lazy init in getPin/ensurePinsInitialized. NUM_DIGITAL_PINS is often a macro.
}

inline uint32_t ArduinoPort::getPortIndex() const {
    return _port_index;
}

// Return reference to the parent GPIO controller
inline flexhal::hal::gpio::IGpio& ArduinoPort::getGpio() {
    // Interface requires non-const, but we stored const. Cast away const.
    // This is generally safe if we know the underlying object might be non-const.
    return const_cast<flexhal::hal::gpio::IGpio&>(_gpio);
}

inline const flexhal::hal::gpio::IGpio& ArduinoPort::getGpio() const {
    return _gpio;
}

inline uint32_t ArduinoPort::getNumberOfPins() const {
    // Return the total number of digital pins available on the Arduino board.
#if defined(NUM_DIGITAL_PINS)
    return NUM_DIGITAL_PINS;
#else
    // Fallback if NUM_DIGITAL_PINS is not defined (should be defined by Arduino core)
    // Return a reasonable default or 0, indicating an issue.
    // Or maybe determine dynamically if possible?
    // Let's return 0 for now to indicate potential problem.
    // Consider adding a warning/error log here.
    // Warning: NUM_DIGITAL_PINS not defined for this Arduino platform.
    return 0; // Or throw?
#endif
}

// Helper for lazy initialization
inline void ArduinoPort::ensurePinsInitialized() const {
    if (!_pins_initialized) {
        _pins.resize(getNumberOfPins()); // Resize vector to hold all pins
        _pins_initialized = true;
    }
}

// Non-const getPin
inline flexhal::hal::gpio::IPin& ArduinoPort::getPin(uint32_t pin_index) {
    ensurePinsInitialized(); // Make sure vector is sized

    if (pin_index >= _pins.size()) {
        // Since exceptions are disabled, assert in debug, return pin 0 in release.
        assert(false && "ArduinoPort::getPin: pin_index out of range");
        // TODO: Log an error here in release builds?
        // Fallback: Return pin 0. This might hide errors but avoids crashing.
        // Be careful of infinite recursion if 0 is also invalid!
        return getPin(0);
    }

    // Lazy initialization of the specific pin
    if (!_pins[pin_index]) {
        _pins[pin_index] = std::make_unique<ArduinoPin>(*this, pin_index);
    }
    return *_pins[pin_index];
}

// Const getPin
inline const flexhal::hal::gpio::IPin& ArduinoPort::getPin(uint32_t pin_index) const {
    ensurePinsInitialized(); // Make sure vector is sized (mutable allows this)

    if (pin_index >= _pins.size()) { // Check bounds
        assert(false && "ArduinoPort::getPin(const): pin_index out of range");
        // TODO: Log an error here in release builds?
        return getPin(0); // Fallback to pin 0 (const version)
    }

    // Lazy initialization of the specific pin (mutable allows this)
    if (!_pins[pin_index]) {
        // With no exceptions, assert and fallback.
        assert(false && "ArduinoPort::getPin(const): Pin requested before initialization");
        // This case is less likely if bounds check passes and ensurePinsInitialized worked.
        // If it happens, returning pin 0 might be the safest fallback.
        return getPin(0);
        // Alternative: Return a static const error pin?
        // static const ErrorPin nullPin; return nullPin;
    }
    return *_pins[pin_index]; // Return const reference
}

// Implement write and read as unsupported for Arduino port-level access
inline base::status ArduinoPort::write(uint32_t value) {
    (void)value; // Mark unused
    return flexhal::base::status::unsupported; // Port write not supported
}

inline uint32_t ArduinoPort::read() const {
    // Port read not supported, return a default value (e.g., 0)
    return 0; // Or perhaps std::numeric_limits<uint32_t>::max() to indicate error?
}

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOPORT_IPP
#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
