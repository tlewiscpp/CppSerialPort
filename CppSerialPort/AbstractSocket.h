#ifndef CPPSERIALPORT_ABSTRACTSOCKET_H
#define CPPSERIALPORT_ABSTRACTSOCKET_H

#include <string>
#if defined(_WIN32)
#    include "WinSock2.h"
#    include "Windows.h"
typedef SOCKET socket_t;
#else
#    include <sys/socket.h>
#    include <netinet/in.h>
#    include <netdb.h>
typedef int socket_t;
#endif //defined(_WIN32)


#include <sys/types.h>
#include <memory>
#include "IByteStream.h"
#include "IPV4Address.h"

namespace CppSerialPort {

    class SocketDisconnectedException : public std::runtime_error
    {
    public:
        explicit inline SocketDisconnectedException(const std::string &portName, const std::string &what) :
                std::runtime_error{what},
                m_portName{portName}
        {

        }

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

        char read(bool *readTimeout) override;
        ssize_t write(char i) override;
        ssize_t write(const char *bytes, size_t numberOfBytes) override;
        std::string portName() const override;
        bool isOpen() const override;
        void openPort() override;
        void closePort() override;
        void flushRx() override;
        void flushTx() override;
        void putBack(char c) override;

        void connect(const std::string &hostName, uint16_t portNumber);
        virtual void connect() = 0;
        bool disconnect();
        bool isConnected() const;
        void setPortNumber(uint16_t portNumber);
        void setHostName(const std::string &hostName);
        uint16_t portNumber() const;
        std::string hostName() const;
private:
        socket_t m_socketDescriptor;
        std::string m_hostName;
        uint16_t m_portNumber;
        ByteArray m_readBuffer;
    protected:
        static timeval toTimeVal(uint32_t totalTimeout);
        static std::string getErrorString(int errorCode);
        static int getLastError();
        bool isDisconnected() const;
        socket_t socketDescriptor() const;
        void setSocketDescriptor(socket_t socketDescriptor);

    };

} //namespace CppSerialPor


#endif //CPPSERIALPORT_ABSTRACTSOCKET_H
