/**************************************************************************************************
  Filename:       hal_led.h
  Revised:        $Date: 2007-07-06 10:42:24 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Description:    This file contains the interface to the LED Service.

**************************************************************************************************/

#ifndef HAL_LED_H
#define HAL_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * MACROS
 */
#define HAL_LED    TRUE
#define BLINK_LEDS TRUE

/*********************************************************************
 * CONSTANTS
 */

/* LEDS - The LED number is the same as the bit position */
#define HAL_LED_1     0x01
#define HAL_LED_2     0x02
#define HAL_LED_3     0x04
#define HAL_LED_ALL   (HAL_LED_1 | HAL_LED_2 | HAL_LED_3)

/* Modes */
#define HAL_LED_MODE_OFF     0x00
#define HAL_LED_MODE_ON      0x01
#define HAL_LED_MODE_BLINK   0x02
#define HAL_LED_MODE_FLASH   0x04
#define HAL_LED_MODE_TOGGLE  0x08

/* Defaults */
#define HAL_LED_DEFAULT_MAX_LEDS      4
#define HAL_LED_DEFAULT_DUTY_CYCLE    50
#define HAL_LED_DEFAULT_FLASH_COUNT   50
#define HAL_LED_DEFAULT_FLASH_TIME    1000

/*********************************************************************
 * TYPEDEFS
 */

typedef struct {
    uint8_t  inc;    // step value (4-8)
    uint8_t  bdir;   // Switch from light to dark or from dark to light
    uint16_t min;    // minimum brightness (not 0)
    uint16_t max;    // maximum brightness
    uint16_t out;    // Brightness output
    uint32_t cal;    // initial brightness (not 0)
} HalledBreath_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*
 * Initialize LED Service.
 */
extern void HalLedInit( void );

/*
 * Set the LED ON/OFF/TOGGLE.
 */
extern uint8_t HalLedSet( uint8_t led, uint8_t mode );

/*
 * Blink the LED.
 */
extern void HalLedBlink( uint8_t leds, uint8_t cnt, uint8_t duty, uint16_t time );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
