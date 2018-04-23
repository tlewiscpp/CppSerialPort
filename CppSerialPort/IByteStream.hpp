#ifndef CPPSERIALPORT_IBYTESTREAM_HPP
#define CPPSERIALPORT_IBYTESTREAM_HPP

#include <mutex>
#include <string>
#include <sstream>
#include "ByteArray.hpp"

#if defined(_WIN32)
#    ifndef PATH_MAX
#        define PATH_MAX MAX_PATH
#    endif
#    define ssize_t int
#else

#endif //defined(_WIN32)

namespace CppSerialPort {

class IByteStream
{
public:
    IByteStream();
    virtual ~IByteStream() = default;

	virtual char read(bool *timeout) = 0;
	virtual ssize_t write(char) = 0;
	virtual ssize_t write(const char *, size_t) = 0;
	virtual ssize_t write(const ByteArray &byteArray);

	virtual std::string portName() const = 0;
	virtual bool isOpen() const = 0;
	virtual void openPort() = 0;
	virtual void closePort() = 0;
	virtual void flushRx() = 0;
	virtual void flushTx() = 0;

	virtual size_t available() = 0;
	virtual void setReadTimeout(int timeout);
	int readTimeout() const;

	virtual void setWriteTimeout(int timeout);
	int writeTimeout() const;

	ByteArray lineEnding() const;
	void setLineEnding(const std::string &str);
    void setLineEnding(const ByteArray &str);
    void setLineEnding(char chr);

	virtual ssize_t writeLine(const std::string &str);
    virtual ssize_t writeLine(const ByteArray &byteArray);

    virtual ByteArray readLine(bool *timeout);
	virtual ByteArray readUntil(const ByteArray &until, bool *timeout);
    virtual ByteArray readUntil(const std::string &until, bool *timeout);
    virtual ByteArray readUntil(char until, bool *timeout);

protected:
	static bool fileExists(const std::string &filePath);

	template<typename T> static inline std::string toStdString(const T &t) {
        return dynamic_cast<std::ostringstream &>(std::ostringstream{""} << t).str();
    }

	static inline std::string stripLineEndings(const std::string &input) {
		ByteArray byteArray{input};
		if (byteArray.endsWith("\r\n")) {
            byteArray.popBack().popBack();
		} else if (byteArray.endsWith("\n\r")) {
            byteArray.popBack().popBack();
		} else if (byteArray.endsWith("\r")) {
            byteArray.popBack();
		} else if (byteArray.endsWith("\n")) {
            byteArray.popBack();
		}
		return byteArray.toString();
	}

	static const int DEFAULT_READ_TIMEOUT;
	static const int DEFAULT_WRITE_TIMEOUT;
	static uint64_t getEpoch();


private:
    int m_readTimeout;
    int m_writeTimeout;
    ByteArray m_lineEnding;
    std::mutex m_writeMutex;
	std::mutex m_readMutex;


    static const char *DEFAULT_LINE_ENDING;

    ssize_t write(const std::string &str);
};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_IBYTESTREAM_HPP
