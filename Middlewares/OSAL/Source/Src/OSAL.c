/**************************************************************************************************
  Filename:       OSAL.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:    This API allows the software components in the Z-stack to be written
                  independently of the specifics of the operating system, kernel or tasking
                  environment (including control loops or connect-to-interrupt systems).
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Helper.h"
#include "OSAL_Message.h"
#include "OSAL_Tasks.h"
#include "OSAL_Clock.h"
#include "OSAL_Timers.h"
#include "OSAL_PwrMgr.h"

#include "lwmem.h"

#include "printf.h"

/*********************************************************************
 * MACROS
 */

#define OSAL_ALLOC                       lwmem_malloc
#define OSAL_FREE                        lwmem_free

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
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

extern void lwmem_init(void);

/*
 * Process Polls
 */
extern void Hal_ProcessPoll (void);

/*********************************************************************
 * LOCAL VARIABLES
 */

// Index of active task
static uint8 activeTaskID = TASK_NO_TASK;
// osal_int_enable state
static halIntState_t osal_int_state;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_set_event
 *
 * @brief
 *
 *    This function is called to set the event flags for a task. The
 *    event passed in is OR'd into the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to set
 *
 * @return  SUCCESS, MSG_BUFFER_NOT_AVAIL, FAILURE, INVALID_TASK
 */
uint8 osal_set_event( uint8 task_id, uint16 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] |= event_flag;  // Stuff the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_clear_event
 *
 * @brief
 *
 *    This function is called to clear the event flags for a task. The
 *    event passed in is masked out of the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to clear
 *
 * @return  SUCCESS, INVALID_TASK
 */
uint8 osal_clear_event( uint8 task_id, uint16 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] &= ~(event_flag);   // Clear the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
   else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_isr_register
 *
 * @brief
 *
 *   This function is called to register a service routine with an
 *   interrupt. When the interrupt occurs, this service routine is called.
 *
 * @param   uint8 interrupt_id - Interrupt number
 * @param   void (*isr_ptr)( uint8* ) - function pointer to ISR
 *
 * @return  SUCCESS, INVALID_INTERRUPT_ID,
 */
uint8 osal_isr_register( uint8 interrupt_id, void (*isr_ptr)( uint8* ) )
{
  // Remove these statements when functionality is complete
  (void)interrupt_id;
  (void)isr_ptr;
  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_int_enable
 *
 * @brief
 *
 *   This function is called to enable an interrupt. Once enabled,
 *   occurrence of the interrupt causes the service routine associated
 *   with that interrupt to be called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are enabled.
 *   If a single interrupt is passed in, then interrupts still have
 *   to be enabled with another call to INTS_ALL.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_enable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_ENABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_int_disable
 *
 * @brief
 *
 *   This function is called to disable an interrupt. When a disabled
 *   interrupt occurs, the service routine associated with that
 *   interrupt is not called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are disabled.
 *   If a single interrupt is passed in, then just that interrupt is disabled.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_disable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_DISABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_init_system
 *
 * @brief
 *
 *   This function initializes the "task" system by creating the
 *   tasks defined in the task table (OSAL_Tasks.h).
 *
 * @param   void
 *
 * @return  SUCCESS
 */
uint8 osal_init_system( void )
{
  // Turn off interrupts
  osal_int_disable(INTS_ALL);

  // Hook Initialize
  OSAL_Init_Hook();

  // Initialize the Memory Allocation System
  lwmem_init();

  // Initialize the message queue
  osal_qHead = NULL;

  // Initialize the timers
  osalTimerInit();

  // Initialize clock module
  osalClockInit();

  // Initialize the Power Management System
  osal_pwrmgr_init();
  
#if defined ( POWER_SAVING )
  osal_pwrmgr_device(PWRMGR_BATTERY);
#endif

  // Initialize the system tasks.
  osalInitTasks();

  // Allow interrupts
  osal_int_enable(INTS_ALL);

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_start_system
 *
 * @brief
 *
 *   This function is the main loop function of the task system (if
 *   ZBIT and UBIT are not defined). This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{
  // Initialize the operating system
  osal_init_system();
  
  for(;;)  // Forever Loop
  {
    osal_run_system();
  }
}

/*********************************************************************
 * @fn      osal_run_system
 *
 * @brief
 *
 *   This function will make one pass through the OSAL taskEvents table
 *   and call the task_event_processor() function for the first task that
 *   is found with at least one event pending. If there are no pending
 *   events (all tasks), this function puts the processor into Sleep.
 *
 * @param   void
 *
 * @return  none
 */
void osal_run_system( void )
{
  uint8 idx = 0;

#ifndef USE_SYSTICK_IRQ
  osalTimeUpdate();
#endif

  Hal_ProcessPoll();
  
  do {
    if (tasksEvents[idx])  // Task is highest priority that is ready.
    {
      break;
    }
  } while (++idx < tasksCnt);

  if (idx < tasksCnt)
  {
    uint16 events;
    halIntState_t intState;

    HAL_ENTER_CRITICAL_SECTION(intState);
    events = tasksEvents[idx];
    tasksEvents[idx] = 0;  // Clear the Events for this task.
    HAL_EXIT_CRITICAL_SECTION(intState);

    activeTaskID = idx;
    events = (tasksArr[idx])( idx, events );
    activeTaskID = TASK_NO_TASK;

    HAL_ENTER_CRITICAL_SECTION(intState);
    tasksEvents[idx] |= events;  // Add back unprocessed events to the current task.
    HAL_EXIT_CRITICAL_SECTION(intState);
  }
#if defined( POWER_SAVING )
  else  // Complete pass through all task events with no activity?
  {
    osal_pwrmgr_powerconserve();  // Put the processor/system into sleep
  }
#endif

}

/*********************************************************************
 * @fn      osal_self
 *
 * @brief
 *
 *   This function returns the task ID of the current (active) task.
 *
 * @param   void
 *
 * @return   active task ID or TASK_NO_TASK if no task is active
 */
uint8 osal_self( void )
{
  return ( activeTaskID );
}

/*********************************************************************
 */
