/**************************************************************************************************
  Filename:       OSAL_Memory.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $
    
  Description:    This module defines the OSAL memory control functions. 
**************************************************************************************************/

#ifndef OSAL_MEMORY_H
#define OSAL_MEMORY_H

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

#if !defined ( OSALMEM_METRICS )
  #define OSALMEM_METRICS  TRUE
#endif

/*********************************************************************
 * MACROS
 */
 //#define DPRINTF_OSALHEAPTRACE   1

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
  * Initialize memory manager.
  */
  void osal_mem_init( void );

 /*
  * Setup efficient search for the first free block of heap.
  */
  void osal_mem_kick( void );

 /*
  * Allocate a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void *osal_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum );
#define osal_mem_alloc(_size ) osal_mem_alloc_dbg(_size, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void *osal_mem_alloc( uint16 size );
#endif /* DPRINTF_OSALHEAPTRACE */

 /*
  * Free a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void osal_mem_free_dbg( void *ptr, const char *fname, unsigned lnum );
#define osal_mem_free(_ptr ) osal_mem_free_dbg(_ptr, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void osal_mem_free( void *ptr );
#endif /* DPRINTF_OSALHEAPTRACE */

#if ( OSALMEM_METRICS )
 /*
  * Return the maximum number of blocks ever allocated at once.
  */
  uint16 osal_heap_block_max( void );

 /*
  * Return the current number of blocks now allocated.
  */
  uint16 osal_heap_block_cnt( void );

 /*
  * Return the current number of free blocks.
  */
  uint16 osal_heap_block_free( void );

 /*
  * Return the current number of bytes allocated.
  */
  uint16 osal_heap_mem_used( void );
#endif

  /*
   * Return the highest number of bytes ever used in the heap.
   */
  uint16 osal_heap_high_water( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OSAL_MEMORY_H */
