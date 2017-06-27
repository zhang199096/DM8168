/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

#include "evm816x.h"

/* ------------------------------------------------------------------------ *
 *  Flash Memory Property Definitions                                       *
 * ------------------------------------------------------------------------ */
#define FLASH_BASE                  ( 0x08000000)
#define FLASH_END                   ( 0x0C000000 )
#define FLASH_SECTORSIZE            (Uint32)0x20000     // In bytes
#define FLASH_SECTORS               (Uint32)0x200
#define FLASH_SIZE                  FLASH_SECTORSIZE * FLASH_SECTORS

/* ------------------------------------------------------------------------ *
 *  Flash Memory Base Pointers                                              *
 * ------------------------------------------------------------------------ */
#define FLASH_BASE_PTR32    *( volatile Uint32* )FLASH_BASE
#define FLASH_BASE_PTR16    *( volatile Uint16* )FLASH_BASE
#define FLASH_BASE_PTR8     *( volatile Uint8* ) FLASH_BASE

/* Flash Unlock */
#define FLASH_CTL555        *( volatile Uint16* )( FLASH_BASE + 0xAAA )
#define FLASH_CTL2AA        *( volatile Uint16* )( FLASH_BASE + 0x554 )

/* ------------------------------------------------------------------------ *
 *  Flash Command Codes ( SPANSION )                                        *
 * ------------------------------------------------------------------------ */
#define FLASH_RESET                         0x00F0
#define FLASH_CMD_AA                        0x00AA
#define FLASH_CMD_55                        0x0055
#define FLASH_ID                            0x0090
#define FLASH_PROGRAM                       0x00A0
#define FLASH_WRITE_BUFFER                  0x0025
#define FLASH_PROGRAM_BUFFER                0x0029
#define FLASH_ERASE                         0x0080
#define FLASH_ERASE_CHIP                    0x0010
#define FLASH_ERASE_SECTOR                  0x0030
#define FLASH_ERASE_SUSPEND                 0x00B0
#define FLASH_ERASE_RESUME                  0x0010
#define FLASH_CFI_QUERY                     0x0098

/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
Int16  norflash_init        ( );
Int16  norflash_read        ( Uint32 src, Uint32 dst, Uint32 length );
Int16  norflash_write       ( Uint32 src, Uint32 dst, Uint32 length );
Int16  norflash_erase       ( Uint32 start, Uint32 length );
Uint16 norflash_getId( Uint16* id);
Int16 gpmc_cs1_init( );
Int16 gpmc_cs2_init( );

