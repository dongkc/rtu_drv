#ifndef __TARGET_H__
#define __TARGET_H__

// target (platform/cpu) dependent declarations
#define PLATFORM_TARGET_ENDIANESS   TARGET_LITTLE_ENDIAN  
#define PLATFORM_PROCFAMILY         CpuType_FamArm   
#define PLATFORM_PROCTYPE           CpuType_Arm7    
#define PLATFORM_DATAMODEL          PLATFORM_DATAMODEL_ILP32
#define PLATFORM_STACKPOLICY        TARGET_STACKPOLICY_DOWNWARD

// compiler dependent declarations
typedef unsigned long long  uint64;
typedef long long           sint64;
#define PLATFORM_LONGLONG_SUPPORT

typedef unsigned int        uint32;
typedef signed   int        sint32;
typedef unsigned short int  uint16;
typedef signed   short int  sint16;
typedef unsigned char       uint8;
typedef signed   char       sint8;

#ifdef PLATFORM_PROCTYPE_ARMTHUMB
// thumb code execution on ARM targets
typedef uint16  TargetInstructionType;     // thumb instruction format 16 bit   
#define PLATFORM_PROCFAMILY     CpuType_FamArmThumb  
#define PLATFORM_PROCTYPE       CpuType_ArmThumb
// interwork support for calls from thumb code into native ARM functions
#define PLATFORM_PROCTYPE_ARMTHUMB_INTERWORK_SUPPORT

#else
// 32 bit ARM instructions
typedef uint32  TargetInstructionType;     // arm7 instruction format 32 bit   
#define PLATFORM_PROCFAMILY     CpuType_FamArm  
#define PLATFORM_PROCTYPE       CpuType_Arm7    

#endif

#define PLATFORM_PROGMODEL      ProgModel_Arm 

// macro to get the 'real' function pointer of a class member
// thiscall_p -> c++ member funtion poiner
typedef void(*__fp__)();                             // void f(void)
inline __fp__ get_class_method(void* p)
{
    return *((__fp__*)(p));
}
inline void set_class_method(void* p, __fp__ f)
{
    *((__fp__*)(p)) = f;
}

#endif //#ifndef __TARGET_H__
