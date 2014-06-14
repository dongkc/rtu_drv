#AUX_SOURCE_DIRECTORY(. SERIAL_SRC)

#ADD_LIBRARY(serial STATIC ${SERIAL_SRC})
ADD_LIBRARY(serial STATIC AsyncSerial.cpp BufferedAsyncSerial.cpp)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)

#target_link_libraries(async boost_thread)

TARGET_LINK_LIBRARIES(serial boost_thread)
