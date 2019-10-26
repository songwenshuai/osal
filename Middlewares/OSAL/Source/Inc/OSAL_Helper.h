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

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */
