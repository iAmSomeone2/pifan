//
// Created by bdavidson on 10/3/19.
//

#include "DataAccess.h"
#include <fstream>
#include <iostream>

DataAccess::DataAccess() = default;

/*
 * Retrieves the target temperature from the configuration file.
 */
int DataAccess::getTargetTemp() {
    return 50;
}

/*
 * Retrieves the current system temperature from its file.
 * To get the temperature in °C, divide the return value by 1000;
 */
int DataAccess::getCurrentTemp() {
    std::ifstream thermalFile;
    thermalFile.open(currentTempFile, std::ios::in);

    std::string tempLine;
    if (thermalFile.is_open()) {


        if (!getline(thermalFile, tempLine)) {
            std::cout << "Error: Temperature file is empty.\n";
        }

        thermalFile.close();
    }

    // Convert from text to int
    return  std::stoi(tempLine);
}