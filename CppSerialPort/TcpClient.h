#ifndef CPPSERIALPORT_TCPCLIENT_H
#define CPPSERIALPORT_TCPCLIENT_H

#include "AbstractSocket.h"

namespace CppSerialPort {

class TcpClient : public AbstractSocket {
public:
    TcpClient(const std::string &hostName, uint16_t portNumber);
    TcpClient(const IPV4Address &ipAddress, uint16_t portNumber);
    void connect() override;
};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_TCPCLIENT_H