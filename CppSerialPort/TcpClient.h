//
// Created by pinguinsan on 11/3/17.
//

#ifndef CPPTCP_TCPCLIENT_H
#define CPPTCP_TCPCLIENT_H

#include <string>
#if defined(_MSC_VER)
#    include "WinSock2.h"
#    include "Windows.h"
#else
#    include <sys/socket.h>
#    include <netinet/in.h>
#    include <netdb.h>
#endif //defined(_MSC_VER)


#include <sys/types.h>
#include <memory>
#include "IByteStream.h"

class TcpClient : public CppSerialPort::IByteStream
{
public:
    TcpClient(const std::string &hostName, uint16_t portNumber);
    ~TcpClient() override;

    int read() override;
    ssize_t write(int i) override;
    ssize_t writeLine(const std::string &str) override;
    std::string portName() const override;
    bool isOpen() const override;
    void openPort() override;
    void closePort() override;
    void flushRx() override;
    void flushTx() override;
    void putBack(int c) override;
	void setReadTimeout(int timeout) override;

    void connect(const std::string &hostName, uint16_t portNumber);
    void connect();
    bool disconnect();
    bool isConnected() const;
    void setPortNumber(uint16_t portNumber);
    void setHostName(const std::string &hostName);
    uint16_t portNumber() const;
    std::string hostName() const;
private:
#if defined(_MSC_VER)
	SOCKET m_socketDescriptor;
#else
	int m_socketDescriptor;
#endif //defined(_MSC_VER)
    uint16_t m_portNumber;
    std::string m_hostName;
    std::string m_readBuffer;

    static timeval toTimeVal(uint32_t totalTimeout);

	static std::string getErrorString(int errorCode);
	static int getLastError();
};


#endif //CPPTCP_TCPCLIENT_H
