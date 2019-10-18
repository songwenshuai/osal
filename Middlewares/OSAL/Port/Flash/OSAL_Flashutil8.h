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

#define NV_FLASH_BASE                     ((uint32)nvDataBuf)                      // Flash 

#define HAL_NV_START_ADDR                 NV_FLASH_BASE

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

extern void HalFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt);

extern void HalFlashWrite(uint16 addr, uint8 *buf, uint16 cnt);

extern void HalFlashErase(uint8 pg);

#endif

/*********************************************************************
*********************************************************************/
