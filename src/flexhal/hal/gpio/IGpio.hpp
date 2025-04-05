#pragma once

#include <cstdint>
#include <memory> // For std::shared_ptr
#include "flexhal/base/status.hpp"
#include "flexhal/hal/gpio.hpp"

namespace flexhal { namespace hal { namespace gpio {

// Forward declarations for interfaces used
class IPort;
class IPin; // Keep for context, even if no direct methods use it currently

/**
 * @brief Represents a GPIO peripheral (e.g., a bank of GPIO ports).
 *
 * This interface serves as the entry point to access GPIO ports (IPort)
 * managed by this peripheral.
 */
class IGpio {
public:
    virtual ~IGpio() = default;

    /**
     * @brief Gets the total number of GPIO ports managed by this peripheral.
     * @return The number of available ports.
     */
    virtual uint32_t getNumberOfPorts() const = 0;

    /**
     * @brief Gets a specific GPIO port by its index.
     * @param port_index The index of the port (0 to getNumberOfPorts()-1).
     * @return Reference to the IPort interface.
     * @note Behavior for invalid index is undefined (assertion recommended).
     */
    virtual IPort& getPort(uint32_t port_index) = 0;
    virtual const IPort& getPort(uint32_t port_index) const = 0;

    // Optional methods from the design document (can be added later):
    // virtual IPort& getPortByName(char name) = 0;
    // virtual const IPort& getPortByName(char name) const = 0;
    // virtual IPin& getPin(/* TBD */) = 0;
    // virtual const IPin& getPin(/* TBD */) const = 0;

}; // class IGpio

}}} // namespace flexhal::hal::gpio
