/*
 * main.cxx
 * 
 * Copyright 2019 Brenden Davidson <bdavidson@manjaro-pi>
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


extern "C" {
    #include <pigpiod_if2.h>
}

#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>

#include "Fan.h"
#include "DataAccess.h"

volatile bool active = true;
volatile int pulseCount = 0;

/**
 * Lets the program know that it needs to clean up and shutdown.
 * @param signum interrupt signal value.
 */
void interruptHandler(int signum) {
    std::cout << "\nReceived interrupt signal (" << signum << ").\n";
    active = false;
}

/**
 * Increments the pulseCount by one.
 * For use as a pigpio callback.
 */
void incrementPulse() {
    pulseCount++;
}

int main() {
    DataAccess data = DataAccess();

    std::cout << "Target temp: " << data.getTargetTemp()/1000 << "°C\n";

    // Connect to pigpio daemon
    int rPi = pigpio_start(nullptr, nullptr);
    if (rPi < 0) {
        std::cerr << "Failed to connect to pigpio daemon.\nExiting...\n";
        exit(1);
    }
    
    std::clog << "Connected to pigpio daemon." << std::endl;

    // Create a Fan object to keep track of its operations
    Fan piFan = Fan(rPi);

    // Set up a callback for the fan's tachometer pin.
    int callbackId = callback(rPi, piFan.m_tachPin, RISING_EDGE, reinterpret_cast<CBFunc_t>(incrementPulse));
    switch (callbackId) {
        case pigif_bad_malloc:
            std::cerr << "Pigpio encountered a bad malloc\n";
            break;
        case pigif_duplicate_callback:
            std::cerr << "Pigpio encountered a duplicate callback\n";
            break;
        case pigif_bad_callback:
            std::cerr << "Pigpio encountered a bad callback\n";
            break;
        default:
            std::clog << "Successfully registered callback for fan speed.\n";
    }

    // Register interrupt handler
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);

    // Main program loop
    while (active) {
        int currentTemp = data.getCurrentTemp();
        int targetTemp = data.getTargetTemp();

        std::cout << "Current temp: " << currentTemp << "°C\n";
        std::cout << "Target temp: " << targetTemp << "°C\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pigpio_stop(rPi);
    return 0;
}

