#include "ErrorInformation.hpp"

#include <cstring>
#include <iostream>

#if defined(_WIN32)

#else
#   include <cerrno>
#endif //defined(_WIN32)

#define STRERROR_BUFFER_MAX 1024

#if (_POSIX_C_SOURCE >= 200112L) && !_GNU_SOURCE
#    define INVALID_STRERROR_RESULT -1
#else
#    define INVALID_STRERROR_RESULT nullptr
#endif //(_POSIX_C_SOURCE >= 200112L) && !  _GNU_SOURCE

namespace ErrorInformation {

int getLastError() {
#if defined(_WIN32)
	return static_cast<int>(GetLastError());
#else
    return errno;
#endif //defined(_WIN32)
}

std::string getErrorString(int errorCode) {
    char errorString[STRERROR_BUFFER_MAX];
    memset(errorString, '\0', STRERROR_BUFFER_MAX);
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
    (void)wcstombs(errorString, wideErrorString, STRERROR_BUFFER_MAX);
	LocalFree(wideErrorString);
#else
    return std::string{strerror(errorCode)};
	/*
    auto strerrorCode = strerror_r(errorCode, errorString, STRERROR_BUFFER_MAX);
    if (strerrorCode == INVALID_STRERROR_RESULT) {
        std::cerr << "strerror_r(int, char *, int): error occurred" << std::endl;
        return "";
    }
    */
#endif //defined(_WIN32)
    return errorString;
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
    char errorString[STRERROR_BUFFER_MAX];
    memset(errorString, '\0', STRERROR_BUFFER_MAX);
#if defined(_WIN32)
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

    (void)wcstombs(errorString, wideErrorString, STRERROR_BUFFER_MAX);
    LocalFree(wideErrorString);
#else
    return std::string{strerror(errorCode)};
    /*
    auto strerrorCode = strerror_r(errorCode, errorString, STRERROR_BUFFER_MAX);
    if (strerrorCode == INVALID_STRERROR_RESULT) {
        std::cerr << "strerror_r(int, char *, int): error occurred" << std::endl;
        return "";
    }
    */
#endif //defined(_WIN32)
    return errorString;
}

} //namespace NetworkErrorInformation
