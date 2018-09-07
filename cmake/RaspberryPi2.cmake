set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(RPI_ROOT $ENV{HOME}/CrossCompile/raspi2)

set(CMAKE_SYSROOT ${RPI_ROOT}/sysroot/)
set(CROSS_COMPILING 1)
set(EMBEDDED_BUILD 1)
#set(CMAKE_STAGING_PREFIX /home/devel/stage)

set(TOOL_ROOT ${RPI_ROOT}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/)
set(CMAKE_C_COMPILER ${TOOL_ROOT}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${TOOL_ROOT}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_INSTALL_PREFIX ${RPI_ROOT}/sysroot/usr)
