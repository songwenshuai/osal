/**************************************************************************************************
  Filename:       OSAL_Comdef.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef OSAL_COMDEF_H
#define OSAL_COMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#include "OSAL_Config.h"

/*********************************************************************
 * TYPEDEFS
 */

typedef   signed char       int8;
typedef unsigned char       uint8;

typedef   signed short int  int16;
typedef unsigned short int  uint16;

typedef   signed long int   int24;
typedef unsigned long int   uint24;

typedef   signed long int   int32;
typedef unsigned long int   uint32;

typedef   signed long long  int64;
typedef unsigned long long  uint64;

typedef             uint32  _size_t;
typedef              int32  _bool;
typedef               _bool  halIntState_t;
typedef             uint32  halDataAlign_t;
typedef              uint8  Status_t;

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
 * COMPILER
 */

/* ----------- IAR Compiler ----------- */
#ifdef __IAR_SYSTEMS_ICC__
#define ASM_NOP    asm("NOP")

/* ----------- KEIL Compiler ---------- */
#elif defined __KEIL__
#define ASM_NOP   __nop()

/* ----------- CCS Compiler ----------- */
#elif __TI_COMPILER_VERSION
#define ASM_NOP    asm(" NOP")

/* ----------- GNU Compiler ----------- */
#elif defined __GNUC__
#define ASM_NOP __asm__ __volatile__ ("nop")

/* ---------- MSVC compiler ---------- */
#elif _MSC_VER
#define ASM_NOP __asm NOP

/* ----------- Unrecognized Compiler ----------- */
#else
#error "ERROR: Unknown compiler."
#endif

/*********************************************************************
 * Lint Keywords
 */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef VOID
#define VOID (void)
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

/*********************************************************************
 * CONSTANTS
 */
#ifndef CONST
  #define CONST const
#endif

#ifndef GENERIC
  #define GENERIC
#endif

 /*** Generic Status Return Values ***/
#define OSAL_SUCCESS              0x00
#define OSAL_FAILURE              0x01
#define INVALIDPARAMETER          0x02
#define INVALID_TASK              0x03
#define MSG_BUFFER_NOT_AVAIL      0x04
#define INVALID_MSG_POINTER       0x05
#define INVALID_EVENT_ID          0x06
#define INVALID_INTERRUPT_ID      0x07
#define NO_TIMER_AVAIL            0x08
#define NV_ITEM_UNINIT            0x09
#define NV_OPER_FAILED            0x0A
#define NV_BAD_ITEM_LEN           0x0B

/*********************************************************************
 * TYPEDEFS
 */

typedef                        char *_va_list;

#define _ADDRESSOF(v)          (&(v))
#define _INTSIZEOF(n)          ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define _va_start(ap, v)       ((void)(ap = (_va_list)_ADDRESSOF(v) + _INTSIZEOF(v)))
#define _va_arg(ap, t)         (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define _va_end(ap)            ((void)(ap = (_va_list)0))

/*********************************************************************
 * Global System Events
 */
#define SYS_EVENT_MSG               0x8000  // A message is waiting event

/*********************************************************************
 * Global Generic System Messages
 */

// OSAL System Message IDs/Events Reserved for applications (user applications)
// 0xE0 ?0xFC

/*********************************************************************
 * MACROS
 */

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef BF
#define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#ifndef MIN
#define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
#define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif

#if !defined(BSET)
//#define BSET(a,b) (a |= 1 << b)
#define BSET(VAR,Place)         ( (VAR) |= (uint8_t)((uint8_t)1<<(uint8_t)(Place)) )
#else
#define BSET(VAR,Place)
#endif

#if !defined(BCLR)
//#define BCLR(a,b) (a &= ~(1 << b))
#define BCLR(VAR,Place)         ( (VAR) &= (uint8_t)((uint8_t)((uint8_t)1<<(uint8_t)(Place))^(uint8_t)255) )
#else
#define BCLR(VAR,Place)
#endif

#if !defined(BGET)
//#define BGET(a,b) (a & (1 << b))
#define BGET(VAR,Place)         ((uint8_t)(VAR) & (uint8_t)((uint8_t)1<<(uint8_t)(Place)))
#else
#define BGET(VAR,Place)
#endif

#if !defined(BINV)
#define BINV(VAR,Place)         ( (VAR) ^= (uint8_t)((uint8_t)1<<(uint8_t)(Place)) )
//#define BINV(VAR,Place) ( (VAR) ^= (1<<(uint8_t)(Place)) )
//#define BINV(VAR,Place) if(VAR & (1 << Place)) VAR &= ~(1 << Place); else VAR |= 1 << Place
#else
#define BINV(VAR,Place)
#endif

/* takes a byte out of a uint32 : var - uint32,  ByteNum - byte to take out (0 - 3) */
#define BREAK_UINT32( var, ByteNum ) (uint8)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
            ((uint32)((uint32)((Byte0) & 0x00FF) \
            +  ((uint32)((Byte1) & 0x00FF) << 8) \
            + ((uint32)((Byte2) & 0x00FF) << 16) \
            + ((uint32)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte)  ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a)                  (((a) >> 8) & 0xFF)
#define LO_UINT16(a)                  ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte)   ((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a)                   (((a) >> 4) & 0x0F)
#define LO_UINT8(a)                   ((a) & 0x0F)

// Write the 32bit value of 'val' in little endian format to the buffer pointed 
// to by pBuf, and increment pBuf by 4
#define UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
    do { \
        *(pBuf)++ = ((((uint32)(val)) >>  0) & 0xFF); \
        *(pBuf)++ = ((((uint32)(val)) >>  8) & 0xFF); \
        *(pBuf)++ = ((((uint32)(val)) >> 16) & 0xFF); \
        *(pBuf)++ = ((((uint32)(val)) >> 24) & 0xFF); \
    } while (0)

// Return the 32bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_UINT32_LITTLE_ENDIAN(pBuf) (((pBuf) += 4), BUILD_UINT32((pBuf)[-4], (pBuf)[-3], (pBuf)[-2], (pBuf)[-1]))

#ifndef OSAL_CHECK_BIT
#define OSAL_CHECK_BIT(DISCS, IDX) ((DISCS) & (1<<(IDX)))
#endif          
#ifndef OSAL_GET_BIT
#define OSAL_GET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] & BV((IDX) % 8)) ? TRUE : FALSE)
#endif
#ifndef OSAL_SET_BIT
#define OSAL_SET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] |= BV((IDX) % 8)))
#endif
#ifndef OSAL_CLR_BIT
#define OSAL_CLR_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] &= (BV((IDX) % 8) ^ 0xFF)))
#endif

/*
 *  This macro is for use by other macros to form a fully valid C statement.
 *  Without this, the if/else conditionals could show unexpected behavior.
 *
 *  For example, use...
 *    #define SET_REGS()  st( ioreg1 = 0; ioreg2 = 0; )
 *  instead of ...
 *    #define SET_REGS()  { ioreg1 = 0; ioreg2 = 0; }
 *  or
 *    #define  SET_REGS()    ioreg1 = 0; ioreg2 = 0;
 *  The last macro would not behave as expected in the if/else construct.
 *  The second to last macro will cause a compiler error in certain uses
 *  of if/else construct
 *
 *  It is not necessary, or recommended, to use this macro where there is
 *  already a valid C statement.  For example, the following is redundant...
 *    #define CALL_FUNC()   st(  func();  )
 *  This should simply be...
 *    #define CALL_FUNC()   func()
 *
 * (The while condition below evaluates false without generating a
 *  constant-controlling-loop type of warning on most compilers.)
 */
#define st(x)      do { x } while (__LINE__ == -1)

/*
 *  This macro compares the size of the first parameter to the integer value
 *  of the second parameter.  If they do not match, a compile time error for
 *  negative array size occurs (even gnu chokes on negative array size).
 *
 *  This compare is done by creating a typedef for an array.  No variables are
 *  created and no memory is consumed with this check.  The created type is
 *  used for checking only and is not for use by any other code.  The value
 *  of 10 in this macro is arbitrary, it just needs to be a value larger
 *  than one to result in a positive number for the array size.
 */
#define HAL_ASSERT_SIZE(x,y) typedef char x ## _assert_size_t[-1+10*(sizeof(x) == (y))]

#if defined(DEBUG) || defined(_DEBUG)
#define HAL_ASSERT(expr)                  st( if (!( expr )) {printf("Assertion failed: %s (%s: %s: %u)\n", #expr, __FILE__, __FUNCTION__, __LINE__); while (1){}} )
#define HAL_ASSERT_FORCED()               st( {printf("Assertion failed: %s: %s: %u\n", __FILE__, __FUNCTION__, __LINE__); while (1){}} )
#define HAL_PANIC(expr)                   st( {printf("panic: %s (%s: %s: %u)\n", #expr, __FILE__, __FUNCTION__, __LINE__); while (1){}} )
#else
#define HAL_ASSERT(expr)
#define HAL_ASSERT_FORCED()
#define HAL_PANIC(expr)
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */
extern void OSAL_Init_Hook(void);
extern void SysTickIntDisable(void);
extern void SysTickIntEnable(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_COMDEF_H */
