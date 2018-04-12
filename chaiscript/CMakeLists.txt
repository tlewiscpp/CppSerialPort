cmake_minimum_required(VERSION 3.1)
project(CppSerialPortChaiScript CXX)

set (THIRD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third-party")

if (WIN32 OR WIN64)
    message(STATUS "Detected Windows compiler: ${CMAKE_CXX_COMPILER_ID}")
    set (WIN_COMPILER "${CMAKE_CXX_COMPILER_ID}")
    if (NOT WIN_COMPILER)
        message(FATAL_ERROR "Please specify -DWIN_COMPILER=MSVC or -DWIN_COMPILER=GNU")
    endif()
    if (WIN_COMPILER STREQUAL "MSVC")
        set (WINDOWS_COMPILER MSVC)
        set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /bigobj")
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS,5.01")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    elseif()
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wa,-mbig-obj")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
        message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    endif()

    set (GETOPT_LOCATION ${THIRD_PARTY_ROOT}/win32/)
    set (PLATFORM_GETOPT_LIB getopt-win32)
    set (GETOPT_INCLUDE_DIRECTORY ${GETOPT_LOCATION})
    add_subdirectory(${GETOPT_LOCATION})
else()
    if (NOT CHAISCRIPT_ROOT)
        set (CHAISCRIPT_ROOT /usr/local/include/)
    endif()

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fPIC")
    message (STATUS "Current CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    unset (PLATFORM_GETOPT_LIB)
    set(GETOPT_INCLUDE_DIRECTORY /usr/include/)
endif()

set (MAIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../CppSerialPort")

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
        "${NETWORK_CLIENT_ROOT}/ChaiScriptModuleFactory.hpp")

add_executable(${NETWORK_CLIENT}
        ${${NETWORK_CLIENT}_SOURCE_FILES}
        ${${NETWORK_CLIENT}_HEADER_FILES})

set(${SERIAL_PORT_CLIENT}_SOURCE_FILES
        "${SERIAL_PORT_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.cpp"
        "${SERIAL_PORT_CLIENT_ROOT}/SerialPortMain.cpp")

set (${SERIAL_PORT_CLIENT}_HEADER_FILES
        "${SERIAL_PORT_CLIENT_ROOT}/CppSerialPortChaiScriptModuleFactory.hpp"
        "${SERIAL_PORT_CLIENT_ROOT}/ChaiScriptProgramOption.hpp"
        "${SERIAL_PORT_CLIENT_ROOT}/ChaiScriptModuleFactory.hpp")

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
