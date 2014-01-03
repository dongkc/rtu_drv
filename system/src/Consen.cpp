/*
 ******************************* C SOURCE FILE *******************************
 **                                                                         **
 ** Consen.cpp - 
 **                                                                         **
 ** 12/23/2013 05:00:19 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2010, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#include "Consen.h"
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <Poco/StringTokenizer.h>
#include <Poco/ThreadPool.h>
#include <Poco/Runnable.h>

using namespace std;
using namespace Poco;
using namespace boost;

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

ConsenComManager::ConsenComManager(const string& com_cfg_file,
                                   const string& task_cfg_file)
    : _com_cfg_file(com_cfg_file),
      _task_cfg_file(task_cfg_file),
      _consen_com_1(nullptr),
      _consen_com_2(nullptr)
{
}

void ConsenComManager::init()
{
    parseCom();
    parseTask();
}

void ConsenComManager::debug()
{
    printf("\n--------------------------------\n");
    for (auto cfg : _com_vec) {
        printf("id %d baud %d parity %d data_bit %d stop_bit %d period %d\n",
               cfg.id,
               cfg.baud,
               cfg.parity,
               cfg.data_bit,
               cfg.stop_bit,
               cfg.period);
    }

    for (auto task : _task_vec_1) {
        printf("com_id %d task_id %d slave_id %d cmd_id %d addr %d data_num %d offset %d\n",
               task.com_id,
               task.task_id,
               task.slave_id,
               task.cmd_id,
               task.addr,
               task.data_num,
               task.offset);
    }
    printf("--------------------------------\n");
}

void ConsenComManager::start(const uint8_t *modbus_bool_ptr,
                             const uint16_t *modbus_word_ptr)
{
    consen_com_cfg_t com1_cfg;
    consen_com_cfg_t com2_cfg;

    for (auto cfg : _com_vec) {
        switch (cfg.id) {
            case 0:
                com1_cfg = cfg;
                break;
            case 1:
                com2_cfg = cfg;
                break;
            default:
                break;
        }
    }

    debug();
#if 1
    if (!_task_vec_1.empty()) {
        _consen_com_1 = new ConsenThread(_task_vec_1,
                                         "/dev/ttyS4",
                                         com1_cfg,
                                         modbus_bool_ptr,
                                         modbus_word_ptr);
        ThreadPool::defaultPool().start(*_consen_com_1);
    }

    if (!_task_vec_2.empty()) {
        _consen_com_2 = new ConsenThread(_task_vec_2,
                                         "/dev/ttyS2",
                                         com2_cfg,
                                         modbus_bool_ptr,
                                         modbus_word_ptr);
        ThreadPool::defaultPool().start(*_consen_com_2);
    }
#endif
}

bool ConsenComManager::parseCom()
{
    consen_com_cfg_t *com_cfg_ptr = nullptr;
    string s;
    ifstream com_cfg(_com_cfg_file);
    while (getline(com_cfg, s)) {
        StringTokenizer tokens(s, ":", StringTokenizer::TOK_TRIM);
        if (tokens.count() != 6) {
            continue;
        }
        
        com_cfg_ptr = new consen_com_cfg_t;
        com_cfg_ptr->id         = lexical_cast<uint16_t>(tokens[0]);
        com_cfg_ptr->baud       = lexical_cast<uint32_t>(tokens[1]);
        com_cfg_ptr->parity     = lexical_cast<uint8_t> (tokens[2]);
        com_cfg_ptr->data_bit   = lexical_cast<uint16_t>(tokens[3]);
        com_cfg_ptr->stop_bit   = lexical_cast<uint16_t>(tokens[4]);
        com_cfg_ptr->period     = lexical_cast<uint16_t>(tokens[5]);

        _com_vec.push_back(*com_cfg_ptr);
    }

    return true;
}

bool ConsenComManager::parseTask()
{
    consen_task_cfg_t *task_cfg_ptr = nullptr;
    string s;
    ifstream task_cfg(_task_cfg_file);
    while (getline(task_cfg, s)) {
        StringTokenizer tokens(s, ":", StringTokenizer::TOK_TRIM);
        if (tokens.count() != 7) {
            continue;
        }

        task_cfg_ptr = new consen_task_cfg_t;
        task_cfg_ptr->com_id     = lexical_cast<uint16_t>(tokens[0]);
        task_cfg_ptr->task_id    = lexical_cast<uint16_t>(tokens[1]);
        task_cfg_ptr->slave_id   = lexical_cast<uint16_t>(tokens[2]);
        task_cfg_ptr->cmd_id     = lexical_cast<uint16_t>(tokens[3]);
        task_cfg_ptr->addr       = lexical_cast<uint16_t>(tokens[4]);
        task_cfg_ptr->data_num   = lexical_cast<uint16_t>(tokens[5]);
        task_cfg_ptr->offset     = lexical_cast<uint16_t>(tokens[6]);

        switch (task_cfg_ptr->com_id) {
            case 0:
                _task_vec_1.push_back(*task_cfg_ptr);
                break;
            case 1:
                _task_vec_2.push_back(*task_cfg_ptr);
                break;
            default:
                delete task_cfg_ptr;
        }
    }

    return true;
}

void ConsenThread::init()
{
    ctx = modbus_new_rtu(_com_path.c_str(),
                         _com.baud,
                         _com.parity,
                         _com.data_bit,
                         _com.stop_bit);
    modbus_connect(ctx);
    //modbus_set_debug(ctx, TRUE);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
}

void ConsenThread::setAddr(uint8_t address)
{
    modbus_set_slave(ctx, address);
    usleep(5000);
}

void ConsenThread::poll()
{
    int32_t rc;
    for (auto task : _task_vec) {
        setAddr(task.slave_id);

        static uint8_t buf[256];
        switch (task.cmd_id) {
            case MODBUS_READ_DISCRETE_INPUTS:
                rc = modbus_read_input_bits(ctx,
                                       task.addr,
                                       task.data_num,
                                       buf);
                if (rc < 0) {
                    //printf("return code: %d\n", rc);
                }
                response_io_status(0,
                                   task.data_num,
                                   buf,
                                   (uint8_t*)(_modbus_bool_region + task.offset),
                                   0);
                break;
            case MODBUS_READ_INPUT_REGISTER:
                modbus_read_input_registers(ctx,
                                            task.addr,
                                            task.data_num,
                                            (uint16_t*)(_modbus_word_region + task.offset));
                break;
            case MODBUS_WRITE_MULTIPLE_COILS:
                modbus_write_bits(ctx,
                                  task.addr,
                                  task.data_num,
                                  _modbus_bool_region + task.offset);
                break;
            case MODBUS_WRITE_MULTIPLE_REGISTERS:
                modbus_write_registers(ctx,
                                       task.addr,
                                       task.data_num,
                                       _modbus_word_region + task.offset);
            default:
                break;
        }
    }
}

void ConsenThread::run()
{
    init();

    while (1) {
        poll();
        //usleep(_com.period * 1000);
    }
}

/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
