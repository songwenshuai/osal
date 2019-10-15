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

#include "OSAL_Type.h"
#include "OSAL_Cpu.h"

/*********************************************************************
 * OPTION
 */

#define USE_SYSTICK_IRQ                1

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
#define SUCCESS                   0x00
#define FAILURE                   0x01
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

#ifndef CHECK_BIT
#define CHECK_BIT(DISCS, IDX) ((DISCS) & (1<<(IDX)))
#endif          
#ifndef GET_BIT
#define GET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] & BV((IDX) % 8)) ? TRUE : FALSE)
#endif
#ifndef SET_BIT
#define SET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] |= BV((IDX) % 8)))
#endif
#ifndef CLR_BIT
#define CLR_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] &= (BV((IDX) % 8) ^ 0xFF)))
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
extern void SysTickIntDisable(void);
extern void SysTickIntEnable(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_COMDEF_H */
