#include "OSAL.h"

#include "OSAL_Helper.h"

#include "OSAL_Memory.h"

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
 * EXTERNAL VARIABLES
 */


/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * HELPER FUNCTIONS
 */

static uint32 prngState = 0;

/*********************************************************************
 * @fn      init_rand
 *
 * @brief   Seed pseudo-random number generator
 * 
 * @param   seed An integer value to be used as seed by the pseudo-random number generator
 *
 * @return  Error code
 */
uint32 init_rand(uint32 seed)
{
    //Seed the pseudo-random number generator
    prngState += seed;

    //Successful processing
    return OSAL_SUCCESS;
}

/*********************************************************************
 * @fn      _rand
 *
 * @brief   Get a random value
 * 
 * @param   void
 *
 * @return  Random value
 */
uint32 _rand(void)
{
    uint32 value;

    //Use a linear congruential generator (LCG) to update the state of the PRNG
    prngState *= 1103515245;
    prngState += 12345;
    value = (prngState >> 16) & 0x07FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    prngState *= 1103515245;
    prngState += 12345;
    value <<= 10;
    value |= (prngState >> 16) & 0x03FF;

    //Return the random value
    return value;
}

/*********************************************************************
 * @fn      rand_range
 *
 * @brief   Get a random value in the specified range
 *
 *
 * @param   min Lower bound
 *
 * @param   max Upper bound
 *
 * 
 * @return  Random value in the specified range
 */
int32 rand_range(int32 min, int32 max)
{
    int32 value;

    //Valid parameters?
    if (max > min)
    {
        //Pick up a random value in the given range
        value = min + (_rand() % (max - min + 1));
    }
    else
    {
        //Use default value
        value = min;
    }

    //Return the random value
    return value;
}

/*********************************************************************
 * @fn      osal_strcat
 *
 * @brief
 *
 *
 * @param   char* dst
 *
 * @param   const char* src
 *
 * 
 * @return  char*
 */
char* osal_strcat(char* dst, const char* src)
{
    const _size_t dstlen = osal_strlen(dst);
    const _size_t srclen = osal_strlen(src);
    //  The osal_strcat() and strncat() functions append a copy of the null-
    //  terminated string src to the end of the null-terminated string dst,
    //  then add a terminating '\0'.  The string dst must have sufficient
    //  space to hold the result.
    osal_memcpy(dst + dstlen, src, srclen + 1);
    //  The osal_strcat() and strncat() functions return dst.
    return dst;
}

/*********************************************************************
 * @fn      osal_strcpy
 *
 * @brief
 *
 *
 * @param   char* dst
 *
 * @param   const char* src
 *
 * 
 * @return  char*
 */
char* osal_strcpy(char* dst, const char* src)
{
    const _size_t length = osal_strlen(src);
    //  The stpcpy() and osal_strcpy() functions copy the string src to dst
    //  (including the terminating '\0' character).
    osal_memcpy(dst, src, length + 1);
    //  The osal_strcpy() and strncpy() functions return dst.
    return dst;
}

/*********************************************************************
 * @fn      osal_strnlen
 *
 * @brief
 *
 *
 * @param   char* dst
 *
 * @param   const char* src
 *
 * 
 * @return  char*
 */
_size_t osal_strnlen(const char* str, _size_t maxlen)
{
    const char* cp;

    for(cp = str; maxlen != 0 && *cp != '\0'; cp++, maxlen--)
    {
        {
            ;
        }
    }

    return (_size_t)(cp - str);
}

/*********************************************************************
 * @fn      osal_strncpy
 *
 * @brief
 *
 *
 * @param   char* dst
 *
 * @param   const char* src
 *
 * 
 * @return  char*
 */
char* osal_strncpy(char* dst, const char* src, _size_t maxlen)
{
    const _size_t srclen = osal_strnlen(src, maxlen);
    if(srclen < maxlen)
    {
        //  The stpncpy() and strncpy() functions copy at most maxlen
        //  characters from src into dst.
        osal_memcpy(dst, src, srclen);
        //  If src is less than maxlen characters long, the remainder
        //  of dst is filled with '\0' characters.
        osal_memset(dst + srclen, 0, maxlen - srclen);
    }
    else
    {
        //  Otherwise, dst is not terminated.
        osal_memcpy(dst, src, maxlen);
    }
    //  The strcpy() and strncpy() functions return dst.
    return dst;
}

/*****************************************************************************
  Function:
    char* strncpy_m(char* destStr, _size_t destSize, int nStrings, ...)

  Summary:
    Copies multiple strings to a destination

  Description:
    Copies multiple strings to a destination
    but doesn't copy more than destSize characters.
    Useful where the destination is actually an array and an extra \0
    won't be appended to overflow the buffer
    
  Precondition:
    - valid string pointers
    - destSize should be > 0

  Parameters:
    destStr - Pointer to a string to be initialized with the multiple strings provided as arguments.

    destSize    - the maximum size of the destStr field, that cannot be exceeded.
                  An \0 won't be appended if the resulting size is > destSize

    nStrings    - number of string parameters to be copied into destStr

    ...         - variable number of arguments
    
    
  Returns:
    Length of the destination string, terminating \0 (if exists) not included
  */
_size_t osal_strncpy_m(char *destStr, _size_t destSize, int nStrings, ...)
{
    _va_list args = {0};
    const char *str;
    char *end;
    _size_t len;

    destStr[0] = '\0';
    end = destStr + destSize - 1;
    *end = '\0';
    len = 0;

    _va_start(args, nStrings);

    while (nStrings--)
    {
        if (*end)
        { // if already full don't calculate strlen outside the string area
            len = destSize;
            break;
        }

        str = _va_arg(args, const char *);
        osal_strncpy(destStr + len, str, destSize - len);
        len += osal_strlen(str);
    }

    _va_end(args);

    return len;
}

/*********************************************************************
 * @fn      osal_strlen
 *
 * @brief
 *
 *   Calculates the length of a string.  The string must be null
 *   terminated.
 *
 * @param   char *pString - pointer to text string
 *
 * @return  int - number of characters
 */
int osal_strlen( const char* pString )
{
    const char* p;
    const unsigned long int* lp;

    /* Magic numbers for the algorithm */
    static const unsigned long int mask01 = 0x01010101;
    static const unsigned long int mask80 = 0x80808080;

#define LONGPTR_MASK (sizeof(long) - 1)

    /* Skip the first few bytes until we have an aligned p */
    for (p = pString; (unsigned long int)p & LONGPTR_MASK; p++)
    {
        if (*p == '\0')
        {
            return ((unsigned long int)p - (unsigned long int)pString);
        }
    }

    /*
     * Helper macro to return string length if we caught the zero
     * byte.
     */
#define testbyte(x)                                                   \
  do                                                                  \
  {                                                                   \
    if(p[x] == '\0')                                                  \
      return ((unsigned long int)p - (unsigned long int)pString + x); \
  } while(0)

    /* Scan the rest of the string using word sized operation */
    // Cast to void to prevent alignment warning
    for (lp = (const unsigned long int*)(const void*)p;; lp++)
    {
        if ((*lp - mask01) & mask80)
        {
            p = (const char*)(lp);
            testbyte(0);
            testbyte(1);
            testbyte(2);
            testbyte(3);
        }
    }

    /* NOTREACHED */
    // return (0);
}

/*********************************************************************
 * @fn      osal_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard osal_memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard osal_memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *osal_memcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  char * pd;
  char * ps;
  pd = (char*)dst;
  ps = (char*)src;

  if (len == 0 || dst == src)
  {
      { /* nothing to do */
          goto done;
      }
  }

  //
  // Copy bytes until Source is word aligned
  //
  do {
    if (len == 0) {
      goto done;
    }
    if (((int)ps & 3) == 0) {
      break;
    }
    *(char*)pd++ = *(char*)ps++;
    len--;
  } while (1);
  //
  // Copy words if possible (destination is also word aligned)
  //
  if (((int)pd & 3) == 0) {
    unsigned NumWords = len >> 2;
    while (NumWords >= 4) {
      *(uint32*)pd = *(uint32*)ps;
      pd += 4;
      ps += 4;
      *(uint32*)pd = *(uint32*)ps;
      pd += 4;
      ps += 4;
      *(uint32*)pd = *(uint32*)ps;
      pd += 4;
      ps += 4;
      *(uint32*)pd = *(uint32*)ps;
      pd += 4;
      ps += 4;
      NumWords -= 4;
    }
    if (NumWords) {
      do {
        *(uint32*)pd = *(uint32*)ps;
        pd += 4;
        ps += 4;
      } while (--NumWords);
    }
    len &= 3;
  }
  //
  // Copy half-words if possible (destination is also half-word aligned)
  //
  if (((int)pd & 1) == 0) {
    unsigned NumItems = len >> 1;
    while (NumItems >= 4) {
      *(uint16*)pd = *(uint16*)ps;
      pd += 2;
      ps += 2;
      *(uint16*)pd = *(uint16*)ps;
      pd += 2;
      ps += 2;
      *(uint16*)pd = *(uint16*)ps;
      pd += 2;
      ps += 2;
      *(uint16*)pd = *(uint16*)ps;
      pd += 2;
      ps += 2;
      NumItems -= 4;
    }
    if (NumItems) {
      do {
      *(uint16*)pd = *(uint16*)ps;
      pd += 2;
      ps += 2;
      } while (--NumItems);
    }
    len &= 1;
  }

  //
  // Copy bytes, bulk
  //
  while (len >= 4) {
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    *(char*)pd++ = *(char*)ps++;
    len -= 4;
  };
  //
  // Copy bytes, one at a time
  //
  if (len) {
    do {
      *(char*)pd++ = *(char*)ps++;
    } while (--len);
  };

done:
  return ( pd );
}

/*********************************************************************
 * @fn      osal_revmemcpy
 *
 * @brief   Generic reverse memory copy.  Starts at the end of the
 *   source buffer, by taking the source address pointer and moving
 *   pointer ahead "len" bytes, then decrementing the pointer.
 *
 *   Note: This function differs from the standard osal_memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard osal_memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *osal_revmemcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  char * pd;
  char * ps;
  pd = (char*)dst;
  ps = (char*)src;
  ps += (len-1);

  if (len == 0 || dst == src)
  {
      { /* nothing to do */
          goto done;
      }
  }

  //
  // Copy bytes, bulk
  //
  while (len >= 4) {
    *(char*)pd++ = *(char*)ps--;
    *(char*)pd++ = *(char*)ps--;
    *(char*)pd++ = *(char*)ps--;
    *(char*)pd++ = *(char*)ps--;
    len -= 4;
  };
  //
  // Copy bytes, one at a time
  //
  if (len) {
    do {
      *(char*)pd++ = *(char*)ps--;
    } while (--len);
  };

done:
  return ( pd );
}

/*********************************************************************
 * @fn      osal_memdup
 *
 * @brief   Allocates a buffer [with OSAL_ALLOC()] and copies
 *          the src buffer into the newly allocated space.
 *
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to the new allocated buffer, or NULL if
 *          allocation problem.
 */
void *osal_memdup( const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;

  pDst = osal_mem_alloc( len );
  if ( pDst )
  {
    osal_memcpy( pDst, src, len );
  }

  return ( (void *)pDst );
}

/*********************************************************************
 * @fn      osal_memcmp
 *
 * @brief
 *
 *   Generic memory compare.
 *
 * @param   src1 - source 1 address
 * @param   src2 - source 2 address
 * @param   len - number of bytes to compare
 *
 * @return  TRUE - same, FALSE - different
 */
uint8 osal_memcmp( const void GENERIC *src1, const void GENERIC *src2, unsigned int len )
{
  const uint8 GENERIC *pSrc1;
  const uint8 GENERIC *pSrc2;

  pSrc1 = src1;
  pSrc2 = src2;

  while ( len-- )
  {
    if( *pSrc1++ != *pSrc2++ )
      return FALSE;
  }
  return TRUE;
}


/*********************************************************************
 * @fn      osal_memset
 *
 * @brief
 *
 *   Set memory buffer to value.
 *
 * @param   dest - pointer to buffer
 * @param   value - what to set each uint8 of the message
 * @param   size - how big
 *
 * @return  pointer to destination buffer
 */
void *osal_memset( void *dest, uint8 value, int len )
{
  unsigned char* s = dest;
  _size_t k;

  /* Fill head and tail with minimal branching. Each
   * conditional ensures that all the subsequently used
   * offsets are well-defined and in the dest region. */

  if (!len)
  {
      return dest;
  }
  s[0] = s[len - 1] = (unsigned char)value;
  if (len <= 2)
  {
      return dest;
  }
  s[1] = s[len - 2] = (unsigned char)value;
  s[2] = s[len - 3] = (unsigned char)value;
  if (len <= 6)
  {
      return dest;
  }
  s[3] = s[len - 4] = (unsigned char)value;
  if (len <= 8)
  {
      return dest;
  }

  /* Advance pointer to align it at a 4-byte boundary,
   * and truncate len to a multiple of 4. The previous code
   * already took care of any head/tail that get cut off
   * by the alignment. */

  k = (unsigned long int)s & 3;
  s += k;
  len -= k;
  len &= (unsigned long int)-4;
  len /= 4;

  // Cast to void first to prevent alignment warning
  uint32* ws = (uint32*)(void*)s;
  uint32 wc = value & 0xFF;
  wc |= ((wc << 8) | (wc << 16) | (wc << 24));

  /* Pure C fallback with no aliasing violations. */
  for (; len; len--, ws++)
  {
      {
          *ws = wc;
      }
  }

  return dest;
}

/*********************************************************************
 * @fn      osal_build_uint16
 *
 * @brief
 *
 *   Build a uint16 out of 2 bytes (0 then 1).
 *
 * @param   swapped - 0 then 1
 *
 * @return  uint16
 */
uint16 osal_build_uint16( uint8 *swapped )
{
  return ( BUILD_UINT16( swapped[0], swapped[1] ) );
}

/*********************************************************************
 * @fn      osal_build_uint32
 *
 * @brief
 *
 *   Build a uint32 out of sequential bytes.
 *
 * @param   swapped - sequential bytes
 * @param   len - number of bytes in the uint8 array
 *
 * @return  uint32
 */
uint32 osal_build_uint32( uint8 *swapped, uint8 len )
{
  if ( len == 2 )
    return ( BUILD_UINT32( swapped[0], swapped[1], 0L, 0L ) );
  else if ( len == 3 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], 0L ) );
  else if ( len == 4 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], swapped[3] ) );
  else
    return ( (uint32)swapped[0] );
}

/******************************************************************************
 * @fn      osal_itoa
 *
 * @brief   convert a 16bit number to ASCII
 *
 * @param   num -
 *          buf -
 *          radix -
 *
 * @return  void
 *
 */
void osal_itoa(uint16 num, uint8 *buf, uint8 radix)
{
  char c,i;
  uint8 *p, rst[5];

  p = rst;
  for ( i=0; i<5; i++,p++ )
  {
    c = num % radix;  // Isolate a digit
    *p = c + (( c < 10 ) ? '0' : '7');  // Convert to Ascii
    num /= radix;
    if ( !num )
    {
      break;
    }
  }

  for ( c=0 ; c<=i; c++ )
  {
    *buf++ = *p--;  // Reverse character order
  }

  *buf = '\0';
}

/*********************************************************************
 * @fn      osal_ltoa
 *
 * @brief
 *
 *   convert a long unsigned int to a string.
 *
 * @param  l - long to convert
 * @param  buf - buffer to convert to
 * @param  radix - 10 dec, 16 hex
 *
 * @return  pointer to buffer
 */
uint8* osal_ltoa(uint32 l, uint8* buf, uint8 radix)
{
  unsigned char tmp1[10] = "", tmp2[10] = "", tmp3[10] = "";
  unsigned short num1, num2, num3;
  unsigned char i;

  buf[0] = '\0';

  if ( radix == 10 )
  {
    num1 = l % 10000;
    num2 = (l / 10000) % 10000;
    num3 = (unsigned short)(l / 100000000);

    if (num3) osal_itoa(num3, tmp3, 10);
    if (num2) osal_itoa(num2, tmp2, 10);
    if (num1) osal_itoa(num1, tmp1, 10);

    if (num3)
    {
      osal_strcpy((char*)buf, (char const*)tmp3);
      for (i = 0; i < 4 - osal_strlen((char const*)tmp2); i++)
        osal_strcat((char*)buf, "0");
    }
    osal_strcat((char*)buf, (char const*)tmp2);
    if (num3 || num2)
    {
      for (i = 0; i < 4 - osal_strlen((char const*)tmp1); i++)
        osal_strcat((char*)buf, "0");
    }
    osal_strcat((char*)buf, (char const*)tmp1);
    if (!num3 && !num2 && !num1)
      osal_strcpy((char*)buf, "0");
  }
  else if ( radix == 16 )
  {
    num1 = l & 0x0000FFFF;
    num2 = l >> 16;

    if (num2) osal_itoa(num2, tmp2, 16);
    if (num1) osal_itoa(num1, tmp1, 16);

    if (num2)
    {
      osal_strcpy((char*)buf,(char const*)tmp2);
      for (i = 0; i < 4 - osal_strlen((char const*)tmp1); i++)
        osal_strcat((char*)buf, "0");
    }
    osal_strcat((char*)buf, (char const*)tmp1);
    if (!num2 && !num1)
      osal_strcpy((char*)buf, "0");
  }
  else
    return NULL;

  return buf;
}

/*********************************************************************
 * @fn      osal_atol
 *
 * @brief
 *
 *   convert a string int to a long unsigned.
 *
 * @param  s - buffer to convert to
 *
 * @return  long unsigned
 */
long osal_atol(const char *s)
{
    long r = 0;
    int neg = 0;
    switch (*s)
    {
    case '-':
        neg = 1;
    case '+':
        s++;
        break;
    }
    while (*s >= '0' && *s <= '9')
    {
        int n = *s++ - '0';
        if (neg)
            n = -n;
        r = r * 10 + n;
    }
    return r;
}

/*********************************************************************
 * @fn        osal_rand
 *
 * @brief    Random number generator
 *
 * @param   none
 *
 * @return  uint32 - new random number
 */
uint32 osal_rand( void )
{
  return ( _rand() );
}

/*********************************************************************
 * @fn      osal_buffer_uint32
 *
 * @brief
 *
 *   Buffer an uint32 value - LSB first.
 *
 * @param   buf - buffer
 * @param   val - uint32 value
 *
 * @return  pointer to end of destination buffer
 */
uint8* osal_buffer_uint32( uint8 *buf, uint32 val )
{
  *buf++ = BREAK_UINT32( val, 0 );
  *buf++ = BREAK_UINT32( val, 1 );
  *buf++ = BREAK_UINT32( val, 2 );
  *buf++ = BREAK_UINT32( val, 3 );

  return buf;
}

/*********************************************************************
 * @fn      osal_buffer_uint24
 *
 * @brief
 *
 *   Buffer an uint24 value - LSB first. Note that type uint24 is
 *   typedef to uint32 in OSAL_Comdef.h
 *
 * @param   buf - buffer
 * @param   val - uint24 value
 *
 * @return  pointer to end of destination buffer
 */
uint8* osal_buffer_uint24( uint8 *buf, uint24 val )
{
  *buf++ = BREAK_UINT32( val, 0 );
  *buf++ = BREAK_UINT32( val, 1 );
  *buf++ = BREAK_UINT32( val, 2 );

  return buf;
}

/*********************************************************************
 * @fn      osal_isbufset
 *
 * @brief
 *
 *   Is all of the array elements set to a value?
 *
 * @param   buf - buffer to check
 * @param   val - value to check each array element for
 * @param   len - length to check
 *
 * @return  TRUE if all "val"
 *          FALSE otherwise
 */
uint8 osal_isbufset( uint8 *buf, uint8 val, uint8 len )
{
  uint8 x;

  if ( buf == NULL )
  {
    return ( FALSE );
  }

  for ( x = 0; x < len; x++ )
  {
    // Check for non-initialized value
    if ( buf[x] != val )
    {
      return ( FALSE );
    }
  }
  return ( TRUE );
}


/*********************************************************************
 */
