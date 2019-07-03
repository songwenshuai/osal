/**************************************************************************************************
  Filename:       OSAL_Comdef.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef COMDEF_H
#define COMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * OPTION
 */

//#define SYS_TICK                1

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

#ifdef _WIN32
#define bool                _Bool
#endif

typedef signed   char       int8;
typedef signed   short int  int16;
typedef signed   long int   int32;
typedef signed   long long  int64;

typedef unsigned char       uint8;
typedef unsigned short int  uint16;
typedef unsigned long int   uint32;
typedef unsigned long long  uint64; 

typedef unsigned long       _dword;
typedef int                 _bool;
typedef unsigned char       _byte;
typedef unsigned short      _word;

// Data types
typedef int32               int24;
typedef uint32              uint24;

typedef uint32              halDataAlign_t;

typedef bool                halIntState_t;

// Generic Status return
typedef uint8               Status_t;
 
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

extern uint32 CPUcpsie(void);
extern uint32 CPUcpsid(void);

/* Enable interrupts */
#define HAL_ENABLE_INTERRUPTS()        CPUcpsie()
/* Disable interrupts */   
#define HAL_DISABLE_INTERRUPTS()       CPUcpsid()

/* enter irq */
#define HAL_ENTER_CRITICAL_SECTION(x)  do { (x) = !HAL_DISABLE_INTERRUPTS(); } while (0)

/* exit irq */
#define HAL_EXIT_CRITICAL_SECTION(x)   do { if (x) { (void) HAL_ENABLE_INTERRUPTS(); } } while (0)

/* Hal Critical statement definition */
#define HAL_CRITICAL_STATEMENT(x)       st( halIntState_t s; HAL_ENTER_CRITICAL_SECTION(s); x; HAL_EXIT_CRITICAL_SECTION(s); )

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* COMDEF_H */
