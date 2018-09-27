#include <CppSerialPort/BasicFile.hpp>
#include <CppSerialPort/ErrorInformation.hpp>

#if defined(_WIN32)
#    include <io.h>
#    include <fileapi.h>
#else
#    include <sys/file.h>
#endif //defined(_WIN32)

#include <iostream>
#include <sstream>
#include <unordered_set>

#define INVALID_FILE_DESCRIPTOR -1

#define READ_BUFFER_MAX 1024

using ErrorInformation::getErrorString;
using ErrorInformation::getLastError;

namespace CppSerialPort {

BasicFile::BasicFile(const std::string &fileName) :
    m_fileName{""},
    m_fileHandle{nullptr},
    m_fileLock{false}
{
    this->setFileName(fileName);
}

BasicFile::BasicFile() :
    m_fileName{""},
    m_fileHandle{nullptr},
    m_fileLock{false}
{

}

BasicFile::BasicFile(BasicFile &&file) noexcept :
    m_fileName{std::move(file.m_fileName)},
    m_fileHandle{file.m_fileHandle},
    m_fileLock{file.m_fileLock}
{
    file.m_fileHandle = nullptr;

#if defined(_WIN32)
    this->m_nativeHandle = file.m_nativeHandle;
    file.m_nativeHandle = nullptr;
#endif //defined(_WIN32)
}

BasicFile &BasicFile::operator=(BasicFile &&file) noexcept {
    this->m_fileName = std::move(file.m_fileName);
    this->m_fileHandle = file.m_fileHandle;
    file.m_fileHandle = nullptr;
    this->m_fileLock = file.m_fileLock;

#if defined(_WIN32)
    this->m_nativeHandle = file.m_nativeHandle;
    file.m_nativeHandle = nullptr;
#endif //defined(_WIN32)

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

size_t BasicFile::write(const char *buffer, size_t maximum) {
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

    auto flushResult = fflush(this->m_fileHandle);
    if (flushResult == -1) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::write(): flush of file \"" << this->m_fileName << "\" failed with error code " << errorCode << " (" << getErrorString(errorCode) << ")";
        throw std::runtime_error(message.str());
    }
    return result;
}

size_t BasicFile::write(char c) {
    return this->write(&c, 1);
}

BasicFile &BasicFile::open(const std::string &fileName, const std::string &mode) {
    this->setFileName(fileName);
    return this->doOpen(OpenStyle::OpenFileName, mode);
}

BasicFile &BasicFile::open(int fileDescriptor, const std::string &mode) {
    if (fileDescriptor <= 0) {
        std::stringstream message{};
        message << "BasicFile::open(): FileDescriptor parameter cannot be less than or equal to 0 (" << fileDescriptor << " <= 0)";
        throw std::runtime_error(message.str());
    }
    this->setFileName(std::to_string(fileDescriptor));
    return this->doOpen(OpenStyle::OpenFileDescriptor, mode);
}

BasicFile &BasicFile::open(const std::string &mode) {
    return this->doOpen(OpenStyle::OpenFileName, mode);
}

#if defined(_WIN32)
BasicFile &BasicFile::open(HANDLE nativeHandle, const std::string &mode) {
    if (nativeHandle == nullptr) {
        throw std::runtime_error("BasicFile::open(): NativeHandle parameter cannot be a nullptr");
    }
    this->doOpen(BasicFile::OpenNativeHandle, mode);
    return *this;
}
#endif //defined(_WIN32)


BasicFile &BasicFile::doOpen(BasicFile::OpenStyle openStyle, const std::string &mode) {
    if (this->m_fileName.empty()) {
        throw std::runtime_error("BasicFile::doOpen(): current FileName parameter cannot be empty when opening a file (call BasicFile::setFileName() first");
    }
    if (!BasicFile::checkMode(mode)) {
        std::stringstream message{};
        message << "BasicFile::doOpen(): Mode parameter \"" << mode << "\" is not a valid open mode";
        throw std::runtime_error(message.str());
    }
    FILE *handle{nullptr};
    int fileDescriptor{-1};
    if (openStyle == OpenStyle::OpenFileDescriptor) {
        fileDescriptor = std::stoi(this->m_fileName);
        handle = fdopen(fileDescriptor, mode.c_str());
    } else if (openStyle == OpenStyle::OpenFileName){
        handle = fopen(this->m_fileName.c_str(), mode.c_str());
#if defined(_WIN32)
    } else if (openStyle == OpenStyle::OpenNativeHandle) {
        fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(this->m_nativeHandle), 0);
        handle = fdopen(fileDescriptor, mode.c_str());
#endif //defined(_WIN32)
    }

#if defined(_WIN32)
    if (this->m_nativeHandle == nullptr) {
        auto nativeHandle = reinterpret_cast<HANDLE>(_get_osfhandle(fileDescriptor));
        if (nativeHandle == nullptr) {
            auto errorCode = getLastError();
            std::stringstream message{};
            message << "ByteStream::doOpen(): Failed to get native handle (_get_osfhandle): error code " << errorCode << " (" << getErrorString(errorCode) << ")";
            throw std::runtime_error(message.str());
        }
        this->m_nativeHandle = nativeHandle;
    }
#endif //defined(_WIN32)

    if (handle == nullptr) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::doOpen(): fopen for FileName=\"" << this->m_fileName << "\", OpenMode=\"" << mode << "\" failed with error code " << errorCode << " (" << getErrorString(errorCode) << ")";
        throw std::runtime_error(message.str());
    }

    this->m_fileHandle = handle;
    return *this;
}


bool BasicFile::isAtEnd() {
    if (!this->isOpen()) {
        std::stringstream message{};
        message << "BasicFile::isAtEnd(): file \"" << this->m_fileName << "\" cannot be queried while it is closed (call BasicFile::open() first)";
        throw std::runtime_error(message.str());
    }
    return feof(this->m_fileHandle) != 0;
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

bool BasicFile::isOpen() const {
    return this->m_fileHandle != nullptr;
}

BasicFile &BasicFile::close() {
    if (this->isLocked()) {
        this->unlockFile();
    }
    if (this->m_fileHandle == nullptr) {
        return *this;
    }
    auto returnCode = fclose(this->m_fileHandle);
    if (returnCode == -1) {
        auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::close(): fclose returned error code " << errorCode << " (" << getErrorString(errorCode) << ")" << std::endl;
        throw std::runtime_error(message.str());
    }
    this->clearNativeHandles();
    return *this;
}

BasicFile &BasicFile::clearNativeHandles() {
#if defined(_WIN32)
    this->m_nativeHandle = nullptr;
#else

#endif //defined(_WIN32)
    this->m_fileHandle = nullptr;
    return *this;
}

int BasicFile::getFileDescriptor() const {
    return this->m_fileHandle == nullptr ? INVALID_FILE_DESCRIPTOR : fileno(this->m_fileHandle);
}

FILE *BasicFile::getFileHandle() const {
    return this->m_fileHandle;
}

bool BasicFile::checkMode(const std::string &mode) {
    static const std::unordered_set<std::string> validModes {
            "b", "r", "rb", "w", "wb", "a", "ab", "r+", "rb+", "r+b", "w+", "wb+", "w+b", "a+", "ab+", "a+b"
    };
    return validModes.find(mode) != validModes.end();
}

bool BasicFile::isLocked() const {
    return this->m_fileLock;
}

BasicFile &BasicFile::lockFile() {
    if (!this->isOpen()) {
        std::stringstream message{};
        message << "BasicFile::lockFile(): File cannot be locked while it is closed (call BasicFile::open() first)";
        throw std::runtime_error(message.str());
    }
#if defined(_WIN32)
    auto lockFileResult = LockFile(this->m_nativeHandle, 0, 0, MAXDWORD, MAXDWORD);
#else
    auto lockFileResult = flock(this->getFileDescriptor(), LOCK_EX | LOCK_NB);
#endif //defined(_WIN32)
    if (lockFileResult == -1) {
        const auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::open(): flock returned error code " << errorCode << " (" << getErrorString(errorCode) << ')';
        throw std::runtime_error(message.str());
    }
    this->m_fileLock = true;
    return *this;

}

BasicFile &BasicFile::unlockFile() {
    if (!this->m_fileLock) {
        return *this;
    }
#if defined(_WIN32)
    auto unlockFileResult = UnlockFile(this->getNativeHandle(), 0, 0, MAXDWORD, MAXDWORD);
#else
    auto unlockFileResult = flock(this->getFileDescriptor(), LOCK_UN);
#endif //defined(_WIN32)
    if (unlockFileResult == -1) {
        const auto errorCode = getLastError();
        std::stringstream message{};
        message << "BasicFile::unlockFile(): flock returned error code " << errorCode << " (" << getErrorString(errorCode) << ')';
        throw std::runtime_error(message.str());
    }
    this->m_fileLock = false;
    return *this;
}

native_handle_t BasicFile::getNativeHandle() const {
#if defined(_WIN32)
    return this->m_nativeHandle;
#else
    return this->getFileDescriptor();
#endif //defined(_WIN32)
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
    if (this->isLocked()) {
        try {
            this->unlockFile();
        } catch (const std::exception &e) {
            std::cerr << "BasicFile::~BasicFile(): BasicFile::unlockFile threw exception \"" << e.what() << "\"" << std::endl;
        }
    }
    this->clearNativeHandles();
}

} //namespace CppSerialPort

