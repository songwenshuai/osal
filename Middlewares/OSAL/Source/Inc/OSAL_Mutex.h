/**************************************************************************************************
  Filename:       OSAL_Nv.h
  Revised:        $Date: 2014-10-06 15:40:15 -0700 (Mon, 06 Oct 2014) $
  Revision:       $Revision: 40448 $

  Description:    This module defines the OSAL non-volatile memory functions.
**************************************************************************************************/
#ifndef OSAL_MUTEX_H
#define OSAL_MUTEX_H

#ifdef __cplusplus

extern "C"
{
  
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef struct mutex_struct
{
 uint32 mutex_value;
 struct mutex_struct *next_mutex;
}osal_mutex_t;

/*********************************************************************
 * FUNCTIONS
 */

osal_mutex_t* osalMutexCreate(void);
void osalMutexDelete(osal_mutex_t** mutex);
void osalMutexTake(osal_mutex_t** mutex,uint32 mutex_overtime);
uint32 osalMutexCheck(osal_mutex_t* mutex);
void osalMutexRelease(osal_mutex_t** mutex);
void osalMutexUpdate( uint32 mutexTime );

/*********************************************************************
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif 


