#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <getopt.h>
#include "ChaiScriptProgramOption.h"
#include "CppSerialPortChaiScriptModuleFactory.h"

#include "SerialPort.h"
#include <chaiscript/chaiscript.hpp>

static const ProgramOption portOption   {'p', "port",   required_argument, "Specify the serial port to use (ex COM4 or /dev/ttyUSB1)" };
static const ProgramOption scriptOption {'s', "script", required_argument, "Specify the script to execute (ex C:/Path/To/Script.chai (absolute) or Script.chai (relative) path)" };
static const ProgramOption helpOption   {'h', "help",   no_argument,       "Display help text and exit"};

std::array<const ProgramOption *, 3> programOptions{
    &portOption,
    &scriptOption,
    &helpOption
};

static const option longOptions[] {
    portOption.toPosixOption(),
    scriptOption.toPosixOption(),
    helpOption.toPosixOption(),
    {nullptr, 0, nullptr, 0}
};

void displayHelp();
void delay(unsigned int howLong);
bool startsWith(const std::string &str, const std::string &start);
bool startsWith(const std::string &str, char start);
bool endsWith(const std::string &str, const std::string &ending);
bool endsWith(const std::string &str, char end);

template <typename Container> std::string buildShortOptions(const Container &container);
void notifyScriptError(std::exception *e);

#if defined(_WIN32)
    static const char * const SERIAL_PORT_BASE{"COM"};
#else
    static const char * const SERIAL_PORT_BASE{"/dev/"};
#endif //defined(_WIN32)

static const char * const CHAI_SCRIPT_EXTENSION{"chai"};


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

    while ( (currentOption = getopt_long(argc, argv, buildShortOptions(programOptions).c_str(), longOptions, &optionIndex)) != -1) {    
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
        }
	}
    for (int i = 1; i < argc; i++) {
        if (startsWith(argv[i], SERIAL_PORT_BASE)) {
            serialPortName = argv[i];
        } else if (endsWith(argv[i], CHAI_SCRIPT_EXTENSION)) {
            scriptName = argv[i];
        } else if ( (strlen(argv[i]) == 2) && (argv[i][0] == '-') && (argv[i][1] == '-') ) {
           useStdin = true;
        }
    }
    if ( (!useStdin) && (scriptName.length() == 0) ) {
        std::cout << "No script name specified, using stdin" << std::endl;
        useStdin = true;
    }
    std::cout << "Using SerialPort " << serialPortName << std::endl;
    chaiscript::ChaiScript chaiEngine{}; // loads stdlib from loadable module on file system

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
    chaiEngine.add_global(chaiscript::var(serialPort), "serialPort");
    chaiEngine.add(chaiscript::fun(&delay), "delay");
    
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
        std::cout << "Using file \"" << scriptName << "\"" << std::endl;
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
        std::cout << "Evaluation Error: " << e->what() << std::endl;
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

template <typename Container>
std::string buildShortOptions(const Container &programOptions) {
    std::string returnString{""};
    for (const auto &it : programOptions) {
        returnString += static_cast<char>(it->shortOption());
        if (it->argumentSpecifier() == required_argument) {
            returnString += ':';
        }
    }
    return returnString;
}

void displayHelp() {
    std::cout << "Usage: Option [=value]" << std::endl;
    std::cout << "Options: " << std::endl;
    for (const auto &it : programOptions) {
        std::cout << "    -" << static_cast<char>(it->shortOption()) << ", --" << it->longOption() << ": " << it->description() << std::endl;
    }
}
