#include "IPV4Address.h"

#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>

namespace {
    template <typename T> inline std::string toStdString(const T &t) { return dynamic_cast<std::ostringstream &>(std::ostringstream{} << t).str(); }
}

namespace CppSerialPort {

IPV4Address::IPV4Address(uint32_t address) {
    this->m_bytes.address = address;
}


IPV4Address::IPV4Address(const uint8_t *bytes){
    std::copy(bytes, bytes + 4, this->m_bytes.bytes);
}

IPV4Address::IPV4Address(uint8_t *bytes){
    std::copy(bytes, bytes + 4, this->m_bytes.bytes);
}

IPV4Address::IPV4Address(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    this->m_bytes.bytes[0] = byte0;
    this->m_bytes.bytes[1] = byte1;
    this->m_bytes.bytes[2] = byte2;
    this->m_bytes.bytes[3] = byte3;
}

uint8_t &IPV4Address::operator[](size_t index) {
    if (index > sizeof(uint32_t)) {
        throw std::runtime_error("IPV4Address::operator[]: index cannot be greater maximum index (" + toStdString(index) + " > " + toStdString(sizeof(uint32_t)) + ")");
    }
    return this->m_bytes.bytes[index];
}

const uint8_t &IPV4Address::operator[](size_t index) const {
    if (index > sizeof(uint32_t)) {
        throw std::runtime_error("IPV4Address::operator[]: index cannot be greater maximum index (" + toStdString(index) + " > " + toStdString(sizeof(uint32_t)) + ")");
    }
    return this->m_bytes.bytes[index];
}

uint32_t &IPV4Address::address() {
    return this->m_bytes.address;
}

const uint32_t &IPV4Address::address() const {
    return this->m_bytes.address;
}

std::string IPV4Address::toString() const {
    return (toStdString(static_cast<int>(this->m_bytes.bytes[0])) + '.'
           + toStdString(static_cast<int>(this->m_bytes.bytes[1])) + '.'
             + toStdString(static_cast<int>(this->m_bytes.bytes[2])) + '.'
          + toStdString(static_cast<int>(this->m_bytes.bytes[3])));
}

} //namespace CppSerialPort