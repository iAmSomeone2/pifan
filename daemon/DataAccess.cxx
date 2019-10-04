//
// Created by Brenden Davidson on 10/3/19.
//

#include "DataAccess.h"
#include <fstream>
#include <iostream>
#include <string>

#include <json/json.h>

DataAccess::DataAccess(){
    parseConfigFile();
};

/**
 * Reads through this program's configuration file, and set's the appropriate
 * variables.
 */
void DataAccess::parseConfigFile() {
    std::string config;
    std::ifstream configFile;
    configFile.open(m_configFilePath);
    if(!configFile.is_open()) {
        std::cerr << "Failed to open config file.\n";
        return;
    } else {
        std::string buff;
        while (getline(configFile, buff)) {
            config += buff;
        }
        configFile.close();
    }

    Json::Value root; // Contains the root of the file after parsing
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();

    std::string errors;

    bool parseSuccessful = reader->parse(config.c_str(), config.c_str() + config.size(), &root, &errors);
    delete(reader);
    if (!parseSuccessful) {
        std::cerr << "Failed to parse configuration\n";
        return;
    }

    m_targetTemp = (root.get("targetTemp", m_defaultTargetTemp).asInt()) * 1000;
}

/**
 * Retrieves the target temperature from the configuration file.
 * To get the temperature in °C, divide the return value by 1000;
 * @return target CPU temperature.
 */
int DataAccess::getTargetTemp() {
    return m_targetTemp;
}

/**
 * Retrieves the current system temperature from the Raspberry Pi.
 * The value is stored in a normal text file, so we can read it from
 * there.
 * To get the temperature in °C, divide the return value by 1000;
 * @return current CPU temperature.
 */
int DataAccess::getCurrentTemp() {
    std::ifstream thermalFile;
    thermalFile.open(m_temperatureFilePath, std::ios::in);

    std::string tempLine;
    if (thermalFile.is_open()) {
        if (!getline(thermalFile, tempLine)) {
            std::cerr << "Temperature file is empty.\n";
        }

        thermalFile.close();
    }

    // Convert from text to int
    int temp = atoi(tempLine.c_str());
    return temp;
}
