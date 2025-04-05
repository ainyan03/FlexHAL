\
#pragma once

#include <stdint.h>

// Forward declarations (if needed by includes below, though likely not for interfaces)

namespace flexhal {
namespace hal {
namespace gpio {

/**
 * @brief Type definition for representing a GPIO pin identifier within FlexHAL.
 * Typically maps to a platform-specific pin number or index.
 */
using pin_id_t = uint8_t;

// Basic pin modes (for simple cases)
enum class PinMode : uint8_t {
    Input = 0,
    Output = 1,
    InputPullup = 2,
    InputPulldown = 3,
    OutputOpenDrain = 4,
    Analog = 5,
    Disabled = 6,
};

// Pin direction
enum class PinDir : uint8_t {
    Input = 0,
    Output = 1,
    InOut = 2
};

// Pull-up/pull-down setting
enum class PinPull : uint8_t {
    None = 0,
    Up = 1,
    Down = 2
};

// Pin signal/operation type
enum class PinSignalType : uint8_t {
    Floating = 0,      // Input (Floating/High-Z)
    PushPull = 1,      // Output (Push-Pull)
    OpenDrain = 2,     // Output (Open-Drain)
    Analog = 3,        // Analog I/O
    Pwm = 4            // PWM Output
};

// Digital value
enum class DigitalValue : uint8_t {
    Low = 0,
    High = 1
};

// Detailed pin configuration
struct PinConfig {
    PinDir dir = PinDir::Input;
    PinPull pull = PinPull::None;
    PinSignalType signal_type = PinSignalType::Floating;

    // Default constructor
    PinConfig() = default;

    // Conversion constructor from PinMode (inline implementation)
    inline PinConfig(PinMode mode) {
        switch (mode) {
            case PinMode::Input:
                dir = PinDir::Input;
                pull = PinPull::None;
                signal_type = PinSignalType::Floating;
                break;
            case PinMode::Output:
                dir = PinDir::Output;
                signal_type = PinSignalType::PushPull;
                break;
            case PinMode::InputPullup:
                dir = PinDir::Input;
                pull = PinPull::Up;
                signal_type = PinSignalType::Floating;
                break;
            case PinMode::InputPulldown:
                dir = PinDir::Input;
                pull = PinPull::Down;
                signal_type = PinSignalType::Floating;
                break;
            case PinMode::OutputOpenDrain:
                dir = PinDir::Output;
                signal_type = PinSignalType::OpenDrain;
                break;
            case PinMode::Analog:
                dir = PinDir::Input;
                signal_type = PinSignalType::Analog;
                break;
            case PinMode::Disabled:
                dir = PinDir::Input;
                pull = PinPull::None;
                signal_type = PinSignalType::Floating;
                // Note: Actual disabling might need platform-specific logic in setConfig impl.
                break;
        }
    }
};

// Forward declare interfaces defined in sub-directory headers
class IPin;
class IGpio;

} // namespace gpio
} // namespace hal
} // namespace flexhal


// Child interface headers (will be created next)
#include "gpio/IPin.hpp" 
#include "gpio/IPort.hpp"
#include "gpio/IGpio.hpp"
