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
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <climits>
    #include <sys/file.h>
    #include <cerrno>

#endif

#include "SerialPort.h"
#include <iostream>
#include <limits>
#include <allegro/internal/aintern.h>

namespace CppSerialPort {

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::Eight};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::One};
const Parity SerialPort::DEFAULT_PARITY{Parity::None};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::Baud9600};

#if defined(_WIN32)
    const char *SerialPort::AVAILABLE_PORT_NAMES_BASE{R"(\\.\COM)"};
    const char *SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const char *SerialPort::SERIAL_PORT_REGISTRY_PATH{R"(HARDWARE\DEVICEMAP\SERIALCOMM\)"};
#else
const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"/dev/ttyS", "/dev/ttyACM", "/dev/ttyUSB",
                                                                      "/dev/ttyAMA", "/dev/ttyrfcomm", "/dev/ircomm",
                                                                      "/dev/cuau", "/dev/cuaU", "/dev/rfcomm"};
#endif

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

    this->m_newPortSettings.c_cflag = static_cast<tcflag_t>(cbits | cpar | bstop | CLOCAL | CREAD);
    this->m_newPortSettings.c_iflag = static_cast<tcflag_t>(ipar);
    this->m_newPortSettings.c_oflag = 0;
    this->m_newPortSettings.c_lflag = 0;
    this->m_newPortSettings.c_cc[VMIN] = 0;
    this->m_newPortSettings.c_cc[VTIME] = static_cast<cc_t>(this->readTimeout() / 100);

    cfsetispeed(&this->m_newPortSettings, static_cast<speed_t>(baudRate));
    cfsetospeed(&this->m_newPortSettings, static_cast<speed_t>(baudRate));

    fcntl(this->getFileDescriptor(), F_SETFL, O_SYNC);

    if (this->readTimeout() == 0) {
        fcntl(this->getFileDescriptor(), F_SETFL, O_NDELAY);
    }

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

    /*
    if (this->m_flowControl == FlowControl::Off) {
        this->m_newPortSettings.c_cflag &= (~CRTSCTS);
        this->m_newPortSettings.c_iflag &= (~(IXON | IXOFF | IXANY));
        tcsetattr(fd, TCSAFLUSH, &this->m_newPortSettings);
    } else if (this->m_flowControl == FlowControl::XonXoff) {
        this->m_newPortSettings.c_cflag&=(~CRTSCTS);
        this->m_newPortSettings.c_iflag|=(IXON|IXOFF|IXANY);
        tcsetattr(fd, TCSAFLUSH, &this->m_newPortSettings);
    } else {
        this->m_newPortSettings.c_cflag|=CRTSCTS;
        this->m_newPortSettings.c_iflag&=(~(IXON|IXOFF|IXANY));
        tcsetattr(fd, TCSAFLUSH, &this->m_newPortSettings);
    }
    */

#endif
	this->m_isOpen = true;

}


std::pair<int, int> SerialPort::parseParity(Parity parity)
{
#if defined(_WIN32)
    if (parity == Parity::None) {
        return std::make_pair('n', 0);
    } else if (parity == Parity::Even) {
        return std::make_pair('e', 0);
    } else if (parity == Parity::Odd) {
        return std::make_pair('o', 0);
    }
#else
    if (parity == Parity::None) {
        return std::make_pair(0, IGNPAR);
    } else if (parity == Parity::Even) {
        return std::make_pair(PARENB, INPCK);
    } else if (parity == Parity::Odd) {
        return std::make_pair( (PARENB | PARODD), INPCK);
    }
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
    return static_cast<bool>(status & TIOCM_CAR);
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
    return static_cast<bool>(status & TIOCM_CTS);
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
    return static_cast<bool>(status & TIOCM_DSR);
#endif
}

void SerialPort::flushRx()
{
#if defined(_WIN32)
    PurgeComm(this->m_serialPortHandle, PURGE_RXCLEAR | PURGE_RXABORT);
#else
    tcflush(this->getFileDescriptor(), TCIFLUSH);
#endif
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

/*
void SerialPort::setFlowControl(FlowControl flowControl)
{
    if (!this->m_isOpen) {
        this->m_flowControl = flowControl;
    } else {
        throw std::runtime_error("ERROR: Cannot change flow control while serial port " + this->m_portName + " is open");
    }
}
*/

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

/*
FlowControl SerialPort::flowControl() const
{
    return this->m_flowControl;
}
*/

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
