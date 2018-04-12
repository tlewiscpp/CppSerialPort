#include "CppSerialPortChaiScriptModuleFactory.hpp"

#include <chaiscript/chaiscript.hpp>
#include <CppSerialPort/SerialPort.h>
#include <CppSerialPort/IByteStream.h>
#include <CppSerialPort/AbstractSocket.h>
#include <CppSerialPort/TcpClient.h>
#include <CppSerialPort/UdpClient.h>
#include <CppSerialPort/ByteArray.h>

using namespace CppSerialPort;
using namespace chaiscript;

ModulePtr CppSerialPortChaiScriptModuleFactory::createModule() {
    ModulePtr returnModule{new Module{}};

    /****ByteArray****/
    returnModule->add(user_type<ByteArray>(), "ByteArray");
    returnModule->add(constructor<ByteArray()>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(const char *)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(const std::string &)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char *, size_t)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(const std::vector<char> &)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(const ByteArray &)>(), "ByteArray");

    returnModule->add(constructor<ByteArray(int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)>(), "ByteArray");


    returnModule->add(constructor<ByteArray(char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");
    returnModule->add(constructor<ByteArray(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char)>(), "ByteArray");


    //returnModule->add(constructor<ByteArray(ByteArray &&)>(), "ByteArray");

    returnModule->add(fun<ByteArray &, ByteArray, const ByteArray &>(&ByteArray::operator=), "=");
    returnModule->add(fun<ByteArray &, ByteArray, const std::vector<char> &>(&ByteArray::operator=), "=");
    returnModule->add(fun<ByteArray &, ByteArray, const std::string &>(&ByteArray::operator=), "=");
    returnModule->add(fun<ByteArray &, ByteArray, char>(&ByteArray::operator=), "=");
    //returnModule->add(fun<ByteArray &, ByteArray, ByteArray &&>(&ByteArray::operator=), "=");
    //returnModule->add(fun<ByteArray &, ByteArray, std::vector<char> &&>(&ByteArray::operator=), "=");

    returnModule->add(fun<ByteArray &, ByteArray, char>(&ByteArray::append), "append");
    returnModule->add(fun<ByteArray &, ByteArray, int>(&ByteArray::append), "append");
    returnModule->add(fun<ByteArray &, ByteArray, const ByteArray &>(&ByteArray::append), "append");
    returnModule->add(fun<ByteArray &, ByteArray, const std::string &>(&ByteArray::append), "append");
    returnModule->add(fun<ByteArray &, ByteArray, const std::vector<char> &>(&ByteArray::append), "append");

    returnModule->add(fun<ByteArray &, ByteArray, char>(&ByteArray::operator+=), "+=");
    returnModule->add(fun<ByteArray &, ByteArray, int>(&ByteArray::operator+=), "+=");
    returnModule->add(fun<ByteArray &, ByteArray, const ByteArray &>(&ByteArray::operator+=), "+=");
    returnModule->add(fun<ByteArray &, ByteArray, const std::string &>(&ByteArray::operator+=), "+=");
    returnModule->add(fun<ByteArray &, ByteArray, const std::vector<char> &>(&ByteArray::operator+=), "+=");

    returnModule->add(fun<ByteArray &, ByteArray, char>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray &, ByteArray, int>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray &, ByteArray, const ByteArray &>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray &, ByteArray, const std::string &>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray &, ByteArray, const std::vector<char> &>(&ByteArray::operator+), "+");

    returnModule->add(fun<size_t, ByteArray, const ByteArray &>(&ByteArray::find), "find");
    returnModule->add(fun<size_t, ByteArray, char>(&ByteArray::find), "find");

    returnModule->add(fun<std::vector<char>::iterator>(&ByteArray::begin), "begin");
    returnModule->add(fun<const std::vector<char>::const_iterator>(&ByteArray::cbegin), "cbegin");
    returnModule->add(fun<std::vector<char>::reverse_iterator>(&ByteArray::rbegin), "rbegin");
    returnModule->add(fun<const std::vector<char>::const_reverse_iterator>(&ByteArray::crbegin), "crbegin");
    returnModule->add(fun<std::vector<char>::iterator>(&ByteArray::end), "end");
    returnModule->add(fun<const std::vector<char>::const_iterator>(&ByteArray::cend), "cend");
    returnModule->add(fun<std::vector<char>::reverse_iterator>(&ByteArray::rend), "rend");
    returnModule->add(fun<const std::vector<char>::const_reverse_iterator>(&ByteArray::crend), "crend");

    returnModule->add(fun<ByteArray &, ByteArray>(&ByteArray::clear), "clear");
    returnModule->add(fun<size_t, ByteArray>(&ByteArray::size), "size");
    returnModule->add(fun<size_t, ByteArray>(&ByteArray::length), "length");
    returnModule->add(fun<bool, ByteArray>(&ByteArray::empty), "empty");
    returnModule->add(fun<const char *, ByteArray>(&ByteArray::data), "data");
    returnModule->add(fun<char *, ByteArray>(&ByteArray::data), "data");
    returnModule->add(fun<ByteArray &, ByteArray>(&ByteArray::popBack), "popBack");
    returnModule->add(fun<ByteArray &, ByteArray>(&ByteArray::popFront), "popFront");

    returnModule->add(fun<char &, ByteArray, size_t>(&ByteArray::operator[]), "[]");
    returnModule->add(fun<const char &, ByteArray, size_t>(&ByteArray::operator[]), "[]");
    returnModule->add(fun<char &, ByteArray, size_t>(&ByteArray::at), "at");
    returnModule->add(fun<const char &, ByteArray, size_t>(&ByteArray::at), "at");

    returnModule->add(fun<ByteArray, ByteArray, size_t, size_t>(&ByteArray::subsequence), "subsequence");
    returnModule->add(fun<std::string, ByteArray>(&ByteArray::toString), "toString");

    returnModule->add(fun<bool, ByteArray, char *, size_t>(&ByteArray::endsWith), "endsWith");
    returnModule->add(fun<bool, ByteArray, const char *>(&ByteArray::endsWith), "endsWith");
    returnModule->add(fun<bool, ByteArray, const std::string &>(&ByteArray::endsWith), "endsWith");
    returnModule->add(fun<bool, ByteArray, const ByteArray &>(&ByteArray::endsWith), "endsWith");


    returnModule->add(fun<bool, ByteArray, char *, size_t>(&ByteArray::startsWith), "startsWith");
    returnModule->add(fun<bool, ByteArray, const char *>(&ByteArray::startsWith), "startsWith");
    returnModule->add(fun<bool, ByteArray, const std::string &>(&ByteArray::startsWith), "startsWith");
    returnModule->add(fun<bool, ByteArray, const ByteArray &>(&ByteArray::startsWith), "startsWith");
    /****End ByteArray****/


    /****IByteStream****/
    returnModule->add(user_type<IByteStream>(), "IByteStream");
    returnModule->add(fun<void, IByteStream, const ByteArray &>(&IByteStream::setLineEnding), "setLineEnding");
    returnModule->add(fun<void, IByteStream, const std::string &>(&IByteStream::setLineEnding), "setLineEnding");
    returnModule->add(fun<void, IByteStream, char>(&IByteStream::setLineEnding), "setLineEnding");
    returnModule->add(fun<int, IByteStream>(&IByteStream::readTimeout), "readTimeout");
    returnModule->add(fun<int, IByteStream>(&IByteStream::writeTimeout), "writeTimeout");
    returnModule->add(fun<void, IByteStream, int>(&IByteStream::setReadTimeout), "setReadTimeout");
    returnModule->add(fun<void, IByteStream, int>(&IByteStream::setWriteTimeout), "setWriteTimeout");

    returnModule->add(fun<ByteArray, IByteStream, bool *>(&IByteStream::readLine), "readLine");
    returnModule->add(fun<ByteArray, IByteStream, const ByteArray &, bool *>(&IByteStream::readUntil), "readUntil");
    returnModule->add(fun<ByteArray, IByteStream, const std::string &, bool *>(&IByteStream::readUntil), "readUntil");
    returnModule->add(fun<ByteArray, IByteStream, char, bool *>(&IByteStream::readUntil), "readUntil");

    returnModule->add(fun<void, IByteStream>(&IByteStream::openPort), "openPort");
    returnModule->add(fun<void, IByteStream>(&IByteStream::closePort), "closePort");
    returnModule->add(fun<char, IByteStream, bool *>(&IByteStream::read), "read");
    returnModule->add(fun<bool, IByteStream>(&IByteStream::isOpen), "isOpen");
    returnModule->add(fun<size_t, IByteStream>(&IByteStream::available), "available");


    returnModule->add(fun<void, IByteStream>(&IByteStream::flushRx), "flushRx");
    returnModule->add(fun<void, IByteStream>(&IByteStream::flushTx), "flushTx");
    returnModule->add(fun<ssize_t, IByteStream, char>(&IByteStream::write), "write");
    returnModule->add(fun<ssize_t, IByteStream, const char *, size_t>(&IByteStream::write), "write");

    returnModule->add(fun<ssize_t, IByteStream, const std::string &>(&IByteStream::writeLine), "writeLine");
    returnModule->add(fun<ssize_t, IByteStream, const ByteArray &>(&IByteStream::writeLine), "writeLine");
    /****End IByteStream****/

    /****SerialPort****/
    returnModule->add(user_type<SerialPort>(), "SerialPort");

    returnModule->add(constructor<SerialPort(const std::string &, BaudRate, DataBits, StopBits, Parity, FlowControl, const std::string &)>(), "SerialPort");
    returnModule->add(base_class<IByteStream, SerialPort>());

    returnModule->add(fun(static_cast<void(SerialPort::*)(const ByteArray &)>(&SerialPort::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(SerialPort::*)(const std::string &)>(&SerialPort::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(SerialPort::*)(char)>(&SerialPort::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(SerialPort::*)() const>(&SerialPort::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(SerialPort::*)() const>(&SerialPort::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(SerialPort::*)(int)>(&SerialPort::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(SerialPort::*)(int)>(&SerialPort::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(SerialPort::*)(bool *)>(&SerialPort::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(SerialPort::*)(const ByteArray &, bool *)>(&SerialPort::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(SerialPort::*)(const std::string &, bool *)>(&SerialPort::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(SerialPort::*)(char, bool *)>(&SerialPort::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(SerialPort::*)()>(&SerialPort::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(SerialPort::*)()>(&SerialPort::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(SerialPort::*)(bool *)>(&SerialPort::read)), "read");
    returnModule->add(fun(static_cast<bool(SerialPort::*)() const>(&SerialPort::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(SerialPort::*)()>(&SerialPort::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(SerialPort::*)()>(&SerialPort::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(SerialPort::*)(char)>(&SerialPort::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(SerialPort::*)(const char *, size_t)>(&SerialPort::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(SerialPort::*)(const std::string &)>(&SerialPort::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(SerialPort::*)(const ByteArray &)>(&SerialPort::writeLine)), "writeLine");


    returnModule->add_global_const(const_var(&SerialPort::DEFAULT_BAUD_RATE), "DEFAULT_BAUD_RATE");
    returnModule->add_global_const(const_var(&SerialPort::DEFAULT_DATA_BITS), "DEFAULT_DATA_BITS");
    returnModule->add_global_const(const_var(&SerialPort::DEFAULT_STOP_BITS), "DEFAULT_STOP_BITS");
    returnModule->add_global_const(const_var(&SerialPort::DEFAULT_PARITY), "DEFAULT_PARITY");
    returnModule->add_global_const(const_var(&SerialPort::DEFAULT_FLOW_CONTROL), "DEFAULT_FLOW_CONTROL");

    returnModule->add(fun(&SerialPort::isValidSerialPortName), "isValidSerialPortName");
    returnModule->add(fun(&SerialPort::availableSerialPorts), "availableSerialPorts");


    returnModule->add(fun(&SerialPort::isDCDEnabled), "isDCDEnabled");
    returnModule->add(fun(&SerialPort::isCTSEnabled), "isCTSEnabled");
    returnModule->add(fun(&SerialPort::isDSREnabled), "isDSREnabled");
    returnModule->add(fun(&SerialPort::enableDTR), "enableDTR");
    returnModule->add(fun(&SerialPort::disableDTR), "disableDTR");
    returnModule->add(fun(&SerialPort::enableRTS), "enableRTS");
    returnModule->add(fun(&SerialPort::disableRTS), "disableRTS");


    returnModule->add(fun(&SerialPort::portName), "portName");
    returnModule->add(fun(&SerialPort::baudRate), "baudRate");
    returnModule->add(fun(&SerialPort::stopBits), "stopBits");
    returnModule->add(fun(&SerialPort::dataBits), "dataBits");
    returnModule->add(fun(&SerialPort::parity), "parity");

    returnModule->add(fun(&SerialPort::setBaudRate), "setBaudRate");
    returnModule->add(fun(&SerialPort::setDataBits), "setDataBits");
    returnModule->add(fun(&SerialPort::setDataBits), "setStopBits");
    returnModule->add(fun(&SerialPort::setDataBits), "setParity");
    returnModule->add(fun(&SerialPort::setDataBits), "setFlowControl");

    //BaudRate
#if defined(_WIN32)
    returnModule->add_global_const(const_var(BaudRate::Baud110), "Baud110");
    returnModule->add_global_const(const_var(BaudRate::Baud300), "Baud300");
    returnModule->add_global_const(const_var(BaudRate::Baud600), "Baud600");
    returnModule->add_global_const(const_var(BaudRate::Baud1200), "Baud1200");
    returnModule->add_global_const(const_var(BaudRate::Baud2400), "Baud2400");
    returnModule->add_global_const(const_var(BaudRate::Baud4800), "Baud4800");
    returnModule->add_global_const(const_var(BaudRate::Baud9600), "Baud9600");
    returnModule->add_global_const(const_var(BaudRate::Baud19200), "Baud19200");
    returnModule->add_global_const(const_var(BaudRate::Baud38400), "Baud38400");
    returnModule->add_global_const(const_var(BaudRate::Baud57600), "Baud57600");
    returnModule->add_global_const(const_var(BaudRate::Baud115200), "Baud115200");
    returnModule->add_global_const(const_var(BaudRate::Baud128000), "Baud128000");
    returnModule->add_global_const(const_var(BaudRate::Baud256000), "Baud256000");
#else
    returnModule->add_global_const(const_var(BaudRate::Baud50), "Baud50");
    returnModule->add_global_const(const_var(BaudRate::Baud75), "Baud75");
    returnModule->add_global_const(const_var(BaudRate::Baud110), "Baud110");
    returnModule->add_global_const(const_var(BaudRate::Baud134), "Baud134");
    returnModule->add_global_const(const_var(BaudRate::Baud150), "Baud150");
    returnModule->add_global_const(const_var(BaudRate::Baud200), "Baud200");
    returnModule->add_global_const(const_var(BaudRate::Baud300), "Baud300");
    returnModule->add_global_const(const_var(BaudRate::Baud600), "Baud600");
    returnModule->add_global_const(const_var(BaudRate::Baud1200), "Baud1200");
    returnModule->add_global_const(const_var(BaudRate::Baud1800), "Baud1800");
    returnModule->add_global_const(const_var(BaudRate::Baud2400), "Baud2400");
    returnModule->add_global_const(const_var(BaudRate::Baud4800), "Baud4800");
    returnModule->add_global_const(const_var(BaudRate::Baud9600), "Baud9600");
    returnModule->add_global_const(const_var(BaudRate::Baud19200), "Baud19200");
    returnModule->add_global_const(const_var(BaudRate::Baud38400), "Baud38400");
    returnModule->add_global_const(const_var(BaudRate::Baud57600), "Baud57600");
    returnModule->add_global_const(const_var(BaudRate::Baud115200), "Baud115200");
    returnModule->add_global_const(const_var(BaudRate::Baud230400), "Baud230400");
    returnModule->add_global_const(const_var(BaudRate::Baud460800), "Baud460800");
    returnModule->add_global_const(const_var(BaudRate::Baud500000), "Baud500000");
    returnModule->add_global_const(const_var(BaudRate::Baud576000), "Baud576000");
    returnModule->add_global_const(const_var(BaudRate::Baud921600), "Baud921600");
    returnModule->add_global_const(const_var(BaudRate::Baud1000000), "Baud1000000");
    returnModule->add_global_const(const_var(BaudRate::Baud1152000), "Baud1152000");
    returnModule->add_global_const(const_var(BaudRate::Baud1500000), "Baud1500000");
    returnModule->add_global_const(const_var(BaudRate::Baud2000000), "Baud2000000");
    returnModule->add_global_const(const_var(BaudRate::Baud2500000), "Baud2500000");
    returnModule->add_global_const(const_var(BaudRate::Baud3000000), "Baud3000000");
    returnModule->add_global_const(const_var(BaudRate::Baud3500000), "Baud3500000");
    returnModule->add_global_const(const_var(BaudRate::Baud4000000), "Baud4000000");
#endif //defined(_WIN32)

    //DataBits
    returnModule->add_global_const(const_var(DataBits::DataFive), "DataFive");
    returnModule->add_global_const(const_var(DataBits::DataSix), "DataSix");
    returnModule->add_global_const(const_var(DataBits::DataSeven), "DataSeven");
    returnModule->add_global_const(const_var(DataBits::DataEight), "DataEight");

    //StopBits
    returnModule->add_global_const(const_var(StopBits::StopOne), "StopOne");
#if defined(_WIN32)
    returnModule->add_global_const(const_var(StopBits::StopOneFive), "StopOneFive");
#endif //defined(_WIN32)
    returnModule->add_global_const(const_var(StopBits::StopTwo), "StopTwo");

    //Parity
    returnModule->add_global_const(const_var(Parity::ParityNone), "ParityNone");
    returnModule->add_global_const(const_var(Parity::ParityOdd), "ParityOdd");
    returnModule->add_global_const(const_var(Parity::ParityEven), "ParityEven");
    returnModule->add_global_const(const_var(Parity::ParitySpace), "ParitySpace");
#if defined(_WIN32)
    returnModule->add_global_const(const_var(Parity::ParityMark), "ParityMark");
#endif //defined(_WIN32)

    //FlowControl
    returnModule->add_global_const(const_var(FlowControl::FlowOff), "FlowOff");
    returnModule->add_global_const(const_var(FlowControl::FlowXonXoff), "FlowXonXoff");
    returnModule->add_global_const(const_var(FlowControl::FlowHardware), "FlowHardware");
    /****End SerialPort****/

    /****IPV4Address****/
    returnModule->add(user_type<IPV4Address>(), "IPV4Address");
    returnModule->add(constructor<IPV4Address(uint32_t)>(), "IPV4Address");
    returnModule->add(constructor<IPV4Address(uint8_t *)>(), "IPV4Address");
    returnModule->add(constructor<IPV4Address(const uint8_t *)>(), "IPV4Address");
    returnModule->add(constructor<IPV4Address(uint8_t, uint8_t, uint8_t, uint8_t)>(), "IPV4Address");

    returnModule->add(fun<uint8_t &, IPV4Address, size_t>(&IPV4Address::operator[]), "[]");
    returnModule->add(fun<const uint8_t &, IPV4Address, size_t>(&IPV4Address::operator[]), "[]");

    returnModule->add(fun<const uint32_t &, IPV4Address>(&IPV4Address::address), "address");
    returnModule->add(fun<uint32_t &, IPV4Address>(&IPV4Address::address), "address");

    returnModule->add(fun(&IPV4Address::toString), "toString");
    /****End IPV4Address****/


    /****AbstractSocket****/
    returnModule->add(user_type<SocketDisconnectedException>(), "SocketDisconnectedException");
    returnModule->add(base_class<std::runtime_error, SocketDisconnectedException>());
    returnModule->add(constructor<SocketDisconnectedException(const std::string &, const std::string &)>(), "SocketDisconnectedException");
    returnModule->add(fun(&SocketDisconnectedException::portName), "portName");
    returnModule->add(fun(&SocketDisconnectedException::setPortName), "setPortName");
    returnModule->add(fun(&SocketDisconnectedException::what), "what");

    returnModule->add(user_type<AbstractSocket>(), "AbstractSocket");
    returnModule->add(base_class<IByteStream, AbstractSocket>());


    returnModule->add(fun(static_cast<void(AbstractSocket::*)(const ByteArray &)>(&AbstractSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)(const std::string &)>(&AbstractSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)(char)>(&AbstractSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(AbstractSocket::*)() const>(&AbstractSocket::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(AbstractSocket::*)() const>(&AbstractSocket::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)(int)>(&AbstractSocket::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)(int)>(&AbstractSocket::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(AbstractSocket::*)(bool *)>(&AbstractSocket::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(AbstractSocket::*)(const ByteArray &, bool *)>(&AbstractSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(AbstractSocket::*)(const std::string &, bool *)>(&AbstractSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(AbstractSocket::*)(char, bool *)>(&AbstractSocket::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(AbstractSocket::*)()>(&AbstractSocket::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)()>(&AbstractSocket::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(AbstractSocket::*)(bool *)>(&AbstractSocket::read)), "read");
    returnModule->add(fun(static_cast<bool(AbstractSocket::*)() const>(&AbstractSocket::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(AbstractSocket::*)()>(&AbstractSocket::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(AbstractSocket::*)()>(&AbstractSocket::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(AbstractSocket::*)(char)>(&AbstractSocket::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(AbstractSocket::*)(const char *, size_t)>(&AbstractSocket::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(AbstractSocket::*)(const std::string &)>(&AbstractSocket::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(AbstractSocket::*)(const ByteArray &)>(&AbstractSocket::writeLine)), "writeLine");


    returnModule->add(fun<void, AbstractSocket, const std::string &, uint16_t>(&AbstractSocket::connect), "connect");
    returnModule->add(fun<void, AbstractSocket>(&AbstractSocket::connect), "connect");
    returnModule->add(fun<bool, AbstractSocket>(&AbstractSocket::disconnect), "disconnect");
    returnModule->add(fun<bool, AbstractSocket>(&AbstractSocket::isConnected), "isConnected");
    returnModule->add(fun<void, AbstractSocket, uint16_t>(&AbstractSocket::setPortNumber), "setPortNumber");
    returnModule->add(fun<void, AbstractSocket, const std::string &>(&AbstractSocket::setHostName), "setHostName");
    returnModule->add(fun<uint16_t, AbstractSocket>(&AbstractSocket::portNumber), "portNumber");
    returnModule->add(fun<std::string, AbstractSocket>(&AbstractSocket::hostName), "hostName");
    /****End AbstractSocket****/

    /****TcpClient****/
    returnModule->add(user_type<TcpClient>(), "TcpClient");
    returnModule->add(base_class<AbstractSocket, TcpClient>());
    returnModule->add(constructor<TcpClient(const std::string &, uint16_t)>(), "TcpClient");
    returnModule->add(constructor<TcpClient(const IPV4Address &, uint16_t)>(), "TcpClient");

    returnModule->add(fun(static_cast<void(TcpClient::*)(const ByteArray &)>(&TcpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(TcpClient::*)(const std::string &)>(&TcpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(TcpClient::*)(char)>(&TcpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(TcpClient::*)() const>(&TcpClient::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(TcpClient::*)() const>(&TcpClient::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(TcpClient::*)(int)>(&TcpClient::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(TcpClient::*)(int)>(&TcpClient::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(TcpClient::*)(bool *)>(&TcpClient::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(TcpClient::*)(const ByteArray &, bool *)>(&TcpClient::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(TcpClient::*)(const std::string &, bool *)>(&TcpClient::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(TcpClient::*)(char, bool *)>(&TcpClient::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(TcpClient::*)()>(&TcpClient::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(TcpClient::*)()>(&TcpClient::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(TcpClient::*)(bool *)>(&TcpClient::read)), "read");
    returnModule->add(fun(static_cast<bool(TcpClient::*)() const>(&TcpClient::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(TcpClient::*)()>(&TcpClient::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(TcpClient::*)()>(&TcpClient::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(TcpClient::*)(char)>(&TcpClient::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(TcpClient::*)(const char *, size_t)>(&TcpClient::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(TcpClient::*)(const std::string &)>(&TcpClient::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(TcpClient::*)(const ByteArray &)>(&TcpClient::writeLine)), "writeLine");

    //returnModule->add(fun(static_cast<void(TcpClient::*)(const std::string &, uint16_t)>(&TcpClient::connect)), "connect");
    returnModule->add(fun(static_cast<void(TcpClient::*)()>(&TcpClient::connect)), "connect");
    returnModule->add(fun(static_cast<bool(TcpClient::*)()>(&TcpClient::disconnect)), "disconnect");
    returnModule->add(fun(static_cast<bool(TcpClient::*)() const>(&TcpClient::isConnected)), "isConnected");
    returnModule->add(fun(static_cast<void(TcpClient::*)(uint16_t)>(&TcpClient::setPortNumber)), "setPortNumber");
    returnModule->add(fun(static_cast<void(TcpClient::*)(const std::string &)>(&TcpClient::setHostName)), "setHostName");
    returnModule->add(fun(static_cast<uint16_t(TcpClient::*)() const>(&TcpClient::portNumber)), "portNumber");
    returnModule->add(fun(static_cast<std::string(TcpClient::*)() const>(&TcpClient::hostName)), "hostName");
    /****End TcpClient****/

    /****UdpClient****/
    returnModule->add(user_type<UdpClient>(), "UdpClient");
    returnModule->add(base_class<AbstractSocket, UdpClient>());

    returnModule->add(fun(static_cast<void(UdpClient::*)(const ByteArray &)>(&UdpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(UdpClient::*)(const std::string &)>(&UdpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(UdpClient::*)(char)>(&UdpClient::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(UdpClient::*)() const>(&UdpClient::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(UdpClient::*)() const>(&UdpClient::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(UdpClient::*)(int)>(&UdpClient::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(UdpClient::*)(int)>(&UdpClient::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(UdpClient::*)(bool *)>(&UdpClient::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(UdpClient::*)(const ByteArray &, bool *)>(&UdpClient::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(UdpClient::*)(const std::string &, bool *)>(&UdpClient::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(UdpClient::*)(char, bool *)>(&UdpClient::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(UdpClient::*)()>(&UdpClient::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(UdpClient::*)()>(&UdpClient::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(UdpClient::*)(bool *)>(&UdpClient::read)), "read");
    returnModule->add(fun(static_cast<bool(UdpClient::*)() const>(&UdpClient::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(UdpClient::*)()>(&UdpClient::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(UdpClient::*)()>(&UdpClient::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(UdpClient::*)(char)>(&UdpClient::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(UdpClient::*)(const char *, size_t)>(&UdpClient::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(UdpClient::*)(const std::string &)>(&UdpClient::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(UdpClient::*)(const ByteArray &)>(&UdpClient::writeLine)), "writeLine");


    //returnModule->add(fun(static_cast<void(UdpClient::*)(const std::string &, uint16_t)>(&UdpClient::connect)), "connect");
    returnModule->add(fun(static_cast<void(UdpClient::*)()>(&UdpClient::connect)), "connect");
    returnModule->add(fun(static_cast<bool(UdpClient::*)()>(&UdpClient::disconnect)), "disconnect");
    returnModule->add(fun(static_cast<bool(UdpClient::*)() const>(&UdpClient::isConnected)), "isConnected");
    returnModule->add(fun(static_cast<void(UdpClient::*)(uint16_t)>(&UdpClient::setPortNumber)), "setPortNumber");
    returnModule->add(fun(static_cast<void(UdpClient::*)(const std::string &)>(&UdpClient::setHostName)), "setHostName");
    returnModule->add(fun(static_cast<uint16_t(UdpClient::*)() const>(&UdpClient::portNumber)), "portNumber");
    returnModule->add(fun(static_cast<std::string(UdpClient::*)() const>(&UdpClient::hostName)), "hostName");


    returnModule->add(constructor<UdpClient(const std::string &, uint16_t)>(), "UdpClient");
    returnModule->add(constructor<UdpClient(const IPV4Address &, uint16_t)>(), "UdpClient");
    /****End UdpClient****/

    return returnModule;
}
