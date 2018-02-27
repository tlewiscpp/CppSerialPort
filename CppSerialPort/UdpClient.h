#ifndef CPPSERIALPORT_UDPCLIENT_H
#define CPPSERIALPORT_UDPCLIENT_H

#include "AbstractSocket.h"

namespace CppSerialPort {

class UdpClient : public AbstractSocket {
public:
    UdpClient(const std::string &hostName, uint16_t portNumber);
    UdpClient(const IPV4Address &ipAddress, uint16_t portNumber);

protected:
    ssize_t doWrite(const char *bytes, size_t byteCount) override;
    ssize_t doRead(char *buffer, size_t bufferMax) override;
    void doConnect(addrinfo *addressInfo) override;
    addrinfo getAddressInfoHints() override;

private:
    struct sockaddr m_address;
};

} //namespace CppSerialPort

#endif //CPPUDP_UDPCLIENT_H
