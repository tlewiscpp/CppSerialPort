/***********************************************************************
*    IByteStream.h:                                                    *
*    IByteStream, base class for simple read and write operations      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.serial/tlewiscpp/CppSerialPort                     *
*    This file may be distributed with the CppSerialPort library,      *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an base class IByteStream     *
*    It includes very basic IO operations like read and write          *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef CPPSERIALPORT_IBYTESTREAM_H
#define CPPSERIALPORT_IBYTESTREAM_H

#include <string>
#include <sstream>
#include <mutex>

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

    virtual int read() = 0;
    virtual ssize_t write(int) = 0;

    virtual std::string portName() const = 0;
    virtual bool isOpen() const = 0;
    virtual void openPort() = 0;
    virtual void closePort() = 0;
    virtual void flushRx() = 0;
    virtual void flushTx() = 0;

    bool available();
    int peek();
	void setReadTimeout(int timeout);
    int readTimeout() const;

    void setWriteTimeout(int timeout);
    int writeTimeout() const;

    std::string lineEnding() const;
    void setLineEnding(const std::string &str);
    void setLineEnding(char chr);

    virtual ssize_t writeLine(const std::string &str);

    virtual std::string readLine(bool *timeout = nullptr);
    virtual std::string readUntil(const std::string &until, bool *timeout = nullptr);
    virtual std::string readUntil(char until, bool *timeout = nullptr);

protected:
    virtual void putBack(int c) = 0;
    static inline bool endsWith (const std::string &fullString, const std::string &ending) {
        return ( (fullString.length() < ending.length()) ? false : std::equal(ending.rbegin(), ending.rend(), fullString.rbegin()) );
    }
    template<typename T> static inline std::string toStdString(const T &t) {
        return dynamic_cast<std::ostringstream &>(std::ostringstream{""} << t).str();
    }

    static inline std::string vaToStdString() {
        return "";
    }

    template <typename First, typename ...Args> static inline std::string vaToStdString(First first, Args ...args) {
        return toStdString(first) + vaToStdString(args...);
    }

    static bool fileExists(const std::string &filePath);

	static const int DEFAULT_READ_TIMEOUT;
	static const int DEFAULT_WRITE_TIMEOUT;
	static uint64_t getEpoch();


private:
    int m_readTimeout;
    int m_writeTimeout;
    std::string m_lineEnding;
    std::mutex m_writeMutex;


    static const char *DEFAULT_LINE_ENDING;

    ssize_t write(const std::string &str);
};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_IBYTESTREAM_H
