#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// define the appropriated platform description 
#define PLATFORM_OSTYPE         EclrOS_Linux
  

// general header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>

//#define __LOGGING__
//#define __DEBUG__
//#define LOG_CONSOLE_LOGGING
//#define LOG_UDP_LOGGING

#define NO_SINGLE_FLOATING_POINT

//#define LOG_MSG_LOGGING
//#define LOG_DBG_LOGGING

// enable/disable platform threading support
#define PLATFORM_THREADING_SUPPORT

#define PLATFORM_BUILTIN_LINKER_SUPPORT
#define PLATFORM_DOWNLOADCHANGE_SUPPORT
#define PLATFORM_BREAKPOINT_SUPPORT
#define PLATFORM_DEBUG_SUPPORT
#define PLATFORM_CALLSTACK_SUPPORT

// enable/disable logicanalyser support
#define PLATFORM_LOGICANALYZER_SUPPORT

#include "etarget.h"
#endif //#ifndef __PLATFORM_H__
