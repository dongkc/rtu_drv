#ifndef __CONTROLL_H__
#define __CONTROLL_H__

#include "eclrlib.h"
#include "appdomain.h"
#include "threading.h"



typedef bool(*LibCompat)(const EclrVersion&);
typedef bool(*LibLoadUnload)(ClrAppDomain*);

class CFirmwareLib : public ClrAllocator, public eListNode<CFirmwareLib>
{
    public :
    CFirmwareLib();
    bool init(void* pMeta, unsigned size, LibCompat pCompat, LibLoadUnload pLoad, LibLoadUnload pUnload);
    ClrAssemblyDef* getAssemblyInfo();
    const char* getName();

    void*           m_pMeta;
    unsigned        m_metaSize;
    ClrAssemblyDef* m_pAssemblyInfo;
    LibCompat       m_pCompat;
    LibLoadUnload   m_pLoad;
    LibLoadUnload   m_pUnload;
};

class CContext : public ClrAllocator, public eListNode<CContext>
{
    public:
    CContext();
    void Abort(int code);
    bool IsActiveContext(void* pSp);
    void SetActive(void* pTop, unsigned size);
    inline bool IsActive()
    {
        return m_isInExecution;
    }
    inline void ResetActive()
    {
        m_isInExecution = false;
    }
    inline void* GetStackTop()
    {
        return m_pStackTop;
    }
    inline void* GetStackBase()
    {
        return m_pStackEnd;
    }
    void SetAppDomain(ClrAppDomain* pAppDomain)
    {
        m_pAppDomain = pAppDomain;
    }
    ClrAppDomain* GetAppDomain()
    {
        return m_pAppDomain;
    }
    jmp_buf             m_jmpbuf;
private:
    ClrAppDomain*       m_pAppDomain;
    void*               m_pStackTop;                       
    void*               m_pStackEnd;                       
    bool                m_isInExecution;
};

class CUnmanagedCallback
{
    public:
    typedef  void (CUnmanagedCallback::* CallbackFunc)();

    CUnmanagedCallback();
    bool SetCB(void* pInst, void (CUnmanagedCallback::* pInvoke)());
    bool SetCB(void (* pInvoke)(void* pParam), void* pParam);
    inline mscorlib::ThreadStart* GetDelegate()
    {
        return &m_delegate;
    }
    private:
    void init();
    void invokeParam();
    static void invokeDummy(void* p);
    mscorlib::ThreadStart   m_delegate;
    void*                   m_pParam;
    void (* m_pInvoke)();
    void (* m_pInvoke_helper)(void*);


};

class CDeviceInfoEntry
{
    public:

    inline int getType()
    {
        return m_type;
    }
    inline const char* getValueString()
    {
        return (m_type == TypeString) ? m_strval : "";
    }
    inline const char* getName()
    {
        return m_tokname;
    }
    inline uint32 getValueUint32()
    {
        return m_intval;
    }

    const char*     m_tokname;  // hold the named token if defined 
    union
    {
        const char* m_strval;    // union to hold integer values as well as ctring values
        uint32      m_intval;   
    };

    uint16          m_token;    // this token
    uint16          m_type;     // data type of the value
};


class CDeviceInfo : public ClrAllocator
{
    public:
    CDeviceInfo();
    enum DeviceInfoToken
    {
        Manufacturer             = 0,
        ProductName              = 1,
        CpuType                  = 2,
        HardwareVersion          = 3,
        FirmwareVersion          = 4,
        MaxApplDataSize          = 5,
        MaxApplRetainDataSize    = 6,
        MaxApplProgramSize       = 7,
        MaxApplImageSize         = 8,
        MaxRuntimeHeapSize       = 9,
        TimerResolution          = 10,
        RuntimeVersion           = 11,
        RuntimeType              = 12,
        ThreadingType            = 13,
        ScheduleInterval         = 14,
        ThreadingPriorityLevel   = 15,
        MaxApplImageFilesSize    = 16,
        DownloadChangeSupported  = 17,
        BuiltinLinkerSupported   = 18,
        CorelibsSignature        = 19,
        CpuProgModel             = 20,
		// if items are added here, then also add the item name to s_predefinedAttributes in controller.cpp
        PredefinedTokenAmount           // must be last enumeration element (highest number)
    };
    bool Create(unsigned size);

    CDeviceInfoEntry* getDeviceInfoEntry(unsigned idx);

    // set/get predefined attributes
    bool setAttribute(enum DeviceInfoToken token, const char* pValue);
    bool setAttribute(enum DeviceInfoToken token, uint32 value);
    bool setAttributeBool(enum DeviceInfoToken token, bool value);
    const char* getAttributeString(enum DeviceInfoToken token);
    uint32 getAttributeUint32(enum DeviceInfoToken token);


    CDeviceInfoEntry* getEntry(unsigned id);
    inline unsigned size()
    {
        return m_currSize;
    }

    private:

    CDeviceInfoEntry* getCustomEntry(const char* pToken);
    bool setAttribute(enum DeviceInfoToken token, enum MetaElementType type, uint32 value);

    CDeviceInfoEntry*   m_pEntryTab;
    unsigned            m_entryTabSize;   
    unsigned            m_currSize;   
};

class ClrController
{
    public:
    static ClrController* Create(const char* name, void* pObjectHeap, unsigned nObjectHeapSize);

    static inline ClrAppDomain* GetCurrentDomain(Object* pObj)
    {
        return getDomainByMeta(pObj->getType());
    }
    static ClrAppDomain* getDomainByMeta(void* pMeta);
    static ClrAppDomain* getDomainByCode(void* pCode);
    static ClrAppDomain* getDomain(const char* name);

    static unsigned addNativeLib(void* pMeta, unsigned size, LibCompat pCompat, LibLoadUnload pLoad, LibLoadUnload pUnload);
    static ClrTypeDef* findType(const char* nameSpace, const char* name);

    static void Abort(int exc);
    static void Exception(int exc, void* pRetAddr, void* pMethodFrame);
    static void Process();

    static ClrAppDomain* CreateDomain(const char* name, unsigned creationMode = 0);
    static CFirmwareLib* getFirmwareLib(const char* name);
    static CFirmwareLib* getFirmwareLib(const char* name, EclrVersion& version);
    static CFirmwareLib* getFirmwareLib(unsigned modulHndl);
    static unsigned getFirmwareLibAmount();

    static ClrUploadStream*  createAssemblyListStream();
    static ClrUploadStream*  createNativeLibsMetaStream();
    static ClrUploadStream*  createNativeLibMetaStream(const char* pName);

    static BlockHeap                s_objectHeap;
    
    typedef eList<ClrAppDomain>     AppDomainList;
    typedef AppDomainList::iterator AppDomainListIter;
    static AppDomainList            s_appDomainList;

    typedef eList<CFirmwareLib>         FirmwareLibList;
    typedef FirmwareLibList::iterator   FirmwareLibListIter;
    static FirmwareLibList          s_firmwareLibList;
    
    static bool checkHeader(CAppImageHdr* pImgHeader);

    static void getHeapStatistic(CHeapStatistic& stat)
    {
        s_objectHeap.statistic(stat);  
    }
    
    static inline CDeviceInfo* getDeviceInfo()
    {
        return &s_deviceInfo;
    }

    static inline void critSectEnter()
    {
#ifdef PLATFORM_THREADING_SUPPORT
        s_pCritSect->Enter();
#endif
    }
    static inline void critSectLeave()
    {
#ifdef PLATFORM_THREADING_SUPPORT
        s_pCritSect->Leave();
#endif
    }

    static uint32 getLibsSign()
    {
        return s_nativeLibsSign;
    }

    static CContext* getCurrentContext(void* sp, unsigned& stackDepth);
    static CContext* enterContext(void* pSp, unsigned stackSize, ClrAppDomain* pAppDomain = NULL);
    static void      leaveContext(CContext* pContext);
    static void Ctors();
private:

    static bool createCorLibs();
    
    static ClrController            s_controllerObj;

    static eString<32>              s_controllerName;
    static CDeviceInfo              s_deviceInfo;

#ifdef PLATFORM_THREADING_SUPPORT
    static CMutexImpl*              s_pCritSect;        
#endif
    static uint32                   s_nativeLibsSign;
    
    typedef eList<CContext>         ContextList;
    typedef ContextList::iterator   ContextListIter;
    static ContextList              s_contextList;

    class CAssemblyListUpload : public ClrUploadStream
    {
        public:
        CAssemblyListUpload();
        virtual ~CAssemblyListUpload();

        // virtual override
        int read(void* pData, unsigned length);

        bool init();
        private:
        StreamIO		m_stream;
        char*           m_pBuffer;
        FirmwareLibListIter m_itLib;

    };
    class CorelibsMetaUpload : public ClrUploadStream
    {
        public:
        CorelibsMetaUpload();
        virtual ~CorelibsMetaUpload();

        // virtual override
        int read(void* pData, unsigned length);

        bool init();
        private:

        CAppImageHdr    m_imageHeader;
        StreamIO		m_stream;
        FirmwareLibListIter m_itLib;
    };
    class CorelibMetaUpload : public ClrUploadStream
    {
        public:
        CorelibMetaUpload(const char* pName);
        virtual ~CorelibMetaUpload();
        bool IsOk()
        {
            return (m_pLib != NULL) ? true : false;
        }
        // virtual override
        int read(void* pData, unsigned length);

        private:
        void init();
        CAppImageHdr    m_imageHeader;
        StreamIO		m_stream;
        CFirmwareLib*   m_pLib;
    };
};




#endif //#define __CONTROLL_H__
