#include "eclr.h"
#include "controller.h"
#include "pcosdomain.h"
#include "bsdsocket.h"
#include "remoting.h"
#include "log.h"
#include "eclrImageFile.h"
#include "eclrEnvironment.h"
#include "eclr.h"

#include "IO_DRIVER_LIB.h"

#define STACK_BASE_SYS      PTHREAD_STACK_MIN + 0x8000

// for thread with FIFO scheduling policy, priority ranges from 1 to 99 (low to high)
#define SCHEDULER_PRIO 80
#define HIGH_PRIO      70
#define NORM_HIGH_PRIO 69
#define NORM_LOW_PRIO  51
#define LOWER_PRIO     50


static PcosImageFile appImgFile;

static char loc_code_heap[2000000];
static char loc_data_heap[200000+12];
static char loc_object_heap[200000];

#ifdef PLATFORM_THREADING_SUPPORT
static void setPriorityScheme()
{
     // only one prio level available
     mscorlib::PriorityClass::SetLowest(LOWER_PRIO, LOWER_PRIO);
     // 19 level avalable
     mscorlib::PriorityClass::SetNormal(NORM_LOW_PRIO, NORM_HIGH_PRIO);
     // only one prio level avalable
     mscorlib::PriorityClass::SetHighest(HIGH_PRIO, HIGH_PRIO);
} 
#endif  //PLATFORM_THREADING_SUPPORT
 void check_timer_overflow();
extern "C" int main(int argc, char* argv[] )
{
    argc = argc;
    argv = argv;

    InitLogging(LF_ALWAYS, LL_INFO1);
    LogFacilityOff(LF_THREADING);
    printf("KW-Software embedded CLR Version %s for Linux.ARM\n", ECLR_VERSION_STRING);

#ifdef PLATFORM_THREADING_SUPPORT
	 setPriorityScheme();        
#endif	 

    if (ClrController::Create("Default", loc_object_heap, sizeof(loc_object_heap)) == NULL)
    {
        printf("setup controller failed\n");
        return -1;
    }

    CDeviceInfo* pDeviceInfo = ClrController::getDeviceInfo();
    pDeviceInfo->setAttribute(CDeviceInfo::Manufacturer, "KW");
    pDeviceInfo->setAttribute(CDeviceInfo::ProductName, "eCLR2.2 for Linux-ARM");
    pDeviceInfo->setAttribute(CDeviceInfo::HardwareVersion, "1.0");
    pDeviceInfo->setAttribute(CDeviceInfo::FirmwareVersion, "1.0");
    pDeviceInfo->setAttribute(CDeviceInfo::MaxApplRetainDataSize, (uint32)0);
    pDeviceInfo->setAttribute(CDeviceInfo::TimerResolution, 1000);
    pDeviceInfo->setAttribute(CDeviceInfo::ScheduleInterval, 1000);

    //ClrAppDomain* pAppDomain = ClrController::CreateDomain("Default", ClrAppDomain::CreationDefault);
    ClrAppDomain* pAppDomain = ClrController::CreateDomain("Default", ClrAppDomain::DownloadChangeSupport);
    if (pAppDomain == NULL)
    {
        printf("vorher setup default domain failed\n");
        return -1;
    }
    pAppDomain->SetDataMemory(loc_data_heap, sizeof(loc_data_heap));
    pAppDomain->SetCodeMemory(loc_code_heap, sizeof(loc_code_heap));
    pAppDomain->SetImageStream(&appImgFile);
  
    CPcosDomain* pPcosDomain = pcoslib::CreateDomain(pAppDomain);
    if (pPcosDomain == NULL)
    {
        printf("setup default domain failed\n");
        return -1;
    }

#ifdef PLATFORM_THREADING_SUPPORT
	 pPcosDomain->SetTicksPerSecond(0); // scheduler is working with time base of EclrEnvironment        
#endif	
	
    pPcosDomain->SetCPUCapacity(1000,   // int calcFreq_ms
                                30,     // int minCPULoadPerCent
                                80,     // int maxCPULoadPerCent
                                4 );    // int minFreeTicks
					
		
	IO_DRIVER_LIB::init();
	IO_DRIVER_LIB::loadLibrary();			
    // pPcosDomain->WarmStartBootReq();
    pPcosDomain->ColdStartBootReq();
    // pPcosDomain->AnnounceRetentiveMemory(g_pCshmInterface->m_pMapAddr, 2048);
   
    BsdSocket* pBsdSocket = new BsdSocket();
    if (pBsdSocket != NULL)
    {
        if (pBsdSocket->Open(BsdSocket::Stream, CRemotingDeamon::ClrServerPort) == true)
        {
            CRemotingDeamon::addSocket(pBsdSocket);
        }
        pBsdSocket->release();  
    }
    // start remoting
    CRemotingDeamon::start();
 

 	// change the scheduling policy and priority of this thread
   struct sched_param zSchedParam;
   int policy;
   int nResult;
   nResult = pthread_getschedparam(pthread_self(), &policy, &zSchedParam);
   assert(nResult == 0);
   zSchedParam.sched_priority = SCHEDULER_PRIO;
   nResult = pthread_setschedparam(pthread_self(), SCHED_FIFO, &zSchedParam);
   assert(nResult == 0);
   
    while (true)
    {
        ClrController::Process();
	usleep(1 * 1000);
	
	check_timer_overflow();
    }
    
   return 0;
}



#undef CLEAR_INSN_CACHE
#define CLEAR_INSN_CACHE(BEG, END)					\
{									\
  register unsigned long _beg __asm ("a1") = (unsigned long) (BEG);	\
  register unsigned long _end __asm ("a2") = (unsigned long) (END);	\
  register unsigned long _flg __asm ("a3") = 0;				\
  register unsigned long _scno __asm ("r7") = 0xf0002;			\
  __asm __volatile ("swi 0x9f0002		@ sys_cacheflush"	\
		    : "=r" (_beg)					\
		    : "0" (_beg), "r" (_end), "r" (_flg), "r" (_scno));	\
}

void EclrEnvironment::FlushCache( void* pData, unsigned int len )
{
	unsigned long start = (unsigned long)loc_data_heap;
	unsigned long end = start + (unsigned long)sizeof(loc_data_heap);
	CLEAR_INSN_CACHE(start, end); 

	start = (unsigned long)loc_code_heap;
	end = start + (unsigned long)sizeof(loc_code_heap);
	CLEAR_INSN_CACHE(start, end);
	

 return;
  
}






#define TIMEOUT_HOUR 2
int timer_1ms = 0;
int timer_1s = 0;
int timer_1m = 0;
int timer_1h = 0;
void timer_clear()
{
 timer_1ms = 0;
 timer_1s = 0;
 timer_1m = 0;
 timer_1h = 0;
}
bool timer_inc()
{
	timer_1ms++;
	if(timer_1ms >= 1000)
	{
		timer_1s++;timer_1ms = 0;
		if(timer_1s >= 60)
		{
			timer_1m++;timer_1s = 0;
			if(timer_1m >= 60)
			{
				timer_1h++;timer_1m = 0;
				if(timer_1h >= TIMEOUT_HOUR)
					return true;
			}			
		}	
	}
	return false;
}

void check_timer_overflow()
{
	CPcosDomain* pPcosDomain = pcoslib::getCurrentDomain();
	if(NULL != pPcosDomain)
	{
		if(CPcosDomain::PlcRunning == pPcosDomain->getState())		 
		{
			
			if(timer_inc())
			{
				pPcosDomain ->StopReq();				
			} 
			//pPcosDomain ->StopReq();		
			//pPcosDomain ->ColdStartReq();
		}
		else
		{
			timer_clear();
		}
	}
	
}



