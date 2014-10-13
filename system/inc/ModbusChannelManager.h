/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** ModbusChannelManager.h - 
 **                                                                         **
 ** 12/05/2013 02:41:23 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2011, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __MODBUSCHANNELMANAGER_HEADER__
#define __MODBUSCHANNELMANAGER_HEADER__
#include <vector>
#include <memory>
#include <array>
#include <Poco/Foundation.h>

#include "BufferedAsyncSerial.h"
#include "DataChannel.h"
#include "Interface.h"
#include "Gpio.h"

#define  MODULE_MAX_NUM 32
#define  SPI_BUF_LEN    2048

namespace Zebra {

class ModbusChannelManager

{
public:
    ModbusChannelManager(const std::string& devname, uint32_t baud, const std::string& spi_name);

    ~ModbusChannelManager();

    void init(share_memory_area_t* shm_area);

    void readAll();

    void writeAll();

    Poco::UInt32 spi_transfer();

    Poco::UInt32 spi_checksum();

    bool handshake();

    void reconfig(share_memory_area_t* shm_area);

private:
    void read(Zebra::DataChannel* channel);

    void write(Zebra::DataChannel* channel);

    void send_cmd_conf();

    void debug();

    void check();

public:
    Zebra::Gpio data_ready;
    Zebra::Gpio ack_ready;

private:
    BufferedAsyncSerial serial_port;
    int spi_fd;

    std::vector<std::shared_ptr<Zebra::DataChannel>> vec;
    std::array<uint8_t, MODULE_MAX_NUM> module_conf;
    std::array<uint8_t, SPI_BUF_LEN>    spi_buf;
    std::array<uint8_t, SPI_BUF_LEN>    spi_buf_rx;
    uint32_t  spi_transfer_len;

};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
