#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

#include <getopt.h>
#include "ChaiScriptProgramOption.hpp"
#include "CppSerialPortChaiScriptModuleFactory.hpp"

#include "UdpClient.h"
#include "TcpClient.h"
#include <chaiscript/chaiscript.hpp>

static const ProgramOption portOption        {'p', "port",   required_argument, "Specify the network port to use (ex 5678)" };
static const ProgramOption hostNameOption    {'n', "hostname",   required_argument, "Specify the hostname to use (ex 192.168.1.50)" };
static const ProgramOption scriptOption      {'s', "script", required_argument, "Specify the script to execute (ex C:/Path/To/Script.chai (absolute) or Script.chai (relative) path)" };
static const ProgramOption udpOption         {'u', "udp", no_argument, "Use UDP instead of default (TCP)"};
static const ProgramOption helpOption        {'h', "help",   no_argument,       "Display help text and exit"};

static const option longOptions[] {
        portOption.toPosixOption(),
        hostNameOption.toPosixOption(),
        scriptOption.toPosixOption(),
        udpOption.toPosixOption(),
        helpOption.toPosixOption(),
        {nullptr, 0, nullptr, 0}
};

template <typename T, size_t N> inline constexpr size_t arraySize( T (&t)[N] ) { (void)t; return N; };

static const size_t PROGRAM_OPTION_COUNT{arraySize(longOptions)-1};

std::array<const ProgramOption *, PROGRAM_OPTION_COUNT> programOptions{
    &portOption,
    &hostNameOption,
    &scriptOption,
    &udpOption,
    &helpOption
};

bool looksLikeIP(const char *str);
void displayHelp();
void delay(unsigned int howLong);
bool startsWith(const std::string &str, const std::string &start);
bool startsWith(const std::string &str, char start);
bool endsWith(const std::string &str, const std::string &ending);
bool endsWith(const std::string &str, char end);
void notifyScriptError(std::exception *e);

static const char * const CHAI_SCRIPT_EXTENSION{"chai"};
static const uint16_t MINIMUM_PORT_NUMBER{CppSerialPort::AbstractSocket::MINIMUM_PORT_NUMBER};
static const uint16_t MAXIMUM_PORT_NUMBER{CppSerialPort::AbstractSocket::MAXIMUM_PORT_NUMBER};

static const char * const DEFAULT_HOST_NAME{"127.0.0.1"};

std::string tryParseHostName(std::string hostName);
int32_t tryParsePortNumber(std::string portNumber);
std::string tryParseScriptName(std::string scriptName);

int main(int argc, char *argv[])
{
    if (argc == 1) {
        displayHelp();
        exit(EXIT_FAILURE);
    }
    std::string hostName{};
    int32_t portNumber{-1};
    std::string scriptName{""};
    bool useUdp{false};
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
                portNumber = tryParsePortNumber(optarg);
                break;
            case 's':
                scriptName = tryParseScriptName(optarg);
                break;
            case 'u':
                useUdp = true;
                 break;
            case 'n':
                hostName = tryParseHostName(optarg);
                break;
            default:
                break;
        }
	}

    if (portNumber != -1) {
        if (portNumber < MINIMUM_PORT_NUMBER) {
            std::cerr << "Port number may not be less than " << MINIMUM_PORT_NUMBER << " (" << portNumber << " < " << MINIMUM_PORT_NUMBER << ")" << std::endl;
            exit(EXIT_FAILURE);
        } else if (portNumber > MAXIMUM_PORT_NUMBER) {
            std::cerr << "Port number may not be greater than " << MAXIMUM_PORT_NUMBER << " (" << portNumber << " > " << MAXIMUM_PORT_NUMBER << ")" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if (portNumber == -1) {
        for (int i = 1; i < argc; i++) {
            if ( (strlen(argv[i]) > 0) && (argv[i][0] != '-') && !(looksLikeIP(argv[i]))) {
                portNumber = std::stoi(argv[i]);
            }
        }
        if (portNumber == -1) {
            std::cerr << "Please specify a port number to connect to" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (hostName.empty()) {
        for (int i = 1; i < argc; i++) {
            if ( (strlen(argv[i]) > 0) && (argv[i][0] != '-') && (looksLikeIP(argv[i]))) {
                hostName = argv[i];
            }
        }
        if (hostName.empty()) {
            hostName = DEFAULT_HOST_NAME;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (endsWith(argv[i], CHAI_SCRIPT_EXTENSION) && (scriptName.empty()) ) {
            scriptName = argv[i];
        } else if ( (strlen(argv[i]) == 2) && (argv[i][0] == '-') && (argv[i][1] == '-') ) {
           useStdin = true;
        }
    }

    if ( (!useStdin) && (scriptName.empty()) ) {
        std::cout << "No script name specified, using stdin" << std::endl;
        useStdin = true;
    } else {
        std::cout << "Using ScriptName=\"" << scriptName << "\"" << std::endl;
    }
    std::cout << "Using HostName=\"" << hostName  << "\"" << std::endl;
    std::cout << "Using PortNumber=\"" << portNumber << "\"" << std::endl;
    chaiscript::ChaiScript chaiEngine{};

    chaiEngine.add(CppSerialPort::CppSerialPortChaiScriptModuleFactory{}.createModule());

    std::shared_ptr<CppSerialPort::AbstractSocket> networkClient{nullptr};
    if (useUdp) {
        networkClient = std::make_shared<CppSerialPort::UdpClient>(hostName, portNumber);
    } else {
        networkClient = std::make_shared<CppSerialPort::TcpClient>(hostName, portNumber);
    }

    chaiEngine.add_global(chaiscript::var(networkClient), "networkClient");
    chaiEngine.add(chaiscript::fun(&delay), "delay");
    networkClient->openPort();   
    
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


int32_t tryParsePortNumber(std::string portNumber) {
    if (startsWith(portNumber, '=')) {
        portNumber.erase(portNumber.begin());
    }
    try {
        int32_t returnValue = std::stoi(portNumber);
        return returnValue;
    } catch (std::exception &e) {
        (void)e;
        return -1;
    }
}

std::string tryParseScriptName(std::string scriptName) {
    if (startsWith(scriptName, '=')) {
        scriptName.erase(scriptName.begin());
    }
    return scriptName;
}

std::string tryParseHostName(std::string hostName) {
    if (startsWith(hostName, '=')) {
        hostName.erase(hostName.begin());
    }
    return hostName;
}

void notifyScriptError(std::exception *e) {
    chaiscript::exception::eval_error *evalError{nullptr};
    if ( (evalError = dynamic_cast<chaiscript::exception::eval_error *>(e)) != nullptr) {
		std::cout << "Evaluation Error: " << evalError->pretty_print() << std::endl;
    } else {
        std::cout << "Evaluation Error: " << e->what() << std::endl;
    }
}

bool looksLikeIP(const char *str){
    static const std::regex ipv4Regex{"((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])"};
    static const std::regex ipv6Regex{"(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))"};
    return (std::regex_match(str, ipv4Regex) || std::regex_match(str, ipv6Regex));
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

void displayHelp() {
    std::cout << "Usage: Option [=value]" << std::endl;
    std::cout << "Options: " << std::endl;
    for (const auto &it : programOptions) {
        std::cout << "    -" << static_cast<char>(it->shortOption()) << ", --" << it->longOption() << ": " << it->description() << std::endl;
    }
}
