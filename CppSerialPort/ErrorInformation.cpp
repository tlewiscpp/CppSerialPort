#include "ErrorInformation.hpp"

#include <cstring>
#include <iostream>

#if defined(_WIN32)
#   include <Windows.h>
#else
#   include <cerrno>
#endif //defined(_WIN32)

#define STRERROR_BUFFER_MAX 1024

#if (_POSIX_C_SOURCE >= 200112L) && !_GNU_SOURCE
#    define INVALID_STRERROR_RESULT -1
#else
#    define INVALID_STRERROR_RESULT nullptr
#endif //(_POSIX_C_SOURCE >= 200112L) && !  _GNU_SOURCE

namespace {
    void stripLineEndings(std::string &str) {
        if (str.length() >= 2) {
            str.erase(str.back());
            str.erase(str.back());
        } else if (str.length() > 0) {
            str.erase(str.back());
        }
    }
}

namespace ErrorInformation {

int getLastError() {
#if defined(_WIN32)
	return static_cast<int>(GetLastError());
#else
    return errno;
#endif //defined(_WIN32)
}

std::string getErrorString(int errorCode) {
#if defined(_WIN32)
    char errorStringBuffer[STRERROR_BUFFER_MAX];
    memset(errorStringBuffer, '\0', STRERROR_BUFFER_MAX);
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
    (void)wcstombs(errorStringBuffer, wideErrorString, STRERROR_BUFFER_MAX);
	LocalFree(wideErrorString);
    std::string errorString{errorStringBuffer};
    stripLineEndings(errorString);
    return errorString;
#else
    std::string errorString{strerror(errorCode)};
    stripLineEndings(errorString);
    return errorString;
#endif //defined(_WIN32)
}

} //namespace ErrorInformation


namespace NetworkErrorInformation {

int getLastError() {
#if defined(_WIN32)
    return WSAGetLastError();
#else
    return errno;
#endif //defined(_WIN32)
}

std::string getErrorString(int errorCode) {
#if defined(_WIN32)
    char errorStringBuffer[STRERROR_BUFFER_MAX];
    memset(errorStringBuffer, '\0', STRERROR_BUFFER_MAX);
    wchar_t *wideErrorString{ nullptr };
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&wideErrorString),
        0,
        nullptr
    );

    (void)wcstombs(errorStringBuffer, wideErrorString, STRERROR_BUFFER_MAX);
    LocalFree(wideErrorString);
    std::string errorString{errorStringBuffer};
    stripLineEndings(errorString);
    return errorString;
#else
    std::string errorString{strerror(errorCode)};
    stripLineEndings(errorString);
    return errorString;
#endif //defined(_WIN32)
}

} //namespace NetworkErrorInformation
