#ifndef CPPSERIALPORT_TCPCLIENT_HPP
#define CPPSERIALPORT_TCPCLIENT_HPP

#include "AbstractSocket.hpp"

namespace CppSerialPort {

class TcpClient : public AbstractSocket {
public:
    TcpClient(const std::string &hostName, uint16_t portNumber);
    TcpClient(const IPV4Address &ipAddress, uint16_t portNumber);
    ~TcpClient() override = default;
protected:
    ssize_t doWrite(const char *bytes, size_t byteCount) override;
    ssize_t doRead(char *buffer, size_t bufferMax) override;
    void doConnect(addrinfo *addressInfo) override;
    addrinfo getAddressInfoHints() override;
};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_TCPCLIENT_HPP