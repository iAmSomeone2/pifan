//
// Created by Brenden Davidson on 10/9/19.
//

#include <iostream>
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

    std::string errors;

    bool parseSuccessful = reader->parse(
            m_configFilePath.c_str(), m_configFilePath.c_str() + m_configFilePath.size(),
            &m_configRoot, &errors);
    delete(reader);
    if (!parseSuccessful) {
        std::cerr << "Failed to parse configuration\n";
        return;
    }
}