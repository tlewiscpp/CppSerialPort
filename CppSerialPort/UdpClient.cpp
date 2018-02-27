#include "UdpClient.h"

#if defined(_WIN32)
#    include "Ws2udpip.h"
using accept_reuse_t = char;
#else
#    include <unistd.h>
#    define INVALID_SOCKET -1
using accept_reuse_t = int;
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>

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
    addrinfo hints{};
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
    struct sockaddr from{};
    socklen_t fromLength{};
    return recvfrom(this->socketDescriptor(), buffer, bufferMax, 0, &from, &fromLength);
}


} //namespace CppSerialPort
