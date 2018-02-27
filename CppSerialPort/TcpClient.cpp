#include "TcpClient.h"

#if defined(_WIN32)
#    include "Ws2tcpip.h"
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

TcpClient::TcpClient(const IPV4Address &ipAddress, uint16_t portNumber) :
        AbstractSocket(ipAddress, portNumber)
{

}

TcpClient::TcpClient(const std::string &hostName, uint16_t portNumber) :
        AbstractSocket(hostName, portNumber)
{

}

ssize_t TcpClient::write(const char *bytes, size_t byteCount) {

    if (!this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::write(const char *, size_t): Cannot write on closed socket (call connect first)");
    }
    unsigned sentBytes{0};
    //Make sure all bytes are sent
    auto startTime = IByteStream::getEpoch();
    while (sentBytes < numberOfBytes)  {
        auto sendResult = send(this->m_socketDescriptor, bytes + sentBytes, numberOfBytes - sentBytes, 0);
        if (sendResult == -1) {
        auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::AbstractSocket::write(const char *bytes, size_t): send(int, const void *, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }
        sentBytes += sendResult;
        if ( (getEpoch() - startTime) >= static_cast<unsigned int>(this->writeTimeout()) ) {
            break;
        }
    }
    return sentBytes;
}

void TcpClient::connect()
{
    if (this->isConnected()) {
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): Cannot connect to new host when already connected (call disconnect() first)");
    }
    addrinfo *addressInfo{nullptr};
    addrinfo hints{};
    memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM; //TCP
    auto returnStatus = getaddrinfo(
            this->hostName().c_str(), //IP Address or hostname
            toStdString(this->portNumber()).c_str(), //Service (HTTP, port, etc)
            &hints, //Use the hints specified above
            &addressInfo //Pointer to linked list to be filled in by getaddrinfo
    );
    if (returnStatus != 0) {
        freeaddrinfo(addressInfo);
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): getaddrinfo(const char *, const char *, constr addrinfo *, addrinfo **): error code " + toStdString(returnStatus) + " (" + gai_strerror(returnStatus) + ')');
    }
    auto socketDescriptor = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
    if (socketDescriptor == INVALID_SOCKET) {
        freeaddrinfo(addressInfo);
		auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::TcpClient::connect(): socket(int, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    this->setSocketDescriptor(socketDescriptor);

	accept_reuse_t acceptReuse{1};
    auto reuseSocketResult = setsockopt(this->socketDescriptor(), SOL_SOCKET,  SO_REUSEADDR, &acceptReuse, sizeof(decltype(acceptReuse)));
    if (reuseSocketResult == -1) {
        freeaddrinfo(addressInfo);
		auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): Setting reuse of socket: setsockopt(int, int, int, const void *, socklen_t): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }

    auto connectResult = ::connect(this->socketDescriptor(), addressInfo->ai_addr, addressInfo->ai_addrlen);
    if (connectResult == -1) {
        freeaddrinfo(addressInfo);
		auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): connect(int, const sockaddr *addr, socklen_t): error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
    }
    freeaddrinfo(addressInfo);
    this->flushRx();

    auto tv = toTimeVal(static_cast<uint32_t>(this->readTimeout()));
    auto readTimeoutResult = setsockopt(this->socketDescriptor(), SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
    if (readTimeoutResult == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::TcpClient::connect(): Setting read timeout (" + toStdString(this->readTimeout()) + "): setsockopt(int, int, int, const void *, int) set read timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }

    tv = toTimeVal(static_cast<uint32_t>(this->writeTimeout()));
    auto writeTimeoutResult = setsockopt(this->socketDescriptor(), SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
    if (writeTimeoutResult == -1) {
        auto errorCode = getLastError();

        throw std::runtime_error("CppSerialPort::TcpClient::connect(): Setting write timeout(" + toStdString(this->writeTimeout())  + "): setsockopt(int, int, int, const void *, int) set write timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
}


} //namespace CppSerialPort
