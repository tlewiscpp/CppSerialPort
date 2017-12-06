#include <iostream>
#include <string>
#include <future>
#include <mutex>
#include <cstring>
#include <memory>

#if defined(_WIN32)

#else
#    include <unistd.h>
#    include <signal.h>
#endif
#include "../../CppSerialPort/CppSerialPort/SerialPort.h"

using namespace CppSerialPort;


std::future<std::string> *asyncGetlineFuture{nullptr};
std::future<std::string> *asyncReadlineFuture{ nullptr };

std::shared_ptr<SerialPort> serialPort{nullptr};

std::string asyncGetlineTask() {
    std::string returnString{ "" };
	do {
		std::getline(std::cin, returnString);
	} while (returnString.empty());
    return returnString;
}

std::string asyncReadlineTask() {
    bool timeout{ false };
    while (true) {
        std::string returnString{ serialPort->readLine(&timeout) };
        if (returnString.length() > 0) {
            return returnString;
        }
    }
}


int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cout << "Usage: CppSerialPortTest [PortName]" << std::endl;
        return 1;
    }
    serialPort = std::make_shared<SerialPort>(argv[1], BaudRate::BAUD115200, DataBits::EIGHT, StopBits::ONE, Parity::NONE);
    serialPort->setLineEnding("\r\n");
    serialPort->setReadTimeout(1000);
    serialPort->openPort();
    serialPort->flushRx();
    std::string sendString{""};
    std::string receivedString{""};
    asyncGetlineFuture = new std::future<std::string>{std::async(std::launch::async, asyncGetlineTask)};
    asyncReadlineFuture = new std::future<std::string>{ std::async(std::launch::async, asyncReadlineTask) };
    while (true) {
        if (asyncGetlineFuture->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            sendString = asyncGetlineFuture->get();
            std::cout << "Sending string \"" << sendString << "\" to serial port" << std::endl;
            serialPort->writeLine(sendString);
            delete asyncGetlineFuture;
            asyncGetlineFuture = new std::future<std::string>{std::async(std::launch::async, asyncGetlineTask)};
        }
        if (asyncReadlineFuture->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			receivedString = asyncReadlineFuture->get();
        	std::cout << "Read string \"" << receivedString << "\" from serial port" << std::endl;
            delete asyncReadlineFuture;
            asyncReadlineFuture = new std::future<std::string>{ std::async(std::launch::async, asyncReadlineTask) };
        }
        sendString = "";
        receivedString = "";
    }
}

