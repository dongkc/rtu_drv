#ifndef __APPDOMAIN_H__
#define __APPDOMAIN_H__

#include "blobheap.h"
#include "gcheap.h"
#include "metadata.h"
#include "threading.h"
#include "appdebug.h"
#include "appdcg.h"

#ifdef PLATFORM_INTERPRETER_SUPPORT
#include "cilsim32.h"
#endif

// some forward declarations ...
class ClrController;
class CFirmwareLib;
class EMonitor;
class CilSim32;

class ClrDebugValue
{
public:
    enum ClrDebugValueSpec
    {
        BitPositionMask = 0x3F,     // bitposition 0 .. 63 possible
        FlagxUnused     = 0x40,     // unused flag
        ReadOnly        = 0x80      // readonly
    };

    ClrDebugValue();

    void Init();
    void Init(ClrTypeDef* pTypeInfo, void* pValue = NULL);
    bool IsArray();
    bool IsString();
    bool IsStruct();
    bool IsObject();
    bool IsPointer();
    bool IsFixedArray();
    bool IsElementary();
    bool IsImplicit()
    {
        return (m_implicitType != 0) ? true : false;
    }
    bool IsReference();
    bool Dereference();

    inline void* GetReference()
    {
        return m_pValue;
    }
    void* GetAddress()
    {
        return m_pValue;
    }
    void SetAddress(void* pAddr)
    {
        m_pValue = pAddr;
    }
    unsigned GetSize();
    int GetType();

    bool Read(ClrInt64& val);
    bool Write(ClrInt64& val);
    bool Write(uint32 val);

    inline ClrTypeDef* GetTypeInfo()
    {
        return m_pTypeInfo;
    }
    inline int GetImplicitType()
    {
        return m_implicitType;
    }
    inline int GetImplicitPos()
    {
        return (m_implicitBitpos & BitPositionMask);
    }
    inline bool IsReadOnly()
    {
        return (m_implicitBitpos & ReadOnly) ? true : false;
    }
    inline void SetReadOnly()
    {
        m_implicitBitpos |= ReadOnly;
    }


    void SetImplicitType(int type, int bitpos);
    uint32 GetTypeHandle(ClrAppDomain* pAppDomain);

    // __TODO__ move to the private section
    void SetReference(ClrTypeDef* pTypeInfo, void* pAddr = NULL);
private:

    inline void setImplicitPos(int bitpos)
    {
        m_implicitBitpos &= ~BitPositionMask;
        m_implicitBitpos |= (uint8)(bitpos & BitPositionMask);
    }

    void writeImplicit(uint32 val, uint32 mask);
    uint32 readImplicit(uint32 mask);
    uint32 getU32();
    void setU32(uint32);
    sint32 getS32();

    ClrTypeDef* m_pTypeInfo;
    void*       m_pValue;
    uint16      m_size;
    uint8       m_implicitType;
    uint8       m_implicitBitpos;
};

class ClrUploadStream : public ClrAllocator
{
    public:

    ClrUploadStream();
    virtual ~ClrUploadStream();
    inline void addRef() { m_refCnt++; }
    void release();

    virtual int read(void* pData, unsigned length) = 0;

    private:
    unsigned m_refCnt;

};

class ClrDownloadStream : public ClrAllocator
{
    public:
    ClrDownloadStream();
    virtual ~ClrDownloadStream();
    inline void addRef() { m_refCnt++; }
    void release();

    virtual int write(void* pData, unsigned length) = 0;
    virtual bool terminate() = 0;

    private:
    unsigned m_refCnt;

};

class CImageFileProxy : public ClrAllocator
{
public:
    CImageFileProxy();

    bool Open(int mode, unsigned length = 0);
    void Close();
    bool Activate(const char* pAppName, bool isPerstent, bool isOK);
    void Remove();
    unsigned Read(unsigned pos, void* pData, unsigned length);
    unsigned Write(unsigned pos, void* pData, unsigned length);
    inline bool IsOpen()
    {
        return (m_openCnt > 0) ? true : false;  
    }
    EclrImageFile* m_pImgFileImpl;
private:
    int m_openCnt;
    int m_lastMode;
};
class CAppFileStream : public ClrUploadStream
{
public :
    CAppFileStream(CImageFileProxy* pImgFile, uint32 start, uint32 size);
    virtual ~CAppFileStream();
    enum SeekFlags { SeekCur = 0, SeekSet = 1, SeekEnd = 2 };

    // virtual override
    int read(void* pData, unsigned length);

    // additional file interface
    void close();
    bool seek(int position, int flags);
	int  tell();
private:
    CImageFileProxy*    m_stream;
    uint32              m_start;
    uint32              m_size;
    uint32              m_currPos;

};

class ClrAppDomain : public eListNode<ClrAppDomain>, public ClrAllocator
{
    public:

    // AppDomain state machine interface
    enum AppState
    {
        Unloaded        = 0,
        LoadRequested   = 1,
        Loaded          = 2,
        StartRequested  = 4,
        Running         = 8,
        StopRequested   = 16,
        Stopped         = 32,
        TerminateRequested  = 64,
        Terminated      = 128,
        Undefined       = 256
    };

    enum DownloadMode
    {
        Persistent,
        NonPersistent,
        Bypass
    };
    enum CreationMode
    {
        DownloadChangeSupport   = 1,
        ExecutionInFlash        = 2,
        CreationDefault         = 0
    };

    ~ClrAppDomain();
    static ClrAppDomain* Create(const char* name, unsigned creationMode);
    
    bool SetDataMemory(void* pData, unsigned nDataSize, unsigned usablePercentage = 100);
    bool SetCodeMemory(void* pCode, unsigned nCodeSize);
    bool SetImageStream(EclrImageFile* pImgStream);
    ClrAppDomain* GetSibling()
    {
        return m_pSibling;
    }

    bool Load();
    bool LoadImage();
    bool Start(enum AppState requState = Running);
    bool Process();
    bool StopRequest(int excCode = 0);
    bool Stop(int maxDelay);
    bool Restart();
    bool Continue();
    bool Terminate();
    bool Unload();
    void Exception(int exc, EThread* pThread, void* pRetAddr, ClrMethodFrame* pMethodFrame);
    void Exception(int exc, EThread* pThread, void* pStackTop, unsigned stackSize, void* pRetAddr, ClrMethodFrame* pMethodFrame);
    bool Activate();
    bool Deactivate();
    bool IsActiveDomain();
    int  Invoke(void (*pInvoke)());
    int  Invoke(void (*pInvoke)(int p0), int p0);
    int  Invoke(void* (*pInvoke)(), void*& result);
    int  Invoke(void* (*pInvoke)(int p0), int p0, void*& result);
    int  Invoke(Object* pThis, void (*pInvoke)());
    int  Invoke(Object* pThis, int (*pInvoke)(Object* p0), Object* p0);
    void NotifyBpReached(ClrAppBreakpoint* pBp)
    {
        m_pEvents->AppBpReached(pBp);
    }

    enum AppState getAppState();
    inline bool isRunning()
    {
        return (m_appState == Running) ? true : false;
    }
    inline bool isStopped()
    {
        return (m_appState == Stopped) ? true : false;
    }
    inline bool isTerminated()
    {
        return (m_appState == Terminated) ? true : false;
    }

    void getCodeHeapStatistic(CHeapStatistic& stat);
    void getDataHeapStatistic(CHeapStatistic& stat);

    // appDomain stream interface
    ClrUploadStream*    createImageUploadStream(bool persistent = true);
    ClrUploadStream*    createMetaUploadStream(bool persistent = false);
    ClrDownloadStream*  createImageDownloadStream(enum DownloadMode mode);
    CAppFileStream*     createAppFileStream(const char* pName);
    inline CAppImageHdr* getImageHdr()
    {
        return m_pImageHeader;
    }
    inline uint32 GetCodeCapacity()
    {
        return m_imageCapacity;
    }
    inline void* GetCodeBase()
    {
        return m_pImageHeader;
    }
    inline void* GetDataBase()
    {
        return m_pGcHeap->Base();
    }
    inline void* GetMetaBase()
    {
        return (m_pImageHeader+1);
    }
    inline bool IsExecutionInFlash()
    {
        return (m_creationMode & ExecutionInFlash) ? true : false;
    }
    inline bool IsDownloadChangeSupport()
    {
        return (m_creationMode & DownloadChangeSupport) ? true : false;
    }
    const char* getName();
    void setName(const char* pName);

    inline ClrTypeDef* getMainTypeInfo()
    {
        return m_pMainTypeInfo;
    }
    inline unsigned GetLastException()
    {
        return m_lastException;
    }

    bool RegisterCallback(ClrAppDomainEvents* pEvents);
    bool UnregisterCallback(ClrAppDomainEvents* pEvents);

    bool RegisterThread(EThread* pThread);
    bool DeregisterThread(EThread* pThread);
    void ThreadEnterWaiting(EThread* pThread);
    void ThreadLeaveWaiting(EThread* pThread);
    unsigned GetThreadInvokeCnt()
    {
    #ifdef PLATFORM_THREADING_SUPPORT
        return m_threadInvokeCnt;
    #else
        return 0;
    #endif
    }
    unsigned GetThreadAmount()
    {
    #ifdef PLATFORM_THREADING_SUPPORT
        return m_threadAmount;
    #else
        return 0;
    #endif
    }
    inline void SetThreadState(unsigned requestedState)
    {
        m_threadState = requestedState;
    }
    inline unsigned GetThreadState()
    {
        return m_threadState;
    }
    inline bool isStopRequested()
    {
        return m_stopRequested;
    }

    ClrTypeDef* findType(const char* nameSpace, const char* className);
    ClrTypeDef* findArrayType(const char* nameSpace, const char* className, int dimension);
    ClrAssemblyDef* getAssembly(const char* assemName);
    
    void* getStaticAddress(ClrTypeDef* pType, ClrField* pField);
    void* getStaticAddress(ClrTypeDef* pTypeDef);
    void* getStaticAddress(const char* pNameSpace, const char* pClassName, const char* pFieldName);
    inline BlockHeap* getMetaHeap()
    {
        return &m_metaHeap;
    }
    inline BlockHeap* getCodeHeap()
    {
        return &m_codeHeap;
    }
    inline ClrGcHeap* getDataHeap()
    {
        return m_pGcHeap;
    }

    static bool isMethodValid(ClrMethod* pMethod);
    //bool relocateStaticData(ClrTypeDef* pStaticType1, ClrTypeDef* pStaticType2);
    bool relocateStaticData();
    bool relocateStaticData(ClrTypeDef* pTypeInfo[], ClrTypeStaticData* pNewStaticData[], unsigned nTypeInfo);


#ifdef PLATFORM_DOWNLOADCHANGE_SUPPORT
    bool DlcProlog(ClrDlcStatistic& stat);
    bool DlcChange(bool allowRealtimeViolation, ClrDlcStatistic& stat);
    bool DlcEpilog(ClrDlcStatistic& stat);
    unsigned DlcGetLastError();
#endif
 
    bool BpHandler(void* pRetAddr, ClrMethodFrame* pFrame);
    bool CreateBreakpoint(uint32 methHandle, uint32 addr, int type);
    bool DeleteBreakpoint(uint32 methHandle, uint32 addr);
    void DeleteBreakpointList();
    bool IsBreakpointSet();
    bool CreateCallStack(int exc, EThread* pThread, void* pStackTop, unsigned stackSize, void* pRetAddr, ClrMethodFrame* pMethodFrame);   
    ClrDebugChain* GetCallstackChain();
    bool ExceptionHandler(ClrMethodFrame* pFrame, Object* pExcObject);
    bool IsHaltOnException()
    {
#ifdef PLATFORM_BREAKPOINT_SUPPORT
        return m_isHaltOnException;
#else
        return false;
#endif
    }
    void AttachBreakpointService()
    {
#ifdef PLATFORM_BREAKPOINT_SUPPORT
        m_bpServiceCount++;
#endif
    }
    void DetachBreakpointService()
    {
#ifdef PLATFORM_BREAKPOINT_SUPPORT
        m_bpServiceCount--;
#endif
    }

    int GetDebugValue(const char* pNameList[], int listSize, ClrDebugValue& debugValue);
    bool GetDebugValue(const char* pPath, ClrDebugValue& debugValue, const char** ppLastFieldName = NULL);   

    static bool IsFixedArray(ClrTypeDef* pTypeInfo);
    static bool IsFixedArray(ClrTypeDef* pTypeInfo, ClrTypeDef*& pElementTypeInfo, int& lowerBound, unsigned& elementCount);

#ifdef PLATFORM_BREAKPOINT_SUPPORT
	inline void ActivateBreakpoints()
	{
		resetBreakpoints(ClrAppBreakpoint::BpAll);
		setBreakpoints(ClrAppBreakpoint::BpAll);
	}
#endif

    private:
    
    ClrAppDomain(const char* name, unsigned creationMode);
    bool init();

    int GetDebugValueImpl(const char* pNameList[], unsigned listSize, ClrDebugValue& debugValue);

    static void illegal_call_exception();
    void invokeDummy1();
    int  invokeDummy2(Object* p);

    bool setCodeMemory(void* pCode, unsigned nCodeSize);
    bool setMethodDefaultEntry();
    bool relocateApplication();
    bool createStaticData();
    bool createStringLiterals();
    bool bindFirmware();
    bool loadFirmware();
    bool unloadFirmware();
    bool assignWorkSpace(CAppImageHdr* pImgHeader);
    bool relocateVTable();
    bool executeMain();
    bool appRelocate();
    bool appSetup();
    bool appLoad();
    bool appUnload();
    bool appThreadsAbort();
    bool appThreadsAbortCheck();
    bool typeInitialize();
    bool adjustObjectSize();
    // callstack support
    void deleteCallStack();
    // breakpoint support
    ClrAppBreakpoint* createBreakpoint(void* pCode, uint32 methHandle, uint32 methOffs, int bpType);
    ClrAppBreakpoint* getBreakpoint(void* pAddr);
    ClrAppBreakpoint* getBreakpoint(uint32 methHandle, uint32 methOffs);

    void deleteBreakpoint(ClrAppBreakpoint* pBp);
    void deleteBreakpoints(int bpType);
    void setBreakpoints(int bpType);
    void resetBreakpoints(int bpType);
    void dumpBreakpointList();

    ClrMethod* getMethodInfoFromCode(void* pCode, uint32& methHandle, uint32& methOffset);
    ClrTypeDef* getTypeInfoFromMethod(ClrMethod* pMethod);

    eVector<CFirmwareLib*, 16>  m_firmwareList; 

    ClrAppDomain*           m_pSibling;
    BlockHeap               m_codeHeap;
    BlockHeap               m_metaHeap;
    ClrGcHeap               m_gcHeap;
    ClrGcHeap*              m_pGcHeap;
    CAppImageHdr*           m_pImageHeader;
    uint32                  m_imageCapacity;

    unsigned                m_lastException;
    bool                    m_isInMain;

    bool                    m_stopRequested;

    enum AppState           m_appState;
    enum DownloadMode       m_imageMode;
    unsigned                m_creationMode;
    ClrTypeDef*             m_pMainTypeInfo;
    ClrMethod*              m_pMainMethod;

    class CImageUpload : public ClrUploadStream
    {
        public:
        CImageUpload(CImageFileProxy* pUpload);
        virtual ~CImageUpload();

        // virtual override
        int read(void* pData, unsigned length);

        private:
        CImageFileProxy*    m_pUpload;
        unsigned            m_currPos;

    };
    class CMetaUpload : public ClrUploadStream
    {
        public:
        CMetaUpload(CAppImageHdr* pHdr, uint32 length);
        CMetaUpload(CImageFileProxy* pStream, uint32 length);

        virtual ~CMetaUpload();

        // virtual override
        int read(void* pData, unsigned length);

        private:
        void init();

        StreamIO			m_stream;
        CImageFileProxy*    m_pMetaUpload;
        sint32              m_metaLeft;
        unsigned            m_currPos;
    };


    class CImageDownload : public ClrDownloadStream
    {
        public:
        CImageDownload(ClrAppDomain* pAppDomain, CImageFileProxy* pImgDownload, enum DownloadMode mode);
        virtual ~CImageDownload();

        // virtual override
        int write(void* pData, unsigned length);
        bool terminate();

        //private:

        StreamIO			m_stream;
        CImageFileProxy*    m_pImgDownload;
        ClrAppDomain*       m_pAppDomain;
        uint32              m_codeSize;
        uint32              m_dataSize;
        uint32              m_codeBytesWritten;
        uint32              m_dataBytesWritten;
        uint32              m_fileBytesWritten;
        enum DownloadMode   m_mode;
        CAppImageHdr        m_imageHdr;
        uint32              m_timeProcess;
        bool                m_isTerminated;

    };

    CImageFileProxy         m_imgFileProxy;
    CImageFileProxy*        m_pImgFile;

    ClrAppDomainEvents      m_defaultEvents;
    ClrAppDomainEvents*     m_pEvents;
#ifdef PLATFORM_DOWNLOADCHANGE_SUPPORT
    ClrAppDcg*              m_pDcg;
#endif
#ifdef PLATFORM_BREAKPOINT_SUPPORT
    eList<ClrAppBreakpoint> m_bpList;
    BpHandlerStub           m_pBpHandlerStub;
    int                     m_bpServiceCount;
    bool                    m_isHaltOnException;
#endif
#ifdef PLATFORM_CALLSTACK_SUPPORT
    eList<ClrDebugChain>    m_chain;
#endif

#ifdef PLATFORM_THREADING_SUPPORT
    unsigned                m_threadAmount;
    unsigned                m_threadInvokeCnt;
#endif
    unsigned                m_threadState;
#ifdef PLATFORM_INTERPRETER_SUPPORT
    CilSim32                m_interpreter;
#endif
};

#endif //#define __APPDOMAIN_H__
