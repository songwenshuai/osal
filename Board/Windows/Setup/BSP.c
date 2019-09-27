/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2019 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 5.06.1                                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for embOS simulation
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "BSP.h"
#include "SIM_OS.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
static unsigned int LEDs;

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  LEDs = 0;
  SIM_OS_UpdateWindow();
}

/*********************************************************************
*
*       BSP_SetLED()
*/
void BSP_SetLED(int Index) {
  if (Index < 32) {
    LEDs |= (1u << Index);
    SIM_OS_UpdateWindow();
  }
}

/*********************************************************************
*
*       BSP_ClrLED()
*/
void BSP_ClrLED(int Index) {
  if (Index < 32) {
    LEDs &= ~(1u << Index);
    SIM_OS_UpdateWindow();
  }
}

/*********************************************************************
*
*       BSP_ToggleLED()
*/
void BSP_ToggleLED(int Index) {
  if (Index < 32) {
    LEDs ^= (1u << Index);
    SIM_OS_UpdateWindow();
  }
}

/*********************************************************************
*
*       BSP_GetLEDState()
*/
int BSP_GetLEDState(int Index) {
  return (LEDs & (1u << Index));
}

/****** End Of File *************************************************/
