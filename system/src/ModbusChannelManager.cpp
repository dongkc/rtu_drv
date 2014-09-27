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
#include <iostream>

#include "Interface.h"
#include "DataChannel.h"
#include "ModbusChannelManager.h"

using namespace std;
using namespace Zebra;
using namespace Poco;


namespace {

enum {
    SPI_MODULE_UNUSED       = 0x30 + 0,
    SPI_MODULE_DI           = 0x30 + 1,
    SPI_MODULE_DO           = 0x30 + 2,
    SPI_MODULE_AI           = 0x30 + 3,
    SPI_MODULE_AO           = 0x30 + 4,
    SPI_MODULE_PI           = 0x30 + 5,

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
    :serial_port(device, baud),
    data_ready("26", Gpio::GPIO_IN),
    ack_ready("27", Gpio::GPIO_IN)
{
    data_ready.init();
    ack_ready.init();

    spi_fd = open(spi_name.c_str(), O_RDWR);
    uint8_t mode = SPI_MODE_0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    
    for( uint32_t i = 0; i < spi_buf.size(); ++i) {
        spi_buf[i] = 0;
    }

    for (uint8_t i = 0; i < 32; ++i) {
        module_conf[i] = SPI_MODULE_UNUSED;
    }
}

void ModbusChannelManager::init(share_memory_area_t* shm_area)
{
    uint32_t  di_counter = 0;
    uint32_t  ai_counter = 0;
    uint32_t  do_counter = 0;
    uint32_t  ao_counter = 0;

    for(uint8_t i = 0; i < 32; ++i) {
        spi_buf[i] = 0xFF;
    }
    uint8_t* spi_buf_base = &spi_buf[32];
    uint32_t spi_counter = 0;
    printf("SPI BUF: %p\n", &spi_buf[32]);

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
                    data_channel.reset(new DataChannel(SPI_MODULE_DI,
                                                   24,
                                                   &shm_area->user.input_di[di_counter],
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i - 1] = SPI_MODULE_DI;

                    di_counter += 3;
                    spi_counter += 3;
                    break;

                case MODULE_AI:
                    data_channel.reset(new DataChannel(SPI_MODULE_AI,
                                                   32,
                                                   &shm_area->user.input_ai[ai_counter],
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i - 1] = SPI_MODULE_AI;

                    ai_counter += 16;
                    spi_counter += 32;
                    break;

                case MODULE_DO:
                    data_channel.reset(new DataChannel(SPI_MODULE_DO,
                                                   24,
                                                   &shm_area->user.output_do[do_counter],
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i - 1] = SPI_MODULE_DO;

                    do_counter += 3;
                    spi_counter += 3;
                    break;

                case MODULE_AO:
                    data_channel.reset(new DataChannel(SPI_MODULE_AO,
                                                   16,
                                                   &shm_area->user.output_ao[ao_counter],
                                                   spi_buf_base + spi_counter));
                    vec.push_back(data_channel);
                    module_conf[i - 1] = SPI_MODULE_AO;

                    ao_counter += 8;
                    spi_counter += 16;
                    break;

                default:
                    module_conf[i - 1] = SPI_MODULE_UNUSED;
                    break;
            }
        }
    }

    spi_transfer_len = 32 + spi_counter + 4;

    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        shm_area->user.system_area.modules_health_flag[i -1] = MODULE_NOT_FOUND;
    }

#if 0
    for (auto channel : vec) {
        //shm_area->user.system_area.modules_health_flag[channel->_slaveID - 1] = MODULE_OK;
    }
#endif
}

UInt32 ModbusChannelManager::spi_transfer()
{
    union {
        UInt32 crc32;
        uint8_t crc_array[4];
    } crc_type;

    Checksum crc32(Checksum::TYPE_CRC32);
    crc32.update(reinterpret_cast<const char*>(spi_buf.data()), spi_transfer_len - 4);
    crc_type.crc32 = crc32.checksum();
    for (int i = 0; i < 4; ++i) {
        spi_buf[spi_transfer_len - 4 + i] = crc_type.crc_array[i];
    }

	struct spi_ioc_transfer tr[1] = {
        {
            (unsigned long)&spi_buf[0],
            (unsigned long)&spi_buf_rx[0],
            spi_transfer_len,
            0,
            0,
            8,
        },
   	};

	ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

#if 0
    for (uint32_t i = 0; i < 15; ++i) {
        spi_buf[32 + i] = 0xF0;
//        printf("%x  ", spi_buf[i]);
    }
#endif

    printf("TX: ");
    for (uint32_t i = 0; i < spi_transfer_len; ++i) {
        printf("%x  ", spi_buf[i]);
    }
    printf("\n");

    cout << "length: " << spi_transfer_len << endl;
    printf("RX: ");
    for (uint32_t i = 0; i < spi_transfer_len; ++i) {
        printf("%x  ", spi_buf_rx[i]);
    }
    printf("\n");

    return crc_type.crc32;
}

UInt32 ModbusChannelManager::spi_checksum()
{
    union {
        UInt32 crc32;
        uint8_t crc_array[4];
    } crc_type, rx_ack;

    Checksum crc32(Checksum::TYPE_CRC32);
    crc32.update(reinterpret_cast<const char*>(spi_buf_rx.data()), spi_transfer_len - 4);
    crc_type.crc32 = crc32.checksum();

    std::array<uint8_t, 4>    tx;
    std::array<uint8_t, 4>    rx;
    
    for (int i = 0; i < 4; ++i) {
        tx[i] = crc_type.crc_array[i];
    }

	struct spi_ioc_transfer tr[1] = {
        {
            (unsigned long)&tx[0],
            (unsigned long)&rx[0],
            4,
            0,
            0,
            8,
        },
   	};

	ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

    printf("TX: ");
    for (uint32_t i = 0; i < 4; ++i) {
        printf("%x  ", tx[i]);
    }
    printf("\n");
    printf("RX: ");
    for (uint32_t i = 0; i < 4; ++i) {
        printf("%x  ", rx[i]);
    }
    printf("\n");

    for (int i = 0; i < 4; ++i) {
        rx_ack.crc_array[i] = crc_type.crc_array[i];
    }
    return rx_ack.crc32;
}

void ModbusChannelManager::read(Zebra::DataChannel* channel)
{
//    printf("SPI len: %d SINK: %p DI: %p\n", channel->_len, channel->_sink, channel->_source);
    if (channel->_type == SPI_MODULE_DI) {
        for (int i = 0; i < 3; ++i) {
            *(channel->_sink + i) = *(static_cast<uint8_t*>(channel->_source) + i);
            //printf("di index: %d data: %x\n", i, *(channel->_sink) + i);
        }
    }

    if (channel->_type == SPI_MODULE_AI) {
        for (int i = 0; i < 32; ++i) {
            *(channel->_sink + i) = *(static_cast<uint8_t*>(channel->_source) + i);
            //printf("ai index: %d data: %x\n", i, *(reinterpret_cast<uint16_t*>(channel->_sink) + i));
        }
    }
}

void ModbusChannelManager::readAll()
{
    for (auto e : vec) {
        read(e.get());
    }
}

void ModbusChannelManager::write(Zebra::DataChannel* channel)
{
    if (channel->_type == SPI_MODULE_DO) {
        for (int i = 0; i < 3; ++i) {
//            *(static_cast<uint8_t*>(channel->_sink) + i) = *(channel->_source+i);
            *(static_cast<uint8_t*>(channel->_sink) + i) = *(static_cast<uint8_t*>(channel->_source) + i);
        }
    }

    if (channel->_type == SPI_MODULE_AO) {
        for (int i = 0; i < 16; ++i) {
            *(static_cast<uint8_t*>(channel->_sink) + i) = *(static_cast<uint8_t*>(channel->_source) + i);
            //*(static_cast<uint8_t*>(channel->_source) + i) = *(channel->_sink + i);
//            printf("ao index: %d data: %x\n", i, *(reinterpret_cast<uint16_t*>(channel->_sink) + i));
        }
    }
}

void ModbusChannelManager::writeAll()
{
    for (auto e : vec) {
        write(e.get());
    }
}

bool ModbusChannelManager::handshake()
{
    string response;

    serial_port.writeString("STOP\r\n");
    Poco::Thread::sleep(1000);
    response = serial_port.readString();
    if (response != "STOP_OK\r\n") {
        cout << "stop error: " << response << endl;
        return false;
    }

    send_cmd_conf();
    Poco::Thread::sleep(2000);
    response = serial_port.readString();

    if (response != "CONF_OK\r\n") {
        cout << "conf error" << endl;
        return false;
    }

    serial_port.writeString("START\r\n");
    Poco::Thread::sleep(200);
    response = serial_port.readString();
    if (response != "START_OK\r\n") {
        cout << "start error" << endl;
        return false;
    }

    return true;
}

void ModbusChannelManager::send_cmd_conf()
{
    string module;
    module.assign(module_conf.begin(), module_conf.end());
    string data("CONF:" + module);

    cout << endl;
    for (uint32_t i = 0; i < data.size(); ++i) {
        printf("%x", data[i]);
    }
    cout << endl;

    Checksum crc32(Checksum::TYPE_CRC32);
    crc32.update(data);
    serial_port.write(data.c_str(), data.size());
    UInt32 crc = crc32.checksum();
    printf("CRC: %x\n", crc);
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
