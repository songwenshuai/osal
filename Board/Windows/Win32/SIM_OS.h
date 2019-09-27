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

-------------------------- END-OF-HEADER -----------------------------

File    : SIM_OS.h
Purpose : Function declarations for GUI simulation
*/

#ifndef SIM_OS_H
#define SIM_OS_H


#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*        Compatibility with older embOS simulation versions
*
**********************************************************************
*/

#define SIM_Init(x)  SIM_OS_InitWindow()
#define SIM_Update   SIM_OS_UpdateWindow
#define SIM_Paint    SIM_OS_PaintWindow

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

int  SIM_OS_InitWindow         (void);
void SIM_OS_UpdateWindow       (void);
void SIM_OS_PaintWindow        (void* hDC);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion

/*************************** End of file ****************************/
