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

typedef struct {
    uint8_t  id;
    uint16_t baud;
    uint8_t  parity;
    uint8_t  data_bit;
    uint8_t  stop_bit;
} consen_com_cfg_t;

typedef struct {
    uint8_t  com_id;
    uint8_t  task_id;
    uint8_t  slave_id;
    uint8_t  cmd_id;
    uint16_t addr;
    uint8_t  data_num;
    uint16_t offset;
    uint16_t period;
} consen_task_cfg_t;

class ConsenCom

{
public:
    ConsenCom(const char *device, const consen_com_cfg_t& com_cfg);
    
    void addTask(const consen_task_cfg_t& task);

    void poll();

private:
    std::vector<consen_task_cfg_t> _task_vec;
    uint16_t _task_period;
};

class ConsenComManager

{
public:
    ConsenComManager(const std::string& com_cfg_file,
                     const std::string& task_cfg_file);

    void init();

    void start();

private:
    bool parseCom();

    bool parseTask();

private:
    std::string _com_cfg_file;
    std::string _task_cfg_file;
    std::vector<consen_task_cfg_t*> _com_vec;
    std::vector<consen_task_cfg_t*> _task_vec;
};
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
