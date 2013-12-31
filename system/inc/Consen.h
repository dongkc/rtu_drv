/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** Consen.h - interface for consen third modbus slaves
 **                                                                         **
 ** 12/23/2013 02:50:02 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2011, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __CONSEN_HEADER__
#define __CONSEN_HEADER__
#include <stdint.h>
#include <string>
#include <vector>
#include <Poco/Runnable.h>
#include "modbus.h"

typedef struct {
    uint8_t  id;
    uint16_t baud;
    uint8_t  parity;
    uint8_t  data_bit;
    uint8_t  stop_bit;
    uint16_t period;
} consen_com_cfg_t;

typedef struct {
    uint8_t  com_id;
    uint8_t  task_id;
    uint8_t  slave_id;
    uint8_t  cmd_id;
    uint16_t addr;
    uint8_t  data_num;
    uint16_t offset;
} consen_task_cfg_t;

class ConsenThread : public Poco::Runnable
{
public:
    enum {
        MODBUS_READ_COILS                  = 1,
        MODBUS_READ_DISCRETE_INPUTS        = 2,
        MODBUS_READ_HOLDING_REGISTER       = 3,
        MODBUS_READ_INPUT_REGISTER         = 4,
        MODBUS_WRITE_SINGLE_COIL           = 5,
        MODBUS_WRITE_REGISTER              = 6,
        MODBUS_WRITE_MULTIPLE_COILS        = 15,
        MODBUS_WRITE_MULTIPLE_REGISTERS    = 16
    };

    ConsenThread(const std::vector<consen_task_cfg_t>& task_vec,
                 const std::string& com_path,
                 const consen_com_cfg_t&  com,
                 const uint8_t *modbus_bool,
                 const uint16_t *modbus_word)
        : _task_vec(task_vec),
          _com_path(com_path),
          _com(com),
          _modbus_bool_region(modbus_bool),
          _modbus_word_region(modbus_word)
    {};

    void run();

private:
    void init();
    void setAddr(uint8_t address);
    void poll();

private:
    std::vector<consen_task_cfg_t> _task_vec;
    const std::string _com_path;
    consen_com_cfg_t _com;
    const uint8_t *_modbus_bool_region;
    const uint16_t *_modbus_word_region;
    modbus_t *ctx;
};

class ConsenCom

{
public:
    ConsenCom(const char *device, const consen_com_cfg_t& com_cfg);
    
    void addTask(const consen_task_cfg_t& task);

    void poll();

private:
    std::vector<consen_task_cfg_t*> _task_vec;
    uint16_t _task_period;
};

class ConsenComManager

{
public:
    ConsenComManager(const std::string& com_cfg_file,
                     const std::string& task_cfg_file);

    void init();

    void start(const uint8_t *modbus_bool_ptr,
               const uint16_t *modbus_word_ptr);

private:
    bool parseCom();

    bool parseTask();

private:
    std::string _com_cfg_file;
    std::string _task_cfg_file;
    std::vector<consen_com_cfg_t>  _com_vec;
    std::vector<consen_task_cfg_t> _task_vec_1;
    std::vector<consen_task_cfg_t> _task_vec_2;
    ConsenThread *_consen_com_1;
    ConsenThread *_consen_com_2;
};
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
