/*
 * main.cpp
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

#include <iostream>
#include <csignal>
#include <thread>
#include <string>
#include <fstream>

#include "hardware/Gpio.hpp"
#include "hardware/Fan.hpp"

#include <PiFanConfig.hpp>

using namespace Pifan;

volatile bool active = true;

/**
 * Lets the program know that it needs to clean up and shutdown.
 * @param signum interrupt signal value.
 */
void interruptHandler(int signum) {
    std::cout << "\nReceived interrupt signal (" << signum << ").\n";
    active = false;
}

uint32_t getSystemTemp()
{
    auto inStream = std::ifstream(THERMAL_FILE_PATH, std::ios::ate);
    if (!inStream.is_open())
    {
        throw std::runtime_error("Daemon::getSystemTemp: Failed to open temperature file for reading.");
    }

    uint32_t byteCount = inStream.tellg();
    inStream.seekg(std::ios::beg);

    char *buff = new char[byteCount];
    inStream.readsome(buff, byteCount);
    inStream.close();

    uint32_t sysTemp = std::strtoul(buff, nullptr, 10);
    delete[] buff;

    return sysTemp;
}

int main() {
    int targetTemp = 48000;

    std::clog << "Target temp: " << targetTemp/1000 << "°C\n";

    // Connect to pigpio daemon
    auto gpio = Hardware::Gpio();
    
    std::clog << "Connected to pigpio daemon." << std::endl;

    // Create a Fan object to keep track of its operations
    auto fan = Hardware::Fan(&gpio);

    // Register interrupt handler
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);

    // Main program loop
    while (active) {
        auto currentTemp = getSystemTemp();

        std::string humanTemp =  std::to_string(currentTemp/1000) + "." + std::to_string(currentTemp % 1000);

        std::clog << "Current temp: " << humanTemp << "°C\n";
        if (currentTemp > targetTemp)
        {
            if (!fan.isActive())
                fan.start();
        }
        else
        {
            if (fan.isActive())
                fan.stop();
        }
        
        // Sleep for a specific time.
        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME_SECS));

        fan.updateFanSpeed();
        std::clog << "Current fan speed: " << std::to_string(fan.getFanSpeed()) << " RPM.\n";
    }

    std::clog << "Stopping Pifan daemon...\n";
    fan.stop();

    return 0;
}

