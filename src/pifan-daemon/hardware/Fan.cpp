//
// Created by bdavidson on 9/6/20.
//

#include <iostream>

#include "Fan.hpp"

extern "C"
{
#include <pigpiod_if2.h>
}

using namespace Pifan::Hardware;

Fan::Fan(Gpio *gpio)
{
    this->gpio = gpio;

    // Add pins to GPIO handler
    this->gpio->addPin(enablePin);
    this->gpio->addPin(tachPin);

    // Set starting values for in-use pins
    this->gpio->getPin(enablePin).setMode(PinMode::Output);
    this->gpio->getPin(enablePin).setPullUpDown(ResistorState::PullDown);
    this->gpio->getPin(enablePin).write(PinState::Low);

    this->gpio->getPin(tachPin).setMode(PinMode::Input);
    this->gpio->getPin(tachPin).setPullUpDown(ResistorState::PullDown);

    // Set up a callback for the fan's tachometer pin.
    this->tachCallback = callback_ex(this->gpio->getTargetPi(), tachPin, RISING_EDGE,
                                     reinterpret_cast<CBFuncEx_t>(incrementTachPulse), &this->tachPulseCount);
    switch (this->tachCallback)
    {
        case pigif_bad_malloc:
            std::cerr << "Fan::Fan: Pigpio encountered a bad malloc\n";
            break;
        case pigif_duplicate_callback:
            std::cerr << "Fan::Fan: Pigpio encountered a duplicate callback\n";
            break;
        case pigif_bad_callback:
            std::cerr << "Fan::Fan: Pigpio encountered a bad callback\n";
            break;
        default:
            std::clog << "Fan::Fan: Successfully registered callback for fan speed.\n";
    }

    this->tachStartTime = std::chrono::steady_clock::now();
}

Fan::~Fan() noexcept
{
    callback_cancel(this->tachCallback);
}

void Fan::incrementTachPulse(int pi, uint userGPIO, uint level, uint32_t tick, void *userData)
{
    auto tachNum = reinterpret_cast<int *>(userData);

    tachNum++;
}

void Fan::start()
{
    this->gpio->getPin(enablePin).write(PinState::High);
    this->active = true;
}

void Fan::stop()
{
    this->gpio->getPin(enablePin).write(PinState::Low);
    this->active = false;
}

const bool &Fan::isActive() const
{
    return this->active;
}

const uint32_t &Fan::getFanSpeed() const
{
    return this->fanRPM;
}

void Fan::updateFanSpeed()
{
    if (tachPulseCount == 0)
    {
        this->fanRPM = 0;
        return;
    }

    auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(this->tachStartTime - std::chrono::steady_clock::now());

    this->fanRPM = this->tachPulseCount / timeDiff.count() / 60000;
    this->tachPulseCount = 0u;
}



