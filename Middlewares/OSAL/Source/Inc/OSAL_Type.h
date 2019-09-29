/**************************************************************************************************
  Filename:       OSAL_Type.h
  Revised:        $Date: 2010-07-28 08:42:48 -0700 (Wed, 28 Jul 2010) $
  Revision:       $Revision: 23160 $

  Description:    Type definitions and macros.
**************************************************************************************************/

#ifndef OSAL_TYPE_H
#define OSAL_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

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
typedef              int32  bool;
typedef               bool  halIntState_t;
typedef             uint32  halDataAlign_t;
typedef              uint8  Status_t;
 
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
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_TYPE_H */
