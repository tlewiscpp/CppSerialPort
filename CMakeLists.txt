cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set (PROJECT_NAME CppSerialPort)
project (${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT STATIC_BUILD)
    set (SHARED_LIB_BUILD)
else()
    unset(SHARED_LIB_BUILD)
endif()

if (SHARED_LIB_BUILD)
    set(LIBRARY_TYPE SHARED)
else()
    set (LIBRARY_TYPE STATIC)
endif()

set (SOURCE_ROOT CppSerialPort)

set (${PROJECT_NAME}_SOURCE_FILES
        ${SOURCE_ROOT}/IPV4Address.cpp
        ${SOURCE_ROOT}/IByteStream.cpp
        ${SOURCE_ROOT}/SerialPort.cpp
        ${SOURCE_ROOT}/TcpClient.cpp
        ${SOURCE_ROOT}/ByteArray.cpp)

set (${PROJECT_NAME}_HEADER_FILES
        ${SOURCE_ROOT}/IPV4Address.h
        ${SOURCE_ROOT}/IByteStream.h
        ${SOURCE_ROOT}/SerialPort.h
        ${SOURCE_ROOT}/TcpClient.h
        ${SOURCE_ROOT}/ByteArray.h)

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} 
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${SOURCE_ROOT}/)

if (WIN32)
target_link_libraries(${PROJECT_NAME} shlwapi Ws2_32)
endif()

add_subdirectory(Test/)


if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${${PROJECT_NAME}_HEADER_FILES}")

if (SHARED_LIB_BUILD)
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
            PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/")
else()
    install(TARGETS ${PROJECT_NAME}
            ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/"
            PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/")
endif()


endif()


