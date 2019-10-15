/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#ifndef _WIN32
#ifdef STM32L496xx
#include "stm32l4xx_hal.h"
#endif

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#endif
#else
#include "Windows.h"
#include "stdio.h"
#endif

#include "OSAL.h"
#include "OSAL_Clock.h"

/*********************************************************************
 * MACROS
 */

#define TICK_IN_MS 1 /* 1 millisecond */ 

/* Remainder when divided by 4 */
#define byte_offset(addr) ((uint32)addr & 3)

/* Greatest-multiple-of-4 <= addr */
#define aligned_address(addr) ((uint32)addr & ~3)

#define HAL_NV_ADDR_OFFSET(p_addr)        (((uint32)p_addr) - HAL_NV_START_ADDR)

#define OSAL_NV_PTR_TO_PAGE( p_addr )   (HAL_NV_ADDR_OFFSET(p_addr) / HAL_FLASH_PAGE_SIZE)
#define OSAL_NV_PTR_TO_OFFSET( p_addr ) (HAL_NV_ADDR_OFFSET(p_addr) % HAL_FLASH_PAGE_SIZE)

/*********************************************************************
 * GLOBAL VARIABLES
 */

#ifdef STM32L496xx
extern __IO uint32_t uwTick;
extern uint32_t uwTickFreq;  /* 1KHz */
#endif

#ifdef STM32F103xB
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;  /* 1KHz */
#endif

uint8 nvDataBuf[HAL_NV_PAGE_CNT][HAL_FLASH_PAGE_SIZE];

/*********************************************************************
 * EXTERN FUNCTIONS
 */
#ifndef _WIN32
extern int putc(int ch);
#endif

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
#ifndef _WIN32
    HAL_ResumeTick();
#endif
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
#ifndef _WIN32
    HAL_SuspendTick();
#endif
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
#ifndef _WIN32
    HAL_Delay(osal_timeout);
#else
    Sleep(osal_timeout);
#endif
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
#ifndef _WIN32
  uwTick += uwTickFreq;

  /* Update OSAL timer and clock */
  osalAdjustTimer(TICK_IN_MS);
#endif
}

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
#ifndef _WIN32
  // send char to console etc.
  int c = (int)character;

  if (c == '\n')
    putc('\r');
  putc(c);
#else
  // send char to console etc.
  int c = (int)character;

  if (c == '\n')
    putchar('\r');
  putchar(c);
#endif
}

/*********************************************************************
 * @fn      flash_write_word
 *
 * @brief   Writes 4bytes of data to address ulAddress
 *
 * @param   ulAddress - Address to which data has to be written
 *          address has to be 4byte-aligned.
 *
 * @param   data - 4byte data
 *
 * @return  none
 */
static void flash_write_word( uint32 *ulAddress, uint32 data )
{
  /* -- sws del
  ROM_ProgramFlash((uint32_t *)&data, (uint32_t)ulAddress, 4);
  */
  // ++ sws add
  *(uint32*)(&nvDataBuf[OSAL_NV_PTR_TO_PAGE(ulAddress)][OSAL_NV_PTR_TO_OFFSET(ulAddress)]) = data;
}

/*********************************************************************
 * @fn      initFlash
 *
 * @brief   Sets the clock parameter required by the flash-controller
 *
 * @param   none
 *
 * @return  none
 */
void initFlash( void )
{
  //FlashUsecSet( HAL_CPU_CLOCK_MHZ - 1 );

  // ++ sws add start
  halIntState_t IntState;
  uint16 offset;
  uint8 pg;

  HAL_ENTER_CRITICAL_SECTION(IntState);
  for (pg = 0; pg < HAL_NV_PAGE_CNT; pg++)
  {
      for (offset = 0; offset < HAL_FLASH_PAGE_SIZE; offset++)
      {
          nvDataBuf[pg][offset] = 0xFF;
      }
  }
  HAL_EXIT_CRITICAL_SECTION(IntState);
  // ++ sws add end
}

/*********************************************************************
 * @fn      flashErasePage
 *
 * @brief   Erases the page pointed by addr
 *
 * @param   addr - Address of the page to be erased.
 *          addr has to be page aligned.
 *
 * @return  none
 */
void flashErasePage( uint8 *addr )
{
  halIntState_t IntState;

  /* Set the clock frequency */
  /* -- sws del
  initFlash();
  */

  HAL_ENTER_CRITICAL_SECTION( IntState );

  /* Erase flash */
  /* -- sws del
  FlashMainPageErase( (unsigned long)addr );
  */

  // ++ sws add start
  uint16 cnt = HAL_FLASH_PAGE_SIZE;
  uint8* pData = nvDataBuf[(HAL_NV_ADDR_OFFSET(addr) / HAL_FLASH_PAGE_SIZE)];

  while (cnt--)
  {
    *pData++ = 0xFF;
  }
  // ++ sws add end

  HAL_EXIT_CRITICAL_SECTION( IntState );
}

/*********************************************************************
 * @fn      flashWrite
 *
 * @brief   Copies the data from buf(pointer) to
 * addr(pointer to flash memory). addr need not be aligned.
 * One can write the flash only in multiples of 4. The below logic is
 * required to implement data transfer of any number of bytes at any address
 *
 * @param  addr - To-address of the data
 * @param  len - Number of bytes to be transfered
 * @param  buf - From-address of the data
 *
 * @return len - None
 */
void flashWrite( uint8 *addr, uint16 len, uint8 *buf )
{
  if ( len > 0 )
  {
    /* 4-byte aligned pointer */
  uint32 *uint32ptr;
    /* 4-byte temporary variable */
  uint32 temp_u32;
  uint16 i = 0, j;

  /* start_bytes - unaligned byte count at the beggining
   * middle_bytes - aligned byte count at the middle
   * end_bytes - unaligned byte count at the end
   */
  uint16 start_bytes = 0, middle_bytes = 0, end_bytes = 0;
  halIntState_t IntState;

  /* Set the clock frequency */
  /* -- sws del
  initFlash();
  */

  /* Extract 4-byte aligned address */
  uint32ptr = (uint32 *)aligned_address(addr);

  /* Calculate the start_bytes */
    /* If the addr is not 4-byte aligned */
    if( byte_offset(addr) )
  {
      /* If the start-address and the end-address are in the
     * same 4-byte-aligned-chunk.
     */
    if((((uint32)addr) >> 2) == ((((uint32)addr) + len) >> 2))
    {
      start_bytes = len;
    }
    else
    {
      start_bytes = 4 - (byte_offset(addr));
    }
  }

  /* Calculate the middle_bytes and end_bytes */
    /* If there are any bytes left */
    if( (len - start_bytes) > 0 )
  {
    /* Highest-multiple-of-4 less than (len - start_bytes) */
    middle_bytes = ((len - start_bytes) & (~3));
    /* Remainder when divided by 4 */
    end_bytes = (len - start_bytes) & 3;
  }

    HAL_ENTER_CRITICAL_SECTION( IntState );

  /* Write the start bytes to the flash */
    if( start_bytes > 0 )
  {
    /* Take the first 4-byte chunk into a temp_u32 */
    temp_u32 = *uint32ptr;
    /* Write the required bytes into temp_u32 */
    for(; i < start_bytes; i++)
    {
      *(((uint8 *)(&temp_u32)) + i + byte_offset(addr)) = buf[i];
    }
    /* Write the 4-byte chunk into the flah */
    flash_write_word(uint32ptr, temp_u32);
    /* Increment the 4-byte-aligned-address by 4 */
    uint32ptr++;
  }

  /* Write the middle bytes to the flash */
    while( i < start_bytes + middle_bytes )
  {
    /* Extract 4 bytes into from the buf */
      *((uint8*)(&temp_u32)) = buf[i++];
      *((uint8*)(&temp_u32) + 1) = buf[i++];
      *((uint8*)(&temp_u32) + 2) = buf[i++];
      *((uint8*)(&temp_u32) + 3) = buf[i++];

    /* Write the 4-byte chunk into the flash */
      flash_write_word( uint32ptr, temp_u32 );
    /* Increment the 4-byte-aligned-address by 4 */
    uint32ptr++;
  }

  /* Write the end bytes to the flash */
    if( end_bytes > 0 )
  {
    j = 0;
    /* Take the first 4-byte chunk into a temp_u32 */
    temp_u32 = *uint32ptr;
    for(; i < len; i++)
    {
      *((uint8 *)&temp_u32 + j) = buf[i];
      j++;
    }
    /* Write the 4-byte chunk into the flash */
      flash_write_word( uint32ptr, temp_u32 );
  }

  HAL_EXIT_CRITICAL_SECTION( IntState );
  }
}
