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
#include <Poco/SharedMemory.h>
#include <Poco/DateTime.h>
#include <ctime>
#include <sys/time.h>
#include <sys/shm.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include "Interface.h"

using namespace std;
using namespace boost;
using Poco::SharedMemory;
using Poco::DateTime;

namespace {

void assemble_io_status(int address,
                        int nb,
                        uint8_t *tab_io_status,
                        uint8_t *rsp)
{
    int offset = address / 8;
    uint8_t byte = rsp[offset];
    int shift = address % 8;
    int i;

    for (i = 0; i < nb; i++) {
        if (tab_io_status[i] == 1) {
            byte |= 1 << shift;
        } else {
            byte &= ~(1 << shift);
        }

        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = byte;
            byte = rsp[offset];
            shift = 0;
        } else {
            shift++;
        }
    }

    rsp[offset] = byte;
}

}

namespace Zebra {

void updateTime(share_memory_area_t* shm_ptr)
{
    DateTime date;
    shm_ptr->user.system_area.run_year = date.year();
    shm_ptr->user.system_area.run_mon  = date.month();
    shm_ptr->user.system_area.run_day  = date.day();
    shm_ptr->user.system_area.run_hour = date.hour();
    shm_ptr->user.system_area.run_min  = date.minute();
    shm_ptr->user.system_area.run_sec  = date.second();
    shm_ptr->user.system_area.run_ms   = date.millisecond();
}

void setTime(share_memory_area_t* shm_ptr)
{
    struct tm tm_time;

    tm_time.tm_year = shm_ptr->user.system_area.set_year;
    tm_time.tm_mon  = shm_ptr->user.system_area.set_mon;
    tm_time.tm_mday = shm_ptr->user.system_area.set_day;
    tm_time.tm_hour = shm_ptr->user.system_area.set_hour;
    tm_time.tm_min  = shm_ptr->user.system_area.set_min;
    tm_time.tm_sec  = shm_ptr->user.system_area.set_sec;

    struct timeval time_value;

    time_value.tv_sec = mktime(&tm_time);
    settimeofday(&time_value, NULL);
}

void debugTime(share_memory_area_t* shm_ptr)
{
    printf("=====================================================\n");
    printf("up_time\t%lld\n" ,shm_ptr->user.system_area.up_time); 
    printf("run_year\t%d\n" ,shm_ptr->user.system_area.run_year); 
    printf("run_mon\t\t%d\n"   ,shm_ptr->user.system_area.run_mon ); 
    printf("run_day\t\t%d\n"   ,shm_ptr->user.system_area.run_day ); 
    printf("run_hour\t%d\n"  ,shm_ptr->user.system_area.run_hour); 
    printf("run_min\t\t%d\n"   ,shm_ptr->user.system_area.run_min ); 
    printf("run_sec\t\t%d\n"   ,shm_ptr->user.system_area.run_sec ); 
    printf("run_ms\t\t%d\n"    ,shm_ptr->user.system_area.run_ms  ); 
}

unsigned char *fnShareMemory()
{	key_t key;
	int shm_id;
	char pathname[255];
	strcpy(pathname,"/tmp") ;
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

void printModbusArea(const share_memory_area_t* shm_ptr)
{
    printf("MODBUS BOOL ========== %p offset %d %d\n",
            (uint8_t*)&shm_ptr->user.modbus_bool,
            (uint8_t*)&shm_ptr->user.io_config - (uint8_t*)shm_ptr,
            (uint8_t*)&shm_ptr->user.modbus_bool - (uint8_t*)shm_ptr);

    for (uint8_t i = 0; i < 20; ++i) {
        printf("%X ", shm_ptr->user.modbus_bool[i]);
    }
    printf("\n");

    printf("MODBUS WORD ========== %p offset %d\n",
            (uint8_t*)&shm_ptr->user.modbus_word,
            (uint8_t*)&shm_ptr->user.modbus_word - (uint8_t*)shm_ptr);

    for (uint8_t i = 0; i < 20; ++i) {
        printf("%X ", shm_ptr->user.modbus_word[i]);
    }
    printf("\n");

}

extern "C" int main(int argc, const char *argv[])
{
    unsigned char *raw_shm_ptr = fnShareMemory();
    share_memory_area_t* shm_ptr = new (raw_shm_ptr) share_memory_area_t;


    printf("size of shm : %d\n", sizeof(shm_ptr->user.system_area));
    while (1) {

        //debugTime(shm_ptr);
        printModbusArea(shm_ptr);
        usleep(1000* 1000);
    }
}

}  //  namespace Zebra
