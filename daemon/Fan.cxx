/*
 * Fan.cxx
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

#include "Fan.h"

Fan::Fan(int pi){
    m_pi = pi;
    initializePins();
    
    m_speed = 0;
    m_running = false;
}

/*
    Initializes all fan GPIO pins so that they are correctly configured and inactive.
*/
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
   
   // Make sure all pins are set to "pull-down" so that nothing is active.
   result = set_pull_up_down(m_pi, m_enablePin, PI_PUD_DOWN);
   if (result != 0) {
        std::cout << "Couldn't set resistor on pin " << m_enablePin << ".\n";
        return result;
   }
   
   result = set_pull_up_down(m_pi, m_tachPin, PI_PUD_DOWN);
   if (result != 0) {
        std::cout << "Couldn't set resistor on pin " << m_tachPin << ".\n";
        return result;
   }
   
   result = set_pull_up_down(m_pi, m_pwmPin, PI_PUD_DOWN);
   if (result != 0) {
        std::cout << "Couldn't set resistor on pin " << m_pwmPin << ".\n";
        return result;
   }
   
   return result;
}

/*
    Toggles the fan off or on depending on the current state.
    Returns 0 if successful.
*/
int Fan::toggle() {
    int result;
    if (m_running) {
        result = gpio_write(m_pi, m_enablePin, 0);
        m_running = false;
    } else {
        result = gpio_write(m_pi, m_enablePin, 1);
        m_running = true;
    }
    return result;
}

bool Fan::isRunning() {
    return m_running;
}
