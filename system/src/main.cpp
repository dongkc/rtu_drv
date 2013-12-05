/*
 ******************************* C SOURCE FILE *******************************
 **                                                                         **
 ** main.cpp - 
 **                                                                         **
 ** 11/30/2013 03:58:03 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2010, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#define __MAIN_SRC__
/****************************************************************************/
/**                             MODULES USED                               **/
/****************************************************************************/
#include <iostream>
#include <Poco/SharedMemory.h>
#include <Poco/DateTime.h>
#include <Poco/Net/IPAddress.h>
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include "Interface.h"
#include "net_config.h"
#include "ModbusChannelManager.h"

using namespace std;
using namespace boost;
using Poco::SharedMemory;
using Poco::DateTime;
using Poco::Net::IPAddress;

namespace Zebra {

void updateTime(system_area_t& system_area)
{
    DateTime date;
    system_area.run_year = date.year();
    system_area.run_mon  = date.month();
    system_area.run_day  = date.day();
    system_area.run_hour = date.hour();
    system_area.run_min  = date.minute();
    system_area.run_sec  = date.second();
    system_area.run_ms   = date.millisecond();
}

void setTime(const system_area_t& system_area)
{
    struct tm tm_time;

    tm_time.tm_year = system_area.set_year;
    tm_time.tm_mon  = system_area.set_mon;
    tm_time.tm_mday = system_area.set_day;
    tm_time.tm_hour = system_area.set_hour;
    tm_time.tm_min  = system_area.set_min;
    tm_time.tm_sec  = system_area.set_sec;

    struct timeval time_value;

    time_value.tv_sec = mktime(&tm_time);
    settimeofday(&time_value, NULL);
}

string ipToStr(const ip_struct_t& raw_ip)
{
    return lexical_cast<string>(raw_ip.ip_section_1) + "." +
           lexical_cast<string>(raw_ip.ip_section_2) + "." +
           lexical_cast<string>(raw_ip.ip_section_3) + "." +
           lexical_cast<string>(raw_ip.ip_section_4);
}

void setNetworkParas(system_area_t& system_area)
{
    const string ip(ipToStr(system_area.set_ip));
    const string mask(ipToStr(system_area.set_subnet_mask));
    const string gateway(ipToStr(system_area.set_gateway));
    const string dns(ipToStr(system_area.set_dns));

    IPAddress ip_address;
    if (Poco::Net::IPAddress::tryParse(ip, ip_address) &&
        Poco::Net::IPAddress::tryParse(mask, ip_address) &&
        Poco::Net::IPAddress::tryParse(gateway, ip_address) &&
        Poco::Net::IPAddress::tryParse(dns, ip_address) == false) {

        return;
    }

//    setIP("manual", ip, mask, gateway);
//
//    vector<string> dns_vec{dns};
//    setDNS(dns_vec);
}

extern "C" int main(int argc, const char *argv[])
{
    SharedMemory mem("ConsenShareMemory",
                      SHARE_MEMORY_TOTAL_LENGTH,
                      SharedMemory::AM_WRITE);

    share_memory_area_t* shm_ptr = new (mem.begin()) share_memory_area_t;

    ModbusChannelManager modbus_mgr("/dev/ttyS1", 115200, 'N', 8, 1);

    modbus_mgr.init(*shm_ptr);

    while (1) {
        modbus_mgr.readAll();
        modbus_mgr.transfer();
        modbus_mgr.writeAll();
        updateTime(shm_ptr->user.system_area);
        setTime(shm_ptr->user.system_area);
        usleep(10* 1000);
    }
}


}  //  namespace Zebra
