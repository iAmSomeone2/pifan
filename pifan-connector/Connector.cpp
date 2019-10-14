//
// Created by Brenden Davidson on 10/13/19.
//

extern "C" {
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
}

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "include/Connector.h"

using namespace boost;

/**
 * The Connector class sets up and manages the connection to PiFan's socket.
 *
 * This class offers interfaces for sending data to the socket, receiving data from the socket,
 * and closing the socket connection.
 */
Connector::Connector() {
    // Start by determining the full socket path.
    int userId = getuid();
    std::ostringstream pathBuffer;

    pathBuffer << format("%s%u%s") % SOCKET_BASE_DIR % userId % SOCKET_APPEND_PATH;

    this->m_fullSocketPath.append(pathBuffer.str());
    this->m_fullSocketPath.append(SOCKET_NAME);

    // Set up connection to socket
    struct sockaddr_un addr = {
        AF_UNIX,
        "\0"
    };

    if ((this->m_socketDescriptor = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        std::cerr << format("Error with socket at %s\n") % this->m_fullSocketPath;
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, this->m_fullSocketPath.c_str(), sizeof(addr.sun_path)-1);

    // Attempt connection
    if(connect(this->m_socketDescriptor, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << format("Error connecting to socket at %s\n") % this->m_fullSocketPath;
        exit(-1);
    }
}

/**
 *  Sends a message comprising of a byte array through the socket.
 *
 * @param message byte array containing the message to be sent
 * @param msgLen length of the message in number of bytes
 * @return True if the message was received. False otherwise
 */
bool Connector::send(__int8_t *message, int msgLen) {
    // TODO: Finish implementation
    return false;
}

/**
 * Sends a message comprising of a string through the socket.
 *
 * Since this uses the C++ standard string, we don't need to specify the
 * the length of the data when calling send. We just pass the length of the
 * string to the write function.
 * @param message string containing the message to be sent
 * @return True if the message was received. False otherwise.
 */
bool Connector::send(std::string message) {
    // TODO: Finish implementation
    return false;
}

/**
 * Gets a message from the socket.
 *
 * @param buffer pointer to a buffer array where the message will be stored.
 * @param buffSize expected size of the message
 */
void Connector::recv(__int8_t *buffer, int buffSize) {
    memset(buffer, 0, sizeof(__int8_t) * buffSize);
    // TODO: Finish implementation
}

/**
 * This just wraps the default close command for the socket connection.
 *
 * The socket file descriptor is a private variable, so it can't be closed from outside of
 * this object.
 */
void Connector::closeConnection() {
    close(this->m_socketDescriptor);
}
