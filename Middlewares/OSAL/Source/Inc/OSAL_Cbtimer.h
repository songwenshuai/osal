/******************************************************************************
  Filename:       OSAL_Clock.h
  Revised:        $Date: 2012-02-02 12:55:32 -0800 (Thu, 02 Feb 2012) $
  Revision:       $Revision: 29143 $

  Description:    OSAL Clock definition and manipulation functions.
******************************************************************************/
#ifndef OSAL_CBTIMER_H
#define OSAL_CBTIMER_H

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
// Invalid timer id
#define INVALID_TIMER_ID                           0xFF

// Timed out timer
#define TIMEOUT_TIMER_ID                           0xFE

/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * MACROS
 */
#if ( OSAL_CBTIMER_NUM_TASKS == 0 )
  #error Callback Timer module shouldn't be included (no callback timer is needed)!
#elif ( OSAL_CBTIMER_NUM_TASKS == 1 )
  #define OSAL_CBTIMER_PROCESS_EVENT( a )          ( a )
#elif ( OSAL_CBTIMER_NUM_TASKS == 2 )
  #define OSAL_CBTIMER_PROCESS_EVENT( a )          ( a ), ( a )
#else
  #error Maximum of 2 callback timer tasks are supported! Modify it here.
#endif

/*********************************************************************
 * TYPEDEFS
 */

// Callback Timer function prototype. Callback function will be called
// when the associated timer expires.
//
// pData - pointer to data registered with timer
//
typedef void (*pfnCbTimer_t)( uint8_t *pData );

/*********************************************************************
 * VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Callback Timer task initialization function.
 */
extern void osal_CbTimerInit( uint8_t taskId );

/*
 * Callback Timer task event processing function.
 */
extern uint16_t osal_CbTimerProcessEvent( uint8_t  taskId,
                                        uint16_t events );

/*
 * Function to start a timer to expire in n mSecs.
 */
extern Status_t osal_CbTimerStart( pfnCbTimer_t  pfnCbTimer,
                                   uint8_t        *pData,
                                   uint32_t        timeout,
                                   uint8_t        *pTimerId );

/*
 * Function to start a timer to expire in n mSecs, then reload.
 */
extern Status_t osal_CbTimerStartReload( pfnCbTimer_t  pfnCbTimer,
                                         uint8_t        *pData,
                                         uint32_t        timeout,
                                         uint8_t        *pTimerId );

/*
 * Function to update a timer that has already been started.
 */
extern Status_t osal_CbTimerUpdate( uint8_t  timerId,
                                    uint32_t timeout );

/*
 * Function to stop a timer that has already been started.
 */
extern Status_t osal_CbTimerStop( uint8_t timerId );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CBTIMER_H */
