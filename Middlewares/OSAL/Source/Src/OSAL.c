/**************************************************************************************************
  Filename:       OSAL.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:    This API allows the software components in the Z-stack to be written
                  independently of the specifics of the operating system, kernel or tasking
                  environment (including control loops or connect-to-interrupt systems).
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_Nv.h"
#include "OSAL_Tasks.h"
#include "OSAL_Clock.h"
#include "OSAL_Memory.h"
#include "OSAL_Timers.h"
#include "OSAL_PwrMgr.h"

#include "printf.h"

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

// Message Pool Definitions
osal_msg_q_t osal_qHead;


/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*
 * Process Polls
 */
extern void Hal_ProcessPoll (void);

/*********************************************************************
 * LOCAL VARIABLES
 */

// Index of active task
static uint8 activeTaskID = TASK_NO_TASK;
// osal_int_enable state
static halIntState_t osal_int_state;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 urgent );

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
    return SUCCESS;
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
 * @brief   Allocates a buffer [with osal_mem_alloc()] and copies
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
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_msg_allocate
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *
 *
 * @param   uint8 len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */
uint8 * osal_msg_allocate( uint16 len )
{
  osal_msg_hdr_t *hdr;

  if ( len == 0 )
    return ( NULL );

  hdr = (osal_msg_hdr_t *) osal_mem_alloc( (short)(len + sizeof( osal_msg_hdr_t )) );
  if ( hdr )
  {
    hdr->next = NULL;
    hdr->len = len;
    hdr->dest_id = TASK_NO_TASK;
    return ( (uint8 *) (hdr + 1) );
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      osal_msg_deallocate
 *
 * @brief
 *
 *    This function is used to deallocate a message buffer. This function
 *    is called by a task (or processing element) after it has finished
 *    processing a received message.
 *
 *
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_MSG_POINTER
 */
uint8 osal_msg_deallocate( uint8 *msg_ptr )
{
  uint8 *x;

  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // don't deallocate queued buffer
  if ( OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
    return ( MSG_BUFFER_NOT_AVAIL );

  x = (uint8 *)((uint8 *)msg_ptr - sizeof( osal_msg_hdr_t ));

  osal_mem_free( (void *)x );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_send
 *
 * @brief
 *
 *    This function is called by a task to send a command message to
 *    another task or processing element.  The sending_task field must
 *    refer to a valid task, since the task ID will be used
 *    for the response message.  This function will also set a message
 *    ready event in the destination tasks event list.
 *
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_send( uint8 destination_task, uint8 *msg_ptr )
{
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, FALSE ) );
}

/*********************************************************************
 * @fn      osal_msg_push_front
 *
 * @brief
 *
 *    This function is called by a task to push a command message
 *    to the head of the OSAL queue. The destination_task field
 *    must refer to a valid task, since the task ID will be used to
 *    send the message to. This function will also set a message
 *    ready event in the destination task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_push_front( uint8 destination_task, uint8 *msg_ptr )
{
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, TRUE ) );
}

/*********************************************************************
 * @fn      osal_msg_enqueue_push
 *
 * @brief
 *
 *    This function is called by a task to either enqueue (append to
 *    queue) or push (prepend to queue) a command message to the OSAL
 *    queue. The destination_task field must refer to a valid task,
 *    since the task ID will be used to send the message to. This 
 *    function will also set a message ready event in the destination
 *    task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 * @param   uint8 push - TRUE to push, otherwise enqueue
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 push )
{
  if ( msg_ptr == NULL )
  {
    return ( INVALID_MSG_POINTER );
  }

  if ( destination_task >= tasksCnt )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }

  // Check the message header
  if ( OSAL_MSG_NEXT( msg_ptr ) != NULL ||
       OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_MSG_POINTER );
  }

  OSAL_MSG_ID( msg_ptr ) = destination_task;

  if ( push == TRUE )
  {
    // prepend the message
    osal_msg_push( &osal_qHead, msg_ptr );
  }
  else
  {
    // append the message
    osal_msg_enqueue( &osal_qHead, msg_ptr );
  }

  // Signal the task that a message is waiting
  osal_set_event( destination_task, SYS_EVENT_MSG );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_receive
 *
 * @brief
 *
 *    This function is called by a task to retrieve a received command
 *    message. The calling task must deallocate the message buffer after
 *    processing the message using the osal_msg_deallocate() call.
 *
 * @param   uint8 task_id - receiving tasks ID
 *
 * @return  *uint8 - message information or NULL if no message
 */
uint8 *osal_msg_receive( uint8 task_id )
{
  osal_msg_hdr_t *listHdr;
  osal_msg_hdr_t *prevHdr = NULL;
  osal_msg_hdr_t *foundHdr = NULL;
  halIntState_t   intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Point to the top of the queue
  listHdr = osal_qHead;

  // Look through the queue for a message that belongs to the asking task
  while ( listHdr != NULL )
  {
    if ( (listHdr - 1)->dest_id == task_id )
    {
      if ( foundHdr == NULL )
      {
        // Save the first one
        foundHdr = listHdr;
      }
      else
      {
        // Second msg found, stop looking
        break;
      }
    }
    if ( foundHdr == NULL )
    {
      prevHdr = listHdr;
    }
    listHdr = OSAL_MSG_NEXT( listHdr );
  }

  // Is there more than one?
  if ( listHdr != NULL )
  {
    // Yes, Signal the task that a message is waiting
    osal_set_event( task_id, SYS_EVENT_MSG );
  }
  else
  {
    // No more
    osal_clear_event( task_id, SYS_EVENT_MSG );
  }

  // Did we find a message?
  if ( foundHdr != NULL )
  {
    // Take out of the link list
    osal_msg_extract( &osal_qHead, foundHdr, prevHdr );
  }

  // Release interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ( (uint8*) foundHdr );
}

/**************************************************************************************************
 * @fn          osal_msg_find
 *
 * @brief       This function finds in place an OSAL message matching the task_id and event
 *              parameters.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match.
 *
 * output parameters
 *
 * None.
 *
 * @return      NULL if no match, otherwise an in place pointer to the matching OSAL message.
 **************************************************************************************************
 */
osal_event_hdr_t *osal_msg_find(uint8 task_id, uint8 event)
{
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if (((pHdr-1)->dest_id == task_id) && (((osal_event_hdr_t *)pHdr)->event == event))
    {
      break;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return (osal_event_hdr_t *)pHdr;
}

/**************************************************************************************************
 * @fn          osal_msg_count
 *
 * @brief       This function counts the number of messages, in the OSAL message queue with a
 *              a given task ID and message event type.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match. 0xFF for 
 *              all events.
 *
 * output parameters
 *
 * None.
 *
 * @return      The number of OSAL messages that match the task ID and Event.
 **************************************************************************************************
 */
uint8 osal_msg_count( uint8 task_id, uint8 event )
{
  uint8 count = 0;
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if ( ((pHdr-1)->dest_id == task_id) 
        && ((event == 0xFF) || (((osal_event_hdr_t *)pHdr)->event == event)) )
    {
      count++;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return ( count );
}

/*********************************************************************
 * @fn      osal_msg_enqueue
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_enqueue( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  void *list;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
  }
  else
  {
    // Find end of queue
    for ( list = *q_ptr; OSAL_MSG_NEXT( list ) != NULL; list = OSAL_MSG_NEXT( list ) );

    // Add message to end of queue
    OSAL_MSG_NEXT( list ) = msg_ptr;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_dequeue
 *
 * @brief
 *
 *    This function dequeues an OSAL message from an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 *
 * @return  void * - pointer to OSAL message or NULL of queue is empty.
 */
void *osal_msg_dequeue( osal_msg_q_t *q_ptr )
{
  void *msg_ptr = NULL;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( *q_ptr != NULL )
  {
    // Dequeue message
    msg_ptr = *q_ptr;
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
    OSAL_MSG_NEXT( msg_ptr ) = NULL;
    OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return msg_ptr;
}

/*********************************************************************
 * @fn      osal_msg_push
 *
 * @brief
 *
 *    This function pushes an OSAL message to the head of an OSAL
 *    queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_push( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Push message to head of queue
  OSAL_MSG_NEXT( msg_ptr ) = *q_ptr;
  *q_ptr = msg_ptr;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_extract
 *
 * @brief
 *
 *    This function extracts and removes an OSAL message from the
 *    middle of an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message to be extracted
 * @param   void *prev_ptr  - OSAL message before msg_ptr in queue
 *
 * @return  none
 */
void osal_msg_extract( osal_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( msg_ptr == *q_ptr )
  {
    // remove from first
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
  }
  else
  {
    // remove from middle
    OSAL_MSG_NEXT( prev_ptr ) = OSAL_MSG_NEXT( msg_ptr );
  }
  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_enqueue_max
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue if
 *    the length of the queue is less than max.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 * @param   uint8 max - maximum length of queue
 *
 * @return  TRUE if message was enqueued, FALSE otherwise
 */
uint8 osal_msg_enqueue_max( osal_msg_q_t *q_ptr, void *msg_ptr, uint8 max )
{
  void *list;
  uint8 ret = FALSE;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
    ret = TRUE;
  }
  else
  {
    // Find end of queue or max
    list = *q_ptr;
    max--;
    while ( (OSAL_MSG_NEXT( list ) != NULL) && (max > 0) )
    {
      list = OSAL_MSG_NEXT( list );
      max--;
    }

    // Add message to end of queue if max not reached
    if ( max != 0 )
    {
      OSAL_MSG_NEXT( list ) = msg_ptr;
      ret = TRUE;
    }
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ret;
}

/*********************************************************************
 * @fn      osal_set_event
 *
 * @brief
 *
 *    This function is called to set the event flags for a task. The
 *    event passed in is OR'd into the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to set
 *
 * @return  SUCCESS, MSG_BUFFER_NOT_AVAIL, FAILURE, INVALID_TASK
 */
uint8 osal_set_event( uint8 task_id, uint16 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] |= event_flag;  // Stuff the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_clear_event
 *
 * @brief
 *
 *    This function is called to clear the event flags for a task. The
 *    event passed in is masked out of the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to clear
 *
 * @return  SUCCESS, INVALID_TASK
 */
uint8 osal_clear_event( uint8 task_id, uint16 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] &= ~(event_flag);   // Clear the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
   else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_isr_register
 *
 * @brief
 *
 *   This function is called to register a service routine with an
 *   interrupt. When the interrupt occurs, this service routine is called.
 *
 * @param   uint8 interrupt_id - Interrupt number
 * @param   void (*isr_ptr)( uint8* ) - function pointer to ISR
 *
 * @return  SUCCESS, INVALID_INTERRUPT_ID,
 */
uint8 osal_isr_register( uint8 interrupt_id, void (*isr_ptr)( uint8* ) )
{
  // Remove these statements when functionality is complete
  (void)interrupt_id;
  (void)isr_ptr;
  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_int_enable
 *
 * @brief
 *
 *   This function is called to enable an interrupt. Once enabled,
 *   occurrence of the interrupt causes the service routine associated
 *   with that interrupt to be called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are enabled.
 *   If a single interrupt is passed in, then interrupts still have
 *   to be enabled with another call to INTS_ALL.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_enable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_ENABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_int_disable
 *
 * @brief
 *
 *   This function is called to disable an interrupt. When a disabled
 *   interrupt occurs, the service routine associated with that
 *   interrupt is not called.
 *
 *   If INTS_ALL is the interrupt_id, interrupts (in general) are disabled.
 *   If a single interrupt is passed in, then just that interrupt is disabled.
 *
 * @param   uint8 interrupt_id - Interrupt number
 *
 * @return  SUCCESS or INVALID_INTERRUPT_ID
 */
uint8 osal_int_disable( uint8 interrupt_id )
{

  if ( interrupt_id == INTS_ALL )
  {
    HAL_DISABLE_INTERRUPTS();
    return ( SUCCESS );
  }
  else
  {
    return ( INVALID_INTERRUPT_ID );
  }
}

/*********************************************************************
 * @fn      osal_init_system
 *
 * @brief
 *
 *   This function initializes the "task" system by creating the
 *   tasks defined in the task table (OSAL_Tasks.h).
 *
 * @param   void
 *
 * @return  SUCCESS
 */
uint8 osal_init_system( void )
{
  // Turn off interrupts
  osal_int_disable(INTS_ALL);

  // Hook Initialize
  OSAL_Init_Hook();

  // Initialize NV System
  osal_nv_init(NULL);

  // Initialize the Memory Allocation System
  osal_mem_init();

  // Initialize the message queue
  osal_qHead = NULL;

  // Initialize the timers
  osalTimerInit();

  // Initialize the Power Management System
  osal_pwrmgr_init();
  
#if defined ( POWER_SAVING )
  osal_pwrmgr_device(PWRMGR_BATTERY);
#endif

  // Initialize the system tasks.
  osalInitTasks();

  // Setup efficient search for the first free block of heap.
  osal_mem_kick();

  // Allow interrupts
  osal_int_enable(INTS_ALL);

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_start_system
 *
 * @brief
 *
 *   This function is the main loop function of the task system (if
 *   ZBIT and UBIT are not defined). This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{
  // Initialize the operating system
  osal_init_system();
  
  for(;;)  // Forever Loop
  {
    osal_run_system();
  }
}

/*********************************************************************
 * @fn      osal_run_system
 *
 * @brief
 *
 *   This function will make one pass through the OSAL taskEvents table
 *   and call the task_event_processor() function for the first task that
 *   is found with at least one event pending. If there are no pending
 *   events (all tasks), this function puts the processor into Sleep.
 *
 * @param   void
 *
 * @return  none
 */
void osal_run_system( void )
{
  uint8 idx = 0;

#ifndef USE_SYSTICK_IRQ
  osalTimeUpdate();
#endif

  Hal_ProcessPoll();
  
  do {
    if (tasksEvents[idx])  // Task is highest priority that is ready.
    {
      break;
    }
  } while (++idx < tasksCnt);

  if (idx < tasksCnt)
  {
    uint16 events;
    halIntState_t intState;

    HAL_ENTER_CRITICAL_SECTION(intState);
    events = tasksEvents[idx];
    tasksEvents[idx] = 0;  // Clear the Events for this task.
    HAL_EXIT_CRITICAL_SECTION(intState);

    activeTaskID = idx;
    events = (tasksArr[idx])( idx, events );
    activeTaskID = TASK_NO_TASK;

    HAL_ENTER_CRITICAL_SECTION(intState);
    tasksEvents[idx] |= events;  // Add back unprocessed events to the current task.
    HAL_EXIT_CRITICAL_SECTION(intState);
  }
#if defined( POWER_SAVING )
  else  // Complete pass through all task events with no activity?
  {
    osal_pwrmgr_powerconserve();  // Put the processor/system into sleep
  }
#endif

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
 * @fn      osal_self
 *
 * @brief
 *
 *   This function returns the task ID of the current (active) task.
 *
 * @param   void
 *
 * @return   active task ID or TASK_NO_TASK if no task is active
 */
uint8 osal_self( void )
{
  return ( activeTaskID );
}

/*********************************************************************
 */
