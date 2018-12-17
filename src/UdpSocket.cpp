#include <CppSerialPort/UdpSocket.hpp>
#include <CppSerialPort/ErrorInformation.hpp>

#if defined(_WIN32)
#    include <ws2tcpip.h>
#else
#    include <unistd.h>
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>

using NetworkErrorInformation::getLastError;
using NetworkErrorInformation::getErrorString;

namespace CppSerialPort {

UdpSocket::UdpSocket(const IPV4Address &ipAddress, uint16_t portNumber) :
    AbstractSocket{ipAddress, portNumber}
{

}

UdpSocket::UdpSocket(const std::string &hostName, uint16_t portNumber) :
    AbstractSocket{hostName, portNumber}
{

}

void UdpSocket::doConnect() {

}

addrinfo UdpSocket::getAddressInfoHints() {
    addrinfo hints{0, 0, 0, 0, 0, nullptr, nullptr, nullptr};
    memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
    hints.ai_socktype = SOCK_DGRAM; //UDP
    hints.ai_flags = AI_PASSIVE; //Fill in my IP for me
    return hints;
}

ssize_t UdpSocket::doWrite(const char *bytes, size_t byteCount) {
    return sendto(this->socketDescriptor(), bytes, static_cast<int>(byteCount), 0, this->addressInfo()->ai_addr, sizeof(this->addressInfo()->ai_addr));
}

ssize_t UdpSocket::doRead(char *buffer, size_t bufferMax) {
    return recvfrom(this->socketDescriptor(), buffer, static_cast<int>(bufferMax), 0, nullptr, nullptr);
}


} //namespace CppSerialPort
