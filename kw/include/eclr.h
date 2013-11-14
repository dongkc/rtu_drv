
#ifndef __ECLR_H__
#define __ECLR_H__

#include "eclrversion.h"

// target description
// do not modify this values ! 

// processor families and types
enum EclrCpuType
{
    CpuType_FamIntel16      = 0x0010,                   // Intel x86 16 bit   
    CpuType_i8086           = CpuType_FamIntel16+1,     // Intel 8086
    CpuType_i8186           = CpuType_FamIntel16+2,     // Intel 8186
    CpuType_FamIntel16End   = CpuType_FamIntel16+3,

    CpuType_FamIntel32      = 0x0100,                   // Intel x86 32 bit   
    CpuType_i286            = CpuType_FamIntel32+1,     // Intel 80286
    CpuType_i386            = CpuType_FamIntel32+2,     // Intel 80386
    CpuType_i486            = CpuType_FamIntel32+3,     // Intel 80486
    CpuType_Pentium         = CpuType_FamIntel32+4,     // Intel 80586
    CpuType_FamIntel32End   = CpuType_FamIntel32+5,

    CpuType_FamArm          = 0x0200,                   // ARM
    CpuType_Arm7            = CpuType_FamArm+1,         // ARM 7
    CpuType_FamArmEnd       = CpuType_FamArm+2,

    CpuType_FamNios         = 0x0300,                   // Altera NIOS
    CpuType_Nios2           = CpuType_FamNios+1,        // NIOS 2
    CpuType_FamNiosEnd      = CpuType_FamNios+2,

    CpuType_FamTms320       = 0x0400,                   // Texas tms 320
    CpuType_FamTms320End    = 0x0400,                   // Texas tms 320

    CpuType_FamMips         = 0x0500,                   // Mips
    CpuType_Mips32          = CpuType_FamMips+1,        // Mips32
    CpuType_Mips64          = CpuType_FamMips+2,        // Mips64
    CpuType_FamMipsEnd      = CpuType_FamMips+3,

    CpuType_FamSh           = 0x0600,                   // SH
    CpuType_Sh2             = CpuType_FamSh+1,          // SH2
    CpuType_Sh4             = CpuType_FamSh+2,          // SH4
    CpuType_FamShEnd        = CpuType_FamSh+3,

    CpuType_FamSim           = 0x0700,                  // Simulation
    CpuType_Sim32            = CpuType_FamSim+1,        // Simulation 32
    CpuType_FamSimEnd        = CpuType_FamSim+2,

    CpuType_FamPPC           = 0x0800,                  // PowerPC
    CpuType_PPCe200          = CpuType_FamPPC+1,        // MPC601, PowerQuicc, MPC5xx, ...
    CpuType_PPCe300          = CpuType_FamPPC+2,        // MPC603, PowerQuicc II Pro
    CpuType_FamPPCEnd        = CpuType_FamPPC+3,

    CpuType_FamArmThumb      = 0x0900,                  // ARM Thumb 
    CpuType_ArmThumb         = CpuType_FamArmThumb+1,   // ARM7, Cortex-M3
    CpuType_ArmThumb2        = CpuType_FamArmThumb+2,   // Cortex-M3
    CpuType_FamArmThumbEnd   = CpuType_FamArmThumb+3,

    // to be continued ...
    CpuType_Any             = 0x0000
};  

enum EclrOperatingSystem
{
    EclrOS_KW               = 0x0010,
    EclrOS_VxWorks4         = 0x0021,
    EclrOS_VxWorks5         = 0x0022,
    EclrOS_VxWorks6         = 0x0023,
    EclrOS_Win32            = 0x0031,
    EclrOS_Mona             = 0x0041,
    EclrOS_iTRON            = 0x0051,
    EclrOS_uCOSII           = 0x0061,
    EclrOS_Linux            = 0x0071,
    EclrOS_TKernel          = 0x0081,
    EclrOS_QNX64            = 0x0091,
	EclrOS_CE5              = 0x00A1,
	EclrOS_CE6              = 0x00A2,
	EclrOS_INtime           = 0x00B1,
    EclrOS_eCOS3            = 0x00C1,
    EclrOS_rtlabs           = 0x00D1,
// to be continued ...
    EclrOS_Any              = 0x0000
};

// compiler dependent programming models
enum EclrProgModel
{
    ProgModel_Bcc3          = 0x0011,       // Borlandc 3.1 realmode 16 bit Intel x86
    ProgModel_Gcc2          = 0x0021,       // Gnu gcc 2.x  
    ProgModel_Gcc3          = 0x0022,       // Gnu gcc 3.x
    ProgModel_Msc12         = 0x0031,       // Microsoft VCS 6.0
    ProgModel_Msc13         = 0x0032,       // Microsoft Studio.NET 2003
    ProgModel_Ghs4          = 0x0041,       // Green Hills Software 4.x
    ProgModel_Arm           = 0x0051,       // ARM
    ProgModel_Sim1          = 0x0061,       // Simulation
    ProgModel_Sh            = 0x0071,       // SH
    ProgModel_Rsas          = 0x0081,       // Renesas
    ProgModel_Ppc           = 0x0091,       // PPC
// to be continued ...
    ProgModel_Any           = 0x0000
};

// version of the eclr metadata, counted up from base
#define ECLRMETA_COMPATID_BASE  10

// compiler dependent data type models, do not modify this values !
#define PLATFORM_DATAMODEL_BASE         10
#define PLATFORM_DATAMODEL_ILP32        (PLATFORM_DATAMODEL_BASE+0) // have to be 0 because of compatability !
#define PLATFORM_DATAMODEL_ILP32_64     (PLATFORM_DATAMODEL_BASE+1)
#define PLATFORM_DATAMODEL_ILP64        (PLATFORM_DATAMODEL_BASE+10)
// to be continued ...

// platform endianess (little / big)
#define TARGET_LITTLE_ENDIAN        1
#define TARGET_BIG_ENDIAN           2

// platform stack growing police
#define TARGET_STACKPOLICY_UNDEFINED    0
#define TARGET_STACKPOLICY_UPWARD       1
#define TARGET_STACKPOLICY_DOWNWARD     2

// default is empty function attributes
#define __PInvoke__

// define some physical constraints regarding the addressing (byte/word addressing)
// default is byte byte addressing mode
#define TARGET_SET_ADDRESS(addr)    (addr)
#define TARGET_GET_ADDRESS(addr)    (addr)

#define PLATFORM_ARGLIST_SUPPORT

// general platform 
#include "econfig.h"

// check the eclr associated datamodel
#if PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP32
#elif PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP32_64
#else
#error eclr datamodel not defined!
#endif

// check the associated endianess
#if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
#elif (PLATFORM_TARGET_ENDIANESS == TARGET_LITTLE_ENDIAN)
#else
#error target endianess not defined
#endif

#if PLATFORM_STACKPOLICY == TARGET_STACKPOLICY_UPWARD
#elif PLATFORM_STACKPOLICY == TARGET_STACKPOLICY_DOWNWARD
#else
#error stack policy not defined
#endif

// currently the gc is necessary only if download change is supported
#ifdef PLATFORM_DOWNLOADCHANGE_SUPPORT
#ifndef PLATFORM_GCHEAP_SUPPORT
#define PLATFORM_GCHEAP_SUPPORT
#endif
#endif

// ctsbase.h need
template<class T>
inline T align_up(T size, unsigned align)
{
      return ((size + (align-1)) & ~((T)(align-1)));
}

// common type system
#include "ctsbase.h"
// embedded template container
#include "estl.h"

class EclrVersion
{
public:
    EclrVersion();
    bool cmpLt(unsigned v1,unsigned v2,unsigned v3 = 0,unsigned v4 = 0);
    bool cmpGe(unsigned v1,unsigned v2,unsigned v3 = 0,unsigned v4 = 0);
    bool cmpGe(const EclrVersion& v)
    {
        return cmpGe(v.major,v.minor, v.build, v.revsn);
    }
    bool cmpEq(unsigned v1,unsigned v2,unsigned v3 = 0,unsigned v4 = 0);

    uint16 major;
    uint16 minor;
    uint16 build;
    uint16 revsn;
};

// Application image header
//same as in 
//		MSILv2VersionHandling.h		(MP Adaption)
//		ClrImageHeader.cs			(Ade.MPOnline)
//(keep consistent!)
class CAppImageHdr
{
    public :
    enum ClrMetaHeaderSpec
    {
        CompFlagsDebug      = 0x0001,
        CompFlagsLinked     = 0x0002,
        CompFlagsBigEndian  = 0x0004,
        Any                 = 0x0000
    };

    CAppImageHdr();

    void SetHeader(const char* pName, unsigned metaSize, unsigned codeSize, unsigned dataSize);
    void SetXsum();
    bool CheckXsum();
    bool IsLittleEndian();
    bool IsBigEndian();
    void Swap();
    void SetDataModel(unsigned model)
    {
        dataModel = (uint16)(model-PLATFORM_DATAMODEL_BASE);
    }
    unsigned GetDataModel()
    {
        return dataModel+PLATFORM_DATAMODEL_BASE;
    }
    void SetMetaVersion(unsigned ver)
    {
        metaVersion = (uint16)(ver-ECLRMETA_COMPATID_BASE);
    }
    unsigned GetMetaVersion()
    {
        return metaVersion+ECLRMETA_COMPATID_BASE;
    }
    bool isLinked()
    {
        return (compFlags & CompFlagsLinked) ? true : false;
    }
    // signature and version
    uint32  sEclr;
    // application domain, the meta data belong to
    char    apdomainName[28]; 
    
    // 32 
    // compiler version a.b.c.d 
    EclrVersion compilerVersion;

    // 40
    // platform and programming model
    uint16  procType;   // processor type   (80186, Pentium, Arm7, ...)  
    uint16  osType;     // operating system (VxWorks, KW, ...)  
    uint16  progModel;  // programming model (native call police (cdecl or stdcall), native this police (register or stack)  )
    uint16  compFlags;  // compiler flags 
    
    // 48
    // 64 bit alignment
    // size of the meta data
    uint32  metaSize;
    uint32  codeSize;
    
    // 56
    // 64 bit alignment
    uint32  fileSize;
    uint16  eclrMajor;  
    uint16  eclrMinor; 
    
    // 64
    // 64 bit alignment
    // 64 bit time calculated by ctime64()
    uint32  ctimeLow;
    uint32  ctimeHigh;

    // 72   
    // 64 bit alignment
    uint16  xsumHead;
    uint16  xsumCode;
    uint16  xsumData;
    uint16  unused1;

    // 80   
    uint32  metaSign;
    uint32  codeSign;

    // 88   
    uint32  retainSign;
    uint32  unused2;

    // 96 
    uint32  retainSize;
    uint32  dataSize;       

    // 104
    uint16  metaVersion;
    uint16  dataModel;
    uint32  libsSign;
    
    // 112
    uint32  dataBaseLow;
    uint32  dataBaseHigh;
    
    // 120
    uint32  codeBaseLow;
    uint32  codeBaseHigh;

    // 128 sizeof image header
    static const uint32 eClrSignature;        // signature 'ECLR'

    void init();

    private :

};



typedef void(*VoidFVoid)();                             // void f(void)
typedef void(*VoidFInt)(int);                           // void f(int)
typedef void*(*PVoidFInt)(int);                         // void* f(int)
typedef VoidFVoid *VTabl; 

#define SINT16_MAX      32767    
#define SINT16_MIN     -32768

#define UINT16_MAX      65535
#define UINT16_MIN      0

// manages the overloding of operator new, delete
class ClrAllocator
{
    public:
    void* operator new(size_t size);
    void operator delete(void* p);
#ifndef ARRAYNEW_NOT_SUPPORTED
    void* operator new[](size_t size);
    void operator delete[](void* p);
#endif
};

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

inline int min(int n, int m) 
{
    return n < m ? n : m;
}

inline int max(int n, int m)
{
    return n > m ? n : m;
}




inline void* align_up_ptr(void* ptr, unsigned align)
{
    return (void*)(long)align_up((long)ptr, align);
}

// alignment down to the previous aligned value
template<class T>
inline T align_down(T size, unsigned align)
{
    return (size & ~((T)(align-1)));
}



// little|big endian peripheral to host
inline uint32 letohl(const uint8* p)
{
      // decode little endian buffer in val32
    uint32 val = ((uint32)p[3]<<24) | ((uint32)p[2]<<16) | ((uint32)p[1]<<8) | ((uint32)p[0]);
    return val;
}
inline uint32 betohl(const uint8* p)
{
      // decode big endian buffer in val32
    uint32 val = ((uint32)p[0]<<24) | ((uint32)p[1]<<16) | ((uint32)p[2]<<8) | ((uint32)p[3]);
    return val;
}


// read 2 byte integer from little endian
inline uint16 letohs(const uint8* p)
{
    // decode little endian buffer in val
    uint16 val = (uint16)(((uint16)p[1]<<8) | (uint16)p[0]);
    return val;
}

inline uint16 betohs(const uint8* p)
{
      // decode big endian buffer in val16
    uint16 val = (uint16)(((uint16)p[0]<<8) | (uint16)p[1]);
    return val;
}


// host to little|big endian peripheral
inline void htolel(uint8* p, sint32 val)
{
      // encode val32 in little endian order
    *p++ = (uint8)val;
    *p++ = (uint8)(val>>8);
    *p++ = (uint8)(val>>16);
    *p++ = (uint8)(val>>24);
}
inline void htoles(uint8* p, int val)
{
    // encode val16 in little endian order
    *p++ = (uint8)val;
    *p++ = (uint8)(val>>8);
}

inline void htobes(uint8* p, int val)
{
    // encode val16 in little endian order
    *p++ = (uint8)(val>>8);
    *p++ = (uint8)val;

}

inline void htobel(uint8* p, sint32 val)
{
    // encode val32 in big endian order
    *p++ = (uint8)(val>>24);
    *p++ = (uint8)(val>>16);
    *p++ = (uint8)(val>>8);
    *p++ = (uint8)val;
}

inline int encodeCompressed(uint8* p, uint32 val)
{
    if (val < 0x80)
    {
        *p = (uint8)val;
        return 1;
    }
    else if (val < 0x4000)
    {
        htobes(p, val);
        return 2;
    }
    else if (val < 0x20000000)
    {
        htobel(p, val);
        return 4;
    }
    // not possible to compress such value !!!
    return 0;

}

typedef struct _guid { uint8 id[16]; } Guid;
typedef uint32 ClrToken; 


inline unsigned tokenId(ClrToken tok)
{   
    return tok & 0xFFFFFF;
}

inline int tokenType(ClrToken tok)
{   
    return (int)(tok >> 24);
}

inline ClrToken token(int type, unsigned idx)
{   
    return ((uint32)type << 24) | idx;
}

enum MetaElementType 
{
    TypeEnd         = 0x00,     // marks end of list
    TypeVoid        = 0x01,
    TypeBool        = 0x02,
    TypeChar        = 0x03,
    TypeInt8        = 0x04,
    TypeUint8       = 0x05,
    TypeInt16       = 0x06,
    TypeUint16      = 0x07,
    TypeInt32       = 0x08,
    TypeUint32      = 0x09,
    TypeInt64       = 0x0a,
    TypeUint64      = 0x0b,
    TypeReal32      = 0x0c,
    TypeReal64      = 0x0d,
    TypeElementary  = TypeUint64,
    TypeString      = 0x0e,
    TypePtr         = 0x0f,

    TypeByRef       = 0x10,
    TypeValueType   = 0x11,
    TypeClass       = 0x12,
    TypeUTF8String  = 0x13, // KW defined
    TypeArray       = 0x14,
    TypeTypedByRef  = 0x16,
    TypeNativeInt   = 0x18,
    TypeNativeUint  = 0x19,
    TypeAnsiString  = 0x1a, // KW defined
    TypeFuncPtr     = 0x1b,
    TypeObject      = 0x1c,
    TypeSZarray     = 0x1d,
    TypeUTF16String = 0x1e, // KW defined

    TypeCmodRequ    = 0x1F,
    TypeCmodOpt     = 0x20,

    TypePinned      = 0x45,
    TypeArrayLength = 0x80, // KW defined
    TypeGetType     = 0x81, // KW defined
    TypeMaxRange    = 0xFF,

    TypeUnknown     = 0

};

inline bool typeIsInteger(int type)
{
    bool result = false;
    if (type <= TypeUint64)
    {
        result = true;
    }
    else if (type == TypeNativeInt || type == TypeNativeUint)
    {
        result = true;
    }
    return result;
}

inline bool typeIsSigned(int type)
{
    return ((type & 0x1) == 0) ? true : false;
}

inline bool typeIsFloatingPoint(int type)
{
    bool result = false;
    if (type == TypeReal32 || type == TypeReal64)
    {
        result = true;
    }
    return result;
}

inline bool typeIsReference(int type)
{
    bool result = false;
    switch(type)
    {
        case TypeString     :
        case TypePtr        :
        case TypeClass      :
        case TypeArray      :
        case TypeObject     :
        case TypeSZarray    :
            result = true;
            break;
        default :
            break;

    }
    return result;
}

enum RelocationType
{
    RelocMovai  = 0x0,  // meta relocation
    RelocCalli  = 0x1,  // meta relocation
    RelocAny1   = 0x2,
    RelocAny2   = 0x3
};

class RelocationEntry
{
    public:
    unsigned codeAddr;
    unsigned metaAddr;
    unsigned relocTyp;
};

extern bool eclrSetRelocation(void* pRelocTarget, long int relocValue, int relocType);
extern bool eclrSetBreakpointHandler(void* pCode, void (*pBpHandler)(), char* pBackup, unsigned& nBackup);
extern bool eclrRelocateCalli(void* pCode, PtrType src, PtrType dst);
extern bool eclrRelocateMovai(void* pCode, PtrType src, PtrType dst);


enum EclrException
{
    ExcOutOfMemory          = 1,
    ExcDivisionByZero       = 2,
    ExcIndexOutOfRange      = 3,
    ExcIllegalCast          = 4,
    ExcStackOverflow        = 5,
    ExcNullReference        = 6,
    ExcMissingMethod        = 7,
    ExcThreadCreation       = 8,
    ExcThreadAbort          = 9,
    ExcSynchronizationLockException = 10,
    ExcBreakpointIllegal    = 11,
    ExcBreakpoint           = 12,
    ExcExecutionEngine      = 13,
    ExcUnspecified,
    ExcExternal             = 128
};



class ClrAppDomain;
class ClrAppBreakpoint;
class ClrAppDomainEvents : public eListNode<ClrAppDomainEvents>
{
    public:
///
/// Notifies the callback implementer when an application image is successfully loaded
/// \param pAppDomain Pointer to the domain object that generated the event.
///
    virtual ~ClrAppDomainEvents();
    virtual void AppLoad();
    virtual void AppUnload();
    virtual void AppException(int code);
    virtual void AppSchedule();
    virtual void AppStart();
    virtual void AppStop();
    virtual void AppContinue();
    virtual void AppTerminate();
    virtual void AppSwap(ClrAppDomain* pAppDomain);
    virtual void AppBpReached(ClrAppBreakpoint* pBp);
};


class EclrFile
{
    public:

    enum OpenFlags
    { 
        Open_r = 1,     // Opens for reading. If the file does not exist or cannot be found, the fopen call fails. equivalent of "rb"
        Open_w = 2,     // Opens an empty file for writing. If the given file exists, its contents are destroyed. equivalent of "wb"
        Open_rw = 3,    // Opens for both reading and writing. (The file must exist.) equivalent of "r+b"
        Open_c = 4      // Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed. equivalent of "w+b"
    };

    enum SeekModes
    { 
        Seek_cur = 1,     // seek from current position
        Seek_set = 2,     // seek from begin
        Seek_end = 3      // seek from end
    };

    static EclrFile* Open(const char* fname , enum OpenFlags mode);
    static bool Remove(const char* fname);
    static bool Close(EclrFile* pFile);
    static int Read(EclrFile* pFile, void* buffer, int size);
    static int Write(EclrFile* pFile, void* buffer, int size);
    static long Seek(EclrFile* pFile, long offset, enum SeekModes mode);
};

class EclrImageFile
{
    public:
    enum FileMode 
    { 
        Create = 1, 
        RdOnly = 2, 
        WrOnly = 4, 
        RdWr   = 6, 
        NonPersistent = 8 
    };
    EclrImageFile() {}
    virtual ~EclrImageFile() {}

    // virtual interface
    virtual bool Open(int mode, unsigned length) = 0;
    virtual void Close() = 0;
    virtual bool Activate(const char* pAppName, bool isPerstent, bool isOK) = 0;
    virtual void Remove() = 0;
    virtual unsigned Read(unsigned pos, void* pData, unsigned length) = 0;
    virtual unsigned Write(unsigned pos, void* pData, unsigned length) = 0;
};

class EclrEnvironment
{
    public :
    typedef struct _datetime
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int millisecond;
    } DateTime;

    static uint32   GetTickCount();
    static uint32   GetMicroTickCount();
    static bool     GetCurrentDateTime(DateTime& dt);
    static void     FlushCache(void* pData, unsigned length);

};

#define REMOTING_SOCKET_NEWSTYLE
class EclrRemotingSocket : public ClrAllocator
{
public :
    enum BlockingMode 
    { 
        Blocking = 0, 
        NonBlocking  
    };
    
    EclrRemotingSocket();

    // adaptation interface of abstact remoting socket
    virtual EclrRemotingSocket* Open(BlockingMode mode, bool& wouldblock) = 0;
    virtual int                 Send(const void* buf, int len) = 0;
    virtual int                 Recv(void* buf, int len) = 0;
    virtual void                Close() = 0;

    // reference counting
    inline void AddRef()
    {
        m_refCnt++;
    }
    inline unsigned GetRefCnt()
    {
        return m_refCnt;
    }
    void Release();

    // just for compatibility
    inline void addRef()
    {
        AddRef();
    }
    inline unsigned getRefCnt()
    {
        return GetRefCnt();
    }
    inline void release()
    {
        Release();
    }

    bool m_isEstablished;
protected:
    virtual ~EclrRemotingSocket();

private :

    unsigned m_refCnt;

};

//---------------------------------------------------------
//    Checksums
//---------------------------------------------------------
class IpXsum 
{
    uint16 value;

    public:

    inline IpXsum(uint16 s = 0) : value(s) {}

    void calculate(uint16* pData, uint32 nShort);
    void calculateB(uint16* pData, uint32 nByteLength);

    inline IpXsum(const IpXsum& x) 
    {
        value = x.value;
    }

    inline const IpXsum& operator=(const IpXsum& x) 
    {
        value = x.value;
        return *this;
    }

    inline bool operator==(uint16 x) const 
    {
        return value == x;
    }

    inline bool operator!=(uint16 x) const 
    {
        return value != x;
    }

    inline void operator+=(const IpXsum& x) 
    {
        register uint32 l = value + x.value;
        l += (uint16)(l>>16);
        value = (uint16)l;
    }

    inline void operator+=(uint16 x) 
    {
        register uint32 l = value + x;
        l += (uint16)(l>>16);
        value = (uint16)l;
    }
    inline IpXsum operator+(const IpXsum& x) const 
    {
        register uint32 l = value + x.value;
        l += (uint16)(l>>16);
        return IpXsum((uint16)l);
    }

#if 0
    inline IpXsum operator+(uint16 x) const 
    {
        register uint16 l = value + x;
        l += (uint16)(l>>16);
        return IpXsum((uint16)l);
    }
#endif

    inline IpXsum operator~() const 
    {
        return IpXsum((uint16)~value);
    }

    inline uint16 swap() const 
    {
        return (uint16)((value>>8)|(value<<8));
    }

    inline operator uint16()  const 
    {
        return (value);
    }

    private:

    uint16 makeXsum(uint16* pData, uint16 nShortLength);
    uint16 makeXsumB(uint16* pData, uint16 nByteLength);

};

class StreamIO
{
	public :
	typedef uint8* iterator;

	enum SeekFlags { SeekSet, SeekCur, SeekEnd };
	StreamIO();
	void init(void* p, unsigned n);
	inline iterator begin()
	{
		return m_base;
	}
	inline iterator end()
	{
		return m_limit;
	}
	inline iterator cptr()
	{
		return m_stream;
	}
	inline unsigned size()
	{
		return m_limit-m_base;
	}
	inline unsigned capacity()
	{
		return m_limit-m_base;
	}
    inline unsigned left()
    {
		return m_limit - m_stream;
    }
	inline bool isEOF()
	{
		return (m_stream < m_limit) ? false : true;
	}
	long seek(long offs, int flags);
	unsigned write(const void* buf, unsigned count);
	unsigned read(void* buf, unsigned count);

	private:

	uint8* m_base;
	uint8* m_stream;
	uint8* m_limit;

};

extern "C" void eclrBpHandlerStub();
extern "C" void eclrDiv0HandlerStub();
extern "C" void eclrBoundHandlerStub();
extern "C" void eclrStackHandlerStub();
typedef void(*BpHandlerStub)();
extern "C" void* eclrGetBpHandlerAddr();

#endif //#define __ECLR_H__
