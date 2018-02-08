
#ifndef TRMIDOUBLECHECKBOX_IPV4ADDRESS_H
#define TRMIDOUBLECHECKBOX_IPV4ADDRESS_H

#include <cstdint>
#include <string>


namespace CppSerialPort {

union UnderlyingBytes {
    uint32_t address;
    uint8_t bytes[sizeof(uint32_t)];
};

class IPV4Address {
public:
    explicit IPV4Address(uint32_t address);
    explicit IPV4Address(uint8_t *bytes);
    explicit IPV4Address(const uint8_t *bytes);
    explicit IPV4Address(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);

    uint8_t &operator[](size_t index);
    const uint8_t &operator[](size_t index) const;

    uint32_t &address();
    const uint32_t &address() const;

    std::string toString() const;

private:
    UnderlyingBytes m_bytes;
};

} //namespace CppSerialPort


#endif //TRMIDOUBLECHECKBOX_IPV4ADDRESS_H
