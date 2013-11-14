//
// Logging Facility
//


// Logging Subsystems

#ifndef __LOG_H__
#define __LOG_H__

#define LF_ALWAYS                       0xFFFFFFFF
#define LF_BLOBHEAP                     0x00000001
#define LF_COMPILER                     0x00000002
#define LF_SYSTEM                       0x00000004
#define LF_THREADING                    0x00000008
#define LF_CMDDEAMON                    0x00000010
#define LF_METATABLE                    0x00000020
#define LF_METAFILE                     0x00000040
#define LF_METADATA                     0x00000080
#define LF_ASSEMBLY                     0x00000100
#define LF_APPDOMAIN                    0x00000200
#define LF_CONTROLLER                   0x00000400
#define LF_PCOSLIB                      0x00000800
#define LF_COMMUNICATION                0x00001000
#define LF_OPCUA                        0x00002000

#define LL_EVERYTHING  10   
#define LL_INFO6        9       // can be expected to generate 1,000,000 logs per small but not trival run
#define LL_INFO5        8       // can be expected to generate 100,000 logs per small but not trival run
#define LL_INFO4        7       // can be expected to generate 10,000 logs per small but not trival run
#define LL_INFO3        6       // can be expected to generate 1,000 logs per small but not trival run
#define LL_INFO2        5       // can be expected to generate 100 logs per small but not trival run
#define LL_INFO1        4       // can be expected to generate 10 logs per small but not trival run
#define LL_WARNING      3
#define LL_ERROR        2
#define LL_FATALERROR   1
#define LL_ALWAYS   	0		// impossible to turn off (log level never negative)


class ELogging
{
public:
    static void Init();
    static void Msg(const char *fmt, ... );
};

class PrintIO
{
    public:
    PrintIO() {}
    virtual ~PrintIO() {}
    virtual int put(char c) = 0;
};

typedef void (*LoggingCallback)(const char* pMsg);

#ifndef __LOGGING__

#define InitLogging(a, b)
#define ShutdownLogging()
#define LogFacilityOn(a)
#define LogFacilityOff(a)
#define LogSetLevel(a)
#define LoggingSetCallback(a)

#define DBG(x)
#define MSG(x)

#else

extern void InitLogging(uint32 facility, int level);
extern void ShutdownLogging();
extern void LogFacilityOn(uint32 facility);
extern void LogFacilityOff(uint32 facility);
extern void LogSetLevel(int level);

extern void LogSpew(uint32 facility, int level, const char *fmt, ... );
extern void MsgSpew(const char *fmt, ... );
extern void LoggingSetCallback(LoggingCallback pCallback);

extern const char*  __dbgFileName__;
extern int          __dbgLineNumber__;

#ifdef LOG_DBG_LOGGING
    #define DBG(x) { __dbgFileName__ = __FILE__; __dbgLineNumber__ = __LINE__; LogSpew x; }
#else
    #define DBG(x)
#endif

#ifdef LOG_MSG_LOGGING
    #define MSG(x) { MsgSpew x;}
#else
    #define MSG(x)
#endif

extern void vvsprintf(PrintIO& io, const char *fmt, va_list args);


#endif

#endif //__LOG_H__
