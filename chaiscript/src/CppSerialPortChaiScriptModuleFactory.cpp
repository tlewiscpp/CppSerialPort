#include "CppSerialPortChaiScriptModuleFactory.hpp"

#include <chaiscript/chaiscript.hpp>
#include <CppSerialPort/SerialPort.hpp>
#include <CppSerialPort/IByteStream.hpp>
#include <CppSerialPort/AbstractSocket.hpp>
#include <CppSerialPort/TcpSocket.hpp>
#include <CppSerialPort/UdpSocket.hpp>
#include <CppSerialPort/ByteArray.hpp>

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
    returnModule->add(constructor<ByteArray(char *, int)>(), "ByteArray");
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

    returnModule->add(fun<ByteArray, ByteArray, char>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray, ByteArray, int>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray, ByteArray, const ByteArray &>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray, ByteArray, const std::string &>(&ByteArray::operator+), "+");
    returnModule->add(fun<ByteArray, ByteArray, const std::vector<char> &>(&ByteArray::operator+), "+");

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
    returnModule->add(fun<std::string, ByteArray, int>(&ByteArray::prettyPrint), "prettyPrint");
    returnModule->add(fun<std::string, ByteArray>(&ByteArray::prettyPrint), "prettyPrint");


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
    returnModule->add(fun<ssize_t, IByteStream, const ByteArray &>(&IByteStream::write), "write");

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
    //returnModule->add(fun(&SocketDisconnectedException::what), "what");

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

    /****TcpSocket****/
    returnModule->add(user_type<TcpSocket>(), "TcpSocket");
    returnModule->add(base_class<AbstractSocket, TcpSocket>());
    returnModule->add(constructor<TcpSocket(const std::string &, uint16_t)>(), "TcpSocket");
    returnModule->add(constructor<TcpSocket(const IPV4Address &, uint16_t)>(), "TcpSocket");

    returnModule->add(fun(static_cast<void(TcpSocket::*)(const ByteArray &)>(&TcpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(const std::string &)>(&TcpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(char)>(&TcpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(TcpSocket::*)() const>(&TcpSocket::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(TcpSocket::*)() const>(&TcpSocket::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(int)>(&TcpSocket::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(int)>(&TcpSocket::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(TcpSocket::*)(bool *)>(&TcpSocket::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(TcpSocket::*)(const ByteArray &, bool *)>(&TcpSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(TcpSocket::*)(const std::string &, bool *)>(&TcpSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(TcpSocket::*)(char, bool *)>(&TcpSocket::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(TcpSocket::*)()>(&TcpSocket::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(TcpSocket::*)()>(&TcpSocket::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(TcpSocket::*)(bool *)>(&TcpSocket::read)), "read");
    returnModule->add(fun(static_cast<bool(TcpSocket::*)() const>(&TcpSocket::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(TcpSocket::*)()>(&TcpSocket::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(TcpSocket::*)()>(&TcpSocket::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(TcpSocket::*)(char)>(&TcpSocket::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(TcpSocket::*)(const char *, size_t)>(&TcpSocket::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(TcpSocket::*)(const std::string &)>(&TcpSocket::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(TcpSocket::*)(const ByteArray &)>(&TcpSocket::writeLine)), "writeLine");

    //returnModule->add(fun(static_cast<void(TcpSocket::*)(const std::string &, uint16_t)>(&TcpSocket::connect)), "connect");
    returnModule->add(fun(static_cast<void(TcpSocket::*)()>(&TcpSocket::connect)), "connect");
    returnModule->add(fun(static_cast<bool(TcpSocket::*)()>(&TcpSocket::disconnect)), "disconnect");
    returnModule->add(fun(static_cast<bool(TcpSocket::*)() const>(&TcpSocket::isConnected)), "isConnected");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(uint16_t)>(&TcpSocket::setPortNumber)), "setPortNumber");
    returnModule->add(fun(static_cast<void(TcpSocket::*)(const std::string &)>(&TcpSocket::setHostName)), "setHostName");
    returnModule->add(fun(static_cast<uint16_t(TcpSocket::*)() const>(&TcpSocket::portNumber)), "portNumber");
    returnModule->add(fun(static_cast<std::string(TcpSocket::*)() const>(&TcpSocket::hostName)), "hostName");
    /****End ient****/

    /****UdpSocket****/
    returnModule->add(user_type<UdpSocket>(), "UdpSocket");
    returnModule->add(base_class<AbstractSocket, UdpSocket>());

    returnModule->add(fun(static_cast<void(UdpSocket::*)(const ByteArray &)>(&UdpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(const std::string &)>(&UdpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(char)>(&UdpSocket::setLineEnding)), "setLineEnding");
    returnModule->add(fun(static_cast<int(UdpSocket::*)() const>(&UdpSocket::readTimeout)), "readTimeout");
    returnModule->add(fun(static_cast<int(UdpSocket::*)() const>(&UdpSocket::writeTimeout)), "writeTimeout");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(int)>(&UdpSocket::setReadTimeout)), "setReadTimeout");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(int)>(&UdpSocket::setWriteTimeout)), "setWriteTimeout");

    returnModule->add(fun(static_cast<ByteArray(UdpSocket::*)(bool *)>(&UdpSocket::readLine)), "readLine");
    returnModule->add(fun(static_cast<ByteArray(UdpSocket::*)(const ByteArray &, bool *)>(&UdpSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(UdpSocket::*)(const std::string &, bool *)>(&UdpSocket::readUntil)), "readUntil");
    returnModule->add(fun(static_cast<ByteArray(UdpSocket::*)(char, bool *)>(&UdpSocket::readUntil)), "readUntil");

    returnModule->add(fun(static_cast<void(UdpSocket::*)()>(&UdpSocket::openPort)), "openPort");
    returnModule->add(fun(static_cast<void(UdpSocket::*)()>(&UdpSocket::closePort)), "closePort");
    returnModule->add(fun(static_cast<char(UdpSocket::*)(bool *)>(&UdpSocket::read)), "read");
    returnModule->add(fun(static_cast<bool(UdpSocket::*)() const>(&UdpSocket::isOpen)), "isOpen");

    returnModule->add(fun(static_cast<void(UdpSocket::*)()>(&UdpSocket::flushRx)), "flushRx");
    returnModule->add(fun(static_cast<void(UdpSocket::*)()>(&UdpSocket::flushTx)), "flushTx");
    returnModule->add(fun(static_cast<ssize_t(UdpSocket::*)(char)>(&UdpSocket::write)), "write");
    returnModule->add(fun(static_cast<ssize_t(UdpSocket::*)(const char *, size_t)>(&UdpSocket::write)), "write");

    returnModule->add(fun(static_cast<ssize_t(UdpSocket::*)(const std::string &)>(&UdpSocket::writeLine)), "writeLine");
    returnModule->add(fun(static_cast<ssize_t(UdpSocket::*)(const ByteArray &)>(&UdpSocket::writeLine)), "writeLine");


    //returnModule->add(fun(static_cast<void(UdpSocket::*)(const std::string &, uint16_t)>(&UdpSocket::connect)), "connect");
    returnModule->add(fun(static_cast<void(UdpSocket::*)()>(&UdpSocket::connect)), "connect");
    returnModule->add(fun(static_cast<bool(UdpSocket::*)()>(&UdpSocket::disconnect)), "disconnect");
    returnModule->add(fun(static_cast<bool(UdpSocket::*)() const>(&UdpSocket::isConnected)), "isConnected");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(uint16_t)>(&UdpSocket::setPortNumber)), "setPortNumber");
    returnModule->add(fun(static_cast<void(UdpSocket::*)(const std::string &)>(&UdpSocket::setHostName)), "setHostName");
    returnModule->add(fun(static_cast<uint16_t(UdpSocket::*)() const>(&UdpSocket::portNumber)), "portNumber");
    returnModule->add(fun(static_cast<std::string(UdpSocket::*)() const>(&UdpSocket::hostName)), "hostName");


    returnModule->add(constructor<UdpSocket(const std::string &, uint16_t)>(), "UdpSocket");
    returnModule->add(constructor<UdpSocket(const IPV4Address &, uint16_t)>(), "UdpSocket");
    /****End UdpSocket****/

    return returnModule;
}
