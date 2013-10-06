cmake_minimum_required(VERSION 2.8)

#SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER arm-none-linux-gnueabi-gcc)
#SET(CMAKE_CXX_COMPILER arm-none-linux-gnueabi-g++)
#
#SET(CMAKE_FIND_ROOT_PATH "/home/dongkc/repo/buildroot-at91-master/output/staging")
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

PROJECT(LIBMODBUS)

SET(CMAKE_CXX_FLAGS "--std=c++11")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(./library/modbus/)
ADD_SUBDIRECTORY(./library/qpcpp/qep/source/)
ADD_SUBDIRECTORY(./library/qpcpp/qf/source/)
ADD_SUBDIRECTORY(./library/qpcpp/qs/source/)
ADD_SUBDIRECTORY(./library/qpcpp/ports/posix/gnu/)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tools)
#ADD_SUBDIRECTORY(./library/qpcpp/qk/source/)
