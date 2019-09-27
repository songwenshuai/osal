/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#ifdef STM32L496xx
#include "stm32l4xx_hal.h"
#endif

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#endif

#ifdef _WIN32
#include "Windows.h"
#endif

#include "OSAL.h"
#include "OSAL_Clock.h"

/*********************************************************************
 * MACROS
 */

#define TICK_IN_MS 1 /* 1 millisecond */ 

/*********************************************************************
 * GLOBAL VARIABLES
 */

#ifdef STM32L496xx
extern __IO uint32_t uwTick;
extern uint32_t uwTickFreq;  /* 1KHz */
#endif

#ifdef STM32F103xB
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;  /* 1KHz */
#endif

/*********************************************************************
 * EXTERN FUNCTIONS
 */

extern int putc(int ch);

 /*********************************************************************
  * FUNCTIONS
  */

/***************************************************************************************************
 * @fn      SysTickIntEnable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntEnable(void)
{
#ifndef _WIN32
    HAL_ResumeTick();
#endif
}


/***************************************************************************************************
 * @fn      SysTickIntDisable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntDisable(void)
{
#ifndef _WIN32
    HAL_SuspendTick();
#endif
}

/*******************************************************************************
 * @fn          halSleep
 *
 * @brief       This function is called from the OSAL task loop using and
 *              existing OSAL interface. It determines if an OSAL timer is
 *              pending, in which case it sets up the RTC to wake the device
 *              for that event, and puts the device to sleep. The OSAL timers
 *              are adjusted upon wake in case the device was awoken sooner due
 *              to some other interrupt. If no OSAL timer event is pending,
 *              then the device is put to sleep.
 *
 *              Note: Presently, only CM3 is powered down.
 *
 * input parameters
 *
 * @param       osal_timeout - Next OSAL timer timeout, in msec.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halSleep( uint32 osal_timeout )
{
    //Sleep the task for the specified duration
#ifndef _WIN32
    HAL_Delay(osal_timeout);
#else
    Sleep(osal_timeout);
#endif
}

/***************************************************************************************************
 * @fn      This function is called to increment  a global variable "uwTick"
 *          used as application time base.
 *
 * @note    In the default implementation, this variable is incremented each 1ms
 *          in SysTick ISR.
 * 
 * @note This function is declared as __weak to be overwritten in case of other
 *      implementations in user file.
 * 
 * @brief   The Systick Interrupt module
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void HAL_IncTick(void)
{
#ifndef _WIN32
  uwTick += uwTickFreq;

  /* Update OSAL timer and clock */
  osalAdjustTimer(TICK_IN_MS);
#endif
}

/***************************************************************************************************
 * @fn      _putchar
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void _putchar(char character)
{
#ifndef _WIN32
  // send char to console etc.
  int c = (int)character;

  if (c == '\n')
    putc('\r');
  putc(c);
#endif
}
