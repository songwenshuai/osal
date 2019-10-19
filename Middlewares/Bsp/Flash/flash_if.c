/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
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
#include "flash_if.h"
#include "printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
    uint32_t page = 0;

    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        /* Bank 1 */
        page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
        /* Bank 2 */
        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    }

    return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
    uint32_t bank = 0;

    if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
    {
        /* No Bank swap */
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
        {
            bank = FLASH_BANK_1;
        }
        else
        {
            bank = FLASH_BANK_2;
        }
    }
    else
    {
        /* Bank swap */
        if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
        {
            bank = FLASH_BANK_2;
        }
        else
        {
            bank = FLASH_BANK_1;
        }
    }
    return bank;
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
        printf("read outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return FLASHIF_EINVAL;
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
    size_t i, j;
    uint32_t result = FLASHIF_OK;
    uint64_t write_data = 0, temp_data = 0;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        printf("ERROR: write outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return FLASHIF_EINVAL;
    }
    
    if(addr % 8 != 0)
    {
        printf("write addr must be 8-byte alignment\n");
        return FLASHIF_EINVAL;
    }

    if (size < 1)
    {
        printf("write error[1]: addr (0x%p), size %d\n", (void*)addr, size);
        return FLASHIF_ERASEKO;
    }

    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

    for (i = 0; i < size;)
    {
        if ((size - i) < 8)
        {
            for (j = 0; (size - i) > 0; i++, j++)
            {
                temp_data = *buf;
                write_data = (write_data) | (temp_data << 8 * j);
                buf ++;
            }
        }
        else
        {
            for (j = 0; j < 8; j++, i++)
            {
                temp_data = *buf;
                write_data = (write_data) | (temp_data << 8 * j);
                buf ++;
            }
        }

        /* write data */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, write_data) == HAL_OK)
        {
            /* Check the written value */
            if (*(uint64_t*)addr != write_data)
            {
                printf("ERROR: write data != read data\n");
                result = FLASHIF_ERASEKO;
                goto __exit;
            }
        }
        else
        {
            printf("write error: addr (0x%p), size %d, error code %x\n", (void*)addr, size, pFlash.ErrorCode);
            result = FLASHIF_ERASEKO;
            goto __exit;
        }

        temp_data = 0;
        write_data = 0;

        addr += 8;
    }

__exit:
    HAL_FLASH_Lock();

    return result;
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
    uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
    uint32_t PAGEError = 0;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        printf("ERROR: erase outrange flash size! addr is (0x%p)\n", (void*)(addr + size));
        return FLASHIF_EINVAL;
    }

    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    /* Get the 1st page to erase */
    FirstPage = GetPage(addr);
    /* Get the number of pages to erase from 1st page */
    NbOfPages = GetPage(addr + size - 1) - FirstPage + 1;
    /* Get the bank */
    BankNumber = GetBank(addr);
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = BankNumber;
    EraseInitStruct.Page        = FirstPage;
    EraseInitStruct.NbPages     = NbOfPages;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        printf("erase error: addr (0x%p), size %d\n", (void*)addr, size);
        result = FLASHIF_ERASEKO;
        goto __exit;
    }

__exit:
    HAL_FLASH_Lock();

    printf("erase done: addr (0x%p), size %d", (void*)addr, size);

    return result;
}

/* Public functions ---------------------------------------------------------*/

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
