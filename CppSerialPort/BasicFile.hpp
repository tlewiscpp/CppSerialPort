#ifndef BASICFILE_BASICFILE_H
#define BASICFILE_BASICFILE_H

#include <cstdio>

#include <string>
#include <cstring>

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
    virtual BasicFile &close();
    virtual bool isOpen() const;
    virtual bool isAtEnd();

    virtual BasicFile &lockFile();
    virtual BasicFile &unlockFile();
    bool isLocked() const;

    int getFileDescriptor() const;
    FILE *getFileHandle() const;

    BasicFile &setFileHandle(FILE *fileHandle);

private:
    std::string m_fileName;
    FILE *m_fileHandle;
    bool m_fileLock;

    static bool checkMode(const std::string &mode);

    enum OpenStyle {
        OpenFileDescriptor,
        OpenFileName
    };
    BasicFile &doOpen(OpenStyle openStyle, const std::string &mode);
};


} //namespace CppSerialPort

#endif //BASICFILE_BASICFILE_H
