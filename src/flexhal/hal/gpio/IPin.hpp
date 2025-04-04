\
#pragma once

#include "../gpio.hpp"       // Include types like PinMode, PinConfig, DigitalValue
#include "../../base/error.hpp" // Include base::error_t, to_error, status

namespace flexhal {
namespace hal {
namespace gpio {

// Single Pin Interface
class IPin {
public:
    virtual ~IPin() = default;

    /**
     * @brief Set the pin mode using a basic PinMode enum.
     * 
     * This is a convenience function that calls setConfig() with a converted PinConfig.
     * 
     * @param mode The desired basic pin mode.
     * @return base::error_t OK on success, or an error code.
     */
    virtual base::error_t setMode(PinMode mode) {
        // Default implementation converts PinMode to PinConfig and calls setConfig
        return setConfig(PinConfig(mode)); 
    }

    /**
     * @brief Configure the pin using a detailed PinConfig struct.
     * 
     * This is the primary method for setting the pin's direction, pull resistors,
     * signal type (digital, analog, pwm), and drive mode.
     * 
     * @param config The detailed pin configuration.
     * @return base::error_t OK on success, or an error code.
     */
    virtual base::error_t setConfig(const PinConfig& config) = 0;

    /**
     * @brief Write a digital value (High or Low) to the pin.
     * The pin must be configured as an output.
     * 
     * @param value The digital value to write (DigitalValue::High or DigitalValue::Low).
     * @return base::error_t OK on success, or an error code (e.g., if not configured as output).
     */
    virtual base::error_t digitalWrite(DigitalValue value) = 0;

    /**
     * @brief Read the digital value of the pin.
     * The pin must be configured as an input (or InOut).
     * 
     * @return DigitalValue The current digital value (DigitalValue::High or DigitalValue::Low).
     *         Behavior is undefined if the pin is not configured for digital input.
     */
    virtual DigitalValue digitalRead() = 0;

    /**
     * @brief Write an analog value (e.g., PWM duty cycle) to the pin.
     * This is typically used for PWM output or true DAC output if available.
     * The pin must be configured appropriately (e.g., PinSignalType::Pwm or PinSignalType::Analog for DAC).
     * 
     * @param value The analog value to write. The range depends on the platform 
     *              (e.g., 0-255 for Arduino PWM, 0-4095 for ESP32 DAC).
     * @return base::error_t OK on success, status::not_implemented if not supported, or other error codes.
     */
    virtual base::error_t analogWrite(int value) {
        // Default implementation returns not_implemented
        return base::error_t(base::status::not_implemented);
    }

    /**
     * @brief Read an analog value from the pin.
     * This is typically used for ADC (Analog-to-Digital Converter) input.
     * The pin must be configured as analog input (e.g., PinSignalType::Analog).
     * 
     * @return int The analog value read. The range depends on the platform's ADC resolution
     *           (e.g., 0-1023 for Arduino Uno, 0-4095 for ESP32).
     *           Returns a negative value (e.g., -1) if analog read is not supported or not configured.
     */
    virtual int analogRead() {
        // Default implementation returns -1 to indicate not supported/implemented
        return -1; 
    }

    /**
     * @brief Get the physical pin number associated with this interface.
     * 
     * @return uint8_t The pin number.
     */
    virtual flexhal::hal::gpio::pin_id_t getPinNumber() const = 0;
};

} // namespace gpio
} // namespace hal
} // namespace flexhal
