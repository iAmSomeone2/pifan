/*
 * pifand.cxx
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
#include <pifanconfig.h>

#include "fan.hh"

int main(void) {
    int rPi = pigpio_start(NULL, NULL);
    if (rPi < 0) {
        std::cout << "Failed to connect to pigpio daemon." << std::endl;
    }
    
    std::cout << "Connected to pigpio daemon." << std::endl;
    
    // Create a Fan object to keep track of its operations
    Fan piFan = Fan(rPi, 15, 14, 18);
    
    
    pigpio_stop(rPi);
    return 0;
}

