/****************************************************************************/
/*  DM8168.cmd                                                              */
/*  Copyright (c) 2012  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a DM8168.             */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
#ifndef DSP_CORE  /* ARM memory map */

    L3OCMC0:      o = 0x40300000  l = 0x00040000  /* 256kB L3 OCMC SRAM */
    L3OCMC1:      o = 0x40400000  l = 0x00040000  /* 256kB L3 OCMC SRAM */
    DSPSHL2RAM:   o = 0x40800000  l = 0x00040000  /* 256kB Shared DSP L2 RAM */
    DSPSHL1PRAM:  o = 0x40E00000  l = 0x00008000  /* 32kB Shared DSP L1 Program RAM */
    DSPSHL1DRAM:  o = 0x40F00000  l = 0x00008000  /* 32kB Shared DSP L1 Data RAM */
    DDR0:         o = 0x80000000  l = 0x40000000  /* 1GB external DDR Bank 0 */
    DDR1:         o = 0xC0000000  l = 0x40000000  /* 1GB external DDR Bank 1 */

#else             /* DSP memory map */

    HDVICP0:      o = 0x00400000  l = 0x00040000  /* 256kB HDVICP0 UMAP1 */
    HDVICP1:      o = 0x00500000  l = 0x00040000  /* 256kB HDVICP1 UMAP1 */
    DSPL2RAM:     o = 0x00800000  l = 0x00040000  /* 256kB DSP L2 RAM */
    DSPL1PRAM:    o = 0x00E00000  l = 0x00008000  /* 32kB DSP L1 Program RAM */
    DSPL1DRAM:    o = 0x00F00000  l = 0x00008000  /* 32kB DSP L1 Data RAM */

#endif
}

SECTIONS
{
#ifndef DSP_CORE   /* ARM memory map */

    .text          >  L3OCMC0
    .stack         >  L3OCMC0
    .bss           >  L3OCMC0
    .cio           >  L3OCMC0
    .const         >  L3OCMC0
    .data          >  L3OCMC0
    .switch        >  L3OCMC0
    .sysmem        >  L3OCMC0
    .far           >  L3OCMC0
    .args          >  L3OCMC0
    .ppinfo        >  L3OCMC0
    .ppdata        >  L3OCMC0
  
    /* TI-ABI or COFF sections */
    .pinit         >  L3OCMC0
    .cinit         >  L3OCMC0
  
    /* EABI sections */
    .binit         >  L3OCMC0
    .init_array    >  L3OCMC0
    .neardata      >  L3OCMC0
    .fardata       >  L3OCMC0
    .rodata        >  L3OCMC0
    .c6xabi.exidx  >  L3OCMC0
    .c6xabi.extab  >  L3OCMC0

#else              /* DSP memory map */

    .text          >  DSPL2RAM
    .stack         >  DSPL2RAM
    .bss           >  DSPL2RAM
    .cio           >  DSPL2RAM
    .const         >  DSPL2RAM
    .data          >  DSPL2RAM
    .switch        >  DSPL2RAM
    .sysmem        >  DSPL2RAM
    .far           >  DSPL2RAM
    .args          >  DSPL2RAM
    .ppinfo        >  DSPL2RAM
    .ppdata        >  DSPL2RAM
  
    /* TI-ABI or COFF sections */
    .pinit         >  DSPL2RAM
    .cinit         >  DSPL2RAM
  
    /* EABI sections */
    .binit         >  DSPL2RAM
    .init_array    >  DSPL2RAM
    .neardata      >  DSPL2RAM
    .fardata       >  DSPL2RAM
    .rodata        >  DSPL2RAM
    .c6xabi.exidx  >  DSPL2RAM
    .c6xabi.extab  >  DSPL2RAM

#endif
}
