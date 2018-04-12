#ifndef BASICFILE_BASICFILE_H
#define BASICFILE_BASICFILE_H

#include <cstdio>

#include <string>
#include <cstring>


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
    virtual size_t write(char *buffer, size_t maximum);
    virtual size_t write(char c);
    virtual BasicFile &open(const std::string &mode);
    virtual BasicFile &open(const std::string &name, const std::string &mode);
    virtual BasicFile &close();
    virtual bool isOpen();
    virtual bool isAtEnd();

protected:
    int getFileDescriptor();
    FILE *getFileHandle();

private:
    std::string m_fileName;
    FILE *m_fileHandle;

    static bool checkMode(const std::string &mode);
};

#endif //BASICFILE_BASICFILE_H