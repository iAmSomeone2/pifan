//
// Created by bdavidson on 9/6/20.
//

#include <stdexcept>

#include "Gpio.hpp"

extern "C"
{
#include <pigpiod_if2.h>
}

using namespace Pifan::Hardware;

Gpio::Gpio()
{
    const auto target = pigpio_start(nullptr, nullptr);

    if (target < 0)
    {
        throw std::runtime_error("Gpio::Gpio: Failed to connect to PiGPIO daemon.");
    }

    this->targetPi = target;

    this->pinMap = std::map<uint8_t, Pin>();
}

Gpio::~Gpio() noexcept
{
    pigpio_stop(this->targetPi);
}

void Gpio::addPin(const uint8_t& index)
{
    if (this->pinMap.find(index) != this->pinMap.end())
    {
        // An entry for this pin is already present.
        return;
    }

    auto newPin = Pin(index, this->targetPi);

    this->pinMap.insert(std::pair<uint8_t, Pin>(index, newPin));
}

Pin Gpio::getPin(const uint8_t& index)
{
    return this->pinMap.at(index);
}

const uint8_t &Gpio::getTargetPi() const
{
    return this->targetPi;
}
