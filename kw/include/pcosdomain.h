#ifndef __PCOSDOMAIN_H__
#define __PCOSDOMAIN_H__
// pcoslib.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "controller.h"
#include "appdomain.h"
#include "pcoslib.h"
#include "pcos.h"

enum PlcStartMode
{
    // take the same like pcoslib.cs !!!
    PlcStartCold        = 0,
    PlcStartWarm        = 1,
    PlcStartHot         = 2
};

///
/// \class PcosCallback
///
class PcosCallback
{
public:
    virtual ~PcosCallback();

    /// <summary>
    /// Callback which is called when the PLC application is unloaded
    /// </summary>
    virtual void PlcOn() = 0;
    /// <summary>
    /// Callback which is called before a PLC application is loaded
    /// </summary>
    virtual bool PlcLoading() = 0;
    /// <summary>
    /// Callback which is called befor a PLC application is started
    /// </summary>
    virtual bool PlcStarting(enum PlcStartMode mode, bool& retry) = 0;
    /// <summary>
    /// Callback which is called after a PLC application is started
    /// </summary>
    virtual void PlcRunning() = 0;
    /// <summary>
    /// Callback which is called when the PLC application is halted (breakpoint reached)
    /// </summary>
    virtual void PlcHalt() = 0;
    /// <summary>
    /// Callback which is called when the PLC application will be stopped
    /// </summary>
    virtual bool PlcStopping(bool& retry) = 0;
    /// <summary>
    /// Callback which is called when the PLC application is stopped
    /// </summary>
    virtual void PlcStop() = 0;
    /// <summary>
    /// Callback which is called when the PLC application whill be unloaded
    /// </summary>
    virtual bool PlcResetting() = 0;
    /// <summary>
    /// Callback which is called when the PLC application throw an exception
    /// </summary>
    virtual void PlcException(int code) = 0;
};

class PcosTaskCallback : public eListNode<PcosTaskCallback>, public ClrAllocator
{
public:
    PcosTaskCallback();
    virtual ~PcosTaskCallback();

    /// <summary>
    /// Callback which is called when the PLC application task process is started
    /// </summary>
    virtual void ProcessBegin() = 0;
    /// <summary>
    /// Callback which is called before a PLC application task process is ended
    /// </summary>
    virtual void ProcessEnd() = 0;
    
    CPcosDomain* m_pPcosDomain;
private:
    EThread::ThreadId m_threadId;
};

// pcos domain default callback
class PcosCB : public PcosCallback
{
public:
    PcosCB();
    void PlcOn();
    bool PlcLoading();
    bool PlcStarting(enum PlcStartMode mode, bool& retry);
    void PlcRunning();
    void PlcHalt();
    bool PlcStopping(bool& retry);
    void PlcStop();
    bool PlcResetting();
    void PlcException(int code);
private:
    unsigned m_reentrantCounter;

};

class PcosManagedCallbacks
{
public:
    PcosManagedCallbacks();
    void Init();
    void SetManagedCallbacks(ClrAppDomain* pAppDomain);

    // managed callbacks
    void (*m_pSchedule)();
    void (*m_pStop)();
    void (*m_pHalt)();
    void (*m_pRun)();
    void (*m_pRiseTaskEvent)(int sysEventNo);
    void (*m_pInvokeSysEvent)(int sysEventNo);
    EclrVersion m_pcoslibVersion;
    // dummy functions (do nothing) to avoid null reference
    static void dummyCB();
    static void dummyCB1(int p0);
};

class PcosTaskCallbackList : public eListNode<PcosTaskCallbackList>, public ClrAllocator
{
public:
    PcosTaskCallbackList(uint16* pTaskName, int nameLength, EThread::ThreadId threadId);
    ~PcosTaskCallbackList();


    inline void Enter()
    {
#ifdef PLATFORM_THREADING_SUPPORT
        m_pCritSect->Enter();
#endif
    }
    inline void Leave()
    {
#ifdef PLATFORM_THREADING_SUPPORT
        m_pCritSect->Leave();
#endif
    }

    inline bool IsOk()
    {
#ifdef PLATFORM_THREADING_SUPPORT
        return (m_pCritSect != NULL) ? true : false;        
#else
        return true;
#endif
    }

    EThread::ThreadId   m_threadId;
    EThread::ThreadId   m_threadId2;
    uint16              m_taskName[16];
    int                 m_nameLength;

//private:
    typedef eList<PcosTaskCallback>     TaskCBList;
    typedef TaskCBList::iterator        TaskCBListIter;

    TaskCBList          m_taskCBList;
#ifdef PLATFORM_THREADING_SUPPORT
    CMutexImpl*         m_pCritSect;        
#endif

};

class PcosTaskCallbackManager : public ClrAllocator
{
public:
    PcosTaskCallbackManager(CPcosDomain* pPcosDomain);
    ~PcosTaskCallbackManager();
    void ProcessBegin();
    void ProcessEnd();
    bool Register(uint16* pTaskName, int nameLength, PcosTaskCallback& taskCB);
    bool Unregister(PcosTaskCallback& taskCB);
    // task ids reassignment (download change support)
    void ReassignThreads();
    void ReassignThreadsBackground();
    void ReassignThreads2();
private:
    PcosTaskCallbackList* getTaskCallbackList(EThread::ThreadId  threadId);

    typedef eList<PcosTaskCallbackList> TaskCBListList;
    typedef TaskCBListList::iterator    TaskCBListListIter;
    TaskCBListList  m_taskCBList;
    CPcosDomain*    m_pPcosDomain;
};

class CPcosDomain : public ClrAppDomainEvents, public ClrAllocator
{
public:
    enum PlcState
    {
        PlcOn               = 0,
        PlcLoading          = 1,
        PlcStarting         = 2,
        PlcRunning          = 3,
        PlcHaltRequested    = 4,
        PlcHalt             = 5,
        PlcStopping         = 6,
        PlcStop             = 7,
        PlcResetting        = 8
    };
    enum PlcError
    {
        PlcNonError           = 0,
        PlcLoadError          = 1,
        PlcStartError         = 2,
        PlcDCRealtimeError    = 3,
        PlcDCPrologError      = 4,
        PlcDCForceListError   = 5,
        PlcDCOutOfMemoryError = 6,
        PlcDCInternalError    = 7
    };  
    enum PlcException
    {
        PlcExcNon                       = 0,
        PlcExcString                    = 256,
        PlcExcWatchDogExceeded          = 257,
        PlcExcMaximumCpuLoadExceeded    = 258,
        PlcExcSystem                    = 259,
        PlcExcEnd                       = 300
    };

    enum PLcSystemEvents
    {
        EvColdStart                 = 0,
        EvWarmStart                 = 1,
        EvHotStart                  = 2,
        EvStop                      = 3,
        EvWatchDogExceeded          = 4,
        EvMaximumCpuLoadExceeded    = 5,
        EvExcDiv0                   = 6,
        EvExcStack                  = 7,
        EvExcBound                  = 8,
        EvExcString                 = 9,
        EvExcSystem                 = 10,
        EvRetainMasterInit          = 11,
        EvClrOut                    = 12, // 13 ... 31 is reserved for future use
        EvOEM32 = 32, EvOEM33 = 33, EvOEM34 = 34, EvOEM35 = 35, EvOEM36 = 36, EvOEM37 = 37, EvOEM38 = 38, EvOEM39 = 39,
        EvOEM40 = 40, EvOEM41 = 41, EvOEM42 = 42, EvOEM43 = 43, EvOEM44 = 44, EvOEM45 = 45, EvOEM46 = 46, EvOEM47 = 47,
        EvOEM48 = 48, EvOEM49 = 49, EvOEM50 = 50, EvOEM51 = 51, EvOEM52 = 52, EvOEM53 = 53, EvOEM54 = 54, EvOEM55 = 55,
        EvOEM56 = 56, EvOEM57 = 57, EvOEM58 = 58, EvOEM59 = 59, EvOEM61 = 61, EvOEM62 = 62, EvOEM63 = 63, EvOEM64 = 64,
        MaxSystemEvents             = 65
    };

    enum PLcTaskEvents
    {
        TaskEventProcessEnd,
        TaskEvent
    };


    CPcosDomain(ClrAppDomain* pAppDomain);
    virtual ~CPcosDomain();

    // application event callback implementation
    void AppLoad();
    void AppUnload();
    void AppException(int code);
    void AppSchedule();
    void AppStart();
    void AppStop();
    void AppContinue();
    void AppTerminate();
    void AppSwap(ClrAppDomain* pNewActiveDomain);
    void AppBpReached(ClrAppBreakpoint* pBp);

    // state machine interface
    /// <summary>
    /// Get the current PLC state
    /// \see PlcState
    /// </summary>
    /// \return the current PLC state is returned \see PlcState
    inline enum PlcState getState()
    {
        return (enum PlcState)m_state;
    }
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if state is running and "false" in all other cases.
    inline bool isRunning()
    {
        return (m_state == PlcRunning) ? true : false;
    }

    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool ColdStartBootReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool WarmStartBootReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool StopBootReq();

    ClrDownloadStream* DownloadReq(ClrAppDomain::DownloadMode mode);
    bool DownloadReqEnd();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool ResetReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool ColdStartReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool WarmStartReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool HotStartReq();
    /// <summary>
    /// 
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool StopReq();

    /// <summary>
    /// continues the application after a breakpoint has been reached
    /// </summary>
    /// \return "true" if success else "false" is returned.
    bool ContinueReq();

    // callback called from managed code
    void StartReq();
    void AbortReq(unsigned evNo, unsigned excNo, void* pRetAddr, void* pMethFrame);
    void RestartReq(unsigned mode);

    inline bool isHotStartPossible()
    {
        bool result = (m_state == PlcStop) ? m_hotStartPossible : false;
        return result;
    }

    inline void SetMaximumNumberOfTasks(int maxTasks)
    {
        pcoslib::M1_2::zSysMem.maxTask = (Byte)maxTasks;
    }
    inline void SetMaximumNumberOfErrLogs(int maxErrLogs)
    {
        pcoslib::M1_2::zSysMem.MaxErr = (UInt32)maxErrLogs;
        eclrlib::ErrorLog::SetMaximumErrorListSize(maxErrLogs);
    }

    void SetTicksPerSecond(int ticksPerSecond);
    
    inline void AnnounceRetentiveMemory(void* pMemory, unsigned nSize)
    {
        pcoslib::Retain::AnnounceMemory(pMemory, nSize);
    }
    inline void SetScheduleCorrection(unsigned correction_us)
    {
        pcoslib::M1_2::zSysMem.SchedCorrection = (uint16)correction_us;
    }
    ClrAppDomain* getAppDomain()
    {
        return m_pAppDomain;
    }

    inline void setCallback(PcosCallback* pCallback)
    {
        m_pUnmanagedCB = pCallback;
    }

    void ExecuteForcing();
    void ExecuteTaskBeginCB();
    void ExecuteTaskEndCB();

    inline PlcError getLastError()
    {
        return m_error;
    }

    void RiseTaskEvent(unsigned evNo);
    bool TaskEventAvailable(unsigned evNo, ClrAppDomain* pAppDomain = NULL);
    void InvokeSystemEvent(unsigned evNo, unsigned stackSize);
    void RetainMasterInit();
    bool RegisterUnmanagedCallback(enum PLcTaskEvents, int param, CUnmanagedCallback& cbObject, ClrAppDomain* pAppDomain = NULL);
    EThread::ThreadId GetTaskThreadId(uint16* pName, int nameLength);
    EThread::ThreadId GetTaskThreadIdBackground(uint16* pName, int nameLength);
    bool RegisterTaskCallback(uint16* pTaskName, int nameLength, PcosTaskCallback& taskCB);
    bool UnregisterTaskCallback(PcosTaskCallback& taskCB);
    void SetCPUCapacity(int calcFreq_ms, int minCPULoadPerCent, int maxCPULoadPerCent, int minFreeSlice_ms);
    bool IsWarmStartPossible();
    bool LoadedVersion(EclrVersion& pcoslibVersion);
    void SetHaltOnStringError(bool set);
    bool IsHaltOnStringError();
    void AllowRealtimeViolationDuringOnlineChange()
    {
        m_dlcAllowRealtimeViolation = true;
    }
    bool IsRealtimeViolationDuringOnlineChange()
    {
        return m_dlcAllowRealtimeViolation;
    }

    // force service implementation
    ForceListElement* GetForceElement(unsigned idx);
    bool AddForceItem(const char* pVarName, const char* pValue);
    bool RemoveForceItem(const char* pItemName);
    void ClearForceList();
    unsigned GetForceCount();
    unsigned GetForceListId()
    {
        return m_forceListId;
    }

    bool IsDebug();

    void Process();

private:
    void init();
    bool bootRequest(enum PlcStartMode mode, bool run);
    bool startRequest(enum PlcStartMode mode);
    bool starting(enum PlcStartMode mode, bool run);
    bool starting2(unsigned mode);
    bool stopping(int sysEventNo);
    bool stopping2();
    bool wait4Starting();

    void setRetainChecksum();
    bool invokeManagedCallback(int evNo);
    static Object* getTaskEvent(unsigned evNo, ClrAppDomain* pAppDomain);

    bool isDownloadChangeSupported();
    bool downloadChange();
    bool downloadChangeProlog(ClrAppDomain* pBackgroundDomain);
    bool downloadChangeEpilog();
    static EThread::ThreadId getTaskThreadId(ClrAppDomain* pAppDomain, uint16* pName, int nameLength);

    bool isHotRestartRequested()
    {
        return (m_restartRequest == true && m_restartMode == PlcStartHot) ? true : false;
    }
    void resetRestartRequest()
    {
        m_restartRequest = false;
    }
    ClrAppDomain*   m_pAppDomain;
    enum PlcState   m_state;
    enum PlcError   m_error;

    unsigned        m_restartMode;
    unsigned        m_startingMode;
    bool            m_hotStartPossible;
    bool            m_isPcosApplication;
    bool            m_restartRequest;
    bool            m_startingRequest;
    bool            m_stoppingRequest;
    bool            m_continueRequest;
    bool            m_dlcAllowRealtimeViolation;

    PcosManagedCallbacks m_managedCB;
#ifdef PLATFORM_DOWNLOADCHANGE_SUPPORT
    PcosManagedCallbacks m_managedCB2;
#endif

    PcosCallback*   m_pUnmanagedCB;
    PcosCB          m_pcosCB;

    // force service implementation
    typedef eList<ForceListElement>     ForceList;
    typedef ForceList::iterator         ForceListIter;
    ForceListElement* getForceElement(const char* pItemName);
    // support download change
    bool dublicateForcelist(ClrAppDomain* pBackgroundDomain);
    void cleanupForcelist(ClrAppDomain *pAppDomain);

    ForceList   m_forceList;
    unsigned    m_forceListId;

    PcosTaskCallbackManager* m_pTaskCallbackManager;
};

#endif //#ifndef __PCOSDOMAIN_H__

