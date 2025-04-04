\
#pragma once

#include <stddef.h> // For size_t
#include <stdint.h>

// Forward declare IPin instead of including the full header if only pointers/references are used
// However, since we return IPin*, the definition is needed.
#include "IPin.hpp" 

namespace flexhal {
namespace hal {
namespace gpio {

// GPIO Peripheral Interface
// Represents the collection of GPIO pins for a device or peripheral.
class IGpio {
public:
    virtual ~IGpio() = default;

    /**
     * @brief Get the interface for a specific GPIO pin.
     * 
     * @param pin_id The identifier (number) of the pin to retrieve.
     * @return IPin* A pointer to the IPin interface for the requested pin,
     *               or nullptr if the pin_id is invalid or not available.
     */
    virtual IPin* getPin(pin_id_t pin_id) = 0;

    /**
     * @brief Get the total number of GPIO pins managed by this peripheral.
     * 
     * @return size_t The number of available GPIO pins.
     */
    virtual size_t getPinCount() const = 0;
};

} // namespace gpio
} // namespace hal
} // namespace flexhal
