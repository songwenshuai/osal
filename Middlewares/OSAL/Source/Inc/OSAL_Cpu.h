/**************************************************************************************************
  Filename:       OSAL_Comdef.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef OSAL_CPU_H
#define OSAL_CPU_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#ifndef _WIN32
#ifdef __IAR_SYSTEMS_ICC__
  #include <intrinsics.h>
#endif
#else
  #include <BSP.h>
#endif
/*********************************************************************
 * OPTION
 */

/*********************************************************************
 * COMPILER
 */

/*********************************************************************
 * CONSTANTS
 */

//
// Target is not allowed to perform other OSAL operations while string still has not been stored completely.
// Otherwise we would probably end up with a mixed string in the buffer.
// If using  OSAL from within interrupts, multiple tasks or multi processors, define the OSAL_IRQ_LOCK(LockState) and OSAL_IRQ_UNLOCK(LockState) function here.
//
// OSAL_IRQ_MAX_INTERRUPT_PRIORITY can be used in the sample lock routines on Cortex-M3/4.
// Make sure to mask all interrupts which can send OSAL data, i.e. generate SystemView events, or cause task switches.
// When high-priority interrupts must not be masked while sending OSAL data, OSAL_IRQ_MAX_INTERRUPT_PRIORITY needs to be adjusted accordingly.
// (Higher priority = lower priority number)
// Default value for embOS: 128u
// Default configuration in FreeRTOS: configMAX_SYSCALL_INTERRUPT_PRIORITY: ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
// In case of doubt mask all interrupts: 1 << (8 - BASEPRI_PRIO_BITS) i.e. 1 << 5 when 3 bits are implemented in NVIC
// or define OSAL_IRQ_LOCK(LockState) to completely disable interrupts.
//

#define OSAL_IRQ_MAX_INTERRUPT_PRIORITY         (0x20)   // Interrupt priority to lock on OSAL_IRQ_LOCK on Cortex-M3/4 (Default: 0x20)

/*********************************************************************
 * MACROS
 */

/*********************************************************************
*
*       OSAL lock configuration for SEGGER Embedded Studio,
*       Rowley CrossStudio and GCC
*/
#if (defined __SES_ARM) || (defined __CROSSWORKS_ARM) || (defined __GNUC__) || (defined __clang__)
  #if (defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_8M_BASE__))
    #define OSAL_IRQ_LOCK(LockState)     __asm volatile ("mrs   %0, primask  \n\t"                         \
                                                         "movs  r1, $1       \n\t"                         \
                                                         "msr   primask, r1  \n\t"                         \
                                                         : "=r" (LockState)                                \
                                                         :                                                 \
                                                         : "r1"                                            \
                                                         );

    #define OSAL_IRQ_UNLOCK(LockState)   __asm volatile ("msr   primask, %0  \n\t"                         \
                                                         :                                                 \
                                                         : "r" (LockState)                                 \
                                                         :                                                 \
                                                         );

  #elif (defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__))
    #ifndef   OSAL_IRQ_MAX_INTERRUPT_PRIORITY
      #define OSAL_IRQ_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define OSAL_IRQ_LOCK(LockState)     __asm volatile ("mrs   %0, basepri  \n\t"                         \
                                                         "mov   r1, %1       \n\t"                         \
                                                         "msr   basepri, r1  \n\t"                         \
                                                         : "=r" (LockState)                                \
                                                         : "i"(OSAL_IRQ_MAX_INTERRUPT_PRIORITY)            \
                                                         : "r1"                                            \
                                                         );

    #define OSAL_IRQ_UNLOCK(LockState)   __asm volatile ("msr   basepri, %0  \n\t"                         \
                                                         :                                                 \
                                                         : "r" (LockState)                                 \
                                                         :                                                 \
                                                         );


  #elif defined(__ARM_ARCH_7A__)
    #define OSAL_IRQ_LOCK(LockState)   __asm volatile ("mrs r1, CPSR \n\t"           \
                                                       "mov %0, r1 \n\t"             \
                                                       "orr r1, r1, #0xC0 \n\t"      \
                                                       "msr CPSR_c, r1 \n\t"         \
                                                       : "=r" (LockState)            \
                                                       :                             \
                                                       : "r1"                        \
                                                       );

    #define OSAL_IRQ_UNLOCK(LockState) __asm volatile ("mov r0, %0 \n\t"              \
                                                       "mrs r1, CPSR \n\t"            \
                                                       "bic r1, r1, #0xC0 \n\t"       \
                                                       "and r0, r0, #0xC0 \n\t"       \
                                                       "orr r1, r1, r0 \n\t"          \
                                                       "msr CPSR_c, r1 \n\t"          \
                                                       :                              \
                                                       : "r" (LockState)              \
                                                       : "r0", "r1"                   \
                                                       );

#else
    #define OSAL_IRQ_LOCK(LockState)
    #define OSAL_IRQ_UNLOCK(LockState)
  #endif
#endif

/*********************************************************************
*
*       OSAL lock configuration for IAR EWARM
*/
#ifdef __ICCARM__
  #if (defined (__ARM6M__) && (__CORE__ == __ARM6M__))
    #define OSAL_IRQ_LOCK(LockState)     LockState = __get_PRIMASK();                                      \
                                         __set_PRIMASK(1);

    #define OSAL_IRQ_UNLOCK(LockState)   __set_PRIMASK(LockState);

  #elif ((defined (__ARM7EM__) && (__CORE__ == __ARM7EM__)) || (defined (__ARM7M__) && (__CORE__ == __ARM7M__)))
    #ifndef   OSAL_IRQ_MAX_INTERRUPT_PRIORITY
      #define OSAL_IRQ_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define OSAL_IRQ_LOCK(LockState)     LockState = __get_BASEPRI();                                      \
                                         __set_BASEPRI(OSAL_IRQ_MAX_INTERRUPT_PRIORITY);

    #define OSAL_IRQ_UNLOCK(LockState)   __set_BASEPRI(LockState);

  #endif
#endif

/*********************************************************************
*
*       OSAL lock configuration for IAR RX
*/
#ifdef __ICCRX__
  #define OSAL_IRQ_LOCK(LockState)     LockState = __get_interrupt_state();                                \
                                       __disable_interrupt();

  #define OSAL_IRQ_UNLOCK(LockState)   __set_interrupt_state(LockState);

#endif

/*********************************************************************
*
*       OSAL lock configuration for IAR RL78
*/
#ifdef __ICCRL78__
  #define OSAL_IRQ_LOCK(LockState)     LockState = __get_interrupt_state();                                \
                                       __disable_interrupt();

  #define OSAL_IRQ_UNLOCK(LockState)   __set_interrupt_state(LockState);

#endif

/*********************************************************************
*
*       OSAL lock configuration for KEIL ARM
*/
#ifdef __CC_ARM
  #if (defined __TARGET_ARCH_6S_M)
    #define OSAL_IRQ_LOCK(LockState)     register unsigned char PRIMASK __asm( "primask");                 \
                                         LockState = PRIMASK;                                              \
                                         PRIMASK = 1u;                                                     \
                                         __schedule_barrier();

    #define OSAL_IRQ_UNLOCK(LockState)   PRIMASK = LockState;                                              \
                                         __schedule_barrier();

  #elif (defined(__TARGET_ARCH_7_M) || defined(__TARGET_ARCH_7E_M))
    #ifndef   OSAL_IRQ_MAX_INTERRUPT_PRIORITY
      #define OSAL_IRQ_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define OSAL_IRQ_LOCK(LockState)     register unsigned char BASEPRI __asm( "basepri");                 \
                                         LockState = BASEPRI;                                              \
                                         BASEPRI = OSAL_IRQ_MAX_INTERRUPT_PRIORITY;                        \
                                         __schedule_barrier();

    #define OSAL_IRQ_UNLOCK(LockState)   BASEPRI = LockState;                                              \
                                         __schedule_barrier();

  #endif
#endif

/*********************************************************************
*
*       OSAL lock configuration for TI ARM
*/
#ifdef __TI_ARM__
  #if defined (__TI_ARM_V6M0__)
    #define OSAL_IRQ_LOCK(LockState)     LockState = __get_PRIMASK();                                        \
                                         __set_PRIMASK(1);

    #define OSAL_IRQ_UNLOCK(LockState)   __set_PRIMASK(LockState);

  #elif (defined (__TI_ARM_V7M3__) || defined (__TI_ARM_V7M4__))
    #ifndef   OSAL_IRQ_MAX_INTERRUPT_PRIORITY
      #define OSAL_IRQ_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define OSAL_IRQ_LOCK(LockState)     LockState = _set_interrupt_priority(OSAL_IRQ_MAX_INTERRUPT_PRIORITY);

    #define OSAL_IRQ_UNLOCK(LockState)   _set_interrupt_priority(LockState);

  #endif
#endif

/*********************************************************************
*
*       OSAL lock configuration for CCRX
*/
#ifdef __RX
  #define OSAL_IRQ_LOCK(LockState)     LockState = get_psw() & 0x010000;                                   \
                                       clrpsw_i();                           

  #define OSAL_IRQ_UNLOCK(LockState)   set_psw(get_psw() | LockState);

#endif

/*********************************************************************
*
*       OSAL lock configuration fallback
*/
#ifdef _WIN32
  #define OSAL_IRQ_LOCK(LockState)                (void)LockState   // Lock OSAL (nestable)   (i.e. disable interrupts)

  #define OSAL_IRQ_UNLOCK(LockState)              (void)LockState   // Unlock OSAL (nestable) (i.e. enable previous interrupt lock state)
#endif

/*********************************************************************
*
*       OSAL lock configuration fallback
*/
#ifndef   OSAL_IRQ_LOCK
  #define OSAL_IRQ_LOCK(LockState)                // Lock OSAL (nestable)   (i.e. disable interrupts)
#endif

#ifndef   OSAL_IRQ_UNLOCK
  #define OSAL_IRQ_UNLOCK(LockState)              // Unlock OSAL (nestable) (i.e. enable previous interrupt lock state)
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/* Enable interrupts */
#define HAL_ENABLE_INTERRUPTS()        OSAL_IRQ_LOCK(osal_int_state);
/* Disable interrupts */   
#define HAL_DISABLE_INTERRUPTS()       OSAL_IRQ_UNLOCK(osal_int_state);

/* enter irq */
#define HAL_ENTER_CRITICAL_SECTION(x)  OSAL_IRQ_LOCK(x)

/* exit irq */
#define HAL_EXIT_CRITICAL_SECTION(x)   OSAL_IRQ_UNLOCK(x)

/* Hal Critical statement definition */
#define HAL_CRITICAL_STATEMENT(x)       st( halIntState_t s; HAL_ENTER_CRITICAL_SECTION(s); x; HAL_EXIT_CRITICAL_SECTION(s); )

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_CPU_H */
