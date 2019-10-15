 /******************************************************************************
   Filename:     OSAL_Bufmgr.h
   Revised:      $Date: 2014-06-30 16:38:56 -0700 (Mon, 30 Jun 2014) $
   Revision:     $Revision: 39297 $

   Description:  This file contains the buffer management definitions.
 ******************************************************************************/

#ifndef OSAL_BUFMGR_H
#define OSAL_BUFMGR_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "OSAL_Config.h"

/*********************************************************************
 * CONSTANTS
 */

/*
 * Allocate a block of memory.
 */
#define BM_alloc                osal_bm_alloc
  
/*
 * Free a block of memory.
 */
#define BM_free                 osal_bm_free

/*********************************************************************
 * VARIABLES
 */


/*********************************************************************
 * MACROS
 */


/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Allocate a block of memory.
 */
extern void *osal_bm_alloc( uint16 size );

/*
 * Add or remove header space for the payload pointer.
 */
extern void *osal_bm_adjust_header( void *payload_ptr, int16 size );
  
/*
 * Add or remove tail space for the payload pointer.
 */
extern void *osal_bm_adjust_tail( void *payload_ptr, int16 size );

/*
 * Free a block of memory.
 */
extern void osal_bm_free( void *payload_ptr );
  
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_BUFMGR_H */
