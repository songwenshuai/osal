/******************************************************************************
  Filename:     OSAL_Halper.h
  Revised:      $Date: 2014-06-30 16:38:56 -0700 (Mon, 30 Jun 2014) $
  Revision:     $Revision: 39297 $

  Description:  This API allows the software components in the Z-Stack to be
                written independently of the specifics of the operating system,
                kernel, or tasking environment (including control loops or
                connect-to-interrupt systems).
******************************************************************************/

#ifndef OSAL_HALPER_H
#define OSAL_HALPER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "clk.h"

/*********************************************************************
 * MACROS
 */

#define IsLeapYear(yr)  (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * TYPEDEFS
 */

// To be used with
typedef struct
{
  uint8 seconds;  // 0-59
  uint8 minutes;  // 0-59
  uint8 hour;     // 0-23
  uint8 day;      // 0-30
  uint8 month;    // 0-11
  uint16 year;    // 2000+
} UTCTimeStruct;


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * FUNCTIONS
 */

/*** Helper Functions ***/

  /*
   * Convert an interger to an ascii string
   */
  extern uint32 init_rand(uint32 seed);

  /*
   * Convert an interger to an ascii string
   */
  extern uint32 _rand(void);

  /*
   * Convert an interger to an ascii string
   */
  extern int32 rand_range(int32 min, int32 max);

  /*
   * Convert an interger to an ascii string
   */
  extern char* osal_strcat(char* dst, const char* src);

  /*
   * Convert an interger to an ascii string
   */
  extern char* osal_strcpy(char* dst, const char* src);

  /*
   * Convert an interger to an ascii string
   */
  extern _size_t osal_strnlen(const char* str, _size_t maxlen);

  /*
   * Convert an interger to an ascii string
   */
  extern _size_t osal_strncpy_m(char *destStr, _size_t destSize, int nStrings, ...);

  /*
   * Convert an interger to an ascii string
   */
  extern char* osal_strncpy(char* dst, const char* src, _size_t maxlen);

  /*
   * String Length
   */
  extern int osal_strlen( const char* pString );

  /*
   * Memory copy
   */
  extern void* osal_memcpy(void* dst, const void GENERIC* src, unsigned int len);

  /*
   * Memory Duplicate - allocates and copies
   */
  extern void *osal_memdup( const void GENERIC *src, unsigned int len );

  /*
   * Reverse Memory copy
   */
  extern void* osal_revmemcpy(void* dst, const void GENERIC* src, unsigned int len);

  /*
   * Memory compare
   */
  extern uint8 osal_memcmp( const void GENERIC *src1, const void GENERIC *src2, unsigned int len );

  /*
   * Memory set
   */
  extern void *osal_memset( void *dest, uint8 value, int len );

  /*
   * Build a uint16 out of 2 bytes (0 then 1).
   */
  extern uint16 osal_build_uint16( uint8 *swapped );

  /*
   * Build a uint32 out of sequential bytes.
   */
  extern uint32 osal_build_uint32( uint8 *swapped, uint8 len );

  /*
   * Convert long to ascii string
   */
  extern uint8 *osal_ltoa( uint32 l, uint8 * buf, uint8 radix );

  /*
   * convert a string int to a long unsigned.
   */
  extern long osal_atol(const char *s);

  /*
   * Random number generator
   */
  extern uint32 osal_rand(void);

  /*
   * Buffer an uint32 value - LSB first.
   */
  extern uint8* osal_buffer_uint32( uint8 *buf, uint32 val );

  /*
   * Buffer an uint24 value - LSB first
   */
  extern uint8* osal_buffer_uint24( uint8 *buf, uint24 val );

  /*
   * Is all of the array elements set to a value?
   */
  extern uint8 osal_isbufset( uint8 *buf, uint8 val, uint8 len );

  /*
   * Convert an interger to an ascii string
   */
  extern void osal_itoa( uint16 num, uint8 *buf, uint8 radix );

  /*
   * Converts CLK_TS_SEC to UTCTimeStruct
   *
   * secTime - number of seconds since 0 hrs, 0 minutes,
   *          0 seconds, on the 1st of January 2000 UTC
   * tm - pointer to breakdown struct
   */
  extern void osal_ConvertUTCTime( UTCTimeStruct *tm, CLK_TS_SEC secTime );

  /*
   * Converts UTCTimeStruct to CLK_TS_SEC (seconds since 00:00:00 01/01/2000)
   *
   * tm - pointer to UTC time struct
   */
  extern CLK_TS_SEC osal_ConvertUTCSecs( UTCTimeStruct *tm );

  /*
   * Set the new time.  This will only set the seconds portion
   * of time and doesn't change the factional second counter.
   *     newTime - number of seconds since 0 hrs, 0 minutes,
   *               0 seconds, on the 1st of January 2000 UTC
   */
  extern void osal_setClock( CLK_TS_SEC newTime );

  /*
   * Gets the current time.  This will only return the seconds
   * portion of time and doesn't include the factional second counter.
   *     returns: number of seconds since 0 hrs, 0 minutes,
   *              0 seconds, on the 1st of January 2000 UTC
   */
  extern CLK_TS_SEC osal_getClock( void );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */
