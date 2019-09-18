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

#include "OSAL_Printf.h"
#include "OSAL_Timers.h"
#include "OSAL_Cbtimer.h"
#include "OSAL_Nv.h"

#include "GenericApp.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint8 App_TaskID;

/*********************************************************************
 * LOCAL VARIABLES
 */

 /*********************************************************************
  * FUNCTIONS
  */

static void App_ProcessOSALMsg( DebugStr_t *pInMsg );
static void Periodic_Event(void);
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

    // Setup Cb Timer
    osal_CbTimerStartReload(App_TimerCB, (uint8*)"test", 5000, NULL);
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
 * @fn      msg_send_str
 *
 * @brief
 *
 *   This feature allows modules to display a debug text string as
 *   applications execute in real-time. This feature will output to
 *   the serial port for display in the Z-Test tool.
 *
 *   This feature will most likely be compiled out in the production
 *   code in order to save code space.
 *
 * @param   _byte *str_ptr - pointer to null-terminated string
 *
 * @return  void
 */
static void msg_send_str( _byte *str_ptr )
{
  DebugStr_t *msg;
  _byte mln;
  _byte strLen;

  // Text string length
  strLen = (_byte)osal_strlen( (void*)str_ptr );

  // Debug string message length
  mln = sizeof ( DebugStr_t ) + strLen;

  // Get a message buffer to build the debug message
  msg = (DebugStr_t *)osal_msg_allocate( mln + 1 );
  // add terminator
  msg = osal_memset(msg, 0, mln + 1);
  if ( msg )
  {
    // Message type, length
    msg->hdr.event = APP_MESSAGE;
    msg->strLen = strLen;

    // Append message, has terminator
    msg->pString = (uint8 *)( msg + 1 );
    osal_memcpy ( msg->pString, str_ptr, strLen );

    osal_msg_send( App_TaskID, (uint8 *)msg );
  }
} // msg_send_str()

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
    msg_send_str("message");
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
static void App_ProcessOSALMsg(DebugStr_t *pInMsg)
{
    static uint32 rcv;

    switch (pInMsg->hdr.event)
    {
    case APP_MESSAGE:
        printf("messages  = %04d\r\n", ++rcv);
        printf("rcv       = %s\r\n", pInMsg->pString);
        printf("\r\n");
        break;
    default:
        break;
    }
}

/*********************************************************************
*********************************************************************/
