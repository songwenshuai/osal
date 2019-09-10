/**************************************************************************************************
  Filename:       OSAL_Pwrmgr.c
  Revised:        $Date: 2014-11-21 16:17:37 -0800 (Fri, 21 Nov 2014) $
  Revision:       $Revision: 41218 $

  Description:    This file contains the OSAL Power Management API.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Tasks.h"
#include "OSAL_Timers.h"
#include "OSAL_Clock.h"
#include "OSAL_PwrMgr.h"

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

/* This global variable stores the power management attributes.
 */
pwrmgr_attribute_t pwrmgr_attribute;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern void Delay(uint32 delay);

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_pwrmgr_init
 *
 * @brief   Initialize the power management system.
 *
 * @param   none.
 *
 * @return  none.
 */
void osal_pwrmgr_init( void )
{
  pwrmgr_attribute.pwrmgr_device = PWRMGR_ALWAYS_ON; // No power conservation for routing devices.
  pwrmgr_attribute.pwrmgr_task_state = 0;            // Cleared.  All set to conserve
}

/*********************************************************************
 * @fn      osal_pwrmgr_device
 *
 * @brief   Sets the device power characteristic.
 *
 * @param   pwrmgr_device - type of power devices. With PWRMGR_ALWAYS_ON
 *          selection, there is no power savings and the device is most
 *          likely on mains power. The PWRMGR_BATTERY selection allows the
 *          HAL sleep manager to enter sleep.
 *
 * @return  none
 */
void osal_pwrmgr_device( uint8 pwrmgr_device )
{
  pwrmgr_attribute.pwrmgr_device = pwrmgr_device;
}

/*********************************************************************
 * @fn      osal_pwrmgr_task_state
 *
 * @brief   This function is called by each task to state whether or
 *          not this task wants to conserve power.
 *
 * @param   task_id - calling task ID.
 *          state - whether the calling task wants to
 *          conserve power or not.
 *
 * @return  SUCCESS if task complete
 */
uint8 osal_pwrmgr_task_state( uint8 task_id, uint8 state )
{
  halIntState_t intState;

  if ( task_id >= tasksCnt )
    return ( INVALID_TASK );

  HAL_ENTER_CRITICAL_SECTION( intState );

  if ( state == PWRMGR_CONSERVE )
  {
    // Clear the task state flag
    pwrmgr_attribute.pwrmgr_task_state &= ~(1 << task_id );

  }
  else
  {
    // Set the task state flag
    pwrmgr_attribute.pwrmgr_task_state |= (1 << task_id);
  }

  HAL_EXIT_CRITICAL_SECTION( intState );

  return ( SUCCESS );
}

#if defined( POWER_SAVING )
/*********************************************************************
 * @fn      osal_pwrmgr_powerconserve
 *
 * @brief   This function is called from the main OSAL loop when there are
 *          no events scheduled and shouldn't be called from anywhere else.
 *
 * @param   none.
 *
 * @return  none.
 */
void osal_pwrmgr_powerconserve( void )
{
  uint32        next;
  halIntState_t intState;

  // Should we even look into power conservation
  if ( pwrmgr_attribute.pwrmgr_device != PWRMGR_ALWAYS_ON )
  {
    // Are all tasks in agreement to conserve
    if ( pwrmgr_attribute.pwrmgr_task_state == 0 )
    {
      // Hold off interrupts.
      HAL_ENTER_CRITICAL_SECTION( intState );

      // Get next time-out
      next = osal_next_timeout();

      // Re-enable interrupts.
      HAL_EXIT_CRITICAL_SECTION( intState );

      // Put the processor into sleep mode
      OSAL_SET_CPU_INTO_SLEEP( next );
    }
  }
}
#endif /* POWER_SAVING */

/*********************************************************************
*********************************************************************/
