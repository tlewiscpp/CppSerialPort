#include <CppSerialPort/IByteStream.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>

#if defined(_WIN32)
const char *CppSerialPort::IByteStream::DEFAULT_LINE_ENDING{"\r\n"};
#    include <windows.h>
#    include <shlwapi.h>
#else
#    include <unistd.h>
const char *CppSerialPort::IByteStream::DEFAULT_LINE_ENDING{"\n"};
#endif //defined(_WIN32)

namespace {
    template <typename InputType> static std::string toFixedWidthHex(InputType value, size_t targetLength, bool includeZeroX = true) {
        std::stringstream returnString{};
        if (includeZeroX) {
            returnString << "0x";
        }
        returnString << std::setfill('0');
        returnString << std::setw(static_cast<int>(targetLength));
        returnString << std::hex;
        returnString << static_cast<int>(value);
        return returnString.str();
    }
}

namespace CppSerialPort {

const int IByteStream::DEFAULT_READ_TIMEOUT{1000};
const int IByteStream::DEFAULT_WRITE_TIMEOUT{1000};

IByteStream::IByteStream() :
	m_readTimeout{ DEFAULT_READ_TIMEOUT },
	m_writeTimeout{ DEFAULT_WRITE_TIMEOUT },
	m_lineEnding{ DEFAULT_LINE_ENDING },
	m_writeMutex{},
	m_readMutex{}
{

}

void IByteStream::setReadTimeout(int timeout) {
    if (timeout < 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setReadTimeout(int): invariant failure (read timeout cannot be less than 0, " + toStdString(timeout) + " < 0)");
    }
    this->m_readTimeout = timeout;
}

int IByteStream::readTimeout() const {
    return this->m_readTimeout;

}

void IByteStream::setWriteTimeout(int timeout) {
    if (timeout < 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setWriteTimeout(int): invariant failure (write timeout cannot be less than 0, " + toStdString(timeout) + " < 0)");
    }
    this->m_writeTimeout = timeout;
}

int IByteStream::writeTimeout() const {
    return this->m_writeTimeout;
}

ByteArray IByteStream::lineEnding() const {
    return this->m_lineEnding;
}

void IByteStream::setLineEnding(const std::string &str) {
    if (str.length() == 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setLineEnding(const std::string &): str.length() == 0 (invariant failure)");
    }
    this->m_lineEnding = str;
}

void IByteStream::setLineEnding(const ByteArray &byteArray) {
    if (byteArray.length() == 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setLineEnding(const ByteArray &): byteArray.length() == 0 (invariant failure)");
    }
    this->m_lineEnding = byteArray;
}

void IByteStream::setLineEnding(char chr) {
    if (chr == '\0') {
        throw std::runtime_error("CppSerialPort::IByteStream::setLineEnding(char): chr == '\\0' (invariant failure)");
    }
    this->m_lineEnding = std::string(1, chr);
}

ssize_t IByteStream::writeLine(const std::string &str) {
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
	ByteArray toWrite{};
    toWrite += str;
    toWrite += this->lineEnding();
	return this->write(toWrite.data(), toWrite.length());
}

ssize_t IByteStream::write(const std::string &str) {
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
	return this->write(str.c_str(), str.length());
}

ssize_t IByteStream::writeLine(const ByteArray &byteArray) {
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
    ByteArray toWrite{};
    toWrite += byteArray;
    toWrite += this->lineEnding();
    return this->write(toWrite.data(), toWrite.length());
}

ssize_t IByteStream::write(const ByteArray &byteArray) {
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
    return this->write(byteArray.data(), byteArray.length());
}

ByteArray IByteStream::readLine(bool *timeout) {
    return this->readUntil(this->m_lineEnding, timeout);
}

ByteArray IByteStream::readUntil(const std::string &until, bool *timeout) {
    return this->readUntil(ByteArray{until}, timeout);
}

ByteArray IByteStream::readUntil(const ByteArray &until, bool *timeout) {
	std::lock_guard<std::mutex> readLock{ this->m_readMutex };
    auto startTime = IByteStream::getEpoch();
    ByteArray returnArray{""};
    if (timeout) {
        *timeout = false;
    }
    do {
        bool readTimeout{false};
        char maybeChar{this->read(&readTimeout)};
        if (readTimeout) {
            continue;
        }
        returnArray += maybeChar;
        if (returnArray.endsWith(until)) {
            return returnArray.subsequence(0, returnArray.length() - until.length());
        }
    } while ((IByteStream::getEpoch() - startTime) <= static_cast<unsigned long>(this->m_readTimeout));
    if (timeout) {
        *timeout = true;
    }
    return returnArray;
}

ByteArray IByteStream::readUntil(char until, bool *timeout) {
    return this->readUntil(ByteArray{until}, timeout);
}

bool IByteStream::fileExists(const std::string &fileToCheck) {
#if defined(_WIN32)
    DWORD dwAttrib{GetFileAttributes(fileToCheck.c_str())};
    return ( (dwAttrib != INVALID_FILE_ATTRIBUTES) && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY) );
#else
    return access(fileToCheck.c_str(), F_OK) != -1;
#endif //defined(_WIN32)
}

/*
#if defined(_MSC_VER)
#include <Windows.h>
uint64_t IByteStream::getEpoch() {
    FILETIME ft_now{};
    LONGLONG ll_now{(LONGLONG)ft_now.dwLowDateTime + ((LONGLONG)(ft_now.dwHighDateTime) << 32LL)};
    LONGLONG win32Epoch{ll_now/10000};
    LONGLONG unixEpoch{win32Epoch - 116444736000000000LL};
    return static_cast<uint64_t>(unixEpoch);
}
#else
#include <sys/time.h>
uint64_t IByteStream::getEpoch() {
    struct timeval tv{0, 0};
    gettimeofday(&tv, nullptr);
    return (static_cast<unsigned long long>(tv.tv_sec) * 1000) +
           (static_cast<unsigned long long>(tv.tv_usec) / 1000);
}
 #endif //defined(_WIN32)

 */

int64_t IByteStream::getEpoch() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}


} //namespace CppSerialPort
