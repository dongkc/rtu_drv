/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** Interface.h - interface between os and kw eclr kernel
 **                                                                         **
 ** 11/30/2013 02:04:10 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2011, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __INTERFACE_HEADER__
#define __INTERFACE_HEADER__

#include <stdint.h>

namespace Zebra {
    
static const uint32_t SHARE_MEMORY_TOTAL_LENGTH       = 65536;
static const uint32_t SHARE_MEMORY_DI_LEN             = 512; 
static const uint16_t SHARE_MEMORY_AI_LEN             = 2048 / 2;
static const uint32_t SHARE_MEMORY_INPUT_REAL_LEN     = 512 / 4; 
static const uint32_t SHARE_MEMORY_DO_LEN             = 512; 
static const uint16_t SHARE_MEMORY_AO_LEN             = 512 / 2; 
static const uint32_t SHARE_MEMORY_RESERVED_LEN       = 128 + 1024 + 1024; 
static const uint32_t SHARE_MEMORY_SYSTEM_LEN         = 2048 / 2; 
static const uint32_t SHARE_MEMORY_IO_CONFIG_LEN      = 2048 / 2; 
static const uint32_t SHARE_MEMORY_MODBUS_BYTE_LEN    = 256; 
static const uint32_t SHARE_MEMORY_MODBUS_WORD_LEN    = 4096 / 2; 

#pragma pack(1)
typedef struct {
    uint64_t run_time; /// run time, unit is 0.1 ms
    uint32_t run_ms;
    uint8_t  run_sec;
    uint8_t  run_min;
    uint8_t  run_hour;
    uint8_t  run_day;
    uint8_t  run_mon;
    uint16_t run_year;
    uint8_t  g_remain_flag;
    uint8_t  set_sec;
    uint8_t  set_min;
    uint8_t  set_hour;
    uint8_t  set_day;
    uint8_t  set_mon;
    uint8_t  set_time_flag;
    uint16_t set_year;
    uint32_t set_ip;
    uint32_t set_subnet_mask;
    uint32_t set_gateway;
    uint32_t set_dns;
    uint8_t  set_ip_flag;
    uint8_t  io_update_type;
    uint8_t  io_update_period;
    uint8_t  io_update_read_cmd;
    uint8_t  io_update_write_cmd;

    uint8_t  reserved_area[983];
    uint8_t  modules_health_flag[1024];
} system_area_t;

#pragma pack(1)
typedef union {
    uint8_t share_memory[SHARE_MEMORY_TOTAL_LENGTH];
    struct {
        uint8_t  input_di[SHARE_MEMORY_DI_LEN];
        uint16_t input_ai[SHARE_MEMORY_AI_LEN];
        uint32_t input_real[SHARE_MEMORY_INPUT_REAL_LEN];
        uint8_t  output_do[SHARE_MEMORY_DO_LEN];
        uint16_t output_ao[SHARE_MEMORY_AO_LEN];
        uint8_t  reserved[SHARE_MEMORY_RESERVED_LEN];
        system_area_t system_area;
        uint16_t io_config[SHARE_MEMORY_IO_CONFIG_LEN];
        uint8_t  modbus_byte[SHARE_MEMORY_MODBUS_BYTE_LEN];
        uint16_t modbus_word[SHARE_MEMORY_MODBUS_WORD_LEN];
    } user;
} share_memory_area_t;

}
#ifndef __INTERFACE_SRC__
#endif
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
