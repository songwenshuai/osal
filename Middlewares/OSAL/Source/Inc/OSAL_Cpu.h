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

#ifdef __IAR_SYSTEMS_ICC__
  #include <intrinsics.h>
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
 * TYPEDEFS
 */

typedef signed char         int8;
typedef signed short int    int16;
typedef signed long int     int24;
typedef signed long int     int32;
typedef signed long long    int64;

typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long int   uint24;
typedef unsigned long int   uint32;
typedef unsigned long long  uint64;

typedef unsigned int        _size_t;

typedef long long           _intmax_t;
typedef unsigned long long  _uintmax_t;

typedef unsigned int        _uintptr_t;
typedef int                 _ptrdiff_t;

typedef unsigned char       _byte;
typedef unsigned short      _word;
typedef unsigned long       _dword;

typedef int                 bool;

typedef bool                halIntState_t;

typedef uint32              halDataAlign_t;

// Generic Status return
typedef unsigned char       Status_t;
 
/* ------------------------------------------------------------------------------------------------
 *                            uint8_val, uint8_bits, byte_val, byte_bits
 * ------------------------------------------------------------------------------------------------
 */
typedef union
{
    uint8 val;
    uint8 uval;
    int8 ival;
    struct
    {
        uint8     b0 : 1;
        uint8     b1 : 1;
        uint8     b2 : 1;
        uint8     b3 : 1;
        uint8     b4 : 1;
        uint8     b5 : 1;
        uint8     b6 : 1;
        uint8     b7 : 1;
    } bits;
} uint8_val, uint8_bits, byte_val, byte_bits;

/* ------------------------------------------------------------------------------------------------
 *                            uint16_val, uint16_bits, word_val, word_bits
 * ------------------------------------------------------------------------------------------------
 */
typedef union
{
    uint16 val;
    uint16 uval;
    int16 ival;
    uint8  v[2];
    struct
    {
        uint8 lb;
        uint8 hb;
    } byte;
    struct
    {
        uint8_val low;
        uint8_val high;
    } byteunion;
    struct
    {
        uint16     b0 : 1;
        uint16     b1 : 1;
        uint16     b2 : 1;
        uint16     b3 : 1;
        uint16     b4 : 1;
        uint16     b5 : 1;
        uint16     b6 : 1;
        uint16     b7 : 1;
        uint16     b8 : 1;
        uint16     b9 : 1;
        uint16     b10 : 1;
        uint16     b11 : 1;
        uint16     b12 : 1;
        uint16     b13 : 1;
        uint16     b14 : 1;
        uint16     b15 : 1;
    } bits;
} uint16_val, uint16_bits, word_val, word_bits;

/* ------------------------------------------------------------------------------------------------
 *                            uint32_val, uint32_bits, dword_val, dword_bits
 * ------------------------------------------------------------------------------------------------
 */
typedef union
{
    uint32 val;
    uint32 uval;
    int32 ival;
    uint16 w[2];
    uint8  v[4];
    struct
    {
        uint16 lw;
        uint16 hw;
    } word;
    struct
    {
        uint8 lb;
        uint8 hb;
        uint8 ub;
        uint8 mb;
    } byte;
    struct
    {
        uint16_val low;
        uint16_val high;
    } wordunion;
    struct
    {
        uint32     b0 : 1;
        uint32     b1 : 1;
        uint32     b2 : 1;
        uint32     b3 : 1;
        uint32     b4 : 1;
        uint32     b5 : 1;
        uint32     b6 : 1;
        uint32     b7 : 1;
        uint32     b8 : 1;
        uint32     b9 : 1;
        uint32     b10 : 1;
        uint32     b11 : 1;
        uint32     b12 : 1;
        uint32     b13 : 1;
        uint32     b14 : 1;
        uint32     b15 : 1;
        uint32     b16 : 1;
        uint32     b17 : 1;
        uint32     b18 : 1;
        uint32     b19 : 1;
        uint32     b20 : 1;
        uint32     b21 : 1;
        uint32     b22 : 1;
        uint32     b23 : 1;
        uint32     b24 : 1;
        uint32     b25 : 1;
        uint32     b26 : 1;
        uint32     b27 : 1;
        uint32     b28 : 1;
        uint32     b29 : 1;
        uint32     b30 : 1;
        uint32     b31 : 1;
    } bits;
} uint32_val, uint32_bits, dword_val, dword_bits;

/* ------------------------------------------------------------------------------------------------
 *                            uint64_val, uint64_bits
 * ------------------------------------------------------------------------------------------------
 */
typedef union
{
    uint64 val;
    uint64 uval;
    int64 ival;
    uint32 d[2];
    uint16 w[4];
    uint8 v[8];
    struct
    {
        uint32 ld;
        uint32 hd;
    } dword;
    struct
    {
        uint16 lw;
        uint16 hw;
        uint16 uw;
        uint16 mw;
    } word;
    struct
    {
        uint32_val low;
        uint32_val high;
    } dwordunion;
    struct
    {
         uint8 b0:1;
         uint8 b1:1;
         uint8 b2:1;
         uint8 b3:1;
         uint8 b4:1;
         uint8 b5:1;
         uint8 b6:1;
         uint8 b7:1;
         uint8 b8:1;
         uint8 b9:1;
         uint8 b10:1;
         uint8 b11:1;
         uint8 b12:1;
         uint8 b13:1;
         uint8 b14:1;
         uint8 b15:1;
         uint8 b16:1;
         uint8 b17:1;
         uint8 b18:1;
         uint8 b19:1;
         uint8 b20:1;
         uint8 b21:1;
         uint8 b22:1;
         uint8 b23:1;
         uint8 b24:1;
         uint8 b25:1;
         uint8 b26:1;
         uint8 b27:1;
         uint8 b28:1;
         uint8 b29:1;
         uint8 b30:1;
         uint8 b31:1;
         uint8 b32:1;
         uint8 b33:1;
         uint8 b34:1;
         uint8 b35:1;
         uint8 b36:1;
         uint8 b37:1;
         uint8 b38:1;
         uint8 b39:1;
         uint8 b40:1;
         uint8 b41:1;
         uint8 b42:1;
         uint8 b43:1;
         uint8 b44:1;
         uint8 b45:1;
         uint8 b46:1;
         uint8 b47:1;
         uint8 b48:1;
         uint8 b49:1;
         uint8 b50:1;
         uint8 b51:1;
         uint8 b52:1;
         uint8 b53:1;
         uint8 b54:1;
         uint8 b55:1;
         uint8 b56:1;
         uint8 b57:1;
         uint8 b58:1;
         uint8 b59:1;
         uint8 b60:1;
         uint8 b61:1;
         uint8 b62:1;
         uint8 b63:1;
    } bits;
} uint64_val, uint64_bits;

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
