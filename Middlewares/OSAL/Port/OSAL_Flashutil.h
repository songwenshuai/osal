/**************************************************************************************************
  Filename:       armcm3flashutil.h
  Revised:        $Date: 2011-01-21 14:43:43 -0800 (Fri, 21 Jan 2011) $
  Revision:       $Revision: 24872 $

  Description:    This module defines the flashing utilities for the ARM CM3.
**************************************************************************************************/

#ifndef OSALFLASHUTIL_H
#define OSALFLASHUTIL_H

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

//
// 32 Bit MCU
//

extern void initFlash(void);

extern void flashErasePage( uint8 *addr );

extern void flashWrite( uint8 *addr, uint16 len, uint8 *buf );

//
// 8 Bit MCU
//

extern void HalFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt);

extern void HalFlashWrite(uint16 addr, uint8 *buf, uint16 cnt);

extern void HalFlashErase(uint8 pg);

#endif

/*********************************************************************
*********************************************************************/
