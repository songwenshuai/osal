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
#ifndef _WIN32
#include "main.h"
#include "usart.h"
#endif

#include "OSAL.h"

#include "OSAL_Timers.h"
#include "OSAL_Cbtimer.h"
#include "OSAL_Nv.h"

#include "GenericApp.h"

#include "printf.h"

#ifndef _WIN32
#include "clk.h"
#endif

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
static void Periodic_Event(void);
static void App_TimerCB(uint8* pData);
#ifndef _WIN32
static void Clock_Test(void);
#endif

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
#ifndef _WIN32
    CLK_ERR err;
#endif

    App_TaskID = task_id;

    // Setup a delayed profile startup
    osal_set_event(App_TaskID, SBP_START_DEVICE_EVT);

    // Setup Cb Timer
    osal_CbTimerStartReload(App_TimerCB, (uint8*)"TEST", SBP_CBTIMER_EVT_DELAY, NULL);

#ifndef _WIN32
    Clk_Init(&err);
#endif
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
        Periodic_Event();

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
 * @param   uint8 *str_ptr - pointer to null-terminated string
 *
 * @return  void
 */
static void msg_send_str( uint8 *str_ptr )
{
  DebugStr_t *msg;
  uint8 mln;
  uint8 strLen;

  // Text string length
  strLen = (uint8)osal_strlen( (void*)str_ptr );

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
//------------------------------- hard fault test ------------------------------------
#if 0
    volatile unsigned int *p;
    unsigned int n;
    p = (unsigned int *)0xCCCCCCCC;
    n = *p;
    n = n;
#endif

    /* Get local time */
#ifndef _WIN32
    Clock_Test();
#endif
//------------------------------- time test ------------------------------------
    static int32 oldtime = 0, new_time = 0, deviation = 0;

    new_time = osal_GetSystemClock();
    deviation = ABS((new_time - oldtime) - SBP_PERIODIC_EVT_DELAY);
    oldtime = new_time;
#ifndef _WIN32
    SEGGER_SYSVIEW_PrintfHost("Tick = %d ms \r\n", deviation);
#endif
    printf("deviation  = %d ms\r\n", deviation);
//------------------------------- nv test --------------------------------------
    static uint32 flag = 0;
    static char nvDataWrite[13] = {0};
    static char nvDataRead[13] = {0};

    sprintf(nvDataWrite, "nv   = %04d\r\n", ++flag);
    osal_nv_item_init(NV_APPID, sizeof(nvDataWrite), nvDataWrite);
    osal_nv_write(NV_APPID, 0, osal_nv_item_len(NV_APPID), nvDataWrite);
    printf("weite_%s", nvDataWrite);
    osal_nv_read( NV_APPID, 0, osal_nv_item_len(NV_APPID), nvDataRead);
    printf("reads_%s", nvDataRead);
//------------------------------- message test ------------------------------------
    msg_send_str("message");
//------------------------------- ltoa test ------------------------------------
    static uint8 ltoa_str[11] = { 0 };
    static uint32 ltoa_num = 2147483648;

    osal_ltoa(ltoa_num, ltoa_str, 10);
    printf("ltoa_num   = %s\r\n", ltoa_str);

    osal_ltoa(ltoa_num, ltoa_str, 16);
    printf("ltoa_num   = 0x%s\r\n", ltoa_str);
//------------------------------- osal rand ------------------------------------
    printf("rand       = %d\r\n", osal_rand());
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
        static int32 oldtime1 = 0, new_time1 = 0, deviation1 = 0;
        
        new_time1 = osal_GetSystemClock();
        deviation1 = ABS((new_time1 - oldtime1) - SBP_CBTIMER_EVT_DELAY);
        oldtime1 = new_time1;
#ifndef _WIN32
        SEGGER_SYSVIEW_PrintfHost("Tick = %d ms \r\n", deviation1);
#endif
        printf("cb timer %s\r\n", pData);
        printf("deviation1 = %d ms\r\n", deviation1);
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

/*********************************************************************
 * @fn      Clock_Test
 *
 * @brief   void
 *
 * @param   void
 *
 * @return  none
 */
#ifndef _WIN32
static void Clock_Test(void)
{
    static CLK_DATE_TIME   date_time;
	static   CPU_BOOLEAN       valid;
	static    CLK_TZ_SEC      tz_sec;
	static    CLK_TS_SEC      ts_sec;
	static      CPU_CHAR     str[128];
	static    CLK_TS_SEC  ts_unix_sec;
    static      CPU_CHAR     CLK_FLAG = 0;

	tz_sec = 0;
	
    if(CLK_FLAG == 0){
    	valid = Clk_DateTimeMake(&date_time, 2013, 8, 11, 01, 11, 11, tz_sec);
    	if (valid != DEF_OK) {
    		printf("Clock make date/time failed\r\n");
    	}

    	valid = Clk_SetDateTime(&date_time);
    
    	if (valid != DEF_OK) {
    		printf("Clock set date/time failed\r\n");
    	}

    	Clk_DateTimeToTS(&ts_sec, &date_time);
    
    	if (valid == DEF_OK) {
    		printf("Clock timestamp = %u\r\n", ts_sec);
    	} else {
    		printf("Clock date/time to timestamp failed\r\n");
    	}

    	tz_sec = (-5 * 60 * 60);
    	valid = Clk_SetTZ(tz_sec);
    
    	if (valid != DEF_OK) {
        
    		printf("Clock set timezone unix failed\r\n");
    		return;
    	}

    	ts_sec = Clk_GetTS(); 
    
    	valid = Clk_TS_ToDateTime(ts_sec, 0, &date_time); 
    	if (valid != DEF_OK) {
    		printf("Clock convert timestamp to date/time failed\r\n");
    
    	}
    	valid = Clk_GetTS_Unix(&ts_unix_sec); 
    
    	if (valid != DEF_OK) {
    		printf("Clock get timestamp unix failed\r\n");
    
    	}
    
    	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time); 
    	if (valid != DEF_OK) {
    		printf("Clock timestamp unix to date/time failed\r\n");
    
    	}
    
    	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS, str, 128);
    	if (valid == DEF_OK) {
    		printf("Current Date/time :%s\n\r", str);
    	} else {
    		printf("Clock date/time to string failed\r\n");

    	}
    	ts_unix_sec = 126316799uL;
    
    	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time);
    	if (valid != DEF_OK) {
    		printf("Clock set date/time failed\r\n");
    
    	}

    	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY, str, 128);
    	if (valid == DEF_OK) {
    		printf("Unix timestamp = %s\r\n", str);
    	} else {
    		printf("Clock date/time to string failed\r\n");
    
    	}
        CLK_FLAG = 1;
	}
	Clk_GetDateTime(&date_time);

	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS,str, 128);
	
	if (valid == DEF_OK) {
		printf("Current Date/time :%s\n\r", str);
	} else {
		printf("Clock date/time to string failed\r\n");
	}
}
#endif

#ifndef _WIN32
#if defined(_NO_PRINTF)
#define UART_TIMEOUT_VALUE   1000
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#else
#define UART_TIMEOUT_VALUE   1000
#endif /* _NO_PRINTF */

#if defined(_NO_PRINTF)
/**
  * @brief  Retargets the C library printf function to the USART2.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART2.
  * @param  f: pointer to file (not used)
  * @retval The character received
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  /* We received the charracter on the handler of the USART2 */
  /* The handler must be initialise before */
  HAL_UART_Receive(&hlpuart1, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}

#else

/**
  * @brief  Retargets the C library printf function to the USART2.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */

int putc(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}
#endif /* _NO_PRINTF */
#endif

/*********************************************************************
*********************************************************************/
