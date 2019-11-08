/*******************************************************************************
  Filename:       OSAL_Flashutil8.c
  Revised:        $Date: 2013-05-09 21:41:33 -0700 (Thu, 09 May 2013) $
  Revision:       $Revision: 34219 $

  Description:    Utility functions to erase/write flash memory pages.
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "OSAL_Flashutil8.h"

/*********************************************************************
 * MACROS
 */

/* Remainder when divided by 4 */
#define byte_offset(addr)               ((uint32)addr & 3)

/* Greatest-multiple-of-4 <= addr */
#define aligned_address(addr)           ((uint32)addr & ~3)

#define HAL_NV_ADDR_OFFSET(p_addr)      (((uint32)p_addr) - HAL_NV_START_ADDR)
#define OSAL_NV_PTR_TO_PAGE( p_addr )   (HAL_NV_ADDR_OFFSET(p_addr) / HAL_FLASH_PAGE_SIZE)
#define OSAL_NV_PTR_TO_OFFSET( p_addr ) (HAL_NV_ADDR_OFFSET(p_addr) % HAL_FLASH_PAGE_SIZE)

/*********************************************************************
 * GLOBAL VARIABLES
 */

uint8 nvDataBuf[HAL_NV_PAGE_CNT][HAL_FLASH_PAGE_SIZE];

/*********************************************************************
 * GLOBAL FUNCTION
 */

/**************************************************************************************************
 * @fn          HalFlashRead
 *
 * @brief       This function reads 'cnt' bytes from the internal flash.
 *
 * input parameters
 *
 * @param       pg - A valid flash page number.
 * @param       offset - A valid offset into the page.
 * @param       buf - A valid buffer space at least as big as the 'cnt' parameter.
 * @param       cnt - A valid number of bytes to read.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt)
{
  // Calculate the offset into the containing flash bank as it gets mapped into XDATA.
  uint8 *ptr = (uint8 *)(offset + HAL_FLASH_PAGE_MAP) +
               ((pg % HAL_FLASH_PAGE_PER_BANK) * HAL_FLASH_PAGE_SIZE);
  halIntState_t is;

  HAL_ENTER_CRITICAL_SECTION(is);

  while (cnt--)
  {
    *buf++ = *ptr++;
  }

  HAL_EXIT_CRITICAL_SECTION(is);
}

/**************************************************************************************************
 * @fn          HalFlashWrite
 *
 * @brief       This function writes 'cnt' bytes to the internal flash.
 *
 * input parameters
 *
 * @param       addr - Valid HAL flash write address: actual addr / 4 and quad-aligned.
 * @param       buf - Valid buffer space at least as big as 'cnt' X 4.
 * @param       cnt - Number of 4-byte blocks to write.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashWrite(uint16 addr, uint8 *buf, uint16 cnt)
{

}

/**************************************************************************************************
 * @fn          HalFlashErase
 *
 * @brief       This function erases the specified page of the internal flash.
 *
 * input parameters
 *
 * @param       pg - A valid flash page number to erase.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashErase(uint8 pg)
{

}

/*********************************************************************
*********************************************************************/
