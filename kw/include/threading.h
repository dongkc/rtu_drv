#ifndef __THREADING_H__
#define __THREADING_H__

#ifdef PLATFORM_BUILTIN_THREADING_SUPPORT
// non-preemptive threading based on setjmp/longjump functionality
#include "syncthread.h"
#else
// preemptive threading based on the underlying operating system
#include "osthread.h"
#endif

#include "mscorlib.h"

#if 0
class ECriticalSection : public ClrAllocator
{
    public :
    ECriticalSection();
    ~ECriticalSection();

    void Init();
    void Enter()
    {
        if (m_pMutexImpl != NULL)
        {
            m_pMutexImpl->Enter();   
        }
    }
    void Leave()
    {
        if (m_pMutexImpl != NULL)
        {
            m_pMutexImpl->Leave();   
        }
    }

    private :

    CMutexImpl* m_pMutexImpl;
    
};
#endif

// new thread interface
class ClrAppDomain;
class ClrController;
class EThread : public eListNode<EThread>, public ClrAllocator
{
    public :
    enum EThreadSpec
    {
        MaxSleepDelayMs   = 20  
    };
    inline void addRef()
    {
        m_refCnt++;
    }
    void release();

    // types used from the thread impl 
    typedef CThreadImpl::ThreadParam    ThreadParam;
    typedef CThreadImpl::ThreadId       ThreadId;
    typedef CThreadImpl::ThreadProc     ThreadProcImpl;
    typedef uint32                      ThreadHandle;
    typedef void(*ThreadProc)(ThreadParam par1, ThreadParam par2);

    static EThread* create(const char* name, ThreadProc proc, unsigned stackSize, ClrAppDomain* pAppDomain = NULL);

    void            Start(ThreadParam p1 = NULL, ThreadParam p2 = NULL);
    void            Resume();
    void            Suspend();
    void            StopRequest();
    void            AbortRequest();
    void            Abort(int exitCode);
    void            SafePoint();
    void            Continue();
    static bool     Stop(ClrAppDomain* pAppDomain, int maxDelay);
    static void     Continue(ClrAppDomain* pAppDomain);
    static void     Restart(ClrAppDomain* pAppDomain);
    static void     Abort(ClrAppDomain* pAppDomain, int maxDelay);

    static void     Sleep(int ms);
    static EThread* GetCurrentThread();
    bool            SetPriority(int prioClass, int prioThread);
    void            EnterWaiting(EEvent* pWaitHandle);
    void            LeaveWaiting(EEvent* pWaitHandle);

    inline static ThreadId  GetCurrentThreadId()
    {
        return CThreadImpl::GetCurrentThreadId();  
    }
    inline mscorlib::ThreadState GetThreadState()
    {
        return (mscorlib::ThreadState)m_state;
    }

    inline ThreadId getThreadId()
    {
        return m_threadId;
    }
    inline bool isThread(ThreadId id)
    {
        return (m_threadId == id) ? true : false;
    }
    inline ThreadHandle getThreadHandle()
    {
        return m_threadHandle;
    }
    inline CThreadImpl* getThreadImpl()
    {
        return m_pThreadImpl;
    }
    inline ClrAppDomain* getAppDomain()
    {
        return m_pAppDomain;
    }
    inline const char* getName()
    {
        return m_name;  
    }
    inline void* getStackTop()
    {
        return m_pStack;
    }
    inline unsigned getStackSize()
    {
        return m_stackSize;
    }
    inline bool isAborted()
    {
        return (m_state & mscorlib::EnumThreadState::Aborted) ? true : false;
    }
    inline bool isAbortRequested()
    {
        return (m_state & mscorlib::EnumThreadState::AbortRequested) ? true : false;
    }
    inline bool isStopped()
    {
        return (m_state & mscorlib::EnumThreadState::Stopped) ? true : false;
    }
    inline bool isStopRequested()
    {
        return (m_state & mscorlib::EnumThreadState::StopRequested) ? true : false;
    }
    inline bool isSuspended()
    {
        return (m_state & mscorlib::EnumThreadState::Suspended) ? true : false;
    }
    inline bool isRunning()
    {
        // 
        int mask = mscorlib::EnumThreadState::Unstarted |
                   mscorlib::EnumThreadState::Suspended |
                   mscorlib::EnumThreadState::Stopped   |
                   mscorlib::EnumThreadState::Aborted   |
                   mscorlib::EnumThreadState::WaitSleepJoin;
        return (m_state & mask) ? false : true;
    }
    inline bool isUnstarted()
    {
        return (m_state & mscorlib::EnumThreadState::Unstarted) ? true : false;
    }
    typedef eList<EThread>          ThreadList;
    typedef ThreadList::iterator    ThreadListIter;
    static ThreadList               s_threadList;
    
    static void Ctors();

    private:

    EThread();
    ~EThread();
    inline void setStopRequest()
    {
        m_state |= mscorlib::EnumThreadState::StopRequested;
    }
    inline void resetStopRequest()
    {
        m_state &= ~mscorlib::EnumThreadState::StopRequested;
    }
    inline void setAbortRequest()
    {
        m_state |= mscorlib::EnumThreadState::AbortRequested;
    }
    inline void setStopped()
    {
        m_state |= mscorlib::EnumThreadState::Stopped;
    }
    inline void resetStopped()
    {
        m_state &= ~mscorlib::EnumThreadState::Stopped;
    }
    inline void setWaitSleepJoin()
    {
        m_state |= mscorlib::EnumThreadState::WaitSleepJoin;
    }
    inline void resetWaitSleepJoin()
    {
        m_state &= ~mscorlib::EnumThreadState::WaitSleepJoin;
    }
    inline void setRestartRequest()
    {
        m_restartRequest = true;
    }

    bool createInternal(const char* name, ThreadProc proc, unsigned stackSize);
    void abortInternal();

    static int static_threadProc(EThread* pThread);
    void threadProc();

    CThreadImpl*            m_pThreadImpl;

    ThreadProc              m_proc;
    ThreadId                m_threadId;
    ThreadHandle            m_threadHandle;
    ClrAppDomain*           m_pAppDomain;
    EEvent*                 m_pWaitHandle;
    int                     m_state;
    int                     m_abortCode;
    ThreadParam             m_par1;
    ThreadParam             m_par2;

    jmp_buf                 m_abortJump; 
    unsigned                m_refCnt;
    eString<32>             m_name;
    void*                   m_pStack;
    unsigned                m_stackSize;
    bool                    m_restartRequest;

};

class SThread : public eListNode<SThread>, public ClrAllocator
{
    public :

    SThread();
    virtual ~SThread();

    // virtual adapt
    virtual int Process();

    static void Idle();

    typedef eList<SThread>          ThreadList;
    typedef ThreadList::iterator    ThreadListIter;

    
    static void Ctors();

    protected:

    private:

    static ThreadList               s_threadList;
};

class EEvent : public eListNode<EEvent>, public ClrAllocator
{
    public :
    EEvent(bool state, bool isAutoReset, ClrAppDomain* pAppDomain);
    inline void AddRef()
    {
        m_refCnt++;
    }
    void Release();

    bool Set();
    bool Reset();
    bool WaitOne();


    typedef eList<EEvent>       EventList;
    typedef EventList::iterator EventListIter;
    static EventList  s_eventList;

    inline ClrAppDomain* getAppDomain()
    {
        return m_pAppDomain;
    }

    static void SetAll(ClrAppDomain* pAppDomain);

    private :
    ~EEvent();
    CEventImpl*     m_pEventImpl;
    ClrAppDomain*   m_pAppDomain;
    unsigned        m_refCnt;
    unsigned        m_nWaitCnt;
    
};



class EMonitor : public eListNode<EMonitor>, public ClrAllocator
{
    public:

    static EMonitor* Create(ClrAppDomain* pAppDomain, Object* pObj = NULL);
    inline void addRef()
    {
        m_refCnt++;
    }
    void release();
    
    void Enter();
    void Leave();

    inline Object* getId()
    {
        return m_pId;
    }
    inline void setId(Object* p)
    {
        m_pId = p;
    }
    inline ClrAppDomain* getAppDomain()
    {
        return m_pAppDomain;
    }

	static bool Enter(Object* p);
	static bool Leave(Object* p);
    static void ReleaseAll(ClrAppDomain* pAppDomain);

    typedef eList<EMonitor>         MonitorList;
    typedef MonitorList::iterator   MonitorListIter;
    static MonitorList              s_monitorList;

    private:

    EMonitor();
    ~EMonitor();

    CMutexImpl*             m_pMutexImpl;
    Object*                 m_pId;
    ClrAppDomain*			m_pAppDomain;
    unsigned                m_refCnt;
    int                     m_enterCnt;
};



#endif //#define __THREADING_H__
