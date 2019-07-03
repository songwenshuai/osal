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
#include "OSAL_Cbtimer.h"
#include "OSAL_Nv.h"

#include "GenericApp.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint8 App_TaskID;

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/
static void App_ProcessOSALMsg( osal_event_hdr_t *pInMsg );
static void Periodic_Event(void);
static void App_TimerCB(uint8* pData);

extern uint32 get_second(void);
extern uint32 get_millisecond(void);
extern uint32 get_microsecond(void);

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
    VOID task_id; // Intentionally unreferenced parameter

    if (events & SYS_EVENT_MSG)
    {
        uint8* pMsg;

        if ( (pMsg = osal_msg_receive( App_TaskID )) != NULL )
        {
            App_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );

            // Release the memory
            VOID osal_msg_deallocate( pMsg );
        }
        
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & SBP_START_DEVICE_EVT)
    {
        // Set timer for first periodic event
        osal_start_timerEx(App_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_DELAY);

        return (events ^ SBP_START_DEVICE_EVT);
    }

    if (events & SBP_PERIODIC_EVT)
    {
        Periodic_Event();

        osal_start_timerEx(App_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_DELAY);

        return (events ^ SBP_PERIODIC_EVT);
    }

    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn          Periodic_Event
 *
 * @brief       Periodic Event.
 *
 * @param       none
 *
 * @return      none
 */
static void Periodic_Event(void)
{
//------------------------------- time test ------------------------------------
    static int32 oldtime = 0, new_time = 0, deviation = 0;

    new_time = get_millisecond();
    deviation = ((new_time - oldtime) - SBP_PERIODIC_EVT_DELAY);
    oldtime = new_time;
    printf("deviation = %d\r\n", deviation);
//------------------------------- cbtimer test ------------------------------------
    static int32 cbtimerf = 0;
    if (cbtimerf == 0)
    {
        // Setup Cb Timer
        osal_CbTimerStartReload(App_TimerCB, (uint8*)"test", 5000, NULL);
        cbtimerf = 1;
    }
//------------------------------- nv test --------------------------------------
    static uint32 flag = 0;
    static char nvDataWrite[13] = {0};
    static char nvDataRead[13] = {0};

    sprintf(nvDataWrite, "nv  = %04d\r\n", ++flag);
    osal_nv_item_init(NV_APPID, sizeof(nvDataWrite), nvDataWrite);
    osal_nv_write(NV_APPID, 0, osal_nv_item_len(NV_APPID), nvDataWrite);
    printf("weite_%s", nvDataWrite);
    osal_nv_read( NV_APPID, 0, osal_nv_item_len(NV_APPID), nvDataRead);
    printf("reads_%s", nvDataRead);
//------------------------------- message test ------------------------------------
    osal_event_hdr_t *msgPtr;

    // Send the address to the task
    msgPtr = (osal_event_hdr_t *)osal_msg_allocate(sizeof(osal_event_hdr_t));
    if (msgPtr)
    {
        msgPtr->event = APP_MESSAGE;
        msgPtr->status = (uint8)get_second();

        osal_msg_send(App_TaskID, (uint8 *)msgPtr);
    }
//------------------------------- ltoa test ------------------------------------
    uint8 ltoa_str[11] = { 0 };
    uint32 ltoa_num = 2147483648;

    osal_ltoa(ltoa_num, ltoa_str, 10);
    printf("ltoa_num  = %s\r\n", ltoa_str);

    osal_ltoa(ltoa_num, ltoa_str, 16);
    printf("ltoa_num  = 0x%s\r\n", ltoa_str);
//------------------------------- osal rand ------------------------------------
    printf("rand      = %d\r\n", osal_rand());
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
        printf("cb timer %s\r\n", pData);
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
static void App_ProcessOSALMsg(osal_event_hdr_t *pInMsg)
{
    static uint32 rcv;

    switch (pInMsg->event)
    {
    case APP_MESSAGE:
        printf("messages  = %04d\r\n", ++rcv);
        printf("status    = %04d\r\n", pInMsg->status);
        printf("\r\n");
        break;
    default:
        break;
    }
}

/*********************************************************************
*********************************************************************/
