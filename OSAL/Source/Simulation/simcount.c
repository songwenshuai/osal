/**************************************************************************************************
  Filename:       simcount.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <windows.h>

#include "OSAL.h"

 /*********************************************************************
  * GLOBAL VARIABLES
  */

LARGE_INTEGER count_start;
LARGE_INTEGER count_freq;

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @brief SysTickSetup
 **/

void SysTickSetup(void)
{
    // get count Frequency
    QueryPerformanceFrequency(&count_freq);

    // get initial time
    QueryPerformanceCounter(&count_start);
}

/**
 * @brief macMcuPrecisionCount (320us)
 *
 **/

uint32 macMcuPrecisionCount(void)
{
    LARGE_INTEGER count_end;

    //get end count
    QueryPerformanceCounter(&count_end);

    // get time (1000 000 us / 320us ) = 3125
    return (uint32)((count_end.QuadPart - count_start.QuadPart) / (count_freq.QuadPart / 3125.0));
}

/**
 * @brief get_second (s)
 **/
uint32 get_second(void)
{
    LARGE_INTEGER count_end;

    //get end count
    QueryPerformanceCounter(&count_end);

    // get time
    return (uint32)((count_end.QuadPart - count_start.QuadPart) / (count_freq.QuadPart / 1.0));
}

/**
 * @brief get_millisecond (ms)
 **/
uint32 get_millisecond(void)
{
    LARGE_INTEGER count_end;

    //get end count
    QueryPerformanceCounter(&count_end);

    // get time
    return (uint32)((count_end.QuadPart - count_start.QuadPart) / (count_freq.QuadPart / 1000.0));
}

/**
 * @brief get_microsecond (us)
 **/
uint32 get_microsecond(void)
{
    LARGE_INTEGER count_end;

    //get end count
    QueryPerformanceCounter(&count_end);

    // get time
    return (uint32)((count_end.QuadPart - count_start.QuadPart) / (count_freq.QuadPart / 1000000.0));
}
