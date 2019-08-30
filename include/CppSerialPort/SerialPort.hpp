#ifndef CPPSERIALPORT_SERIALPORT_HPP
#define CPPSERIALPORT_SERIALPORT_HPP

#include <string>
#include <vector>
#include <sstream>

#include "IByteStream.hpp"
#include <unordered_set>
#include <type_traits>

#if defined(_WIN32)
#include <windows.h>
typedef HANDLE file_descriptor_t;
#else
typedef int file_descriptor_t;
#endif //defined(_WIN32)

//Thanks to Jarod42
//https://stackoverflow.com/a/30561530/4791654
//Used to allow only 1 constructor definition to encompass
//all permutations of DataBits, StopBits, etc...


//c++11 or below
#if __cplusplus <= 201103L
namespace std {
    template< bool B, class T, class F > using conditional_t = typename conditional<B,T,F>::type;
    template< bool B, class T = void > using enable_if_t = typename enable_if<B,T>::type;
}
#endif

namespace PermutedConstructor 
{

    template <typename T, typename... Ts> struct has_T;

    template <typename T> struct has_T<T> : std::false_type {};

    template <typename T, typename... Ts> struct has_T<T, T, Ts...> : std::true_type {};

    template <typename T, typename Tail, typename... Ts>
    struct has_T<T, Tail, Ts...> : has_T<T, Ts...> {};

    template <typename T, typename... Ts>
    const T& get_or_default_impl(std::true_type,
                                 const std::tuple<Ts...>& t,
                                 const T&)
    {
        return std::get<T>(t);
    }

    template <typename T, typename... Ts>
    const T& get_or_default_impl(std::false_type,
                                 const std::tuple<Ts...>&,
                                 const T& default_value)
    {
        return default_value;
    }

    template <typename T1, typename T2> struct is_included;

    template <typename... Ts>
    struct is_included<std::tuple<>, std::tuple<Ts...>> : std::true_type {};

    template <typename T, typename... Ts, typename ... Ts2>
    struct is_included<std::tuple<T, Ts...>, std::tuple<Ts2...>> :
        std::conditional_t<has_T<T, Ts2...>::value,
                          is_included<std::tuple<Ts...>, std::tuple<Ts2...>>,
                          std::false_type> {};

}

template <typename T, typename... Ts>
const T& get_or_default(const std::tuple<Ts...>& t,
                        const T& default_value = T{})
{
    return PermutedConstructor::get_or_default_impl<T>(PermutedConstructor::has_T<T, Ts...>{}, t, default_value);
}



namespace CppSerialPort {

class SerialPortDisconnectedException : public std::runtime_error
{
public:
    explicit inline SerialPortDisconnectedException(const std::string &portName, const std::string &what) :
        std::runtime_error{what},
            m_portName{portName}
    {

    }

    SerialPortDisconnectedException(const SerialPortDisconnectedException &) = default;
    SerialPortDisconnectedException(SerialPortDisconnectedException &&) = default;
    SerialPortDisconnectedException &operator=(const SerialPortDisconnectedException &) = default;
    SerialPortDisconnectedException &operator=(SerialPortDisconnectedException &&) = default;
    ~SerialPortDisconnectedException() override = default;

    inline std::string portName() const {
        return this->m_portName;
    }

    inline void setPortName(const std::string &portName) {
        this->m_portName = portName;
    }

private:
    std::string m_portName;
};

enum class FlowControl {
    FlowOff,
    FlowHardware,
    FlowXonXoff
};


#if defined(_WIN32)

typedef DWORD modem_status_t;

enum class StopBits {
    StopOne     = ONESTOPBIT,
    StopOneFive = ONE5STOPBITS,
    StopTwo     = TWOSTOPBITS
};

enum class Parity : unsigned char {
    ParityNone  = NOPARITY,
    ParityOdd   = ODDPARITY,
    ParityEven  = EVENPARITY,
    ParityMark  = MARKPARITY,
    ParitySpace = SPACEPARITY
};

enum class DataBits {
    DataFive = 5,
    DataSix = 6,
    DataSeven = 7,
    DataEight = 8
};

enum class BaudRate {
    Baud110     = CBR_110,
    Baud300     = CBR_300,
    Baud600     = CBR_600,
    Baud1200    = CBR_1200,
    Baud2400    = CBR_2400,
    Baud4800    = CBR_4800,
    Baud9600    = CBR_9600,
    Baud19200   = CBR_19200,
    Baud38400   = CBR_38400,
    Baud57600   = CBR_57600,
    Baud115200  = CBR_115200,
    Baud128000  = CBR_128000,
    Baud256000  = CBR_256000,
    Baud230400  = 230400,
    Baud460800  = 460800,
    Baud500000  = 500000,
    Baud576000  = 576000,
    Baud921600  = 921600,
    Baud1000000 = 1000000,
    Baud1152000 = 1152000,
    Baud1500000 = 1500000,
    Baud2000000 = 2000000,
    Baud2500000 = 2500000,
    Baud3000000 = 3000000,
    Baud3500000 = 3500000,
    Baud4000000 = 4000000
};

#else
#include <termios.h>
typedef int modem_status_t;
enum class Parity {
    ParityEven,
    ParityOdd,
    ParityNone,
    ParitySpace
};
enum class StopBits {
    StopOne,
    StopTwo
};
enum class DataBits {
    DataFive = CS5,
    DataSix = CS6,
    DataSeven = CS7,
    DataEight = CS8
};
enum class BaudRate {
    Baud50 = B50,
    Baud75 = B75,
    Baud110 = B110,
    Baud134 = B134,
    Baud150 = B150,
    Baud200 = B200,
    Baud300 = B300,
    Baud600 = B600,
    Baud1200 = B1200,
    Baud1800 = B1800,
    Baud2400 = B2400,
    Baud4800 = B4800,
    Baud9600 = B9600,
    Baud19200 = B19200,
    Baud38400 = B38400,
    Baud57600 = B57600,
    Baud115200 = B115200,
    Baud230400 = B230400,
    Baud460800 = B460800,
    Baud500000 = B500000,
    Baud576000 = B576000,
    Baud921600 = B921600,
    Baud1000000 = B1000000,
    Baud1152000 = B1152000,
    Baud1500000 = B1500000,
    Baud2000000 = B2000000,
    Baud2500000 = B2500000,
    Baud3000000 = B3000000,
    Baud3500000 = B3500000,
    Baud4000000 = B4000000
};
#endif

class SerialPort : public IByteStream
{
public:
    explicit SerialPort(const std::string &name,
    BaudRate baudRate = DEFAULT_BAUD_RATE, 
    DataBits dataBits = DEFAULT_DATA_BITS, 
    StopBits stopBits = DEFAULT_STOP_BITS, 
    Parity parity = DEFAULT_PARITY, 
    FlowControl flowControl = DEFAULT_FLOW_CONTROL, 
    const std::string &lineEnding = DEFAULT_LINE_ENDING);

    //Thanks to Jarod42
    //https://stackoverflow.com/a/30561530/4791654
    //Used to allow only 1 constructor definition to encompass
    //all permutations of DataBits, StopBits, etc...
    template <typename ... Ts,
              typename std::enable_if_t<
                  PermutedConstructor::is_included<std::tuple<Ts...>,
                  std::tuple<const std::string &, BaudRate, DataBits, StopBits, Parity, FlowControl, const std::string &>>::value>* = nullptr>    
    explicit SerialPort(const Ts&... ts) :
        SerialPort{
            get_or_default<const std::string&>(std::tie(ts...)),
            get_or_default<BaudRate>(std::tie(ts...)),
            get_or_default<DataBits>(std::tie(ts...)),
            get_or_default<StopBits>(std::tie(ts...)),
            get_or_default<Parity>(std::tie(ts...)),
            get_or_default<FlowControl>(std::tie(ts...)),
            get_or_default<const std::string &>(std::tie(ts...))
        }
    {}

    SerialPort(SerialPort &&other) = delete;
    SerialPort &operator=(const SerialPort &rhs) = delete;
    SerialPort &operator=(SerialPort &&rhs) = delete;
    SerialPort(const SerialPort &other) = delete;
	~SerialPort() override;

    //IByteStream interface
	void openPort() override;
    void closePort() override;
    char read(bool *readTimeout) override;

    void setReadTimeout(int timeout) override;

    std::string portName() const override;
    bool isOpen() const override;

    bool isDCDEnabled() const;
    bool isCTSEnabled() const;
    bool isDSREnabled() const;
    void enableDTR();
    void disableDTR();
    void enableRTS();
    void disableRTS();
    void flushRx() override;
    void flushTx() override;
    ssize_t write(char c) override;
	ssize_t write(const char *bytes, size_t numberOfBytes) override;
    size_t available() override;

    void setBaudRate(BaudRate baudRate);
    void setStopBits(StopBits stopBits);
    void setParity(Parity parity);
    void setDataBits(DataBits dataBits);
    void setFlowControl(FlowControl flowControl);

    BaudRate baudRate() const;
    StopBits stopBits() const;
    DataBits dataBits() const;
    Parity parity() const;
    FlowControl flowControl() const;

    static const StopBits DEFAULT_STOP_BITS;
    static const Parity DEFAULT_PARITY;
    static const BaudRate DEFAULT_BAUD_RATE;
    static const DataBits DEFAULT_DATA_BITS;
    static const FlowControl DEFAULT_FLOW_CONTROL;
    static const std::string DEFAULT_LINE_ENDING;

    static std::unordered_set<std::string> availableSerialPorts();
    static bool isValidSerialPortName(const std::string &serialPortName);
    static const long DEFAULT_RETRY_COUNT;
    static bool isAvailableSerialPort(const std::string &name);
private:
    ByteArray m_readBuffer;
    std::string m_portName;
    int m_portNumber;
    BaudRate m_baudRate;
    StopBits m_stopBits;
    DataBits m_dataBits;
    Parity m_parity;
    FlowControl m_flowControl;
#if defined(_WIN32)
    HANDLE m_fileDescriptor;
#else
    int m_fileDescriptor;
#endif //defined(_WIN32)

    static const long constexpr SERIAL_PORT_BUFFER_MAX{4096};

    static std::pair<int, std::string> getPortNameAndNumber(const std::string &name);
    static std::vector<std::string> generateSerialPortNames();

    static const std::vector<std::string> SERIAL_PORT_NAMES;

#if defined(_WIN32)
	static const char *AVAILABLE_PORT_NAMES_BASE;
    static const char *SERIAL_PORT_REGISTRY_PATH;
    COMMCONFIG m_portSettings;
#else
	static const std::vector<const char *> AVAILABLE_PORT_NAMES_BASE;
    static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256*9};
    termios m_portSettings;
    termios m_oldPortSettings;
#endif //defined(_WIN32)
    file_descriptor_t getFileDescriptor() const;
    void applyPortSettings();
    modem_status_t getModemStatus() const;

    bool isDisconnected();
};

} //namespace CppSerialPort


#endif //CPPSERIALPORT_SERIALPORT_HPP
