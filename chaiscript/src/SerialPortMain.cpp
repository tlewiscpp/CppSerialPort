#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <getopt.h>
#include "ChaiScriptProgramOption.hpp"
#include "CppSerialPortChaiScriptModuleFactory.hpp"

#include "Random.hpp"
#include "SerialPort.hpp"
#include <chaiscript/chaiscript.hpp>

static const ProgramOption portOption   {'p', "port",   required_argument, "Specify the serial port to use (ex COM4 or /dev/ttyUSB1)" };
static const ProgramOption scriptOption {'s', "script", required_argument, "Specify the script to execute (ex C:/Path/To/Script.chai (absolute) or Script.chai (relative) path)" };
static const ProgramOption helpOption   {'h', "help",   no_argument,       "Display help text and exit"};

static const option longOptions[] {
        portOption.toPosixOption(),
        scriptOption.toPosixOption(),
        helpOption.toPosixOption(),
        {nullptr, 0, nullptr, 0}
};

template <typename T, size_t N> inline constexpr size_t arraySize( T (&t)[N] ) { (void)t; return N; }

static const size_t PROGRAM_OPTION_COUNT{arraySize(longOptions)-1};

std::array<const ProgramOption *, PROGRAM_OPTION_COUNT> programOptions{
        &portOption,
        &scriptOption,
        &helpOption
};

void displayHelp();
void delay(unsigned int howLong);
bool startsWith(const std::string &str, const std::string &start);
bool startsWith(const std::string &str, char start);
bool endsWith(const std::string &str, const std::string &ending);
bool endsWith(const std::string &str, char end);

void notifyScriptError(std::exception *e);

#if defined(_WIN32)
    static const char * const SERIAL_PORT_BASE{"COM"};
#else
    static const char * const SERIAL_PORT_BASE{"/dev/"};
#endif //defined(_WIN32)

static const char * const CHAI_SCRIPT_EXTENSION{"chai"};

unsigned long milliseconds();
unsigned long seconds();
unsigned long minutes();
unsigned long hours();

std::string tryParseSerialPortName(std::string portName);
std::string tryParseScriptName(std::string scriptName);


int main(int argc, char *argv[])
{
    if (argc == 1) {
        displayHelp();
        exit(EXIT_FAILURE);
    }
    std::string serialPortName{"/dev/ttyACM0"};
    std::string scriptName{""};
    bool useStdin{false};
    
	int optionIndex{0};
    int currentOption{0};
    opterr = 0;

    while ( (currentOption = getopt_long(argc, argv, ProgramOption::buildShortOptions(programOptions).c_str(), longOptions, &optionIndex)) != -1) {
        switch (currentOption) {    
            case 'h':
                displayHelp();
                exit(EXIT_SUCCESS);
            case 'p':
                serialPortName = tryParseSerialPortName(optarg);
                break;
            case 's':
                scriptName = tryParseScriptName(optarg);
                break;
            default:
                break;
        }
	}
    for (int i = 1; i < argc; i++) {
        if ( (startsWith(argv[i], SERIAL_PORT_BASE)) && (serialPortName.empty()) ) {
            serialPortName = argv[i];
        } else if ( (endsWith(argv[i], CHAI_SCRIPT_EXTENSION)) && (scriptName.empty()) ) {
            scriptName = argv[i];
        } else if ( (strlen(argv[i]) == 2) && (argv[i][0] == '-') && (argv[i][1] == '-') ) {
           useStdin = true;
        }
    }
    if ( (!useStdin) && (scriptName.length() == 0) ) {
        std::cout << "No script name specified, using stdin" << std::endl;
        useStdin = true;
    } else {
        std::cout << "Using ScriptName=\"" << scriptName << "\"" << std::endl;
    }
    std::cout << "Using SerialPort=\"" << serialPortName << "\"" << std::endl;
    chaiscript::ChaiScript chaiEngine{};

    chaiEngine.add(CppSerialPort::CppSerialPortChaiScriptModuleFactory{}.createModule());
    
    std::shared_ptr<CppSerialPort::SerialPort> serialPort{
            std::make_shared<CppSerialPort::SerialPort>(serialPortName,
                                                          CppSerialPort::BaudRate::Baud115200,
                                                          CppSerialPort::DataBits::DataEight,
                                                          CppSerialPort::StopBits::StopOne,
                                                          CppSerialPort::Parity::ParityNone,
                                                          CppSerialPort::FlowControl::FlowOff,
                                                          "\n")
    };

    std::shared_ptr<Random> random{std::make_shared<Random>()};

    chaiEngine.add_global(chaiscript::var(serialPort), "serialPort");
    chaiEngine.add(chaiscript::fun(static_cast<int(Random::*)(int, int)>(&Random::randomBetween<int>), random.get()), "randomBetween");
    chaiEngine.add(chaiscript::fun(&delay), "delay");
    chaiEngine.add(chaiscript::fun(&milliseconds), "millis");
    chaiEngine.add(chaiscript::fun(&seconds), "seconds");
    chaiEngine.add(chaiscript::fun(&minutes), "minutes");
    chaiEngine.add(chaiscript::fun(&hours), "hours");
    serialPort->openPort();

    if (useStdin) {
        std::string tempBuffer{""};
        std::string scriptBuffer{""};
        std::cout << "Reading from stdin" << std::endl;
        std::cout << "Press CTRL+C at any time to quit, or CTRL+D to execute entered code" << std::endl;
		//std::cout << '>';
		while (std::getline(std::cin, tempBuffer)) {
            scriptBuffer += tempBuffer;
            scriptBuffer += '\n';
            //std::cout << '>';
        }
        std::cout << "Code entered, executing with ChaiScript engine" << std::endl;
        
        try {
            chaiEngine.eval(scriptBuffer);
        } catch (std::exception &e) {
            notifyScriptError(&e);
            exit(EXIT_FAILURE);
        }

    } else {
        try {
            chaiEngine.eval_file(scriptName.c_str());
        } catch (std::exception &e) {
            notifyScriptError(&e);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);

}


std::string tryParseSerialPortName(std::string portName) {
    if (startsWith(portName, '=')) {
        portName.erase(portName.begin());
    }
    if (!CppSerialPort::SerialPort::isAvailableSerialPort(portName)) {
        return portName;
    } else {
        std::cout << "WARNING: SerialPortName \"" << portName << "\" is not an available serial port" << std::endl;
        return "";
    }
}

std::string tryParseScriptName(std::string scriptName) {
    if (startsWith(scriptName, '=')) {
        scriptName.erase(scriptName.begin());
    }
    return scriptName;
}

void notifyScriptError(std::exception *e) {
    chaiscript::exception::eval_error *evalError{nullptr};
    if ( (evalError = dynamic_cast<chaiscript::exception::eval_error *>(e)) != nullptr) {
		std::cout << "Evaluation Error: " << evalError->pretty_print() << std::endl;
    } else {
        std::cout << "Error: " << e->what() << std::endl;
    }
}

bool startsWith(const std::string &str, const std::string &start) {
    if (start.length() > str.length()) {
        return false;
    }
    return std::equal(start.begin(), start.end(), str.begin());
}


bool startsWith(const std::string &str, char start) {
    return ((str.length() > 0) && (str.back() == start));
}

bool endsWith(const std::string &str, const std::string &ending) {
    if (ending.length() > str.length()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

bool endsWith(const std::string &str, char ending) {
    return ((str.length() > 0) && (str.back() == ending));
}

void delay(unsigned int howLong) {
    std::this_thread::sleep_for(std::chrono::milliseconds(howLong));
}

static auto startTime = std::chrono::high_resolution_clock::now();

auto getElapsedTimePoint() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    return (currentTime - startTime);
}

unsigned long milliseconds() {
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::milliseconds>(getElapsedTimePoint()).count());
}

unsigned long seconds() {
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::seconds>(getElapsedTimePoint()).count());
}

unsigned long minutes() {
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::minutes>(getElapsedTimePoint()).count());
}

unsigned long hours() {
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::hours>(getElapsedTimePoint()).count());
}

void displayHelp() {
    std::cout << "Usage: Option [=value]" << std::endl;
    std::cout << "Options: " << std::endl;
    for (const auto &it : programOptions) {
        std::cout << "    -" << static_cast<char>(it->shortOption()) << ", --" << it->longOption() << ": " << it->description() << std::endl;
    }
}
