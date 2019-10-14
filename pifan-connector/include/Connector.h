//
// Created by Brenden Davidson on 10/13/19.
//

#ifndef PIFAN_CONNECTOR_H
#define PIFAN_CONNECTOR_H

#include <iostream>

#include <boost/filesystem.hpp>

class Connector {
private:
    const std::string SOCKET_BASE_DIR = "/run/user/";
    const std::string SOCKET_APPEND_PATH = "/pifand/";
    const std::string SOCKET_NAME = "pifand-socket";

    boost::filesystem::path m_fullSocketPath;
    int m_socketDescriptor;
public:
    Connector();
    bool send(__int8_t message[], int msgLen);
    bool send(std::string message);
    void recv(__int8_t *buffer, int buffSize);
    void closeConnection();
};


#endif //PIFAN_CONNECTOR_H
