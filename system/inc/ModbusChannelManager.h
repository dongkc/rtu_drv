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

#include "DataChannel.h"
#include "modbus.h"
#include "Interface.h"

namespace Zebra {

class ModbusChannelManager

{
public:
    ModbusChannelManager(const char *device,
                         int baud,
                         char parity,
                         int data_bit,
                         int stop_bit);

    ~ModbusChannelManager();

    void init(share_memory_area_t* shm_area);

    void readAll();

    void writeAll();

    void setSlaveId(uint8_t address);

    void transferWriteData();

    void transferReadData();

    void debug();

    void reconfig(share_memory_area_t* shm_area);

private:
    std::vector<std::shared_ptr<Zebra::DataChannel>> vec;
    modbus_t* ctx;

    std::array<uint8_t, 32>   model_conf;
    std::array<uint8_t, 2046> spi_buf;
};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
