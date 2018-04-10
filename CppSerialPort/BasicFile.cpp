#include "BasicFile.hpp"

#include "ErrorInformation.hpp"

#include <iostream>
#include <sstream>
#include <unordered_set>


#if defined(_WIN32)
#    define INVALID_FILE_DESCRIPTOR -1
#else
#    define INVALID_FILE_DESCRIPTOR -1
#endif //defined(_WIN32)

#define READ_BUFFER_MAX 1024

using ErrorInformation::getErrorString;
using ErrorInformation::getLastError;

BasicFile::BasicFile(const std::string &fileName) :
    m_fileName{""},
    m_fileHandle{nullptr}
{
    this->setFileName(fileName);
}

BasicFile::BasicFile() :
    m_fileName{""},
    m_fileHandle{nullptr}
{

}

BasicFile::BasicFile(BasicFile &&file) noexcept :
    m_fileName{std::move(file.m_fileName)},
    m_fileHandle{file.m_fileHandle}
{
    file.m_fileHandle = nullptr;
}

BasicFile &BasicFile::operator=(BasicFile &&file) noexcept {
    this->m_fileName = std::move(file.m_fileName);
    this->m_fileHandle = file.m_fileHandle;
    file.m_fileHandle = nullptr;
    return *this;
}

char BasicFile::read() {
    char c{};
    auto size = this->read(&c, 1);
    return (size == 1) ? c : static_cast<char>('\0');
}

size_t BasicFile::read(char *buffer, size_t maximum) {
    if (!this->isOpen()) {
        std::stringstream message{};
        message << "BasicFile::read(): file \"" << this->m_fileName << "\" cannot be read from while it is closed (call BasicFile::open() first)";
        throw std::runtime_error(message.str());
    }
    auto result = fread(buffer, sizeof(char), maximum, this->m_fileHandle);
    if (ferror(this->m_fileHandle)) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::read(): read from file \"" << this->m_fileName << "\" failed with error code " << errorCode << " (" << getErrorString(errorCode) << ")";
        throw std::runtime_error(message.str());
    }
    return result;
}

size_t BasicFile::write(char *buffer, size_t maximum) {
    if (!this->isOpen()) {
        std::stringstream message{};
        message << "BasicFile::write(): file \"" << this->m_fileName << "\" cannot be read from while it is closed (call BasicFile::open() first)";
        throw std::runtime_error(message.str());
    }
    auto result = fwrite(buffer, sizeof(char), maximum, this->m_fileHandle);
    if (ferror(this->m_fileHandle)) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::write(): write to file \"" << this->m_fileName << "\" failed with error code " << errorCode << " (" << getErrorString(errorCode) << ")";
        throw std::runtime_error(message.str());
    }
    return result;
}

size_t BasicFile::write(char c) {
    return this->write(&c, 1);
}

BasicFile &BasicFile::open(const std::string &fileName, const std::string &mode) {
    this->setFileName(fileName);
    return this->open(mode);
}

bool BasicFile::isAtEnd() {
    if (!this->isOpen()) {
        std::stringstream message{};
        message << "BasicFile::isAtEnd(): file \"" << this->m_fileName << "\" cannot be queried while it is closed (call BasicFile::open() first)";
        throw std::runtime_error(message.str());
    }
    return feof(this->m_fileHandle) != 0;
}

BasicFile &BasicFile::open(const std::string &mode) {
    if (this->m_fileName.empty()) {
        throw std::runtime_error("BasicFile::open(): current FileName parameter cannot be empty when opening a file (call BasicFile::setFileName() first");
    }
    if (!BasicFile::checkMode(mode)) {
        std::stringstream message{};
        message << "BasicFile::open(): Mode parameter \"" << mode << "\" is not a valid open mode";
        throw std::runtime_error(message.str());
    }
    auto handle = fopen(this->m_fileName.c_str(), mode.c_str());
    if (handle == nullptr) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::open(): fopen for FileName=\"" << this->m_fileName << "\", OpenMode=\"" << mode << "\" failed with error code " << errorCode << " (" << getErrorString(errorCode) << ")";
        throw std::runtime_error(message.str());
    }
    this->m_fileHandle = handle;
    return *this;
}

std::string BasicFile::fileName() const {
    return this->m_fileName;
}

BasicFile &BasicFile::setFileName(const std::string &fileName) {
    if (fileName.empty()) {
        throw std::runtime_error("BasicFile::setFileName(): FileName parameter cannot be empty");
    }
    if (this->isOpen()) {
        throw std::runtime_error("BasicFile::setFileName(): FileName cannot be set while file is currently open (call BasicFile::close() first)");
    }
    this->m_fileName = fileName;
    return *this;
}

bool BasicFile::isOpen() {
    return this->m_fileHandle != nullptr;
}

BasicFile &BasicFile::close() {
    auto returnCode = fclose(this->m_fileHandle);
    if (returnCode == -1) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::close(): fclose returned error code " << errorCode << " (" << getErrorString(errorCode) << ")" << std::endl;
        throw std::runtime_error(message.str());
    }
    return *this;
}

fd_t BasicFile::getFileDescriptor() {
#if defined(_WIN32)
    return this->m_fileHandle == nullptr ? INVALID_FILE_DESCRIPTOR : _osft_handle(this->m_fileHandle);
#else
    return this->m_fileHandle == nullptr ? INVALID_FILE_DESCRIPTOR : fileno(this->m_fileHandle);
#endif //defined(_WIN32)
}

file_handle_t *BasicFile::getFileHandle() {
    return this->m_fileHandle;
}


bool BasicFile::checkMode(const std::string &mode) {
    static const std::unordered_set<std::string> validModes {
            "b", "r", "rb", "w", "wb", "a", "ab", "r+", "rb+", "r+b", "w+", "wb+", "w+b", "a+", "ab+", "a+b"
    };
    return validModes.find(mode) != validModes.end();
}

BasicFile::~BasicFile() {
    if (this->m_fileHandle == nullptr) {
        return;
    }
    auto returnCode = fclose(this->m_fileHandle);
    if (returnCode == -1) {
        auto errorCode = getLastError();
        std::cerr << "BasicFile::~BasicFile(): fclose returned error code " << errorCode << " (" << getErrorString(errorCode) << ")" << std::endl;
    }
}

