/**************************************************************************************************
  Filename:       OSAL_Flashutil8.h
  Revised:        $Date: 2011-01-21 14:43:43 -0800 (Fri, 21 Jan 2011) $
  Revision:       $Revision: 24872 $

  Description:    This module defines the flashing utilities for the ARM CM3.
**************************************************************************************************/

#ifndef OSALFLASHUTIL8_H
#define OSALFLASHUTIL8_H

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

// Z-Stack uses flash pages for NV
#define HAL_NV_PAGE_CNT                   6
#define HAL_NV_PAGE_END                   (HAL_NV_PAGE_CNT - 1)                    // 0-5 six page
#define HAL_NV_PAGE_BEG                   (HAL_NV_PAGE_END - HAL_NV_PAGE_CNT + 1)

#define NV_FLASH_BASE                     ((uint32)nvDataBuf)                      // Flash 

#define HAL_NV_START_ADDR                 NV_FLASH_BASE

/* OSAL NV implemented by internal flash pages. */

// Flash is partitioned into 8 banks of 32 KB or 16 pages.
#define HAL_FLASH_PAGE_PER_BANK        16

// Flash is constructed of 128 pages of 2 KB.
#define HAL_FLASH_PAGE_PHYS            2048

// SNV can use a larger logical page size to accomodate more or bigger items or extend lifetime.
#define HAL_FLASH_PAGE_SIZE            HAL_FLASH_PAGE_PHYS
#define HAL_FLASH_WORD_SIZE            4

// CODE banks get mapped into the XDATA range 8000-FFFF.
#define HAL_FLASH_PAGE_MAP             0x8000

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/**************************************************************************************************
 * @fn          HalFlashRead
 *
 * @brief       This function reads 'cnt' bytes from the internal flash.
 *
 * input parameters
 *
 * @param       pg - Valid HAL flash page number (ie < 128).
 * @param       offset - Valid offset into the page (so < HAL_NV_PAGE_SIZE and byte-aligned is ok).
 * @param       buf - Valid buffer space at least as big as the 'cnt' parameter.
 * @param       cnt - Valid number of bytes to read: a read cannot cross into the next 32KB bank.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt);

/**************************************************************************************************
 * @fn          HalFlashWrite
 *
 * @brief       This function reads 'cnt' bytes from the internal flash.
 *
 * input parameters
 *
 * @param       addr - Valid HAL flash write address: actual addr / 4 and quad-aligned.
 * @param       buf - Valid buffer space at least as big as the 'cnt' parameter.
 * @param       cnt - Valid number of bytes to write: a write cannot cross into the next 32KB bank.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashWrite(uint16 addr, uint8 *buf, uint16 cnt);

/**************************************************************************************************
 * @fn          HalFlashErase
 *
 * @brief       This function erases 'cnt' pages of the internal flash.
 *
 * input parameters
 *
 * @param       pg - Valid HAL flash page number (ie < 128) to erase.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashErase(uint8 pg);

#endif

/*********************************************************************
*********************************************************************/
