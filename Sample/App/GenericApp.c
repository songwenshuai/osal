/**************************************************************************************************
  Filename:       GenericApp.c
  Revised:        $Date: 2008-02-07 12:10:00 -0800 (Thu, 07 Feb 2008) $
  Revision:       $Revision: 16360 $

  Description:    This file contains all the settings and other functions
                  that the user should set and change.

**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "OSAL.h"

#include "OSAL_Timers.h"
#include "OSAL_Memory.h"
#include "OSAL_Cbtimer.h"
#include "OSAL_Nv.h"

#include "GenericApp.h"

#include "printf.h"

#include "hal_led.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint8 App_TaskID;

/*********************************************************************
 * LOCAL VARIABLES
 */

 /*********************************************************************
  * GLOBAL FUNCTIONS
  */

 /*********************************************************************
  * FUNCTIONS
  */

static void App_ProcessOSALMsg( DebugStr_t *pInMsg );
static void App_TimerCB(uint8* pData);

/*********************************************************************
 * @fn          App_Init
 *
 * @brief       Initialization function for the zclGeneral layer.
 *
 * @param       none
 *
 * @return      none
 */
void App_Init(uint8 task_id)
{
    App_TaskID = task_id;

    // Setup a delayed profile startup
    osal_set_event(App_TaskID, SBP_START_DEVICE_EVT);
}

/*********************************************************************
 * @fn          App_ProcessEvent
 *
 * @brief       Event Loop Processor for zclGeneral.
 *
 * @param       none
 *
 * @return      none
 */
uint16 App_ProcessEvent(uint8 task_id, uint16 events)
{
    uint8* pMsg;

    VOID task_id; // Intentionally unreferenced parameter

    if ( events & SYS_EVENT_MSG )
    {
        pMsg = osal_msg_receive( App_TaskID );

        while ( pMsg )
        {
            /* Do something here - for now, just deallocate the msg and move on */
            App_ProcessOSALMsg( (DebugStr_t *)pMsg );
            /* De-allocate */
            VOID osal_msg_deallocate( pMsg );
            /* Next */
            pMsg = osal_msg_receive( App_TaskID );
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & SBP_START_DEVICE_EVT)
    {
        ledCmd_t *pCmd = (ledCmd_t *)osal_mem_alloc( sizeof ( ledCmd_t ) );
    
        pCmd->connHandle = HAL_LED_MODE_BLINK;
        pCmd->code = HAL_LED_3;
    
        // Setup Cb Timer
        osal_CbTimerStartReload(App_TimerCB, (uint8*)pCmd, SBP_CBTIMER_EVT_DELAY, NULL);

        // Set timer for first periodic event
        osal_start_timerEx(App_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_DELAY);

        return (events ^ SBP_START_DEVICE_EVT);
    }

    if (events & SBP_PERIODIC_EVT)
    {
        // Restart timer
        if ( SBP_PERIODIC_EVT_DELAY )
        {
            osal_start_timerEx(App_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_DELAY);
        }
        
        // Perform periodic application task

        return (events ^ SBP_PERIODIC_EVT);
    }

    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn          App_TimerCB
 *
 * @brief       App Timer callback.
 *
 * @param       none
 *
 * @return      none
 */
static void App_TimerCB(uint8* pData)
{
    if (pData)
    {
        ledCmd_t *pCmd = (ledCmd_t *)pData;

        HalLedSet(pCmd->code, pCmd->connHandle);
    }
}

/*********************************************************************
 * @fn      App_ProcessOSALMsg
 *
 * @brief   Process ZCL Foundation incoming message
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  none
 */
static void App_ProcessOSALMsg(DebugStr_t *pInMsg)
{
    switch (pInMsg->hdr.event)
    {
    case APP_MESSAGE:
        printf("rcv        = %s\r\n", pInMsg->pString);
        break;
    default:
        break;
    }
}

/*********************************************************************
*********************************************************************/
