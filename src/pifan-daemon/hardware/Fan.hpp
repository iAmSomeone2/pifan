/*
 * Fan.hpp
 *
 * Copyright 2019-2020 Brenden Davidson <davidson.brenden15@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#pragma once

#include <cstdint>
#include <chrono>

#include "Gpio.hpp"
#include "Pin.hpp"

#include <PiFanConfig.hpp>

namespace Pifan::Hardware
{
    class Fan
    {
    protected:
        // Pin assignments
        static constexpr uint8_t enablePin = 15u;
        static constexpr uint8_t tachPin = 14u;
        static constexpr uint8_t pwmPin = 18u;

        Gpio *gpio;

        // Tachometer tracking
        uint32_t tachPulseCount = 0u;
        int tachCallback = 0;
        uint32_t fanRPM = 0u;
        std::chrono::steady_clock::time_point tachStartTime;

        /**
         * \brief Increments the tachometer pulse variable.
         *
         * \param pi unused
         * \param userGPIO unused
         * \param level unused
         * \param tick unused
         * \param userData used to pass pointer to tachometer pulse number
         */
        static void incrementTachPulse(int pi, uint userGPIO, uint level, uint32_t tick, void *userData);

        // Current state
        bool active = false;
    public:
        explicit Fan(Gpio *gpio);

        /**
         * \brief Clean up instance and deregister callbacks.
         */
        ~Fan() noexcept;

        /**
         * \brief Activates the fan hardware.
         */
        void start();

        /**
         * \brief Deactivates the fan hardware.
         */
        void stop();

        /**
         * \brief Retrieves current active state of the fan hardware.
         * \return active status of fan hardware.
         */
        [[nodiscard]] const bool& isActive() const;

        [[nodiscard]] const uint32_t& getFanSpeed() const;

        void updateFanSpeed();
    };
}
