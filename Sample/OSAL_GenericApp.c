/**************************************************************************************************
  Filename:       OSAL_GenericApp.c
  Revised:        $Date: 2008-02-07 12:10:00 -0800 (Thu, 07 Feb 2008) $
  Revision:       $Revision: 16360 $

  Description:    This file contains all the settings and other functions
                  that the user should set and change.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Helper.h"
#include "OSAL_Cbtimer.h"
#include "OSAL_Tasks.h"

#include "OSAL_Memory.h"

#include "GenericApp.h"
#include "hal_drivers.h"

#include "printf.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// The order in this table must be identical to the task initialization calls below in osalInitTask.
const pTaskEventHandlerFn tasksArr[] = {
  
  /* Hal Tasks */
  Hal_ProcessEvent,

  /* Callback Timer Tasks */
  OSAL_CBTIMER_PROCESS_EVENT( osal_CbTimerProcessEvent ),
  
  /* Application */
  App_ProcessEvent
};

const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16 *tasksEvents;

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
  uint8 taskID = 0;

  tasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);

  /* The tasksEvents allocated pointer must be valid */
  if (tasksEvents != NULL)
  {
  	osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));
  }
  else
  {
    HAL_ASSERT_FORCED();
  }

  /* Hal Tasks */
  Hal_Init( taskID++ );

  /* Callback Timer Tasks */
  osal_CbTimerInit( taskID );
  taskID += OSAL_CBTIMER_NUM_TASKS;
  
  /* Application */
  App_Init( taskID );
}

/*********************************************************************
*********************************************************************/
