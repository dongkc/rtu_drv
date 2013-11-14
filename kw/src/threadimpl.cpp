/*
+-----------------------------------------------------------------------------+
| ***************************** threadimpl.cpp ****************************** |
+-----------------------------------------------------------------------------+
|                      Copyright 2009 by KW Software GmbH                     |
|                                                                             |
|                      KW Software GmbH                                       |
|                      Lagesche Str. 32                                       |
|                      D-32657 Lemgo                                          |
|                      Germany                                                |
|                                                                             |
+-----------------------------------------------------------------------------+
|Description: this file contains the adaptation of the OS-related task        |
|             management functions and system resource management functions.  |
+-----------------------------------------------------------------------------+
|Author: Xiangyang Ren , started at: Augst 20th, 2009                         |
+-----------------------------------------------------------------------------+
|modification history:                                                        |
|  Jul. 18th, 2009 by X. Ren -                                                |
+-----------------------------------------------------------------------------+
*/

#include "eclr.h"
#include "osthread.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// task management functions
// ----------------------------------------------------------------------------

/*
 * Handle SIGUSR1 in the target thread, to suspend it until
 * receiving SIGUSR2 (resume).
 */
static void suspend_signal_handler (int sig)
{
    sigset_t signal_set;

    /*
     * Block all signals except SIGUSR2 while suspended.
     */
    sigfillset (&signal_set);
    sigdelset (&signal_set, SIGUSR2);
    sigsuspend (&signal_set);

    /*
     * Once I'm here, I've been resumed, and the resume signal
     * handler has been run to completion.
     */
    return;
}

/*
 * Handle SIGUSR2 in the target thread, to resume it. Note that
 * the signal handler does nothing. It exists only because we need
 * to cause sigsuspend() to return.
 */
static void resume_signal_handler (int sig)
{
    return;
}

// ----------------------------------------------------------------------------
//        TaskWrapper
//
// Description:  This function is the task body for all user tasks.
//
// Arguments  :  void* exinf -- pointer toCThreadImpl class
//
// Returns    :  NULL
// ----------------------------------------------------------------------------
void* TaskWrapper(void* exinf)
{
	int nResult;
	
	nResult = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	assert(nResult==0);
	
	nResult = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	assert(nResult==0);
	
   CThreadImpl *pThreadImpl = (CThreadImpl*)exinf;
   
   sem_wait(&pThreadImpl->m_threadSem) ; 
   
   pThreadImpl->m_proc(pThreadImpl->m_param);  
   
   return NULL; // Exit task 
}

// ----------------------------------------------------------------------------
//        CThreadImpl::CThreadImpl
//
// Description:  constructor
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CThreadImpl::CThreadImpl()
{
    m_threadId = 0;
    m_proc = NULL;
    m_param = NULL;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::~CThreadImpl
//
// Description:  destructor.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CThreadImpl::~CThreadImpl()
{
   if (m_threadId != 0)
   { 
      m_threadId = 0;
   }
}

// ----------------------------------------------------------------------------
//        CThreadImpl::Release
//
// Description:  This function deletes the CThreadImpl object.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
void CThreadImpl::Release()
{
    delete this;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::Create
//
// Description:  This function creates a new CThreadImpl object.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
CThreadImpl* CThreadImpl::Create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize)
{
   CThreadImpl* result = new CThreadImpl;
   if (result != NULL)
   {
       if (result->create(name,proc, param, stackSize) == true)
       {
                
       }
       else
       {
           delete result;
           result = NULL;
       }
   }
   return result;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::create
//
// Description:  This function creates a task.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CThreadImpl::create(const char* name, ThreadProc proc, ThreadParam param, unsigned stackSize)
{
	int         nResult;
	int         nStackSize = stackSize + 8000; // add extra 8K to hold the thread info produce by pthread_create() 
	pthread_attr_t attr;
	
	if (nStackSize < PTHREAD_STACK_MIN)
	{
		nStackSize = PTHREAD_STACK_MIN + 4; // adjust stack size
	}
	
	/*
     * Install the signal handlers for suspend/resume.
     */
    sigusr1.sa_flags = 0;
    sigusr1.sa_handler = suspend_signal_handler;

    sigemptyset (&sigusr1.sa_mask);
    sigusr2.sa_flags = 0;
    sigusr2.sa_handler = resume_signal_handler;
    sigusr2.sa_mask = sigusr1.sa_mask;

    sigaction (SIGUSR1, &sigusr1, NULL);
    sigaction (SIGUSR2, &sigusr2, NULL);
	
	nResult = pthread_attr_init(&attr);
	assert(nResult==0);

	nResult = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	assert(nResult==0);

   nResult = sem_init(&m_threadSem, 0, 0);
   if(nResult != 0)
   {
   		//printf("sem_init failed\n");
   		return false;
   }

	if (pthread_attr_setstacksize(&attr, nStackSize) != 0)
	{
		sem_destroy(&m_threadSem);
		return false;
	}
	
    m_proc = proc;
    m_param = param;

	nResult = pthread_create(&m_threadId, &attr, TaskWrapper, (void*)this);	
	if (nResult != 0)
	{
		sem_destroy(&m_threadSem);
		return false;
	}
  
   return true;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::SetPriority
//
// Description:  This function changes the task priority.
//
// Arguments  :  new priority
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CThreadImpl::SetPriority(int prioThread)
{ 
   struct sched_param zSchedParam;
   int policy;
   int nResult;
   
   nResult = pthread_getschedparam(m_threadId, &policy, &zSchedParam);
   if(nResult == 0)
   {
   		zSchedParam.sched_priority = prioThread;
   		nResult = pthread_setschedparam(m_threadId, SCHED_FIFO, &zSchedParam);
   		if(nResult == 0)
   		{
   			return true;
   		}
   }
   
   return false;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::Start
//
// Description:  This function starts the task, changes the state from DORMANT to READY.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CThreadImpl::Start()
{
   sem_post(&m_threadSem);	
}   

// ----------------------------------------------------------------------------
//        CThreadImpl::Resume
//
// Description:  This function resumes the task.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CThreadImpl::Resume()
{
   pthread_kill(m_threadId, SIGUSR2);
}   

// ----------------------------------------------------------------------------
//        CThreadImpl::Suspend
//
// Description:  This function suspends the task.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CThreadImpl::Suspend()
{
  pthread_kill(m_threadId, SIGUSR1);
}  

// ----------------------------------------------------------------------------
//        CThreadImpl::Sleep
//
// Description:  This function delays the task.
//
// Arguments  :  task delay time
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CThreadImpl::Sleep(int ms)
{
   usleep(ms * 1000);
   return;
}

// ----------------------------------------------------------------------------
//        CThreadImpl::GetThreadId
//
// Description:  This function gets the task ID of this object.
//
// Arguments  :  none
//
// Returns    :  task ID of this object
// ----------------------------------------------------------------------------
CThreadImpl::ThreadId CThreadImpl::GetThreadId()
{
    return((ThreadId)m_threadId);
}

// ----------------------------------------------------------------------------
//        CThreadImpl::GetCurrentThreadId
//
// Description:  This function gets the task ID of currently running task.
//
// Arguments  :  none
//
// Returns    :  task ID of currently running task
// ----------------------------------------------------------------------------
CThreadImpl::ThreadId CThreadImpl::GetCurrentThreadId()
{
   pthread_t tskid = pthread_self();
   return((ThreadId)tskid);
}

// ----------------------------------------------------------------------------
//        CThreadImpl::Abort
//
// Description:  This function deletes the task and associated resources.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CThreadImpl::Abort()
{
	int nResult = 0;
	
	if (!m_threadId)
	{  
		return false; // object not available
	}  

   	if (m_threadId == (unsigned int)GetCurrentThreadId())
   	{
		nResult = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		assert(nResult==0);
		pthread_exit(NULL);	// never return !!!
   	}
   	else
   	{
		pthread_t tid = (pthread_t)m_threadId;
		pthread_cancel(tid);
		pthread_detach(tid);// PTHREAD_CREATE_DETACHED (don't wait, asynchron)
   	}
	
   sem_destroy(&m_threadSem);
	
   return true;
}



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// event implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//        CEventImpl::Create
//
// Description:  This function creates a CEventImpl object.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
CEventImpl* CEventImpl::Create(bool initialstate, bool isAutoReset)
{
   CEventImpl* result = new CEventImpl();
   if (result != NULL)
   {
       if (result->create(initialstate, isAutoReset) == false)
       {
           delete result;
           result = NULL;
       }
   } 
   return result;
}

// ----------------------------------------------------------------------------
//        CEventImpl::create
//
// Description:  This function creates an event resource.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CEventImpl::create(bool initialstate, bool isAutoReset)
{   
   int nResult = 0;  
   
   if(initialstate == true)
   {
      nResult = sem_init(&m_EventSemID, 0, 1);
   }
   else
   {
      nResult = sem_init(&m_EventSemID, 0, 0);
   }
   
   if(nResult != 0)
   {
   	  //printf("event init failed\n");
      return false;
   }
   return true;    
}

// ----------------------------------------------------------------------------
//        CEventImpl::CEventImpl()
//
// Description:  constructor.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CEventImpl::CEventImpl()
{
}

// ----------------------------------------------------------------------------
//        CEventImpl::Release
//
// Description:  This function deletes an event resource.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CEventImpl::Release()
{
   sem_destroy(&m_EventSemID);
}

// ----------------------------------------------------------------------------
//        CEventImpl::~CEventImpl()
//
// Description:  deconstructor.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CEventImpl::~CEventImpl()
{
}

// ----------------------------------------------------------------------------
//        CEventImpl::Set
//
// Description:  This function singals the event resource.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CEventImpl::Set()
{
    if(sem_post(&m_EventSemID) != 0)
    {
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------
//        CEventImpl::Reset
//
// Description:  This function resets the event resource.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CEventImpl::Reset()
{
   //sig_sem(EventSemId);  /* Release Semaphore resources */
   
   return true;
}

// ----------------------------------------------------------------------------
//        CEventImpl::WaitOne
//
// Description:  This function waits for the event resource.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CEventImpl::WaitOne()
{
   if(sem_wait(&m_EventSemID) != 0)
   {
       return false;
   }

  return true;
}



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// crititical section implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//        CMutexImpl::CMutexImpl()
//
// Description:  constructor.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CMutexImpl::CMutexImpl()
{
   //m_tMutexID = 0;
}

// ----------------------------------------------------------------------------
//        CMutexImpl::~CMutexImpl()
//
// Description:  deconstructor.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CMutexImpl::~CMutexImpl()
{
   
}

// ----------------------------------------------------------------------------
//        CMutexImpl::Create
//
// Description:  This function creates a new CMutexImpl object.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
CMutexImpl* CMutexImpl::Create()
{
   CMutexImpl* result = new CMutexImpl();
   if (result != NULL)
   {
       if (result->create() == false)
       {
           delete result;
           result = NULL;
       }
   }
   return result;    
}

// ----------------------------------------------------------------------------
//        CMutexImpl::create
//
// Description:  This function creates an mutex resource.
//
// Arguments  :  none
//
// Returns    :  true or false
// ----------------------------------------------------------------------------
bool CMutexImpl::create()
{
   if(pthread_mutex_init(&m_tMutexID, NULL) != 0)
   {
      return false;
   }
   
   return true;    
}

// ----------------------------------------------------------------------------
//        CMutexImpl::Release
//
// Description:  This function deletes an event resource.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CMutexImpl::Release()
{
   pthread_mutex_destroy(&m_tMutexID);
}

// ----------------------------------------------------------------------------
//        CMutexImpl::Enter
//
// Description:  This function enters a critical area.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CMutexImpl::Enter()
{
  pthread_mutex_lock(&m_tMutexID);
}

// ----------------------------------------------------------------------------
//        CMutexImpl::Leave
//
// Description:  This function leaves a critical area.
//
// Arguments  :  none
//
// Returns    :  none
// ----------------------------------------------------------------------------
void CMutexImpl::Leave()
{
   pthread_mutex_unlock(&m_tMutexID);
}

