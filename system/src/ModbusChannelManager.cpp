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
#include <time.h>
#include <unistd.h>

#include "ModbusChannelManager.h"
#include "Interface.h"
#include "DataChannel.h"

using namespace std;
using namespace Zebra;


namespace {
enum {
    SPI_MODULE_UNUSED       = 0,
    SPI_MODULE_DI           = 1,
    SPI_MODULE_DO           = 2,
    SPI_MODULE_AI           = 3,
    SPI_MODULE_AO           = 4,
    SPI_MODULE_PI           = 5,

    SPI_MODULE_UNUSED_LEN   = 3,
    SPI_MODULE_DI_LEN       = 3,
    SPI_MODULE_DO_LEN       = 3,
    SPI_MODULE_AI_LEN       = 32,
    SPI_MODULE_AO_LEN       = 16,
    SPI_MODULE_PI_LEN       = 16,
};

void assemble_io_status(int address,
                        int nb,
                        uint8_t *tab_io_status,
                        uint8_t *rsp)
{
    int offset = address / 8;
    uint8_t byte = rsp[offset];
    int shift = address % 8;
    int i;

    for (i = 0; i < nb; i++) {
        if (tab_io_status[i] == 0) {
            byte &= ~(1 << shift);
        } else {
            byte |= 1 << shift;
        }

        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = byte;

            byte = rsp[offset];
            shift = 0;
        } else {
            shift++;
        }
    }

    rsp[offset] = byte;
}

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

    uint32_t  spi_counter = 0;

    uint8_t* spi_buf_base = &spi_buf[32];

    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        if (shm_area->user.io_config[i - 1].channel_type == MODULE_TYPE_DONE) {
            //TODO
            break;
        }

        if (shm_area->user.io_config[i - 1].channel_type == MODULE_NOT_PRESENT) {
            continue;
        } else {
            std::shared_ptr<DataChannel> data_channel;
            switch(shm_area->user.io_config[i - 1].channel_type) {
                case MODULE_DI:
                    data_channel.reset(new DataChannel(i,
                                                   24,
                                                   0,
                                                   0,
                                                   0,
                                                   &shm_area->user.input_di[di_counter],
                                                   nullptr,
                                                   nullptr,
                                                   nullptr,
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i] = SPI_MODULE_DI;

                    di_counter += 3;
                    spi_counter += 3;
                    break;

                case MODULE_DO:
                    data_channel.reset(new DataChannel(i,
                                                   0,
                                                   24,
                                                   0,
                                                   0,
                                                   nullptr,
                                                   &shm_area->user.output_do[do_counter],
                                                   nullptr,
                                                   nullptr,
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i] = SPI_MODULE_DO;

                    do_counter += 3;
                    spi_counter += 3;
                    break;

                case MODULE_AI:
                    data_channel.reset(new DataChannel(i,
                                                   0,
                                                   0,
                                                   32,
                                                   0,
                                                   nullptr,
                                                   nullptr,
                                                   &shm_area->user.input_ai[ai_counter],
                                                   nullptr,
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i] = SPI_MODULE_AI;

                    ai_counter += 32;
                    spi_counter += 32;
                    break;

                case MODULE_AO:
                    data_channel.reset(new DataChannel(i,
                                                   0,
                                                   0,
                                                   0,
                                                   16,
                                                   nullptr,
                                                   nullptr,
                                                   nullptr,
                                                   &shm_area->user.output_ao[ao_counter],
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i] = SPI_MODULE_AO;

                    ao_counter += 16;
                    spi_counter += 16;
                    break;

                default:
                    module_conf[i] = SPI_MODULE_UNUSED;
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
}

void ModbusChannelManager::reconfig(share_memory_area_t* shm_area)
{
    vec.clear();

    init(shm_area);
}
void ModbusChannelManager::transferReadData()
{
    for (auto channel : vec) {
        if (channel->_di_sink) {
            assemble_io_status(0,
                               channel->_di_len,
                               channel->_di.data(),
                               channel->_di_sink);
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
    struct timespec slptm;
    slptm.tv_sec = 0;
    slptm.tv_nsec = 100;
    nanosleep(&slptm, nullptr);
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
