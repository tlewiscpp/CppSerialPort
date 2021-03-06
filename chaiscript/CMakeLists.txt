cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(CppSerialPortChaiScript CXX)

set (THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third-party")

if (WIN32 OR WIN64)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    message(STATUS "Detected Windows compiler: ${CMAKE_CXX_COMPILER_ID}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /bigobj")
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS,5.01")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    elseif()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    endif()

    set (GETOPT_LOCATION ${THIRD_PARTY_ROOT}/win32/)
    set (PLATFORM_GETOPT_LIB getopt-win32)
    set (GETOPT_INCLUDE_DIRECTORY ${GETOPT_LOCATION})
    add_subdirectory(${GETOPT_LOCATION})
else()
    if (NOT CHAISCRIPT_ROOT)
        set (CHAISCRIPT_ROOT /usr/local/include/)
    endif()

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    unset (PLATFORM_GETOPT_LIB)
    set(GETOPT_INCLUDE_DIRECTORY /usr/include/)
endif()

set (MAIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include/")

set (NETWORK_CLIENT ${PROJECT_NAME}NetworkClient)
set (SERIAL_PORT_CLIENT ${PROJECT_NAME}SerialPortClient)

set (NETWORK_CLIENT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")
set (SERIAL_PORT_CLIENT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")

set(${NETWORK_CLIENT}_SOURCE_FILES
        "${NETWORK_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.cpp"
        "${NETWORK_CLIENT_ROOT}/NetworkClientMain.cpp")

set (${NETWORK_CLIENT}_HEADER_FILES
        "${NETWORK_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.hpp"
        "${NETWORK_CLIENT_ROOT}/ChaiScriptProgramOption.hpp"
        "${NETWORK_CLIENT_ROOT}/ChaiScriptModuleFactory.hpp"
        "${NETWORK_CLIENT_ROOT}/Random.hpp")

add_executable(${NETWORK_CLIENT}
        ${${NETWORK_CLIENT}_SOURCE_FILES}
        ${${NETWORK_CLIENT}_HEADER_FILES})

set(${SERIAL_PORT_CLIENT}_SOURCE_FILES
        "${SERIAL_PORT_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.cpp"
        "${SERIAL_PORT_CLIENT_ROOT}/SerialPortMain.cpp")

set (${SERIAL_PORT_CLIENT}_HEADER_FILES
        "${SERIAL_PORT_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.hpp"
        "${SERIAL_PORT_CLIENT_ROOT}/ChaiScriptProgramOption.hpp"
        "${SERIAL_PORT_CLIENT_ROOT}/ChaiScriptModuleFactory.hpp"
        "${SERIAL_PORT_CLIENT_ROOT}/Random.hpp")

add_executable(${SERIAL_PORT_CLIENT}
        ${${SERIAL_PORT_CLIENT}_SOURCE_FILES}
        ${${SERIAL_PORT_CLIENT}_HEADER_FILES})

target_link_libraries(${NETWORK_CLIENT}
        CppSerialPort
        dl
        pthread)

target_link_libraries(${SERIAL_PORT_CLIENT}
        CppSerialPort
        dl
        pthread)

target_include_directories(${NETWORK_CLIENT}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${MAIN_LIB_DIR}")

target_include_directories(${SERIAL_PORT_CLIENT}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${MAIN_LIB_DIR}")

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)
    install(TARGETS ${NETWORK_CLIENT} RUNTIME DESTINATION bin)
    install(TARGETS ${SERIAL_PORT_CLIENT} RUNTIME DESTINATION bin)
endif()
