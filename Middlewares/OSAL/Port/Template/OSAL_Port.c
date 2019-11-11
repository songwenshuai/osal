/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "OSAL.h"
#include "OSAL_Clock.h"

/*********************************************************************
 * MACROS
 */

#define TICK_IN_MS 1 /* 1 millisecond */ 

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERN FUNCTIONS
 */

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
void halSleep( uint32_t osal_timeout )
{

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

}

/***************************************************************************************************
 * @fn      OSAL_Init_Hook
 *
 * @brief   Hook Osal init function
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void OSAL_Init_Hook(void)
{

}
