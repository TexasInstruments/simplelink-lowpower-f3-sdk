/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: ZigBee trace. Application should include it.
*/

#ifndef ZB_LOGGER_H
#define ZB_LOGGER_H 1

/*! \addtogroup ZB_TRACE */
/*! @{ */

#if defined ZB_TRACE_LEVEL || defined DOXYGEN

#ifndef DOXYGEN
#define TRACE_ENABLED_(mask,lev) ((lev) <= ZB_TRACE_LEVEL && ((mask) & ZB_TRACE_MASK))
#endif

/**
 *  @brief Checks if a trace is enabled for the provided level.
 *  To be used in constructions like:
 *  @code
 *  if (TRACE_ENABLED(TRACE_APS3))
 *  {
 *    call_some_complex_trace();
 *  }
 *  @endcode
 *  @param m Trace level macro
 *  @return 1 if enabled, 0 if disabled
 */
#define TRACE_ENABLED(m) TRACE_ENABLED_(m)


#if defined ZB_TRACE_TO_FILE || defined DOXYGEN

#ifndef DOXYGEN
/**
   \par Trace to the file means trace to the disk file using printf() or its analog.
   Tricks to decrease code size by excluding format strings are not used.
 */
void zb_trace_init_file(zb_char_t *name);
void zb_trace_deinit_file();
#endif


/**
 Initializes trace subsystem.

 @param name Trace file name component
*/
#define TRACE_INIT(name)   zb_trace_init_file(name)

/**
 Deinitializes trace subsystem.
*/
#define TRACE_DEINIT zb_trace_deinit_file

#ifndef DOXYGEN
/**
 *  @cond internals_doc
 *  @brief Print trace message
 */
void zb_trace_msg_file(
    zb_char_t *format, zb_char_t *file_name, zb_int_t line_number, zb_int_t args_size, ...);

#define _T0(...) __VA_ARGS__
#define _T1(s, l, fmts, args) if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_msg_file(fmts, _T0 args)

#endif

/** @endcond */ /* internals_doc */

/**
 *  @brief Puts trace output.
 *  @param lm Trace subsystem and level marker
 *  @param fmts printf()-like format string
 *  @param args Format string substitution parameters with a size marker.
 */
#define TRACE_MSG(lm, fmts, args) _T1(lm, fmts, args)

#elif defined ZB_TRACE_TO_PORT

/*
  8051 trace does not use format string in the code to save code space.

- will modify trace at device only, Linux will work as before
- trace implementation will hex dump all arguments as set of bytes
- external utility will parse dump, divide trace arguments dump into separate arguments and
convert hex-int, unsigned etc.
- utility will get argument strings from the source files (trace macros) and find it
by file:line
- Add one more parameter to the trace macro: sum of the trace argument sizes.
Define readable constants like
#define FMT_D_HD_X 5
- create script/program to modify existing trace calls
- combine dump parse utility functionality with win_com_dump, so it will produce human-readable trace

 */

#ifndef ZB_SERIAL_FOR_TRACE
#define TRACE_INIT(name)
#else
#define TRACE_INIT(name) zb_osif_serial_init()
#endif
/* No trace deinit */
#define TRACE_DEINIT()

#ifndef ZB_BINARY_TRACE
void zb_trace_msg_port(
    zb_char_t ZB_IAR_CODE *file_name,
    zb_int_t line_number,
    zb_uint8_t args_size, ...);

#else

#ifndef ZB_TRACE_FILE_ID
//#warning ZB_TRACE_FILE_ID undefined! Insert #define ZB_TRACE_FILE_ID NNN before 1-st #include
#define ZB_TRACE_FILE_ID -1
#endif

extern zb_int_t g_trace_level, g_trace_mask, g_o_trace_level;

#define ZB_SET_TRACE_LEVEL(l) g_trace_level = (l);
#define ZB_SET_TRACE_MASK(m) g_trace_mask = (m);
#define ZB_SET_TRACE_OFF() g_o_trace_level = g_trace_level, g_trace_level = -1
#define ZB_SET_TRACE_ON() g_trace_level = g_o_trace_level

void zb_trace_msg_port(
    zb_uint16_t file_id,
    zb_uint16_t line_number,
    zb_uint16_t args_size, ...);

#endif

#ifdef ZB_BINARY_TRACE
#define _T1(s, l, args) \
if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_msg_port args
#else
#define _T1(s, l, args) \
if ((zb_int_t)ZB_TRACE_LEVEL>=(zb_int_t)l && ((s) & ZB_TRACE_MASK)) zb_trace_msg_port args
#endif


#define TRACE_MSG(lm, fmt, args) _T1(lm, args)

#else

#error Must define either ZB_TRACE_TO_FILE or ZB_TRACE_TO_PORT

#endif  /* trace type */


#else  /* if trace off */

#ifndef KEIL
#define TRACE_MSG(...)
#else
/* Keil does not support vararg macros */
#define TRACE_MSG(a,b,c)
#endif

#define TRACE_INIT(name)
#define TRACE_DEINIT(c)

#define TRACE_ENABLED(m) 0

#endif  /* trace on/off */


#ifdef ZB_TRACE_TO_FILE

/** @cond internals_doc */

/** @brief Traces format for 64-bit address. */
#define TRACE_FORMAT_64 "%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx"
#define TRACE_FORMAT_128 "%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx.%hx"

/** @brief Traces format arguments for 64-bit address. */
#define TRACE_ARG_64(a) (zb_uint8_t)((a)[7]),(zb_uint8_t)((a)[6]),(zb_uint8_t)((a)[5]),(zb_uint8_t)((a)[4]),(zb_uint8_t)((a)[3]),(zb_uint8_t)((a)[2]),(zb_uint8_t)((a)[1]),(zb_uint8_t)((a)[0])
#define TRACE_ARG_128(a) (zb_uint8_t)((a)[0]),(zb_uint8_t)((a)[1]),(zb_uint8_t)((a)[2]),(zb_uint8_t)((a)[3]),(zb_uint8_t)((a)[4]),(zb_uint8_t)((a)[5]),(zb_uint8_t)((a)[6]),(zb_uint8_t)((a)[7]),(zb_uint8_t)((a)[8]),(zb_uint8_t)((a)[9]),(zb_uint8_t)((a)[10]),(zb_uint8_t)((a)[11]),(zb_uint8_t)((a)[12]),(zb_uint8_t)((a)[13]),(zb_uint8_t)((a)[14]),(zb_uint8_t)((a)[15])

/** @endcond */ /* internals_doc */

#elif defined(SDCC)

#define TRACE_FORMAT_64 "%A"
#define TRACE_FORMAT_128 "%A.%A"

typedef struct zb_addr64_struct_s
{
  zb_uint16_t a1;
  zb_uint16_t a2;
  zb_uint16_t a3;
  zb_uint16_t a4;
} zb_addr64_struct_t;

/* Pass 8-bytes address as structure by value */
#define TRACE_ARG_64(a) ((zb_addr64_struct_t ZB_SDCC_XDATA * ZB_SDCC_XDATA)a)->a1, ((zb_addr64_struct_t ZB_SDCC_XDATA * ZB_SDCC_XDATA)a)->a2, ((zb_addr64_struct_t ZB_SDCC_XDATA * ZB_SDCC_XDATA)a)->a3, ((zb_addr64_struct_t ZB_SDCC_XDATA * ZB_SDCC_XDATA)a)->a4


#else

/** @cond internals_doc */

/** @brief Traces format for 64-bit address - single argument for 8051. */
#define TRACE_FORMAT_64 "%A"
#define TRACE_FORMAT_128 "%A.%A"

typedef struct zb_addr64_struct_s
{
  zb_64bit_addr_t addr;
} zb_addr64_struct_t;

typedef struct zb_byte128_struct_s
{
  zb_uint8_t d[16];
} zb_byte128_struct_t;

/* Pass 8-bytes address as structure by value */
#define TRACE_ARG_64(a) *((zb_addr64_struct_t *)a)
/* Pass 16-bytes key as structure by value */
#define TRACE_ARG_128(a) *((zb_byte128_struct_t *)a)

#endif  /* ti file / !to file */

/** @endcond */ /* internals_doc */

/**
 *  @name General trace messages
 *  @{
 */
/** @brief Error message. */
#define TRACE_ERROR -1, 1
/** @brief Information message level 1. */
#define TRACE_INFO1 -1, 2
/** @brief Information message level 2. */
#define TRACE_INFO2 -1, 3
/** @brief Information message level 3. */
#define TRACE_INFO3 -1, 4

/**
 *  @}
 */

/**
 *  @name Trace subsystems
 *  @{
 */
#define TRACE_SUBSYSTEM_COMMON    0x0001  /**< Common subsystem. */
#define TRACE_SUBSYSTEM_OSIF      0x0002  /**< OSIF subsystem. */
#define TRACE_SUBSYSTEM_MAC       0x0004  /**< MAC subsystem. */
#define TRACE_SUBSYSTEM_NWK       0x0008  /**< NWK subsystem. */
#define TRACE_SUBSYSTEM_APS       0x0010  /**< APS subsystem. */
#define TRACE_SUBSYSTEM_AF        0x0020  /**< AF subsystem. */
#define TRACE_SUBSYSTEM_ZDO       0x0040  /**< ZDO subsystem. */
#define TRACE_SUBSYSTEM_SECUR     0x0080  /**< Security subsystem. */
#define TRACE_SUBSYSTEM_ZCL       0x0100  /**< ZCL subsystem. */
#define TRACE_SUBSYSTEM_ZLL       0x0200  /**< ZLL subsystem. */
#define TRACE_SUBSYSTEM_SPI       0x0400  /**< SPI subsystem */
#define TRACE_SUBSYSTEM_MACLL     0x0400  /**< SSL subsystem - same as SPI */
#define TRACE_SUBSYSTEM_APP       0x0800  /**< User application */
#define TRACE_SUBSYSTEM_SPECIAL1  0x1000  /**< Some special debug */
#define TRACE_SUBSYSTEM_SPECIAL2  0x2000  /**< Some special debug */
#define TRACE_SUBSYSTEM_ZGP       0x4000  /**< ZGP subsystem */
#define TRACE_SUBSYSTEM_USB       0x8000  /**< USB subsystem */
/* to be continued... */

/** @} */ /* Trace subsystems */


#ifdef ZB_TRACE_LEVEL
#ifndef ZB_TRACE_MASK
#ifdef ZB_NS_BUILD
#define ZB_TRACE_MASK ((-1) & (~0x1000))
#elif defined ZB_UZ2410
#define ZB_TRACE_MASK -1
/* 1fb == all but MAC */
//#define ZB_TRACE_MASK 0x1FB
//#define ZB_TRACE_MASK 0xffff & (~(TRACE_SUBSYSTEM_SECUR|TRACE_SUBSYSTEM_ZCL))
#elif defined C8051F120

//#define ZB_TRACE_MASK 0xffff & (~(TRACE_SUBSYSTEM_SECUR|TRACE_SUBSYSTEM_ZCL))
#define ZB_TRACE_MASK -1
#else
#define ZB_TRACE_MASK -1
#endif  /* uz2410... */
#endif  /* ifndef trace_mask */
#endif  /* ifdef trace level */

/**
 *  @name Trace data format definitions
 *  @{
 */

/*
  Trace format constants for serial trace
*/

/* Keil and sdcc put byte values to the stack as is, but IAR/8051 casts it to 16-bit
 * integers, so constant lengths differs */

#ifdef KEIL

/**
 *  @brief Calculates trace argument size.
 *  @param n_h Number of Hs/Cs.
 *  @param n_d Number of Ds.
 *  @param n_l Number of Ls.
 *  @param n_p Number of Ps.
 *  @param n_a Number of As.
 *  @hideinitializer
 */
/* Keil pass 1-byte to varargs as is, pointer is 3-bytes, short is 2-bytes */

#ifndef ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,__LINE__, (n_h + n_d*2 + n_l*4 + n_p*3 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h + n_d*2 + n_l*4 + n_p*3 + n_a*8)
#endif

#elif defined ZB_IAR && defined ZB8051

/* IAR for 8051 passes 1-byte arguments as 2-bytes to vararg functions. Keil uses
 * 3-bytes pointers while IAR - 2-bytes pointers */
#ifndef ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*2 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h*2 + n_d*2 + n_l*4 + n_p*2 + n_a*8)
#endif

#else  /* IAR & GCC - 32-bit */
/* IAR for Cortex passes 1-byte abd 2-bytes arguments as 4-bytes to vararg functions.
 * Pointers are 4-bytes. */

#ifndef ZB_BINARY_TRACE
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) __FILE__,__LINE__, (n_h*4 + n_d*4 + n_l*4 + n_p*4 + n_a*8)
#else
#define TRACE_ARG_SIZE(n_h, n_d, n_l, n_p, n_a) ZB_TRACE_FILE_ID,__LINE__, (n_h*4 + n_d*4 + n_l*4 + n_p*4 + n_a*8)
#endif

#endif

#define FMT__0                                          TRACE_ARG_SIZE(0,0,0,0,0)
#define FMT__A                                          TRACE_ARG_SIZE(0,0,0,0,1)
#define FMT__A_A                                        TRACE_ARG_SIZE(0,0,0,0,2)
#define FMT__A_D_A_P                                    TRACE_ARG_SIZE(0,1,0,1,2)
#define FMT__A_D_D_P_H                                  TRACE_ARG_SIZE(1,2,0,1,1)
#define FMT__A_D_H                                      TRACE_ARG_SIZE(1,1,0,0,1)
#define FMT__A_D_H_D                                    TRACE_ARG_SIZE(1,2,0,0,1)
#define FMT__C                                          TRACE_ARG_SIZE(1,0,0,0,0)
#define FMT__D                                          TRACE_ARG_SIZE(0,1,0,0,0)
#define FMT__D_A                                        TRACE_ARG_SIZE(0,1,0,0,1)
#define FMT__D_A_D_D_D_D_D_D_D_D                        TRACE_ARG_SIZE(0,9,0,0,1)
#define FMT__D_A_D_P_H_H_H                              TRACE_ARG_SIZE(3,2,0,1,1)
#define FMT__D_A_P                                      TRACE_ARG_SIZE(0,1,0,1,1)
#define FMT__A_P                                        TRACE_ARG_SIZE(0,0,0,1,1)
#define FMT__D_C                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__D_D                                        TRACE_ARG_SIZE(0,2,0,0,0)
#define FMT__D_D_A_A                                    TRACE_ARG_SIZE(0,2,0,0,2)
#define FMT__D_D_A_D                                    TRACE_ARG_SIZE(0,3,0,0,1)
#define FMT__D_D_A_D_D_D_D                              TRACE_ARG_SIZE(0,6,0,0,1)
#define FMT__D_D_D                                      TRACE_ARG_SIZE(0,3,0,0,0)
#define FMT__D_D_D_C                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__D_D_D_D                                    TRACE_ARG_SIZE(0,4,0,0,0)
#define FMT__D_D_D_D_D_D_D_D_D_D_D_D_D_D_D_D_D          TRACE_ARG_SIZE(0,17,0,0,0)
#define FMT__D_D_D_P                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__D_D_P                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__D_H_D_P                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__D_D_P_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__D_D_P_P_P                                  TRACE_ARG_SIZE(0,2,0,3,0)
#define FMT__D_H                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__D_D_H                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__D_H_H                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__D_H_H_H_H_H_H_D_D_D_D                      TRACE_ARG_SIZE(6,5,0,0,0)
#define FMT__D_H_P                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__D_P                                        TRACE_ARG_SIZE(0,1,0,1,0)
#define FMT__D_P_D                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__D_P_H_H_D_H_H                              TRACE_ARG_SIZE(4,2,0,1,0)
#define FMT__D_P_P                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__D_P_P_D_D_H_H                              TRACE_ARG_SIZE(2,3,0,2,0)
#define FMT__D_P_P_H                                    TRACE_ARG_SIZE(1,1,0,2,0)
#define FMT__H                                          TRACE_ARG_SIZE(1,0,0,0,0)
#define FMT__H_A                                        TRACE_ARG_SIZE(1,0,0,0,1)
#define FMT__H_A_A                                      TRACE_ARG_SIZE(1,0,0,0,2)
#define FMT__D_A_A_H                                    TRACE_ARG_SIZE(1,1,0,0,2)
#define FMT__H_A_H_H_H_H_H_H_H_H                        TRACE_ARG_SIZE(9,0,0,0,1)
#define FMT__H_C_D_C                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_D                                        TRACE_ARG_SIZE(1,1,0,0,0)
#define FMT__H_D_A_H_D                                  TRACE_ARG_SIZE(2,2,0,0,1)
#define FMT__H_D_A_H_H_H_H                              TRACE_ARG_SIZE(5,1,0,0,1)
#define FMT__H_D_D                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__H_D_D_D_H_H_D                              TRACE_ARG_SIZE(3,4,0,0,0)
#define FMT__H_H                                        TRACE_ARG_SIZE(2,0,0,0,0)
#define FMT__H_H_D                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__H_H_H                                      TRACE_ARG_SIZE(3,0,0,0,0)
#define FMT__H_H_H_H                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__H_H_P                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__H_H_P_D                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_H_D_P                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__H_P                                        TRACE_ARG_SIZE(1,0,0,1,0)
#define FMT__L                                          TRACE_ARG_SIZE(0,0,1,0,0)
#define FMT__L_L                                        TRACE_ARG_SIZE(0,0,2,0,0)
#define FMT__H_L                                        TRACE_ARG_SIZE(1,0,1,0,0)
#define FMT__L_D_D_D                                    TRACE_ARG_SIZE(0,3,1,0,0)
#define FMT__L_D_D                                      TRACE_ARG_SIZE(0,2,1,0,0)
#define FMT__L_D                                        TRACE_ARG_SIZE(0,1,1,0,0)
#define FMT__D_L_L                                      TRACE_ARG_SIZE(0,1,2,0,0)
#define FMT__D_L_L_L                                    TRACE_ARG_SIZE(0,1,3,0,0)
#define FMT__P_L_P_P_P                                  TRACE_ARG_SIZE(0,0,1,4,0)
#define FMT__P                                          TRACE_ARG_SIZE(0,0,0,1,0)
#define FMT__P_D                                        TRACE_ARG_SIZE(0,1,0,1,0)
#define FMT__P_D_D                                      TRACE_ARG_SIZE(0,2,0,1,0)
#define FMT__P_D_D_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__P_D_D_D_D_D                                TRACE_ARG_SIZE(0,5,0,1,0)
#define FMT__P_D_D_D_D_D_D                              TRACE_ARG_SIZE(0,6,0,1,0)
#define FMT__P_D_D_D_D_D_D_D                            TRACE_ARG_SIZE(0,7,0,1,0)
#define FMT__P_D_D_D_H_D                                TRACE_ARG_SIZE(1,4,0,1,0)
#define FMT__P_D_H                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__P_H_D_H                                    TRACE_ARG_SIZE(2,1,0,1,0)
#define FMT__P_H_D_D                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__P_D_P                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__P_D_P_D                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__P_H                                        TRACE_ARG_SIZE(1,0,0,1,0)
#define FMT__P_H_D                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__P_H_H                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__P_H_H_L                                    TRACE_ARG_SIZE(2,0,1,1,0)
#define FMT__P_H_L                                      TRACE_ARG_SIZE(1,0,1,1,0)
#define FMT__P_H_P_H_L                                  TRACE_ARG_SIZE(2,0,1,2,0)
#define FMT__P_H_P_P                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__P_H_P_P_P                                  TRACE_ARG_SIZE(1,0,0,4,0)
#define FMT__P_P                                        TRACE_ARG_SIZE(0,0,0,2,0)
#define FMT__P_P_D                                      TRACE_ARG_SIZE(0,1,0,2,0)
#define FMT__P_P_D_D_H                                  TRACE_ARG_SIZE(1,2,0,2,0)
#define FMT__P_P_D_H_H                                  TRACE_ARG_SIZE(2,1,0,2,0)
#define FMT__P_P_H                                      TRACE_ARG_SIZE(1,0,0,2,0)
#define FMT__P_P_P                                      TRACE_ARG_SIZE(0,0,0,3,0)
#define FMT__P_P_P_D                                    TRACE_ARG_SIZE(0,1,0,3,0)
#define FMT__H_H_H_D_D_H_A_H_A                          TRACE_ARG_SIZE(5,2,0,0,2)
#define FMT__H_H_P_P_P                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__H_P_P_P_P                                  TRACE_ARG_SIZE(1,0,0,4,0)
#define FMT__H_P_P_P_P_P                                TRACE_ARG_SIZE(1,0,0,5,0)
#define FMT__D_H_D_P_D                                  TRACE_ARG_SIZE(1,3,0,1,0)
#define FMT__D_D_D_D_D                                  TRACE_ARG_SIZE(0,5,0,0,0)
#define FMT__H_D_D_D_D                                  TRACE_ARG_SIZE(1,4,0,0,0)
#define FMT__D_D_D_D_H                                  TRACE_ARG_SIZE(1,4,0,0,0)
#define FMT__D_H_H_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_P_D_D                                    TRACE_ARG_SIZE(0,3,0,1,0)
#define FMT__H_H_H_D                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__H_D_H_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__P_H_H_H_H_H_H_H                            TRACE_ARG_SIZE(7,0,0,1,0)
#define FMT__P_H_H_H_H_H_H                              TRACE_ARG_SIZE(6,0,0,1,0)
#define FMT__D_D_H_D_H                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_D_D_H_H_H_H                              TRACE_ARG_SIZE(5,2,0,0,0)
#define FMT__H_H_A_A                                    TRACE_ARG_SIZE(2,0,0,0,2)
#define FMT__P_H_P_P_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_H_P_H                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__P_P_H_P                                    TRACE_ARG_SIZE(1,0,0,3,0)
#define FMT__A_D_D                                      TRACE_ARG_SIZE(0,2,0,0,1)
#define FMT__P_H_H_H                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__P_H_P                                      TRACE_ARG_SIZE(1,0,0,2,0)
#define FMT__P_P_H_H                                    TRACE_ARG_SIZE(2,0,0,2,0)
#define FMT__D_P_H_H_D_D                                TRACE_ARG_SIZE(2,3,0,1,0)
#define FMT__A_H                                        TRACE_ARG_SIZE(1,0,0,0,1)
#define FMT__P_H_D_L                                    TRACE_ARG_SIZE(1,1,1,1,0)
#define FMT__H_H_H_P                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__A_D_P_H_H_H                                TRACE_ARG_SIZE(3,1,0,1,1)
#define FMT__H_P_H_P_H_H                                TRACE_ARG_SIZE(4,0,0,2,0)
#define FMT__H_P_H_P_H_H                                TRACE_ARG_SIZE(4,0,0,2,0)
#define FMT__H_P_H_H_H_H                                TRACE_ARG_SIZE(5,0,0,1,0)
#define FMT__H_D_H_H_H_H_H_H                            TRACE_ARG_SIZE(7,1,0,0,0)
#define FMT__H_D_D_H_H_H                                TRACE_ARG_SIZE(4,2,0,0,0)
#define FMT__D_D_H_H                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__H_H_D_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__D_H_H_H_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__H_H_H_D_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__H_D_H                                      TRACE_ARG_SIZE(2,1,0,0,0)
#define FMT__H_D_H_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_H_D_H_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__H_P_H_P_H                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__H_P_H_P_H_H                                TRACE_ARG_SIZE(4,0,0,2,0)
#define FMT__H_P_H_P_H_H_P                              TRACE_ARG_SIZE(4,0,0,3,0)
#define FMT__H_P_H_H_H                                  TRACE_ARG_SIZE(4,0,0,1,0)
#define FMT__D_H_D_H                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_H_H_H                                    TRACE_ARG_SIZE(3,1,0,0,0)
#define FMT__D_P_H_P                                    TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__H_H_D_H_P                                  TRACE_ARG_SIZE(3,1,0,1,0)
#define FMT__H_H_H_D_H_P                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__A_H_H                                      TRACE_ARG_SIZE(2,0,0,0,1)
#define FMT__P_H_H_H_H                                  TRACE_ARG_SIZE(4,0,0,1,0)
#define FMT__H_D_P_H_H_H_H_H                            TRACE_ARG_SIZE(6,1,0,1,0)
#define FMT__P_H_H_H_L                                  TRACE_ARG_SIZE(3,0,1,1,0)
#define FMT__H_H_H_H_H_H_H_H                            TRACE_ARG_SIZE(8,0,0,0,0)
#define FMT__H_H_H_H_H_H_H                              TRACE_ARG_SIZE(7,0,0,0,0)
#define FMT__H_H_H_H_H_H                                TRACE_ARG_SIZE(6,0,0,0,0)
#define FMT__H_H_H_H_H                                  TRACE_ARG_SIZE(5,0,0,0,0)
#define FMT__H_D_H_H_H                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__D_D_D_D_D_D                                TRACE_ARG_SIZE(0,6,0,0,0)
#define FMT__P_H_H_H_H_D                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__H_D_D_H_D_H                                TRACE_ARG_SIZE(3,3,0,0,0)
#define FMT__H_P_H                                      TRACE_ARG_SIZE(2,0,0,1,0)
#define FMT__H_H_D_D                                    TRACE_ARG_SIZE(2,2,0,0,0)
#define FMT__D_P_H_H_H_H                                TRACE_ARG_SIZE(4,1,0,1,0)
#define FMT__H_H_D_H_H_H_H_H_H_H_H                      TRACE_ARG_SIZE(10,1,0,0,0)
#define FMT__P_H_H_H_A                                  TRACE_ARG_SIZE(3,0,0,1,1)
#define FMT__H_D_D_H_H                                  TRACE_ARG_SIZE(3,2,0,0,0)
#define FMT__H_D_H_H_H_H                                TRACE_ARG_SIZE(5,1,0,0,0)
#define FMT__H_A_H_H_H                                  TRACE_ARG_SIZE(4,0,0,0,1)
#define FMT__H_H                                        TRACE_ARG_SIZE(2,0,0,0,0)
#define FMT__H_A_A_H_H                                  TRACE_ARG_SIZE(3,0,0,0,2)
#define FMT__H_D_D_H_D                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_D_H_D_H_H                                TRACE_ARG_SIZE(4,2,0,0,0)
#define FMT__H_A_H                                      TRACE_ARG_SIZE(2,0,0,0,1)
#define FMT__L_L_D_D                                    TRACE_ARG_SIZE(0,2,2,0,0)
#define FMT__H_H_H_H_D                                  TRACE_ARG_SIZE(4,1,0,0,0)
#define FMT__P_P_P_H_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_P_P_D_P                                  TRACE_ARG_SIZE(0,1,0,4,0)
#define FMT__H_P_P_P_D_P_P                              TRACE_ARG_SIZE(1,1,0,5,0)
#define FMT__P_P_D_P_D                                  TRACE_ARG_SIZE(0,2,0,3,0)
#define FMT__H_P_H_H                                    TRACE_ARG_SIZE(3,0,0,1,0)
#define FMT__H_H_H_H_D_H                                TRACE_ARG_SIZE(5,1,0,0,0)
#define FMT__D_H_D                                      TRACE_ARG_SIZE(1,2,0,0,0)
#define FMT__D_H_D_D                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__D_D_D_H                                    TRACE_ARG_SIZE(1,3,0,0,0)
#define FMT__P_D_D_D_D                                  TRACE_ARG_SIZE(0,4,0,1,0)
#define FMT__H_P_D                                      TRACE_ARG_SIZE(1,1,0,1,0)
#define FMT__D_D_D_H_H                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__D_A_H_H                                    TRACE_ARG_SIZE(2,1,0,0,1)
#define FMT__P_P_P_P                                    TRACE_ARG_SIZE(0,0,0,4,0)
#define FMT__P_D_D_H                                    TRACE_ARG_SIZE(1,2,0,1,0)
#define FMT__P_P_H_H_H                                  TRACE_ARG_SIZE(3,0,0,2,0)
#define FMT__H_H_L_H                                    TRACE_ARG_SIZE(3,0,1,0,0)
#define FMT__H_C_H_C                                    TRACE_ARG_SIZE(4,0,0,0,0)
#define FMT__H_D_H_D_D                                  TRACE_ARG_SIZE(2,3,0,0,0)
#define FMT__H_L_A                                      TRACE_ARG_SIZE(1,0,1,0,1)
#define FMT__A_A_A_A                                    TRACE_ARG_SIZE(0,0,0,0,4)
#define FMT__AA                                         TRACE_ARG_SIZE(0,0,0,0,2)
#define FMT__H_L_L_H_A                                  TRACE_ARG_SIZE(2,0,2,0,1)
#define FMT__L_H_H                                      TRACE_ARG_SIZE(2,0,1,0,0)
#define FMT__L_H                                        TRACE_ARG_SIZE(1,0,1,0,0)
#define FMT__H_L_H_H_P                                  TRACE_ARG_SIZE(3,0,1,4,0)
#define FMT__H_H_L_H_P                                  TRACE_ARG_SIZE(3,0,1,4,0)
#define FMT__D_D_L                                      TRACE_ARG_SIZE(0,2,1,0,0)
#define FMT__D_L_L_H                                    TRACE_ARG_SIZE(1,1,2,0,0)
#define FMT__H_D_D_H_P                                  TRACE_ARG_SIZE(2,2,0,1,0)
#define FMT__P_A                                        TRACE_ARG_SIZE(0,0,0,1,1)
#define FMT__P_P_D_D                                    TRACE_ARG_SIZE(0,2,0,2,0)
#define FMT__P_P_H_P_H                                  TRACE_ARG_SIZE(2,0,0,3,0)
#define FMT__P_P_H_P_D_P                                TRACE_ARG_SIZE(1,2,0,4,0)
#define FMT__P_P_D_D_P                                  TRACE_ARG_SIZE(0,2,0,3,0)

/** @} */ /* Trace data format definitions. */

/** @cond internals_doc */

/**
 \par per-subsystem trace definitions
*/
#define TRACE_COMMON1 TRACE_SUBSYSTEM_COMMON, 1
#define TRACE_COMMON2 TRACE_SUBSYSTEM_COMMON, 2
#define TRACE_COMMON3 TRACE_SUBSYSTEM_COMMON, 3

#define TRACE_OSIF1 TRACE_SUBSYSTEM_OSIF, 1
#define TRACE_OSIF2 TRACE_SUBSYSTEM_OSIF, 2
#define TRACE_OSIF3 TRACE_SUBSYSTEM_OSIF, 3

#define TRACE_MAC1 TRACE_SUBSYSTEM_MAC, 1
#define TRACE_MAC2 TRACE_SUBSYSTEM_MAC, 2
#define TRACE_MAC3 TRACE_SUBSYSTEM_MAC, 3

#define TRACE_NWK1 TRACE_SUBSYSTEM_NWK, 1
#define TRACE_NWK2 TRACE_SUBSYSTEM_NWK, 2
#define TRACE_NWK3 TRACE_SUBSYSTEM_NWK, 3

#define TRACE_APS1 TRACE_SUBSYSTEM_APS, 1
#define TRACE_APS2 TRACE_SUBSYSTEM_APS, 2
#define TRACE_APS3 TRACE_SUBSYSTEM_APS, 3

#define TRACE_AF1 TRACE_SUBSYSTEM_AF, 1
#define TRACE_AF2 TRACE_SUBSYSTEM_AF, 2
#define TRACE_AF3 TRACE_SUBSYSTEM_AF, 3

#define TRACE_ZDO1 TRACE_SUBSYSTEM_ZDO, 1
#define TRACE_ZDO2 TRACE_SUBSYSTEM_ZDO, 2
#define TRACE_ZDO3 TRACE_SUBSYSTEM_ZDO, 3

#define TRACE_SECUR1 TRACE_SUBSYSTEM_SECUR, 1
#define TRACE_SECUR2 TRACE_SUBSYSTEM_SECUR, 2
#define TRACE_SECUR3 TRACE_SUBSYSTEM_SECUR, 3

#define TRACE_ZCL1 TRACE_SUBSYSTEM_ZCL, 1
#define TRACE_ZCL2 TRACE_SUBSYSTEM_ZCL, 2
#define TRACE_ZCL3 TRACE_SUBSYSTEM_ZCL, 3

#define TRACE_ZLL1 TRACE_SUBSYSTEM_ZLL, 1
#define TRACE_ZLL2 TRACE_SUBSYSTEM_ZLL, 2
#define TRACE_ZLL3 TRACE_SUBSYSTEM_ZLL, 3

#define TRACE_ZGP1 TRACE_SUBSYSTEM_ZGP, 1
#define TRACE_ZGP2 TRACE_SUBSYSTEM_ZGP, 2
#define TRACE_ZGP3 TRACE_SUBSYSTEM_ZGP, 3

#define TRACE_SPI1 TRACE_SUBSYSTEM_SPI, 1
#define TRACE_SPI2 TRACE_SUBSYSTEM_SPI, 2
#define TRACE_SPI3 TRACE_SUBSYSTEM_SPI, 3

#define TRACE_MACLL1 TRACE_SUBSYSTEM_MACLL, 1
#define TRACE_MACLL2 TRACE_SUBSYSTEM_MACLL, 2
#define TRACE_MACLL3 TRACE_SUBSYSTEM_MACLL, 3

#define TRACE_APP1 TRACE_SUBSYSTEM_APP, 1
#define TRACE_APP2 TRACE_SUBSYSTEM_APP, 2
#define TRACE_APP3 TRACE_SUBSYSTEM_APP, 3

#define TRACE_SPECIAL1 TRACE_SUBSYSTEM_SPECIAL1, 1
#define TRACE_SPECIAL2 TRACE_SUBSYSTEM_SPECIAL1, 2
#define TRACE_SPECIAL3 TRACE_SUBSYSTEM_SPECIAL1, 3

#define TRACE_USB1 TRACE_SUBSYSTEM_USB, 1
#define TRACE_USB2 TRACE_SUBSYSTEM_USB, 2
#define TRACE_USB3 TRACE_SUBSYSTEM_USB, 3

/** @endcond */ /* internals_doc */


#ifndef ZB_SET_TRACE_LEVEL

/* empty definitions if not implemented */
#define ZB_SET_TRACE_LEVEL(l)
#define ZB_SET_TRACE_MASK(m)
#define ZB_SET_TRACE_OFF()
#define ZB_SET_TRACE_ON()
#endif


/*! @} */

#endif /* ZB_LOGGER_H */
