#include "TcpClient.h"

#if defined(_WIN32)
#    include "Ws2udpip.h"
#else
#    include <unistd.h>
#    include <fcntl.h>
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>

namespace CppSerialPort {

#define TCP_CLIENT_BUFFER_MAX 8192

TcpClient::TcpClient(const IPV4Address &ipAddress, uint16_t portNumber) :
        AbstractSocket(ipAddress, portNumber)
{

}

TcpClient::TcpClient(const std::string &hostName, uint16_t portNumber) :
        AbstractSocket(hostName, portNumber)
{

}

void TcpClient::doConnect(addrinfo *addressInfo) {
    fcntl(this->socketDescriptor(), F_SETFL, O_NONBLOCK);
    auto connectResult = ::connect(this->socketDescriptor(), addressInfo->ai_addr, addressInfo->ai_addrlen);

    fd_set fdset{};
    struct timeval connectTimeout{};
    connectTimeout.tv_sec = 3; //3 second timeout
    connectTimeout.tv_usec = 0; //0 extra microseconds
    FD_ZERO(&fdset);
    FD_SET(this->socketDescriptor(), &fdset);

    if (select(this->socketDescriptor() + 1, nullptr, &fdset, nullptr, &connectTimeout) == 1) {
        int socketError{};
        socklen_t socketErrorLength{sizeof(socketError)};
        auto getSockOptResult = getsockopt(this->socketDescriptor(), SOL_SOCKET, SO_ERROR, &socketError, &socketErrorLength);
        if (getSockOptResult == -1) {

            auto errorCode = this->getLastError();
            throw std::runtime_error("CppSerialPort::TcpClient::doConnect(): doConnect(addrinfo *): error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
        }
        if (socketError != 0) {
            errno = socketError;
        } else {
            connectResult = 0;
        }
    } else {
        connectResult = -1;
    }


    if (connectResult == -1) {
        auto errorCode = this->getLastError();
        throw std::runtime_error("CppSerialPort::TcpClient::doConnect(): doConnect(addrinfo *): error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
    }
}

addrinfo TcpClient::getAddressInfoHints() {
    addrinfo hints{};
    memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE; //Fill in my IP for me
    return hints;
}


ssize_t TcpClient::doWrite(const char *bytes, size_t byteCount) {
    return send(this->socketDescriptor(), bytes, byteCount, 0);
}

ssize_t TcpClient::doRead(char *buffer, size_t bufferMax) {
    return recv(this->socketDescriptor(), buffer, bufferMax, 0);
}


} //namespace CppSerialPort
