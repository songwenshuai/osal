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

// Message Pool Definitions
osal_msg_q_t osal_qHead;


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

static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 urgent );


/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_msg_allocate
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *
 *
 * @param   uint8 len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */
uint8 * osal_msg_allocate( uint16 len )
{
  osal_msg_hdr_t *hdr;

  if ( len == 0 )
    return ( NULL );

  hdr = (osal_msg_hdr_t *) OSAL_ALLOC( (short)(len + sizeof( osal_msg_hdr_t )) );
  if ( hdr )
  {
    hdr->next = NULL;
    hdr->len = len;
    hdr->dest_id = TASK_NO_TASK;
    return ( (uint8 *) (hdr + 1) );
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      osal_msg_deallocate
 *
 * @brief
 *
 *    This function is used to deallocate a message buffer. This function
 *    is called by a task (or processing element) after it has finished
 *    processing a received message.
 *
 *
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_MSG_POINTER
 */
uint8 osal_msg_deallocate( uint8 *msg_ptr )
{
  uint8 *x;

  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // don't deallocate queued buffer
  if ( OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
    return ( MSG_BUFFER_NOT_AVAIL );

  x = (uint8 *)((uint8 *)msg_ptr - sizeof( osal_msg_hdr_t ));

  OSAL_FREE( (void *)x );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_send
 *
 * @brief
 *
 *    This function is called by a task to send a command message to
 *    another task or processing element.  The sending_task field must
 *    refer to a valid task, since the task ID will be used
 *    for the response message.  This function will also set a message
 *    ready event in the destination tasks event list.
 *
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_send( uint8 destination_task, uint8 *msg_ptr )
{
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, FALSE ) );
}

/*********************************************************************
 * @fn      osal_msg_push_front
 *
 * @brief
 *
 *    This function is called by a task to push a command message
 *    to the head of the OSAL queue. The destination_task field
 *    must refer to a valid task, since the task ID will be used to
 *    send the message to. This function will also set a message
 *    ready event in the destination task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_push_front( uint8 destination_task, uint8 *msg_ptr )
{
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, TRUE ) );
}

/*********************************************************************
 * @fn      osal_msg_enqueue_push
 *
 * @brief
 *
 *    This function is called by a task to either enqueue (append to
 *    queue) or push (prepend to queue) a command message to the OSAL
 *    queue. The destination_task field must refer to a valid task,
 *    since the task ID will be used to send the message to. This 
 *    function will also set a message ready event in the destination
 *    task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 * @param   uint8 push - TRUE to push, otherwise enqueue
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 push )
{
  if ( msg_ptr == NULL )
  {
    return ( INVALID_MSG_POINTER );
  }

  if ( destination_task >= tasksCnt )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }

  // Check the message header
  if ( OSAL_MSG_NEXT( msg_ptr ) != NULL ||
       OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_MSG_POINTER );
  }

  OSAL_MSG_ID( msg_ptr ) = destination_task;

  if ( push == TRUE )
  {
    // prepend the message
    osal_msg_push( &osal_qHead, msg_ptr );
  }
  else
  {
    // append the message
    osal_msg_enqueue( &osal_qHead, msg_ptr );
  }

  // Signal the task that a message is waiting
  osal_set_event( destination_task, SYS_EVENT_MSG );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_receive
 *
 * @brief
 *
 *    This function is called by a task to retrieve a received command
 *    message. The calling task must deallocate the message buffer after
 *    processing the message using the osal_msg_deallocate() call.
 *
 * @param   uint8 task_id - receiving tasks ID
 *
 * @return  *uint8 - message information or NULL if no message
 */
uint8 *osal_msg_receive( uint8 task_id )
{
  osal_msg_hdr_t *listHdr;
  osal_msg_hdr_t *prevHdr = NULL;
  osal_msg_hdr_t *foundHdr = NULL;
  halIntState_t   intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Point to the top of the queue
  listHdr = osal_qHead;

  // Look through the queue for a message that belongs to the asking task
  while ( listHdr != NULL )
  {
    if ( (listHdr - 1)->dest_id == task_id )
    {
      if ( foundHdr == NULL )
      {
        // Save the first one
        foundHdr = listHdr;
      }
      else
      {
        // Second msg found, stop looking
        break;
      }
    }
    if ( foundHdr == NULL )
    {
      prevHdr = listHdr;
    }
    listHdr = OSAL_MSG_NEXT( listHdr );
  }

  // Is there more than one?
  if ( listHdr != NULL )
  {
    // Yes, Signal the task that a message is waiting
    osal_set_event( task_id, SYS_EVENT_MSG );
  }
  else
  {
    // No more
    osal_clear_event( task_id, SYS_EVENT_MSG );
  }

  // Did we find a message?
  if ( foundHdr != NULL )
  {
    // Take out of the link list
    osal_msg_extract( &osal_qHead, foundHdr, prevHdr );
  }

  // Release interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ( (uint8*) foundHdr );
}

/**************************************************************************************************
 * @fn          osal_msg_find
 *
 * @brief       This function finds in place an OSAL message matching the task_id and event
 *              parameters.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match.
 *
 * output parameters
 *
 * None.
 *
 * @return      NULL if no match, otherwise an in place pointer to the matching OSAL message.
 **************************************************************************************************
 */
osal_event_hdr_t *osal_msg_find(uint8 task_id, uint8 event)
{
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if (((pHdr-1)->dest_id == task_id) && (((osal_event_hdr_t *)pHdr)->event == event))
    {
      break;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return (osal_event_hdr_t *)pHdr;
}

/**************************************************************************************************
 * @fn          osal_msg_count
 *
 * @brief       This function counts the number of messages, in the OSAL message queue with a
 *              a given task ID and message event type.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match. 0xFF for 
 *              all events.
 *
 * output parameters
 *
 * None.
 *
 * @return      The number of OSAL messages that match the task ID and Event.
 **************************************************************************************************
 */
uint8 osal_msg_count( uint8 task_id, uint8 event )
{
  uint8 count = 0;
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if ( ((pHdr-1)->dest_id == task_id) 
        && ((event == 0xFF) || (((osal_event_hdr_t *)pHdr)->event == event)) )
    {
      count++;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return ( count );
}

/*********************************************************************
 * @fn      osal_msg_enqueue
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_enqueue( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  void *list;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
  }
  else
  {
    // Find end of queue
    for ( list = *q_ptr; OSAL_MSG_NEXT( list ) != NULL; list = OSAL_MSG_NEXT( list ) );

    // Add message to end of queue
    OSAL_MSG_NEXT( list ) = msg_ptr;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_dequeue
 *
 * @brief
 *
 *    This function dequeues an OSAL message from an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 *
 * @return  void * - pointer to OSAL message or NULL of queue is empty.
 */
void *osal_msg_dequeue( osal_msg_q_t *q_ptr )
{
  void *msg_ptr = NULL;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( *q_ptr != NULL )
  {
    // Dequeue message
    msg_ptr = *q_ptr;
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
    OSAL_MSG_NEXT( msg_ptr ) = NULL;
    OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return msg_ptr;
}

/*********************************************************************
 * @fn      osal_msg_push
 *
 * @brief
 *
 *    This function pushes an OSAL message to the head of an OSAL
 *    queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_push( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Push message to head of queue
  OSAL_MSG_NEXT( msg_ptr ) = *q_ptr;
  *q_ptr = msg_ptr;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_extract
 *
 * @brief
 *
 *    This function extracts and removes an OSAL message from the
 *    middle of an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message to be extracted
 * @param   void *prev_ptr  - OSAL message before msg_ptr in queue
 *
 * @return  none
 */
void osal_msg_extract( osal_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( msg_ptr == *q_ptr )
  {
    // remove from first
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
  }
  else
  {
    // remove from middle
    OSAL_MSG_NEXT( prev_ptr ) = OSAL_MSG_NEXT( msg_ptr );
  }
  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_enqueue_max
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue if
 *    the length of the queue is less than max.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 * @param   uint8 max - maximum length of queue
 *
 * @return  TRUE if message was enqueued, FALSE otherwise
 */
uint8 osal_msg_enqueue_max( osal_msg_q_t *q_ptr, void *msg_ptr, uint8 max )
{
  void *list;
  uint8 ret = FALSE;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
    ret = TRUE;
  }
  else
  {
    // Find end of queue or max
    list = *q_ptr;
    max--;
    while ( (OSAL_MSG_NEXT( list ) != NULL) && (max > 0) )
    {
      list = OSAL_MSG_NEXT( list );
      max--;
    }

    // Add message to end of queue if max not reached
    if ( max != 0 )
    {
      OSAL_MSG_NEXT( list ) = msg_ptr;
      ret = TRUE;
    }
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ret;
}

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
