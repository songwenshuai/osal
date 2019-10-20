/**
  ******************************************************************************
  * @file    IAP_Main/Inc/flash_if.h 
  * @author  MCD Application Team
  * @brief   This file provides all the headers of the flash_if functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

enum 
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_EINVAL,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR,
  FLASHIF_PROTECTION_ERRROR
};

/* Exported macro ------------------------------------------------------------*/

#define STM32_FLASH_START_ADRESS               ((uint32_t)0x08000000)
#define STM32_FLASH_PAGE_SIZE                  ((uint32_t)0x800)
#define STM32_FLASH_SIZE                       (1024 * 1024)
#define STM32_FLASH_END_ADDRESS                ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#define STM32_FLASH_PAGE_TO_PTR( page )        (STM32_FLASH_START_ADRESS + ( page * STM32_FLASH_PAGE_SIZE)
#define STM32_FLASH_ADDR_OFFSET( p_addr )      (((uint32)p_addr) - STM32_FLASH_START_ADRESS)
#define STM32_FLASH_PTR_TO_PAGE( p_addr )      (STM32_FLASH_ADDR_OFFSET(p_addr) / STM32_FLASH_PAGE_SIZE)
#define STM32_FLASH_PTR_TO_OFFSET( p_addr )    (STM32_FLASH_ADDR_OFFSET(p_addr) % STM32_FLASH_PAGE_SIZE)

#define STM32_FLASH_PAGE_NBPERBANK             256
#define STM32_FLASH_BANK_NUMBER                2

/* Exported functions ------------------------------------------------------- */


int stm32_flash_read(uint32_t addr, uint8_t *buf, size_t size);
int stm32_flash_write(uint32_t addr, const uint8_t *buf, size_t size);
int stm32_flash_erase(uint32_t addr, size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* __FLASH_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
