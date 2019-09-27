/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                         STM32H743ZI NUCLEO
*
* Filename : bsp_led.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "stm32l4xx_hal.h"

#include "OSAL_Comdef.h"

#include "bsp_led.h"

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  USER_LD1_GPIOB_PIN             GPIO_PIN_6
#define  USER_LD2_GPIOE_PIN             GPIO_PIN_3
#define  USER_LD3_GPIOD_PIN             GPIO_PIN_15

#define  USER_LED1_GPIO_PORT             GPIOB
#define  USER_LED2_GPIO_PORT             GPIOE
#define  USER_LED3_GPIO_PORT             GPIOD


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initializes the required pins that control the LEDs.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Init (void)
{
    GPIO_InitTypeDef  gpio_init;


    __HAL_RCC_GPIOB_CLK_ENABLE();                               /* Enable GPIO clock for USER LEDs                      */
    __HAL_RCC_GPIOE_CLK_ENABLE();                               /* Enable GPIO clock for USER LEDs                      */
    __HAL_RCC_GPIOD_CLK_ENABLE();                               /* Enable GPIO clock for USER LEDs                      */

                                                                /* ----------- CONFIGURE GPIOB FOR USER LEDS ---------- */
    gpio_init.Pin   = USER_LD1_GPIOB_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(USER_LED1_GPIO_PORT, &gpio_init);

    gpio_init.Pin   = USER_LD2_GPIOE_PIN;
    HAL_GPIO_Init(USER_LED2_GPIO_PORT, &gpio_init);

    gpio_init.Pin   = USER_LD3_GPIOD_PIN;
    HAL_GPIO_Init(USER_LED3_GPIO_PORT, &gpio_init);
}


/*
*********************************************************************************************************
*                                            BSP_LED_On()
*
* Description : Turn ON a specific LED.
*
* Argument(s) : led    The ID of the LED to control.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (BSP_LED  led)
{
    switch (led) {
        case USER_LED_ALL:
             HAL_GPIO_WritePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN, GPIO_PIN_SET);
             HAL_GPIO_WritePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN, GPIO_PIN_SET);
             HAL_GPIO_WritePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN, GPIO_PIN_SET);
             break;


        case USER_LD1:
             HAL_GPIO_WritePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN, GPIO_PIN_SET);
             break;


        case USER_LD2:
             HAL_GPIO_WritePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN, GPIO_PIN_SET);
             break;


        case USER_LD3:
             HAL_GPIO_WritePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN, GPIO_PIN_SET);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : Turn OFF a specific LED.
*
* Argument(s) : led    The ID of the LED to control.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (BSP_LED  led)
{
    switch (led) {
        case USER_LED_ALL:
             HAL_GPIO_WritePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN, GPIO_PIN_RESET);
             HAL_GPIO_WritePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN, GPIO_PIN_RESET);
             HAL_GPIO_WritePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN, GPIO_PIN_RESET);
             break;


        case USER_LD1:
             HAL_GPIO_WritePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN, GPIO_PIN_RESET);
             break;


        case USER_LD2:
             HAL_GPIO_WritePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN, GPIO_PIN_RESET);
             break;


        case USER_LD3:
             HAL_GPIO_WritePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN, GPIO_PIN_RESET);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                          BSP_LED_Read()
*
* Description : Toggles a specific LED.
*
* Argument(s) : led    The ID of the LED to control.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

uint32  BSP_LED_Read (BSP_LED  led)
{
    uint8_t LedState = 0;
    switch (led) {
        case USER_LED_ALL:
             LedState |= HAL_GPIO_ReadPin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN);
             LedState |= HAL_GPIO_ReadPin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN) << 1;
             LedState |= HAL_GPIO_ReadPin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN) << 2;
             break;


        case USER_LD1:
             LedState =  HAL_GPIO_ReadPin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN);
             break;


        case USER_LD2:
             LedState =  HAL_GPIO_ReadPin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN);
             break;


        case USER_LD3:
             LedState =  HAL_GPIO_ReadPin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN);
             break;


        default:
             break;
    }
    
    return LedState;
}

/*
*********************************************************************************************************
*                                          BSP_LED_Toggle()
*
* Description : Toggles a specific LED.
*
* Argument(s) : led    The ID of the LED to control.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (BSP_LED  led)
{
    switch (led) {
        case USER_LED_ALL:
             HAL_GPIO_TogglePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN);
             HAL_GPIO_TogglePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN);
             HAL_GPIO_TogglePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN);
             break;


        case USER_LD1:
             HAL_GPIO_TogglePin(USER_LED1_GPIO_PORT, USER_LD1_GPIOB_PIN);
             break;


        case USER_LD2:
             HAL_GPIO_TogglePin(USER_LED2_GPIO_PORT, USER_LD2_GPIOE_PIN);
             break;


        case USER_LD3:
             HAL_GPIO_TogglePin(USER_LED3_GPIO_PORT, USER_LD3_GPIOD_PIN);
             break;


        default:
             break;
    }
}
