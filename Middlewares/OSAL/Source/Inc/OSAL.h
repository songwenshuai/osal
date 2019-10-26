/******************************************************************************
  Filename:     OSAL.h
  Revised:      $Date: 2014-06-30 16:38:56 -0700 (Mon, 30 Jun 2014) $
  Revision:     $Revision: 39297 $

  Description:  This API allows the software components in the Z-Stack to be
                written independently of the specifics of the operating system,
                kernel, or tasking environment (including control loops or
                connect-to-interrupt systems).
******************************************************************************/

#ifndef OSAL_H
#define OSAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "OSAL_Comdef.h"

/*********************************************************************
 * MACROS
 */
#if ( UINT_MAX == 65535 ) /* 8-bit and 16-bit devices */
  #define osal_offsetof(type, member) ((uint16) &(((type *) 0)->member))
#else /* 32-bit devices */
  #define osal_offsetof(type, member) ((uint32) &(((type *) 0)->member))
#endif

/*********************************************************************
 * CONSTANTS
 */

/*** Interrupts ***/
#define INTS_ALL    0xFF

/*********************************************************************
 * TYPEDEFS
 */

typedef struct
{
  uint8  event;
  uint8  status;
} osal_event_hdr_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * FUNCTIONS
 */

/*** Task Synchronization  ***/

  /*
   * Set a Task Event
   */
  extern uint8 osal_set_event( uint8 task_id, uint16 event_flag );


  /*
   * Clear a Task Event
   */
  extern uint8 osal_clear_event( uint8 task_id, uint16 event_flag );


  /*** Interrupt Management  ***/

  /*
   * Register Interrupt Service Routine (ISR)
   */
  extern uint8 osal_isr_register(uint8 interrupt_id, void (*isr_ptr)(uint8*));

  /*
   * Enable Interrupt
   */
  extern uint8 osal_int_enable( uint8 interrupt_id );

  /*
   * Disable Interrupt
   */
  extern uint8 osal_int_disable( uint8 interrupt_id );


/*** Task Management  ***/


  /*
   * Initialize the Task System
   */
  extern uint8 osal_init_system( void );

  /*
   * System Processing Loop
   */
  extern void osal_start_system( void );

  /*
   * One Pass Throu the OSAL Processing Loop
   */
  extern void osal_run_system( void );

  /*
   * Get the active task ID
   */
  extern uint8 osal_self( void );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */
