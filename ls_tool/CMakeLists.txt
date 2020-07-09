cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(lscomm CXX)

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
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

set (MAIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
set (LSCOMM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(${PROJECT_NAME}_SOURCE_FILES
        "${LSCOMM_ROOT}/lscomm.cpp")

set (${PROJECT_NAME}_HEADER_FILES)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCE_FILES}
    ${${PROJECT_NAME}_HEADER_FILES})


target_link_libraries(${PROJECT_NAME}
        CppSerialPort)

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
        PUBLIC "${MAIN_LIB_DIR}")

if(${CMAKE_SYSTEM_NAME} MATCHES Linux|.*BSD|DragonFly)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif()
