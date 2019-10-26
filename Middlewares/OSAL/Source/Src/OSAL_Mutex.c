/*******************************************************************************
  Filename:       OSAL_Nv.c
  Revised:        $Date: 2013-09-05 09:47:48 -0700 (Thu, 05 Sep 2013) $
  Revision:       $Revision: 35218 $

  Description:    This module contains the OSAL non-volatile memory functions.
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Mutex.h"

#include "lwmem.h"

/*********************************************************************
 * MACROS
 */

#define MUTEX_ALLOC                       lwmem_malloc
#define MUTEX_FREE                        lwmem_free

/*********************************************************************
 * GLOBAL VARIABLES
 */

osal_mutex_t *osal_mutex_head = NULL;

/*********************************************************************
 * @fn      osalMutexCreate
 *
 * @brief   create a muter
 *
 * @param   none
 *
 * @return  a pointer point to creater muter
 */
osal_mutex_t* osalMutexCreate( void )
{
    osal_mutex_t *ptr;
    osal_mutex_t *pseach;
    ptr = ( osal_mutex_t*)MUTEX_ALLOC( sizeof(osal_mutex_t) );
    if( ptr != NULL )
    {
        ptr->next_mutex = NULL;
        ptr->mutex_value = FALSE;
        if( osal_mutex_head == NULL )
        {
            osal_mutex_head = ptr;
        }
        else
        {
            pseach = osal_mutex_head;
            while( pseach->next_mutex != NULL )
            {
                pseach = pseach->next_mutex;
            }
            pseach->next_mutex = ptr;
        }
    }
    return ptr;
}

/*********************************************************************
 * @fn      osalMutexDelete
 *
 * @brief  delete a muter
 *
 * @param  point to be delete muter
 *
 * @return none
 */
void osalMutexDelete( osal_mutex_t** mutex )
{
    osal_mutex_t* pseach = osal_mutex_head;
    if( pseach == NULL )
        return;
    if( pseach == *mutex )
    {
        osal_mutex_head = (*mutex)->next_mutex;
        MUTEX_FREE( (uint8*)(*mutex) );
        *mutex = NULL;

    }
    else
    {
        while( (pseach->next_mutex != *mutex) && (pseach->next_mutex != NULL) )
        {
            pseach = pseach->next_mutex;
        }
        if( pseach->next_mutex == *mutex )
        {
            pseach->next_mutex = (*mutex)->next_mutex;
            MUTEX_FREE( (uint8*)(*mutex) );
            *mutex = NULL;
        }
    }
}
/*********************************************************************
 * @fn      osalMutexTake
 *
 * @brief  take a muter
 *
 * @param   a pointer point to be taken muter
 *
 * @return none
 */
void osalMutexTake( osal_mutex_t** mutex,uint32 mutex_overtime )
{
    if( *mutex == NULL )
    {
        *mutex = osalMutexCreate();
    }
    if( *mutex != NULL )
    {
        (*mutex)->mutex_value = mutex_overtime;
    }
}
/*********************************************************************
 * @fn      osalMutexCheck
 *
 * @brief  cheak muter state
 *
 * @param   a pointer point to be check muter
 *
 * @return  muter state   1 - take  0 - release
 */
uint32 osalMutexCheck( osal_mutex_t* mutex )
{
    if( mutex == NULL )
        return FALSE;
    if( mutex->mutex_value != 0 ) 
        return TRUE;
    return FALSE;
}
/*********************************************************************
 * @fn      osalMutexRelease
 *
 * @brief  release a mutex
 *
 * @param   a pointer point to be  release  mutex
 *
 * @return none
 */
void osalMutexRelease( osal_mutex_t** mutex )
{
    if( (*mutex) == NULL ) 
        return;
    (*mutex)->mutex_value = 0;
    osalMutexDelete(mutex);
}


/*********************************************************************
 * @fn      osalMutexUpdate
 *
 * @brief   update mutex clock information
 *
 * @param  NONE
 *
 * @return NONE
 */
void osalMutexUpdate( uint32 mutexTime )
{
    osal_mutex_t *pseach;
    pseach = osal_mutex_head;
    while( pseach != NULL )
    {
        if( pseach->mutex_value > 0 )
        {
            pseach->mutex_value--;
        }
        pseach = pseach->next_mutex;
    }
}


