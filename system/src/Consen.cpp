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
#define __CONSEN_SRC__
#include "Consen.h"
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace Poco;
using namespace boost;

ConsenComManager::ConsenComManager(const string& com_cfg_file,
                                   const string& task_cfg_file)
    : _com_cfg_file(com_cfg_file),
      _task_cfg_file(task_cfg_file)
{
}

void ConsenComManager::init()
{

}

void ConsenComManager::parseCom()
{
    string s;
    ifstream com_cfg(_com_cfg_file);
    while (getline(com_cfg, s)) {
        StringTokenizer tokens(s, ":", StringTokenizer::TOK_TRIM);
        if (tokens.count() != 8) {
            continue;
        }
        
        static consen_com_cfg_t *com_cfg_ptr = new consen_com_cfg_t;

        _com_vec.push_back(com_cfg_ptr);
    }
}

void ConsenComManager::parseTask()
{
    consen_task_cfg_t *task_cfg_ptr = nullptr;
    string s;
    ifstream task_cfg(_task_cfg_file);
    while (getline(task_cfg, s)) {
        StringTokenizer tokens(s, ":", StringTokenizer::TOK_TRIM);
        if (tokens.count() != 10) {
            continue;
        }

        static consen_task_cfg_t *task_cfg_ptr = new consen_task_cfg_t;
        task_cfg_ptr->com_id     = lexical_cast<uint8_t> (tokens[0]);
        task_cfg_ptr->task_id    = lexical_cast<uint8_t> (tokens[1]);
        task_cfg_ptr->slave_id   = lexical_cast<uint8_t> (tokens[2]);
        task_cfg_ptr->cmd_id     = lexical_cast<uint8_t> (tokens[3]);
        task_cfg_ptr->addr       = lexical_cast<uint16_t>(tokens[4]);
        task_cfg_ptr->data_num   = lexical_cast<uint8_t> (tokens[5]);
        task_cfg_ptr->offset     = lexical_cast<uint16_t>(tokens[6]);
        task_cfg_ptr->period     = lexical_cast<uint16_t>(tokens[7]);

    }
}

/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
