//
// Created by Brenden Davidson on 10/9/19.
//

#include <iostream>
#include <fstream>
#include <json/json.h>
#include "Configure.h"
using namespace PiFan;

/**
 * Sets initial object variables and reads config file into memory.
 */
Configure::Configure(std::string configFilePath) {
    m_configFilePath = configFilePath;
    loadConfig();
};

void Configure::loadConfig() {
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();

    // Open config file and read it into a string
    std::ifstream configFile;
    configFile.open(m_configFilePath);
    std::string configStr;
    if (configFile.is_open()) {
        std::cerr << "Failed to open config file\nMake sure that \"pifan.json\" is present at " << m_configFilePath << std::endl;
        return;
        // We may want to create a config file here if we can't find one
    } else {
        std::string buff;
        while (getline(configFile, buff)) {
            configStr += buff;
        }
        configFile.close();
    }


    std::string errors;

    bool parseSuccessful = reader->parse( configStr.c_str(), configStr.c_str() + configStr.size(),
            &m_configRoot, &errors);
    delete(reader);
    if (!parseSuccessful) {
        std::cerr << "Failed to parse configuration\n" << errors << std::endl;
        return;
    }
}