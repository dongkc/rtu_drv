//=============================================================================
//
//                      Copyright 2009 by KW Software GmbH                     
//                                                                             
//                      KW Software GmbH                                       
//                      Lagesche Str. 32                                       
//                      D-32657 Lemgo                                          
//                      Germany                                                
//                                                                             
//-----------------------------------------------------------------------------
// Module Name: eCLR Environment interface implementation
//-----------------------------------------------------------------------------
// Author: Xiangyang Ren                           
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
// 	Jun. 15th, 2009 by X. Ren - created
//
// ============================================================================

// ----------------------------------------------------------------------------
// Pragma�s
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes 
// ----------------------------------------------------------------------------
#include "eclrEnvironment.h"

// ----------------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// User Type Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Data Object Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Method Declarations
// ----------------------------------------------------------------------------

// ============================================================================
// Start of Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Methods
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Global Methods
// ----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetTickCount
//
// Description: Retrieves the millisecond count.
//
// Return:      uint32 - tick count in milliseconds
//
// Parameters:  None
//
// Note:        STATIC method
// ---------------------------------------------------------------------------

uint32 EclrEnvironment::GetTickCount() //resulotion is millisecond
{
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	uint32 tick = (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);
	return tick;
}


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetMicroTickCount
//
// Description: Initializes the embedded CLR environment class.
//
// Return:      uint32 - tick count in microseconds
//
// Parameters:  None
//
// Note:        STATIC method
// ---------------------------------------------------------------------------

uint32 EclrEnvironment::GetMicroTickCount() // resolution is microsecond
{
    struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	uint32 tick = (tp.tv_sec * 1000000) + (tp.tv_nsec / 1000);
	return tick;
}


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::FlushCache
//
// Description: 
//
// Return:      Nothing
//
// Parameters:  None
//
// Note:        STATIC method 
//              However should be implemented in eclr_main module since it 
//              is strongly dependent on CPU and OS type.
// ---------------------------------------------------------------------------

//void EclrEnvironment::FlushCache(void* pData, unsigned length)
//{
//   return;
//}   


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetCurrentDateTime
//
// Description: Gets the current data and time.
//
// Return:      bool - true = retrieved successfully
//                     false = could not retrieve data or time
//
// Parameters:  dateTime - DateTime - structure to poulate with current time
//                                    and date.
//
// Note:        STATIC method
// ---------------------------------------------------------------------------

bool EclrEnvironment::GetCurrentDateTime(DateTime& dateTime)
{
	bool result = false;
   time_t currentTime;
	time(&currentTime);

	struct tm* currentLocalTime = localtime(&currentTime);

	if (currentLocalTime != NULL)
	{
		dateTime.year = currentLocalTime->tm_year + 1900;
		dateTime.month = currentLocalTime->tm_mon + 1;
		dateTime.day = currentLocalTime->tm_mday;
		dateTime.hour = currentLocalTime->tm_hour;
		dateTime.minute = currentLocalTime->tm_min;
		dateTime.second = currentLocalTime->tm_sec;
		dateTime.millisecond = 0;
      result = true;
	}

    return result;
}


// ----------------------------------------------------------------------------
// End of Implementation
// ============================================================================
