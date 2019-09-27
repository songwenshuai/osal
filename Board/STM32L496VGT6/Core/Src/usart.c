/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

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

/* USER CODE END 0 */

UART_HandleTypeDef hlpuart1;

/* LPUART1 init function */

void MX_LPUART1_UART_Init(void)
{

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */
    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**LPUART1 GPIO Configuration    
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();
  
    /**LPUART1 GPIO Configuration    
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

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

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
