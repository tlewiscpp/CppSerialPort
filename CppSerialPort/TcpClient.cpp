#include "TcpClient.h"

#if defined(_WIN32)
#    include "Ws2udpip.h"
#else
#    include <unistd.h>
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
    auto connectResult = ::connect(this->socketDescriptor(), addressInfo->ai_addr, addressInfo->ai_addrlen);
    if (connectResult == -1) {
        freeaddrinfo(addressInfo);
        auto errorCode = this->getLastError();
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): connect(int, const sockaddr *addr, socklen_t): error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
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
