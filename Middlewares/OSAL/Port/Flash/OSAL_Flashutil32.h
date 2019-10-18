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

extern void initFlash(void);

extern void flashErasePage( uint8 *addr );

extern void flashWrite( uint8 *addr, uint16 len, uint8 *buf );

#endif

/*********************************************************************
*********************************************************************/
