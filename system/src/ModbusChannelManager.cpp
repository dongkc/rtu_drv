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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <Poco/Thread.h>
#include <Poco/Checksum.h>

#include "Interface.h"
#include "DataChannel.h"
#include "ModbusChannelManager.h"

using namespace std;
using namespace Zebra;
using namespace Poco;


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

}

namespace Zebra {
ModbusChannelManager::ModbusChannelManager(const string& device,
                                           uint32_t baud,
                                           const string& spi_name)
    :serial_port(device, baud)
{
    spi_fd = open(spi_name.c_str(), O_RDWR);
    uint8_t mode = SPI_MODE_0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    
    for( uint32_t i = 0; i < spi_buf.size(); ++i) {
        spi_buf[i] = 0;
    }
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

    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        shm_area->user.system_area.modules_health_flag[i -1] = MODULE_NOT_FOUND;
    }

    for (auto channel : vec) {
        shm_area->user.system_area.modules_health_flag[channel->_slaveID - 1] = MODULE_OK;
    }
}

bool ModbusChannelManager::handshake()
{
    string response;

    serial_port.writeString("STOP\r\n");
    Poco::Thread::sleep(200);
    response = serial_port.readString();
    if (response != "STOP_OK\r\n") {
        return false;
    }

    send_cmd_conf();
    Poco::Thread::sleep(2000);
    response = serial_port.readString();

    if (response != "CONF_OK\r\n") {
        return false;
    }

    serial_port.writeString("START\r\n");
    Poco::Thread::sleep(200);
    response = serial_port.readString();
    if (response != "START_OK\r\n") {
        return false;
    }

    return true;
}

void ModbusChannelManager::send_cmd_conf()
{
    string module;
    module.assign(module_conf.begin(), module_conf.end());
    string data("CONF:" + module);
    Checksum crc32(Checksum::TYPE_CRC32);
    crc32.update(data);
    serial_port.write(data.c_str(), data.size());
    UInt32 crc = crc32.checksum();
    serial_port.write(reinterpret_cast<const char*>(&crc), sizeof(uint32_t));
    serial_port.write("\r\n", 2);
}

ModbusChannelManager::~ModbusChannelManager()
{
}

void ModbusChannelManager::reconfig(share_memory_area_t* shm_area)
{
    vec.clear();

    init(shm_area);
}

}
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
