#include <CppSerialPort/TcpSocket.hpp>
#include <CppSerialPort/ErrorInformation.hpp>

#if defined(_WIN32)
#    include <ws2tcpip.h>
     using getsockopt_t = char;
#else
#    include <unistd.h>
#    include <fcntl.h>
     using getsockopt_t = int;
#endif //defined(_WIN32)

#include <cstring>
#include <climits>
#include <iostream>

using NetworkErrorInformation::getLastError;
using NetworkErrorInformation::getErrorString;

namespace CppSerialPort {

#define TCP_CLIENT_BUFFER_MAX 8192

TcpSocket::TcpSocket(const IPV4Address &ipAddress, uint16_t portNumber) :
        AbstractSocket(ipAddress, portNumber)
{

}

TcpSocket::TcpSocket(const std::string &hostName, uint16_t portNumber) :
        AbstractSocket(hostName, portNumber)
{

}

void TcpSocket::doConnect() {
    fd_set fileDescriptorSet{};
    struct timeval timeout{3, 0}; //3 seconds, 0 milliseconds

    FD_ZERO(&fileDescriptorSet);
    FD_SET(this->socketDescriptor(), &fileDescriptorSet);

    this->setBlockingFlag(false); //Set socket to non-blocking mode

    auto connectResult = ::connect(this->socketDescriptor(), this->addressInfo()->ai_addr, static_cast<int>(this->addressInfo()->ai_addrlen));
    /*
    if (connectResult == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::TcpSocket::doConnect(): doConnect(addrinfo *): connect(int, sockaddr *, size_t) returned error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
    }
    */
    (void)connectResult;

    //Windows will return 1 if the connection was successful, 0 otherwise, and will always return 0 for socketError, because logic
    if (select(static_cast<int>(this->socketDescriptor() + 1), nullptr, &fileDescriptorSet, nullptr, &timeout) == 1) {
#if !defined(_WIN32)
        getsockopt_t socketError{};
        socklen_t socketErrorLength{sizeof(socketError)};

        getsockopt(this->socketDescriptor(), SOL_SOCKET, SO_ERROR, &socketError, &socketErrorLength);

        if (socketError != 0) {
            auto errorCode = getLastError();
            throw std::runtime_error("CppSerialPort::TcpSocket::doConnect(): doConnect(addrinfo *): connect(int, sockaddr *, size_t) failed with error code " + toStdString(errorCode) +  " (" + getErrorString(errorCode) + ')');
        }
#endif //!defined(_WIN32)
    } else {
        throw std::runtime_error("CppSerialPort::TcpSocket::doConnect(): doConnect(addrinfo *): connect(int, sockaddr *, size_t) did not complete in the specified time frame");
    }
    this->setBlockingFlag(true); //Return socket to blocking mode
}

addrinfo TcpSocket::getAddressInfoHints() {
    addrinfo hints{0, 0, 0, 0, 0, nullptr, nullptr, nullptr};
    memset(reinterpret_cast<void *>(&hints), 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE; //Fill in my IP for me
    return hints;
}


ssize_t TcpSocket::doWrite(const char *bytes, size_t byteCount) {
    return send(this->socketDescriptor(), bytes, byteCount, 0);
}

ssize_t TcpSocket::doRead(char *buffer, size_t bufferMax) {
    return recv(this->socketDescriptor(), buffer, bufferMax, 0);
}


} //namespace CppSerialPort
