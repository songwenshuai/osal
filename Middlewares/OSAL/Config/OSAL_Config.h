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

// Memory Allocation Heap
#define MAXMEMHEAP                 4096   /* Typically, 1.0-6.0K */
#define OSALMEM_IN_USE             0x8000
 //#define DPRINTF_OSALHEAPTRACE   1

/*********************************************************************
 * MACROS
 */

// ++ sws add start
#define ZCD_NV_NWKKEY                     0x0082
#define ZCD_NV_NWK_ACTIVE_KEY_INFO        0x003A
#define ZCD_NV_NWK_ALTERN_KEY_INFO        0x003B

// Flash is partitioned into 8 banks of 32 KB or 16 pages.
#define HAL_FLASH_PAGE_PER_BANK           16
// Flash consists of 256 pages of 2 KB.
#define HAL_FLASH_PAGE_SIZE               1024
#define HAL_FLASH_WORD_SIZE               4

// Z-Stack uses flash pages for NV
#define HAL_NV_PAGE_CNT                   6
#define HAL_NV_PAGE_END                   (HAL_NV_PAGE_CNT - 1)                    // 0-5 six page
#define HAL_NV_PAGE_BEG                   (HAL_NV_PAGE_END - HAL_NV_PAGE_CNT + 1)

// ++ sws add end

// Power conservation
#define OSAL_SET_CPU_INTO_SLEEP(timeout) halSleep(timeout);  /* Called from OSAL_PwrMgr */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CONFIG_H */
