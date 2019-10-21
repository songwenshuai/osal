/**************************************************************************************************
  Filename:       OSAL_Flashutil32.h
  Revised:        $Date: 2011-01-21 14:43:43 -0800 (Fri, 21 Jan 2011) $
  Revision:       $Revision: 24872 $

  Description:    This module defines the flashing utilities for the ARM CM3.
**************************************************************************************************/

#ifndef OSALFLASHUTIL32_H
#define OSALFLASHUTIL32_H

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

#define ZCD_NV_NWKKEY                     0x0082
#define ZCD_NV_NWK_ACTIVE_KEY_INFO        0x003A
#define ZCD_NV_NWK_ALTERN_KEY_INFO        0x003B

// Flash consists of 256 pages of 2 KB.
#define HAL_FLASH_PAGE_SIZE               2048
#define HAL_FLASH_WORD_SIZE               8

// Z-Stack uses flash pages for NV
#define HAL_NV_PAGE_CNT                   6
#define HAL_NV_PAGE_END                   (HAL_NV_PAGE_CNT - 1)                    // 0-5 six page
#define HAL_NV_PAGE_BEG                   (HAL_NV_PAGE_END - HAL_NV_PAGE_CNT + 1)

#define NV_FLASH_BASE                     ((uint32)nvDataBuf)                      // Flash 

#define HAL_NV_START_ADDR                 NV_FLASH_BASE

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

extern uint8 nvDataBuf[HAL_NV_PAGE_CNT][HAL_FLASH_PAGE_SIZE];

/*********************************************************************
 * FUNCTIONS
 */

extern void initFlash(void);

extern void flashErasePage( uint8 *addr );

extern void flashWrite( uint8 *addr, uint16 len, uint8 *buf );

#endif

/*********************************************************************
*********************************************************************/
