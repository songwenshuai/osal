/**************************************************************************************************
  Filename:       OSAL_Config.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef OSAL_CONFIG_H
#define OSAL_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * OPTION
 */

#define USE_SYSTICK_IRQ                1

#define POWER_SAVING                   1

#define OSAL_CBTIMER_NUM_TASKS         1

/*********************************************************************
 * MACROS
 */

// Power conservation
#define OSAL_SET_CPU_INTO_SLEEP(timeout) halSleep(timeout);  /* Called from OSAL_PwrMgr */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CONFIG_H */
