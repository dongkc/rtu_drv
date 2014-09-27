#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <memory>
#include <sched.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Poco/SharedMemory.h>
#include <Poco/DateTime.h>
#include <Poco/Timer.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/AutoPtr.h>
#include <Poco/FileChannel.h>
#include <Poco/AsyncChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include "Interface.h"
#include "ModbusChannelManager.h"
#include "lym_RT01.h"
#include "Consen.h"
#include "BufferedAsyncSerial.h"
#include "Gpio.h"
#include <time.h>

using namespace std;
using namespace boost;

using Poco::SharedMemory;
using Poco::DateTime;
using Poco::Net::IPAddress;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::AutoPtr;
using Poco::FileChannel;
using Poco::AsyncChannel;
using Poco::Logger;
using Poco::FormattingChannel;
using Poco::PatternFormatter;

namespace Zebra {

class TimeUpdater : public Poco::Runnable
{
public:
    TimeUpdater(system_area_t& ptr, uint32_t period)
        : _system_area(ptr), _period(period) {}

    void run()
    {
        while (1) {
            updateTime();
            uptime();

            if (_system_area.set_time_flag != 0) {
                setTime();
                _system_area.set_time_flag = 0;
            }

            usleep(_period);
        }
    }

private:
    void updateTime()
    {
        DateTime date;
        _system_area.run_year = date.year();
        _system_area.run_mon  = date.month();
        _system_area.run_day  = date.day();
        _system_area.run_hour = date.hour();
        _system_area.run_min  = date.minute();
        _system_area.run_sec  = date.second();
        _system_area.run_ms   = date.millisecond();
    }

    void setTime()
    {
        struct tm tm_time;
        tm_time.tm_year = _system_area.set_year;
        tm_time.tm_mon  = _system_area.set_mon;
        tm_time.tm_mday = _system_area.set_day;
        tm_time.tm_hour = _system_area.set_hour;
        tm_time.tm_min  = _system_area.set_min;
        tm_time.tm_sec  = _system_area.set_sec;

        struct timeval time_value;
        time_value.tv_sec = mktime(&tm_time);
        settimeofday(&time_value, NULL);
    }

    void uptime()
    {
        struct sysinfo sys_info;
        sysinfo(&sys_info);
        _system_area.up_time = sys_info.uptime;
    }

private:
    system_area_t& _system_area;
    uint32_t _period;
};

void setRealTime()
{
    struct sched_param param;

    pid_t pid = getpid();

    sched_getparam(pid, &param);
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    sched_setscheduler(pid, SCHED_RR, &param);
}

unsigned char *fnShareMemory()
{	key_t key;
	int shm_id;
	char pathname[255];

	strcpy(pathname,"/tmp") ;
	key = ftok(pathname,0x03);

	shm_id=shmget(key,65536,IPC_CREAT|IPC_EXCL|0600);
	if(shm_id==-1)
	{
		printf("shmget error");
		shm_id=shmget(key,0,0);
	}

    return (unsigned char*)shmat(shm_id,NULL,0);
}

extern "C" int main(int argc, const char *argv[])
{
    unsigned char *raw_shm_ptr = fnShareMemory();

    std::shared_ptr<share_memory_area_t> shm_ptr(new (raw_shm_ptr) share_memory_area_t);

    for (uint32_t i = 1; i < SHARE_MEMORY_IO_CONFIG_LEN; ++i) {
        shm_ptr->user.io_config[i - 1].channel_type = MODULE_NOT_PRESENT;
    }

    ModbusChannelManager modbus_mgr("/dev/ttyS4", 115200, "/dev/spidev32765.0");

#if 1
    shm_ptr->user.io_config[1].channel_type = MODULE_AI;
    for (int i = 0; i < 16; ++i) {
        shm_ptr->user.input_ai[i] = 0x4567 + i;
    }
#endif

#if 1
    shm_ptr->user.io_config[0].channel_type = MODULE_DO;
    shm_ptr->user.io_config[1].channel_type = MODULE_DO;
    shm_ptr->user.io_config[2].channel_type = MODULE_DO;
    shm_ptr->user.io_config[3].channel_type = MODULE_DO;
    shm_ptr->user.io_config[4].channel_type = MODULE_DO;

    for (int i = 0; i < 15; ++i) {
        shm_ptr->user.output_do[i] = 0xF0;
    }
#endif

    shm_ptr->user.io_config[5].channel_type = MODULE_DI;
#if 0
    for (int i = 0; i < 3; ++i) {
        shm_ptr->user.input_di[i] = i + 5;
    }
#endif


#if 0
    shm_ptr->user.io_config[3].channel_type = MODULE_AO;
    for (int i = 0; i < 16; ++i) {
        shm_ptr->user.output_ao[i] = i + 3;
    }
#endif

    modbus_mgr.init(shm_ptr.get());

    modbus_mgr.handshake();
    struct timespec req = {0, 2000000};

    while (1) {
        for (int i = 0; i < 15; ++i) {
            shm_ptr->user.output_do[i] = ~shm_ptr->user.output_do[i];
        }
        modbus_mgr.readAll();
        modbus_mgr.writeAll();
#if 1
        while(!modbus_mgr.data_ready.read()){
            nanosleep(&req, NULL);
        };
#endif

        modbus_mgr.spi_transfer();
        nanosleep(&req, NULL);
#if 0
        while(!modbus_mgr.ack_ready.read()) {
            nanosleep(&req, NULL);
        }
        modbus_mgr.spi_checksum();
        nanosleep(&req, NULL);
#endif
    }

    return 0;
}

}  //  namespace Zebra
