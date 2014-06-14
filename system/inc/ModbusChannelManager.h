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

#include "BufferedAsyncSerial.h"
#include "DataChannel.h"
#include "Interface.h"

#define  MODULE_MAX_NUM 32
#define  SPI_BUF_LEN    2048

namespace Zebra {

class ModbusChannelManager

{
public:
    ModbusChannelManager(const std::string& devname, uint32_t baud, const std::string& spi_name);

    ~ModbusChannelManager();

    void init(share_memory_area_t* shm_area);

    void walk();

    bool handshake();

    void reconfig(share_memory_area_t* shm_area);

private:
    void send_cmd_conf();

private:
    BufferedAsyncSerial serial_port;
    int spi_fd;

    std::vector<std::shared_ptr<Zebra::DataChannel>> vec;
    std::array<uint8_t, MODULE_MAX_NUM> module_conf;
    std::array<uint8_t, SPI_BUF_LEN>    spi_buf;
};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
