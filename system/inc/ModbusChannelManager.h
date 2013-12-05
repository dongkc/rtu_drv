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
    void init(share_memory_area_t& shm_area);
    void readAll();
    void writeAll();
    void setSlaveId(uint8_t address);
    void transfer();

private:
    std::vector<Zebra::DataChannel*> vec;
    modbus_t* ctx;
};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
