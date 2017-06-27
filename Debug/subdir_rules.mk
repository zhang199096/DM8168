################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../Dsp.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_23_04_60/xs" --xdcpath="C:/ti/mathlib_c674x_3_1_0_0/packages;C:/ti/xdais_7_22_00_03/packages;C:/ti/xdais_7_22_00_03/examples;C:/ti/ccsv5/ccs_base;C:/ti/ipc_1_24_03_32/packages;C:/ti/uia_1_01_01_14/packages;D:/interactive2/work_program/clone_control/others/cnck-pulse;C:/ti/bios_6_33_05_46/packages;C:/ti/syslink_2_20_00_14/packages;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C674 -p ti.platforms.evmTI816X -r debug -b "D:/work_space_1013_light/kdsppulse/config.bld" -c "C:/ti/ccsv5/tools/compiler/c6000_7.4.4" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: D:/work_space_1013_light/kdsppulse/config.bld
configPkg/compiler.opt: | configPkg/linker.cmd D:/work_space_1013_light/kdsppulse/config.bld
configPkg/: | configPkg/linker.cmd D:/work_space_1013_light/kdsppulse/config.bld


