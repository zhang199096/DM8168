/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.targets.elf package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"D:\work_space_1013_light\kdsppulse\Debug\configPkg\package\cfg\Dsp_pe674.oe674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\gates\lib\debug\ti.sdo.ipc.gates.ae674"
-l"C:\ti\syslink_2_20_00_14\packages\ti\syslink\ipc\rtos\lib\debug\ti.syslink.ipc.rtos.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\heaps\lib\debug\ti.sdo.ipc.heaps.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\nsremote\lib\debug\ti.sdo.ipc.nsremote.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\transports\lib\debug\ti.sdo.ipc.transports.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\notifyDrivers\lib\debug\ti.sdo.ipc.notifyDrivers.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\family\ti81xx\lib\debug\ti.sdo.ipc.family.ti81xx.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\lib\debug\ti.sdo.ipc.ae674"
-l"C:\ti\uia_1_01_01_14\packages\ti\uia\services\lib\release\ti.uia.services.ae674"
-l"C:\ti\uia_1_01_01_14\packages\ti\uia\runtime\lib\release\ti.uia.runtime.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\utils\lib\debug\ti.sysbios.utils.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\family\c64p\ti81xx\lib\debug\ti.sysbios.family.c64p.ti81xx.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\family\c62\lib\debug\ti.sysbios.family.c62.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\xdcruntime\lib\debug\ti.sysbios.xdcruntime.ae674"
-l"C:\ti\uia_1_01_01_14\packages\ti\uia\events\lib\release\ti.uia.events.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\timers\dmtimer\lib\debug\ti.sysbios.timers.dmtimer.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\heaps\lib\debug\ti.sysbios.heaps.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\syncs\lib\debug\ti.sysbios.syncs.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\interfaces\lib\debug\ti.sdo.ipc.interfaces.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\utils\lib\debug\ti.sdo.utils.ae674"
-l"C:\ti\ipc_1_24_03_32\packages\ti\sdo\ipc\family\lib\debug\ti.sdo.ipc.family.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\gates\lib\debug\ti.sysbios.gates.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\family\c64p\lib\debug\ti.sysbios.family.c64p.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\knl\lib\debug\ti.sysbios.knl.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\hal\lib\debug\ti.sysbios.hal.ae674"
-l"C:\ti\bios_6_33_05_46\packages\ti\sysbios\lib\debug\ti.sysbios.ae674"
-l"C:\ti\xdctools_3_23_04_60\packages\ti\targets\rts6000\lib\ti.targets.rts6000.ae674"
-l"C:\ti\xdctools_3_23_04_60\packages\ti\targets\rts6000\lib\boot.ae674"

--retain="*(xdc.meta)"


--args 0x64
-heap  0x0
-stack 0x20000

MEMORY
{
    OCMC_0 (RWX) : org = 0x40300000, len = 0x40000
    OCMC_1 (RWX) : org = 0x40400000, len = 0x40000
    SR_0 (RWX) : org = 0x9f700000, len = 0x200000
    SR_1 (RWX) : org = 0x98000000, len = 0x1000000
    LOGGERSM (RWX) : org = 0x9e400000, len = 0x200000
    DSP_PROG (RWX) : org = 0x99000000, len = 0x1000000
}

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from ti.targets.rts6000 (null): */

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from ti.sysbios (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios.family.c64p (null): */

/* Content from ti.sysbios.gates (null): */

/* Content from xdc.services.getset (null): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.sdo.ipc.family (null): */

/* Content from ti.sdo.utils (null): */

/* Content from ti.sdo.ipc.interfaces (null): */

/* Content from ti.sysbios.syncs (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from ti.sysbios.timers.dmtimer (null): */

/* Content from xdc.rta (null): */

/* Content from ti.uia.events (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.sysbios.family.c62 (null): */

/* Content from ti.sysbios.family.c64p.ti81xx (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from ti.catalog.c6000 (null): */

/* Content from ti.catalog (null): */

/* Content from ti.catalog.peripherals.hdvicp2 (null): */

/* Content from xdc.platform (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.catalog.arm.peripherals.timers (null): */

/* Content from ti.catalog.arm.cortexm3 (null): */

/* Content from ti.catalog.arm.cortexa8 (null): */

/* Content from ti.platforms.evmTI816X (null): */

/* Content from ti.uia.runtime (null): */

/* Content from ti.uia.services (null): */

/* Content from ti.sdo.ipc (ti/sdo/ipc/linkcmd.xdt): */

SECTIONS
{
    ti.sdo.ipc.SharedRegion_0:  { . += 0x200000;} run > 0x9f700000, type = NOLOAD
    ti.sdo.ipc.SharedRegion_1:  { . += 0x1000000;} run > 0x98000000, type = NOLOAD

    ti_sdo_ipc_init: load > DSP_PROG, type = NOINIT 
}


/* Content from ti.sdo.ipc.family.ti81xx (null): */

/* Content from ti.sdo.ipc.notifyDrivers (null): */

/* Content from ti.sdo.ipc.transports (null): */

/* Content from ti.sdo.ipc.nsremote (null): */

/* Content from ti.sdo.ipc.heaps (null): */

/* Content from ti.syslink.ipc.rtos (ti/syslink/ipc/rtos/linkcmd.xdt): */


        -eti_sysbios_family_c64p_Hwi0

/* Add the flags needed for SysLink ELF build. */
    --dynamic
    --retain=_Ipc_ResetVector

/* Content from ti.sdo.ipc.gates (null): */

/* Content from configPkg (null): */


/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;
TSK_idle = ti_sysbios_knl_Task_Object__table__V + 0;

SECTIONS
{
    .text: load >> DSP_PROG
    .ti.decompress: load > DSP_PROG
    .stack: load > DSP_PROG
    GROUP: load > DSP_PROG
    {
        .bss:
        .neardata:
        .rodata:
    }
    .cinit: load > DSP_PROG
    .pinit: load >> DSP_PROG
    .init_array: load > DSP_PROG
    .const: load >> DSP_PROG
    .data: load >> DSP_PROG
    .fardata: load >> DSP_PROG
    .switch: load >> DSP_PROG
    .sysmem: load > DSP_PROG
    .far: load >> DSP_PROG
    .args: load > DSP_PROG align = 0x4, fill = 0 {_argsize = 0x64; }
    .cio: load >> DSP_PROG
    .ti.handler_table: load > DSP_PROG
    .loggerSM: load >> LOGGERSM, type = NOLOAD
    .vecs: load >> DSP_PROG
    xdc.meta: load >> DSP_PROG, type = COPY

}
