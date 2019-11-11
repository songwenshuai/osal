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
extern uint8_t osal_nv_item_init( uint16_t id, uint16_t len, void *buf );

/*
 * Read an NV attribute
 */
extern uint8_t osal_nv_read( uint16_t id, uint16_t offset, uint16_t len, void *buf );

/*
 * Write an NV attribute
 */
extern uint8_t osal_nv_write( uint16_t id, uint16_t offset, uint16_t len, void *buf );

/*
 * Get the length of an NV item.
 */
extern uint16_t osal_nv_item_len( uint16_t id );

/*
 * Delete an NV item.
 */
extern uint8_t osal_nv_delete( uint16_t id, uint16_t len );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_NV_H */
