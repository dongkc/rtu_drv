#ifndef __MSCORLIB_H__
#define __MSCORLIB_H__
// mscorlib.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

// common type system
#include "ctsbase.h"
/// \addtogroup Corelib Core Libraries
/// \{

class EWaitHandle;
class EEvent;
class EThread;
class ClrAppDomain;
class ClrMethod;

/// <summary>
/// public scope class for library implementation
/// use mscorlib::init() and mscorlib::loadLibrary() during startup 
/// to make the library accessible by the managed application.
/// <see cref="mscorlib::init()"/>
/// <see cref="mscorlib::loadLibrary()"/>
/// </summary>
class mscorlib
{
// @Begin automatically generated code, do not modify !
public:
enum LibVersion { Major=2, Minor=4, Build=0, Revision=0 };
static bool init();
static bool loadLibrary();
static bool isCompatible(const EclrVersion& requestedVersion);
static bool load(ClrAppDomain* pAppDomain);
static bool unload(ClrAppDomain* pAppDomain);
static unsigned s_modulHndl;
// @End automatically generated code

class EnumThreadState
{
// @Begin automatically generated code, do not modify native enums !
public:
enum ThreadState
{
    /// <summary>
    /// Data type size of each enumerator elemet in number of bytes
    /// </summary>
    ThreadState_sizeof=2,
    /// <summary>
    /// The thread has been started, it is not blocked, and there is no pending ThreadAbortException.
    /// </summary>
    Running=0,
    /// <summary>
    /// The thread is being requested to stop. This is for internal use only.
    /// </summary>
    StopRequested=1,
    /// <summary>
    /// The thread is being requested to suspend.
    /// </summary>
    SuspendRequested=2,
    /// <summary>
    /// The thread is being executed as a background thread, as opposed to a foreground thread. This state is controlled by setting the Thread.IsBackground property.
    /// </summary>
    Background=4,
    /// <summary>
    /// The Thread.Start method has not been invoked on the thread.<see cref="Thread.Start"/>
    /// </summary>
    Unstarted=8,
    /// <summary>
    /// The thread has stopped.
    /// </summary>
    Stopped=16,
    /// <summary>
    /// The thread is blocked as a result of a call to Wait, Sleep, or Join.
    /// </summary>
    WaitSleepJoin=32,
    /// <summary>
    /// The thread has been suspended.
    /// </summary>
    Suspended=64,
    /// <summary>
    /// The Thread.Abort method has been invoked on the thread, but the thread has not yet 
    /// received the pending System.Threading.ThreadAbortException that will attempt to terminate it.
    /// </summary>
    AbortRequested=128,
    /// <summary>
    /// The thread is in the Stopped state.
    /// </summary>
    Aborted=256
};
// @End automatically generated code
};
typedef EnumThreadState::ThreadState ThreadState;

class EnumThreadPriority
{
// @Begin automatically generated code, do not modify native enums !
public:
/// <summary>
/// Specifies the scheduling priority of a Thread.
/// </summary>
enum ThreadPriority
{
    // @Begin automatically generated code, do not modify native enums !
    /// <summary>
    /// Data type size of each enumerator elemet in number of bytes
    /// </summary>
    ThreadPriority_sizeof=2,
    /// <summary>
    /// The Thread can be scheduled after threads with any other priority
    /// </summary>
    Lowest=0,
    /// <summary>
    /// The Thread can be scheduled after threads with Normal priority and before those with Lowest priority.
    /// </summary>
    BelowNormal=1,
    /// <summary>
    /// The Thread can be scheduled after threads with AboveNormal priority and before those with 
    /// BelowNormal priority. Threads have Normal priority by default.
    /// </summary>
    Normal=2,
    /// <summary>
    /// The Thread can be scheduled after threads with Highest priority and before those with Normal priority.
    /// </summary>
    AboveNormal=3,
    /// <summary>
    /// The Thread can be scheduled before threads with any other priority.
    /// </summary>
    Highest=4
};
// @End automatically generated code
};
typedef EnumThreadPriority::ThreadPriority ThreadPriority;


/// <summary>
/// Specifies the scheduling priority class of a Thread.
/// </summary>
class PriorityClass
{
    public:
    /// <summary>
    /// This function sets the priority range for the Lowest priority class. 
    /// </summary>
    /// <param name="minPrio">minimal priority associated to this priority class</param>
    /// <param name="minPrio">maximal priority associated to this priority class</param>
    static void SetLowest(int minPrio, int maxPrio)
    {
        s_minLowest = minPrio;
        s_maxLowest = maxPrio;
    }
    /// <summary>
    /// This function sets the priority range for the Normal priority class. 
    /// </summary>
    /// <param name="minPrio">minimal priority associated to this priority class</param>
    /// <param name="minPrio">maximal priority associated to this priority class</param>
    static void SetNormal(int minPrio, int maxPrio);
    /// <summary>
    /// This function sets the priority range for the Highest priority class. 
    /// </summary>
    /// <param name="minPrio">minimal priority associated to this priority class</param>
    /// <param name="minPrio">maximal priority associated to this priority class</param>
    static void SetHighest(int minPrio, int maxPrio)
    {
        s_minHighest = minPrio;
        s_maxHighest = maxPrio;
    }

    /// <summary>
    /// Gets the priority range of the given priority class. 
    /// </summary>
    static void GetRange(int prioClass, int& minPrio, int& maxPrio);
    static unsigned GetPrioAmount(int prioClass);
    enum PriorityClassEnum
    {
        /// <summary>
        /// The Thread can be scheduled after threads with any other priority class
        /// Typically, background threads like Remoting runs at this class level
        /// </summary>
        Lowest  =0,
        /// <summary>
        /// Priority class of normal tasks. 
        /// Typically, all threads created by the .NET Threading interface run at this class level. 
        /// </summary>
        Normal  =1,
        /// <summary>
        /// Priority class of time-critical tasks that must be executed immediately. 
        /// The threads preempt all the threads of normal or lowest priority class threads. 
        /// Typically, the Schedule thread of the Eclr runs at this level
        /// </summary>
        Highest =2
    };

    private:
    static int s_minLowest;
    static int s_maxLowest;
    static int s_minNormal;
    static int s_maxNormal;
    static int s_minHighest;
    static int s_maxHighest;
};

/// <summary>
/// The Seek methods take an offset parameter that is relative to the 
/// position specified by SeekOrigin
/// </summary>
enum SeekOrigin
{
    // @Begin automatically generated code, do not modify native enums !
    /// <summary>
    /// Data type size of each enumerator elemet in number of bytes
    /// </summary>
    SeekOrigin_sizeof=4,
    /// <summary>
    /// Specifies the beginning of a stream.
    /// </summary>
    Begin=0,
    /// <summary>
    /// Specifies the current position within a stream.
    /// </summary>
    Current=1,
    /// <summary>
    /// Specifies the end of a stream.
    /// </summary>
    End=2
    // @End automatically generated code
};

/// <summary>
/// Specifies how the operating system should open a file.
/// </summary>
enum FileMode
{
    // @Begin automatically generated code, do not modify native enums !
    /// <summary>
    /// Data type size of each enumerator elemet in number of bytes
    /// </summary>
    FileMode_sizeof=4,
    /// <summary>
    /// Specifies that the operating system should create a new file. 
    /// If the file already exists, it will be overwritten. 
    /// </summary>
    Create=0,
    /// <summary>
    /// Specifies that the operating system should open an existing file.
    /// </summary>
    Open=1
    // @End automatically generated code
};

/// <summary>
/// Base type Enum
/// </summary>
struct Enum
{
    // @Begin automatically generated code, do not modify native structs !
    Enum();
    void ctor();
    // @End automatically generated code
};

/// <summary>
/// Class MulticastDelegate
/// </summary>
class MulticastDelegate : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// <summary>
    /// Initializes a new instance of the MulticastDelegate class.
    /// </summary>
    MulticastDelegate(Object* p0, IntPtr p1);

    /// \cond EXTERNAL_DOCUMENTATION_INCLUDE
    void ctor(Object* p0, IntPtr p1);
    Object* _o;
    IntPtr  _m;
    // @End automatically generated code
    // insert additional private methods and member here ! 
    MulticastDelegate() {}
    static void (*s_ctor)();
    /// \endcond

    void Invoke();
    static void Invoke(Object* pObject, ClrMethod* pMethod, Object* pParam);
    private:
    void Invoke(Object* pParam);
};

/// <summary>
/// Represents the standard input, output, and error streams for console applications.
/// </summary>
class Console : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// <summary>
    /// Writes the current line terminator to the standard output stream.
    /// </summary>
    static void WriteLine(String* p0, Object* p1);
    /// <summary>
    /// Writes the current line terminator to the standard output stream.
    /// </summary>
    static void WriteLine(String* p0);
    /// <summary>
    /// Initializes a new instance of the Console class.
    /// </summary>
    Console();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

/// <summary>
/// Provides information about, and means to manipulate, the current environment and platform.
/// </summary>
class Environment : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Int32 get_TickCount();
    static void Exit(Int32 p0);
    // @End automatically generated code
};
/// <summary>
/// Represents an instant in time, typically expressed as a date and time of day.
/// </summary>
struct DateTime
{
    // @Begin automatically generated code, do not modify native structs !
    Int16 year;
    Int16 month;
    Int16 day;
    Int16 hour;
    Int16 minute;
    Int16 second;
    Int16 millisecond;
    // @End automatically generated code
};
 
/// <summary>
/// Provides constants and static methods for trigonometric, logarithmic, and other common mathematical functions.
/// </summary>
class Math : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static UInt64 Sin(UInt64 p0);
    static UInt64 Cos(UInt64 p0);
    static UInt64 Tan(UInt64 p0);
    static UInt64 Asin(UInt64 p0);
    static UInt64 Acos(UInt64 p0);
    static UInt64 Atan(UInt64 p0);
    static UInt64 Exp(UInt64 p0);
    static UInt64 Log(UInt64 p0);
    static UInt64 Log10(UInt64 p0);
    static UInt64 Sqrt(UInt64 p0);
    static UInt64 Pow(UInt64 p0, UInt64 p1);
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

/// <summary>
/// Represents the method that executes on the Thread.
/// </summary>
class ThreadStart : public MulticastDelegate
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// <summary>
    /// Initializes a new instance of the ThreadStart class.
    /// </summary>
    ThreadStart(Object* p0, IntPtr p1);
    void ctor(Object* p0, IntPtr p1);
    // @End automatically generated code
    // insert additional private methods and member here ! 

    ThreadStart() {}
    void Invoke();
};

/// <summary>
/// Creates and controls a thread, sets its priority, and gets its status.
/// </summary>
class Thread : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// <summary>
    ///  Causes the operating system to change the state of the current instance to ThreadState.Running.
    /// </summary>
    void Start();
    /// <summary>
    /// Resumes a thread that has been suspended.
    /// </summary>
    void Resume();
    /// <summary>
    /// Either suspends the thread, or if the thread is already suspended, has no effect.
    /// </summary>
    void Suspend();
    /// <summary>
    ///  Raises a ThreadAbortException in the thread on which it is invoked, to begin the process of terminating the thread.
    ///  Calling this method usually terminates the thread.
    /// </summary>
    void Abort();
    /// <summary>
    /// Sets a value indicating the scheduling priority of a thread.
    /// </summary>
    void set_Priority(ThreadPriority p0);
    /// <summary>
    /// Suspends the current thread for a specified time.
    /// </summary>
    /// <param name="ms">The number of milliseconds for which the thread is blocked. Specify zero (0) to indicate that this thread should be suspended to allow other waiting threads to execute.</param>
    static void Sleep(Int32 ms);
    /// <summary>
    /// Finalize method, used to release unmanaged internal objects
    /// This method can only be called by the Garbage collector
    /// </summary>
    void Finalize();
    /// <summary>
    /// Initializes a new instance of the Thread class.
    /// </summary>
    /// <param name="worker">Worker routine of thread to create</param>
    Boolean create(ThreadStart* p0);
    Boolean isCurrent();
    Thread* m_next;
    String* m_name;
    UInt16  m_state;
    UInt16  m_prio;
    ThreadStart* m_start;
    // @End automatically generated code
    // insert additional private methods and member here ! 
    EThread* GetThreadImpl()
    {
        return m_pThreadImpl;
    }
    private:
    Thread();

    EThread*        m_pThreadImpl;
#ifdef PLATFORM_INTERPRETER_SUPPORT
    ClrAppDomain*   m_pAppDomain;
#endif
    const char*     getAsciiName();
    static void static_threadEntry(Thread* pThis);
    void threadEntry();
};
/// <summary>
/// Encapsulates operating system-specific objects that wait for exclusive access to shared resources.
/// </summary>
class WaitHandle : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:

    /// \cond EXTERNAL_DOCUMENTATION_INCLUDE

    /// <summary>
    /// Initializes a new instance of the WaitHandle class.
    /// </summary>
    WaitHandle();
    void ctor();
    
    /// \endcond

    /// <summary>
    /// Blocks the current thread until the current WaitHandle receives a signal.
    /// </summary>
    /// <returns>true if the current instance receives a signal; otherwise, false</returns>
    Boolean WaitOne();
    // @End automatically generated code
    // insert additional private methods and member here ! 
  
    /// \cond EXTERNAL_DOCUMENTATION_INCLUDE
    protected:
    EEvent*    m_pEvent;
    /// \endcond
};

class EnumEventResetMode
{
// @Begin automatically generated code, do not modify native enums !
public:
enum EventResetMode
{
    EventResetMode_sizeof=2,
    AutoReset=0,
    ManualReset=1
};
// @End automatically generated code
};
typedef EnumEventResetMode::EventResetMode EventResetMode;

class EventWaitHandle : public WaitHandle
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    EventWaitHandle(Boolean p0, EventResetMode p1);
    void ctor(Boolean p0, EventResetMode p1);
    Boolean Set();
    Boolean Reset();
    void Finalize();
    // @End automatically generated code
    // insert additional private methods and member here ! 

};

class Monitor : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static void Enter(Object* p0);
    static void Exit(Object* p0);
    static void Pulse(Object* p0);
    static void PulseAll(Object* p0);
    static Boolean TryEnter(Object* p0);
    static Boolean Wait(Object* p0);
    Monitor();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

}; // class mscorlib

/// \}

#endif //#ifndef __MSCORLIB_H__
