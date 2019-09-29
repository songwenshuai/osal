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
    uint8  inc;    // step value (4-8)
    uint8  bdir;   // Switch from light to dark or from dark to light
    uint16 min;    // minimum brightness (not 0)
    uint16 max;    // maximum brightness
    uint16 out;    // Brightness output
    uint32 cal;    // initial brightness (not 0)
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
extern uint8 HalLedSet( uint8 led, uint8 mode );

/*
 * Blink the LED.
 */
extern void HalLedBlink( uint8 leds, uint8 cnt, uint8 duty, uint16 time );

/*
 * Put LEDs in sleep state - store current values
 */
extern void HalLedEnterSleep( void );

/*
 * Retore LEDs from sleep state
 */
extern void HalLedExitSleep( void );

/*
 * Return LED state
 */
extern uint8 HalLedGetState ( void );

/*
 * Breath led
 */
extern uint32 HalledBreathHandle(HalledBreath_t* breath);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
