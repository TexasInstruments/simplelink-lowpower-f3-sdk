/******************************************************************************

 @file  comdef.h

 @brief This file contains useful macros and data types

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2004-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/**
 *  @file  comdef.h
 *  @brief      Common Defines
 */

#ifndef COMDEF_H
#define COMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include <stdbool.h>

/// @cond NODOC

/*********************************************************************
 * Lint Keywords
 */
#define VOID (void)

/*********************************************************************
 * CONSTANTS
 */

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true 1
#endif

#ifndef GENERIC
  #define GENERIC
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef UTRUE
#define UTRUE 1U
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef UFALSE
#define UFALSE 0U
#endif

#ifndef NULL
#define NULL 0L
#endif

/// @endcond // NODOC

/*** Generic Status Return Values ***/
#define SUCCESS                   0x00 //!< SUCCESS
#define USUCCESS                  0U   //!< SUCCESS
#define FAILURE                   0x01 //!< Failure
#define UFAILURE                  1U   //!< Failure
#define INVALIDPARAMETER          0x02 //!< Invalid Parameter
#define UINVALIDPARAMETER         2U   //!< Invalid Parameter
#define INVALID_TASK              0x03 //!< Invalid Task
#define MSG_BUFFER_NOT_AVAIL      0x04 //!< No HCI Buffer is Available
#define INVALID_MSG_POINTER       0x05 //!< Invalid Message Pointer
#define INVALID_EVENT_ID          0x06 //!< Invalid Event ID
#define INVALID_INTERRUPT_ID      0x07 //!< Invalid Interupt ID
#define NO_TIMER_AVAIL            0x08 //!< No Timer Available
#define NV_ITEM_UNINIT            0x09 //!< NV Item Uninitialized
#define NV_OPER_FAILED            0x0A //!< NV Operation Failed
#define INVALID_MEM_SIZE          0x0B //!< Invalid Memory Size
#define NV_BAD_ITEM_LEN           0x0C //!< NV Bad Item Length

/*********************************************************************
 * TYPEDEFS
 */

/// @cond NODOC

// Generic Status return
typedef uint8_t Status_t;

// Data types
typedef signed   char   int8;
typedef unsigned char   uint8;

typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;

typedef uint32_t        halDataAlign_t;

/// @endcond // NODOC

/*********************************************************************
 * MACROS
 */

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef BF
#define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#ifndef ABS
#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif

// Break a uint32_t variable into a specific byte
#define BREAK_UINT32(var, ByteNum) \
            ((((var) >> ((ByteNum) * 8)) & 0x00FF))

// Build a uint32_t from four bytes
#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) ( (((Byte3) & 0xFF) << 24) | \
                           (((Byte2) & 0xFF) << 16) | \
                           (((Byte1) & 0xFF) << 8)  | \
                           ((Byte0) & 0xFF) )

// Build a uint16_t from two bytes
#define BUILD_UINT16(loByte, hiByte) ( ((loByte) & 0xFF) | (((hiByte) & 0xFF) << 8) )

// Get the high byte of a uint16_t
#define HI_UINT16(a) (((a) >> 8) & 0xFF)

// Get the low byte of a uint16_t
#define LO_UINT16(a) ((a) & 0xFF)

// Get a uint16_t from a pointer to a byte array
#define GET_UINT16(a) ((uint16_t)((*((uint8_t *)a)) + (((uint16_t)(*(((uint8_t *)a) + 1))) << 8)))

// Build a uint8_t from two nibbles
#define BUILD_UINT8(hiByte, loByte) \
      (((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4))

// Get the high nibble of a uint8_t
#define HI_UINT8(a) (((a) >> 4) & 0x0F)

// Get the low nibble of a uint8_t
#define LO_UINT8(a) ((a) & 0x0F)

// Write the 32bit value of 'val' in little endian format to the buffer pointed
// to by pBuf, and increment pBuf by 4
#define UINT32_TO_BUF_LITTLE_ENDIAN(pBuf, val) \
  do { \
    *(pBuf)++ = ((val >>  0) & 0xFF); \
    *(pBuf)++ = ((val >>  8) & 0xFF); \
    *(pBuf)++ = ((val >> 16) & 0xFF); \
    *(pBuf)++ = ((val >> 24) & 0xFF); \
  } while (0)

// Return the 32bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_UINT32_LITTLE_ENDIAN(pBuf) (((pBuf) += 4), BUILD_UINT32((pBuf)[-1], (pBuf)[-2], (pBuf)[-3], (pBuf)[-4]))

#ifndef GET_BIT
// Macro to get the value of a specific bit in a byte array
#define GET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] & BV((IDX) % 8)) ? TRUE : FALSE)
#endif

#ifndef SET_BIT
// Macro to set a specific bit in a byte array
#define SET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] |= BV((IDX) % 8)))
#endif

#ifndef CLR_BIT
// Macro to clear a specific bit in a byte array
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

/*********************************************************************
 * MEMORY ATTRIBUTES
 */

#if defined (__IAR_SYSTEMS_ICC__)
#define XDATA
#define CODE
#define DATA_ALIGN(x)                   _Pragma data_alignment=(x)
#define ALIGNED
#define PACKED                          __packed
#define PACKED_STRUCT                   PACKED struct
#define PACKED_TYPEDEF_STRUCT           PACKED typedef struct
#define PACKED_TYPEDEF_CONST_STRUCT     PACKED typedef const struct
#define PACKED_TYPEDEF_UNION            PACKED typedef union
#define PACKED_ALIGNED                  PACKED
#define PACKED_ALIGNED_TYPEDEF_STRUCT   PACKED_TYPEDEF_STRUCT

#elif defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#define XDATA
#define CODE
#define DATA
#define NEARFUNC
#define ALIGNED
#define PACKED                              __attribute__((packed))
#define PACKED_STRUCT                       struct PACKED
#define PACKED_TYPEDEF_STRUCT               typedef struct PACKED
#define PACKED_TYPEDEF_CONST_STRUCT         typedef const struct PACKED
#define PACKED_TYPEDEF_UNION                typedef union PACKED
#define PACKED_ALIGNED                      __attribute__((packed,aligned(4)))
#define PACKED_ALIGNED_TYPEDEF_STRUCT       typedef struct PACKED_ALIGNED

#elif defined (__GNUC__)
#define ALIGNED                             __attribute__((aligned(4)))
#ifdef CC33xx
#define PACKED                              __attribute__((aligned(1)))  __attribute__((packed))
#else
#define PACKED                              __attribute__((__packed__))
#endif
#define PACKED_STRUCT                       struct PACKED
#define PACKED_TYPEDEF_STRUCT               typedef struct PACKED
#define PACKED_TYPEDEF_CONST_STRUCT         typedef const struct PACKED
#define PACKED_TYPEDEF_UNION                typedef union PACKED
#define PACKED_ALIGNED                      __attribute__((packed,aligned(4)))
#define PACKED_ALIGNED_TYPEDEF_STRUCT       typedef struct PACKED_ALIGNED
#endif

/*********************************************************************
 * COMPILER MACROS
 */

/* ----------- IAR Compiler ----------- */
#ifdef __IAR_SYSTEMS_ICC__
#define ASM_NOP    asm("NOP")
#define NO_INIT    __no_init
#define WEAK_FUNC __weak

/* ----------- CCS Compiler ----------- */
#elif defined __TI_COMPILER_VERSION || defined __TI_COMPILER_VERSION__
#define ASM_NOP    asm(" NOP")
#define NO_INIT    __attribute__((noinit))

/* ----------- GNU Compiler ----------- */
#elif defined __GNUC__
#define ASM_NOP __asm__ __volatile__ ("nop")
#define WEAK_FUNC __attribute__((__weak__))

/* ----------- Unrecognized Compiler ----------- */
#else
#error "ERROR: Unknown compiler."
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* COMDEF_H */
