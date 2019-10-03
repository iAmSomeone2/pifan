/*
 * fan.cxx
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

#include "fan.hh"

Fan::Fan(int pi, int enablePin, int tachPin, int pwmPin){
    m_pi = pi;
    m_enablePin = enablePin;
    m_tachPin = tachPin;
    m_pwmPin = pwmPin;
    initializePins();
    
    m_speed = 0;
    m_running = false;
}

int Fan::initializePins() {
   int result = 0;
   // Set pin modes for fan I/O
   result = set_mode(m_pi, m_enablePin, PI_OUTPUT);
   if (result != 0) {
        std::cout << "Couldn't set mode on pin " << m_enablePin << ".\n";
        return result; 
   }
   
   result = set_mode(m_pi, m_tachPin, PI_INPUT);
   if (result != 0) {
        std::cout << "Couldn't set mode on pin " << m_tachPin << ".\n";
        return result; 
   }
  
  result = set_mode(m_pi, m_pwmPin, PI_OUTPUT);
   if (result != 0) {
        std::cout << "Couldn't set mode on pin " << m_pwmPin << ".\n";
        return result; 
   }
   
   return result;
}

void Fan::toggleFan() {
    if (m_running) {
        m_running = false;
    } else {
        m_running = true;
    }
}

bool Fan::isRunning() {
    return m_running;
}
