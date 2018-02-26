#include "UdpClient.h"

#if defined(_WIN32)
#    include "Ws2udpip.h"
using accept_reuse_t = int;
#else
#    include <unistd.h>
#    define INVALID_SOCKET -1
using accept_reuse_t = char;
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>

namespace CppSerialPort {

    void UdpClient::connect()
    {
        if (this->isConnected()) {
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): Cannot connect to new host when already connected (call disconnect() first)");
        }
        addrinfo *addressInfo{nullptr};
        addrinfo hints{};
        memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
        hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
        hints.ai_socktype = SOCK_DGRAM; //UDP
        auto returnStatus = getaddrinfo(
                this->hostName().c_str(), //IP Address or hostname
                toStdString(this->portNumber()).c_str(), //Service (HTTP, port, etc)
                &hints, //Use the hints specified above
                &addressInfo //Pointer to linked list to be filled in by getaddrinfo
        );
        if (returnStatus != 0) {
            freeaddrinfo(addressInfo);
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): getaddrinfo(const char *, const char *, constr addrinfo *, addrinfo **): error code " + toStdString(returnStatus) + " (" + gai_strerror(returnStatus) + ')');
        }
        auto socketDescriptor = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
        if (socketDescriptor == INVALID_SOCKET) {
            freeaddrinfo(addressInfo);
            auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): socket(int, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }
        this->setSocketDescriptor(socketDescriptor);

        accept_reuse_t acceptReuse{ 1 };
        auto reuseSocketResult = setsockopt(this->socketDescriptor(), SOL_SOCKET,  SO_REUSEADDR, &acceptReuse, sizeof(decltype(acceptReuse)));
        if (reuseSocketResult == -1) {
            freeaddrinfo(addressInfo);
            auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): setsockopt(int, int, int, const void *, socklen_t): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }

        freeaddrinfo(addressInfo);
        this->flushRx();

        auto tv = toTimeVal(static_cast<uint32_t>(this->readTimeout()));
        auto readTimeoutResult = setsockopt(this->socketDescriptor(), SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
        if (readTimeoutResult == -1) {
            auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): setsockopt(int, int, int, const void *, int) set read timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }

        tv = toTimeVal(static_cast<uint32_t>(this->writeTimeout()));
        auto writeTimeoutResult = setsockopt(this->socketDescriptor(), SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
        if (writeTimeoutResult == -1) {
            auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::UdpClient::connect(): setsockopt(int, int, int, const void *, int) set write timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }
    }

    UdpClient::UdpClient(const IPV4Address &ipAddress, uint16_t portNumber) :
            AbstractSocket(ipAddress, portNumber)
    {

    }

    UdpClient::UdpClient(const std::string &hostName, uint16_t portNumber) :
            AbstractSocket(hostName, portNumber)
    {

    }


} //namespace CppSerialPort
