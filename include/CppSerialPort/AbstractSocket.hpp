#ifndef CPPSERIALPORT_ABSTRACTSOCKET_HPP
#define CPPSERIALPORT_ABSTRACTSOCKET_HPP

#if defined(_WIN32)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#    include <windows.h>
typedef SOCKET socket_t;
#else
#    include <sys/socket.h>
#    include <netinet/in.h>
#    include <netdb.h>
typedef int socket_t;
#endif //defined(_WIN32)

#include <sys/types.h>
#include <memory>
#include <string>
#include "IByteStream.hpp"
#include "IPV4Address.hpp"

namespace CppSerialPort {

    class SocketDisconnectedException : public std::runtime_error
    {
    public:
        explicit inline SocketDisconnectedException(const std::string &portName, const std::string &what) :
            std::runtime_error{what},
            m_portName{portName}
        {

        }
        SocketDisconnectedException(const SocketDisconnectedException &) = default;
        SocketDisconnectedException(SocketDisconnectedException &&) = default;
        SocketDisconnectedException &operator=(const SocketDisconnectedException &) = default;
        SocketDisconnectedException &operator=(SocketDisconnectedException &&) = default;
        ~SocketDisconnectedException() override = default;

        inline std::string portName() const {
            return this->m_portName;
        }

        inline void setPortName(const std::string &portName) {
            this->m_portName = portName;
        }

    private:
        std::string m_portName;
    };


    class AbstractSocket : public IByteStream
    {
    public:
        AbstractSocket(const std::string &hostName, uint16_t portNumber);
        AbstractSocket(const IPV4Address &ipAddress, uint16_t portNumber);
        ~AbstractSocket() override;

        ssize_t write(const char *bytes, size_t byteCount) override;
        char read(bool *readTimeout) override;
        size_t rawRead(char *buffer, size_t max);
        ssize_t write(char i) override;
        std::string portName() const override;
        bool isOpen() const override;
        void openPort() override;
        void closePort() override;
        void flushRx() override;
        void flushTx() override;
        size_t available() override;

        void connect(const std::string &hostName, uint16_t portNumber);
        void connect();
        bool disconnect();
        bool isConnected() const;
        void setPortNumber(uint16_t portNumber);
        void setHostName(const std::string &hostName);
        uint16_t portNumber() const;
        std::string hostName() const;

        static const uint16_t MINIMUM_PORT_NUMBER;
        static const uint16_t MAXIMUM_PORT_NUMBER;
private:
        socket_t m_socketDescriptor;
        std::string m_hostName;
        uint16_t m_portNumber;
        ByteArray m_readBuffer;

protected:
        virtual ssize_t doRead(char *buffer, size_t bufferMax) = 0;
        virtual ssize_t doWrite(const char *bytes, size_t numberOfBytes) = 0;
        virtual void doConnect(addrinfo *addressInfo) = 0;
        virtual addrinfo getAddressInfoHints() = 0;

        static timeval toTimeVal(uint32_t totalTimeout);
        bool isDisconnected() const;
        socket_t socketDescriptor() const;
        void setSocketDescriptor(socket_t socketDescriptor);
        ssize_t checkAvailable();

        void setBlockingFlag(bool blocking);
};

} //namespace CppSerialPort


#endif //CPPSERIALPORT_ABSTRACTSOCKET_HPP
