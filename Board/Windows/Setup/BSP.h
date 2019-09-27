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
Purpose : BSP (Board support package)
*/

#ifndef BSP_H
#define BSP_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// In order to avoid warnings for undefined parameters
//
#ifndef BSP_USE_PARA
  #define BSP_USE_PARA(para)  (void) (para)
#endif

#if (defined(__ICCARM__) && (__CPU_MODE__ == 1))  // If IAR and THUMB mode
  #define INTERWORK  __interwork
#else
  #define INTERWORK
#endif

/*********************************************************************
*
*       Basic type defines
*
**********************************************************************
*/
#ifndef   OS_I8
  #define OS_I8     signed char
#endif

#ifndef   OS_U8
  #define OS_U8     unsigned char
#endif

#ifndef   OS_I16
  #define OS_I16    signed short
#endif

#ifndef   OS_U16
  #define OS_U16    unsigned short
#endif

#ifndef   OS_I32
  #define OS_I32    long
#endif

#ifndef   OS_U32
  #define OS_U32    unsigned OS_I32
#endif

#ifdef    OS_I64
  #ifndef   OS_U64
    #define OS_U64  unsigned OS_I64
  #endif
#endif


//
// Make it possible to place const data in RAM via
// define and therefore via command line. This is required
// for some smaller CPUs in small memory models
// It is defined to be "const" by default, but can be defined to nothing
// by simply adding a -DOS_CONST_DATA on the commandline
//

#ifndef   OS_CODE_SECTION_ATTRIBUTE
  #define OS_CODE_SECTION_ATTRIBUTE
#endif

/*********************************************************************
*
*       Core / compiler specific settings
*
**********************************************************************
*/
//
#ifndef   OS_USEPARA
  #define OS_USEPARA(para)                   (para)=(para)
#endif

/*********************************************************************
*
*       BSP related (RTOSInit)
*
**********************************************************************
*/
void   OS_InitHW          (void)            OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

void          BSP_Init        (void);
void          BSP_SetLED      (int Index);
void          BSP_ClrLED      (int Index);
void          BSP_ToggleLED   (int Index);
int           BSP_GetLEDState (int Index);

#ifdef __cplusplus
}
#endif

#endif  // BSP_H

/*************************** End of file ****************************/
