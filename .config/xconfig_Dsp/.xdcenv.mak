#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/mathlib_c674x_3_1_0_0/packages;C:/ti/xdais_7_22_00_03/packages;C:/ti/xdais_7_22_00_03/examples;C:/ti/ccsv5/ccs_base;C:/ti/ipc_1_24_03_32/packages;C:/ti/uia_1_01_01_14/packages;D:/interactive2/work_program/clone_control/others/cnck-pulse;C:/ti/bios_6_33_05_46/packages;C:/ti/syslink_2_20_00_14/packages;D:/work_space_1013_light/kdsppulse/.config
override XDCROOT = C:/ti/xdctools_3_23_04_60
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/mathlib_c674x_3_1_0_0/packages;C:/ti/xdais_7_22_00_03/packages;C:/ti/xdais_7_22_00_03/examples;C:/ti/ccsv5/ccs_base;C:/ti/ipc_1_24_03_32/packages;C:/ti/uia_1_01_01_14/packages;D:/interactive2/work_program/clone_control/others/cnck-pulse;C:/ti/bios_6_33_05_46/packages;C:/ti/syslink_2_20_00_14/packages;D:/work_space_1013_light/kdsppulse/.config;C:/ti/xdctools_3_23_04_60/packages;..
HOSTOS = Windows
endif
