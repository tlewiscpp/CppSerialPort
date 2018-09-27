#ifndef CPPSERIALPORT_BASICFILE_H
#define CPPSERIALPORT_BASICFILE_H

#include <cstdio>
#include <string>
#include <cstring>

#if defined(_WIN32)
using HANDLE = void*;
#endif //defined(_WIN32)

namespace CppSerialPort {

class BasicFile {
public:
    explicit BasicFile(const std::string &name);
    BasicFile();
    BasicFile(const BasicFile &file) = delete;
    BasicFile(BasicFile &&file) noexcept;
    BasicFile &operator=(const BasicFile &file) = delete;
    BasicFile &operator=(BasicFile &&file) noexcept;
    virtual ~BasicFile();

    std::string fileName() const;
    BasicFile &setFileName(const std::string &fileName);

    virtual char read();
    virtual size_t read(char *buffer, size_t maximum);
    virtual size_t write(const char *buffer, size_t maximum);
    virtual size_t write(char c);
    virtual BasicFile &open(const std::string &mode);
    virtual BasicFile &open(int fileDescriptor, const std::string &mode);
    virtual BasicFile &open(const std::string &name, const std::string &mode);
#if defined(_WIN32)
    virtual BasicFile &open(HANDLE nativeHandle, const std::string &mode);
#endif //defined(_WIN32)
    virtual BasicFile &close();
    virtual bool isOpen() const;
    virtual bool isAtEnd();

    virtual BasicFile &lockFile();
    virtual BasicFile &unlockFile();
    bool isLocked() const;

    int getFileDescriptor() const;
    FILE *getFileHandle() const;

#if defined(_WIN32)
    HANDLE getNativeHandle() const;
#endif //defined(_WIN320

private:
    std::string m_fileName;
    FILE *m_fileHandle;
    bool m_fileLock;

#if defined(_WIN32)
    HANDLE m_nativeHandle;
#endif //defined(_WIN32)

    static bool checkMode(const std::string &mode);

    enum OpenStyle {
        OpenFileDescriptor,
        OpenFileName,
#if defined(_WIN32)
        OpenNativeHandle
#endif //defined(_WIN32)
    };
    BasicFile &doOpen(OpenStyle openStyle, const std::string &mode);
    BasicFile &clearNativeHandles();
};


} //namespace CppSerialPort

#endif //CPPSERIALPORT_BASICFILE_H