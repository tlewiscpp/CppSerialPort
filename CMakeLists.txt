cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project (CppSerialPort CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32 OR WIN64)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    message(STATUS "CppSerialPort: Detected CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /bigobj")
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS,5.01")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    endif()
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(COVERAGE_LINK_FLAGS  "")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
endif()

set (SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set (INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set (HEADER_ROOT "${INCLUDE_ROOT}/CppSerialPort/")

set (${PROJECT_NAME}_SOURCE_FILES
    "${SOURCE_ROOT}/IPV4Address.cpp"
    "${SOURCE_ROOT}/IByteStream.cpp"
    "${SOURCE_ROOT}/SerialPort.cpp"
    "${SOURCE_ROOT}/TcpSocket.cpp"
    "${SOURCE_ROOT}/UdpSocket.cpp"
    "${SOURCE_ROOT}/AbstractSocket.cpp"
    "${SOURCE_ROOT}/ErrorInformation.cpp"
    "${SOURCE_ROOT}/ByteArray.cpp")

set (${PROJECT_NAME}_HEADER_FILES
    "${HEADER_ROOT}/IPV4Address.hpp"
    "${HEADER_ROOT}/IByteStream.hpp"
    "${HEADER_ROOT}/SerialPort.hpp"
    "${HEADER_ROOT}/TcpSocket.hpp"
    "${HEADER_ROOT}/UdpSocket.hpp"
    "${HEADER_ROOT}/AbstractSocket.hpp"
    "${HEADER_ROOT}/ErrorInformation.hpp"
    "${HEADER_ROOT}/ByteArray.hpp")

add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

add_library(${PROJECT_NAME}_STATIC STATIC
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${INCLUDE_ROOT}"
        PUBLIC "${SOURCE_ROOT}/")

target_include_directories(${PROJECT_NAME}_STATIC
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${INCLUDE_ROOT}"
        PUBLIC "${SOURCE_ROOT}/")

if (WIN32 OR WIN64)
    target_link_libraries(${PROJECT_NAME} shlwapi Ws2_32)
endif()

option (WITH_CHAISCRIPT "Building with chaiscript support" OFF)
option (BUILD_LS_TOOL "Build lscomm tool" ON)

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_HEADER_FILES}")

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/")

install(TARGETS ${PROJECT_NAME}_STATIC
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/")
endif()


if (WITH_CHAISCRIPT)
    add_subdirectory(chaiscript)
endif()

if (BUILD_LS_TOOL)
    add_subdirectory(ls_tool)
endif()

