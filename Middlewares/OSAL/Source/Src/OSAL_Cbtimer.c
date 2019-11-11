/**************************************************************************************************
  Filename:       OSAL_Cbtimer.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:    This file contains the buffer management APIs. These APIs are not
                  reentrant hence cannot be called from an interrupt context.
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Tasks.h"
#include "OSAL_Timers.h"
#include "OSAL_Cbtimer.h"

/*********************************************************************
 * MACROS
 */
// Macros to convert one-dimensional index 'timer id' to two-dimensional indices
// 'task id' and 'event id'.

// Find out event id using timer id
#define EVENT_ID( timerId )            ( 0x0001 << ( ( timerId ) % NUM_CBTIMERS_PER_TASK ) )

// Find out task id using timer id
#define TASK_ID( timerId )             ( ( ( timerId ) / NUM_CBTIMERS_PER_TASK ) + baseTaskID )

// Find out bank task id using task id
#define BANK_TASK_ID( taskId )         ( ( baseTaskID - ( taskId ) ) * NUM_CBTIMERS )

/*********************************************************************
 * CONSTANTS
 */
// Number of callback timers supported per task (limited by the number of OSAL event timers)
#define NUM_CBTIMERS_PER_TASK          15

// Total number of callback timers
#define NUM_CBTIMERS                   ( OSAL_CBTIMER_NUM_TASKS * NUM_CBTIMERS_PER_TASK )

/*********************************************************************
 * TYPEDEFS
 */
// Callback Timer structure
typedef struct
{
  pfnCbTimer_t  pfnCbTimer; // callback function to be called when timer expires
  uint8_t        *pData;      // data to be passed in to callback function
} cbTimer_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Callback Timer base task id
uint16_t baseTaskID = TASK_NO_TASK;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// Callback Timers table.
#if ( NUM_CBTIMERS > 0 )
  cbTimer_t cbTimers[NUM_CBTIMERS];
#endif

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static Status_t cbTimerSetup( pfnCbTimer_t  pfnCbTimer,
                              uint8_t        *pData,
                              uint32_t        timeout,
                              uint8_t        *pTimerId,
                              uint8_t         reload );

/*********************************************************************
 * API FUNCTIONS
 */


/*********************************************************************
 * @fn          osal_CbTimerInit
 *
 * @brief       Callback Timer task initialization function. This function
 *              can be called more than once (OSAL_CBTIMER_NUM_TASKS times).
 *
 * @param       taskId - Message Timer task ID.
 *
 * @return      void
 */
void osal_CbTimerInit( uint8_t taskId )
{
  if ( baseTaskID == TASK_NO_TASK )
  {
    // Only initialize the base task id
    baseTaskID = taskId;

    // Initialize all timer structures
    osal_memset( cbTimers, 0, sizeof( cbTimers ) );
  }
}


/*********************************************************************
 * @fn          osal_CbTimerProcessEvent
 *
 * @brief       Callback Timer task event processing function.
 *
 * @param       taskId - task ID.
 * @param       events - events.
 *
 * @return      events not processed
 */
uint16_t osal_CbTimerProcessEvent( uint8_t taskId, uint16_t events )
{
  if ( events & SYS_EVENT_MSG )
  {
    // Process OSAL messages

    // return unprocessed events
    return ( events ^ SYS_EVENT_MSG );
  }

  if ( events )
  {
    uint8_t i;
    uint16_t event = 0;
    halIntState_t cs;

    HAL_ENTER_CRITICAL_SECTION(cs);

    // Process event timers
    for ( i = 0; i < NUM_CBTIMERS_PER_TASK; i++ )
    {
      if ( ( events >> i ) & 0x0001 )
      {
        cbTimer_t *pTimer = &cbTimers[BANK_TASK_ID( taskId )+i];

        // Found the first event
        event =  0x0001 << i;

        // check there is a callback function to call
        if ( pTimer->pfnCbTimer != NULL )
        {
          // Timer expired, call the registered callback function
          pTimer->pfnCbTimer( pTimer->pData );
        }

        // check if the timer is still active
        // Note: An active timer means the timer was started with reload!
        if ( osal_get_timeoutEx( taskId, event ) == 0 )
        {
          // Mark entry as free
          pTimer->pfnCbTimer = NULL;

          // Null out data pointer
          pTimer->pData = NULL;
        }

        // We only process one event at a time
        break;
      }
    }

    HAL_EXIT_CRITICAL_SECTION(cs);

    // return unprocessed events
    return ( events ^ event );
  }

  // If reach here, the events are unknown
  // Discard or make more handlers
  return 0;
}


/*********************************************************************
 * @fn      osal_CbTimerStart
 *
 * @brief   This function is called to start a callback timer to expire
 *          in n mSecs. When the timer expires, the registered callback
 *          function will be called.
 *
 * input parameters
 *
 * @param   pfnCbTimer - Callback function to be called when timer expires.
 * @param   pData      - Data to be passed in to callback function.
 * @param   timeout    - In milliseconds.
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 *
 * output parameters
 *
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 *
 * @return  Success, or Failure.
 */
Status_t osal_CbTimerStart( pfnCbTimer_t  pfnCbTimer,
                            uint8_t        *pData,
                            uint32_t        timeout,
                            uint8_t        *pTimerId )
{
  return ( cbTimerSetup( pfnCbTimer,
                         pData,
                         timeout,
                         pTimerId,
                         FALSE ) );
}


/*********************************************************************
 * @fn      osal_CbTimerReload
 *
 * @brief   This function is called to start a callback timer to expire
 *          in n mSecs. When the timer expires, it will automatically
 *          reload the timer, and the registered callback function will
 *          be called.
 *
 * input parameters
 *
 * @param   pfnCbTimer - Callback function to be called when timer expires.
 * @param   pData      - Data to be passed in to callback function.
 * @param   timeout    - In milliseconds.
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 *
 * output parameters
 *
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 *
 * @return  Success, or Failure.
 */
Status_t osal_CbTimerStartReload( pfnCbTimer_t  pfnCbTimer,
                                  uint8_t        *pData,
                                  uint32_t        timeout,
                                  uint8_t        *pTimerId )
{
  return ( cbTimerSetup( pfnCbTimer,
                         pData,
                         timeout,
                         pTimerId,
                         TRUE ) );
}


/*********************************************************************
 * @fn      osal_CbTimerUpdate
 *
 * @brief   This function is called to update a message timer that has
 *          already been started. If OSAL_SUCCESS, the function will update
 *          the timer's timeout value. If INVALIDPARAMETER, the timer
 *          either doesn't exit.
 *
 * @param   timerId - identifier of the timer that is to be updated
 * @param   timeout - new timeout in milliseconds.
 *
 * @return  OSAL_SUCCESS or INVALIDPARAMETER if timer not found
 */
Status_t osal_CbTimerUpdate( uint8_t timerId, uint32_t timeout )
{
  halIntState_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Look for the existing timer
  if ( timerId < NUM_CBTIMERS )
  {
    if ( cbTimers[timerId].pfnCbTimer != NULL )
    {
      // Make sure the corresponding OSAL event timer is still running
      if ( osal_get_timeoutEx( TASK_ID( timerId ), EVENT_ID( timerId ) ) != 0 )
      {
        // Timer exists; update it
        osal_start_timerEx( TASK_ID( timerId ), EVENT_ID( timerId ), timeout );

        HAL_EXIT_CRITICAL_SECTION(cs);

        return (  OSAL_SUCCESS );
      }
    }
  }

  HAL_EXIT_CRITICAL_SECTION(cs);

  // Timer not found
  return ( INVALIDPARAMETER );
}


/*********************************************************************
 * @fn      osal_CbTimerStop
 *
 * @brief   This function is called to stop a timer that has already been
 *          started. If OSAL_SUCCESS, the function will cancel the timer. If
 *          INVALIDPARAMETER, the timer doesn't exit.
 *
 * @param   timerId - identifier of the timer that is to be stopped
 *
 * @return  OSAL_SUCCESS or INVALIDPARAMETER if timer not found
 */
Status_t osal_CbTimerStop( uint8_t timerId )
{
  halIntState_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Look for the existing timer
  if ( timerId < NUM_CBTIMERS )
  {
    if ( cbTimers[timerId].pfnCbTimer != NULL )
    {
      // Timer exists; stop the OSAL event timer first
      osal_stop_timerEx( TASK_ID( timerId ), EVENT_ID( timerId ) );

      // Mark entry as free
      cbTimers[timerId].pfnCbTimer = NULL;

      // Null out data pointer
      cbTimers[timerId].pData = NULL;

      HAL_EXIT_CRITICAL_SECTION(cs);

      return ( OSAL_SUCCESS );
    }
  }

  HAL_EXIT_CRITICAL_SECTION(cs);

  // Timer not found
  return ( INVALIDPARAMETER );
}

/*
** Local Functions
*/

/*********************************************************************
 * @fn      cbTimerSetup
 *
 * @brief   This function is a common routine that can be used to start
 *          or reload a callback timer to expire in n mSecs.
 *
 * input parameters
 *
 * @param   pfnCbTimer - Callback function to be called when timer expires.
 * @param   pData      - Data to be passed in to callback function.
 * @param   timeout    - In milliseconds.
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 * @param   reload     - Indicate start (FALSE) or start reload (TRUE) timer.
 *
 * output parameters
 *
 * @param   pTimerId   - Pointer to new timer Id or NULL.
 *
 * @return  Success, or Failure.
 */
static Status_t cbTimerSetup( pfnCbTimer_t  pfnCbTimer,
                              uint8_t        *pData,
                              uint32_t        timeout,
                              uint8_t        *pTimerId,
                              uint8_t         reload )
{
  uint8_t i;
  halIntState_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Validate input parameters
  if ( pfnCbTimer == NULL )
  {
    HAL_EXIT_CRITICAL_SECTION(cs);

    return ( INVALIDPARAMETER );
  }

  // Look for an unused timer first
  for ( i = 0; i < NUM_CBTIMERS; i++ )
  {
    if ( cbTimers[i].pfnCbTimer == NULL )
    {
      // Start the OSAL event timer first
      if ( ( (reload==TRUE)          ?
             osal_start_reload_timer :
             osal_start_timerEx )( TASK_ID( i ), EVENT_ID( i ), timeout ) == OSAL_SUCCESS )
      {
        // Set up the callback timer
        // Note: An odd pointer will be used to indicate to the process event
        //       handler that the timer was started with reload.
        cbTimers[i].pfnCbTimer = pfnCbTimer; //(pfnCbTimer_t)((uint32_t)pfnCbTimer | reload);
        cbTimers[i].pData      = pData;

        // Check if the caller wants the timer Id
        if ( pTimerId != NULL )
        {
          // Caller is interested in the timer id
          *pTimerId = i;
        }

        HAL_EXIT_CRITICAL_SECTION(cs);

        return ( OSAL_SUCCESS );
      }
    }
  }

  HAL_EXIT_CRITICAL_SECTION(cs);

  // No timer available
  return ( NO_TIMER_AVAIL );
}

/****************************************************************************
****************************************************************************/
