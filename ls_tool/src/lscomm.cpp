#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <CppSerialPort/SerialPort.hpp>

std::string nativeOptionsPrefix{"/dev/ttyS"};
std::vector<std::string> optionPrefixes{"-", "--"};
std::vector<std::string> skipNativeOptions{"s, skip-native"};

bool skipNativePorts{false};
bool unknownOption{false};

int findLastNonNumeric(const std::string &str);
int getNumericPortNumber(const std::string &str);

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string temp{argv[i]};
        std::string copy{temp};
        
        //To lower case
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        
        //Strip off option prefixes
        for (const auto &it : optionPrefixes) {
            if (temp.rfind(it, 0) == 0) {
                temp = temp.substr(it.length());
                break;
            }
        }

        unknownOption = true;
        for (const auto &it : skipNativeOptions) {
            if (temp == it) {
                skipNativePorts = true;
                unknownOption = false;
                break;
            }
        }
        if (unknownOption) {
            std::cout << "Unknown option \"" << copy << "\"" << std::endl;
        }
    }
       
    std::vector<std::string> serialPortNames{};
    std::set<std::string> prefixes{};
    std::set<std::string> unknownNames{};
    for (const auto &it : CppSerialPort::SerialPort::availableSerialPorts()) {
        auto index = findLastNonNumeric(it);
        if (index == -1) {
            unknownNames.emplace(it);
            continue;
        }
        std::string prefix{it.substr(0, index)};
        prefixes.insert(prefix);
        serialPortNames.emplace_back(it);
    }
    for (const auto &it : prefixes) {
        if ((skipNativePorts) && (it == nativeOptionsPrefix)) {
            continue;
        }
        std::map<int, std::string> values{};
        std::vector<std::string> tempList{};
        std::copy_if(serialPortNames.begin(), serialPortNames.end(), std::back_inserter(tempList), [it](const std::string &str) { return str.rfind(it, 0) == 0;} );
        for (const auto &innerIt : tempList) {
            auto value = getNumericPortNumber(innerIt);
            if (value != -1) {
                values.emplace(value, innerIt);
            }
        }
        for (const auto &innerIt: values) {
            std::cout << innerIt.second << std::endl;
        }
    }
    for (const auto &it : unknownNames) {
        std::cout << it << std::endl;
    }
    return 0;
}

int findLastNonNumeric(const std::string &str) {
    int index{-1};
    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            index = i;
            break;
        }
    }
    return index;
}

int getNumericPortNumber(const std::string &str) {
    int numeric{-1};
    auto index = findLastNonNumeric(str);
    if (index == -1) {
        return numeric;
    }
    std::string temp{str.substr(index)};
    try {
        numeric = std::stoi(temp);
        return numeric;
    } catch (const std::exception &e) {
        return numeric;
    }
}
