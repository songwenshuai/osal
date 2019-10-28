/**
  ******************************************************************************
  * @file    drv_flash.c 
  * @author  MCD Application Team
  * @brief   This file provides all the memory related operation functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32L4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/

#include "drv_flash.h"
#include "printf.h"
#include "fal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t addr)
{
    uint32_t page = 0;
    page = RT_ALIGN_DOWN(addr, STM32_FLASH_PAGE_SIZE);
    return page;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
int stm32_flash_read(uint32_t addr, uint8_t *buf, size_t size)
{
    size_t i;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        printf("read outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -FLASHIF_EINVAL;
    }

    for (i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
int stm32_flash_write(uint32_t addr, const uint8_t *buf, size_t size)
{
    uint32_t result        = FLASHIF_OK;
    uint32_t end_addr   = addr + size;

    if (addr % 4 != 0)
    {
        printf("write addr must be 4-byte alignment");
        return -FLASHIF_EINVAL;
    }

    if ((end_addr) > STM32_FLASH_END_ADDRESS)
    {
        printf("write outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -FLASHIF_EINVAL;
    }

    HAL_FLASH_Unlock();

    while (addr < end_addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *((uint32_t *)buf)) == HAL_OK)
        {
            if (*(uint32_t *)addr != *(uint32_t *)buf)
            {
                result = -FLASHIF_ERASEKO;
                break;
            }
            addr += 4;
            buf  += 4;
        }
        else
        {
            result = -FLASHIF_ERASEKO;
            break;
        }
    }

    HAL_FLASH_Lock();

    if (result != FLASHIF_OK)
    {
        return result;
    }

    return size;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int stm32_flash_erase(uint32_t addr, size_t size)
{
    uint32_t result = FLASHIF_OK;
    uint32_t PAGEError = 0;

    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        printf("ERROR: erase outrange flash size! addr is (0x%p)\n", (void *)(addr + size));
        return -FLASHIF_EINVAL;
    }

    HAL_FLASH_Unlock();

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = GetPage(addr);
    EraseInitStruct.NbPages     = (size + STM32_FLASH_PAGE_SIZE - 1) / STM32_FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        result = -FLASHIF_ERASEKO;
        goto __exit;
    }

__exit:
    HAL_FLASH_Lock();

    if (result != FLASHIF_OK)
    {
        return result;
    }

    printf("erase done: addr (0x%p), size %d", (void *)addr, size);
    return size;
}

static int fal_flash_read(long offset, uint8_t *buf, size_t size);
static int fal_flash_write(long offset, const uint8_t *buf, size_t size);
static int fal_flash_erase(long offset, size_t size);

const struct fal_flash_dev stm32_onchip_flash = { "onchip_flash", STM32_FLASH_START_ADRESS, STM32_FLASH_SIZE, STM32_FLASH_PAGE_SIZE, {NULL, fal_flash_read, fal_flash_write, fal_flash_erase} };

static int fal_flash_read(long offset, uint8_t *buf, size_t size)
{
    return stm32_flash_read(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_write(long offset, const uint8_t *buf, size_t size)
{
    return stm32_flash_write(stm32_onchip_flash.addr + offset, buf, size);
}

static int fal_flash_erase(long offset, size_t size)
{
    return stm32_flash_erase(stm32_onchip_flash.addr + offset, size);
}

/* Public functions ---------------------------------------------------------*/

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
