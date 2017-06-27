################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c6000_7.4.4

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./src/uart.obj" \
"./src/test.obj" \
"./src/struct.obj" \
"./src/spi.obj" \
"./src/sharemem.obj" \
"./src/sci.obj" \
"./src/norflash.obj" \
"./src/main_dsp.obj" \
"./src/int_code.obj" \
"./src/cputimer.obj" \
"./src/cpld.obj" \
"./src/area_control.obj" \
"./src/area_code_rec.obj" \
"./src/RUN_NC.obj" \
"../DM8168.cmd" \
-l"C:\ti\mathlib_c674x_3_1_0_0\packages\ti\mathlib\lib\mathlib.lib" \
-l"C:\ti\ccsv5\tools\compiler\c6000_7.4.4\lib\libc.a" \
-l"rts6740_elf.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: kdsppulse.out

# Tool invocations
kdsppulse.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 --abi=eabi -g --relaxed_ansi --define=dm8168 --display_error_number --diag_warning=225 --diag_wrap=off -z --stack_size=0x800 -m"kdsppulse.map" --heap_size=0x800 -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/lib" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --reread_libs --define=DSP_CORE=1 --warn_sections --display_error_number --diag_wrap=off --xml_link_info="kdsppulse_linkInfo.xml" --rom_model -o "kdsppulse.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED) "kdsppulse.out"
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "src\area_code_rec.pp" "src\cpld.pp" "src\cputimer.pp" "src\int_code.pp" "src\main_dsp.pp" "src\norflash.pp" "src\sci.pp" "src\sharemem.pp" "src\spi.pp" "src\struct.pp" "src\test.pp" "src\uart.pp" 
	-$(RM) "src\RUN_NC.pp" "src\area_control.pp" 
	-$(RM) "src\RUN_NC.obj" "src\area_code_rec.obj" "src\area_control.obj" "src\cpld.obj" "src\cputimer.obj" "src\int_code.obj" "src\main_dsp.obj" "src\norflash.obj" "src\sci.obj" "src\sharemem.obj" "src\spi.obj" "src\struct.obj" "src\test.obj" "src\uart.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

