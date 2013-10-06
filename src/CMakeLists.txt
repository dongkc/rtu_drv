
INCLUDE_DIRECTORIES(../library/dbus/include/)
INCLUDE_DIRECTORIES(../library/modbus/)
INCLUDE_DIRECTORIES(../library/qpcpp/include/)
INCLUDE_DIRECTORIES(../library/qpcpp/ports/posix/gnu)

AUX_SOURCE_DIRECTORY(. RTU_SRC)

ADD_EXECUTABLE(rtu ${RTU_SRC})

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
TARGET_LINK_LIBRARIES(rtu qpport qf qep qs modbus pthread)
