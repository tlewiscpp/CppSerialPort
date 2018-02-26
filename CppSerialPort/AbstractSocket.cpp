#include "AbstractSocket.h"

#if defined(_WIN32)
#    include "Ws2tcpip.h"
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

#define MINIMUM_PORT_NUMBER 1024
#define TCP_CLIENT_BUFFER_MAX 8192

AbstractSocket::AbstractSocket(const std::string &hostName, uint16_t portNumber) :
        m_socketDescriptor{INVALID_SOCKET},
        m_hostName{hostName},
        m_portNumber{portNumber},
        m_readBuffer{""}
    {
#if defined(_WIN32)
        WSADATA wsaData{};
	// if this doesn't work
	//WSAData wsaData; // then try this instead
	// MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:
	
	auto wsaStartupResult = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (wsaStartupResult != 0) {
		throw std::runtime_error("CppSerialPort::AbstractSocket::AbstractSocket(const std::string &, uint16_t): WSAStartup failed: error code " + toStdString(wsaStartupResult) + " (" + this->getErrorString(wsaStartupResult) + ')');
	}
#endif //defined(_WIN32)
        if (portNumber < MINIMUM_PORT_NUMBER) {
            this->m_portNumber = 0;
            throw std::runtime_error("CppSerialPort::AbstractSocket::AbstractSocket(const std::string &, uint16_t): portNumber cannot be less than minimum value (" + toStdString(portNumber) + " < " + toStdString(MINIMUM_PORT_NUMBER) + ')');
        }
        this->setReadTimeout(DEFAULT_READ_TIMEOUT);
    }

    AbstractSocket::AbstractSocket(const IPV4Address &ipAddress, uint16_t portNumber) :
            AbstractSocket{ipAddress.toString(), portNumber}
    {

    }

    bool AbstractSocket::isDisconnected() const {
        return false;
    }

    int AbstractSocket::getLastError()
    {
#if defined(_WIN32)
        return WSAGetLastError();
#else
        return errno;
#endif //defined(_WIN32)
    }

    std::string AbstractSocket::getErrorString(int errorCode)
    {
        char errorString[PATH_MAX];
        memset(errorString, '\0', PATH_MAX);
#if defined(_WIN32)
        wchar_t *wideErrorString{ nullptr };
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&wideErrorString),
		0,
		nullptr
	);
	//size_t converted{ 0 };
	//auto conversionResult = wcstombs_s(&converted, errorString, PATH_MAX, wideErrorString, PATH_MAX);
	//(void)conversionResult;

    (void)wcstombs(errorString, wideErrorString, PATH_MAX);
	LocalFree(wideErrorString);
    return stripLineEndings(errorString);
#else
        auto strerrorCode = strerror_r(errorCode, errorString, PATH_MAX);
        if (strerrorCode == nullptr) {
            std::cerr << "strerror_r(int, char *, int): error occurred" << std::endl;
            return "";
        }
        return stripLineEndings(strerrorCode);
#endif //defined(_WIN32)
    }

    void AbstractSocket::connect(const std::string &hostName, uint16_t portNumber)
    {
        if (this->isConnected()) {
            throw std::runtime_error("CppSerialPort::AbstractSocket::connect(const std::string &, uint16_t): Cannot connect to new host when already connected (call disconnect() first)");
        }
        this->setHostName(hostName);
        this->setPortNumber(portNumber);
        this->connect();
    }

    bool AbstractSocket::disconnect()
    {
#if defined(_WIN32)
        closesocket(this->m_socketDescriptor);
#else
        close(this->m_socketDescriptor);
#endif //defined(_WIN32)
        this->m_socketDescriptor = INVALID_SOCKET;
        return true;
    }

    bool AbstractSocket::isConnected() const
    {
        return this->m_socketDescriptor != INVALID_SOCKET;
    }

    char AbstractSocket::read(bool *readTimeout)
    {
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
        static char readBuffer[TCP_CLIENT_BUFFER_MAX];
        memset(readBuffer, '\0', TCP_CLIENT_BUFFER_MAX);

        if (select(this->m_socketDescriptor + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
            auto receiveResult = recv(this->m_socketDescriptor, readBuffer, TCP_CLIENT_BUFFER_MAX - 1, 0);
            if (receiveResult == -1) {
                auto errorCode = getLastError();
                if (errorCode != EAGAIN) {
                    throw std::runtime_error("CppSerialPort::AbstractSocket::read(): recv(int, void *, size_t, int): error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
                }
                if (readTimeout) {
                    *readTimeout = true;
                }
                return 0;
            } else if (receiveResult == 0) {
                this->closePort();
                if (this->isDisconnected()) {
                    this->closePort();
                    throw SocketDisconnectedException{this->portName(), "CppSerialPort::AbstractSocket::read(): The server hung up unexpectedly"};
                }
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

    ssize_t AbstractSocket::write(char c)
    {
        if (!this->isConnected()) {
            throw std::runtime_error("CppSerialPort::AbstractSocket::write(char): Cannot write on closed socket (call connect first)");
        }
        return this->write(&c, 1);
    }

    ssize_t AbstractSocket::write(const char *bytes, size_t numberOfBytes)
    {
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

    std::string AbstractSocket::portName() const
    {
        return '[' + this->m_hostName + ':' + toStdString(this->m_portNumber) + ']';
    }

    bool AbstractSocket::isOpen() const
    {
        return this->isConnected();
    }

    void AbstractSocket::openPort()
    {
        if (!this->isConnected()) {
            this->connect();
        }
    }

    void AbstractSocket::closePort()
    {
        if (this->isConnected()) {
            this->disconnect();
        }
    }

    void AbstractSocket::flushRx()
    {
        this->m_readBuffer.clear();
    }

    void AbstractSocket::flushTx()
    {

    }

    void AbstractSocket::putBack(char c)
    {
        this->m_readBuffer = (c + this->m_readBuffer);
    }

    timeval AbstractSocket::toTimeVal(uint32_t totalTimeout)
    {
        timeval tv{};
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


    AbstractSocket::~AbstractSocket()
    {
        if (this->isConnected()) {
            this->disconnect();
        }
    }


} //namespace CppSerialPort
