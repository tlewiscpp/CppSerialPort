#include "UdpClient.hpp"

#if defined(_WIN32)
#    include "Ws2tcpip.h"
#else
#    include <unistd.h>
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>
#include "ErrorInformation.hpp"

using NetworkErrorInformation::getLastError;
using NetworkErrorInformation::getErrorString;

namespace CppSerialPort {

UdpClient::UdpClient(const IPV4Address &ipAddress, uint16_t portNumber) :
        AbstractSocket(ipAddress, portNumber)
{

}

UdpClient::UdpClient(const std::string &hostName, uint16_t portNumber) :
        AbstractSocket(hostName, portNumber)
{

}

void UdpClient::doConnect(addrinfo *addressInfo) {
    //No connect needed in UDP

    //Save addressInfo for later use with sendto and recvfrom
    this->m_address = *addressInfo->ai_addr;
}

addrinfo UdpClient::getAddressInfoHints() {
    addrinfo hints{0, 0, 0, 0, 0, nullptr, nullptr, nullptr};
    memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
    hints.ai_socktype = SOCK_DGRAM; //UDP
    hints.ai_flags = AI_PASSIVE; //Fill in my IP for me
    return hints;
}

ssize_t UdpClient::doWrite(const char *bytes, size_t byteCount) {
    return sendto(this->socketDescriptor(), bytes, byteCount, 0, &this->m_address, sizeof(this->m_address));
}

ssize_t UdpClient::doRead(char *buffer, size_t bufferMax) {
    return recvfrom(this->socketDescriptor(), buffer, bufferMax, 0, nullptr, nullptr);
}


} //namespace CppSerialPort
