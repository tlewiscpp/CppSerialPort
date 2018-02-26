#ifndef CPPSERIALPORT_UDPCLIENT_H
#define CPPSERIALPORT_UDPCLIENT_H

#include "AbstractSocket.h"

namespace CppSerialPort {

    class UdpClient : public AbstractSocket {
    public:
        UdpClient(const std::string &hostName, uint16_t portNumber);
        UdpClient(const IPV4Address &ipAddress, uint16_t portNumber);
        void connect() override;
    };

} //namespace CppSerialPort

#endif //CPPUDP_UDPCLIENT_H
