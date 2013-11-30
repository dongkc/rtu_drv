
#INCLUDE_DIRECTORIES(../library/dbus/include/)
INCLUDE_DIRECTORIES(../library/modbus/)
#INCLUDE_DIRECTORIES(../library/qpcpp/include/)
#INCLUDE_DIRECTORIES(../library/qpcpp/ports/posix/gnu)
#INCLUDE_DIRECTORIES(../web/)
INCLUDE_DIRECTORIES(include)

AUX_SOURCE_DIRECTORY(src DEMO_SRC)

ADD_EXECUTABLE(rtu_plc ${DEMO_SRC})

TARGET_LINK_LIBRARIES(rtu_plc  modbus pthread rt
    ${CMAKE_SOURCE_DIR}/kw/lib/CONSEN_IO-cli.o
    ${CMAKE_SOURCE_DIR}/kw/lib/CONSEN_SHM-cli.o
    ${CMAKE_SOURCE_DIR}/kw/lib/CONSEN_SHM.o
    ${CMAKE_SOURCE_DIR}/kw/lib/GT_LIB10-cli.o
    ${CMAKE_SOURCE_DIR}/kw/lib/GT_LIB10.o
    ${CMAKE_SOURCE_DIR}/kw/lib/libeclrlib.a
    )
#TARGET_LINK_LIBRARIES(rtu_plc  ${CMAKE_SOURCE_DIR}/kw/lib/libeclrlib.a)
