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

#include "printf.h"
#include  <shell.h>
#include  <sh_shell.h>
#include  <terminal.h>

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

#ifndef _WIN32
void  Clk_TaskHandler (void);
#endif

 /*********************************************************************
  * FUNCTIONS
  */

static void App_ProcessOSALMsg( DebugStr_t *pInMsg );
static void App_TimerCB(uint8* pData);
static CPU_BOOLEAN UCOS_Shell_Init (void);

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

    UCOS_Shell_Init();

    // Setup a delayed profile startup
    osal_set_event(App_TaskID, SBP_START_DEVICE_EVT);

    // Setup Cb Timer
    osal_CbTimerStartReload(App_TimerCB, (uint8*)"TEST", SBP_CBTIMER_EVT_DELAY, NULL);
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
#ifndef _WIN32
        osal_start_timerEx(App_TaskID, SBP_CLOCK_EVT, SBP_CLOCK_EVT_DELAY);
#endif
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
        Terminal_Task(NULL);
        return (events ^ SBP_PERIODIC_EVT);
    }

#ifndef _WIN32
    if (events & SBP_CLOCK_EVT)
    {
        // Restart timer
        if ( SBP_CLOCK_EVT_DELAY )
        {
            osal_start_timerEx(App_TaskID, SBP_CLOCK_EVT, SBP_CLOCK_EVT_DELAY);
        }
        
        // Perform clock task
        Clk_TaskHandler();

        return (events ^ SBP_CLOCK_EVT);
    }
#endif
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
        printf("messages   = %04d\r\n", ++rcv);
        printf("rcv        = %s\r\n", pInMsg->pString);
        printf("\r\n");
        break;
    default:
        break;
    }
}

/*
*********************************************************************************************************
*                                          UCOS Shell Init
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static CPU_BOOLEAN UCOS_Shell_Init (void)
{
    CPU_BOOLEAN err_shell, terminal;

    printf(("Initializing uC/Shell.\r\n"));

    err_shell = Shell_Init();

    if (err_shell != DEF_OK) {
        printf(("Error initializing uC/Shell.\r\n"));
        return (DEF_FAIL);
    }
    err_shell = ShShell_Init();
    if (err_shell != DEF_OK) {
        printf(("Error initializing uC/Shell.\r\n"));
        return (DEF_FAIL);
    }

    terminal = Terminal_Init();

    if (terminal != DEF_OK) {
        printf(("Error initializing uC/Terminal.\r\n"));
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*********************************************************************
*********************************************************************/
