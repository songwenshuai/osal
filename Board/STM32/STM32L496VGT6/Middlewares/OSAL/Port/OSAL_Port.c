/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "stm32l4xx_hal.h"


#include "OSAL.h"
#include "OSAL_Clock.h"

#include "SEGGER_SYSVIEW_Conf.h"
#include "SEGGER_SYSVIEW.h"

/*********************************************************************
 * MACROS
 */

#define TICK_IN_MS 1 /* 1 millisecond */ 

#if defined(_NO_PRINTF)
#define UART_TIMEOUT_VALUE   1000
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* _NO_PRINTF */

/*********************************************************************
 * GLOBAL VARIABLES
 */

extern __IO uint32_t uwTick;
extern uint32_t uwTickFreq;  /* 1KHz */


extern UART_HandleTypeDef hlpuart1;

/*********************************************************************
 * EXTERN FUNCTIONS
 */
extern int putc(int ch);

 /*********************************************************************
  * FUNCTIONS
  */

/***************************************************************************************************
 * @fn      SysTickIntEnable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntEnable(void)
{
    HAL_ResumeTick();
}


/***************************************************************************************************
 * @fn      SysTickIntDisable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntDisable(void)
{
    HAL_SuspendTick();
}

/*******************************************************************************
 * @fn          halSleep
 *
 * @brief       This function is called from the OSAL task loop using and
 *              existing OSAL interface. It determines if an OSAL timer is
 *              pending, in which case it sets up the RTC to wake the device
 *              for that event, and puts the device to sleep. The OSAL timers
 *              are adjusted upon wake in case the device was awoken sooner due
 *              to some other interrupt. If no OSAL timer event is pending,
 *              then the device is put to sleep.
 *
 *              Note: Presently, only CM3 is powered down.
 *
 * input parameters
 *
 * @param       osal_timeout - Next OSAL timer timeout, in msec.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halSleep( uint32 osal_timeout )
{
    //Sleep the task for the specified duration
    HAL_Delay(osal_timeout);
}

/***************************************************************************************************
 * @fn      This function is called to increment  a global variable "uwTick"
 *          used as application time base.
 *
 * @note    In the default implementation, this variable is incremented each 1ms
 *          in SysTick ISR.
 * 
 * @note This function is declared as __weak to be overwritten in case of other
 *      implementations in user file.
 * 
 * @brief   The Systick Interrupt module
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void HAL_IncTick(void)
{
  SEGGER_SYSVIEW_RecordEnterISR();

  uwTick += uwTickFreq;

  /* Update OSAL timer and clock */
  osalAdjustTimer(TICK_IN_MS);

  SEGGER_SYSVIEW_RecordExitISR();
}

/***************************************************************************************************
 * @fn      OSAL_Init_Hook
 *
 * @brief   Hook Osal init function
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void OSAL_Init_Hook(void)
{

  SEGGER_SYSVIEW_Conf();            /* Configure and initialize SystemView  */

}

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
  hlpuart1.Instance->TDR = ch;
  while ((__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_TC) == RESET));

  return 1;
}

/**
  * @brief  Retargets the C library scanf function to the USART2.
  * @param  f: pointer to file (not used)
  * @retval The character received
  */
int fgetc(void)
{
  int ch;

  ch = -1;
  if (__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_RXNE) != RESET)
    ch = hlpuart1.Instance->RDR & 0xff;

  return ch;
}

/* FUNCTION: kbhit()
 *
 * Tests if there is a character available from the keyboard
 *
 * PARAMS: none
 *
 * RETURN: TRUE if a character is available, otherwise FALSE
 */
int kbhit(void)
{
  if (__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_RXNE) != RESET)
  {
    return TRUE;
  }
  return FALSE;
}

#endif /* _NO_PRINTF */

/***************************************************************************************************
 * @fn      _putchar
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void _putchar(char character)
{
  // send char to console etc.
  int c = (int)character;

  if (c == '\n')
    putc('\r');
  putc(c);
}
