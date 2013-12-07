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
#include <sys/shm.h>
#include <string>
#include <fstream>
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

#if 0
    setIP("manual", ip, mask, gateway);

    vector<string> dns_vec{dns};
    setDNS(dns_vec);
#endif
}

unsigned char *fnShareMemory()
{	key_t key;
	int shm_id;
	char pathname[255];
	strcpy(pathname,"/tmps") ;
	key = ftok(pathname,0x03);
	if(key==-1)
	{
		printf("ftok error");
	}
	printf("key=%d\n",key) ;
	shm_id=shmget(key,65536,IPC_CREAT|IPC_EXCL|0600);
	
	if(shm_id==-1)
	{
		printf("shmget error");
		shm_id=shmget(key,0,0);

	}
	printf("shm_id%d\n",shm_id) ;
	printf("shm_id=%d\n", shm_id) ;
	unsigned char* SHM_Buff_SYS =(unsigned char*)shmat(shm_id,NULL,0);
    return SHM_Buff_SYS;

}

extern "C" int main(int argc, const char *argv[])
{
#if 0
    SharedMemory mem("/tmp",
                      SHARE_MEMORY_TOTAL_LENGTH,
                      SharedMemory::AM_WRITE);
#endif

    unsigned char *raw_shm_ptr = fnShareMemory();
    //share_memory_area_t* shm_ptr = new (mem.begin()) share_memory_area_t;
    share_memory_area_t* shm_ptr = new (raw_shm_ptr) share_memory_area_t;

    ModbusChannelManager modbus_mgr("/dev/ttyS1", 115200, 'N', 8, 1);

//    printf("%p %p\n", &shm_ptr->user.io_config, &shm_ptr->share_memory[8320]);
#if 0
    for (int i = 0; i < 100; ++i) {
        printf("%d, %d, %p\n", i, shm_ptr->share_memory[8320 + i], &shm_ptr->share_memory[8320 + i]);
    }
#endif

    shm_ptr->user.io_config[1].channel_type = 9;
    shm_ptr->user.io_config[2].channel_type = 9;
    shm_ptr->user.io_config[3].channel_type = 9;
    shm_ptr->user.io_config[4].channel_type = 9;
    shm_ptr->user.io_config[5].channel_type = 255;

    shm_ptr->user.output_do[0] = 0xFF;
    shm_ptr->user.output_do[1] = 0xFF;
    shm_ptr->user.output_do[2] = 0xFF;
    shm_ptr->user.output_do[3] = 0xFF;

    modbus_mgr.init(shm_ptr);

    system_area_t& system_area = shm_ptr->user.system_area;

    while (1) {

#if 0
    for (int i = 0; i < 10; ++i) {
        printf("%d, %d, %p\n", i, shm_ptr->share_memory[3072 + i], &shm_ptr->share_memory[3072 + i]);
    }
#endif
        if (system_area.io_update_config == 1) {
            printf("=======================\n");
            modbus_mgr.reconfig(shm_ptr);
            system_area.io_update_config = 0;
        }

        //modbus_mgr.debug();
        system_area.io_update_type = 1;
        switch (system_area.io_update_type) {
            case 1:
                modbus_mgr.readAll();
                modbus_mgr.transferReadData();

                modbus_mgr.transferWriteData();
                modbus_mgr.writeAll();
                break;

            case 2:
                modbus_mgr.readAll();
                while (system_area.io_update_read_cmd != 1);
                modbus_mgr.transferReadData();
                system_area.io_update_read_cmd = 0;

                while (system_area.io_update_write_cmd != 1);
                modbus_mgr.transferWriteData();
                system_area.io_update_write_cmd = 0;
                modbus_mgr.writeAll();
                break;

            default:
                break;
        }

        if (system_area.set_time_flag == 255) {
            setTime(system_area);
            system_area.set_time_flag = 0;
        }
        updateTime(system_area);

        usleep(1000 * (system_area.io_update_period > 0 ? system_area.io_update_period : 2));
    }
}


}  //  namespace Zebra
