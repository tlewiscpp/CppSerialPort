/***********************************************************************
*    SerialPort.cpp:                                                   *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.serial/tlewiscpp/CppSerialPort                     *
*    This file may be distributed with the entire CppSerialPort library*
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SerialPort class          *
*    It is used to connect to RS232 compliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <cstdio>
#include <cstring>
#include <utility>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <future>
#include <set>
#include <climits>

#if defined(_WIN32)
#    include <Windows.h>
#    include <io.h>
#    include <Fcntl.h>
//#    include <Winsock2.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <limits.h>
    #include <sys/file.h>
    #include <errno.h>
#endif

#include "SerialPort.h"
#include <iostream>
#include <limits>

namespace CppSerialPort {

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::EIGHT};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::ONE};
const Parity SerialPort::DEFAULT_PARITY{Parity::NONE};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::BAUD115200};

#if defined(_WIN32)
    const char *SerialPort::AVAILABLE_PORT_NAMES_BASE{R"(\\.\COM)"};
    const char *SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const char *SerialPort::SERIAL_PORT_REGISTRY_PATH{R"(HARDWARE\DEVICEMAP\SERIALCOMM\)"};
#else
const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"/dev/ttyS", "/dev/ttyACM", "/dev/ttyUSB",
                                                                      "/dev/ttyAMA", "/dev/ttyrfcomm", "/dev/ircomm",
                                                                      "/dev/cuau", "/dev/cuaU", "/dev/rfcomm"};
#endif

//#if defined(_WIN32) || defined(__CYGWIN__)
//    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"110", "300", "600", "1200", "2400", "4800",
//                                                                    "9600", "19200", "38400", "57600", "115200",
//                                                                    "128000", "256000", "500000", "1000000"};
//#else
//    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"50", "75", "110", "134", "150", "200", "300",
//                                                                    "600", "1200", "1800", "2400", "4800", "9600", "19200",
//                                                                    "38400", "57600", "115200", "230400", "460800", "500000",
//                                                                    "576000", "921600", "1000000", "1152000", "1500000",
//                                                                    "2000000", "2500000", "3000000", "3500000", "4000000"};
//#endif

const std::vector<std::string> SerialPort::SERIAL_PORT_NAMES{SerialPort::generateSerialPortNames()};

SerialPort::SerialPort(const std::string &name) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, dataBits, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits) :
        SerialPort(name, baudRate, stopBits, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, Parity parity) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, Parity parity) :
        SerialPort(name, baudRate, stopBits, DEFAULT_DATA_BITS, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity) :
        SerialPort(name, baudRate, stopBits, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, dataBits, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, dataBits, DEFAULT_PARITY)

{
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, dataBits, parity)
{
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, DEFAULT_DATA_BITS, parity)
{
}

SerialPort::SerialPort(const std::string &name, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, DataBits dataBits, Parity parity) :
        m_portName{name},
        m_portNumber{0},
        m_baudRate{baudRate},
        m_stopBits{stopBits},
        m_dataBits{dataBits},
        m_parity{parity},
        m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
#if defined(_WIN32)
    this->m_serialPortHandle = INVALID_HANDLE_VALUE;
#else
    this->m_fileStream = nullptr;
#endif //defined(_WIN32)
}

int SerialPort::getFileDescriptor() const
{
#if defined(_WIN32)
    return _get_osfhandle(reinterpret_cast<intptr_t>(this->m_serialPortHandle));
#else
    return fileno(this->m_fileStream);
#endif
}


void SerialPort::openPort()
{
	if (!isAvailableSerialPort(this->portName())) {
		throw std::runtime_error("ERROR: " + this->portName() + " is not a currently available serial port (is something else using it?)");
	}
#if defined(_WIN32)
    std::string mode{""};
    mode += "baud=" + std::to_string(static_cast<int>(this->m_baudRate)) + " ";
    mode += "data=" + std::to_string(static_cast<int>(this->m_dataBits)) + " ";
    mode += "parity=" + std::string(1, static_cast<char>(parseParity(this->m_parity).first)) + " ";
    mode += "stop=" + std::to_string(static_cast<int>(this->m_stopBits)) + " ";
    mode += DTR_RTS_ON_IDENTIFIER;

    this->m_serialPortHandle = CreateFileA(this->m_portName.c_str(),
                                                  GENERIC_READ|GENERIC_WRITE,
                                                  0,                          /* no share  */
                                                  nullptr,                    /* no security */
                                                  OPEN_EXISTING,
                                                  0,                          /* no threads */
		                                          nullptr);                   /* no templates */

    if(this->m_serialPortHandle == INVALID_HANDLE_VALUE) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE, HANDLE): Unable to open serial port " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    DCB portSettings{};
    memset(&portSettings, 0, sizeof(portSettings));
    portSettings.DCBlength = sizeof(portSettings);

    if(!BuildCommDCBA(mode.c_str(), &portSettings)) {
        const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("BuildCommDCBA(HANDLE, DCB*): Unable to set comm state settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    if(!SetCommState(this->m_serialPortHandle, &portSettings)) {
        const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("SetCommState(HANDLE, DCB*): Unable to set comm state settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    COMMTIMEOUTS commTimeouts{};
    commTimeouts.ReadIntervalTimeout         = MAXDWORD;
    commTimeouts.ReadTotalTimeoutMultiplier  = 0;
    commTimeouts.ReadTotalTimeoutConstant    = static_cast<DWORD>(this->readTimeout());
    commTimeouts.WriteTotalTimeoutMultiplier = 0;
    commTimeouts.WriteTotalTimeoutConstant   = static_cast<DWORD>(this->writeTimeout());

    if(!SetCommTimeouts(this->m_serialPortHandle, &commTimeouts)) {
        auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("SetCommTimeouts(HANDLE, COMMTIMEOUTS*): Unable to set timeout settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
    }
#else

    int status{0};
    int error{0};
    int baudRate{static_cast<int>(this->m_baudRate)};
    int cbits{static_cast<int>(this->m_dataBits)};
    std::pair<int, int> parityPair{parseParity(this->m_parity)};
    int cpar{parityPair.first};
    int ipar{parityPair.second};
    int bstop{static_cast<int>(this->m_stopBits)};
    this->m_fileStream = fopen(this->portName().c_str(), "r+");
    if (!this->m_fileStream) {
		const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("fopen(const char *, const char *): Unable to open FILE pointer for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
    }

    if(flock(this->getFileDescriptor(), LOCK_EX | LOCK_NB) != 0) {
		const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("flock(int, int): Unable to lock serial port " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    error = tcgetattr(this->getFileDescriptor(), this->m_oldPortSettings + this->m_portNumber);
    if (error == -1) {
		const auto errorCode = getLastError();
		this->closePort();
		throw std::runtime_error("tcgetattr(int, termios *): Unable to read port settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}
    memset(&this->m_newPortSettings, 0, sizeof(this->m_newPortSettings));

    this->m_newPortSettings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
    this->m_newPortSettings.c_iflag = ipar;
    this->m_newPortSettings.c_oflag = 0;
    this->m_newPortSettings.c_lflag = 0;
    this->m_newPortSettings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    this->m_newPortSettings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

    cfsetispeed(&this->m_newPortSettings, static_cast<speed_t>(baudRate));
    cfsetospeed(&this->m_newPortSettings, static_cast<speed_t>(baudRate));

    error = tcsetattr(this->getFileDescriptor(), TCSANOW, &this->m_newPortSettings);
    if(error == -1) {
		const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("tcsetattr(int, int, termios *): Unable to set IO speed settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
		const auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("ioctl(int, int, int): Unable to get DTR & RTS settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("ioctl(int, int, int): Unable to set DTR & RTS settings for " + this->portName() + ": " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ")");
	}

#endif
	this->m_isOpen = true;

}


std::pair<int, int> SerialPort::parseParity(Parity parity)
{
#if defined(_WIN32)
    if (parity == Parity::NONE) {
        return std::make_pair('n', 0);
    } else if (parity == Parity::EVEN) {
        return std::make_pair('e', 0);
    } else if (parity == Parity::ODD) {
        return std::make_pair('o', 0);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#else
    if (parity == Parity::NONE) {
        return std::make_pair(0, IGNPAR);
    } else if (parity == Parity::EVEN) {
        return std::make_pair(PARENB, INPCK);
    } else if (parity == Parity::ODD) {
        return std::make_pair( (PARENB | PARODD), INPCK);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#endif
}

int SerialPort::getLastError() {
#if defined(_WIN32)
	return static_cast<int>(GetLastError());
#else
	return errno;
#endif //defined(_WIN32
}

std::string SerialPort::getErrorString(int errorCode) {
	char errorString[PATH_MAX];
	memset(errorString, '\0', PATH_MAX);
#if defined(_WIN32)
	wchar_t *wideErrorString{ nullptr };
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
        static_cast<DWORD>(errorCode),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&wideErrorString),
		0,
		nullptr
	);
	size_t converted{ 0 };
	auto conversionResult = wcstombs_s(&converted, errorString, PATH_MAX, wideErrorString, PATH_MAX);
	(void)conversionResult;
	//wcstombs(errorString, wideErrorString, PATH_MAX);
	LocalFree(wideErrorString);
#else
    strerror_r(errorCode, errorString, PATH_MAX);
#endif //defined(_WIN32)
	return std::string{ errorString };
}

int SerialPort::read()
{
#if defined(_WIN32)
    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }

	static char readStuff[SERIAL_PORT_BUFFER_MAX];
	memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

	DWORD commErrors{};
	COMSTAT commStatus{};
	auto clearErrorsResult = ClearCommError(this->m_serialPortHandle, &commErrors, &commStatus);
	if (clearErrorsResult == 0) {
		const auto errorCode = getLastError();
		std::cout << "ClearCommError(HANDLE, LPDWORD, LPCOMSTAT) error: " << toStdString(errorCode) << " (" << getErrorString(errorCode) << ")" << std::endl;
	}

    DWORD readBytes{0};
	DWORD maxBytes{ commStatus.cbInQue };
    bool firstByte{false};
    if (commStatus.cbInQue == 0) {
        maxBytes = 1;
        firstByte = true;
    }

    auto readResult = ReadFile(this->m_serialPortHandle, readStuff, maxBytes, &readBytes, nullptr);
    if (readResult == 0) {
		const auto errorCode = getLastError();
		std::cout << "ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPDWORD) error: " << toStdString(errorCode) << " (" << getErrorString(errorCode) << ")" << std::endl;
		return 0;
	}
	if ( ( readBytes <= 0 ) || (readStuff[0] == '\0') ) {
		return 0;
    }
    this->m_readBuffer += std::string{readStuff};
    if (firstByte) {
        clearErrorsResult = ClearCommError(this->m_serialPortHandle, &commErrors, &commStatus);
        if ( (clearErrorsResult != 0) && (commStatus.cbInQue != 0) ) {
            readResult = ReadFile(this->m_serialPortHandle, readStuff, commStatus.cbInQue, &readBytes, nullptr);
            if ((readResult != 0) && (readBytes > 0)) {
                this->m_readBuffer += readStuff;
            }
        }
    }
    char returnValue{this->m_readBuffer.front()};
    this->m_readBuffer = this->m_readBuffer.substr(1);
    return static_cast<int>(returnValue);
#else
    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }

    // Initialize file descriptor sets
    fd_set read_fds{0, 0, 0};
    fd_set write_fds{0, 0, 0};
    fd_set except_fds{0, 0, 0};
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(this->getFileDescriptor(), &read_fds);

    struct timeval timeout{0, 0};
    timeout.tv_sec = 0;
    timeout.tv_usec = (this->readTimeout() * 1000);
    static char readStuff[SERIAL_PORT_BUFFER_MAX];
    memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (select(this->getFileDescriptor() + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
        int bytesAvailable{0};
        ioctl(this->getFileDescriptor(), FIONREAD, &bytesAvailable);
        auto returnedBytes = fread(readStuff, sizeof(char), static_cast<size_t>(bytesAvailable), this->m_fileStream);
        //auto returnedBytes = fread(&readValue, sizeof(char), 1, this->m_fileStream);
        if ((returnedBytes <= 0) || (readStuff[0] == '\0')) {
            return 0;
        }
        this->m_readBuffer += std::string{readStuff};
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }
    return 0;
#endif
}

ssize_t SerialPort::write(char c)
{
#if defined(_WIN32)
    DWORD writtenBytes{0};
	if (!WriteFile(this->m_serialPortHandle, &c, 1 , &writtenBytes, nullptr)) {
        auto errorCode = getLastError();
        (void)errorCode;
        //TODO: Check if errorCode is IO_NOT_COMPLETED or whatever
        return 0;
    }
    return static_cast<ssize_t>(writtenBytes);
#else
    auto writtenBytes = ::write(this->getFileDescriptor(), &c, 1);
    if (writtenBytes != 0) {
        return (getLastError() == EAGAIN ? 0 : writtenBytes);
    }
    return writtenBytes;
#endif //defined(_WIN32)
}

ssize_t SerialPort::write(const char *bytes, size_t numberOfBytes) {
#if defined(_WIN32)
	DWORD writtenBytes{ 0 };
	if (!WriteFile(this->m_serialPortHandle, bytes, numberOfBytes, &writtenBytes, nullptr)) {
		auto errorCode = getLastError();
		(void)errorCode;
		//TODO: Check if errorCode is IO_NOT_COMPLETED or whatever
		return 0;
	}
	return static_cast<ssize_t>(writtenBytes);
#else
	auto writtenBytes = ::write(this->getFileDescriptor(), bytes, numberOfBytes);
	if (writtenBytes != 0) {
		return (getLastError() == EAGAIN ? 0 : writtenBytes);
	}
	return writtenBytes;
#endif //defined(_WIN32)
}

void SerialPort::closePort()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
	CancelIo(this->m_serialPortHandle);
    CloseHandle(this->m_serialPortHandle);
#else
    int status{0};
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_DTR;    /* turn off DTR */
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
    }

    tcsetattr(this->getFileDescriptor(), TCSANOW, this->m_oldPortSettings + this->m_portNumber);
    //close(this->getFileDescriptor());
    flock(this->getFileDescriptor(), LOCK_UN);
    fclose(this->m_fileStream);
#endif
	this->m_isOpen = false;
}


void SerialPort::enableDTR()
{
#if defined(_WIN32)
    EscapeCommFunction(this->m_serialPortHandle, SETDTR);
#else
    int status{0};
    status |= TIOCM_DTR;    /* turn on DTR */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableDTR()
{
#if defined(_WIN32)
    EscapeCommFunction(this->m_serialPortHandle, CLRDTR);
#else
    int status{0};
    status &= ~TIOCM_DTR;    /* turn off DTR */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::enableRTS()
{

#if defined(_WIN32)
    EscapeCommFunction(this->m_serialPortHandle, SETRTS);
#else
    int status{0};
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableRTS()
{
#if defined(_WIN32)
    EscapeCommFunction(this->m_serialPortHandle, CLRRTS);
#else
    int status;
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

bool SerialPort::isDCDEnabled() const
{
#if defined(_WIN32)
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, reinterpret_cast<LPDWORD>((static_cast<void *>(&status))));
        return (status & MS_RLSD_ON) != 0;
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_CAR);
#endif
}


bool SerialPort::isCTSEnabled() const
{
#if defined(_WIN32)
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, reinterpret_cast<LPDWORD>((static_cast<void *>(&status))));
        return (status & MS_CTS_ON) != 0;
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_CTS);
#endif
}

bool SerialPort::isDSREnabled() const
{
#if defined(_WIN32)
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, reinterpret_cast<LPDWORD>((static_cast<void *>(&status))));
        return (status & MS_DSR_ON) != 0;
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_DSR);
#endif
}

void SerialPort::flushRx()
{
#if defined(_WIN32)
    PurgeComm(this->m_serialPortHandle, PURGE_RXCLEAR | PURGE_RXABORT);
#else
    tcflush(this->getFileDescriptor(), TCIFLUSH);
#endif
    int readValue{0};
    do {
        readValue = this->read();
    } while (readValue != 0);
}


void SerialPort::flushTx()
{
#if defined(_WIN32)
    PurgeComm(this->m_serialPortHandle, PURGE_TXCLEAR | PURGE_TXABORT);
#else
    tcflush(this->getFileDescriptor(), TCOFLUSH);
#endif
}

bool SerialPort::isAvailableSerialPort(const std::string &name)
{
	auto availablePorts = availableSerialPorts();
#if defined(_WIN32)
    std::string copyName{name};
    copyName.erase(std::remove_if(copyName.begin(), copyName.end(), [](char c) { return ( (c == '.') || (c == '\\') ); }), copyName.end());
	return (availablePorts.find(copyName) != availablePorts.end());
#else
	return (availablePorts.find(name) != availablePorts.end());
#endif //defined(_WIN32)
}

std::pair<int, std::string> SerialPort::getPortNameAndNumber(const std::string &name)
{
#if defined(_WIN32)
	auto foundCom = name.find("COM");
	if ((foundCom != 0) || (name.length() == 3)) {
		throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
	}
	try {
		int comNumber{ std::stoi(name.substr(3)) };
            return std::make_pair(comNumber, AVAILABLE_PORT_NAMES_BASE + toStdString(comNumber));
	} catch (std::exception &e) {
		(void)e;
		throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
	}

#else
    std::string str{name};
    auto iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
    str = name;
    if (str.find("/dev/tty") == std::string::npos) {
        str = "/dev/tty" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
    str = name;
    if (str.find("/dev/") == std::string::npos) {
        str = "/dev/" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }

    throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
#endif
}

bool SerialPort::isOpen() const
{
    return this->m_isOpen;
}

void SerialPort::setDataBits(DataBits dataBits)
{
    if (!this->m_isOpen) {
        this->m_dataBits = dataBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change data bits while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setBaudRate(BaudRate baudRate)
{
    if (!this->m_isOpen) {
        this->m_baudRate = baudRate;
    } else {
        throw std::runtime_error("ERROR: Cannot change baud rate while serial port " + this->portName() + " is open");
    }
}

void SerialPort::setStopBits(StopBits stopBits)
{
    if (!this->m_isOpen) {
        this->m_stopBits = stopBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change stop bits while serial port " + this->portName() + " is open");
    }
}

void SerialPort::setParity(Parity parity)
{
    if (!this->m_isOpen) {
        this->m_parity = parity;
    } else {
        throw std::runtime_error("ERROR: Cannot change parity while serial port " + this->portName() + " is open");
    }
}

BaudRate SerialPort::baudRate() const
{
    return this->m_baudRate;
}

StopBits SerialPort::stopBits() const
{
    return this->m_stopBits;
}

DataBits SerialPort::dataBits() const
{
    return this->m_dataBits;
}

Parity SerialPort::parity() const
{
    return this->m_parity;
}

std::string SerialPort::portName() const
{
#if defined(_WIN32)
    std::string copyName{this->m_portName};
    copyName.erase(std::remove_if(copyName.begin(), copyName.end(), [](char c) { return ( (c == '.') || (c == '\\') ); }), copyName.end());
    return copyName;
#else
    return this->m_portName;
#endif //defined(_WIN32)
}


BaudRate SerialPort::parseBaudRateFromRaw(const std::string &baudRate)
{
    std::string copyString{baudRate};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
#if defined(_WIN32)
    if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600;
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200;
    } else if (copyString == "128000") {
        return BaudRate::BAUD128000;
    } else if (copyString == "256000") {
        return BaudRate::BAUD256000;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#else
    if (copyString == "50") {
        return BaudRate::BAUD50;
    } else if (copyString == "75") {
        return BaudRate::BAUD75;
    } else if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "134") {
        return BaudRate::BAUD134;
    } else if (copyString == "150") {
        return BaudRate::BAUD150;
    } else if (copyString == "200") {
        return BaudRate::BAUD200;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200;
    } else if (copyString == "1800") {
        return BaudRate::BAUD1800;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600;
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200;
    } else if (copyString == "230400") {
        return BaudRate::BAUD230400;
    } else if (copyString == "460800") {
        return BaudRate::BAUD460800;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "576000") {
        return BaudRate::BAUD576000;
    } else if (copyString == "921600") {
        return BaudRate::BAUD921600;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else if (copyString == "1152000") {
        return BaudRate::BAUD1152000;
    } else if (copyString == "1500000") {
        return BaudRate::BAUD1500000;
    } else if (copyString == "2000000") {
        return BaudRate::BAUD2000000;
    } else if (copyString == "2500000") {
        return BaudRate::BAUD2500000;
    } else if (copyString == "3000000") {
        return BaudRate::BAUD3000000;
    } else if (copyString == "3500000") {
        return BaudRate::BAUD3500000;
    } else if (copyString == "4000000") {
        return BaudRate::BAUD4000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#endif
}

DataBits SerialPort::parseDataBitsFromRaw(const std::string &dataBits)
{
    std::string copyString{dataBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "eight") || (copyString == "8")) {
        return DataBits::EIGHT;
    } else if ((copyString == "seven") || (copyString == "7")) {
        return DataBits::SEVEN;
    } else if ((copyString == "six") || (copyString == "6")) {
        return DataBits::SIX;
    } else if ((copyString == "five") || (copyString == "5")) {
        return DataBits::FIVE;
    } else {
        throw std::runtime_error("Invalid data bits passed to parseDataBitsFromRaw(const std::string &): " + dataBits);
    }
}

StopBits SerialPort::parseStopBitsFromRaw(const std::string &stopBits)
{
    std::string copyString{stopBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "one") || (copyString == "1")) {
        return StopBits::ONE;
    } else if ((copyString == "two" || copyString == "2")) {
        return StopBits::TWO;
    } else {
        throw std::runtime_error("Invalid stop bits passed to parseStopBitsFromRaw(const std::string &): " + stopBits);
    }
}

Parity SerialPort::parseParityFromRaw(const std::string &parity)
{
    std::string copyString{parity};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "none") || (copyString == "n")) {
        return Parity::NONE;
    } else if ((copyString == "even") || (copyString == "e")) {
        return Parity::EVEN;
    } else if ((copyString == "odd") || (copyString == "o")) {
        return Parity::ODD;
    } else {
        throw std::runtime_error("Invalid parity passed to parseParityFromRaw(const std::string &): " + parity);
    }
}

std::string SerialPort::baudRateToString(BaudRate baudRate)
{
#if defined(_WIN32)
    if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600";
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200";
    } else if (baudRate == BaudRate::BAUD128000) {
        return "128000";
    } else if (baudRate == BaudRate::BAUD256000) {
        return "256000";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else {
        return "";
    }
#else
    if (baudRate == BaudRate::BAUD50) {
        return "50";
    } else if (baudRate == BaudRate::BAUD75) {
        return "75";
    } else if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD134) {
        return "134";
    } else if (baudRate == BaudRate::BAUD150) {
        return "150";
    } else if (baudRate == BaudRate::BAUD200) {
        return "200";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200";
    } else if (baudRate == BaudRate::BAUD1800) {
        return "1800";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600";
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200";
    } else if (baudRate == BaudRate::BAUD230400) {
        return "230400";
    } else if (baudRate == BaudRate::BAUD460800) {
        return "460800";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD576000) {
        return "576000";
    } else if (baudRate == BaudRate::BAUD921600) {
        return "921600";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else if (baudRate == BaudRate::BAUD1152000) {
        return "1152000";
    } else if (baudRate == BaudRate::BAUD1500000) {
        return "1500000";
    } else if (baudRate == BaudRate::BAUD2000000) {
        return "2000000";
    } else if (baudRate == BaudRate::BAUD2500000) {
        return "2500000";
    } else if (baudRate == BaudRate::BAUD3000000) {
        return "3000000";
    } else if (baudRate == BaudRate::BAUD3500000) {
        return "3500000";
    } else if (baudRate == BaudRate::BAUD4000000) {
        return "4000000";
    } else {
        return "";
    }
#endif
}
std::string SerialPort::stopBitsToString(StopBits stopBits)
{
    if (stopBits == StopBits::ONE) {
        return "1";
    } else if (stopBits == StopBits::TWO) {
        return "2";
    } else {
        return "";
    }
}

std::string SerialPort::dataBitsToString(DataBits dataBits)
{
    if (dataBits == DataBits::EIGHT) {
        return "8";
    } else if (dataBits == DataBits::SEVEN) {
        return "7";
    } else if (dataBits == DataBits::SIX) {
        return "6";
    } else if (dataBits == DataBits::FIVE) {
        return "5";
    } else {
        return "";
    }
}

std::string SerialPort::parityToString(Parity parity)
{
    if (parity == Parity::EVEN) {
        return "Even";
    } else if (parity == Parity::ODD) {
        return "Odd";
    } else if (parity == Parity::NONE) {
        return "None";
    } else {
        return "";
    }
}

std::unordered_set<std::string> SerialPort::availableSerialPorts()
{
    std::unordered_set<std::string> returnSet;
#if defined(_WIN32)
    try {
        HKEY hRegistryKey;
        LONG operationResult{ RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, 0, KEY_READ, &hRegistryKey) };
        if (operationResult != ERROR_SUCCESS) {
            return returnSet;
        }
        for (DWORD index = 0; ; index++) {
            char SubKeyName[PATH_MAX];
            DWORD cName{ PATH_MAX };
            DWORD cbData{ PATH_MAX };
            char hRegistryKeyValue[PATH_MAX];
            operationResult = RegEnumValueA(hRegistryKey, index, SubKeyName, &cName, nullptr, nullptr, nullptr, nullptr);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            operationResult = RegGetValueA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, SubKeyName, RRF_RT_REG_SZ, nullptr, hRegistryKeyValue, &cbData);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            returnSet.emplace(hRegistryKeyValue);
        }
        RegCloseKey(hRegistryKey);
        return returnSet;
    } catch (std::exception &e) {
        (void)e;
        return returnSet;
    }
#else
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (IByteStream::fileExists(std::string{it})) {
            returnSet.emplace(it);
        }
    }
    return returnSet;
#endif
}

std::vector<std::string> SerialPort::generateSerialPortNames()
{
    std::vector<std::string> returnSet;
#if defined(_WIN32)
	for (int i = 0; i < UCHAR_MAX; i++) {
		returnSet.push_back(AVAILABLE_PORT_NAMES_BASE + toStdString(i));
	}
#else
	for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
		for (int i = 0; i < UCHAR_MAX; i++) {
			returnSet.push_back(it + toStdString(i));
		}
	}
#endif //defined(_WIN32)
    return returnSet;
}

bool SerialPort::isValidSerialPortName(const std::string &serialPortName)
{
#if defined(_WIN32)
	auto foundCom = serialPortName.find("COM");
	if ( (foundCom == std::string::npos) || (foundCom != 0) || (serialPortName.length() == 3)) {
		return false;
	}
	try {
		int comNumber{ std::stoi(serialPortName.substr(3)) };
		return ((comNumber > 0) && (comNumber < 256));
	} catch (std::exception &e) {
		return false;
	}
#else
    for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
        for (int i = 0; i < 256; i++) {
            if (serialPortName == (it + toStdString(i))) {
                return true;
            }
        }
    }
    return false;
#endif
}

void SerialPort::putBack(int c)
{
    if (this->m_readBuffer.length() > 0) {
        this->m_readBuffer.insert(this->m_readBuffer.begin(), static_cast<char>(c));
    } else {
#if !defined(_WIN32)
        ungetc(c, this->m_fileStream);
#endif //!defined(_WIN32)
    }
}

SerialPort::~SerialPort() 
{
	this->closePort();
}

} //namespace CppSerialPort
