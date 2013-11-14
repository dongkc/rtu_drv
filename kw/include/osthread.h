#ifndef __THREADIMPL_H__
#define __THREADIMPL_H__

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Linux Header */
#include <unistd.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>

#include <sys/sysinfo.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <termios.h>
#include <assert.h>
#include <errno.h>
}

/// <summary>
/// \class CThreadImpl
/// CThreadImpl defines the thread interface which is used by eCLR for thread creation and interthread communication.
/// This class definition contains parts which must not be modified!
/// \attention The code section between "// @Begin ..." and "// @End ..." must not be modified.
/// </summary>
class CThreadImpl
{
    // @Begin interface code which must not be modified!
    public :

    /// <summary>
    /// Type of the argument which is passed via the body function 
    /// </summary>
    typedef void* ThreadParam;
    /// <summary>
    /// Pointer to the thread body function.
    /// </summary>
    typedef int(*ThreadProc)(ThreadParam param);
    /// <summary>
    /// Type of the thread id which is returned via GetThreadID()
    /// </summary>
    typedef void* ThreadId;

    ///
    /// Create a thread in suspended mode
    ///
    /// \param name Pointer to the name of the thread to create
    /// \param proc Pointer to the thread body function
    /// \param param Pointer to parametes for the thread body function
    /// \param prio Priority of the thread to create 
    /// \param stackSize Size of the stack which will be associated to the thread which will be created
    /// \return true if successfull or false if an error occured
    ///
    static CThreadImpl* Create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize);
    
    /// <summary>
    /// public destructor
    /// </summary>
    void            Release();

    /// <summary>
    ///  Causes the operating system to change the state of the current instance to ThreadState.Running.
    /// </summary>
    void            Start();
    /// <summary>
    /// Resumes a thread that has been suspended.
    /// </summary>
    void            Resume();
    /// <summary>
    /// Either suspends the thread, or if the thread is already suspended, has no effect.
    /// </summary>
    void            Suspend();
    /// <summary>
    /// Suspends the current thread for a specified time.
    /// </summary>
    /// <param name="ms">The number of milliseconds for which the thread is blocked. Specify zero (0) to indicate that this thread should be suspended to allow other waiting threads to execute.</param>
    static void     Sleep(int ms);
    /// <summary>
    /// Gets the current thread identifier.
    /// </summary>
    /// <returns>A ThreadId type that is the identifier of the current thread.</returns>
    static ThreadId GetCurrentThreadId();
    /// <summary>
    /// Gets the thread identifier.
    /// </summary>
    /// <returns>A ThreadId type that is the identifier of the thread.</returns>
    ThreadId GetThreadId();
    /// <summary>
    /// Sets the priority of a thread.
    /// </summary>
    /// <param name="threadPriority">Priority, which will be associated to the thread.</param>
    bool SetPriority(int prioThread);
    /// <summary>
    /// Terminates the thread.
    /// </summary>
    bool Abort();

    private:
    /// <summary>
    /// Private Constructor. Forbides the instantiation without using the Create(...) method
    /// </summary>
    CThreadImpl();
    /// <summary>
    /// private Destructor. Allows usage of private allocator
    /// </summary>
    ~CThreadImpl();
    bool  create(const char* name,ThreadProc proc, ThreadParam param, unsigned stackSize);
    int*  pPriority;
    // @End interface code which must not be modified!
    // add private implemenation here !
    
    public:
    //char             m_name[32];
    pthread_t        m_threadId;
    ThreadProc       m_proc;
    ThreadParam      m_param;
    sem_t            m_threadSem;
    
    struct sigaction sigusr1, sigusr2;
    
};

/// <summary>
/// Notifies a waiting thread that an event has occurred. This class cannot be inherited.
/// </summary>
class CEventImpl : public ClrAllocator
{
    // @Begin interface code which must not be modified!
    public :
    /// <summary>
    /// Initializes a new instance of the Event class with a Boolean value indicating whether to set the initial state to signaled.
    /// <param name="initialstate">true to set the initial state to signaled; false to set the initial state to nonsignaled.</param>
    /// </summary>
    static CEventImpl* Create(bool initialstate, bool isAutoReset);
    void Release();

    /// <summary>
    /// Sets the state of the specified event to signaled.
    /// </summary>
    /// <returns>true if the function succeeds; otherwise, false.</returns>
    bool Set();
    /// <summary>
    /// Sets the state of the specified event to nonsignaled.
    /// </summary>
    /// <returns>true if the function succeeds; otherwise, false.</returns>
    bool Reset();
    /// <summary>
    /// Blocks the current thread until the current WaitHandle receives a signal.
    /// </summary>
    /// <returns>true if the current instance receives a signal; otherwise, false</returns>
    bool WaitOne();

    private:
    /// <summary>
    /// Private Constructor, allows the usage of an private allocator
    /// </summary>
    CEventImpl();
    /// <summary>
    /// Private Destructor, allows the usage of an private allocator
    /// </summary>
    ~CEventImpl();
    bool create(bool initialstate, bool isAutoReset);
    sem_t m_EventSemID;
    bool m_signaled;
    // @End interface code which must not be modified!
    // add private implemenation here !
};

// dont derive from global allocator ClrAllocator !!!
class CMutexImpl
{
    // @Begin interface code which must not be modified!
    public :
    static CMutexImpl* Create();
    void Release();

    void Enter();
    void Leave();

    private :
    CMutexImpl();
    ~CMutexImpl();
    
    pthread_mutex_t m_tMutexID;
    bool create();
    // @End interface code which must not be modified!
    // add private implemenation here !
};

#endif //#define __THREADIMPL_H__
