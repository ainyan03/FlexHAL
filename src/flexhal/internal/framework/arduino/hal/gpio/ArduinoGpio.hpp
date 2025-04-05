#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <cstdlib>         // For abort()
#include <cassert>         // For assert()

// Include the public interface this class implements
#include "flexhal/hal/gpio/IGpio.hpp"
// Include the Port interface needed by getPort()
#include "flexhal/hal/gpio/IPort.hpp"

// #include "ArduinoPin.hpp" // No longer directly needed here
// #include "flexhal/hal/gpio/IPin.hpp" // No longer directly needed here

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// Forward declare the port implementation
class ArduinoPort;

// Implement the IGpio interface for standard Arduino digital pins
class ArduinoGpio : public flexhal::hal::gpio::IGpio { // Inherit from IGpio
public:
    ArduinoGpio() = default; // Default constructor
    virtual ~ArduinoGpio() = default; // Virtual destructor for interface

    // --- IGpio Interface Implementation ---

    // Arduino typically presents digital pins as a single logical port (port 0)
    uint32_t getNumberOfPorts() const override;

    // Get the specific port interface
    flexhal::hal::gpio::IPort& getPort(uint32_t port_index) override;
    const flexhal::hal::gpio::IPort& getPort(uint32_t port_index) const override;

    // --- End IGpio Interface ---


    // --- Deprecated / Removed methods ---
    // flexhal::hal::gpio::IPin* getPin(uint32_t pin_index) override; // Removed
    // size_t getPinCount() const override; // Removed


private:
    // Store the single port (Port 0 for Arduino)
    // Use mutable to allow lazy initialization in const getPort()
    mutable std::unique_ptr<ArduinoPort> _port0;
    mutable bool _port0_initialized = false;

     // Helper for lazy initialization in const version
    void ensurePortInitialized() const;

    // --- Removed Pin Cache ---
    // We no longer cache pins directly in Gpio; the Port does.
    // mutable std::vector<std::unique_ptr<ArduinoPin>> _pin_cache;
    // mutable bool _cache_initialized = false;
    // void initializeCache() const;
};

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal


// --- Implementation Section ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
#ifndef FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOGPIO_IPP
#define FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOGPIO_IPP

#include "ArduinoPort.hpp" // Need the definition for make_unique

namespace flexhal {
namespace internal {
namespace framework {
namespace arduino {
namespace hal {
namespace gpio {

// --- IGpio Implementation ---

inline uint32_t ArduinoGpio::getNumberOfPorts() const {
    // Standard Arduino digital pins are treated as a single port.
    return 1;
}

// Helper for lazy initialization
inline void ArduinoGpio::ensurePortInitialized() const {
     if (!_port0_initialized) {
        _port0 = std::make_unique<ArduinoPort>(*this, 0); // Pass *this and port index 0
        _port0_initialized = true;
    }
}


// Non-const getPort
inline flexhal::hal::gpio::IPort& ArduinoGpio::getPort(uint32_t port_index) {
    // Check for invalid index first
    if (port_index != 0) {
        // Invalid port index for Arduino - only port 0 exists.
        assert(false && "ArduinoGpio::getPort: Invalid port_index (only port 0 exists).");
        abort(); // Terminate in release builds if assert is disabled
    }
    // If port_index is 0, proceed normally
    ensurePortInitialized(); // Initialize if needed
    return *_port0;
}

// Const getPort
inline const flexhal::hal::gpio::IPort& ArduinoGpio::getPort(uint32_t port_index) const {
     // Check for invalid index first
      if (port_index != 0) {
        // Invalid port index for Arduino - only port 0 exists.
        assert(false && "ArduinoGpio::getPort(const): Invalid port_index (only port 0 exists).");
        abort(); // Terminate in release builds if assert is disabled
      }
     // If port_index is 0, proceed normally
     ensurePortInitialized(); // Initialize if needed (mutable allows this)
     return *_port0;
}


// --- Removed Implementations ---
/*
inline void ArduinoGpio::initializeCache() const {
    // Obsolete: Port manages pins now.
    if (!_cache_initialized) {
#ifdef NUM_DIGITAL_PINS
        _pin_cache.resize(NUM_DIGITAL_PINS);
#else
        // Handle error or default size?
#endif
        _cache_initialized = true;
    }
}

inline flexhal::hal::gpio::IPin* ArduinoGpio::getPin(uint32_t pin_index) {
     // Obsolete: Use getPort(0).getPin(pin_index)
    initializeCache();
    if (pin_index >= _pin_cache.size()) {
        return nullptr; // Or throw?
    }
    if (!_pin_cache[pin_index]) {
        // Error: Pin constructor needs Port reference now!
        // _pin_cache[pin_index] = std::make_unique<ArduinoPin>(pin_index);
        // This old way is broken. Must go through Port.
        throw std::runtime_error("Cannot create pin directly in ArduinoGpio anymore.");
    }
    return _pin_cache[pin_index].get();
}


inline size_t ArduinoGpio::getPinCount() const {
    // Obsolete: Use getPort(0).getNumberOfPins()
#ifdef NUM_DIGITAL_PINS
    return NUM_DIGITAL_PINS;
#else
    return 0; // Or handle error
#endif
}
*/

} // namespace gpio
} // namespace hal
} // namespace arduino
} // namespace framework
} // namespace internal
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_FRAMEWORK_ARDUINO_HAL_GPIO_ARDUINOGPIO_IPP
#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
