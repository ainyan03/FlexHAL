#pragma once

#include <cstdint>
#include <flexhal/base/error.hpp>

namespace flexhal { namespace hal { namespace gpio {

// Forward declarations
class IGpio;
class IPin;

/**
 * @brief Represents a single GPIO port.
 *
 * This interface provides methods for port-wide operations (read/write)
 * and for accessing individual pins (IPin) within the port.
 */
class IPort {
public:
    virtual ~IPort() = default;

    /**
     * @brief Gets the parent IGpio instance this port belongs to.
     * @return Reference to the parent IGpio.
     */
    virtual IGpio& getGpio() = 0;
    virtual const IGpio& getGpio() const = 0;

    /**
     * @brief Gets the index of this port within its parent IGpio.
     * @return The port index (0-based).
     */
    virtual uint32_t getPortIndex() const = 0;

    /**
     * @brief Gets the number of pins available in this port.
     * @return The number of pins (e.g., 8, 16, 32).
     */
    virtual uint32_t getNumberOfPins() const = 0;

    /**
     * @brief Writes a value to the entire port.
     *
     * The interpretation of the value depends on the port size (getNumberOfPins()).
     * For an 8-pin port, only the lower 8 bits of 'value' might be used.
     * Implementations should clarify the behavior for different port sizes.
     *
     * @param value The value to write to the port.
     * @return flexhal::base::status::Ok on success, or an error code.
     */
    virtual base::error_t write(uint32_t value) = 0; // Data size fixed to uint32_t for now?

    /**
     * @brief Reads the current state of the entire port.
     *
     * The returned value represents the state of all pins in the port.
     * For an 8-pin port, only the lower 8 bits might be relevant.
     * Implementations should clarify the behavior.
     * Consider error handling if read fails. (Returning value directly for now)
     *
     * @return The current value of the port.
     */
    virtual uint32_t read() const = 0; // Need to define error handling.

    /**
     * @brief Gets a specific pin within this port by its index.
     * @param pin_index_in_port The index of the pin within this port (0 to getNumberOfPins()-1).
     * @return Reference to the IPin interface.
     * @note Behavior for invalid index is undefined (assertion recommended).
     */
    virtual IPin& getPin(uint32_t pin_index_in_port) = 0;
    virtual const IPin& getPin(uint32_t pin_index_in_port) const = 0;

    // Potential future additions:
    // - Configure multiple pins at once (setModeMask, setConfigMask)
    // - More robust error handling for read()
};

}}} // namespace flexhal::hal::gpio
