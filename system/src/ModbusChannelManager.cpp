/*
 ******************************* C SOURCE FILE *******************************
 **                                                                         **
 ** ModbusChannelManager.cpp - 
 **                                                                         **
 ** 12/05/2013 02:58:45 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2010, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#define __MODBUSCHANNELMANAGER_SRC__
#include <stdio.h>
#include <unistd.h>

#include "ModbusChannelManager.h"
#include "Interface.h"
#include "DataChannel.h"

using namespace std;
using namespace Zebra;


namespace {

int response_io_status(int address, int nb,
                              uint8_t *tab_io_status,
                              uint8_t *rsp, int offset)
{
    int shift = 0;
    int byte = 0;
    int i;

    for (i = address; i < address+nb; i++) {
        byte |= tab_io_status[i] << shift;
        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = byte;
            byte = shift = 0;
        } else {
            shift++;
        }
    }

    if (shift != 0)
        rsp[offset++] = byte;

    return offset;
}

}

namespace Zebra {
ModbusChannelManager::ModbusChannelManager(const char *device,
                         int baud,
                         char parity,
                         int data_bit,
                         int stop_bit)
{
    ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);
    //modbus_set_debug(ctx, TRUE);
    modbus_connect(ctx);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
}

void ModbusChannelManager::init(share_memory_area_t* shm_area)
{
    uint32_t  di_counter = 0;
    uint32_t  ai_counter = 0;
    uint32_t  pi_counter = 0;
    uint32_t  do_counter = 0;
    uint32_t  ao_counter = 0;

    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        if (shm_area->user.io_config[i - 1].channel_type == MODULE_TYPE_DONE) {
            //TODO
            break;
        }

        if (shm_area->user.io_config[i - 1].channel_type == MODULE_NOT_PRESENT) {
            continue;
        } else {
            DataChannel *data_channel = nullptr;
            switch(shm_area->user.io_config[i - 1].channel_type) {
                case MODULE_DI_16:
                    data_channel = new DataChannel(i,
                                                   16,
                                                   0,
                                                   0,
                                                   0,
                                                   &shm_area->user.input_di[di_counter],
                                                   nullptr,
                                                   nullptr,
                                                   nullptr);
                    vec.push_back(data_channel);

                    di_counter += 2;
                    break;

                case MODULE_DO_16:
                    data_channel = new DataChannel(i,
                                                   0,
                                                   16,
                                                   0,
                                                   0,
                                                   nullptr,
                                                   &shm_area->user.output_do[do_counter],
                                                   nullptr,
                                                   nullptr);
                    vec.push_back(data_channel);

                    do_counter += 2;
                    break;

                case MODULE_AI_16:
                    data_channel = new DataChannel(i,
                                                   0,
                                                   0,
                                                   16,
                                                   0,
                                                   nullptr,
                                                   nullptr,
                                                   &shm_area->user.input_ai[ai_counter],
                                                   nullptr);
                    vec.push_back(data_channel);

                    ai_counter += 16;
                    break;

                case MODULE_AO_08:
                    data_channel = new DataChannel(i,
                                                   0,
                                                   0,
                                                   0,
                                                   8,
                                                   nullptr,
                                                   nullptr,
                                                   nullptr,
                                                   &shm_area->user.output_ao[ao_counter]);
                    vec.push_back(data_channel);

                    ao_counter += 8;
                    break;

                case MODULE_DI_32:
                    data_channel = new DataChannel(i,
                                                   32,
                                                   0,
                                                   0,
                                                   0,
                                                   &shm_area->user.input_di[di_counter],
                                                   nullptr,
                                                   nullptr,
                                                   nullptr);
                    vec.push_back(data_channel);

                    di_counter += 4;
                    break;

                case MODULE_DO_32:
                    data_channel = new DataChannel(i,
                                                   0,
                                                   32,
                                                   0,
                                                   0,
                                                   nullptr,
                                                   &shm_area->user.output_do[do_counter],
                                                   nullptr,
                                                   nullptr);
                    vec.push_back(data_channel);

                    do_counter += 4;
                    break;

                case MODULE_AI_32:
                    data_channel = new DataChannel(i,
                                                   0,
                                                   0,
                                                   32,
                                                   0,
                                                   nullptr,
                                                   nullptr,
                                                   &shm_area->user.input_ai[ai_counter],
                                                   nullptr);
                    vec.push_back(data_channel);

                    ai_counter += 4;
                    break;

                case MODULE_DAIO_32:
                    data_channel = new DataChannel(i,
                                                   8,
                                                   8,
                                                   8,
                                                   0,
                                                   &shm_area->user.input_di[di_counter],
                                                   &shm_area->user.output_do[do_counter],
                                                   &shm_area->user.input_ai[ai_counter],
                                                   nullptr);
                    vec.push_back(data_channel);

                    di_counter += 1;
                    do_counter += 1;
                    ai_counter += 8;
                    break;

                default:
                    break;
            }
        }
    }

    // io_status init
    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        shm_area->user.system_area.modules_health_flag[i -1] = MODULE_NOT_FOUND;
    }

    for (auto channel : vec) {
        shm_area->user.system_area.modules_health_flag[channel->_slaveID - 1] = MODULE_OK;
    }
}

void ModbusChannelManager::readAll()
{
    for (auto channel : vec) {
        int rc = 0;

        setSlaveId(channel->_slaveID);

        if (channel->_di_len) {
            rc = modbus_read_input_bits(ctx, 0, channel->_di_len, channel->_di.data());
        }

        if (channel->_ai_len) {
            rc = modbus_read_input_registers(ctx, 0, channel->_ai_len, channel->_ai.data());
        }

        if (rc == -1) {
            channel->_status = 0;
        } else {
            channel->_status = 1;
        }
            
    }
}

void ModbusChannelManager::writeAll()
{
    for (auto channel : vec) {
        int rc = 0;

        setSlaveId(channel->_slaveID);

        if (channel->_do_len) {
            rc = modbus_write_bits(ctx, 0, channel->_do_len, channel->_do.data());
        }

        if (channel->_ao_len) {
            rc = modbus_write_registers(ctx, 0, channel->_ao_len, channel->_ao.data());
        }

        if (rc == -1) {
            channel->_status = 0;
        } else {
            channel->_status = 1;
        }
    }
}

ModbusChannelManager::~ModbusChannelManager()
{
    modbus_close(ctx);

    for (auto channel : vec) {
        delete channel;
    }
}

void ModbusChannelManager::reconfig(share_memory_area_t* shm_area)
{
    for (auto channel : vec) {
        delete channel;
    }

    vec.clear();

    init(shm_area);
}
void ModbusChannelManager::transferReadData()
{
    for (auto channel : vec) {
        if (channel->_di_sink) {
            response_io_status(0,
                               channel->_di_len,
                               channel->_di.data(),
                               channel->_di_sink,
                               0);
        }

        if (channel->_ai_sink) {
            uint16_t* dest_ptr = channel->_ai_sink;
            for (uint8_t i = 0; i < channel->_ai_len; ++i) {
                *dest_ptr = channel->_ai[i];
                dest_ptr++;
            }
        }
    }
}

void ModbusChannelManager::transferWriteData()
{
    for (auto channel : vec) {
        if (channel->_do_sink) {
           modbus_set_bits_from_bytes(channel->_do.data(),
                                      0,
                                      channel->_do_len,
                                      channel->_do_sink);
        }

        if (channel->_ao_sink) {
            uint16_t* src_ptr = channel->_ao_sink;
            for (uint8_t i = 0; i < channel->_ao_len; ++i) {
                channel->_ao[i] = *src_ptr;
                src_ptr++;
            }
        }
    }
}
void ModbusChannelManager::setSlaveId(uint8_t address)
{
    modbus_set_slave(ctx, address);
    usleep(5000);
}

void ModbusChannelManager::debug()
{
    for (auto channel : vec) {
        printf("-----------------------\n");
        printf("address: %d\n", channel->_slaveID);
        printf("     di: %d\n", channel->_di_len);
        printf("     ai: %d\n", channel->_ai_len);
        printf("     do: %d\n", channel->_do_len);
        printf("     ai: %d\n", channel->_ao_len);
    }
}

}
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
