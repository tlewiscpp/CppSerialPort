cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
project (CppSerialPort CXX)

if (WIN32 OR WIN64)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32 OR WIN64)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "-DNOMINMAX /EHsc /bigobj")
        set(COVERAGE_LINK_FLAGS  "/SUBSYSTEM:WINDOWS,5.01")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    else()
        #set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wa,-mbig-obj")
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
        set(COVERAGE_LINK_FLAGS  "-mwindows")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
    endif()
else()
    #set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fPIC")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(COVERAGE_LINK_FLAGS  "")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
endif()

set (SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/CppSerialPort/")

set (${PROJECT_NAME}_SOURCE_FILES
    "${SOURCE_ROOT}/IPV4Address.cpp"
    "${SOURCE_ROOT}/IByteStream.cpp"
    "${SOURCE_ROOT}/SerialPort.cpp"
    "${SOURCE_ROOT}/TcpClient.cpp"
    "${SOURCE_ROOT}/UdpClient.cpp"
    "${SOURCE_ROOT}/AbstractSocket.cpp"
    "${SOURCE_ROOT}/ErrorInformation.cpp"
    "${SOURCE_ROOT}/BasicFile.cpp"
    "${SOURCE_ROOT}/ByteArray.cpp")

set (${PROJECT_NAME}_HEADER_FILES
    "${SOURCE_ROOT}/IPV4Address.hpp"
    "${SOURCE_ROOT}/IByteStream.hpp"
    "${SOURCE_ROOT}/SerialPort.hpp"
    "${SOURCE_ROOT}/TcpClient.hpp"
    "${SOURCE_ROOT}/UdpClient.hpp"
    "${SOURCE_ROOT}/AbstractSocket.hpp"
    "${SOURCE_ROOT}/ErrorInformation.hpp"
    "${SOURCE_ROOT}/BasicFile.hpp"
    "${SOURCE_ROOT}/ByteArray.hpp")

add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

add_library(${PROJECT_NAME}_STATIC STATIC
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${SOURCE_ROOT}/")

if (WIN32)
    target_link_libraries(${PROJECT_NAME} shlwapi Ws2_32)
endif()


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
