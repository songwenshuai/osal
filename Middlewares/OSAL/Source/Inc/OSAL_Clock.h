/******************************************************************************
  Filename:       OSAL_Clock.h
  Revised:        $Date: 2012-02-02 12:55:32 -0800 (Thu, 02 Feb 2012) $
  Revision:       $Revision: 29143 $

  Description:    OSAL Clock definition and manipulation functions.
******************************************************************************/

#ifndef OSAL_CLOCK_H
#define OSAL_CLOCK_H

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

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

  /*
   * Updates the OSAL clock and Timers from the MAC 320us timer tick.
   */
  extern void osalTimeUpdate( void );

  /*
   * Initialize Clock module.
   */
  void osalClockInit( void );

  /*
   * Update/Adjust the osal clock and timers
   * Msec - elapsed time in milli seconds 
   */
  extern void osalAdjustTimer( uint32 Msec );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CLOCK_H */
