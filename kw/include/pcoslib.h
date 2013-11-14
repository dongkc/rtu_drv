#ifndef __PCOSLIB_H__
#define __PCOSLIB_H__
// pcoslib.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"
#include "eclrlib.h"
#include "metadata.h"
#include "appdomain.h"

class IecString : public FixedUtf8String
{
public:

};

class PcosDebugValue : public ClrDebugValue
{
public:
    inline bool IsIecString()
    {
        return IecString::IsString(GetTypeInfo());
    }
    bool GetValueRef(ClrAppDomain* pAppDomain, const char* pNameList[], int listSize);
    bool GetValueRef(ClrAppDomain* pAppDomain, const char* pPath);
};

class PcosPddProgInst : public eListNode<PcosPddProgInst>, public ClrAllocator
{
public:
    Object*     m_pProgram;
    ClrTypeDef* m_pProgTypeInfo;
    const char* m_pFieldName;
};


class PcosPdd
{
public:
    PcosPdd();
    ~PcosPdd();
	enum PddError
	{
		Err_NoError		= 0,
		Err_SymbolNotFound, 
		Err_BufferTooShort 
	};
    void Init(ClrAppDomain* pAppDomain);
    bool GetSymbolByAddr(PcosDebugValue& debugValue, char* pSymbol, unsigned symbolLen);
    inline int LastError()
	{
		return m_errorCode;
	}
    typedef eList<PcosPddProgInst>  ProgList;
    typedef ProgList::iterator      ProgListIter;

private:
    int findReference(ClrTypeDef* pTypeInfo, void* pInstance, int refPathCount);
    void initProgList();
    void clearProgList();

    ClrAppDomain* m_pAppDomain;
    Object* m_pValueObject;
    void*   m_pValueObjectEnd; 
    bool    m_isValueStatic;
    char*   m_arrayIndexBufferPtr;
    char*   m_arrayIndexBufferEnd;
    void*   m_pValue;
    ClrTypeDef* m_pValueTypeInfo;
    char  m_arrayIndexBuffer[64];
    static const int RefPathSize = 32;
    static const int MaxRefPathIndex = RefPathSize-2;
    const char* m_refPathTab[RefPathSize];
    const char* m_pPcosNameSpace;

    ProgList	m_progList;
	int			m_errorCode;

};

typedef struct _pcosdownloadstat
{
    // 0:
    uint32 m_contHeapUsed;
    // 4:
    uint32 m_contHeapMaxFree;
    // 8:
    uint32 m_applHeapUsed;
    // 12:
    uint32 m_applHeapMaxFree;
    // 16:
    uint32 m_dcMemoryUsed;
    // 20:
    uint32 m_dcTimeAll;
    // 24:
    uint32 m_dcTimeProlog;
    // 28:
    uint32 m_dcTimeIProlog;
    // 32:
    uint32 m_dcTimeChange;
    // 36:
    uint32 m_dcTimeIChange;
    // 40:
    uint32 m_dcTimeChangeIntern;
    // 44:
    uint32 m_dcTimeEpilog;
    // 48:
    uint32 m_dcTimeIEpilog;
    // 52:
    uint8  m_dcActions;
    // 53:
    uint8  m_dcRTVAllowed;
    // 54:
    uint8  m_dcTrials;
    // 55:
    uint8  m_unused1;
    // 56:
    uint32 m_unused2;
    // 60:
    uint32 m_unused3;
    // 64:
    uint32 m_unused4;
    // 68:
    uint32 m_unused5;
    // 72:
    uint32 m_unused6;
    // 76:
    uint32 m_unused7;
    // 80:
    uint32 m_unused8;
    // 84:
    uint32 m_unused9;
    // 88:
    uint32 m_unused10;
    // 92:
    uint32 m_unused11;
    // 96:
    uint32 m_unused12;
    // 100:
    uint32 m_unused13;
    // 104:
    uint32 m_unused14;
    // 108:
    uint32 m_unused15;
    // 112:
    uint32 m_unused16;
    // 116:
    uint32 m_unused17;
    // 120:
    uint32 m_unused18;
    // 124:
    uint32 m_unused19;

} PcosDownloadStat; // sizeof must be 128 !!

class CPcosDomain;
//class CUnmanagedCallback;
/// \defgroup PcosLib Pcos Library
/// \{

// public scope class for library implementation
// use pcoslib::init() and pcoslib::loadLibrary() during startup 
// to make the library accessible by the managed application.
class pcoslib
{
    // @Begin automatically generated code, do not modify !
public:
    enum LibVersion { Major=2, Minor=3, Build=0, Revision=0 };
    static bool init();
    static bool loadLibrary();
    static bool isCompatible(const EclrVersion& requestedVersion);
    static bool load(ClrAppDomain* pAppDomain);
    static bool unload(ClrAppDomain* pAppDomain);
    static unsigned s_modulHndl;
    // @End automatically generated code
    /// <summary>
    /// Error Codes
    /// <see cref="NativeErrorCodes"/>
    /// </summary>
    /// <remarks>
    /// Error numbers which are used with the current implementaion of the eCLR.
    /// Every error number must differentiate between the error qualifier and the error identifier.
    /// The error qualifier has a special meaning.
    /// The error identifier is simply a number which must be unique in the eCLR system.
    /// Each bit in the error qualifier has a special meaning.
    /// <para>Bit 15: "true" if it is an eCLR occupied error number.</para>
    /// <para>Bit 13-14: both bits are "false" if it is an error number else it is a warning level</para>
    /// <para>      false, true level 1,</para>
    /// <para>      true, false level 2,</para>
    /// <para>      true, true  level 3.</para>
    /// <para>Bit  8-12: reserved for future use, must be set to false.</para>
    /// <para>Bit  0- 8: Logging group enables 255 logging groups,</para>
    /// <para>      all bits false indicates that it is a warning or error message.</para>
    /// </remarks>
    enum ErrorCodes
    {
        /// <summary>
        /// Qualifier for error number when the watchdog error occurs.
        /// <see cref="WatchdogErrorIdentifier"/>
        /// </summary>
        WatchdogErrorQualifier = 0x8000,
        /// <summary>
        /// Identifier for error number when the watchdog error occurs.
        /// <see cref="WatchdogErrorQualifier"/>
        /// </summary>
        WatchdogErrorIdentifier = 0x0001,
        /// <summary>
        /// Qualifier for error number when the CPU over load error occurs.
        /// <see cref="MaximumCpuLoadExceededIdentifier"/>
        /// </summary>
        MaximumCpuLoadExceededQualifier = 0x8000,
        /// <summary>
        /// Identifier for error number when the CPU over load error occurs.
        /// <see cref="MaximumCpuLoadExceededQualifier"/>
        /// </summary>
        MaximumCpuLoadExceededIdentifier = 0x0002,
        /// <summary>
        /// File functionality returns an error
        /// </summary>
        FileErrorQualifier = 0x8000,
        /// <summary>
        /// File functionality returns an error
        /// </summary>
        FileErrorIdentifier = 0x0003,
        /// <summary>
        /// Identifier for error number when the CPU over load error occurs.
        /// <see cref="MaximumCpuLoadExceededQualifier"/>
        /// </summary>
        TestInfoErrorQualifier = 0x8000,
        /// <summary>
        /// Identifier for testing purposes.
        /// <see cref="TestInfoError"/>
        /// </summary>
        TestInfoError = 0x0003,
        /// <summary>
        /// Identifier for testing purposes error info containing string.
        /// <see cref="TestInfoSringError"/>
        /// </summary>
        TestInfoSringError = 0x0004,
        /// <summary>
        /// Appears on wrong string accessing
        /// </summary>
        StrErrPlcStopQualifier = 0x8000,
        /// <summary>
        /// Appears on wrong string accessing
        /// </summary>
        StrErrPlcStopIdentifier = 0x0010,
        /// <summary>
        /// String error
        /// </summary>
        StrErrNoPlcStopQualifier = 0x8000,
        /// <summary>
        /// String error
        /// </summary>
        StrErrNoPlcStopIdentifier = 0x0011,
        /// <summary>
        /// Output string is too short
        /// </summary>
        StrErrOutputTooShortQualifier = 0x8000,
        /// <summary>
        /// Output string is too short
        /// </summary>
        StrErrOutputTooShortIdentifier = 0x0012,
        /// <summary>
        /// Input string is too short
        /// </summary>
        StrErrInputTooShortQualifier = 0x8000,
        /// <summary>
        /// Input string is too short
        /// </summary>
        StrErrInputTooShortIdentifier = 0x0013,
        /// <summary>
        /// Invalid input parameter
        /// </summary>
        StrErrInvParQualifier = 0x8000,
        /// <summary>
        /// Invalid input parameter
        /// </summary>
        StrErrInvParIdentifier = 0x0014,
        /// <summary>
        /// Strings are identical
        /// </summary>
        StrErrIdenticalQualifier = 0x8000,
        /// <summary>
        /// Strings are identical
        /// </summary>
        StrErrIdenticalIdentifier = 0x0015,
        /// <summary>
        /// Invalid string comparison
        /// </summary>
        StrErrInvCmpQualifier = 0x8000,
        /// <summary>
        /// Invalid string comparison
        /// </summary>
        StrErrInvCmpIdentifier = 0x0016,
        /// <summary>
        /// Data type is not supported for string conversion
        /// </summary>
        StrErrInvDTypeQualifier = 0x8000,
        /// <summary>
        /// Data type is not supported for string conversion
        /// </summary>
        StrErrInvDTypeIdentifier = 0x0017,
        /// <summary>
        /// Error in format string
        /// </summary>
        StrErrFormatStrQualifier = 0x8000,
        /// <summary>
        /// Error in format string
        /// </summary>
        StrErrFormatStrIdentifier = 0x0018,
        /// <summary>
        /// Invalid input value for the format string
        /// </summary>
        StrErrInvValQualifier = 0x8000,
        /// <summary>
        /// Invalid input value for the format string
        /// </summary>
        StrErrInvValIdentifier = 0x0019,
        /// <summary>
        /// Error during string conversion
        /// </summary>
        StrErrStrCnvQualifier = 0x8000,
        /// <summary>
        /// Error during string conversion
        /// </summary>
        StrErrStrCnvIdentifier = 0x001A,
        /// <summary>
        /// Qualifier Saved retain checksum is not equal to project retain checksum.
        /// <see cref="NoWarmStarQual"/>
        /// </summary>
        NoWarmStarQual = 0x8000,
        /// <summary>
        /// Identifier Saved retain checksum is not equal to project retain checksum.
        /// <see cref="NoWarmStartID"/>
        /// </summary>
        NoWarmStartID = 0x001B,
        /// <summary>
        /// Maximum number of recordeable errors is reached
        /// </summary>
        MaxErrors = 0x001C
        // highest used ID is: 0x001F
        // if ID's are added, change comment for highest used ID in "enum NativeErrorCodes"
    };  // If additional error codes are added, this enum must be copied to pcoslib.cs

    /// <summary>
    /// Native Error Codes
    /// <see cref="ErrorCodes"/>
    /// </summary>
    /// <remarks>
    /// Error numbers which are used with the current implementaion of the eCLR.
    /// Every error number must differentiate between the error qualifier and the error identifier.
    /// The error qualifier has a special meaning.
    /// The error identifier is simply a number which must be unique in the eCLR system.
    /// Each bit in the error qualifier has a special meaning.
    /// <para>Bit 15: "true" if it is an eCLR occupied error number.</para>
    /// <para>Bit 13-14: both bits are "false" if it is an error number else it is a warning level</para>
    /// <para>      false, true level 1,</para>
    /// <para>      true, false level 2,</para>
    /// <para>      true, true  level 3.</para>
    /// <para>Bit  8-12: reserved for future use, must be set to false.</para>
    /// <para>Bit  0- 8: Logging group enables 255 logging groups,</para>
    /// <para>      all bits false indicates that it is a warning or error message.</para>
    /// </remarks>
    enum NativeErrorCodes
    {
        /// <summary>
        /// Qualifier: Runtime exception! division by zero.
        /// <see cref="Div0ID"/>
        /// </summary>
        Div0Qual = 0x8000,
        /// <summary>
        /// Identifier: Runtime exception! division by zero.
        /// <see cref="Div0Qual"/>
        /// </summary>
        Div0ID              = 1000,
        /// <summary>
        /// Qualifier: Runtime exception! array index out of range
        /// <see cref="OutOfRangeID"/>
        /// </summary>
        OutOfRangeQual = 0x8000,
        /// <summary>
        /// Identifier: Runtime exception! array index out of range
        /// <see cref="OutOfRangeQual"/>
        /// </summary>
        OutOfRangeID        = 1001,
        /// <summary>
        /// Qualifier: Runtime exception! stack overflow
        /// <see cref="StackOverflowQual"/>
        /// </summary>
        StackOverflowQual = 0x8000,
        /// <summary>
        /// Identifier: Runtime exception! stack overflow
        /// <see cref="StackOverflowQual"/>
        /// </summary>
        StackOverflowID     = 1002,
        /// <summary>
        /// Qualifier: Runtime exception! System error
        /// <see cref="SystemID"/>
        /// </summary>
        SystemQual = 0x8000,
        /// <summary>
        /// Identifier: Runtime exception! System error
        /// <see cref="SystemQual"/>
        /// </summary>
        SystemID            = 1003,
        /// <summary>
        /// Qualifier: The sum of static data size and dynamic data size exceeds the pyhsical available data memory size
        /// <see cref="OutOfMemoryQual"/>
        /// </summary>
        OutOfMemoryQual = 0x8000,
        /// <summary>
        /// Identifier: The sum of static data size and dynamic data size exceeds the pyhsical available data memory size
        /// <see cref="OutOfMemoryID"/>
        /// </summary>
        OutOfMemoryID		= 1004
        // highest used ID is: 1004
        // if ID's are added change comment for highest used ID in "enum ErrorCodes"
    };

    enum PcosType
    {
        TypeIecStringType = 0x100
    };


    struct IOGroup
    {
        // @Begin automatically generated code, do not modify native structs !
        SZArray* param;
        UInt32 addr;
        UInt32 size;
        // @End automatically generated code
    };

	struct Any
	{
		Void* pData;		// pointer to data
		UInt32 length;		// size of data
		UIntPtr rttHandle;	// runtime type handle
	};

    struct SysMem2
    {
        // same structure defined in pcoslib.cs 
        // take care of the consistency !!
        //    [FieldOffset(0)]
        UInt16 usTickPerSec; // Offset 0
        //    [FieldOffset(2)]
        // this value is used by the ProCOnOS scheduler to correct time intervals less than the projected intervals
        UInt16 SchedCorrection;
        //    [FieldOffset(4)]
        UInt32 MaxErr;      // Offset 4
        //    [FieldOffset(8)]
        UInt32 NumErr;      // Offset 8
        //    [FieldOffset(12)]
        Int16 maxTask;      // Offset 12
        //    [FieldOffset(14)]
        Int16 MaxSysTasks;  // Offset 14
        //    [FieldOffset(16)]
        Byte On;            // Offset 16
        //    [FieldOffset(17)]
        Byte Ld;            // Offset 17
        //    [FieldOffset(18)]
        Byte Force;         // Offset 18
        //    [FieldOffset(19)]
        Byte Bp;            // Offset 19
        //    [FieldOffset(20)]
        Byte Pf;            // Offset 20
        //    [FieldOffset(21)]
        Byte HaltOnStringError;          // Offset 21
        //    [FieldOffset(22)]
        UInt16 MinFrTm;     // Offset 22
        //    [FieldOffset(24)]
        UInt32 MinLdTm;     // Offset 24
        //    [FieldOffset(28)]
        UInt32 MaxLdTm;     // Offset 28
        //    [FieldOffset(32)]
        UInt32 LdCalcTm;    // Offset 32
        //    [FieldOffset(36)]
        UInt32 ScheduleInterval; // Offset 36

        UInt32 unused1;         // Offset 40
        UInt32 unused2;         // Offset 44
        UInt32 unused3;         // Offset 48
        UInt32 unused4;         // Offset 52
        UInt32 unused5;         // Offset 56
        UInt32 unused6;         // Offset 60
        // sizeof() = 64

    };

    /// <summary>
    /// Retain management header which lays at the beginning of the retain data
    /// </summary>
    struct RetainHeader
    {
        // @Begin automatically generated code, do not modify native structs !
        /// <summary>
        /// Retain CRC must is associated to the retain Layout of current project
        /// </summary>
        UInt32 retainCRC;
        /// <summary>
        /// maximum Size of the retain memory
        /// </summary>
        UInt32 bufferSize;
        /// <summary>
        /// Major version number of eCLR which last time wrote to the retain memory.
        /// </summary>
        Byte majorVersion;
        /// <summary>
        /// Minor version number of eCLR which last time wrote to the retain memory.
        /// </summary>
        Byte minorVersion;
        /// <summary>
        /// If retain memory is split into two buffers the currently used buffer number (1/0) is hold here
        /// </summary>
        Byte currentBuffer;
        /// <summary>
        /// Unused
        /// </summary>
        Byte unusedByte;
        /// <summary>
        /// Unused
        /// </summary>
        UInt32 unusedDWord;
        // @End automatically generated code
    };

    class M1_2 : public Object
    {
        // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
        static Void* GetMemPtr();
        // @End automatically generated code
        // insert additional private methods and member here ! 
        static SysMem2 zSysMem;

        inline static void incNumOfErrors()
        {
            zSysMem.NumErr++;
        }
        inline static void decNumOfErrors()
        {
            zSysMem.NumErr++;
        }
        inline static Int32 getNumOfErrors()
        {
            return zSysMem.NumErr;
        }
        inline static void resetNumOfErrors()
        {
            zSysMem.NumErr = (Int32)0;
        }
        inline static Int32 getMaxErrListSize()
        {
            return zSysMem.MaxErr;

        }
        inline static void setMaxErrListSize(int nErrorListSize)
        {
            zSysMem.MaxErr = (Int32)nErrorListSize;
        }

    };

    class M1_3 : public Object
    {
        // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
        public:
        static Void* GetMemPtr();
        // @End automatically generated code
        // insert additional private methods and member here ! 
        static PcosDownloadStat s_pcosDownloadStat;
    };

	class IecStringEx : public IecString
	{
	public:

	};

	class IecString80 : public IecStringEx
	{
	public:
		IecString80()
		{
			SetCapacity(80);
		}
	private:
		char data[81];
	};

    struct IecStringHead
    {
        // @Begin automatically generated code, do not modify native structs !
        Int16 Capacity;
        Int16 Length;
        // @End automatically generated code
        inline operator IecString* () 
        {   
            return (IecString*)this;
        }
    };

    class PcosDomain : public Object
    {
        // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
        static void Start();
        static void Stop();
        static void Abort(UInt16 p0);
        static void Restart(UInt16 p0);
        static void PreCycle();
        static void PostCycle();
        static UInt16 GetHandle();
        static Int16 PddWrite(Byte* p0, Void* p1, Int32 p2, UInt16 p3);
        static Int16 PddRead(Byte* p0, Void* p1, Int32 p2, UInt16 p3);
        static Int16 PddGetSym(Void* p0, UIntPtr p1, IecStringHead* p2);
        // @End automatically generated code
        // insert additional private methods and member here ! 

        static void Abort(unsigned excCode, void* pRetAddr, void* pMethFrame);
    };

    static CPcosDomain* getCurrentDomain();
    static CPcosDomain* CreateDomain(ClrAppDomain* pAppDomain);

    class Retain : public Object
    {
        // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
        /// <summary>
        /// Gets retain memory for the desired object (reference type)
        /// Gets retain memory for the desired array (reference type)
        /// This function is not thread save, which means that it is not reentrant.
        /// </summary>
        /// <param name="dataType">Type of object</param>
        /// <returns>A pointer to the requested Object is returned</returns>
        static Object* RGetObject(UIntPtr dataType);
        /// <summary>
        /// Gets retain memory for the desired array (reference type)
        /// This function is not thread save, which means that it is not reentrant.
        /// </summary>
        /// <param name="dataType">Type of array</param>
        /// <param name="numOfElements">Number of array elements</param>
        /// <returns>A pointer to the requested array is returned</returns>
        static Object* RGetArray(UIntPtr dataType, IntPtr numOfElements);
        /// <summary>
        /// Gets retain memory for the given value object (value type)
        /// Gets retain memory for the desired array (reference type)
        /// This function is not thread save, which means that it is not reentrant.
        /// </summary>
        /// <param name="dataType">Type of object</param>
        /// <returns>A pointer to the requested Object is returned</returns>
        static Void* RGetValue(UIntPtr dataType);
        // Allocates retain memory from the retantive heap without initialization 
        static Void* Alloc(Int32 p0, Int32 p1);
        // @End automatically generated code
        // insert additional private methods and member here ! 

        /// <summary>
        /// Announce retain memory to eCLR
        /// </summary>
        /// <param name="retainMemory">Base pointer to retain memory</param>
        /// <param name="size">size of availabel retain memory at all to an eCLR application</param>
        static bool AnnounceMemory(void* retainMemory, int size);

        static void ClearBuffer();
        static void ResetBufferPtr();
        static void SwapBufferPtr(bool forceRewriteHeader);

        static bool IsWarmStartPossible(uint32 crc32);
        static bool SetChecksum(uint32 crc32);

    private:  
        /// <summary>
        /// Base pointer to retain memory, usually points to Retain Header
        /// </summary>
        /// <see cref="RetainHeader"/>
        static RetainHeader* s_pHeader;
        /// <summary>
        /// Pointer to first retain data
        /// </summary>
        static char*        s_pBuffer;
        /// <summary>
        /// Pointer to free retain data
        /// </summary>
        static char*        s_pFree;
        /// <summary>
        /// Retain memory size which is left after retain header is created
        /// </summary>
        static unsigned     s_bufferSize;

        static void initPtr(unsigned bufferId);
        static Object* getObjInternal(unsigned size, unsigned align);
    };

    class TypeInfo : public Object
    {
        // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
        static Boolean IsIecString(UIntPtr p0);
        static Boolean IsArray(UIntPtr p0);
        static Boolean IsIecArray(UIntPtr p0);
        static Boolean GetIecArrayProperty(UIntPtr p0, UIntPtr* p1, Int32* p2, Int32* p3);
        // @End automatically generated code
        // insert additional private methods and member here !
        
        // used by pcospdd
        static ClrTypeDef* GetArrayElementType(UIntPtr p0);
        static ClrTypeDef* GetSubtype(UIntPtr p0, unsigned idx);
        static ClrTypeDef* GetTypeInfo(UIntPtr p0);
    };

    static bool pddGetValueBySymbol(const char* pPath, PcosDebugValue& debugValue);
    static int pddGetSymbolByAddr(PcosDebugValue& debugValue, char* pPath, unsigned pathLen);

}; // class pcoslib

extern "C" void pcosAbortHandlerStub(uint16 exccode);

#ifdef PLATFORM_CALLSTACK_SUPPORT
#ifndef PLATFORM_INTERPRETER_SUPPORT

// override the abort handler with an asm handler stub
#define META_PcosDomain_Abort_2 ((void (*)(UInt16))&pcosAbortHandlerStub)

#endif //#ifndef PLATFORM_INTERPRETER_SUPPORT

#endif //#ifdef PLATFORM_CALLSTACK_SUPPORT

#endif //#ifndef __PCOSLIB_H__
