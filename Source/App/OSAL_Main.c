/**************************************************************************************************
  Filename:       main.c
  Revised:        $Date:  $
  Revision:       $Revision:  $

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

/**
 * @brief main
 **/

int main(int argc, char **argv)
{
    // Turn off interrupts
    osal_int_disable(INTS_ALL);

    // Initialize the operating system
    osal_init_system();

    // Allow interrupts
    osal_int_enable(INTS_ALL);

    // Tick Init
    SysTickSetup();

    // No Return from here
    osal_start_system();

    return 0;
}
