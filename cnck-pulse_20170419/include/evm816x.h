/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Definitions & Register
 *
 */


/* ------------------------------------------------------------------------ *
 *  Variable types                                                          *
 * ------------------------------------------------------------------------ */
#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define Int32   int
#define Int16   short
#define Int8    char

/* ------------------------------------------------------------------------ *
 *  Software Breakpoint code                                                *
 *      Uses inline assembly command                                        *
 * ------------------------------------------------------------------------ */
#define SW_BREAKPOINT       asm( " .long 0xe1200070" );

/* ------------------------------------------------------------------------ *
 *  Device Control Module                                                   *
 * ------------------------------------------------------------------------ */
#define CTRL_BASE                0x48140000

/* ------------------------------------------------------------------------ *
 *  Function Renaming                                                       *
 * ------------------------------------------------------------------------ */
#define _wait                   EVM816X_wait
#define _waitusec               EVM816X_waitusec
#define _waitmsec               EVM816X_waitmsec

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
/* Board Initialization */
Int16 EVM816X_init( );

/* Wait Functions */
extern void EVM816X_wait    ( Uint32 delay );
extern void EVM816X_waitusec( Uint32 usec );
extern void EVM816X_waitmsec( Uint32 msec );

