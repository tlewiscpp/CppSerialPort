#ifndef ERROR_INFORMATION_HPP
#define ERROR_INFORMATION_HPP

#include <string>

namespace ErrorInformation {
    int getLastError();
    std::string getErrorString(int errorCode);
} //namespace ErrorInformation

namespace NetworkErrorInformation {
    int getLastError();
    std::string getErrorString(int errorCode);
} //namespace NetworkErrorInformation

#endif //ERROR_INFORMATION_HPP
