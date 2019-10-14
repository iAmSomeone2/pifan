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
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
}

#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>

// Boost libs
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <PiFanConfig.h>

#include "Fan.h"
#include "DataAccess.h"

using namespace boost;

const static std::string socketBaseDir = "/run/user/";
const static std::string socketAppendPath = "/pifand/";
const static std::string socketName = "pifand-socket";

volatile bool active = true;
int pulseCount = 0;

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

/**
 * Bundles constants along with environment variables to create the full directory path
 * for the daemon socket.
 * @return a path object representing the directory in which to put the socket
 */
filesystem::path getSocketPath() {
    // Create path based on user id
    int uid = getuid();
    std::ostringstream pathBuffer;

    pathBuffer << format("%s%u%s") % socketBaseDir % uid % socketAppendPath;

    filesystem::path socketPath(pathBuffer.str());

    // Check if path exists and create it if it does not
    if (!filesystem::exists(socketPath)) {
        try {
            filesystem::create_directories(socketPath);
        } catch (const filesystem::filesystem_error& error) {
            std::cerr << format("Couldn't create directory: %s\n") % socketPath.string();
            exit(-2);
        }
    }

    return socketPath;
}

/**
 * Set up and monitor socket for allowing external programs to change values in
 * the configuration file.
 */
void configSocket() {
    std::clog << "Setting up socket..." << std::endl;
    // Initialized to make compiler happy
    struct sockaddr_un addr = {
        AF_UNIX,
        "\0"
    };

    int fd, cl, rc;

    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket error" << std::endl;
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    // Get path for socket
    filesystem::path socketPath = getSocketPath();
    socketPath.append(socketName);

    // Set up and link socket file
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path)-1);
    unlink(socketPath.c_str());

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Bind error" << std::endl;
        exit(-1);
    }

    if (listen(fd, 5) == -1) {
        std::cerr << "Listen error" << std::endl;
        exit(-1);
    }

    while (true) {
        if ((cl = accept(fd, nullptr, nullptr)) == -1) {
            std::cerr << "accept error" << std::endl;
            continue;
        }

        // For now, just print whatever gets sent to the socket
        char buffer[128];
        while ( (rc = read(cl, buffer, sizeof(buffer))) > 0) {
            printf("Read %u bytes: %.*s\n", rc, rc, buffer);
        }

        if (rc == -1) {
            std::cerr << "Read error" << std::endl;
            exit(-1);
        } else if (rc == 0) {
            std::cout << "EOF" << std::endl;
            close(cl);
        }

        return;
    }
}

int main() {
    PiFan::DataAccess data = PiFan::DataAccess();

    int targetTemp = data.getTargetTemp();

    std::clog << "Target temp: " << targetTemp/1000 << "°C\n";
    std::clog << "Speed monitor enabled: " << data.monitoringEnabled() << "\n";

    // Set up socket control thread
    std::thread socketThread (configSocket);

    // Connect to pigpio daemon
    int rPi = pigpio_start(nullptr, nullptr);
    if (rPi < 0) {
        std::cerr << "Failed to connect to pigpio daemon.\nExiting...\n";
        exit(1);
    }
    
    std::clog << "Connected to pigpio daemon." << std::endl;

    // Create a Fan object to keep track of its operations
    Fan piFan = Fan(rPi, data.m_upperTempBuffer, data.m_lowerTempBuffer);

    // Set up a callback for the fan's tachometer pin.
    int speedCallback = 0;
    if (data.monitoringEnabled()) {
        speedCallback = callback(rPi, piFan.m_tachPin, RISING_EDGE, reinterpret_cast<CBFunc_t>(incrementPulse));
        switch (speedCallback) {
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
    }

    // Register interrupt handler
    signal(SIGINT, interruptHandler);
    signal(SIGTERM, interruptHandler);

    time_t startTime = 1;
    int fanSpeed = 0;

    // Main program loop
    while (active) {
        int currentTemp = data.getCurrentTemp();

        std::cout << "Current temp: " << (float)currentTemp/1000 << "°C\n";

        piFan.determineState(currentTemp, targetTemp);
        
        // Sleep for a specific time.
        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME_SECS));
        
        // Figure out how fast the fan is spinning.
        if (time(nullptr) % 2 == 0 && data.monitoringEnabled()) {
            fanSpeed = piFan.getFanSpeed(&pulseCount, startTime);
            startTime = time(nullptr);

            std::cout << "Current fan speed: " << fanSpeed << " RPM\n";
        }
    }

    std::clog << "Stopping PiFan daemon...\n";

    // Sync threads before exiting
    socketThread.join();
    
    if (piFan.isRunning()) {
        piFan.toggle();
    }
    if (data.monitoringEnabled()){
        callback_cancel(speedCallback);
    }
    pigpio_stop(rPi);
    return 0;
}

