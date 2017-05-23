/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Flash Setup
 *
 */

#include "..\\include\\main_dsp.h"

/* Constant table containing end address of each sector */
static Uint32 sector_end[FLASH_SECTORS];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_init( )                                                        *
 *                                                                          *
 *      Setup Flash                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_init( )
{
	Int16 i;
	
	/* Configure GPMC */
	GPMC_SYSCONFIG = 0x0;
    GPMC_IRQENABLE = 0x0;
    GPMC_TIMEOUT_CONTROL = 0x0;
    GPMC_CONFIG = 0x0;

	GPMC_CONFIG7_0 = 0x00;
	GPMC_CONFIG1_0 = 0x00001010;
	GPMC_CONFIG2_0 = 0x00101080;
	GPMC_CONFIG3_0 = 0x00020201;
	GPMC_CONFIG4_0 = 0x0f031003;
	GPMC_CONFIG5_0 = 0x000f1111;
	GPMC_CONFIG6_0 = 0x0f030080;
	GPMC_CONFIG7_0 = 0x00000C48;

	/* Populate sector table */
	for (i = 0; i < FLASH_SECTORS; i++)
	{
    	sector_end[i] = FLASH_BASE +(((i + 1) * FLASH_SECTORSIZE) - 1);
	}
	return 0;
}

Int16 gpmc_cs1_init( )
{
	/* Configure GPMC */
	GPMC_SYSCONFIG = 0x0;
    GPMC_IRQENABLE = 0x0;
    GPMC_TIMEOUT_CONTROL = 0x0;
    GPMC_CONFIG = 0x0;

	GPMC_CONFIG7_1 = 0x00;
	GPMC_CONFIG1_1 = 0x00001010;
	GPMC_CONFIG2_1 = 0x00101080;
	GPMC_CONFIG3_1 = 0x00020201;
	GPMC_CONFIG4_1 = 0x0f031003;
	GPMC_CONFIG5_1 = 0x000f1111;
	GPMC_CONFIG6_1 = 0x0f030080;
	GPMC_CONFIG7_1 = 0x00000F42;

#define STNOR_GPMC_CONFIG1        0x000f1000
#define STNOR_GPMC_CONFIG2        0x1e << 16 | 0x1e << 8 | 1 << 0
#define STNOR_GPMC_CONFIG3        0x3 << 16 | 0x3 << 8 | 2 << 0
#define STNOR_GPMC_CONFIG4        0x18 << 24 | 0x4 << 16 | 0x1c << 8 | 0x4 << 0
#define STNOR_GPMC_CONFIG5        0x14 << 16 | 0x1f << 8 | 0x1f << 0
#define STNOR_GPMC_CONFIG6        0x1e << 24|0X4<<8|3<<6|0X4<<0

	//return 0;
}

Int16 gpmc_cs2_init( )
{
	/* Configure GPMC */
	GPMC_SYSCONFIG = 0x0;
    GPMC_IRQENABLE = 0x0;
    GPMC_TIMEOUT_CONTROL = 0x0;
    GPMC_CONFIG = 0x0;

	GPMC_CONFIG7_2 = 0x00;
	GPMC_CONFIG1_2 = 0x00001010;
	GPMC_CONFIG2_2 = 0x00101080;
	GPMC_CONFIG3_2 = 0x00020201;
	GPMC_CONFIG4_2 = 0x0f031003;
	GPMC_CONFIG5_2 = 0x000f1111;
	GPMC_CONFIG6_2 = 0x0f030080;
	GPMC_CONFIG7_2 = 0x00000C48;

	return 0;
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_erase( start, length )                                         *
 *      Erase Flash containing address ( start ) to ( start + length ).     *
 *      Flash can only erase entire blocks containing the range.            *
 *                                                                          *
 *      start  <- starting address                                          *
 *      length <- length in bytes                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_erase( Uint32 start, Uint32 length )
{
    Int16 i;
    Uint16 *pdata;
    Uint32 sector_base,end;


    end = start + length - 1;                   // Calculate end of range
    start &= 0xFFFFFFFE;                        // Align to 16-bit words

  /* Walk through each sector, erase any sectors within range */
    sector_base = FLASH_BASE;
    for (i = 0; i < FLASH_SECTORS; i++)
    {
        if ( ( ( sector_base >= start ) || ( sector_end[i] >= start ) ) &&
             ( ( sector_base <= end )   || ( sector_end[i] <= end ) ) )
        {
            /* Start sector erase sequence */
            FLASH_CTL555 = FLASH_CMD_AA;
            FLASH_CTL2AA = FLASH_CMD_55 ;
            FLASH_CTL555 = FLASH_ERASE;
            FLASH_CTL555 = FLASH_CMD_AA;
            FLASH_CTL2AA = FLASH_CMD_55 ;

            /* Start erase at sector address */
            pdata = (Uint16 *)sector_base;
            *pdata = FLASH_ERASE_SECTOR;

            /* Wait for erase to complete */
            while (1)
                if (*pdata & 0x80)
                    break;

            /* Put back in read mode */
            *((Uint16 *)FLASH_BASE) = FLASH_RESET;
        }

        /* Advance to next sector */
        sector_base = sector_end[i] + 1;
    }
    *((Uint8 *)FLASH_BASE) = FLASH_RESET;
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_getId( )                                                       *
 *                                                                          *
 *      Get IDs                                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Uint16 norflash_getId( Uint16* id)
{
    Uint32 addr = FLASH_BASE;
    
    volatile Uint16* addr16  = ( Uint16* )addr;
    volatile Uint16* pmfgid  = ( Uint16* )addr;

    FLASH_CTL555 = FLASH_RESET;
    //EVM816X_waitusec(10);

    /* Configure to read manufacturer ID */
    FLASH_CTL555 = FLASH_CMD_AA;
    FLASH_CTL2AA = FLASH_CMD_55;
    FLASH_CTL555 = FLASH_ID;

    /* Insert small delay for device to respond */
    //EVM816X_waitusec(10);

    *id++ = *pmfgid;          // Read MFG_ID
    *id++ = *(pmfgid + 0x01);    // Read DEV_ID1
    *id++ = *(pmfgid + 0x0E);    // Read DEV_ID2
    *id++ = *(pmfgid + 0x0F);    // Read DEV_ID3

    *addr16 = FLASH_RESET;
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_read( src, dst, length )                                         *
 *      Read from Flash address ( src ) to the data at non-Flash address    *
 *      ( dst ) for ( length ) bytes.                                       *
 *                                                                          *
 *      src     <- source address                                           *
 *      dest    <- destination address                                      *
 *      length  <- length in bytes                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_read( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;
    Uint16* psrc16 = ( Uint16* )src;
    Uint16* pdst16 = ( Uint16* )dst;

    /*
     *  Set to Read Mode
     */
    FLASH_BASE_PTR16 = FLASH_RESET;

    /*
     *  Read Data to Buffer
     */
    for ( i = 0 ; i < length ; i ++ )
        *pdst16++ = *psrc16++;

    return 0;
}

Uint32 write_timeout = 0x00010000;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_write( src, dst, length )                                      *
 *      Write to Flash address [dst] the data at a non-Flash address [src]  *
 *      for [length] #bytes.                                                *
 *                                                                          *
 *      src     <- source address                                           *
 *      dest    <- destination address                                      *
 *      length  <- length in bytes                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_write( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;
    Uint16* psrc16;
    Uint16* pdst16;
    Uint32 addr = FLASH_BASE;
    
    volatile Uint16* addr16  = ( Uint16* )addr;
    /*
     *  Align to 8 or 16 bits
     */
    psrc16 = ( Uint16* )src;
    pdst16 = ( Uint16* )dst;
    length &= 0xFFFFFFFE;

     for ( i = 0 ; i < length ; i ++ )
     {
         /* Program one 16-bit word */
         FLASH_CTL555 = FLASH_CMD_AA;
         FLASH_CTL2AA = FLASH_CMD_55;
         FLASH_CTL555 = FLASH_PROGRAM;
         *pdst16 = *psrc16;

         /* Wait for programming to complete */
         // Wait for operation to complete
         while(1)
             if (*pdst16 == *psrc16)
                 break;
         pdst16++;
         psrc16++;
     }
     
    *addr16 = FLASH_RESET;
    return 0;
}

