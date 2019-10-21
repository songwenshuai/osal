
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
    static int32 oldtime = 0, new_time = 0, deviation = 0;

    new_time = osal_GetSystemClock();
    deviation = ABS((new_time - oldtime) - SBP_PERIODIC_EVT_DELAY);
    oldtime = new_time;
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

void cb_timer_test(uint8* pData){
        static int32 oldtime1 = 0, new_time1 = 0, deviation1 = 0;
        
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
