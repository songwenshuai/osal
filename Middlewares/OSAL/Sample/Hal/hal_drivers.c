/**************************************************************************************************
  Filename:       hal_drivers.c
  Revised:        $Date: 2007-07-06 10:42:24 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Description:    This file contains the interface to the Drivers Service.

**************************************************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/

#include "OSAL.h"
#include "OSAL_Timers.h"

#include "hal_drivers.h"

#include "hal_led.h"

 /*********************************************************************
  * GLOBAL FUNCTIONS
  */

#ifndef _WIN32
extern void flex_button_scan(void);
extern void user_button_init( void );
#endif

/**************************************************************************************************
 *                                      GLOBAL VARIABLES
 **************************************************************************************************/
uint8 Hal_TaskID;

extern void HalLedUpdate( void ); /* Notes: This for internal only so it shouldn't be in hal_led.h */

/**************************************************************************************************
 * @fn      Hal_Init
 *
 * @brief   Hal Initialization function.
 *
 * @param   task_id - Hal TaskId
 *
 * @return  None
 **************************************************************************************************/
void Hal_Init( uint8 task_id )
{
  /* Register task ID */
  Hal_TaskID = task_id;

  // Initialze HAL drivers
  HalDriverInit();

  /* Start polling if callback function is setup*/
  osal_set_event(Hal_TaskID, HAL_KEY_EVENT);
}

/**************************************************************************************************
 * @fn      Hal_DriverInit
 *
 * @brief   Initialize HW - These need to be initialized before anyone.
 *
 * @param   task_id - Hal TaskId
 *
 * @return  None
 **************************************************************************************************/
void HalDriverInit (void)
{

  /* LED */
#if (defined HAL_LED) && (HAL_LED == TRUE)
  HalLedInit();
#endif

  /* KEY */
#if (defined HAL_KEY) && (HAL_KEY == TRUE)
#ifndef _WIN32
  user_button_init();
#endif
#endif
  
}

/**************************************************************************************************
 * @fn      Hal_ProcessEvent
 *
 * @brief   Hal Process Event
 *
 * @param   task_id - Hal TaskId
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
uint16 Hal_ProcessEvent( uint8 task_id, uint16 events )
{
  uint8 *msgPtr;

  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    msgPtr = osal_msg_receive(Hal_TaskID);

    while (msgPtr)
    {
      /* Do something here - for now, just deallocate the msg and move on */

      /* De-allocate */
      osal_msg_deallocate( msgPtr );
      /* Next */
      msgPtr = osal_msg_receive( Hal_TaskID );
    }
    return events ^ SYS_EVENT_MSG;
  }

  if ( events & HAL_LED_BLINK_EVENT )
  {
#if (defined (BLINK_LEDS)) && (HAL_LED == TRUE)
    HalLedUpdate();
#endif /* BLINK_LEDS && HAL_LED */
    return events ^ HAL_LED_BLINK_EVENT;
  }

  if (events & HAL_KEY_EVENT)
  {
#if (defined HAL_KEY) && (HAL_KEY == TRUE)
    /* Check for keys */
#ifndef _WIN32
    flex_button_scan();
#endif

    /* if interrupt disabled, do next polling */
    osal_start_timerEx( Hal_TaskID, HAL_KEY_EVENT, 20);
#endif
    return events ^ HAL_KEY_EVENT;
  }

  return 0;
}

/**************************************************************************************************
 * @fn      Hal_ProcessPoll
 *
 * @brief   This routine will be called by OSAL to poll UART, TIMER...
 *
 * @param   task_id - Hal TaskId
 *
 * @return  None
 **************************************************************************************************/
void Hal_ProcessPoll ()
{

}

/**************************************************************************************************
**************************************************************************************************/

