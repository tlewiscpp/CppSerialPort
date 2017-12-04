cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set (PROJECT_NAME CppSerialPortTest)
project (${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set (SOURCE_ROOT CppSerialPortTest)
set (CPP_SERIAL_PORT_ROOT ../../CppSerialPort/)

set (${PROJECT_NAME}_SOURCE_FILES
        ${SOURCE_ROOT}/CppSerialPortTest.cpp)

set (${PROJECT_NAME}_HEADER_FILES)

add_executable(${PROJECT_NAME} 
        ${${PROJECT_NAME}_SOURCE_FILES}
        ${${PROJECT_NAME}_HEADER_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${SOURCE_ROOT}/
        PUBLIC ${CPP_SERIAL_PORT_ROOT}/)

		target_link_libraries(${PROJECT_NAME} CppSerialPort)

