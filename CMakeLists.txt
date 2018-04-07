cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set (PROJECT_NAME CppSerialPort)
project (${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set (SOURCE_ROOT CppSerialPort)

set (${PROJECT_NAME}_SOURCE_FILES
    ${SOURCE_ROOT}/IPV4Address.cpp
    ${SOURCE_ROOT}/IByteStream.cpp
    ${SOURCE_ROOT}/SerialPort.cpp
    ${SOURCE_ROOT}/TcpClient.cpp
    ${SOURCE_ROOT}/UdpClient.cpp
    ${SOURCE_ROOT}/AbstractSocket.cpp
    ${SOURCE_ROOT}/ErrorInformation.cpp
    ${SOURCE_ROOT}/ByteArray.cpp)

set (${PROJECT_NAME}_HEADER_FILES
    ${SOURCE_ROOT}/IPV4Address.h
    ${SOURCE_ROOT}/IByteStream.h
    ${SOURCE_ROOT}/SerialPort.h
    ${SOURCE_ROOT}/TcpClient.h
    ${SOURCE_ROOT}/UdpClient.h
    ${SOURCE_ROOT}/AbstractSocket.h
        ${SOURCE_ROOT}/ErrorInformation.hpp
    ${SOURCE_ROOT}/ByteArray.h)

add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

add_library(${PROJECT_NAME}_STATIC STATIC
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${SOURCE_ROOT}/)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} shlwapi Ws2_32)
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_HEADER_FILES}")

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/")

endif()

if (WITH_CHAISCRIPT)
add_subdirectory(chaiscript)
endif()
