#ifndef CPPSERIALPORT_UDPCLIENT_HPP
#define CPPSERIALPORT_UDPCLIENT_HPP

#include "AbstractSocket.hpp"

namespace CppSerialPort {

class UdpSocket : public AbstractSocket {
public:
    UdpSocket(const std::string &hostName, uint16_t portNumber);
    UdpSocket(const IPV4Address &ipAddress, uint16_t portNumber);
    ~UdpSocket() override = default;
protected:
    ssize_t doWrite(const char *bytes, size_t byteCount) override;
    ssize_t doRead(char *buffer, size_t bufferMax) override;
    void doConnect() override;
    addrinfo getAddressInfoHints() override;
};

} //namespace CppSerialPort

#endif //CPPUDP_UDPCLIENT_HPP
