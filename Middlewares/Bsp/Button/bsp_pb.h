/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PB_H
#define __BSP_PB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/**
  * @}
  */

/** @defgroup BSP BSP Nucleo 144
  * @{
  */

typedef enum
{
  BUTTON_A = 0,
  BUTTON_M = 1,
  BUTTON_B = 2,
}Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

/**
  * @}
  */

/** @defgroup BUTTON
  * @{
  */
#define BUTTONn                                 3

/**
 * @brief Key push-button
 */
#define USER_BUTTON_A_PIN                          GPIO_PIN_14
#define USER_BUTTON_A_GPIO_PORT                    GPIOE
#define USER_BUTTON_A_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define USER_BUTTON_A_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()
#define USER_BUTTON_A_EXTI_LINE                    GPIO_PIN_14
#define USER_BUTTON_A_EXTI_IRQn                    EXTI15_10_IRQn

#define USER_BUTTON_M_PIN                          GPIO_PIN_11
#define USER_BUTTON_M_GPIO_PORT                    GPIOE
#define USER_BUTTON_M_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define USER_BUTTON_M_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()
#define USER_BUTTON_M_EXTI_LINE                    GPIO_PIN_11
#define USER_BUTTON_M_EXTI_IRQn                    EXTI15_10_IRQn

#define USER_BUTTON_B_PIN                          GPIO_PIN_10
#define USER_BUTTON_B_GPIO_PORT                    GPIOE
#define USER_BUTTON_B_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define USER_BUTTON_B_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()
#define USER_BUTTON_B_EXTI_LINE                    GPIO_PIN_10
#define USER_BUTTON_B_EXTI_IRQn                    EXTI15_10_IRQn

/**
  * @}
  */

/** @defgroup Exported Functions
  * @{
  */
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_PB_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
