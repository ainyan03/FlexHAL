#pragma once

#include <flexhal/base/error.hpp>
#include "../gpio.hpp" // Correct include path for types like PinMode, PinConfig
#include <cstdint>

// Forward declarations
namespace flexhal { namespace hal { namespace gpio {
class IPort; // Forward declare IPort
// Ensure PinMode, PinConfig are either defined here or included via gpio.hpp
// enum class PinMode : uint8_t;
// struct PinConfig;
}}}

namespace flexhal { namespace hal { namespace gpio {

/**
 * @brief Represents a single GPIO pin.
 *
 * Provides methods for configuring and interacting with an individual pin.
 */
class IPin {
public:
    virtual ~IPin() = default;

    /**
     * @brief Gets the parent IPort instance this pin belongs to.
     * @return Reference to the parent IPort.
     */
    virtual IPort& getPort() = 0;
    virtual const IPort& getPort() const = 0;

    /**
     * @brief Gets the index of this pin within its parent IPort.
     * @return The pin index (0-based).
     */
    virtual uint32_t getPinIndex() const = 0;

    // --- Pin Configuration ---

    /**
     * @brief Sets the basic mode of the pin (Input, Output, etc.).
     * @param mode The desired pin mode.
     * @return base::error_t OK on success, or an error code.
     */
    virtual base::error_t setMode(PinMode mode) = 0;

    /**
     * @brief Applies detailed configuration to the pin.
     * Allows setting speed, pull-up/down, open-drain, etc.
     * @param config The pin configuration structure.
     * @return base::error_t OK on success, or an error code.
     */
    virtual base::error_t setConfig(const PinConfig& config) = 0;

    // Optional config getters (can be added later)
    // virtual PinMode getMode() const = 0;
    // virtual PinConfig getConfig() const = 0;


    // --- Digital I/O ---

    /**
     * @brief Write a digital value to the pin.
     * The pin should be configured as an output.
     * @param level The digital level to write (true for HIGH, false for LOW).
     * @return base::error_t OK on success, or an error code.
     */
    virtual base::error_t digitalWrite(bool level) = 0; // Changed parameter to bool

    /**
     * @brief Read the digital value of the pin.
     * The pin should be configured as an input.
     * @return int The current digital level (1 for HIGH, 0 for LOW),
     *         or a negative error code on failure.
     */
    virtual int digitalRead() const = 0; // Changed return type to int

    // --- Analog I/O (Optional) ---

    /**
     * @brief Write an analog value (e.g., PWM duty cycle) to the pin.
     * Behavior depends on hardware capabilities (PWM, DAC).
     * @param value The analog value to write. Range is platform-dependent.
     * @return base::error_t OK on success, status::unsupported if unavailable, or other error code.
     */
    virtual base::error_t analogWrite(uint32_t value) { // Changed parameter to uint32_t
        // Default implementation returns not_supported
        return base::error_t(base::status::unsupported); // Use correct status code
    }

    /**
     * @brief Read an analog value from the pin.
     * Typically used for ADC input.
     * @return int The analog value read (0 to Max), range is platform-dependent,
     *         or a negative error code on failure or if unsupported.
     */
    virtual int analogRead() const { // Changed return type to int
        // Default implementation returns error
        // Consider returning a specific error code like NotSupported
        // For now, let's return a generic error indicator (e.g., -1 or a specific error code)
        return static_cast<int>(base::status::unsupported); // Return an error code
    }

    // --- Interrupts (Optional - Future) ---
    // virtual base::error_t attachInterrupt(...) = 0;
    // virtual base::error_t detachInterrupt() = 0;

}; // class IPin

}}} // namespace flexhal::hal::gpio
