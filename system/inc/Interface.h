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
static const uint32_t SHARE_MEMORY_AO_LEN             = 512 / 2; 
static const uint32_t SHARE_MEMORY_RESERVED_LEN       = 128 + 1024 + 1024; 
static const uint32_t SHARE_MEMORY_SYSTEM_LEN         = 2048 / 2; 
static const uint32_t SHARE_MEMORY_IO_CONFIG_LEN      = 256 / 2; 
static const uint32_t SHARE_MEMORY_MODBUS_BYTE_LEN    = 256; 
static const uint32_t SHARE_MEMORY_MODBUS_WORD_LEN    = 4096 / 2; 

#pragma pack(1)
typedef struct {
    uint8_t ip_section_1;
    uint8_t ip_section_2;
    uint8_t ip_section_3;
    uint8_t ip_section_4;
} ip_struct_t;

typedef enum {
    MODULE_NOT_PRESENT = 0,

    MODULE_DI_16 = 1,
    MODULE_DO_16 = 2,
    MODULE_AI_16 = 3,
    MODULE_AO_08 = 4,
    MODULE_PI_08 = 5,
    MODULE_DI_32 = 6,
    MODULE_DO_32 = 7,
    MODULE_AI_32 = 8,
    MODULE_DAIO_32 = 9,
    MODBUS_RTU_CM = 10,

    MODULE_TYPE_DONE = 255
} module_type_t;

#pragma pack(1)
typedef struct {
    uint8_t channel_type;
    uint8_t channel_num;
} io_config_t;

typedef enum {
    MODULE_DISCONNECT = 0,
    MODULE_OK = 1,
    MODULE_INTERNAL_ERROR = 2,
    MODULE_NOT_FOUND = 255
} module_status_t;

#pragma pack(1)
typedef struct {
    uint64_t up_time; /// run time, unit is 0.1 ms
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
    uint16_t set_year;
    uint8_t  set_time_flag;
    ip_struct_t set_ip;
    ip_struct_t set_subnet_mask;
    ip_struct_t set_gateway;
    ip_struct_t set_dns;
    uint8_t  set_ip_flag;
    uint8_t  io_update_type;
    uint16_t io_update_period;
    uint8_t  io_update_read_cmd;
    uint8_t  io_update_write_cmd;
    uint8_t  io_update_config;
    uint8_t  modules_health_flag[256];
    uint8_t  reserved_area[1741];
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
        io_config_t io_config[SHARE_MEMORY_IO_CONFIG_LEN];
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
