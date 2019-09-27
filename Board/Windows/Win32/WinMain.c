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

File    : WinMain.c
Purpose : Win32 entry point used to initialize the simulation before
          calling the main() entry point which shall be used as the
          main entry point of the simulated application.
*/

#include <Windows.h>
#include "SIM_OS.h"

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif
int main(void);
#ifdef __cplusplus
}
#endif

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       WinMain()
*
*  Function description
*    Initializes the embOS simulation and calls the main() routine.
*
*  Parameters
*    hInstance    : A handle to the current instance of the application.
*    hPrevInstance: A handle to the previous instance of the application. This parameter is always NULL.
*    lpCmdLine    : The command line for the application, excluding the program name.
*    nCmdShow     : Controls how the window is to be shown.
*
*  Return value
*    == 0: Execution has properly terminated.
*    == 1: Could not create "embOS Simulation" window.
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  //
  // Avoid warnings for unused parameters.
  //
  (void)hInstance;
  (void)hPrevInstance;
  (void)lpCmdLine;
  (void)nCmdShow;
  //
  // Initialize the Win32 specific components of the embOS simulation.
  // If no Win32 window is desired, this initialization can be removed
  // and main() can be called directly.
  //
  SIM_OS_InitWindow();
  //
  // Call the main entry point of the application.
  //
  main();
  return 0;
}

/*************************** End of file ****************************/
