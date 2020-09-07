//
// Created by bdavidson on 9/6/20.
//

#include <stdexcept>

extern "C"
{
#include <pigpiod_if2.h>
}

#include "Pin.hpp"

using namespace Pifan::Hardware;

Pin::Pin(const uint8_t &index, const uint8_t &target) : Pin(index, target, "Generic")
{
}

Pin::Pin(const uint8_t &index, const uint8_t &target, std::string label) : pinNumber(index), target(target),
                                                                           pinLabel(std::move(label))
{
    // Run a quick test to make sure the pin is accessible
    const auto result = get_mode(this->target, this->pinNumber);
    if (result == PI_BAD_GPIO)
    {
        throw std::runtime_error("Pin::Pin: Invalid pin or target number used when creating object.");
    }

    this->mode = PinMode(result);

    this->state = PinState(gpio_read(this->target, this->pinNumber));
}

void Pin::setPullUpDown(const ResistorState &resistorState) const
{
    const auto result = set_pull_up_down(this->target, this->pinNumber, static_cast<int>(resistorState));

    switch (result)
    {
        case 0:
            break;
        case PI_BAD_GPIO:
            throw std::runtime_error("Pin::setPullUpDown: Not a valid GPIO pin number");
        case PI_BAD_PUD:
            throw std::runtime_error("Pin::setPullUpDown: Not a valid resistor state.");
        case PI_NOT_PERMITTED:
            throw std::runtime_error("Pin::setPullUpDown: User does not have permission to set resistor state.");
        default:
            throw std::runtime_error("Pin::setPullUpDown: Unknown error occurred.");
    }
}

void Pin::setMode(const PinMode &newMode)
{
    const auto result = set_mode(this->target, this->pinNumber, static_cast<int>(newMode));

    switch (result)
    {
        case 0:
            this->mode = newMode;
            break;
        case PI_BAD_GPIO:
            throw std::runtime_error("Pin::setMode: Not a valid GPIO pin number");
        case PI_BAD_MODE:
            throw std::runtime_error("Pin::setMode: Not a valid pin mode.");
        case PI_NOT_PERMITTED:
            throw std::runtime_error("Pin::setMode: User does not have permission to set pin mode.");
        default:
            throw std::runtime_error("Pin::setMode: Unknown error occurred.");
    }
}

const PinMode &Pin::getMode() const
{
    return this->mode;
}

void Pin::write(const PinState &newState)
{
    const auto result = gpio_write(this->target, this->pinNumber, static_cast<int>(newState));

    switch (result)
    {
        case 0:
            this->state = newState;
            break;
        case PI_BAD_GPIO:
            throw std::runtime_error("Pin::write: Not a valid GPIO pin number");
        case PI_BAD_LEVEL:
            throw std::runtime_error("Pin::write: Not a valid pin state.");
        case PI_NOT_PERMITTED:
            throw std::runtime_error("Pin::write: User does not have permission to set pin state.");
        default:
            throw std::runtime_error("Pin::write: Unknown error occurred.");
    }
}

const PinState &Pin::read()
{
    const auto result = gpio_read(this->target, this->pinNumber);

    if (result == PI_BAD_GPIO)
    {
        throw std::runtime_error("Pin::read: Not a valid GPIO pin number");
    }

    this->state = PinState(result);

    return this->state;
}


