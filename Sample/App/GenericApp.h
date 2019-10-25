/**************************************************************************************************
  Filename:       GenericApp.h
  Revised:        $Date: 2014-06-19 08:38:22 -0700 (Thu, 19 Jun 2014) $
  Revision:       $Revision: 39101 $

  Description:    This file contains the ZigBee Cluster Library Home
                  Automation Sample Application.
**************************************************************************************************/

#ifndef _OSAL_APP_H
#define _OSAL_APP_H

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

#define SBP_PERIODIC_EVT_DELAY  1000
#define SBP_CBTIMER_EVT_DELAY   6000

/*********************************************************************
 * CONSTANTS
 */
#define APP_MESSAGE           0xC0  // Key Events

#define SBP_START_DEVICE_EVT  0x0002
#define SBP_PERIODIC_EVT      0x0004  
#define NV_APPID              0x100

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern uint8 App_TaskID;

/*********************************************************************
 * TYPEDEF
 */
typedef struct
{
  osal_event_hdr_t  hdr;
  uint8             strLen;
  uint8             *pString;
} DebugStr_t;

// LED Command info
typedef struct
{
  uint16 connHandle;
  uint8 code;
} ledCmd_t;

/*********************************************************************
 * FUNCTIONS
 */
void         App_Init( uint8 task_id );
uint16       App_ProcessEvent( uint8 task_id, uint16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _OSAL_APP_H */
