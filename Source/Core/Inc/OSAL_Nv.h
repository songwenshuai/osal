/**************************************************************************************************
  Filename:       OSAL_Nv.h
  Revised:        $Date: 2014-10-06 15:40:15 -0700 (Mon, 06 Oct 2014) $
  Revision:       $Revision: 40448 $

  Description:    This module defines the OSAL non-volatile memory functions.
**************************************************************************************************/

#ifndef OSAL_NV_H
#define OSAL_NV_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

// ++ sws add start
#define ZCD_NV_NWKKEY                     0x0082
#define ZCD_NV_NWK_ACTIVE_KEY_INFO        0x003A
#define ZCD_NV_NWK_ALTERN_KEY_INFO        0x003B

#define FLASH_BASE                        ((uint32)nvDataBuf)                      // Flash 
#define HAL_NV_PAGE_END                   (HAL_NV_PAGE_CNT - 1)                    // 0-5 six page

// Flash consists of 256 pages of 2 KB.
#define HAL_FLASH_PAGE_SIZE               2048
#define HAL_FLASH_WORD_SIZE               4

// Z-Stack uses flash pages for NV
#define HAL_NV_PAGE_CNT                   6
#define HAL_NV_PAGE_BEG                   (HAL_NV_PAGE_END - HAL_NV_PAGE_CNT + 1)

#define HAL_NV_START_ADDR                 FLASH_BASE
#define HAL_NV_ADDR_OFFSET(p_addr)        (((uint32)p_addr) - HAL_NV_START_ADDR)
// ++ sws add end

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Initialize NV service
 */
extern void osal_nv_init( void *p );

/*
 * Initialize an item in NV
 */
extern uint8 osal_nv_item_init( uint16 id, uint16 len, void *buf );

/*
 * Read an NV attribute
 */
extern uint8 osal_nv_read( uint16 id, uint16 offset, uint16 len, void *buf );

/*
 * Write an NV attribute
 */
extern uint8 osal_nv_write( uint16 id, uint16 offset, uint16 len, void *buf );

/*
 * Get the length of an NV item.
 */
extern uint16 osal_nv_item_len( uint16 id );

/*
 * Delete an NV item.
 */
extern uint8 osal_nv_delete( uint16 id, uint16 len );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_NV_H */
