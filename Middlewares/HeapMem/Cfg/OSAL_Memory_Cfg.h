/**************************************************************************************************
  Filename:       OSAL_Memory_Cfg.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef OSAL_MEMORY_CFG_H
#define OSAL_MEMORY_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * OPTION
 */
// Memory Allocation Heap
#define MAXMEMHEAP                 4096   /* Typically, 1.0-6.0K */
#define OSALMEM_IN_USE             0x8000
 //#define DPRINTF_OSALHEAPTRACE   1

/*********************************************************************
 * MACROS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_MEMORY_CFG_H */
