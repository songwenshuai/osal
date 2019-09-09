/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <windows.h>
#include <stdio.h>
#include "OSAL.h"

 /*********************************************************************
  * FUNCTIONS
  */

#if defined(ewarm)
/**
 * @brief
 **/

uint32 CPUcpsid(void)
{
    //
    // Read PRIMASK and disable interrupts.
    //
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsid   i\n");

    //
    // "Warning[Pe940]: missing return statement at end of non-void function"
    // is suppressed here to avoid putting a "bx lr" in the inline assembly
    // above and a superfluous return statement here.
    //
#pragma diag_suppress = Pe940
}
#pragma diag_default = Pe940
#endif

#if defined(rvmdk) || defined(__ARMCC_VERSION)
/**
 * @brief
 **/

__asm uint32 CPUcpsid(void)
{
    //
    // Read PRIMASK and disable interrupts.
    //
    mrs r0, PRIMASK;
    cpsid i;
    bx lr
}
#endif

#if defined(_WIN32)
/**
 * @brief
 **/

uint32 CPUcpsid(void)
{
    return 0;
}
#endif

#if defined(ewarm)
/**
 * @brief
 **/

uint32 CPUcpsie(void)
{
    //
    // Read PRIMASK and enable interrupts.
    //
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsie   i\n");

    //
    // "Warning[Pe940]: missing return statement at end of non-void function"
    // is suppressed here to avoid putting a "bx lr" in the inline assembly
    // above and a superfluous return statement here.
    //
#pragma diag_suppress = Pe940
}
#pragma diag_default = Pe940
#endif

#if defined(rvmdk) || defined(__ARMCC_VERSION)
/**
 * @brief
 **/

__asm uint32 CPUcpsie(void)
{
    //
    // Read PRIMASK and enable interrupts.
    //
    mrs r0, PRIMASK;
    cpsie i;
    bx lr
}
#endif

#if defined(_WIN32)
/**
 * @brief
 **/

uint32 CPUcpsie(void)
{
    return 0;
}
#endif

#if defined(_WIN32)
/**
 * @brief
 **/

void SysTickIntEnable(void)
{
}
#endif

#if defined(_WIN32)
/**
 * @brief 
 **/

void SysTickIntDisable(void)
{
}
#endif

/**
 * @brief put char to console
 **/

void _putchar(char character)
{
    // send char to console etc.
    uint8 c = (uint8)character;

    if (c == '\n')
        putchar('\r');
    putchar(c);
}

/**
 * @brief Delay routine
 * @param[in] delay Amount of time for which the calling task should block
 **/

void Delay(uint32 delay)
{
    //Delay the task for the specified duration
    Sleep(delay);
}
