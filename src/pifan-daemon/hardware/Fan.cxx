/*
 * Fan.cxx
 * 
 * Copyright 2019 Brenden Davidson <davidson.brenden15@gmail.com>
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
#include <ctime>

#include "Fan.h"

/**
 * Fan object used for connecting to and managing the cpu fan for the Raspberry Pi.
 * @param pi pi to connect to with pigpio calls.
 */
Fan::Fan(int pi, int upperBuffer, int lowerBuffer){
    m_pi = pi;
    initializePins();
    
    m_speed = 0;
    m_running = false;
    m_upperBuffer = upperBuffer;
    m_lowerBuffer = lowerBuffer;
}

/**
 * Sets up all relevant GPIO pins so that they are configured and inactive.
 * @return 0 if successful, otherwise a pigpio mode failure value.
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
   
   // Send the off signal to the fan enable pin so we know it's off
   result = gpio_write(m_pi, m_enablePin, 0);
   
   return result;
}

/**
 * Sets the state of the fan based on current and target temperatures.
 * @param currentTemp
 * @param targetTemp
 * @return 0 if the operation was successful, otherwise a pigpio mode failure value.
 */
int Fan::determineState(int currentTemp, int targetTemp) {
    int result = 0;
    if ((currentTemp > (targetTemp + m_upperBuffer) ) && !m_running) {
        // Turn on the fan.
        result = toggle();
    } else if ((currentTemp <= (targetTemp - m_lowerBuffer)) && m_running) {
        // Turn off the fan
        result = toggle();
    }

    return result;
}

/**
 * Toggles the fan's state.
 * @return 0 if successful, otherwise a pigpio mode failure value.
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

/**
 * Determines the fan's speed based on the number of pulses its tachometer has sent
 * since the last call to getFanSpeed()
 * @param *pulseCount pointer to number of pulses since the last call
 * @param initTime time when tracking started.
 * @return the fan's speed in RPM 
 */
int Fan::getFanSpeed(int *pulseCount, time_t initTime) {
    time_t stopTime = time(nullptr);
    
    int secs = stopTime - initTime;
    int rpm = (*pulseCount/secs) * 60;
    
    *pulseCount = 0;
    return rpm;
}

/**
 * Gets current state of the fan.
 * @return value of this.m_running.
 */
bool Fan::isRunning() {
    return m_running;
}
