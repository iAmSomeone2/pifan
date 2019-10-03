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
#include <pifanconfig.h>

#include "Fan.h"
#include "DataAccess.h"

volatile __uint8_t active = 1;

void interruptHandler(int signum) {
    std::cout << "\nReceived interrupt signal (" << signum << ").\n";
    active = 0;
}

int main() {
    int rPi = pigpio_start(nullptr, nullptr);
    if (rPi < 0) {
        std::cout << "Failed to connect to pigpio daemon." << std::endl;
        exit(1);
    }
    
    std::cout << "Connected to pigpio daemon." << std::endl;
    
    // Create a Fan object to keep track of its operations
    Fan piFan = Fan(rPi);
    DataAccess data = DataAccess();

    // Register interrupt handler
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);

    // Main program loop
    while (active) {
        int currentTemp = data.getCurrentTemp();
        int targetTemp = data.getTargetTemp();

        std::cout << "Current temp: " << currentTemp << "°C\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pigpio_stop(rPi);
    return 0;
}

