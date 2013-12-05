cmake_minimum_required(VERSION 2.8)

#SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc)
#SET(CMAKE_CXX_COMPILER arm-none-linux-gnueabi-g++)
#
#SET(CMAKE_FIND_ROOT_PATH "/home/dongkc/repo/buildroot-at91-master/output/staging")
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#
#INCLUDE_DIRECTORIES(${CMAKE_FIND_ROOT_PATH}/usr/include)
#
PROJECT(LIBMODBUS)

SET(CMAKE_CXX_FLAGS "-Wall -O2 -fmessage-length=0 -fno-strict-aliasing --std=c++11")
SET(CMAKE_C_FLAGS "-O2 -Wall -fmessage-length=0 -fno-strict-aliasing")
#SET(CMAKE_C_FLAGS "-static -msoft-float -D__GCC_FLOAT_NOT_NEEDED")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

ADD_SUBDIRECTORY(./library/modbus/)
ADD_SUBDIRECTORY(./library/qpcpp/qep/source/)
ADD_SUBDIRECTORY(./library/qpcpp/qf/source/)
ADD_SUBDIRECTORY(./library/qpcpp/qs/source/)
ADD_SUBDIRECTORY(./library/qpcpp/ports/posix/gnu/)
#ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(tools)
#ADD_SUBDIRECTORY(dbus)
#ADD_SUBDIRECTORY(web)
#ADD_SUBDIRECTORY(kw)
ADD_SUBDIRECTORY(system)
#ADD_SUBDIRECTORY(./library/qpcpp/qk/source/)
