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

string ipToStr(const ip_struct_t& raw_ip)
{
    return lexical_cast<string>((uint16_t)raw_ip.ip_section_1) + "." +
           lexical_cast<string>((uint16_t)raw_ip.ip_section_2) + "." +
           lexical_cast<string>((uint16_t)raw_ip.ip_section_3) + "." +
           lexical_cast<string>((uint16_t)raw_ip.ip_section_4);
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

//        return;
    }

    cout << ip  << " " << mask << " " << gateway<< endl;
    
#if 0
    setIP("manual", ip, mask, gateway);

    vector<string> dns_vec{dns};
    setDNS(dns_vec);
#endif
}

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

void log_init()
{
    AutoPtr<FileChannel> pChannel(new FileChannel);
    pChannel->setProperty("path", "/home/consen/rtu.log");
    pChannel->setProperty("rotation", "2K");
    pChannel->setProperty("archive", "timestamp");

    AutoPtr<FormattingChannel> pFCFile(new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t")));
    pFCFile->setChannel(pChannel);
    AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pFCFile));
    Logger::root().setChannel(pAsync);
}

extern "C" int main(int argc, const char *argv[])
{
    unsigned char *raw_shm_ptr = fnShareMemory();
    //std::shared_ptr<share_memory_area_t> shm_ptr(new (raw_shm_ptr) share_memory_area_t);
    std::shared_ptr<share_memory_area_t> shm_ptr(new share_memory_area_t);

    log_init();

    ModbusChannelManager modbus_mgr("/dev/ttyS4", 115200, "/dev/spidev32765.0");

    shm_ptr->user.io_config[0].channel_type = MODULE_DI;
    for (int i = 0; i < 14; ++i) {
        shm_ptr->user.input_di[i] = i + 5;
    }

    shm_ptr->user.io_config[1].channel_type = MODULE_AI;
    for (int i = 0; i < 16; ++i) {
        shm_ptr->user.input_ai[i] = 0x4567 + i;
    }

    shm_ptr->user.io_config[2].channel_type = MODULE_DO;
    for (int i = 0; i < 3; ++i) {
        shm_ptr->user.output_do[i] = i + 3;
    }

    shm_ptr->user.io_config[3].channel_type = MODULE_AO;
    for (int i = 0; i < 16; ++i) {
        shm_ptr->user.output_ao[i] = i + 3;
    }

    modbus_mgr.init(shm_ptr.get());

    modbus_mgr.handshake();

    while (1) {
        modbus_mgr.readAll();
        modbus_mgr.writeAll();
        while(!modbus_mgr.data_ready.read());

        modbus_mgr.spi_transfer();
        Poco::Thread::sleep(1000);
        while(!modbus_mgr.ack_ready.read());
        modbus_mgr.spi_checksum();
        Poco::Thread::sleep(5000);
    }

    return 0;
}

}  //  namespace Zebra
