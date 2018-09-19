#ifndef ERROR_INFORMATION_H
#define ERROR_INFORMATION_H

#include <string>

namespace ErrorInformation {
    int getLastError();
    std::string getErrorString(int errorCode);
} //namespace ErrorInformation

namespace NetworkErrorInformation {
    int getLastError();
    std::string getErrorString(int errorCode);
} //namespace NetworkErrorInformation

#endif //ERROR_INFORMATION_H
