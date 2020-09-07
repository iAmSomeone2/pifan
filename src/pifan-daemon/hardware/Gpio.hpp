//
// Created by bdavidson on 9/6/20.
//

#pragma once

#include <cstdint>
#include <map>

#include "Pin.hpp"

namespace Pifan::Hardware
{
    class Gpio
    {
    protected:
        uint8_t targetPi; /**< Numeric indicator of target Pi */
        std::map<uint8_t, Pin> pinMap;
    public:
        Gpio();
        ~Gpio() noexcept;

        void addPin(const uint8_t& index);

        Pin getPin(const uint8_t& index);

        const uint8_t& getTargetPi() const;
    };
}
