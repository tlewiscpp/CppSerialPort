#include <CppSerialPort/AbstractSocket.hpp>
#include <CppSerialPort/ErrorInformation.hpp>

#if defined(_WIN32)
#    include <ws2tcpip.h>
#    include <winsock2.h>
     using accept_reuse_t = char;

#else
#    include <unistd.h>
#    include <fcntl.h>
#    include <sys/ioctl.h>
#    define INVALID_SOCKET (-1)
     using accept_reuse_t = int;

#endif //defined(_WIN32)
#include <cstring>
#include <climits>
#include <iostream>
#include <include/CppSerialPort/AbstractSocket.hpp>

using NetworkErrorInformation::getLastError;
using NetworkErrorInformation::getErrorString;

namespace CppSerialPort {

const uint16_t AbstractSocket::MINIMUM_PORT_NUMBER{1024};
const uint16_t AbstractSocket::MAXIMUM_PORT_NUMBER{std::numeric_limits<uint16_t>::max()};

#define ABSTRACT_SOCKET_BUFFER_MAX 8192

AbstractSocket::AbstractSocket(const std::string &hostName, uint16_t portNumber) :
    IByteStream{},
    m_socketDescriptor{INVALID_SOCKET},
    m_hostName{hostName},
    m_portNumber{portNumber},
    m_readBuffer{""}
{
#if defined(_WIN32)
#   if defined(_WIN64)
    WSADATA wsaData{0, 0, 0, 0, nullptr, {}, {}};
#   else
    WSADATA wsaData{0, 0, {}, {}, 0, 0, nullptr};
#   endif //defined(_WIN64)

    memset(&wsaData, 0, sizeof(WSADATA));
    auto wsaStartupResult = WSAStartup(MAKEWORD(2, 0), &wsaData);
    if (wsaStartupResult != 0) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::AbstractSocket(const std::string &, uint16_t): WSAStartup failed: error code " + toStdString(wsaStartupResult) + " (" + getErrorString(wsaStartupResult) + ')');
    }
#endif //defined(_WIN32)
    if (portNumber < MINIMUM_PORT_NUMBER) {
        this->m_portNumber = 0;
        throw std::runtime_error("CppSerialPort::AbstractSocket::AbstractSocket(const std::string &, uint16_t): portNumber cannot be less than minimum value (" + toStdString(portNumber) + " < " + toStdString(MINIMUM_PORT_NUMBER) + ')');
    }
    IByteStream::setReadTimeout(DEFAULT_READ_TIMEOUT);
    IByteStream::setReadTimeout(DEFAULT_WRITE_TIMEOUT);
}

AbstractSocket::AbstractSocket(const IPV4Address &ipAddress, uint16_t portNumber) :
        AbstractSocket{ipAddress.toString(), portNumber}
{

}

bool AbstractSocket::isDisconnected() const {
    return false;
}

void AbstractSocket::connect(const std::string &hostName, uint16_t portNumber) {
    if (this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(const std::string &, uint16_t): Cannot connect to new host when already connected (call disconnect() first)");
    }
    this->setHostName(hostName);
    this->setPortNumber(portNumber);
    this->connect();
}

bool AbstractSocket::disconnect() {
#if defined(_WIN32)
    closesocket(this->m_socketDescriptor);
#else
    close(this->m_socketDescriptor);
#endif //defined(_WIN32)
    this->m_socketDescriptor = INVALID_SOCKET;
    return true;
}

bool AbstractSocket::isConnected() const {
    return this->m_socketDescriptor != INVALID_SOCKET;
}

ssize_t AbstractSocket::write(char c) {
    if (!this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::write(char): Cannot write on closed socket (call connect first)");
    }
    return this->write(&c, 1);
}

std::string AbstractSocket::portName() const {
    return '[' + this->m_hostName + ':' + toStdString(this->m_portNumber) + ']';
}

bool AbstractSocket::isOpen() const {
    return this->isConnected();
}

void AbstractSocket::openPort() {
    if (!this->isConnected()) {
        this->connect();
    }
}

void AbstractSocket::closePort() {
    this->disconnect();
}

void AbstractSocket::flushRx() {
    this->m_readBuffer.clear();
}

void AbstractSocket::flushTx() {

}

size_t AbstractSocket::available() {
    return this->m_readBuffer.size() + this->checkAvailable();
}

size_t AbstractSocket::rawRead(char *buffer, size_t max) {
    size_t returnSize{0};
    while (!this->m_readBuffer.empty()) {
        buffer[returnSize++] = this->m_readBuffer[0];
        this->m_readBuffer.popFront();
        if (returnSize >= max) {
            return returnSize;
        }
    }
    auto remainingMax = max - returnSize;
    auto result = this->doRead( (buffer + returnSize), remainingMax);
    if (result == -1) {
        auto errorCode = getLastError();
        if (errorCode != EAGAIN) {
            this->closePort();
            throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::rawRead(): The server hung up unexpectedly"};
        }
        return returnSize;
    } else if (result == 0) {
        this->closePort();
        throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::rawRead(): The server hung up unexpectedly"};
    } else {
        returnSize += result;
        return returnSize;
    }
}

char AbstractSocket::read(bool *readTimeout) {
    if (!this->m_readBuffer.empty()) {
        char returnValue{ this->m_readBuffer[0] };
        this->m_readBuffer.popFront();
        if (readTimeout) {
            *readTimeout = false;
        }
        return returnValue;
    }
    if (this->isDisconnected()) {
        this->closePort();
        throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::read(): The server hung up unexpectedly"};
    }

    //Use select() to wait for data to arrive
    //At socket, then read and return
    fd_set read_fds{0, 0, 0};
    fd_set write_fds{0, 0, 0};
    fd_set except_fds{0, 0, 0};
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(this->m_socketDescriptor, &read_fds);

    struct timeval timeout{0, 0};
    timeout.tv_sec = 0;
    timeout.tv_usec = (this->readTimeout() * 1000);
    static char readBuffer[ABSTRACT_SOCKET_BUFFER_MAX];
    memset(readBuffer, '\0', ABSTRACT_SOCKET_BUFFER_MAX);

    if (select(this->socketDescriptor() + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
        auto receiveResult = this->doRead(readBuffer, ABSTRACT_SOCKET_BUFFER_MAX - 1);
        if (receiveResult == -1) {
            auto errorCode = getLastError();
            if (errorCode != EAGAIN) {
                this->closePort();
                throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::read(): The server hung up unexpectedly"};
            }
            if (readTimeout) {
                *readTimeout = true;
            }
            return 0;
        } else if (receiveResult == 0) {
            this->closePort();
            throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::read(): The server hung up unexpectedly"};
        } else {
            for (int i = 0; i < receiveResult; i++) {
                this->m_readBuffer.append(readBuffer[i]);
            }
            char returnValue{this->m_readBuffer[0]};
            this->m_readBuffer.popFront();
            if (readTimeout) {
                *readTimeout = false;
            }
            return returnValue;
        }
    }
    if (readTimeout) {
        *readTimeout = true;
    }
    return 0;
}

ssize_t AbstractSocket::checkAvailable() {
#if defined(_WIN32)
    unsigned long bytesAvailable{0};
    auto result = ioctlsocket(this->m_socketDescriptor, FIONREAD, &bytesAvailable);
    if (result == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::checkAvailable(): ioctlsocket(SOCKET*, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    unsigned long bytesAvailable{0};
    auto result = ioctl(this->m_socketDescriptor, FIONREAD, &bytesAvailable);
    if (result == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::checkAvailable(): ioctl(int, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif //defined(_WIN32)
    return static_cast<ssize_t>(bytesAvailable);
}

ssize_t AbstractSocket::write(const char *bytes, size_t byteCount) {
    if (!this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::write(const char *, size_t): Cannot write on closed socket (call connect first)");
    }
    unsigned sentBytes{0};
    //Make sure all bytes are sent
    auto startTime = IByteStream::getEpoch();
    while (sentBytes < byteCount)  {
        auto sendResult = this->doWrite(bytes + sentBytes, byteCount - sentBytes);
        if (sendResult == -1) {
            auto errorCode = getLastError();
            if ( (errorCode == ENOTCONN) || (errorCode == EPIPE) || (errorCode == ECONNRESET) ) {
                this->closePort();
                throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::write(): The server hung up unexpectedly"};
            }
            throw std::runtime_error("CppSerialPort::AbstractSocket::write(const char *bytes, size_t): send(int, const void *, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
        }
        sentBytes += sendResult;
        if ( (getEpoch() - startTime) >= static_cast<unsigned int>(this->writeTimeout()) ) {
            break;
        }
    }
    return sentBytes;
}

timeval AbstractSocket::toTimeVal(uint32_t totalTimeout) {
    timeval tv{0, 0};
    tv.tv_sec = static_cast<long>(totalTimeout / 1000);
    tv.tv_usec = static_cast<long>((totalTimeout % 1000) * 1000);
    return tv;
}

void AbstractSocket::setPortNumber(uint16_t portNumber) {
    if (this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::setPortNumber(uint16_t): Cannot set port number when already connected (call disconnect() first)");
    }
    this->m_portNumber = portNumber;
}

void AbstractSocket::setHostName(const std::string &hostName) {
    if (this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::setHostName(const std::string &): Cannot set host name when already connected (call disconnect() first)");
    }
    this->m_hostName = hostName;
}

uint16_t AbstractSocket::portNumber() const {
    return this->m_portNumber;
}

std::string AbstractSocket::hostName() const {
    return this->m_hostName;
}

socket_t AbstractSocket::socketDescriptor() const {
    return this->m_socketDescriptor;
}

void AbstractSocket::setSocketDescriptor(socket_t socketDescriptor) {
    this->m_socketDescriptor = socketDescriptor;
}

int AbstractSocket::getAddressInfo(addrinfo *addressInfo) {
    //Get address info from inheriting class (UDP, TCP, raw socket, etc)
    auto hints = this->getAddressInfoHints();
    auto returnStatus = getaddrinfo(
            this->hostName().c_str(), //IP Address or hostname
            toStdString(this->portNumber()).c_str(), //Service (HTTP, port, etc)
            &hints, //Use the hints specified above
            &addressInfo //Pointer to linked list to be filled in by getaddrinfo
    );
    return returnStatus;
}

void AbstractSocket::connect() {
    if (this->isConnected()) {
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(): Cannot connect to new host when already connected (call disconnect() first)");
    }

    //Get address info from inheriting class (UDP, TCP, raw socket, etc)
    addrinfo *addressInfo{nullptr};
    auto returnStatus = this->getAddressInfo(addressInfo);

    if (returnStatus != 0) {
        freeaddrinfo(addressInfo);
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(): getaddrinfo(const char *, const char *, constr addrinfo *, addrinfo **): error code " + toStdString(returnStatus) + " (" + gai_strerror(returnStatus) + ')');
    }

    //Create a socket
    auto socketDescriptor = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
    if (socketDescriptor == INVALID_SOCKET) {
        freeaddrinfo(addressInfo);
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(): socket(int, int, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    this->m_socketDescriptor = socketDescriptor;

    //Let the kernel reuse the socket if I close it
    accept_reuse_t acceptReuse{1};
    auto reuseSocketResult = setsockopt(this->m_socketDescriptor, SOL_SOCKET,  SO_REUSEADDR, &acceptReuse, sizeof(decltype(acceptReuse)));
    if (reuseSocketResult == -1) {
        freeaddrinfo(addressInfo);
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(): Setting reuse of socket: setsockopt(int, int, int, const void *, socklen_t): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    try {
        this->doConnect(addressInfo);
    } catch (std::exception &e) {
        freeaddrinfo(addressInfo);
        std::rethrow_exception(std::current_exception());
    }

    this->setReadTimeout(this->readTimeout());
    this->setWriteTimeout(this->writeTimeout());

    //Free address info
    freeaddrinfo(addressInfo);
    this->flushRx();

}

void AbstractSocket::setReadTimeout(int timeout) {
    if (!this->isConnected()) {
        return IByteStream::setReadTimeout(timeout);
    }
    //TcpSocket read timeout
    auto tv = toTimeVal(static_cast<uint32_t>(this->readTimeout()));
    auto readTimeoutResult = setsockopt(this->m_socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
    if (readTimeoutResult == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::setReadTimeout(): Setting read timeout (" + toStdString(this->readTimeout()) + "): setsockopt(int, int, int, const void *, int) set read timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    return IByteStream::setReadTimeout(timeout);
}

void AbstractSocket::setWriteTimeout(int timeout) {
    if (!this->isConnected()) {
        return IByteStream::setWriteTimeout(timeout);
    }
    //TcpSocket write timeout
    auto tv = toTimeVal(static_cast<uint32_t>(this->writeTimeout()));
    auto writeTimeoutResult = setsockopt(this->m_socketDescriptor, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&tv), sizeof(struct timeval));
    if (writeTimeoutResult == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::connect(): Setting write timeout(" + toStdString(this->writeTimeout())  + "): setsockopt(int, int, int, const void *, int) set write timeout failed: error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    return IByteStream::setWriteTimeout(timeout);
}

void AbstractSocket::setBlockingFlag(bool blocking) {
#if defined(_WIN32)
    unsigned long mode{blocking ? 0UL : 1UL};
    auto result = ioctlsocket(this->socketDescriptor(), FIONBIO, &mode);
    if (result) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::setBlockingFlag(bool): Getting current socket flags: ioctlsocket(int, int, unsigned long): failed with error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    int currentFlags{fcntl(this->socketDescriptor(), F_GETFL, nullptr)};
    if (currentFlags == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::setBlockingFlag(bool): Getting current socket flags: fcntl(int, int, const void *): failed with error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }

    if (blocking) {
        currentFlags = (currentFlags & ~O_NONBLOCK);
    } else {
        currentFlags = (currentFlags | O_NONBLOCK);
    }
    auto result = fcntl(this->socketDescriptor(), F_SETFL, currentFlags);
    if (result == -1) {
        auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::AbstractSocket::setBlockingFlag(bool): Setting socket flags: fcntl(int, int, const void *): failed with error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif //defined(_WIN32)
}


AbstractSocket::~AbstractSocket() {
    this->closePort();
}


} //namespace CppSerialPort
