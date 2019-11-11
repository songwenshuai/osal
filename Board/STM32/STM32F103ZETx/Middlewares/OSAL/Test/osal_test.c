
#include "OSAL.h"
#include "GenericApp.h"

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
    static int32_t oldtime = 0, new_time = 0, deviation = 0;

    new_time = osal_GetSystemClock();
    deviation = ABS((new_time - oldtime) - SBP_PERIODIC_EVT_DELAY);
    oldtime = new_time;
    printf("deviation  = %d ms\r\n", deviation);
//------------------------------- nv test --------------------------------------
    // Initialize NV System
    osal_nv_init(NULL);

    static uint32_t flag = 0;
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
    static uint8_t ltoa_str[11] = { 0 };
    static uint32_t ltoa_num = 2147483648;

    osal_ltoa(ltoa_num, ltoa_str, 10);
    printf("ltoa_num   = %s\r\n", ltoa_str);

    osal_ltoa(ltoa_num, ltoa_str, 16);
    printf("ltoa_num   = 0x%s\r\n", ltoa_str);
//------------------------------- osal rand ------------------------------------
    printf("rand       = %d\r\n", osal_rand());
}

void cb_timer_test(uint8_t* pData){
        static int32_t oldtime1 = 0, new_time1 = 0, deviation1 = 0;
        
        new_time1 = osal_GetSystemClock();
        deviation1 = ABS((new_time1 - oldtime1) - SBP_CBTIMER_EVT_DELAY);
        oldtime1 = new_time1;
        printf("cb timer %s\r\n", pData);
        printf("deviation1 = %d ms\r\n", deviation1);
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
 * @param   uint8_t *str_ptr - pointer to null-terminated string
 *
 * @return  void
 */
static void msg_send_str( uint8_t *str_ptr )
{
  DebugStr_t *msg;
  uint8_t mln;
  uint8_t strLen;

  // Text string length
  strLen = (uint8_t)osal_strlen( (void*)str_ptr );

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
    msg->pString = (uint8_t *)( msg + 1 );
    osal_memcpy ( msg->pString, str_ptr, strLen );

    osal_msg_send( App_TaskID, (uint8_t *)msg );
  }
} // msg_send_str()


/*
 * Put LEDs in sleep state - store current values
 */
extern void HalLedEnterSleep( void );

/*
 * Retore LEDs from sleep state
 */
extern void HalLedExitSleep( void );

/*
 * Return LED state
 */
extern uint8_t HalLedGetState ( void );

/*
 * Breath led
 */
extern uint32_t HalledBreathHandle(HalledBreath_t* breath);

static uint8_t HalSleepLedState;         // LED state at last set/clr/blink update

/***************************************************************************************************
 * @fn      HalLedEnterSleep
 *
 * @brief   Store current LEDs state before sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedEnterSleep( void )
{
#ifdef BLINK_LEDS
  /* Sleep ON */
  HalLedStatusControl.sleepActive = TRUE;
#endif /* BLINK_LEDS */

#if (HAL_LED == TRUE)
  /* Save the state of each led */
  HalSleepLedState = 0;
#ifndef _WIN32
  HalSleepLedState |= BSP_LED_Read(USER_LD1);
  HalSleepLedState |= BSP_LED_Read(USER_LD2) << 1;
  HalSleepLedState |= BSP_LED_Read(USER_LD3) << 2;

#else
  HalSleepLedState |= BSP_GetLEDState(0);
  HalSleepLedState |= BSP_GetLEDState(1) << 1;
  HalSleepLedState |= BSP_GetLEDState(2) << 2;
#endif
  /* TURN OFF all LEDs to save power */
  HalLedOnOff (HAL_LED_ALL, HAL_LED_MODE_OFF);
#endif /* HAL_LED */

}

/***************************************************************************************************
 * @fn      HalLedExitSleep
 *
 * @brief   Restore current LEDs state after sleep
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
void HalLedExitSleep( void )
{
#if (HAL_LED == TRUE)
  /* Load back the saved state */
  HalLedOnOff(HalSleepLedState, HAL_LED_MODE_ON);

  /* Restart - This takes care BLINKING LEDS */
  HalLedUpdate();
#endif /* HAL_LED */

#ifdef BLINK_LEDS
  /* Sleep OFF */
  HalLedStatusControl.sleepActive = FALSE;
#endif /* BLINK_LEDS */
}

/***************************************************************************************************
 * @fn      HalledBreathHandle
 *
 * @brief   Breath LEDs
 *
 * @param   none
 *
 * @return  none
 ***************************************************************************************************/
uint32_t HalledBreathHandle(HalledBreath_t* breath)
{
    uint32_t min, max, cal;

    cal = breath->cal;
    min = (uint32_t)(breath->min << 8);
    max = (uint32_t)(breath->max << 8);

    if (breath->bdir)
    {
        if (cal < min)
            cal = min;

        cal += cal >> breath->inc;

        if (cal >= max)
        {
            cal = max;
            breath->bdir = 0;
        }
    }
    else
    {
        cal -= cal >> breath->inc;

        if (cal <= min)
        {
            cal = min;
            breath->bdir = 1;
        }
    }
    breath->cal = cal;
    breath->out = (uint16_t)(cal >> 8);
    return (breath->out);
}