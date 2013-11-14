#ifndef __ECLRLIB_H__
#define __ECLRLIB_H__
// eclrlib.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"
#include "blobheap.h"

class CAppFileStream;

/// \remarks Internal Test documentation
#define TEST_VERSION
#define SEND_BUFFER_SIZE 80
#define MAXIMUM_LIST_SIZE 100

/// \defgroup Corelib Core Libraries
/// \{


#define IS_HARD_ERROR 0x60FF0000

#ifdef PLATFORM_THREADING_SUPPORT
class CMutexImpl;
#endif // #ifdef PLATFORM_THREADING_SUPPORT

// public scope class for library implementation
// use eclrlib::init() and eclrlib::loadLibrary() during startup 
// to make the library accessible by the managed application.
class eclrlib
{
// @Begin automatically generated code, do not modify !
public:
enum LibVersion { Major=2, Minor=5, Build=0, Revision=0 };

///
/// if necessary, insert some initialization code for the eclrlib library here !
/// call eclrlib::init() somewhere before eclrlib::loadLibrary() is called.
/// loadLibrary() have to be called after the ClrContoller object is instanciated
/// for detailed information see the native library builder documentation
/// \return
///
static bool init();
///
/// This function is implemented in automatically generated "firmware module meta file"
/// Before using a firmware module this function must be called once. 
/// \return Returns true on success and false if an error has occured.
///
static bool loadLibrary();
///
/// This function is called by the eCLR if this library will be bound
/// to the application.
/// If no libraries could be found, the runtime aborts the loading process with linker-error
/// \param requestedVersion Version number of module which should be checked for compatibility \see EclrVersion
/// \return if this library fits to the requested interface "true" is returned. 
/// "false" forces the runtime to search for other libraries with the same name by calling isCompatible().
///
static bool isCompatible(const EclrVersion& requestedVersion);
///
/// This function is called by the eCLR when an application will be started.
/// At this point, the firmware library is able to initialize some internal objects.
/// \param pAppDomain Pointer to the domain object witch hosts the current application process.
/// \return "true" if the initialization is done successful else "false" is returned.
///
static bool load(ClrAppDomain* pAppDomain);
///
/// This function is called by the runtime if an application will be terminated.
/// At this point, the firmware library is able to finalize objects.
/// \param pAppDomain Pointer to the domain object witch hosts the current application process.
/// \return "true" if the finalization is done successful else "false" is returned.
///
static bool unload(ClrAppDomain* pAppDomain);
///
/// The implementaion of this variable is in the "firmware module meta data file" .
/// It Contains an eCLR managed module handle.
/// This handle is used to identify different modules.
/// Use this handle when generating error messages. \see ErrorManagement \see SetError
/// \attention The module handle is managed by the eCLR and it is not guaranteed that the handle
/// for a specific module will always be the same. The handle is only the same during runtime of the eCLR.
///
static unsigned s_modulHndl;
// @End automatically generated code

///                
/// \enum InfoTypeIdentifiers
/// An eCLR error message can contain more Information beside the UInt32 error message.
/// To handle such a message it is necessary that Multiprog and the eCLR interpret the additional information
/// in the same way. To make this possible, such an error must carry an info type identifier.
/// this identifier must be unique to eCLR and Multiprog.
///
enum InfoTypeIdentifiers
{
    // @Begin automatically generated code, do not modify native enums !
    InfoTypeIdentifiers_sizeof=2,               ///< Data type size of each enumerator elemet in number of bytes
    NoInfoAvailable=0x0,                        ///< Word type identifier for unused type identifier, must be != zero when the type identifier is used
    InfoString=0x101,                           ///< Pre defined type identifier for info being a string
    TestInfoType1=0x102,                        ///< Pre defined type for testing purposes
    Info2Strings=0x103                          ///< Pre defined type identifier for info being two strings
    // @End automatically generated code
};


enum FileSeekOrigin
{
    // @Begin automatically generated code, do not modify native enums !
    FileSeekOrigin_sizeof=2,
    Begin=0,
    Current=1,
    End=2
    // @End automatically generated code
};

enum FileMode
{
    // @Begin automatically generated code, do not modify native enums !
    FileMode_sizeof=2,
    Create=0,
    Open=1
    // @End automatically generated code
};


///
/// \class GCHeap
/// Garbage Collected Heap
/// If objects, arrays or strings need to be allocated by the Application they are allocated from the
/// garbage collected heap.
///
class GCHeap : public Object
{
// @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
public:
    ///
    /// \fn static Object* newobj(UIntPtr type)
    /// In C# private method of class GCHeap to create a new object which is derivated from Object \see Object.
    /// \param type Type of new object 
    /// \return Returns a pointer to the new object. 
    ///
    static Object* newobj(UIntPtr type);
    ///
    /// In C# private method of class GCHeap to create a new array.
    /// \param type Type of new object
    /// \param num_element number of objects
    /// \return Returns a pointer to the new array object. 
    ///
    static Object* newarr(UIntPtr type, IntPtr num_element);
    ///
    /// In C# private method of class GCHeap to create a new string.
    /// \param length Size of the string which will be created
    /// \return Returns a pointer to the new string object. 
    ///
    static String* newstr(UInt16 length);
    /// \cond INTERNAL_DOCUMENTATION_INCLUDE

    // @End automatically generated code
    // insert additional private methods and member here !

private:
    static Object* newobjInternal(ClrType* pType, unsigned size);
 
};

class Helper : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Object* isinst(Object* p0, UIntPtr p1);
    static Object* castclass(Object* p0, UIntPtr p1);
	static void Copy(Void* p0, Void* p1, UInt32 p2);
	static void Move(Void* p0, Void* p1, UInt32 p2);
    static void Clear(Void* p0, UIntPtr p1);
    static void div0();
    static void bound();
    static void stkchk(UInt16 p0);
    static Void* getbphandler();
    static UInt16 GetType(UIntPtr p0);
	static void printf(Byte* buffer);
	static void ItoS(Byte* buffer, Byte* format, Int32 value);
	static void ItoS(Byte* buffer, Byte* format, Int64 value);
	static void ItoS(Byte* buffer, Byte* format, Int32 value, UInt16 type);
	static void ItoS(Byte* buffer, Byte* format, Int64 value, UInt16 type);
	static void FtoS(Byte* buffer, Byte* format, UInt32 value);
	static void DtoS(Byte* buffer, Byte* format, UInt64 value);
    static Int32 StoI(Byte* buffer, Byte* format);
	static UInt32 StoF(Byte* buffer, Byte* format);
	static UInt64 StoD(Byte* buffer, Byte* format);
	static UInt32 StoF2(Byte* buffer, Boolean* bConversionPerformed);
	static UInt64 StoD2(Byte* buffer, Boolean* bConversionPerformed);
	static Byte* StrStr(Byte* buffer, Byte* search);
	static Boolean IsLittleEndian();
	static Int32 StrCmp(Byte* string1, Byte* string2);
    static void DTNow(mscorlib::DateTime* dateTime);
    static void DTtoS(Byte* buffer, Byte* format, mscorlib::DateTime* dateTime);
	static UInt16 GetHandle();
    static Boolean IsEnum(UIntPtr p0);
    static UInt16 GetEnumUnderlyingType(UIntPtr p0);

    // @End automatically generated code
    // insert additional private methods and member here ! 
};


/// Common error message structure, internal in eCLR applicable for all error messages which will displayed on Multiprog side.
struct ErrorMessage
{
    // @Begin automatically generated code, do not modify native structs !
    //UInt32 __test;     ///< Error number consists of error qualifier in high word and error identifier in low word. See eclrlib::ErrorCodes for more information.                
    UInt32 uErrorNumber;     ///< Error number consists of error qualifier in high word and error identifier in low word. See eclrlib::ErrorCodes for more information.                
    UInt32 uErrorTimeStamp;  ///< Time stamp                                                                                                                                           
    UInt16 wErrorInfoType;   ///< Error type identifier, specifies a typ with a number which must be consistent to the number which Multiprog expects. See eclrlib::InfoTypeIdentifiers
    UInt16 wErrorModul;      ///< Module where the error occured                                                                                                                       
    char*  pInfo;             ///< pointer to the info data with the specified type in uErrorInfoType                                                                                   
    // @End automatically generated code
};

enum ErrorCodes
{
    /// <summary>
    /// Qualifier for error number generated by the eCLR kernel
    /// </summary>
    EclrQualifier = 0x8000,
	/// <summary>
	/// Application image is invalid
	/// </summary>
	AppImageError          = 1,
	/// <summary>
	/// Application image could not be bound to the kernel firmware profile
	/// </summary>
	AppBindError            = 2,
	/// <summary>
	/// a firmware libary module could not be loaded
	/// </summary>
	AppFirmwareLoadError    = 3,
	/// <summary>
	/// 
	/// </summary>
	Reserved                = 4,
	/// <summary>
	/// Maximum number of recordable errors reached
	/// </summary>
	MaxErrors               = 5
};

///
/// \class ErrorManagement
/// Base class of error management
/// The error management is splitted into one base class ErrorManagement
/// and one class for logging error messages and one for returning an error 
/// message to Multiprog.
/// \see ErrorLog
/// \see CReturnErrorMessage
/// \note An error message is not only for reporting errors.
/// Additionally warnings and loggin events are supported by the same handling.
///
class ErrorManagement : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// \cond INTERNAL_TEST_DOCUMENTATION_INCLUDE
    ErrorManagement();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 

    ~ErrorManagement()
    {
        m_pErrorTypeInterpreter = NULL;
    }
    /// \endcond

    /// Common error info head structure, internal in eCLR applicable for all error messages which carry additional informatíon.
    struct ErrorInfoHead
    {
        uint16 uSize;                               ///< Size of additional error information.
        uint16 uUnused;                             ///< Currently not used but necessary for alignment.
    };

    /// \cond INTERNAL_TEST_DOCUMENTATION_INCLUDE
    struct TestErrorInforTypeStruct
    {
        uint16 wTypeInfo1;
        uint16 wDummy1;
        uint16 wTypeInfo2;
        uint16 wDummy2;
    };
    /// \endcond

    /// \cond INTERNAL_DOCUMENTATION_INCLUDE
    static void* m_pErrorTypeInterpreter;           ///< For future use, currently no meaning
    static char m_SendBufferArray[SEND_BUFFER_SIZE*sizeof(uint16)]; ///< Buffer which is will be returned to Multiprog
#ifdef PLATFORM_THREADING_SUPPORT
    static void EnterMutex();
    static void LeaveMutex();
    static CMutexImpl* s_pMutex;
#endif // #ifdef PLATFORM_THREADING_SUPPORT

    ///
    /// \param pzErrorMessageContainingInfo 
    /// \param wInfoType Type identifier of additional info. \see eclrlib::InfoTypeIdentifiers
    /// \param pnInfoTypeSize Size of additional info.
    /// \return Pointer to additional info
    ///
    static void* GetInfoType(ErrorMessage* pzErrorMessageContainingInfo, uint16 wInfoType, int* pnInfoTypeSize);
    /// \endcond

};

// pzErrorMessageContainingInfo Pointer to a message which contains an error message with info
// Type of info
// Size of info
// returns a pointer to type information


///
/// \class ErrorLog
/// Logging errors in error list.\n
/// CLoggingErrorMessage distinguishes between errors, warnings and event logs.
/// The only differentiation attribute is the error message qualifier. \see eclrlib::ErrorCodes
/// Errors are logged in a chained list. The main function of CLoggingErrorMessage  
/// is to manage this list. \n
/// Function of adding single messages, deleting the complete list and reading logged messages into a 
/// given buffer is supported by this class.
///
class ErrorLog : public Object
{

    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    /// \cond INTERNAL_TEST_DOCUMENTATION_INCLUDE

    ///
    /// ErrorLog constructor 
    ///
    ErrorLog();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 

    ///
    /// ErrorLog destructor 
    ///
    ~ErrorLog();
    struct ErrorMessageChain
    {
        ErrorMessage zErrorMessage;                 ///< Error Message
        ErrorMessageChain *pzPreviousErrorMessageInChain; ///< Pointer to previous element in error message chain
        ErrorMessageChain *pzNextErrorMessage;      ///< Pointer to next element in message chain
    }; 
    /// \endcond

    ///
    /// Logs an error without additional information in error list
    /// \param errorQualifier Error qualifier (the high word of the error number)
    /// \param errorIdentifier Error identifier (the low word of the error number)
    /// \param uModuleHandle eCLR managed module handle where the error occured
    /// \see eclrlib::ErrorCodes
    ///
    static void SetError(UInt16 errorQualifier, UInt16 errorIdentifier, UInt16 uModuleHandle);
    
    ///
    /// Logs an error with additional information in error list
    /// \param errorQualifier Error qualifier
    /// \param errorIdentifier Error number
    /// \param uModuleHandle eCLR managed module handle where the error occured
    /// \param errorType Info type identifier
    /// \param errorInfo Pointer to error information
    /// \see eclrlib::ErrorCodes
    /// \see eclrlib::InfoTypeIdentifiers
    ///
    static void SetError(UInt16 errorQualifier, UInt16 errorIdentifier, UInt16 uModuleHandle, UInt16 errorType, void *errorInfo, UInt16 infoSize);

    ///
    /// Clears all logged errors and frees the associated memory blocks
    ///
    static void ClearErrorLog();
    
    ///
    /// Read Error Logs into a Buffer
    /// \param m_nSendBufferSize Size of Buffer for Error Logs
    /// \param m_pcSendBuffer Buffer for Error Logs
    /// \param bSendMore true if not all error logs fit into Buffer, false if all error logs read
    /// \return Returns the Number of Bytes read
    ///
    static int ReadErrorLogs(int m_nSendBufferSize, char* m_pcSendBuffer, bool *bSendMore);

    /// JK 2005.11.03
    /// Returns Error Log Pointer
    ///
    static inline struct ErrorMessageChain* GetFirstErrorLogEntry()
    {   return(m_pzErrorMessageChainAnchor);
    }


    ///
    /// Sets the maximum number of error messages. When this number of error messages is reached,
    /// the oldest message is deleted and the new message is appended to the beginning of the list.
    /// so a ring buffer for error messages is realized.
    /// \param nErrorListSize Maximun number of error logs
    ///
    static void SetMaximumErrorListSize(int nErrorListSize);

    ///
    /// \if INTERNAL_DOCUMENTATION_INCLUDE 
    /// Returns the maximum number of error messages.
    /// \param none
    /// \return Maximum number of errors which can be logged
    /// \endif
    ///
    static Int32 GetMaximumErrorListSize();

    ///
    /// Get the number of available errors.
    /// \return Number of currently available errors.
    ///
    static Int32 GetCurrentNumberOfErrors();
    
    ///
    /// \if INTERNAL_DOCUMENTATION_INCLUDE 
    /// Get information on a recorded error.
    /// \param Position of error in error stack
    /// \param Pointer to error message structure
    /// \return 1 if an error could be found else 0
    /// \endif
    ///
    static Int32 GetError(Int32 nErrorNumber, ErrorMessage* zErrMsg);

    ///
    /// Indicates that a none volatile error message is recorded
    /// and the available space for error messages is utilized which leads 
    /// to stop recording error messages
    ///
    static bool ms_bErrorLogginIsStopped;
    ///
    /// Indicates that a none volatile error message is recorded
    ///
    static bool ms_bErrorIsRecorded;

    /// \cond INTERNAL_TEST_DOCUMENTATION_INCLUDE
    static ErrorMessageChain *ms_zNextToReadMore;           ///< Pointer to next element when not all errors fit into a given buffer.
    static int m_nNumberOfChainedErrorMessages;             ///< Number of currently chained error messages
    static ErrorMessageChain *m_pzErrorMessageChainAnchor;  ///< Error Message Chain Anchor
    static ErrorMessageChain *m_pzErrorMessageChainTail;    ///< Error Message Chain Tail
    static int m_nMaximumErrorListSize;                     ///< Maximum Size of error list in numbers of errors
    /// \endcond

private:
    /// Common error info head structure, internal in eCLR applicable for all error messages which carry additional informatíon.
    struct ErrorInfoHead
    {
        uint16 uSize;                               ///< Size of additional error information.
        uint16 uUnused;                             ///< Currently not used but necessary for alignment.
    };


    ///
    /// Sets an error with additional information
    /// \param errorQualifier Which type of error message is it
    /// \param errorIdentifier Error Identifier
    /// \param uModuleHandle eCLR managed module handle where the error occured
    /// \param errorType additional error Type
    /// \param *errorInfo Pointer to additional Error Information
    ///
    static void ErrorWithInfo(UInt16 errorQualifier, UInt16 errorIdentifier, uint16 uModuleHandle, InfoTypeIdentifiers errorType, void *errorInfo, UInt16 errorInfoSize);
    static void SaveErrorInfo(ErrorMessage* errorMessageContainingInfo, uint32 errorInfoType, void *pErrorInfo, UInt16 errorInfoSize);
};

class Environment : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static UInt32 GetMicroTickCount();
    static Boolean IsThreading();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class TypeInfo : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static UInt16 GetType(UIntPtr p0);
    static UIntPtr GetFieldInfo(UIntPtr p0, Int32 p1);
    static Int32 GetSize(UIntPtr p0);
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class FieldInfo : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static UIntPtr GetTypeInfo(UIntPtr p0);
    static Int32 GetOffset(UIntPtr p0);
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class DcgNonGeneric : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    Boolean Prolog(Object* p0);
    void Change(Object* p0);
    void Epilog(Object* p0);
    // @End automatically generated code
    // insert additional private methods and member here !
};

class IDcgNonGeneric
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

#ifndef NO_SINGLE_FLOATING_POINT
#define FLOATARG_OPTIMIZED
#endif //NO_SINGLE_FLOATING_POINT

class Emu : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Int32 add(Int32 p0, Int32 p1);
    static Int64 addl(Int64 p0, Int64 p1);
    static Int32 sub(Int32 p0, Int32 p1);
    static Int64 subl(Int64 p0, Int64 p1);
    static Int32 mul(Int32 p0, Int32 p1);
    static UInt32 mulu(UInt32 p0, UInt32 p1);
    static Int32 div(Int32 p0, Int32 p1);
    static UInt32 divu(UInt32 p0, UInt32 p1);
    static Int32 rem(Int32 p0, Int32 p1);
    static UInt32 remu(UInt32 p0, UInt32 p1);
    static Int64 divl(Int64 p0, Int64 p1);
    static UInt64 divul(UInt64 p0, UInt64 p1);
    static Int64 reml(Int64 p0, Int64 p1);
    static UInt64 remul(UInt64 p0, UInt64 p1);
    static Int64 mull(Int64 p0, Int64 p1);
    static UInt64 mulul(UInt64 p0, UInt64 p1);
    static Int32 neg(Int32 p0);
    static Int64 negl(Int64 p0);
    static UInt32 shl(UInt32 p0, UInt16 p1);
    static UInt64 shll(UInt64 p0, UInt16 p1);
    static Int32 shr(Int32 p0, UInt16 p1);
    static Int64 shrl(Int64 p0, UInt16 p1);
    static UInt32 shru(UInt32 p0, UInt16 p1);
    static UInt64 shrul(UInt64 p0, UInt16 p1);
    static Boolean ceq(Int32 p0, Int32 p1);
    static Boolean ceql(Int64 p0, Int64 p1);
    static Boolean cgt(Int32 p0, Int32 p1);
    static Boolean cgtu(UInt32 p0, UInt32 p1);
    static Boolean cgtl(Int64 p0, Int64 p1);
    static Boolean cgtul(UInt64 p0, UInt64 p1);
    static Boolean clt(Int32 p0, Int32 p1);
    static Boolean cltu(UInt32 p0, UInt32 p1);
    static Boolean cltl(Int64 p0, Int64 p1);
    static Boolean cltul(UInt64 p0, UInt64 p1);
    static UInt32 addf(UInt32 p0, UInt32 p1);
    static UInt32 subf(UInt32 p0, UInt32 p1);
    static UInt32 mulf(UInt32 p0, UInt32 p1);
    static UInt32 divf(UInt32 p0, UInt32 p1);
    static UInt32 remf(UInt32 p0, UInt32 p1);
    static UInt32 negf(UInt32 p0);
    static UInt64 addd(UInt64 p0, UInt64 p1);
    static UInt64 subd(UInt64 p0, UInt64 p1);
    static UInt64 muld(UInt64 p0, UInt64 p1);
    static UInt64 divd(UInt64 p0, UInt64 p1);
    static UInt64 remd(UInt64 p0, UInt64 p1);
    static UInt64 negd(UInt64 p0);
    static Boolean ceqf(UInt32 p0, UInt32 p1);
    static Boolean cgtf(UInt32 p0, UInt32 p1);
    static Boolean cltf(UInt32 p0, UInt32 p1);
    static Boolean ceqd(UInt64 p0, UInt64 p1);
    static Boolean cgtd(UInt64 p0, UInt64 p1);
    static Boolean cltd(UInt64 p0, UInt64 p1);
    static UInt32 ci4f(Int32 p0);
    static UInt32 cu4f(UInt32 p0);
    static UInt32 ci8f(Int64 p0);
    static UInt32 cu8f(UInt64 p0);
    static Int32 cfi4(UInt32 p0);
    static UInt32 cfu4(UInt32 p0);
    static Int64 cfi8(UInt32 p0);
    static UInt64 cfu8(UInt32 p0);
    static UInt64 ci4d(Int32 p0);
    static UInt64 cu4d(UInt32 p0);
    static UInt64 ci8d(Int64 p0);
    static UInt64 cu8d(UInt64 p0);
    static Int32 cdi4(UInt64 p0);
    static UInt32 cdu4(UInt64 p0);
    static Int64 cdi8(UInt64 p0);
    static UInt64 cdu8(UInt64 p0);
    static UInt64 cfd(UInt32 p0);
    static UInt32 cdf(UInt64 p0);
    static UInt32 Sin(UInt32 p0);
    static UInt32 Cos(UInt32 p0);
    static UInt32 Tan(UInt32 p0);
    static UInt32 Asin(UInt32 p0);
    static UInt32 Acos(UInt32 p0);
    static UInt32 Atan(UInt32 p0);
    static UInt32 Exp(UInt32 p0);
    static UInt32 Log(UInt32 p0);
    static UInt32 Log10(UInt32 p0);
    static UInt32 Sqrt(UInt32 p0);
    static UInt32 Pow(UInt32 p0, UInt32 p1);
    static UInt32 PowU4(UInt32 p0, UInt32 p1);
    static UInt32 PowI4(UInt32 p0, Int32 p1);
    static UInt32 U4mulF(UInt32 p0, UInt32 p1);
    static UInt32 U4divF(UInt32 p0, UInt32 p1);
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class File : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    File();
    void ctor();
	void Finalize();
    Boolean Open(Byte* p0, UInt16 p1);
    Int32 Read(Void* p0, UInt32 p1, UInt32 p2);
    Int32 Write(Void* p0, UInt32 p1, UInt32 p2);
    Int32 Seek(Int32 p0, UInt16 p1);
	Int32 Tell();
    Boolean Close();
    static Boolean Delete(Byte* p0);
    Byte* FileName;
    UInt16 TableEntry;
    Boolean IsUsed;
    Boolean IsReserved;
    // @End automatically generated code
    // insert additional private methods and member here !
private:
    EclrFile* fileFileHandle;
	CAppFileStream* fileStream;
};

}; // class eclrlib


typedef eclrlib::ErrorManagement ErrorManagement;


#ifdef PLATFORM_CALLSTACK_SUPPORT

// override the abort handler with an asm handler stub
#define META_Helper_div0_5 ((void (*)())&eclrDiv0HandlerStub)
#define META_Helper_bound_6 ((void (*)())&eclrBoundHandlerStub)

#endif //#ifdef PLATFORM_CALLSTACK_SUPPORT

#endif //#ifndef __ECLRLIB_H__
