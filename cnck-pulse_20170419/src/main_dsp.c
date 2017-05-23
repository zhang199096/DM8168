/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "..\include\main_dsp.h"

const double PI = 3.1415926535897932384626433832795;

Error_Block eb;
Hwi_Handle hwipwr;
Hwi_Params hwipwrparams;
Hwi_Handle hwi10;
Hwi_Params hwi10params;
Hwi_Handle hwi11;
Hwi_Params hwi11params;
Hwi_Handle hwi12;
Hwi_Params hwi12params;
Hwi_Handle hwi13;
Hwi_Params hwi13params;

Task_Handle task1,task2;
Task_Params task1params;
Task_Params task2params;

extern Uint16 	MonitorBuffer1Cnt, MonitorBuffer2Cnt, MonitorBuffer3Cnt, MonitorBuffer4Cnt;
extern Uint16 	PositionPointerCnt,PositionPointerCnt_Bak;
extern Uint16 	LensCompensationBufferCnt;
extern Int32		DeltaStepAxis19_Bak;
extern Int32		DeltaStepAxis19_CPS;

/*----------------------------------------------------------------------------
 - Purpose: Writes a single TLB entry into the MMU
 -
 - Inputs: entry number
 - virtual address (as seen from DSP)
 - physical address (as seen from ARM)
 -
 - Notes: Assumes a super section 16 MB size (0x0100_0000)
 ----------------------------------------------------------------------------*/
void mmuWriteStaticTlb(int entry, unsigned long va, unsigned long pa)
{
	const int MMU_BASE = 0x08010000;
	volatile int *mmu_lock = (int *) (MMU_BASE + 0x50);
	volatile int *mmu_ld_tlb = (int *) (MMU_BASE + 0x54);
	volatile int *mmu_cam = (int *) (MMU_BASE + 0x58);
	volatile int *mmu_ram = (int *) (MMU_BASE + 0x5c);

	// configure TLB virtual address , 16 MB super-section, protect, validate
	*mmu_cam = (va & 0xfff00000) | 0x0000000f;

	// load physical address of the page
	*mmu_ram = (pa & 0xfff00000);

	// lock from base value 0
	*mmu_lock = ((entry + 1) << 10) | (entry << 4);

	// load the specified entry in the TLB
	*mmu_ld_tlb = 0x00000001;
}

/*----------------------------------------------------------------------------
 - Purpose: Configures the MMU for all access to external memory
 -
 - Notes: Required for access to the GPMC bus
 - See spprugz8 Figure 1-22 MMU Global Initialization & table 1-50
 ----------------------------------------------------------------------------*/
void configureMmu(void) 
{
	const int MMU_BASE = 0x08010000;
	volatile int *mmu_revision = (int *) (MMU_BASE + 0x00);
	volatile int *mmu_sysconfig = (int *) (MMU_BASE + 0x10);
	volatile int *mmu_sysstatus = (int *) (MMU_BASE + 0x14);
	volatile int *mmu_irqenable = (int *) (MMU_BASE + 0x1c);
	volatile int *mmu_cntl = (int *) (MMU_BASE + 0x44);
	int entry = 0;

	Log_info1("Configuring MMU revision = 0x%08x\n", *mmu_revision);
	//System_flush();

	// execute SW reset
	*mmu_sysconfig = 0x00000002;

	// wait for reset
	while ((*mmu_sysstatus & 0x00000001) != 0)
		;

	// enable power save
	//*mmu_sysconfig = 0x00000001; crashes with this line

	// configure 16 16-MB segments of DDR for straight through
	for (entry = 0; entry < 10; entry++) {
		mmuWriteStaticTlb(entry, (0x96000000 + (entry << 24)),
				(0x96000000 + (entry << 24)));
	}

	// registers
	mmuWriteStaticTlb(entry, 0x40000000, 0x40000000);
	entry++;

	// GPMC
	mmuWriteStaticTlb(entry, 0x11000000, 0x01000000);
	entry++;
	mmuWriteStaticTlb(entry, 0x12000000, 0x02000000);
	entry++;
	mmuWriteStaticTlb(entry, 0x13000000, 0x03000000);
	entry++;
	
	// DDR control (cache control by syslink)
	mmuWriteStaticTlb(entry, 0x4c000000, 0x4c000000);
	entry++;
	mmuWriteStaticTlb(entry, 0x48000000, 0x48000000);
	entry++;
	Log_info0("MMU configured Tlb\n");

	// enable multihit fault and TLB miss
	*mmu_irqenable = 0x00000011;
	Log_info0("MMU configured irqenable\n");

	// enable memory translations (turn it on)
	*mmu_cntl = 0x00000002;
	Log_info0("MMU configured cntl\n");

	// if this prints, at least the DDR mapping is working
	Log_info0("MMU configured\n");
	//stem_flush();
}

//configure intrrupts hardware pin since no set in linux now
void configInterruptsPin(void)
{
	/*
#define GPIO0_IRQ_ENABLE_SET0 (*(volatile unsigned int *)(0x48032034))
#define GPIO0_IRQ_ENABLE_SET1 (*(volatile unsigned int *)(0x48032038))
#define GPIO0_FALLINGDETECT   (*(volatile unsigned int *)(0x4803214c))
			
#define GPIO1_IRQ_ENABLE_SET0 (*(volatile unsigned int *)(0x4804C034))
#define GPIO1_IRQ_ENABLE_SET1 (*(volatile unsigned int *)(0x4804C038))
#define GPIO1_FALLINGDETECT   (*(volatile unsigned int *)(0x4804C14c))
	*/

	volatile int *PINCTRL196 = (int *) (0x48140B0C);//0x48140B0C PINCTRL196 0 0 GPMC_A[16] GP0[21]
	volatile int *PINCTRL197 = (int *) (0x48140B10);//0x48140B10 PINCTRL197 0 1 GPMC_A[15] GP0[22]
	volatile int *PINCTRL198 = (int *) (0x48140B14);//0x48140B14 PINCTRL198 0 1 GPMC_A[14] GP0[23]
	volatile int *PINCTRL199 = (int *) (0x48140B18);//0x48140B18 PINCTRL199 0 0 GPMC_A[13] GP0[24]

	volatile int *PINCTRL205 = (int *) (0x48140B30);//0x48140B30 PINCTRL205 0 0 TIM6_OUT GPMC_A[24] GP0[30]
	volatile int *PINCTRL206 = (int *) (0x48140B34);//0x48140B34 PINCTRL206 0 0 TIM7_OUT GPMC_A[12] GP0[31]

	volatile int *PINCTRL178 = (int *) (0x48140AC4);//0x48140AC4 PINCTRL178 1 0 UART0_DSR GPMC_A[19] GPMC_A[24] GP1[17]
	volatile int *PINCTRL180 = (int *) (0x48140ACC);//0x48140ACC PINCTRL180 1 0 UART0_RIN GPMC_A[17] GPMC_A[22] GP1[19]


	/*
	GPIO0 HEX ADDRESS GPIO1 HEX ADDRESS 			ACRONYM 					REGISTER NAME
	0x48032000 			0x4804C000 				GPIO_REVISION 				GPIO Revision
	0x48032010 			0x4804C010 				GPIO_SYSCONFIG 				System Configuration
	0x48032020 			0x4804C020 				GPIO_EOI 					End of Interrupt
	0x48032024 			0x4804C024 				GPIO_IRQSTATUS_RAW_0 		Status Raw for Interrupt 1
	0x48032028 			0x4804C028 				GPIO_IRQSTATUS_RAW_1 		Status Raw for Interrupt 2
	0x4803202C 			0x4804C02C 				GPIO_IRQSTATUS_0 			Status for Interrupt 1
	0x48032030 			0x4804C030 				GPIO_IRQSTATUS_1 			Status for Interrupt 2
	0x48032034 			0x4804C034 				GPIO_IRQSTATUS_SET_0 		Enable Set for Interrupt 1
	0x48032038 			0x4804C038 				GPIO_IRQSTATUS_SET_1 		Enable Set for Interrupt 2
	0x4803203C 			0x4804C03C 				GPIO_IRQSTATUS_CLR_0 		Enable Clear for Interrupt 1
	0x48032040 			0x4804C040 				GPIO_IRQSTATUS_CLR_1 		Enable Clear for Interrupt 2
	0x48032044 			0x4804C044 				GPIO_IRQWAKEN_0 			Wakeup Enable for Interrupt 1
	0x48032048 			0x4804C048 				GPIO_IRQWAKEN_1 			Wakeup Enable for Interrupt 2
	0x48032114 			0x4804C114 				GPIO_SYSSTATUS 				System Status
	0x48032130 			0x4804C130 				GPIO_CTRL 					Module Control
	0x48032134 			0x4804C134 				GPIO_OE 						Output Enable
	0x48032138 			0x4804C138 				GPIO_DATAIN 					Data Input
	0x4803213C 			0x4804C13C 				GPIO_DATAOUT 				Data Output
	0x48032140 			0x4804C140 				GPIO_LEVELDETECT0 			Detect Low Level
	0x48032144 			0x4804C144 				GPIO_LEVELDETECT1 			Detect High Level
	0x48032148 			0x4804C148 				GPIO_RISINGDETECT 			Detect Rising Edge
	0x4803214C 			0x4804C14C 				GPIO_FALLINGDETECT 			Detect Falling Edge
	0x48032150 			0x4804C150 				GPIO_DEBOUNCENABLE 			Debouncing Enable
	0x48032154 			0x4804C154 				GPIO_DEBOUNCINGTIME 		Debouncing Value
	0x48032190 			0x4804C190 				GPIO_CLEARDATAOUT 			Clear Data Output
	0x48032194 			0x4804C194 				GPIO_SETDATAOUT 			Set Data Output
	*/


	const int GPIO0_BASE = 0x48032000;
	volatile int *GPIO0_revision = (int *) (GPIO0_BASE + 0x00);
	volatile int *GPIO0_sysconfig = (int *) (GPIO0_BASE + 0x10);
	volatile int *GPIO0_EOI = (int *) (GPIO0_BASE + 0x20);
	volatile int *GPIO0_IRQSTATUS_RAW_0 = (int *) (GPIO0_BASE + 0x24);
	volatile int *GPIO0_IRQSTATUS_RAW_1 = (int *) (GPIO0_BASE + 0x28);
	volatile int *GPIO0_IRQSTATUS_0 = (int *) (GPIO0_BASE + 0x2C);
	volatile int *GPIO0_IRQSTATUS_1 = (int *) (GPIO0_BASE + 0x30);
	volatile int *GPIO0_IRQSTATUS_SET_0 = (int *) (GPIO0_BASE + 0x34);
	volatile int *GPIO0_IRQSTATUS_SET_1 = (int *) (GPIO0_BASE + 0x38);
	volatile int *GPIO0_IRQSTATUS_CLR_0 = (int *) (GPIO0_BASE + 0x3C);
	volatile int *GPIO0_IRQSTATUS_CLR_1 = (int *) (GPIO0_BASE + 0x40);
	volatile int *GPIO0_IRQWAKEN_0 = (int *) (GPIO0_BASE + 0x44);
	volatile int *GPIO0_IRQWAKEN_1 = (int *) (GPIO0_BASE + 0x48);
	volatile int *GPIO0_SYSSTATUS = (int *) (GPIO0_BASE + 0x114);
	volatile int *GPIO0_CTRL = (int *) (GPIO0_BASE + 0x130);
	volatile int *GPIO0_OE = (int *) (GPIO0_BASE + 0x134);
	volatile int *GPIO0_DATAIN = (int *) (GPIO0_BASE + 0x138);
	volatile int *GPIO0_DATAOUT = (int *) (GPIO0_BASE + 0x13C);
	volatile int *GPIO0_LEVELDETECT0 = (int *) (GPIO0_BASE + 0x140);
	volatile int *GPIO0_LEVELDETECT1 = (int *) (GPIO0_BASE + 0x144);
	volatile int *GPIO0_RISINGDETECT = (int *) (GPIO0_BASE + 0x148);
	volatile int *GPIO0_FALLINGDETECT = (int *) (GPIO0_BASE + 0x14C);
	volatile int *GPIO0_DEBOUNCENABLE = (int *) (GPIO0_BASE + 0x150);
	volatile int *GPIO0_DEBOUNCINGTIME = (int *) (GPIO0_BASE + 0x154);
	volatile int *GPIO0_CLEARDATAOUT = (int *) (GPIO0_BASE + 0x190);
	volatile int *GPIO0_SETDATAOUT = (int *) (GPIO0_BASE + 0x194);

	const int GPIO1_BASE = 0x4804C000;
	volatile int *GPIO1_revision = (int *) (GPIO1_BASE + 0x00);
	volatile int *GPIO1_sysconfig = (int *) (GPIO1_BASE + 0x10);
	volatile int *GPIO1_EOI = (int *) (GPIO1_BASE + 0x20);
	volatile int *GPIO1_IRQSTATUS_RAW_0 = (int *) (GPIO1_BASE + 0x24);
	volatile int *GPIO1_IRQSTATUS_RAW_1 = (int *) (GPIO1_BASE + 0x28);
	volatile int *GPIO1_IRQSTATUS_0 = (int *) (GPIO1_BASE + 0x2C);
	volatile int *GPIO1_IRQSTATUS_1 = (int *) (GPIO1_BASE + 0x30);
	volatile int *GPIO1_IRQSTATUS_SET_0 = (int *) (GPIO1_BASE + 0x34);
	volatile int *GPIO1_IRQSTATUS_SET_1 = (int *) (GPIO1_BASE + 0x38);
	volatile int *GPIO1_IRQSTATUS_CLR_0 = (int *) (GPIO1_BASE + 0x3C);
	volatile int *GPIO1_IRQSTATUS_CLR_1 = (int *) (GPIO1_BASE + 0x40);
	volatile int *GPIO1_IRQWAKEN_0 = (int *) (GPIO1_BASE + 0x44);
	volatile int *GPIO1_IRQWAKEN_1 = (int *) (GPIO1_BASE + 0x48);
	volatile int *GPIO1_SYSSTATUS = (int *) (GPIO1_BASE + 0x114);
	volatile int *GPIO1_CTRL = (int *) (GPIO1_BASE + 0x130);
	volatile int *GPIO1_OE = (int *) (GPIO1_BASE + 0x134);
	volatile int *GPIO1_DATAIN = (int *) (GPIO1_BASE + 0x138);
	volatile int *GPIO1_DATAOUT = (int *) (GPIO1_BASE + 0x13C);
	volatile int *GPIO1_LEVELDETECT0 = (int *) (GPIO1_BASE + 0x140);
	volatile int *GPIO1_LEVELDETECT1 = (int *) (GPIO1_BASE + 0x144);
	volatile int *GPIO1_RISINGDETECT = (int *) (GPIO1_BASE + 0x148);
	volatile int *GPIO1_FALLINGDETECT = (int *) (GPIO1_BASE + 0x14C);
	volatile int *GPIO1_DEBOUNCENABLE = (int *) (GPIO1_BASE + 0x150);
	volatile int *GPIO1_DEBOUNCINGTIME = (int *) (GPIO1_BASE + 0x154);
	volatile int *GPIO1_CLEARDATAOUT = (int *) (GPIO1_BASE + 0x190);
	volatile int *GPIO1_SETDATAOUT = (int *) (GPIO1_BASE + 0x194);


	*PINCTRL196 = 0x00000002;//bit4_PULLTYPESEL=0,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=2(GP0[21])
	*PINCTRL197 = 0x0000000A;//bit4_PULLTYPESEL=0,bit3_PULLDIS=1,bi0~2_MUXMODE[2:0]=2(GP0[22])
	*PINCTRL198 = 0x0000000A;//bit4_PULLTYPESEL=0,bit3_PULLDIS=1,bi0~2_MUXMODE[2:0]=2(GP0[23])
	*PINCTRL199 = 0x00000002;//bit4_PULLTYPESEL=0,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=2(GP0[24])
	*PINCTRL205 = 0x00000002;//bit4_PULLTYPESEL=0,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=2(GP0[30])
	*PINCTRL206 = 0x00000002;//bit4_PULLTYPESEL=0,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=2(GP0[31])
	*PINCTRL178 = 0x00000013;//bit4_PULLTYPESEL=1,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=3(GP1[17])
	*PINCTRL180 = 0x00000013;//bit4_PULLTYPESEL=1,bit3_PULLDIS=0,bi0~2_MUXMODE[2:0]=3(GP1[19])

	// execute SW reset
	*GPIO0_sysconfig = 0x00000002;
	*GPIO1_sysconfig = 0x00000002;

	// wait for reset
	while ((*GPIO0_SYSSTATUS & 0x00000001) != 0)
		;
	// wait for reset
	while ((*GPIO1_SYSSTATUS & 0x00000001) != 0)
		;
	
	/*
	DSP interrupts as follows
	64 GPIOINT0A GPIO 0
	65 GPIOINT0B GPIO 0
	66 GPIOINT1A GPIO 1
	67 GPIOINT1B GPIO 1
	*/
	//GPIO0 pin31
	int GPIO0_IRQ_BIT_SET0 = 0,GPIO0_IRQ_BIT_SET1=0,GPIO1_IRQ_BIT_SET0=0,GPIO1_IRQ_BIT_SET1=0;
	//GPIO0_IRQ_BIT_SET0 = 0x1<<22;//gpio0 bit 22(0start)
	GPIO0_IRQ_BIT_SET1 = 0x1<<23;//gpio0 bit 23(0start)
	GPIO1_IRQ_BIT_SET0 = 0x1<<17;//gpio1 bit 17(0start)
	//GPIO1_IRQ_BIT_SET1 = 0x1<<17;//gpio1 bit 19(0start)
		
	*GPIO0_IRQSTATUS_SET_0=GPIO0_IRQ_BIT_SET0;  		//int64 pulse_interpolation  gpio0 pin=gpio0_bit23;DSP  INT 64
	*GPIO0_IRQSTATUS_SET_1=GPIO0_IRQ_BIT_SET1;  		//int65 pulse_interpolation  gpio0 pin=gpio0_bit24; DSP INT 65
	//*GPIO0_FALLINGDETECT  =GPIO0_IRQ_BIT_SET0 |GPIO0_IRQ_BIT_SET1;  	//int65 pwr falling edge
	*GPIO0_RISINGDETECT =GPIO0_IRQ_BIT_SET0 |GPIO0_IRQ_BIT_SET1;  	//int65 pwr falling edge

	
	*GPIO1_IRQSTATUS_SET_0=GPIO1_IRQ_BIT_SET0; 		//int66 pulse_interpolation gpio1 pin=gpio1_bit17;DSP  INT 66
	*GPIO1_IRQSTATUS_SET_1=GPIO1_IRQ_BIT_SET1; 		//int67 pulse_interpolation gpio1 pin=gpio1_bit19;DSP  INT 67
	//*GPIO1_FALLINGDETECT  =GPIO1_IRQ_BIT_SET0 |GPIO1_IRQ_BIT_SET1; 		//int66 falling edge
	*GPIO1_RISINGDETECT  =GPIO1_IRQ_BIT_SET0 |GPIO1_IRQ_BIT_SET1; 		//int66 falling edge
	#if 1
	*GPIO0_IRQSTATUS_CLR_0 = 0;
	*GPIO0_IRQSTATUS_CLR_1 = 0;

	*GPIO1_IRQSTATUS_CLR_0 = 0;
	*GPIO1_IRQSTATUS_CLR_1 = 0;
	#endif

	#if 0
	Log_info1("Configuring GPIO0_revision = 0x%x\n", *GPIO0_revision);
	Log_info1("Configuring GPIO0_sysconfig = 0x%x\n", *GPIO0_sysconfig);
	Log_info1("Configuring GPIO0_EOI = 0x%x\n", *GPIO0_EOI);
	Log_info1("Configuring GPIO0_IRQSTATUS_RAW_0 = 0x%x\n", *GPIO0_IRQSTATUS_RAW_0);
	Log_info1("Configuring GPIO0_IRQSTATUS_RAW_1 = 0x%x\n", *GPIO0_IRQSTATUS_RAW_1);
	Log_info1("Configuring GPIO0_IRQSTATUS_0= 0x%x\n", *GPIO0_IRQSTATUS_0);
	Log_info1("Configuring GPIO0_IRQSTATUS_1 = 0x%x\n", *GPIO0_IRQSTATUS_1);
	Log_info1("Configuring GPIO0_IRQSTATUS_SET_0 = 0x%x\n", *GPIO0_IRQSTATUS_SET_0);
	Log_info1("Configuring GPIO0_IRQSTATUS_SET_1 = 0x%x\n", *GPIO0_IRQSTATUS_SET_1);
	Log_info1("Configuring GPIO0_IRQSTATUS_CLR_0 = 0x%x\n", *GPIO0_IRQSTATUS_CLR_0);
	Log_info1("Configuring GPIO0_IRQSTATUS_CLR_1= 0x%x\n", *GPIO0_IRQSTATUS_CLR_1);
	Log_info1("Configuring GPIO0_IRQWAKEN_0 = 0x%x\n", *GPIO0_IRQWAKEN_0);
	Log_info1("Configuring GPIO0_IRQWAKEN_1 = 0x%x\n", *GPIO0_IRQWAKEN_1);
	Log_info1("Configuring GPIO0_SYSSTATUS= 0x%x\n", *GPIO0_SYSSTATUS);
	Log_info1("Configuring GPIO0_CTRL = 0x%x\n", *GPIO0_CTRL);
	Log_info1("Configuring GPIO0_OE = 0x%x\n", *GPIO0_OE);
	Log_info1("Configuring GPIO0_DATAIN = 0x%x\n", *GPIO0_DATAIN);
	Log_info1("Configuring GPIO0_DATAOUT = 0x%x\n", *GPIO0_DATAOUT);
	Log_info1("Configuring GPIO0_LEVELDETECT0 = 0x%x\n", *GPIO0_LEVELDETECT0);
	Log_info1("Configuring GPIO0_LEVELDETECT1 = 0x%x\n", *GPIO0_LEVELDETECT1);
	Log_info1("Configuring GPIO0_RISINGDETECT= 0x%x\n", *GPIO0_RISINGDETECT);
	Log_info1("Configuring GPIO0_FALLINGDETECT = 0x%x\n", *GPIO0_FALLINGDETECT);
	Log_info1("Configuring GPIO0_DEBOUNCENABLE = 0x%x\n", *GPIO0_DEBOUNCENABLE);
	Log_info1("Configuring GPIO0_DEBOUNCINGTIME= 0x%x\n", *GPIO0_DEBOUNCINGTIME);
	Log_info1("Configuring GPIO0_CLEARDATAOUT = 0x%x\n", *GPIO0_CLEARDATAOUT);
	Log_info1("Configuring GPIO0_SETDATAOUT = 0x%x\n", *GPIO0_SETDATAOUT);

	Log_info1("Configuring GPIO1_revision = 0x%x\n", *GPIO1_revision);
	Log_info1("Configuring GPIO1_sysconfig = 0x%x\n", *GPIO1_sysconfig);
	Log_info1("Configuring GPIO1_EOI = 0x%x\n", *GPIO1_EOI);
	Log_info1("Configuring GPIO1_IRQSTATUS_RAW_0 = 0x%x\n", *GPIO1_IRQSTATUS_RAW_0);
	Log_info1("Configuring GPIO1_IRQSTATUS_RAW_1 = 0x%x\n", *GPIO1_IRQSTATUS_RAW_1);
	Log_info1("Configuring GPIO1_IRQSTATUS_0= 0x%x\n", *GPIO1_IRQSTATUS_0);
	Log_info1("Configuring GPIO1_IRQSTATUS_1 = 0x%x\n", *GPIO1_IRQSTATUS_1);
	Log_info1("Configuring GPIO1_IRQSTATUS_SET_0 = 0x%x\n", *GPIO1_IRQSTATUS_SET_0);
	Log_info1("Configuring GPIO1_IRQSTATUS_SET_1 = 0x%x\n", *GPIO1_IRQSTATUS_SET_1);
	Log_info1("Configuring GPIO1_IRQSTATUS_CLR_0 = 0x%x\n", *GPIO1_IRQSTATUS_CLR_0);
	Log_info1("Configuring GPIO1_IRQSTATUS_CLR_1= 0x%x\n", *GPIO1_IRQSTATUS_CLR_1);
	Log_info1("Configuring GPIO1_IRQWAKEN_0 = 0x%x\n", *GPIO1_IRQWAKEN_0);
	Log_info1("Configuring GPIO1_IRQWAKEN_1 = 0x%x\n", *GPIO1_IRQWAKEN_1);
	Log_info1("Configuring GPIO1_SYSSTATUS= 0x%x\n", *GPIO1_SYSSTATUS);
	Log_info1("Configuring GPIO1_CTRL = 0x%x\n", *GPIO1_CTRL);
	Log_info1("Configuring GPIO1_OE = 0x%x\n", *GPIO1_OE);
	Log_info1("Configuring GPIO1_DATAIN = 0x%x\n", *GPIO1_DATAIN);
	Log_info1("Configuring GPIO1_DATAOUT = 0x%x\n", *GPIO1_DATAOUT);
	Log_info1("Configuring GPIO1_LEVELDETECT0 = 0x%x\n", *GPIO1_LEVELDETECT0);
	Log_info1("Configuring GPIO1_LEVELDETECT1 = 0x%x\n", *GPIO1_LEVELDETECT1);
	Log_info1("Configuring GPIO1_RISINGDETECT= 0x%x\n", *GPIO1_RISINGDETECT);
	Log_info1("Configuring GPIO1_FALLINGDETECT = 0x%x\n", *GPIO1_FALLINGDETECT);
	Log_info1("Configuring GPIO1_DEBOUNCENABLE = 0x%x\n", *GPIO1_DEBOUNCENABLE);
	Log_info1("Configuring GPIO1_DEBOUNCINGTIME= 0x%x\n", *GPIO1_DEBOUNCINGTIME);
	Log_info1("Configuring GPIO1_CLEARDATAOUT = 0x%x\n", *GPIO1_CLEARDATAOUT);
	Log_info1("Configuring GPIO1_SETDATAOUT = 0x%x\n", *GPIO1_SETDATAOUT);
	#endif
}

//configure GPIO_Output pin since no set in linux now
void configGPIO_OutputPin(void)
{

	volatile int *PINCTRL189 = (int *) (0x48140AF0);//0x48140AF0 PINCTRL189 0 0 GPMC_A[27] GP1[9] Mod=010
	volatile int *PINCTRL190 = (int *) (0x48140AF4);//0x48140AF4 PINCTRL190 0 1 GPMC_A[22] GP1[10]Mod=010
	volatile int *PINCTRL191 = (int *) (0x48140AF8);//0x48140AF8 PINCTRL191 0 1 GPMC_A[26] GP1[11]Mod=010
	volatile int *PINCTRL192 = (int *) (0x48140AFC);//0x48140AFC PINCTRL192 0 0 GPMC_A[25] GP1[12]Mod=010
	volatile int *PINCTRL193 = (int *) (0x48140B00);//0x48140B00 PINCTRL193 0 1 GP1[13]		Mod=001								
	volatile int *PINCTRL194 = (int *) (0x48140B04);//0x48140B04 PINCTRL194 0 1 GPMC_A[23] GP1[14]Mod=010
	volatile int *PINCTRL195 = (int *) (0x48140B08);//0x48140B08 PINCTRL195 0 1 GPMC_A[24] GP1[15]Mod=010


	/*
	GPIO0 HEX ADDRESS GPIO1 HEX ADDRESS 			ACRONYM 					REGISTER NAME
	0x48032000 			0x4804C000 				GPIO_REVISION 				GPIO Revision
	0x48032010 			0x4804C010 				GPIO_SYSCONFIG 				System Configuration
	0x48032020 			0x4804C020 				GPIO_EOI 					End of Interrupt
	0x48032024 			0x4804C024 				GPIO_IRQSTATUS_RAW_0 		Status Raw for Interrupt 1
	0x48032028 			0x4804C028 				GPIO_IRQSTATUS_RAW_1 		Status Raw for Interrupt 2
	0x4803202C 			0x4804C02C 				GPIO_IRQSTATUS_0 			Status for Interrupt 1
	0x48032030 			0x4804C030 				GPIO_IRQSTATUS_1 			Status for Interrupt 2
	0x48032034 			0x4804C034 				GPIO_IRQSTATUS_SET_0 		Enable Set for Interrupt 1
	0x48032038 			0x4804C038 				GPIO_IRQSTATUS_SET_1 		Enable Set for Interrupt 2
	0x4803203C 			0x4804C03C 				GPIO_IRQSTATUS_CLR_0 		Enable Clear for Interrupt 1
	0x48032040 			0x4804C040 				GPIO_IRQSTATUS_CLR_1 		Enable Clear for Interrupt 2
	0x48032044 			0x4804C044 				GPIO_IRQWAKEN_0 			Wakeup Enable for Interrupt 1
	0x48032048 			0x4804C048 				GPIO_IRQWAKEN_1 			Wakeup Enable for Interrupt 2
	0x48032114 			0x4804C114 				GPIO_SYSSTATUS 				System Status
	0x48032130 			0x4804C130 				GPIO_CTRL 					Module Control
	0x48032134 			0x4804C134 				GPIO_OE 						Output Enable
	0x48032138 			0x4804C138 				GPIO_DATAIN 					Data Input
	0x4803213C 			0x4804C13C 				GPIO_DATAOUT 				Data Output
	0x48032140 			0x4804C140 				GPIO_LEVELDETECT0 			Detect Low Level
	0x48032144 			0x4804C144 				GPIO_LEVELDETECT1 			Detect High Level
	0x48032148 			0x4804C148 				GPIO_RISINGDETECT 			Detect Rising Edge
	0x4803214C 			0x4804C14C 				GPIO_FALLINGDETECT 			Detect Falling Edge
	0x48032150 			0x4804C150 				GPIO_DEBOUNCENABLE 			Debouncing Enable
	0x48032154 			0x4804C154 				GPIO_DEBOUNCINGTIME 		Debouncing Value
	0x48032190 			0x4804C190 				GPIO_CLEARDATAOUT 			Clear Data Output
	0x48032194 			0x4804C194 				GPIO_SETDATAOUT 			Set Data Output
	*/


	const int GPIO0_BASE = 0x48032000;
	volatile int *GPIO0_revision = (int *) (GPIO0_BASE + 0x00);
	volatile int *GPIO0_sysconfig = (int *) (GPIO0_BASE + 0x10);
	volatile int *GPIO0_EOI = (int *) (GPIO0_BASE + 0x20);
	volatile int *GPIO0_IRQSTATUS_RAW_0 = (int *) (GPIO0_BASE + 0x24);
	volatile int *GPIO0_IRQSTATUS_RAW_1 = (int *) (GPIO0_BASE + 0x28);
	volatile int *GPIO0_IRQSTATUS_0 = (int *) (GPIO0_BASE + 0x2C);
	volatile int *GPIO0_IRQSTATUS_1 = (int *) (GPIO0_BASE + 0x30);
	volatile int *GPIO0_IRQSTATUS_SET_0 = (int *) (GPIO0_BASE + 0x34);
	volatile int *GPIO0_IRQSTATUS_SET_1 = (int *) (GPIO0_BASE + 0x38);
	volatile int *GPIO0_IRQSTATUS_CLR_0 = (int *) (GPIO0_BASE + 0x3C);
	volatile int *GPIO0_IRQSTATUS_CLR_1 = (int *) (GPIO0_BASE + 0x40);
	volatile int *GPIO0_IRQWAKEN_0 = (int *) (GPIO0_BASE + 0x44);
	volatile int *GPIO0_IRQWAKEN_1 = (int *) (GPIO0_BASE + 0x48);
	volatile int *GPIO0_SYSSTATUS = (int *) (GPIO0_BASE + 0x114);
	volatile int *GPIO0_CTRL = (int *) (GPIO0_BASE + 0x130);
	volatile int *GPIO0_OE = (int *) (GPIO0_BASE + 0x134);
	volatile int *GPIO0_DATAIN = (int *) (GPIO0_BASE + 0x138);
	volatile int *GPIO0_DATAOUT = (int *) (GPIO0_BASE + 0x13C);
	volatile int *GPIO0_LEVELDETECT0 = (int *) (GPIO0_BASE + 0x140);
	volatile int *GPIO0_LEVELDETECT1 = (int *) (GPIO0_BASE + 0x144);
	volatile int *GPIO0_RISINGDETECT = (int *) (GPIO0_BASE + 0x148);
	volatile int *GPIO0_FALLINGDETECT = (int *) (GPIO0_BASE + 0x14C);
	volatile int *GPIO0_DEBOUNCENABLE = (int *) (GPIO0_BASE + 0x150);
	volatile int *GPIO0_DEBOUNCINGTIME = (int *) (GPIO0_BASE + 0x154);
	volatile int *GPIO0_CLEARDATAOUT = (int *) (GPIO0_BASE + 0x190);
	volatile int *GPIO0_SETDATAOUT = (int *) (GPIO0_BASE + 0x194);

	const int GPIO1_BASE = 0x4804C000;
	volatile int *GPIO1_revision = (int *) (GPIO1_BASE + 0x00);
	volatile int *GPIO1_sysconfig = (int *) (GPIO1_BASE + 0x10);
	volatile int *GPIO1_EOI = (int *) (GPIO1_BASE + 0x20);
	volatile int *GPIO1_IRQSTATUS_RAW_0 = (int *) (GPIO1_BASE + 0x24);
	volatile int *GPIO1_IRQSTATUS_RAW_1 = (int *) (GPIO1_BASE + 0x28);
	volatile int *GPIO1_IRQSTATUS_0 = (int *) (GPIO1_BASE + 0x2C);
	volatile int *GPIO1_IRQSTATUS_1 = (int *) (GPIO1_BASE + 0x30);
	volatile int *GPIO1_IRQSTATUS_SET_0 = (int *) (GPIO1_BASE + 0x34);
	volatile int *GPIO1_IRQSTATUS_SET_1 = (int *) (GPIO1_BASE + 0x38);
	volatile int *GPIO1_IRQSTATUS_CLR_0 = (int *) (GPIO1_BASE + 0x3C);
	volatile int *GPIO1_IRQSTATUS_CLR_1 = (int *) (GPIO1_BASE + 0x40);
	volatile int *GPIO1_IRQWAKEN_0 = (int *) (GPIO1_BASE + 0x44);
	volatile int *GPIO1_IRQWAKEN_1 = (int *) (GPIO1_BASE + 0x48);
	volatile int *GPIO1_SYSSTATUS = (int *) (GPIO1_BASE + 0x114);
	volatile int *GPIO1_CTRL = (int *) (GPIO1_BASE + 0x130);
	volatile int *GPIO1_OE = (int *) (GPIO1_BASE + 0x134);
	volatile int *GPIO1_DATAIN = (int *) (GPIO1_BASE + 0x138);
	volatile int *GPIO1_DATAOUT = (int *) (GPIO1_BASE + 0x13C);
	volatile int *GPIO1_LEVELDETECT0 = (int *) (GPIO1_BASE + 0x140);
	volatile int *GPIO1_LEVELDETECT1 = (int *) (GPIO1_BASE + 0x144);
	volatile int *GPIO1_RISINGDETECT = (int *) (GPIO1_BASE + 0x148);
	volatile int *GPIO1_FALLINGDETECT = (int *) (GPIO1_BASE + 0x14C);
	volatile int *GPIO1_DEBOUNCENABLE = (int *) (GPIO1_BASE + 0x150);
	volatile int *GPIO1_DEBOUNCINGTIME = (int *) (GPIO1_BASE + 0x154);
	volatile int *GPIO1_CLEARDATAOUT = (int *) (GPIO1_BASE + 0x190);
	volatile int *GPIO1_SETDATAOUT = (int *) (GPIO1_BASE + 0x194);

	*PINCTRL189 =0x00000002;//0x48140AF0 PINCTRL189 0 0 GPMC_A[27] GP1[9] Mod=010
	*PINCTRL190 =0x0000000A;//0x48140AF4 PINCTRL190 0 1 GPMC_A[22] GP1[10]Mod=010
	*PINCTRL191 =0x0000000A;//0x48140AF8 PINCTRL191 0 1 GPMC_A[26] GP1[11]Mod=010
	*PINCTRL192 =0x00000002;//0x48140AFC PINCTRL192 0 0 GPMC_A[25] GP1[12]Mod=010
	*PINCTRL193 =0x00000009;//0x48140B00 PINCTRL193 0 1 GP1[13] 	Mod=001 							
	*PINCTRL194 =0x0000000A;//0x48140B04 PINCTRL194 0 1 GPMC_A[23] GP1[14]Mod=010
	*PINCTRL195 =0x0000000A;//0x48140B08 PINCTRL195 0 1 GPMC_A[24] GP1[15]Mod=010

	*GPIO1_CLEARDATAOUT = 0;		
	*GPIO1_OE = 0xFFFF01FF;
	
	*GPIO1_DATAOUT = 0xFE00;
	*GPIO1_SETDATAOUT = 0;	
	
	#if 0
	Log_info1("Configuring GPIO1_EOI = 0x%x\n", *GPIO0_EOI);
	Log_info1("Configuring GPIO1_OE = 0x%x\n", *GPIO0_OE);
	Log_info1("Configuring GPIO1_DATAIN = 0x%x\n", *GPIO0_DATAIN);
	Log_info1("Configuring GPIO1_DATAOUT = 0x%x\n", *GPIO0_DATAOUT);
	Log_info1("Configuring GPIO1_CLEARDATAOUT = 0x%x\n", *GPIO0_CLEARDATAOUT);
	Log_info1("Configuring GPIO1_SETDATAOUT = 0x%x\n", *GPIO0_SETDATAOUT);	
	#endif
}

void Hwipwr_Func(UArg arg0)
{
    Log_info1("Entering Hwi pwr time = %d\n",Clock_getTicks());
}
void HwiPulse_Func(UArg arg0)
{
	//Log_info2("Entering Hwi10 id:%d, time = %d\n",hwi10params.eventId, Clock_getTicks());
	NCAreaPulseToServo_LPMEN1();
}

void HwiPulse1_Func(UArg arg0)
{
	//Log_info2("Entering Hwi10 id:%d, time = %d\n",hwi10params.eventId, Clock_getTicks());
	NCAreaPulseToServo_LPMEN1();
	*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<23);
}

void HwiPulse2_Func(UArg arg0)
{
	//Log_info2("Entering Hwi11 id:%d, time = %d\n",hwi11params.eventId, Clock_getTicks());
	NCAreaPulseToServo_LPMEN2();
	*(GPIO_IRQStatus.GPIO0_IRQStatus_1) = (0x1<<23);
}
void HwiPulse3_Func(UArg arg0)
{
	//Log_info2("Entering Hwi12 id:%d, time = %d\n",hwi12params.eventId, Clock_getTicks());
	NCAreaPulseToServo_LPMEN1();
	*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = (0x1<<17);
}

void HwiPulse4_Func(UArg arg0)
{	
	//Log_info2("Entering Hwi13 id:%d, time = %d\n",hwi13params.eventId, Clock_getTicks());
	NCAreaPulseToServo_LPMEN2();
	*(GPIO_IRQStatus.GPIO1_IRQStatus_1) = (0x1<<17);

	#if 0
		test_PWM_CS();
	#endif
}

void HwiMbus_Func(UArg arg0)
{
	Log_info2("Entering Hwi11 id:%d, time = %d\n",hwi11params.eventId, Clock_getTicks());
    	//NCAreaPulseToServo_LPMEN1();
}

void Task1_Func(UArg arg0, UArg arg1) 
{
	Log_info0("task1 start");	
	
	#if 0
	//test();
	//init485UART();
	//SPI_Init();
	#endif
	
	SHM_Init();
	SCI_Init();

	Poweron_Initial();
	System_Initial();

	Parameter_Initial(); 	

    	Log_info0("Hwi start.\n");
		
	//IRQ_ENABLE(IRQNUM_PLUSE);
	//IRQ_ENABLE(IRQNUM_MBUS);
	IRQ_ENABLE(IRQNUM_PLUSE1);
	IRQ_ENABLE(IRQNUM_PLUSE2);	
	IRQ_ENABLE(IRQNUM_PLUSE3);
	IRQ_ENABLE(IRQNUM_PLUSE4);	

	main_cycle();
}

Int main(Int argc, Char* argv[]) 
{	
	//Diags_setMask("xdc.runtime.Main-F");
	/*
	 * Diags.EMERGENCY    	level 1 cannot close
	 * Diags.CRITICAL		level 2 cannot close
	 * Diags.ERROR			level 3 cannot close
	 * Diags.INFO           Log_info()
	 * close info output:Diags_setMask("xdc.runtime.Main-F");33
	 * E           ENTRY           Function entry
	 * X           EXIT            Function exit
	 * L           LIFECYCLE       Object life-cycle
	 * I           INTERNAL        Internal diagnostics
	 * A           ASSERT          Assert checking
	 * Z           ANALYSIS        Analysis event
	 * F           INFO            Informational event
	 * S           STATUS          Status (error, warning) event
	 * 1           USER1           User defined diagnostics
	 * 2           USER2           User defined diagnostics
	 * 3           USER3           User defined diagnostics
	 * 4           USER4           User defined diagnostics
	 * 5           USER5           User defined diagnostics
	 * 6           USER6           User defined diagnostics
	 * 7           USER7           User defined diagnostics
	 * 8           USER8           User defined diagnostics
	 %c                      Character
	 %d                      Signed integer
	 %u                      Unsigned integer
	 %x                      Unsigned hexadecimal integer
	 %o                      Unsigned octal integer
	 %s                      Character string
	 %p                      Pointer
	 %f                      Single precision floating point (float)
	 Log_print2(Diags_USER2, "list[%u] = %s\n", i, (xdc_IArg)list[i]);
	 */
	/* must initialize the error block before using it */

	Error_init(&eb);

	/* create main thread (interrupts not enabled in main on BIOS) */
      Log_info0("-->dsp main");
	#if 0
  	Hwi_Params_init(&hwipwrparams);
  	hwipwrparams.instance->name = "Hwipwr";
  	hwipwrparams.eventId = 65; //even id is the hardware interrupt num, ref to datasheet
  	hwipwr = Hwi_create(IRQNUM_PWR, Hwipwr_Func, &hwipwrparams, &eb);
  	if (Error_check(&eb)) 
	{
  		Log_error0("main: failed to create hwi power down startup thread");
  	}
	
	Hwi_Params_init(&hwi10params);
	hwi10params.instance->name = "Hwi10";
	hwi10params.eventId = 66;
	hwi10 = Hwi_create(IRQNUM_PLUSE, HwiPulse_Func, &hwi10params, &eb);
	if(Error_check(&eb)) 
	{
		Log_error0("main: failed to create hwi 10 startup thread");
	}

	Hwi_Params_init(&hwi11params);
	hwi11params.instance->name = "Hwi11";
	hwi11params.eventId = 64;
	hwi11 = Hwi_create(IRQNUM_MBUS, HwiMbus_Func, &hwi11params, &eb);
	if(Error_check(&eb))
	{
		Log_error0("main: failed to create hwi 11 startup thread");
	}
	#endif
	
	Hwi_Params_init(&hwi10params);
	hwi10params.instance->name = "Hwi10";
	hwi10params.eventId = 64;		//GPIOINT0A	GPIO0
	hwi10 = Hwi_create(IRQNUM_PLUSE1, HwiPulse1_Func, &hwi10params, &eb);
	if(Error_check(&eb)) 
	{
		Log_error0("main: failed to create hwi 10 startup thread");
	}

	Hwi_Params_init(&hwi11params);
	hwi11params.instance->name = "Hwi11";
	hwi11params.eventId = 65;		//GPIOINT0B	GPIO0
	hwi11 = Hwi_create(IRQNUM_PLUSE2, HwiPulse2_Func, &hwi11params, &eb);
	if(Error_check(&eb))
	{
		Log_error0("main: failed to create hwi 11 startup thread");
	}

		
	Hwi_Params_init(&hwi12params);
	hwi12params.instance->name = "Hwi12";
	hwi12params.eventId = 66;		//GPIOINT1A	GPIO1
	hwi12 = Hwi_create(IRQNUM_PLUSE3, HwiPulse3_Func, &hwi12params, &eb);
	if(Error_check(&eb)) 
	{
		Log_error0("main: failed to create hwi 12 startup thread");
	}

	Hwi_Params_init(&hwi13params);
	hwi13params.instance->name = "Hwi13";
	hwi13params.eventId = 67;		//GPIOINT1B	GPIO1
	hwi13 = Hwi_create(IRQNUM_PLUSE4, HwiPulse4_Func, &hwi13params, &eb);
	if(Error_check(&eb))
	{
		Log_error0("main: failed to create hwi 13 startup thread");
	}
	
	Task_Params_init(&task1params);
	task1params.instance->name = "DSPTask1";
	task1params.stackSize = 0x4000;
	task1params.priority = 3;
	task1 = Task_create(Task1_Func, &task1params, &eb);
	if(Error_check(&eb))
	{
		Log_error0("main: failed to create application1 startup thread");
	}	

	/* start scheduler, this never returns */
	configureMmu();
	configInterruptsPin();
	configGPIO_OutputPin();
	Log_info0("Starting BIOS");
	BIOS_start();

	/* should never get here */
	Log_info0("Error leaving main");
	return (0);
}

void Init_Error(void)	
{		
	Error.NCCodeBreak = 0;
	Error.InterpolationOver = 0;
	Error.TrackRunOutError = 0;
	Error.HaveGetErrorDataSign= 0;
	Error.MainErrorSign = 0;

	Error.AlmHLimitPos = 0;
	Error.AlmHLimitNeg = 0;
	Error.AlmSLimitPos = 0;
	Error.AlmSLimitNeg = 0;
	Error.InsideRAMCheckError = 0;
	Error.OutsideRAMCheckError = 0;
	Error.CompDataCheckError = 0;	
	Error.OpticalFlashTriggerBreakError = 0;
	Error.CameraShutterTriggerBreakError = 0;
	Error.LensPositionBreakError = 0;

	memset(&Error,0x0,sizeof(Error));
}

void Init_Test(void)	
{	
	STDNCSign.ResendGcodeSign = 0;
	NCRunSign.RealRunCount			= 0;
	NCRunSign.RealSendCount			= 0;
	NCRunSign.PrecalCode 			= 0;
	NC_AREAControl.Pointer_EndCode	= 0;
	NC_AREAControl.VNow_Module.Module1 = 0;
	NC_AREAControl.VNow_Module.Module2 = 0;
	NC_AREAControl.VNow_Module.Module3 = 0;
	NC_AREAControl.VNow_Module.Module4 = 0;
	NC_AREAControl.VNow_Module.Module5 = 0;
	NC_AREAControl.VNow_Module.Module6 = 0;
	NC_AREAControl.VNow_Module.Module7 = 0;
	NC_AREAControl.VNow_Module.Module8 = 0;
	NC_AREAControl.VNow_Module.Module9 = 0;
	NC_AREAControl.VNow_Module.Module10 = 0;
	NC_AREAControl.VNow_Module.Module11 = 0;
	NC_AREAControl.VNow_Module.Module12 = 0;
	NC_AREAControl.Vm_Module.Module1 = 0;
	NC_AREAControl.Vm_Module.Module2 = 0;
	NC_AREAControl.Vm_Module.Module3 = 0;
	NC_AREAControl.Vm_Module.Module4 = 0;
	NC_AREAControl.Vm_Module.Module5 = 0;
	NC_AREAControl.Vm_Module.Module6 = 0;
	NC_AREAControl.Vm_Module.Module7 = 0;
	NC_AREAControl.Vm_Module.Module8 = 0;
	NC_AREAControl.Vm_Module.Module9 = 0;
	NC_AREAControl.Vm_Module.Module10 = 0;
	NC_AREAControl.Vm_Module.Module11 = 0;
	NC_AREAControl.Vm_Module.Module12 = 0;
	NC_AREAControl.Ve_Module.Module1 = 0;
	NC_AREAControl.Ve_Module.Module2 = 0;
	NC_AREAControl.Ve_Module.Module3 = 0;
	NC_AREAControl.Ve_Module.Module4 = 0;
	NC_AREAControl.Ve_Module.Module5 = 0;
	NC_AREAControl.Ve_Module.Module6 = 0;
	NC_AREAControl.Ve_Module.Module7 = 0;
	NC_AREAControl.Ve_Module.Module8 = 0;
	NC_AREAControl.Ve_Module.Module9 = 0;
	NC_AREAControl.Ve_Module.Module10 = 0;
	NC_AREAControl.Ve_Module.Module11 = 0;
	NC_AREAControl.Ve_Module.Module12 = 0;
	NC_AREAControl.Vs_Module.Module1 = 0;
	NC_AREAControl.Vs_Module.Module2 = 0;
	NC_AREAControl.Vs_Module.Module3 = 0;
	NC_AREAControl.Vs_Module.Module4 = 0;
	NC_AREAControl.Vs_Module.Module5 = 0;
	NC_AREAControl.Vs_Module.Module6 = 0;
	NC_AREAControl.Vs_Module.Module7 = 0;
	NC_AREAControl.Vs_Module.Module8 = 0;
	NC_AREAControl.Vs_Module.Module9 = 0;
	NC_AREAControl.Vs_Module.Module10 = 0;
	NC_AREAControl.Vs_Module.Module11 = 0;
	NC_AREAControl.Vs_Module.Module12 = 0;
	NC_AREAControl.VmReal_Module.Module1 = 0;
	NC_AREAControl.VmReal_Module.Module2 = 0;
	NC_AREAControl.VmReal_Module.Module3 = 0;
	NC_AREAControl.VmReal_Module.Module4 = 0;
	NC_AREAControl.VmReal_Module.Module5 = 0;
	NC_AREAControl.VmReal_Module.Module6 = 0;
	NC_AREAControl.VmReal_Module.Module7 = 0;
	NC_AREAControl.VmReal_Module.Module8 = 0;
	NC_AREAControl.VmReal_Module.Module9 = 0;
	NC_AREAControl.VmReal_Module.Module10 = 0;
	NC_AREAControl.VmReal_Module.Module11 = 0;
	NC_AREAControl.VmReal_Module.Module12 = 0;
	
	Encoder_Axis1.MACH_PositionValue	= 0;
	Encoder_Axis2.MACH_PositionValue	= 0;
	Encoder_Axis3.MACH_PositionValue	= 0;
	Encoder_Axis4.MACH_PositionValue	= 0;
	Encoder_Axis5.MACH_PositionValue	= 0;
	Encoder_Axis6.MACH_PositionValue	= 0;
	Encoder_Axis7.MACH_PositionValue	= 0;
	Encoder_Axis8.MACH_PositionValue	= 0;
	Encoder_Axis9.MACH_PositionValue	= 0;
	Encoder_Axis10.MACH_PositionValue	= 0;
	Encoder_Axis11.MACH_PositionValue	= 0;
	Encoder_Axis12.MACH_PositionValue	= 0;
	Encoder_Axis13.MACH_PositionValue	= 0;
	Encoder_Axis14.MACH_PositionValue	= 0;
	Encoder_Axis15.MACH_PositionValue	= 0;
	Encoder_Axis16.MACH_PositionValue	= 0;
	Encoder_Axis17.MACH_PositionValue	= 0;
	Encoder_Axis18.MACH_PositionValue	= 0;
	Encoder_Axis19.MACH_PositionValue	= 0;
	Encoder_Axis20.MACH_PositionValue	= 0;
	Encoder_Axis21.MACH_PositionValue	= 0;
	Encoder_Axis22.MACH_PositionValue	= 0;
	Encoder_Axis23.MACH_PositionValue	= 0;


	OverallSign.InterpolationSendCnt_1 = 0;
	OverallSign.InterpolationRunCnt_1 = 0;
	OverallSign.InterpolationSendCnt_2 = 0;
	OverallSign.InterpolationRunCnt_2 = 0;	
	System.Tsample1	= 0;

	//clear error sign
	Error.EncoderError.Axis1	= 0;
	Error.EncoderError.Axis2	= 0;
	Error.EncoderError.Axis3	= 0;
	Error.EncoderError.Axis4 	= 0;
	Error.EncoderError.Axis5	= 0;
	Error.EncoderError.Axis6	= 0;
	Error.EncoderError.Axis7	= 0;
	Error.EncoderError.Axis8 	= 0;
	Error.EncoderError.Axis9	= 0;
	Error.EncoderError.Axis10	= 0;
	Error.EncoderError.Axis11	= 0;
	Error.EncoderError.Axis12	= 0;
	Error.EncoderError.Axis13	= 0;
	Error.EncoderError.Axis14 	= 0;
	Error.EncoderError.Axis15	= 0;
	Error.EncoderError.Axis16	= 0;
	Error.EncoderError.Axis17	= 0;
	Error.EncoderError.Axis18 	= 0;
	Error.EncoderError.Axis19	= 0;
	Error.EncoderError.Axis20	= 0;
	Error.EncoderError.Axis21	= 0;
	Error.EncoderError.Axis22	= 0;
	Error.EncoderError.Axis23	= 0;	

	MainCommand_ForTestSign.all = 0;
	MainCommand_ForTestSign.all = 0xff;//for test
}


void Init_DspPara(void)
{	
	System.Tsample = 0.001;	//1ms
	System.Tsample1 = System.Tsample;
	System.PWM_PeriodRegister_ZeroPeriod = 2;
	System.PWM_PeriodRegister_MIN = divdp(FPGA_MAX_F,2000000.0);//2M
	System.PWM_PeriodRegister_MAX = 65500;
	System.SlaveMAX = 23;	
	System.TrackRunOutRangeSQR = 10000;
	System.a_SET_Module.Module1 = 2000;
	System.a_SET_Module.Module2 = 2000;
	System.a_SET_Module.Module3 = 2000;
	System.a_SET_Module.Module4 = 2000;
	System.a_SET_Module.Module5 = 2000;
	System.a_SET_Module.Module6 = 2000;
	System.a_SET_Module.Module7 = 2000;
	System.a_SET_Module.Module8 = 2000;
	System.a_SET_Module.Module9 = 2000;
	System.a_SET_Module.Module10 = 2000;
	System.a_SET_Module.Module11 = 2000;
	System.a_SET_Module.Module12 = 2000;

	System.a_A4_SET_Module.Module1 = 2000;
	System.a_A4_SET_Module.Module2 = 2000;
	System.a_A4_SET_Module.Module3 = 2000;
	System.a_A4_SET_Module.Module4 = 2000;
	System.a_A4_SET_Module.Module5 = 2000;
	System.a_A4_SET_Module.Module6 = 2000;
	System.a_A4_SET_Module.Module7 = 2000;
	System.a_A4_SET_Module.Module8 = 2000;
	System.a_A4_SET_Module.Module9 = 2000;
	System.a_A4_SET_Module.Module10 = 2000;
	System.a_A4_SET_Module.Module11 = 2000;
	System.a_A4_SET_Module.Module12 = 2000;


	System.VeerDeltaV = 3;
	System.NicetyVeerDeltaV = 0.5;	
	System.VeerDeltaT = 0.0015; 
	System.NicetyVeerDeltaT = 0.0005; 
	//System.REFStopVariable = 0;
	System.REFStopVariable.Axis1 = 0;
	System.REFStopVariable.Axis2 = 0;
	System.REFStopVariable.Axis3 = 0;
	System.REFStopVariable.Axis4 = 0;
	System.REFStopVariable.Axis5 = 0;
	System.REFStopVariable.Axis6 = 0;
	System.REFStopVariable.Axis7 = 0;
	System.REFStopVariable.Axis8 = 0;
	System.REFStopVariable.Axis9 = 0;
	System.REFStopVariable.Axis10 = 0;
	System.REFStopVariable.Axis11 = 0;
	System.REFStopVariable.Axis12 = 0;
	System.REFStopVariable.Axis13 = 0;
	System.REFStopVariable.Axis14 = 0;
	System.REFStopVariable.Axis15 = 0;
	System.REFStopVariable.Axis16 = 0;
	System.REFStopVariable.Axis17 = 0;
	System.REFStopVariable.Axis18 = 0;
	System.REFStopVariable.Axis19 = 0;
	System.REFStopVariable.Axis20 = 0;
	System.REFStopVariable.Axis21 = 0;
	System.REFStopVariable.Axis22 = 0;
	System.REFStopVariable.Axis23 = 0;
	System.REFStopVariable.Axis24 = 0;
	System.REFStopVariable.Axis25 = 0;	

	System.RefDir.Axis1 = 0;
	System.RefDir.Axis2 = 0;
	System.RefDir.Axis3 = 0;
	System.RefDir.Axis4 = 0;
	System.RefDir.Axis5 = 0;
	System.RefDir.Axis6 = 0;
	System.RefDir.Axis7 = 0;
	System.RefDir.Axis8 = 0;
	System.RefDir.Axis9 = 0;
	System.RefDir.Axis10 = 0;
	System.RefDir.Axis11 = 0;
	System.RefDir.Axis12 = 0;
	System.RefDir.Axis13 = 0;
	System.RefDir.Axis15 = 0;
	System.RefDir.Axis15 = 0;
	System.RefDir.Axis16 = 0;
	System.RefDir.Axis17 = 0;
	System.RefDir.Axis18 = 0;
	System.RefDir.Axis19 = 0;
	System.RefDir.Axis20 = 0;
	System.RefDir.Axis21 = 0;
	System.RefDir.Axis22 = 0;
	System.RefDir.Axis23 = 0;
	System.RefDir.Axis24 = 0;
	System.RefDir.Axis25 = 0;
	
	System.EncoderCheckChoose.Axis1 = 1;
	System.EncoderCheckChoose.Axis2 = 1;
	System.EncoderCheckChoose.Axis3 = 1;
	System.EncoderCheckChoose.Axis4 = 1;
	System.EncoderCheckChoose.Axis5 = 1;
	System.EncoderCheckChoose.Axis6 = 1;
	System.EncoderCheckChoose.Axis7 = 1;
	System.EncoderCheckChoose.Axis8 = 1;
	System.EncoderCheckChoose.Axis9 = 1;
	System.EncoderCheckChoose.Axis10 = 1;
	System.EncoderCheckChoose.Axis11 = 1;
	System.EncoderCheckChoose.Axis12 = 1;
	System.EncoderCheckChoose.Axis13 = 1;
	System.EncoderCheckChoose.Axis14 = 1;
	System.EncoderCheckChoose.Axis15 = 1;
	System.EncoderCheckChoose.Axis16 = 1;
	System.EncoderCheckChoose.Axis17 = 1;
	System.EncoderCheckChoose.Axis18 = 1;
	System.EncoderCheckChoose.Axis19 = 1;
	System.EncoderCheckChoose.Axis20 = 1;
	System.EncoderCheckChoose.Axis21 = 1;
	System.EncoderCheckChoose.Axis22 = 1;
	System.EncoderCheckChoose.Axis23 = 1;
	System.EncoderCheckChoose.Axis24 = 0;
	System.EncoderCheckChoose.Axis25 = 0;		

	System.LinearAxisMinUnit = 1000;		
	System.UnitTo_mm =0.001; 
	System.LinearAxisOutUnitEQU = 1000;	
	
	System.G0Speed = 100;
	System.G0Speed_2 = 100;

	System.G0Speed_Module.Module1 = 100;
	System.G0Speed_Module.Module2 = 100;
	System.G0Speed_Module.Module3 = 100;
	System.G0Speed_Module.Module4 = 100;
	System.G0Speed_Module.Module5 = 100;
	System.G0Speed_Module.Module6 = 100;
	System.G0Speed_Module.Module7 = 100;
	System.G0Speed_Module.Module8 = 100;
	System.G0Speed_Module.Module9 = 100;
	System.G0Speed_Module.Module10 = 100;
	System.G0Speed_Module.Module11 = 100;
	System.G0Speed_Module.Module12 = 100;
	
	System.G1Speed = 10;
	System.G1Speed_2 = 10;

	System.G1Speed_Module.Module1 = 10;
	System.G1Speed_Module.Module2 = 10;
	System.G1Speed_Module.Module3 = 10;
	System.G1Speed_Module.Module4 = 10;
	System.G1Speed_Module.Module5 = 10;
	System.G1Speed_Module.Module6 = 10;
	System.G1Speed_Module.Module7 = 10;
	System.G1Speed_Module.Module8 = 10;
	System.G1Speed_Module.Module9 = 10;
	System.G1Speed_Module.Module10 = 10;
	System.G1Speed_Module.Module11 = 10;
	System.G1Speed_Module.Module12 = 10;

	System.SRefSpeed = 30;
	System.SRefSpeedBack = 3;
	System.SRefBack = 20;

	System.SRefSpeed_2 = 10;
	System.SRefSpeedBack_2 = 2;
	System.SRefBack_2 = 1;

	System.FunctionSelect01 = 0;
	System.FunctionSelect02 = 0;
	System.FunctionSelect03 = 0;
	System.FunctionSelect04 = 0;
	System.FunctionSelect05 = 0;	
	System.FunctionSelect06 = 0;
	System.FunctionSelect07 = 0;
	System.FunctionSelect08 = 0;
	System.FunctionSelect09 = 0;
	System.FunctionSelect10 = 0;

	System.MotorChangeDir.Axis1 = 1;
	System.MotorChangeDir.Axis2 = 0;
	System.MotorChangeDir.Axis3 = 1;
	System.MotorChangeDir.Axis4 = 1;
	System.MotorChangeDir.Axis5 = 0;
	System.MotorChangeDir.Axis6 = 1;
	System.MotorChangeDir.Axis7 = 0;
	System.MotorChangeDir.Axis8 = 0;
	System.MotorChangeDir.Axis9 = 0;
	System.MotorChangeDir.Axis10 = 0;
	System.MotorChangeDir.Axis11 = 0;
	System.MotorChangeDir.Axis12 = 1;
	System.MotorChangeDir.Axis13 = 0;
	System.MotorChangeDir.Axis14 = 0;
	System.MotorChangeDir.Axis15 = 0;
	System.MotorChangeDir.Axis16 = 0;
	System.MotorChangeDir.Axis17 = 0;
	System.MotorChangeDir.Axis18 = 0;
	System.MotorChangeDir.Axis19 = 0;
	System.MotorChangeDir.Axis20 = 0;
	System.MotorChangeDir.Axis21 = 0;
	System.MotorChangeDir.Axis22 = 0;
	System.MotorChangeDir.Axis23 = 0;
	System.MotorChangeDir.Axis24 = 0;
	System.MotorChangeDir.Axis25 = 0;

	Encoder_Axis1.EncoderDIR = 0;
	Encoder_Axis2.EncoderDIR = 0;
	Encoder_Axis3.EncoderDIR = 0;
	Encoder_Axis4.EncoderDIR = 0;
	Encoder_Axis5.EncoderDIR = 0;
	Encoder_Axis6.EncoderDIR = 0;
	Encoder_Axis7.EncoderDIR = 1;
	Encoder_Axis8.EncoderDIR = 1;
	Encoder_Axis9.EncoderDIR = 0;
	Encoder_Axis10.EncoderDIR = 0;
	Encoder_Axis11.EncoderDIR = 1;
	Encoder_Axis12.EncoderDIR = 0;
	Encoder_Axis13.EncoderDIR = 0;
	Encoder_Axis14.EncoderDIR = 0;
	Encoder_Axis15.EncoderDIR = 0;
	Encoder_Axis16.EncoderDIR = 0;
	Encoder_Axis17.EncoderDIR = 0;
	Encoder_Axis18.EncoderDIR = 0;
	Encoder_Axis19.EncoderDIR = 0;
	Encoder_Axis20.EncoderDIR = 0;	
	Encoder_Axis21.EncoderDIR = 0;
	Encoder_Axis22.EncoderDIR = 0;
	Encoder_Axis23.EncoderDIR = 0;

	System.AxisResolution.Axis1 = 1000;
	System.AxisResolution.Axis2 = 1000;
	System.AxisResolution.Axis3 = 1000;
	System.AxisResolution.Axis4 = 1000;
	System.AxisResolution.Axis5 = 1000;
	System.AxisResolution.Axis6 = 1000;
	System.AxisResolution.Axis7 = 1000;
	System.AxisResolution.Axis8 = 1000;
	System.AxisResolution.Axis9 = 1000;
	System.AxisResolution.Axis10 = 1000;
	System.AxisResolution.Axis11 = 1000;
	System.AxisResolution.Axis12 = 1000;
	System.AxisResolution.Axis13 = 1000;
	System.AxisResolution.Axis14 = 1000;
	System.AxisResolution.Axis15 = 800;
	System.AxisResolution.Axis16 = 800;
	System.AxisResolution.Axis17 = 800;
	System.AxisResolution.Axis18 = 800;
	System.AxisResolution.Axis19 = 1000;
	System.AxisResolution.Axis20 = 800;
	System.AxisResolution.Axis21 = 800;
	System.AxisResolution.Axis22 = 800;
	System.AxisResolution.Axis23 = 1000;
	System.AxisResolution.Axis24 = 1000;
	System.AxisResolution.Axis25 = 1000;

	Encoder_Axis1.Resolution = 1;
	Encoder_Axis2.Resolution = 1;
	Encoder_Axis3.Resolution = 1;
	Encoder_Axis4.Resolution = 1;
	Encoder_Axis5.Resolution = 1;
	Encoder_Axis6.Resolution = 1;
	Encoder_Axis7.Resolution = 1;
	Encoder_Axis8.Resolution = 1;
	Encoder_Axis9.Resolution = 1;
	Encoder_Axis10.Resolution = 1;
	Encoder_Axis11.Resolution = 1;
	Encoder_Axis12.Resolution = 1;
	Encoder_Axis13.Resolution = 1;
	Encoder_Axis14.Resolution = 1;
	Encoder_Axis15.Resolution = 0.8;
	Encoder_Axis16.Resolution = 0.8;
	Encoder_Axis17.Resolution = 0.8;
	Encoder_Axis18.Resolution = 0.8;
	Encoder_Axis19.Resolution = 0.5;
	Encoder_Axis20.Resolution = 0.8;	
	Encoder_Axis21.Resolution = 0.8;
	Encoder_Axis22.Resolution = 0.8;
	Encoder_Axis23.Resolution = 1;
	
	System.CoordORG.Axis1 = 0;
	System.CoordORG.Axis2 = 0;
	System.CoordORG.Axis3 = 0;
	System.CoordORG.Axis4 = 0;
	System.CoordORG.Axis5 = 0;
	System.CoordORG.Axis6 = 0;
	System.CoordORG.Axis7 = 0;
	System.CoordORG.Axis8 = 0;
	System.CoordORG.Axis9 = 0;
	System.CoordORG.Axis10 = 0;
	System.CoordORG.Axis11 = 0;
	System.CoordORG.Axis12 = 0;
	System.CoordORG.Axis13 = 0;
	System.CoordORG.Axis14 = 0;
	System.CoordORG.Axis15 = 0;
	System.CoordORG.Axis16 = 0;
	System.CoordORG.Axis17 = 0;
	System.CoordORG.Axis18 = 0;
	System.CoordORG.Axis19 = 0;
	System.CoordORG.Axis20 = 0;
	System.CoordORG.Axis21 = 0;
	System.CoordORG.Axis22 = 0;
	System.CoordORG.Axis23 = 0;
	System.CoordORG.Axis24 = 0;
	System.CoordORG.Axis25 = 0;		
			
	System.MAXSpeed.Axis1 = 20;
	System.MAXSpeed.Axis2 = 100;
	System.MAXSpeed.Axis3 = 100;
	System.MAXSpeed.Axis4 = 100;
	System.MAXSpeed.Axis5 = 100;
	System.MAXSpeed.Axis6 = 100;
	System.MAXSpeed.Axis7 = 100;
	System.MAXSpeed.Axis8 = 100;
	System.MAXSpeed.Axis9 = 100;
	System.MAXSpeed.Axis10 = 100;
	System.MAXSpeed.Axis11 = 100;
	System.MAXSpeed.Axis12 = 100;
	System.MAXSpeed.Axis13 = 100;
	System.MAXSpeed.Axis14 = 100;
	System.MAXSpeed.Axis15 = 100;
	System.MAXSpeed.Axis16 = 100;
	System.MAXSpeed.Axis17 = 100;
	System.MAXSpeed.Axis18 = 100;
	System.MAXSpeed.Axis19 = 100;
	System.MAXSpeed.Axis20 = 100;	
	System.MAXSpeed.Axis21 = 100;
	System.MAXSpeed.Axis22 = 100;
	System.MAXSpeed.Axis23 = 100;


	System.REFStopPosition.Axis1 = 0;
	System.REFStopPosition.Axis2 = 0;
	System.REFStopPosition.Axis3 = 0;
	System.REFStopPosition.Axis4 = 0;
	System.REFStopPosition.Axis5 = 0;
	System.REFStopPosition.Axis6 = 0;
	System.REFStopPosition.Axis7 = 0;
	System.REFStopPosition.Axis8 = 0;
	System.REFStopPosition.Axis9 = 0;
	System.REFStopPosition.Axis10 = 0;
	System.REFStopPosition.Axis11 = 0;
	System.REFStopPosition.Axis12 = 0;
	System.REFStopPosition.Axis13 = 0;
	System.REFStopPosition.Axis14 = 0;
	System.REFStopPosition.Axis15 = 0;
	System.REFStopPosition.Axis16 = 0;
	System.REFStopPosition.Axis17 = 0;
	System.REFStopPosition.Axis18 = 0;
	System.REFStopPosition.Axis19 = 0;
	System.REFStopPosition.Axis20 = 0;
	System.REFStopPosition.Axis21 = 0;
	System.REFStopPosition.Axis22 = 0;
	System.REFStopPosition.Axis23 = 0;
	System.REFStopPosition.Axis24 = 0;
	System.REFStopPosition.Axis25 = 0;	

	

	System.SLimitPos.Axis1 = 1000000;
	System.SLimitPos.Axis2 = 1000000;
	System.SLimitPos.Axis3 = 115000;
	System.SLimitPos.Axis4 = 115000;
	System.SLimitPos.Axis5 = 1000000;
	System.SLimitPos.Axis6 = 363000;
	System.SLimitPos.Axis7 = 788500;
	System.SLimitPos.Axis8 = 385000;
	System.SLimitPos.Axis9 = 1000000;
	System.SLimitPos.Axis10 = 1000000;
	System.SLimitPos.Axis11 = 385000;
	System.SLimitPos.Axis12 = 779000;
	System.SLimitPos.Axis13 = 1000000;
	System.SLimitPos.Axis14 = 1000000;
	System.SLimitPos.Axis15 = 656000;
	System.SLimitPos.Axis16 = 651000;
	System.SLimitPos.Axis17 = 165000;
	System.SLimitPos.Axis18 = 315000;
	System.SLimitPos.Axis19 = 20000;
	System.SLimitPos.Axis20 = 355000;
	System.SLimitPos.Axis21 = 1000000;
	System.SLimitPos.Axis22 = 1000000;
	System.SLimitPos.Axis23 = 1000000;
	System.SLimitPos.Axis24 = 1000000;
	System.SLimitPos.Axis25 = 1000000;	

	System.SLimitNeg.Axis1 = -1000000;
	System.SLimitNeg.Axis2 = -1000000;
	System.SLimitNeg.Axis3 = -1000;
	System.SLimitNeg.Axis4 = -1000;
	System.SLimitNeg.Axis5 = -1000000;
	System.SLimitNeg.Axis6 = -4000;
	System.SLimitNeg.Axis7 = -5000;
	System.SLimitNeg.Axis8 = -1000;
	System.SLimitNeg.Axis9 = -1000000;
	System.SLimitNeg.Axis10 = -1000000;
	System.SLimitNeg.Axis11 = -1000;
	System.SLimitNeg.Axis12 = -5000;
	System.SLimitNeg.Axis13 = -1000000;
	System.SLimitNeg.Axis14 = -1000000;
	System.SLimitNeg.Axis15 = -5000;
	System.SLimitNeg.Axis16 = -5000;
	System.SLimitNeg.Axis17 = 4000;
	System.SLimitNeg.Axis18 = -5000;
	System.SLimitNeg.Axis19 = -30000;
	System.SLimitNeg.Axis20 = -5000;
	System.SLimitNeg.Axis21 = -1000000;
	System.SLimitNeg.Axis22 = -1000000;
	System.SLimitNeg.Axis23 = -1000000;
	System.SLimitNeg.Axis24 = -1000000;
	System.SLimitNeg.Axis25 = -1000000;	

	System.LEDFlashTime1 = 500;
	System.LEDFlashTime2 = 500;

	System.RunState =0;
	System.ReferenceFrameMode = REF_XYZ;
	STDNCSign.FG0 = 200;
}


void Poweron_Initial(void)		
{
	static Int16  Gap_Axis15 = 1;
	Uint32 Voltage1_Initial,Voltage2_Initial;
	
	System.RunState = RealTimeRun;		

	Timer0.GenericCounter = 0;
	Timer0.SamplePeriod = 500;
	
	SDRAM_check();
	Init_DspPara();

	CPLD_GPIOinit();
	CPLD_GPIO_IRQStatusInit();
	CPLD_ServoInit();	
	//CPLD_ServoOn();			//伺服使能
	CPLD_ServoOff();	
	CPLD_EncoderResetAllAxis();
	CPLD_ResetPMTAll();

	//*(Servo_Axis1.LPM_CounterPort) = 0;
	//*(Servo_Axis2.LPM_CounterPort) = 0;
	//*(Servo_Axis3.LPM_CounterPort) = 0;
	Servo_LPMEN_Register.bit.LPMEN_1 = 0;
	Write_Servo_LPMEN();

	Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= 1;
	Write_Servo_Sign1_Out();

	Init_Test();		

	//for Refference		
	Reference.SearchRefSign 		= 0;
	Reference.NeedFindAxisSign.Axis1 	= 0;
	Reference.NeedFindAxisSign.Axis2 	= 0;
	Reference.NeedFindAxisSign.Axis3 	= 0;
	Reference.NeedFindAxisSign.Axis4 	= 0;
	Reference.NeedFindAxisSign.Axis5 	= 0;
	Reference.NeedFindAxisSign.Axis6 	= 0;
	Reference.NeedFindAxisSign.Axis7 	= 0;
	Reference.NeedFindAxisSign.Axis8 	= 0;
	Reference.NeedFindAxisSign.Axis9 	= 0;
	Reference.NeedFindAxisSign.Axis10 	= 0;
	Reference.NeedFindAxisSign.Axis11 	= 0;
	Reference.NeedFindAxisSign.Axis12 	= 0;
	Reference.NeedFindAxisSign.Axis13 	= 0;
	Reference.NeedFindAxisSign.Axis14 	= 0;
	Reference.NeedFindAxisSign.Axis15 	= 0;
	Reference.NeedFindAxisSign.Axis16 	= 0;
	Reference.NeedFindAxisSign.Axis17 	= 0;
	Reference.NeedFindAxisSign.Axis18 	= 0;
	Reference.NeedFindAxisSign.Axis19 	= 0;
	Reference.NeedFindAxisSign.Axis20 	= 0;
	Reference.NeedFindAxisSign.Axis21 	= 0;
	Reference.NeedFindAxisSign.Axis22 	= 0;
	Reference.NeedFindAxisSign.Axis23 	= 0;

	Reference.FindRefSign.Axis1 = 0;
	Reference.FindRefSign.Axis2 = 0;
	Reference.FindRefSign.Axis3 = 0;
	Reference.FindRefSign.Axis4 = 0;
	Reference.FindRefSign.Axis5 = 0;
	Reference.FindRefSign.Axis6 = 0;
	Reference.FindRefSign.Axis7 = 0;
	Reference.FindRefSign.Axis8 = 0;	
	Reference.FindRefSign.Axis9 = 0;
	Reference.FindRefSign.Axis10 = 0;
	Reference.FindRefSign.Axis11 = 0;
	Reference.FindRefSign.Axis12 = 0;
	Reference.FindRefSign.Axis13 = 0;
	Reference.FindRefSign.Axis14 = 0;
	Reference.FindRefSign.Axis15 = 0;
	Reference.FindRefSign.Axis16 = 0;
	Reference.FindRefSign.Axis17 = 0;
	Reference.FindRefSign.Axis18 = 0;	
	Reference.FindRefSign.Axis19 = 0;
	Reference.FindRefSign.Axis20 = 0;
	Reference.FindRefSign.Axis21 = 0;
	Reference.FindRefSign.Axis22 = 0;
	Reference.FindRefSign.Axis23 = 0;
	
	Reference.RefStep		= 0;
	
	OverallSign.Int4IdleSign_1 = 0;
	OverallSign.ParameterReadCompleteSign = 0;
	OverallSign.JOGDistance  = 0;	
	OverallSign.JOGFeedrate = 0;
	OverallSign.JOGAxis = 0;
	OverallSign.JOGDir = 0;
	OverallSign.LEDFlashCtr1 = 0;
	OverallSign.LEDFlashCtr2 = 0;
	OverallSign.LEDFlashTime1 = 100;
	OverallSign.LEDFlashTime2 = 100;

	OverallSign.CameraNeedCaptureSign = 0;
	OverallSign.CameraScanSign = 0;
	OverallSign.CameraStaticCaptureSign= 0;
	OverallSign.MicroEScanSign = 0;
	OverallSign.CameraSoftTriggerLedFlash = 0;
	OverallSign.OpticalFlashSign = 0;

	OverallSign.MicroEDataBaseStartCnt = 0;
	OverallSign.MicroEDataBaseEndCnt = 0;
	OverallSign.MicroEDataBaseTotalCnt = 0;
	psEepromPara_Arm->Macro[0] = 0;//OverallSign.MicroEDataBaseStartCnt = OverallSign.MicroEDataBaseEndCnt;
	psEepromPara_Arm->Macro[1] = 0;
	psEepromPara_Arm->Macro[2] = 0;
	psDSPPacketOut->PacketCheckCodeBuf[0] = 0;
	memset(psDSPPacketOut->PacketCheckCodeBuf,0,sizeof(psDSPPacketOut->PacketCheckCodeBuf));
	
	psDSPPacketOut->MicroEDataBaseStartCnt = 0;;
	MonitorBuffer2Cnt = 0;
	LensCompensationBufferCnt = 0;

	ABSENCODER_SEN_Register.all = 0xFFFF;
	Write_ABSENCODER_SEN();	
	psSHMEvent->AbsCoordinateInitial = 1;

	NC_AREAControl.OldValue.Axis2 = 0;  
	NC_AREAControl.OldValue.Axis10 = 0;
	
	System.Gap.Axis1 = 0;
	System.Gap.Axis2 = 0;
	System.Gap.Axis3 = 0;
	System.Gap.Axis4 = 0;
	System.Gap.Axis5 = 0;
	System.Gap.Axis6 = 0;
	System.Gap.Axis7 = 0;
	System.Gap.Axis8 = 0;
	System.Gap.Axis9 = 0;
	System.Gap.Axis10 = 0;
	System.Gap.Axis11 = 0;
	System.Gap.Axis12 = 0;
	System.Gap.Axis13 = 0;
	System.Gap.Axis14 = 0;
	System.Gap.Axis15 = 0;
	System.Gap.Axis16 = 0;
	System.Gap.Axis17 = 0;
	System.Gap.Axis18 = 0;
	System.Gap.Axis19 = 0;
	System.Gap.Axis20 = 0;
	System.Gap.Axis21 = 0;
	System.Gap.Axis22 = 0;
	System.Gap.Axis23 = 0;
	System.Gap.Axis24 = 0;
	System.Gap.Axis25 = 0;

	System.GapPulse.Axis1 = 0;
	System.GapPulse.Axis2 = 0;
	System.GapPulse.Axis3 = 0;
	System.GapPulse.Axis4 = 0;
	System.GapPulse.Axis5 = 0;
	System.GapPulse.Axis6 = 0;
	System.GapPulse.Axis7 = 0;
	System.GapPulse.Axis8 = 0;
	System.GapPulse.Axis9 = 0;
	System.GapPulse.Axis10 = 0;
	System.GapPulse.Axis11 = 0;
	System.GapPulse.Axis12 = 0;
	System.GapPulse.Axis13 = 0;
	System.GapPulse.Axis14 = 0;
	System.GapPulse.Axis15 = 0;
	System.GapPulse.Axis16 = 0;
	System.GapPulse.Axis17 = 0;
	System.GapPulse.Axis18 = 0;
	System.GapPulse.Axis19 = 0;
	System.GapPulse.Axis20 = 0;
	System.GapPulse.Axis21 = 0;
	System.GapPulse.Axis22 = 0;
	System.GapPulse.Axis23 = 0;
	System.GapPulse.Axis24 = 0;
	System.GapPulse.Axis25 = 0;

	System.AxisCompMaxL.Axis1 = 2;
	System.AxisCompMaxL.Axis2 = 2;
	System.AxisCompMaxL.Axis3 = 2;
	System.AxisCompMaxL.Axis4 = 2;
	System.AxisCompMaxL.Axis5 = 2;
	System.AxisCompMaxL.Axis6 = 2;
	System.AxisCompMaxL.Axis7 = 2;
	System.AxisCompMaxL.Axis8 = 2;
	System.AxisCompMaxL.Axis9 = 2;
	System.AxisCompMaxL.Axis10 = 2;
	System.AxisCompMaxL.Axis11 = 2;
	System.AxisCompMaxL.Axis12 = 2;
	System.AxisCompMaxL.Axis13 = 2;
	System.AxisCompMaxL.Axis14 = 2;
	System.AxisCompMaxL.Axis15 = 2;
	System.AxisCompMaxL.Axis16 = 2;
	System.AxisCompMaxL.Axis17 = 2;
	System.AxisCompMaxL.Axis18 = 2;
	System.AxisCompMaxL.Axis19 = 2;
	System.AxisCompMaxL.Axis20 = 2;
	System.AxisCompMaxL.Axis21 = 2;
	System.AxisCompMaxL.Axis22 = 2;
	System.AxisCompMaxL.Axis23 = 2;
	System.AxisCompMaxL.Axis24 = 2;
	System.AxisCompMaxL.Axis25 = 2;

	#if 0
	//for test compensation and gap
	System.Gap.Axis15 = Gap_Axis15;
	System.Gap.Axis16 = 3;
	System.GapPulse.Axis15 = Gap_Axis15;
	System.GapPulse.Axis16 = 3;
	System.AxisCompMaxL.Axis15 = 22;
	System.AxisCompMaxL.Axis16 = 27;
	#endif

	DeltaStepAxis19_Bak = 0;
	DeltaStepAxis19_CPS = 0;
	OverallSign.LensMotionOverSign = LensMotionNormal;

	
	#if 0
		//for athena
		//DA total 2 channel(0~5V)
		//Channel 2 for LED flash light
		//Channel 3 for Bright field light
		Voltage1_Initial = divdp(230,500.0)*1023;
		Write_DA_CONVERT_CONTROL(Voltage1_Initial,2,0);//Channel 2=>Beta machine LTM8042 LED flash light power supply(initial 2.3V)
		Voltage2_Initial = Voltage1_Initial;
		Write_DA_CONVERT_CONTROL(Voltage2_Initial,3,0);//Channel 3=>Beta machine LTM8042 Bright field light power supply(initial 2.3V)
	#else
		//for Light sheet
		//DA1 total 4 channel(0~10V)
		//Channel 1 for LED flash light
		//Channel 2 for Bright field light
		//Channel 3 for AOTF
		//Channel 4 for Pockels Cell

		//DA2 total 4 channel(0~10V)
		//Channel (5)1 for Galvanometer 1
		//Channel (6)2 for Galvanometer 2
		//Channel (7)3 for Galvanometer 3
		//Channel (8)4 for Galvanometer 4

		//DA3 total 1 channel(0~5V)
		//Channel (9)1 for PMT measure
	
		//Initial 0V
		Write_DA_CONVERT_CONTROL(0,0x0f,0);
		//Initial 5V
		//4 channel Galvanometer all initial to 5V in middle position(0~10V)
		Voltage1_Initial = divdp(250,500.0)*1023;
		DA_Convert1.DA_Data = Voltage1_Initial;
		DA_Convert1.AutoSign = 0;
		DA_Convert1.DA_CS = 0;
		DA_Convert1.DA_ADDR = 0x0f;
		Write_DA_CONVERT_CONTROL_2(&DA_Convert1);	

		//Initial 0V
		Write_DA_CONVERT_CONTROL(0,0x0f,0);
		//Initial 5V
		Voltage1_Initial = divdp(250,500.0)*1023;
		Write_DA_CONVERT_CONTROL_2(&DA_Convert1);	
	#endif

	COM232_2();//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)
	delay(1);
	COM232_1();//All machine Fluorescence control
}

void STD_Initial(void)	
{
	STDNCSign.F = divdp( 2000, 60.0 );
	//initial communication
	tsCODE_G00 *psCodeInfo;
	psCodeInfo = (tsCODE_G00 *) &psDSPCodeSTD->CodeBuf[0];
	psCodeInfo->SendCountD = 0; 
	
	STDNCSign.RealRunCount = 0;
	STDNCSign.RealSendCount = 0;
	STDNCSign.LastRealSendCount = 0;
	STDNCSign.ResendGcodeSign = 0;
	OverallSign.NCSign = STDCODERUN;	
	get_ABS_coordinate();
	OverallSign.STDLastMoveGCode.EndPoint = OverallSign.ABS_Coordinate;		
	OverallSign.LastMoveCodeEndVector.Axis1 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis2 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis3 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis4 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis5 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis6 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis7 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis8 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis9 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis10 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis11 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis12 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis13 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis14 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis15 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis16 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis17 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis18 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis19 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis20 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis21 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis22 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis23 = 0.57735;

	//initial NC_AREAControl
	NCRunSign.NewAreaSign	= 0;
	NCRunSign.RealSendCount = 0;
	NCRunSign.RealRunCount	= 0;
	NC_AREAControl.Pointer_EndCode = 0;
	NC_AREAControl.VNow_Module.Module1 = 0;
	NC_AREAControl.VNow_Module.Module2 = 0;
	NC_AREAControl.VNow_Module.Module3 = 0;
	NC_AREAControl.VNow_Module.Module4 = 0;
	NC_AREAControl.VNow_Module.Module5 = 0;
	NC_AREAControl.VNow_Module.Module6 = 0;
	NC_AREAControl.VNow_Module.Module7 = 0;
	NC_AREAControl.VNow_Module.Module8 = 0;
	NC_AREAControl.VNow_Module.Module9 = 0;
	NC_AREAControl.VNow_Module.Module10 = 0;
	NC_AREAControl.VNow_Module.Module11 = 0;
	NC_AREAControl.VNow_Module.Module12 = 0;
	NC_AREAControl.VMin_Module.Module1 = 1;
	NC_AREAControl.VMin_Module.Module2 = 1;
	NC_AREAControl.VMin_Module.Module3 = 1;
	NC_AREAControl.VMin_Module.Module4 = 1;
	NC_AREAControl.VMin_Module.Module5 = 1;
	NC_AREAControl.VMin_Module.Module6 = 1;
	NC_AREAControl.VMin_Module.Module7 = 1;
	NC_AREAControl.VMin_Module.Module8 = 1;
	NC_AREAControl.VMin_Module.Module9 = 1;
	NC_AREAControl.VMin_Module.Module10 = 1;
	NC_AREAControl.VMin_Module.Module11 = 1;
	NC_AREAControl.VMin_Module.Module12 = 1;
	NCRunSign.VeVmCalCount = 1;
	NCRunSign.EndCodeCount = 1;
	NCRunSign.EndCodeCount = 1;
	NC_AREAControl.Snow_Module.Module1 = 1;
	NC_AREAControl.Snow_Module.Module2 = 1;
	NC_AREAControl.Snow_Module.Module3 = 1;
	NC_AREAControl.Snow_Module.Module4 = 1;
	NC_AREAControl.Snow_Module.Module5 = 1;
	NC_AREAControl.Snow_Module.Module6 = 1;
	NC_AREAControl.Snow_Module.Module7 = 1;	
	NC_AREAControl.Snow_Module.Module8 = 1;
	NC_AREAControl.Snow_Module.Module9 = 1;
	NC_AREAControl.Snow_Module.Module10 = 1;
	NC_AREAControl.Snow_Module.Module11 = 1;
	NC_AREAControl.Snow_Module.Module12 = 1;	
	NC_AREAControl.Pointer_StartCode = 0;
	NC_AREAControl.Pointer_EndCode = 0;
	NC_AREAControl.DeltaStep.Axis1 =0;
	NC_AREAControl.DeltaStep.Axis2 =0;
	NC_AREAControl.DeltaStep.Axis3 =0;        
	NC_AREAControl.DeltaStep.Axis4 =0;
	NC_AREAControl.DeltaStep.Axis5 =0;
	NC_AREAControl.DeltaStep.Axis6 =0;    
	NC_AREAControl.DeltaStep.Axis7 =0;
	NC_AREAControl.DeltaStep.Axis8 =0;
	NC_AREAControl.DeltaStep.Axis9 =0;    
	NC_AREAControl.DeltaStep.Axis10 =0;
	NC_AREAControl.DeltaStep.Axis11 =0;
	NC_AREAControl.DeltaStep.Axis12 =0;
	NC_AREAControl.DeltaStep.Axis13 =0;        
	NC_AREAControl.DeltaStep.Axis14 =0;
	NC_AREAControl.DeltaStep.Axis15 =0;
	NC_AREAControl.DeltaStep.Axis16 =0;    
	NC_AREAControl.DeltaStep.Axis17 =0;
	NC_AREAControl.DeltaStep.Axis18 =0;
	NC_AREAControl.DeltaStep.Axis19 =0;    
	NC_AREAControl.DeltaStep.Axis20 =0;
	NC_AREAControl.DeltaStep.Axis21 =0;
	NC_AREAControl.DeltaStep.Axis22 =0;
	NC_AREAControl.DeltaStep.Axis23 =0;        
	
	NC_AREAControl.OldValue.Axis1 = 0;                         
	//NC_AREAControl.OldValue.Axis2 = 0;                         
	NC_AREAControl.OldValue.Axis3 = 0; 
	NC_AREAControl.OldValue.Axis4 = 0;                         
	NC_AREAControl.OldValue.Axis5 = 0;                         
	NC_AREAControl.OldValue.Axis6 = 0; 
	NC_AREAControl.OldValue.Axis7 = 0;                         
	NC_AREAControl.OldValue.Axis8 = 0;                         
	NC_AREAControl.OldValue.Axis9 = 0; 
	//NC_AREAControl.OldValue.Axis10 = 0;                         
	NC_AREAControl.OldValue.Axis11 = 0;                         
	NC_AREAControl.OldValue.Axis12 = 0;                         
	NC_AREAControl.OldValue.Axis13 = 0; 
	NC_AREAControl.OldValue.Axis14 = 0;                         
	NC_AREAControl.OldValue.Axis15 = 0;                         
	NC_AREAControl.OldValue.Axis16 = 0; 
	NC_AREAControl.OldValue.Axis17 = 0;                         
	NC_AREAControl.OldValue.Axis18 = 0;                         
	NC_AREAControl.OldValue.Axis19 = 0; 
	NC_AREAControl.OldValue.Axis20 = 0;                         
	NC_AREAControl.OldValue.Axis21 = 0;                         
	NC_AREAControl.OldValue.Axis22 = 0;                         
	NC_AREAControl.OldValue.Axis23 = 0; 

	NC_AREAControl.a_ac_Module.Module1 = 2000;
	NC_AREAControl.a_ac_Module.Module2 = 2000;
	NC_AREAControl.a_ac_Module.Module3 = 2000;	
	NC_AREAControl.a_ac_Module.Module4 = 2000;
	NC_AREAControl.a_ac_Module.Module5 = 2000;
	NC_AREAControl.a_ac_Module.Module6 = 2000;
	NC_AREAControl.a_ac_Module.Module7 = 2000;
	NC_AREAControl.a_ac_Module.Module8 = 300;	
	NC_AREAControl.a_ac_Module.Module9 = 300;
	NC_AREAControl.a_ac_Module.Module10 = 2000;
	NC_AREAControl.a_ac_Module.Module11 = 2000;
	NC_AREAControl.a_ac_Module.Module12 = 2000;

	NC_AREAControl.a_de_Module.Module1 = 2000;
	NC_AREAControl.a_de_Module.Module2 = 2000;
	NC_AREAControl.a_de_Module.Module3 = 2000;
	NC_AREAControl.a_de_Module.Module4 = 2000;
	NC_AREAControl.a_de_Module.Module5 = 2000;
	NC_AREAControl.a_de_Module.Module6 = 2000;
	NC_AREAControl.a_de_Module.Module7 = 2000;
	NC_AREAControl.a_de_Module.Module8 = 300;
	NC_AREAControl.a_de_Module.Module9 = 300;
	NC_AREAControl.a_de_Module.Module10 = 2000;
	NC_AREAControl.a_de_Module.Module11 = 2000;
	NC_AREAControl.a_de_Module.Module12 = 2000;
	
	NC_AREAControl.OldPointMACHPulseOut.Axis1 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis2 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis3 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis4 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis5 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis6 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis7 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis8 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis9 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis10 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis11 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis12 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis13 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis14 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis15 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis16 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis17 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis18 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis19 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis20 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis21 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis22 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis23 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis24 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis25 = 0;

	NC_AREAControl.Vm_Module.Module1 = 0;
	NC_AREAControl.Vm_Module.Module2 = 0;
	NC_AREAControl.Vm_Module.Module3 = 0;
	NC_AREAControl.Vm_Module.Module4 = 0;
	NC_AREAControl.Vm_Module.Module5 = 0;
	NC_AREAControl.Vm_Module.Module6 = 0;
	NC_AREAControl.Vm_Module.Module7 = 0;
	NC_AREAControl.Vm_Module.Module8 = 0;
	NC_AREAControl.Vm_Module.Module9 = 0;
	NC_AREAControl.Vm_Module.Module10 = 0;
	NC_AREAControl.Vm_Module.Module11 = 0;
	NC_AREAControl.Vm_Module.Module12 = 0;

	NC_AREAControl.Ve_Module.Module1 = 0;
	NC_AREAControl.Ve_Module.Module2 = 0;
	NC_AREAControl.Ve_Module.Module3 = 0;
	NC_AREAControl.Ve_Module.Module4 = 0;
	NC_AREAControl.Ve_Module.Module5 = 0;
	NC_AREAControl.Ve_Module.Module6 = 0;
	NC_AREAControl.Ve_Module.Module7 = 0;
	NC_AREAControl.Ve_Module.Module8 = 0;
	NC_AREAControl.Ve_Module.Module9 = 0;
	NC_AREAControl.Ve_Module.Module10 = 0;
	NC_AREAControl.Ve_Module.Module11 = 0;
	NC_AREAControl.Ve_Module.Module12 = 0;
	
	NC_AREAControl.Vs_Module.Module1 = 0;
	NC_AREAControl.Vs_Module.Module2 = 0;
	NC_AREAControl.Vs_Module.Module3 = 0;
	NC_AREAControl.Vs_Module.Module4 = 0;
	NC_AREAControl.Vs_Module.Module5 = 0;
	NC_AREAControl.Vs_Module.Module6 = 0;
	NC_AREAControl.Vs_Module.Module7 = 0;
	NC_AREAControl.Vs_Module.Module8 = 0;
	NC_AREAControl.Vs_Module.Module9 = 0;
	NC_AREAControl.Vs_Module.Module10 = 0;
	NC_AREAControl.Vs_Module.Module11 = 0;
	NC_AREAControl.Vs_Module.Module12 = 0;

	NC_AREAControl.VmReal_Module.Module1 = 0;
	NC_AREAControl.VmReal_Module.Module2 = 0;
	NC_AREAControl.VmReal_Module.Module3 = 0;
	NC_AREAControl.VmReal_Module.Module4 = 0;
	NC_AREAControl.VmReal_Module.Module5 = 0;
	NC_AREAControl.VmReal_Module.Module6 = 0;
	NC_AREAControl.VmReal_Module.Module7 = 0;
	NC_AREAControl.VmReal_Module.Module8 = 0;
	NC_AREAControl.VmReal_Module.Module9 = 0;
	NC_AREAControl.VmReal_Module.Module10 = 0;
	NC_AREAControl.VmReal_Module.Module11 = 0;
	NC_AREAControl.VmReal_Module.Module12 = 0;	
	
	InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
	OverallSign.InterpolationSendCnt_1 = 0;
	OverallSign.InterpolationRunCnt_1 = 0;
	InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
	OverallSign.InterpolationSendCnt_2 = 0;
	OverallSign.InterpolationRunCnt_2 = 0;

	
	//initial STDNCSign
	STDNCSign.CodeRun_Over = 1;
	STDNCSign.CodeRun_Over_Module.Module1 = 1;               
	STDNCSign.CodeRun_Over_Module.Module2 = 1; 
	STDNCSign.CodeRun_Over_Module.Module3 = 1; 
	STDNCSign.CodeRun_Over_Module.Module4 = 1; 
	STDNCSign.CodeRun_Over_Module.Module5 = 1; 
	STDNCSign.CodeRun_Over_Module.Module6 = 1; 
	STDNCSign.CodeRun_Over_Module.Module7 = 1; 
	STDNCSign.CodeRun_Over_Module.Module8 = 1; 
	STDNCSign.CodeRun_Over_Module.Module9 = 1; 
	STDNCSign.CodeRun_Over_Module.Module10 = 1; 
	STDNCSign.CodeRun_Over_Module.Module11 = 1; 
	STDNCSign.CodeRun_Over_Module.Module12 = 1; 

	//单段功能初妾??	STDNCSign.SingleModeSign = 0;
	STDNCSign.GetPositionSign = 1;
	Timer0.GetPositionCounter = Timer0.GenericCounter;
	OverallSign.ParameterReadCompleteSign = 0;

	OverallSign.LEDFlashCtr1 = 0;
	OverallSign.LEDFlashCtr2 = 0;
	OverallSign.LEDFlashTime1 = 100;
	OverallSign.LEDFlashTime2 = 100;
	
	OverallSign.LensMotionOverSign = LensMotionNormal;
}


void SPC_Initial(void)	
{	
	SPCNCSign.F = divdp( 2000, 60.0 );
	psDSPCodeSPC->sInfoBuf[0].CountID = 0; 
	SPCNCSign.RealRunCount = 0;		//initial communication
	SPCNCSign.RealSendCount = 0;
	SPCNCSign.LastRealSendCount = 0;
	
	OverallSign.NCSign = SPCCODERUN;
	get_ABS_coordinate();
	OverallSign.STDLastMoveGCode.EndPoint = OverallSign.ABS_Coordinate;		
	OverallSign.LastMoveCodeEndVector.Axis1 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis2 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis3 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis4 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis5 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis6 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis7 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis8 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis9 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis10 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis11 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis12 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis13 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis14 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis15 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis16 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis17 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis18 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis19 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis20 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis21 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis22 = 0.57735;
	OverallSign.LastMoveCodeEndVector.Axis23 = 0.57735;

	//initial NC_AREAControl
	NCRunSign.NewAreaSign	= 0;
	NCRunSign.RealSendCount = 0;
	NCRunSign.RealRunCount	= 0;
	NC_AREAControl.Pointer_EndCode = 0;
	NC_AREAControl.VNow_Module.Module1 = 0;
	NC_AREAControl.VNow_Module.Module2 = 0;
	NC_AREAControl.VNow_Module.Module3 = 0;
	NC_AREAControl.VNow_Module.Module4 = 0;
	NC_AREAControl.VNow_Module.Module5 = 0;
	NC_AREAControl.VNow_Module.Module6 = 0;
	NC_AREAControl.VNow_Module.Module7 = 0;
	NC_AREAControl.VNow_Module.Module8 = 0;
	NC_AREAControl.VNow_Module.Module9 = 0;
	NC_AREAControl.VNow_Module.Module10 = 0;
	NC_AREAControl.VNow_Module.Module11 = 0;
	NC_AREAControl.VNow_Module.Module12 = 0;
	NC_AREAControl.VMin_Module.Module1 = 1;
	NC_AREAControl.VMin_Module.Module2 = 1;
	NC_AREAControl.VMin_Module.Module3 = 1;
	NC_AREAControl.VMin_Module.Module4 = 1;
	NC_AREAControl.VMin_Module.Module5 = 1;
	NC_AREAControl.VMin_Module.Module6 = 1;
	NC_AREAControl.VMin_Module.Module7 = 1;
	NC_AREAControl.VMin_Module.Module8 = 1;
	NC_AREAControl.VMin_Module.Module9 = 1;
	NC_AREAControl.VMin_Module.Module10 = 1;
	NC_AREAControl.VMin_Module.Module11 = 1;
	NC_AREAControl.VMin_Module.Module12 = 1;
	NCRunSign.VeVmCalCount = 1;
	NCRunSign.EndCodeCount = 1;
	NCRunSign.EndCodeCount = 1;
	NC_AREAControl.Snow_Module.Module1 = 1;
	NC_AREAControl.Snow_Module.Module2 = 1;
	NC_AREAControl.Snow_Module.Module3 = 1;
	NC_AREAControl.Snow_Module.Module4 = 1;
	NC_AREAControl.Snow_Module.Module5 = 1;
	NC_AREAControl.Snow_Module.Module6 = 1;
	NC_AREAControl.Snow_Module.Module7 = 1;	
	NC_AREAControl.Snow_Module.Module8 = 1;
	NC_AREAControl.Snow_Module.Module9 = 1;
	NC_AREAControl.Snow_Module.Module10 = 1;
	NC_AREAControl.Snow_Module.Module11 = 1;
	NC_AREAControl.Snow_Module.Module12 = 1;	
	NC_AREAControl.Pointer_StartCode = 0;
	NC_AREAControl.Pointer_EndCode = 0;
	NC_AREAControl.DeltaStep.Axis1 =0;
	NC_AREAControl.DeltaStep.Axis2 =0;
	NC_AREAControl.DeltaStep.Axis3 =0;        
	NC_AREAControl.DeltaStep.Axis4 =0;
	NC_AREAControl.DeltaStep.Axis5 =0;
	NC_AREAControl.DeltaStep.Axis6 =0;    
	NC_AREAControl.DeltaStep.Axis7 =0;
	NC_AREAControl.DeltaStep.Axis8 =0;
	NC_AREAControl.DeltaStep.Axis9 =0;    
	NC_AREAControl.DeltaStep.Axis10 =0;
	NC_AREAControl.DeltaStep.Axis11 =0;
	NC_AREAControl.DeltaStep.Axis12 =0;
	NC_AREAControl.DeltaStep.Axis13 =0;        
	NC_AREAControl.DeltaStep.Axis14 =0;
	NC_AREAControl.DeltaStep.Axis15 =0;
	NC_AREAControl.DeltaStep.Axis16 =0;    
	NC_AREAControl.DeltaStep.Axis17 =0;
	NC_AREAControl.DeltaStep.Axis18 =0;
	NC_AREAControl.DeltaStep.Axis19 =0;    
	NC_AREAControl.DeltaStep.Axis20 =0;
	NC_AREAControl.DeltaStep.Axis21 =0;
	NC_AREAControl.DeltaStep.Axis22 =0;
	NC_AREAControl.DeltaStep.Axis23 =0;        
	
	NC_AREAControl.OldValue.Axis1 = 0;                         
	//NC_AREAControl.OldValue.Axis2 = 0;                         
	NC_AREAControl.OldValue.Axis3 = 0; 
	NC_AREAControl.OldValue.Axis4 = 0;                         
	NC_AREAControl.OldValue.Axis5 = 0;                         
	NC_AREAControl.OldValue.Axis6 = 0; 
	NC_AREAControl.OldValue.Axis7 = 0;                         
	NC_AREAControl.OldValue.Axis8 = 0;                         
	NC_AREAControl.OldValue.Axis9 = 0; 
	//NC_AREAControl.OldValue.Axis10 = 0;                         
	NC_AREAControl.OldValue.Axis11 = 0;                         
	NC_AREAControl.OldValue.Axis12 = 0;                         
	NC_AREAControl.OldValue.Axis13 = 0; 
	NC_AREAControl.OldValue.Axis14 = 0;                         
	NC_AREAControl.OldValue.Axis15 = 0;                         
	NC_AREAControl.OldValue.Axis16 = 0; 
	NC_AREAControl.OldValue.Axis17 = 0;                         
	NC_AREAControl.OldValue.Axis18 = 0;                         
	NC_AREAControl.OldValue.Axis19 = 0; 
	NC_AREAControl.OldValue.Axis20 = 0;                         
	NC_AREAControl.OldValue.Axis21 = 0;                         
	NC_AREAControl.OldValue.Axis22 = 0;                         
	NC_AREAControl.OldValue.Axis23 = 0; 

	NC_AREAControl.a_ac_Module.Module1 = 2000;
	NC_AREAControl.a_ac_Module.Module2 = 2000;
	NC_AREAControl.a_ac_Module.Module3 = 2000;	
	NC_AREAControl.a_ac_Module.Module4 = 2000;
	NC_AREAControl.a_ac_Module.Module5 = 2000;
	NC_AREAControl.a_ac_Module.Module6 = 2000;
	NC_AREAControl.a_ac_Module.Module7 = 2000;
	NC_AREAControl.a_ac_Module.Module8 = 300;	
	NC_AREAControl.a_ac_Module.Module9 = 300;
	NC_AREAControl.a_ac_Module.Module10 = 2000;
	NC_AREAControl.a_ac_Module.Module11 = 2000;
	NC_AREAControl.a_ac_Module.Module12 = 2000;

	NC_AREAControl.a_de_Module.Module1 = 2000;
	NC_AREAControl.a_de_Module.Module2 = 2000;
	NC_AREAControl.a_de_Module.Module3 = 2000;
	NC_AREAControl.a_de_Module.Module4 = 2000;
	NC_AREAControl.a_de_Module.Module5 = 2000;
	NC_AREAControl.a_de_Module.Module6 = 2000;
	NC_AREAControl.a_de_Module.Module7 = 2000;
	NC_AREAControl.a_de_Module.Module8 = 300;
	NC_AREAControl.a_de_Module.Module9 = 300;
	NC_AREAControl.a_de_Module.Module10 = 2000;
	NC_AREAControl.a_de_Module.Module11 = 2000;
	NC_AREAControl.a_de_Module.Module12 = 2000;
	
	NC_AREAControl.OldPointMACHPulseOut.Axis1 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis2 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis3 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis4 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis5 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis6 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis7 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis8 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis9 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis10 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis11 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis12 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis13 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis14 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis15 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis16 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis17 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis18 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis19 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis20 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis21 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis22 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis23 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis24 = 0;
	NC_AREAControl.OldPointMACHPulseOut.Axis25 = 0;

	NC_AREAControl.Vm_Module.Module1 = 0;
	NC_AREAControl.Vm_Module.Module2 = 0;
	NC_AREAControl.Vm_Module.Module3 = 0;
	NC_AREAControl.Vm_Module.Module4 = 0;
	NC_AREAControl.Vm_Module.Module5 = 0;
	NC_AREAControl.Vm_Module.Module6 = 0;
	NC_AREAControl.Vm_Module.Module7 = 0;
	NC_AREAControl.Vm_Module.Module8 = 0;
	NC_AREAControl.Vm_Module.Module9 = 0;
	NC_AREAControl.Vm_Module.Module10 = 0;
	NC_AREAControl.Vm_Module.Module11 = 0;
	NC_AREAControl.Vm_Module.Module12 = 0;

	NC_AREAControl.Ve_Module.Module1 = 0;
	NC_AREAControl.Ve_Module.Module2 = 0;
	NC_AREAControl.Ve_Module.Module3 = 0;
	NC_AREAControl.Ve_Module.Module4 = 0;
	NC_AREAControl.Ve_Module.Module5 = 0;
	NC_AREAControl.Ve_Module.Module6 = 0;
	NC_AREAControl.Ve_Module.Module7 = 0;
	NC_AREAControl.Ve_Module.Module8 = 0;
	NC_AREAControl.Ve_Module.Module9 = 0;
	NC_AREAControl.Ve_Module.Module10 = 0;
	NC_AREAControl.Ve_Module.Module11 = 0;
	NC_AREAControl.Ve_Module.Module12 = 0;
	
	NC_AREAControl.Vs_Module.Module1 = 0;
	NC_AREAControl.Vs_Module.Module2 = 0;
	NC_AREAControl.Vs_Module.Module3 = 0;
	NC_AREAControl.Vs_Module.Module4 = 0;
	NC_AREAControl.Vs_Module.Module5 = 0;
	NC_AREAControl.Vs_Module.Module6 = 0;
	NC_AREAControl.Vs_Module.Module7 = 0;
	NC_AREAControl.Vs_Module.Module8 = 0;
	NC_AREAControl.Vs_Module.Module9 = 0;
	NC_AREAControl.Vs_Module.Module10 = 0;
	NC_AREAControl.Vs_Module.Module11 = 0;
	NC_AREAControl.Vs_Module.Module12 = 0;

	NC_AREAControl.VmReal_Module.Module1 = 0;
	NC_AREAControl.VmReal_Module.Module2 = 0;
	NC_AREAControl.VmReal_Module.Module3 = 0;
	NC_AREAControl.VmReal_Module.Module4 = 0;
	NC_AREAControl.VmReal_Module.Module5 = 0;
	NC_AREAControl.VmReal_Module.Module6 = 0;
	NC_AREAControl.VmReal_Module.Module7 = 0;
	NC_AREAControl.VmReal_Module.Module8 = 0;
	NC_AREAControl.VmReal_Module.Module9 = 0;
	NC_AREAControl.VmReal_Module.Module10 = 0;
	NC_AREAControl.VmReal_Module.Module11 = 0;
	NC_AREAControl.VmReal_Module.Module12 = 0;	
	
	InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
	OverallSign.InterpolationSendCnt_1 = 0;
	OverallSign.InterpolationRunCnt_1 = 0;
	InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
	OverallSign.InterpolationSendCnt_2 = 0;
	OverallSign.InterpolationRunCnt_2 = 0;
	
	//initial SPCNCSign
	SPCNCSign.CodeRun_Over = 1;
	SPCNCSign.CodeRun_Over_Module.Module1= 1;               
	SPCNCSign.CodeRun_Over_Module.Module2 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module3 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module4 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module5 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module6 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module7 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module8 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module9 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module10 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module11 = 1; 
	SPCNCSign.CodeRun_Over_Module.Module12 = 1; 	
	SPCNCSign.GetPositionSign = 1;		
	
	Timer0.GetPositionCounter = Timer0.GenericCounter;
//	OverallSign.ParameterReadCompleteSign = 0;

	OverallSign.LEDFlashCtr1 = 0;
	OverallSign.LEDFlashCtr2 = 0;
	OverallSign.LEDFlashTime1 = 100;
	OverallSign.LEDFlashTime2 = 100;
}

void System_Initial(void)		
{		
	//Servo_Encoder_Clear1_Out_Register.all = 0;
	//Write_Servo_Encoder_Clear1_Out();
	//Servo_Encoder_Clear2_Out_Register.all = 0;
	//Write_Servo_Encoder_Clear2_Out();
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= 1;
	Write_Servo_Sign1_Out();

	if(System.FunctionSelect05 == True)
	{	//for test Beta	
		Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_1= 0; 
	}
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_2= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_3= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_4= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_5= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_15= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_16= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_17= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_18= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_11= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_12= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_13= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_14= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	//Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_23= 0; 

	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_1 = 0;
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_2 = 0;
	Write_Servo_Encoder_Clear1_Out();

	if(System.FunctionSelect05 == True)
	{		
		Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_1= 1; 
	}
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_2= 1;  
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_3= 1;  
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_4= 1;  
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_5= 1;  
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_15= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_16= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_17= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_18= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_11= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_12= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_13= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_14= 1; 
	//Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_23= 1; 
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_1 = 1;
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_2 = 1;



	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_6 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_7 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_8 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_9 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_10 = 0;
	//Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_19 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_20 = 0;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_21 = 0;	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_22 = 0;
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_6 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_7 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_8 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_9 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_10 = 1;
	//Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_19 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_20 = 1;
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_21 = 1;	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_22 = 1;
	
	//CPLD_ServoOn();		
	//CPLD_ServoOff();	
	//Servo_Encoder_Clear1_Out_Register.all = 0xff;
	//Servo_Encoder_Clear2_Out_Register.all = 0xff;
	
	Init_Error();
	STD_Initial();
	SPC_Initial();

	OverallSign.NCSign = NOCODERUN;

	#if 1
	Dsp_Com_PacketIn_Initial();
	#endif

	#if 1
	psSHMEvent->PacketARM = 1;	//for test
	//psDSPPacketIn_InfoEvent->ServoOn = 1;
	psSHMEvent->AbsCoordinateInitial = 1;
	#endif

	OverallSign.ParameterReadCompleteSign = 1;
	Error.MainErrorSign = 0;
	
	OverallSign.CameraNeedCaptureSign = 0;
	OverallSign.CameraScanSign = 0;
	OverallSign.MicroEScanSign = 0;
	OverallSign.CameraSoftTriggerLedFlash = 0;
	OverallSign.CameraStaticCaptureSign= 0;
	OverallSign.OpticalFlashSign = 0;

	OverallSign.MicroEDataBaseStartCnt = 0;
	OverallSign.MicroEDataBaseEndCnt = 0;
	OverallSign.MicroEDataBaseTotalCnt = 0;
	OverallSign.LensMotionOverSign = LensMotionNormal;
	
	psEepromPara_Arm->Macro[0] = 0;//OverallSign.MicroEDataBaseStartCnt = OverallSign.MicroEDataBaseEndCnt;
	psEepromPara_Arm->Macro[1] = 0;
	psEepromPara_Arm->Macro[2] = 0;
	psDSPPacketOut->MicroEDataBaseStartCnt = 0;;
	LensCompensationBufferCnt = 0;

	MonitorBuffer1Cnt = 0;
	MonitorBuffer2Cnt = 0;
	MonitorBuffer3Cnt = 0;
	MonitorBuffer4Cnt = 0;
	PositionPointerCnt = 0;
	PositionPointerCnt_Bak = 0;
	DeltaStepAxis19_Bak = 0;
	DeltaStepAxis19_CPS = 0;

	if(System.FunctionSelect05 == False)
	{	//for test Alpha Machine
		//for test compensation and gap
		System.Gap.Axis15 = 1;
		System.Gap.Axis16 = 3;
		System.GapPulse.Axis15 = 1;
		System.GapPulse.Axis16 = 3;
		//System.Gap.Axis19 = 1;
		//System.GapPulse.Axis19 = 20;
		System.AxisCompMaxL.Axis15 = 22;
		System.AxisCompMaxL.Axis16 = 27;
	}
	else
	{	//for test Beta Machine
		//for test compensation and gap
		System.Gap.Axis15 = 0;
		System.Gap.Axis16 = 0;
		System.GapPulse.Axis15 = 0;
		System.GapPulse.Axis16 = 0;
		System.AxisCompMaxL.Axis15 = 2;
		System.AxisCompMaxL.Axis16 = 2;
	}
}

void Parameter_Initial(void)
{
	#if 0
	// test eeprom  FM25V10 read and write operation
	INT16U i,j;

	for(i=0;i<256;i++)
	{
		for(j=0;j<32*1024;j++)
		{
			sSHMAddress.pEepromARM[j]=j+i;
			//sSHMAddress.pEepromDSP[j]=j+i;	
		}
		//memset((INT8U *)sSHMAddress.pEepromARM,i,128*1024);
		memset((INT8U *)sSHMAddress.pEepromDSP,0,128*1024);
		SPI_Write(0, (INT8U *)sSHMAddress.pEepromARM, 128*1024);
		SPI_Read(0, (INT8U *)sSHMAddress.pEepromDSP, 128*1024);
		for(j=0;j<32*1024;j++)
		{
			if(sSHMAddress.pEepromDSP[j]!=sSHMAddress.pEepromARM[j])
			{
				j=32*1024;
			}
		}
			
	}	
	#endif

	#if 0		//comment 20150909 for test shareMemory
	SPI_Read(0, (INT8U *)sSHMAddress.pEepromDSP, 128*1024);
	memcpy((INT8U *)sSHMAddress.pEepromARM,(INT8U *)sSHMAddress.pEepromDSP,128*1024);
	#endif
	
	//Parameter_Initial_SHMDSP();
	Parameter_Initial_SHMDSP_2();
	
}

void Parameter_Initial_SHMDSP_2(void)
{	
	Int32	i,j;
	Uint32	StartAddress=0,Length=0;
	//Int8	data8;
	Int32	data32;
	Uint32	*UINT32UPointer1,*UINT32UPointer2;
	Int32	*INT32UPointer1,*INT32UPointer2;

	memset((INT8U *)sSHMAddress.pEepromDSP,0,128*1024);

	//for test
	StartAddress = EEPROMBUF_PARASIGN;
	Length = EEPROMIDS_PARASIGN/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];						
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

	StartAddress = EEPROMBUF_PARAARM_CFG;
	Length = EEPROMIDS_PARAARM_CFG;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];						
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

#if 0
	StartAddress = EEPROMBUF_PARAARM_CTRL;
	Length = EEPROMIDS_PARAARM_CTRL*4;
	UINT32UPointer1 = sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = 0;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}
#endif
	Parameter_Initial_SHMDSP_CTRL_2();

	StartAddress = EEPROMBUF_PARACOORD;
	Length = EEPROMIDS_PARACOORD;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

	StartAddress = EEPROMBUF_PARAMACRO;
	Length = EEPROMIDS_PARAMACRO;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

	StartAddress = EEPROMBUF_PARAALM;
	Length = EEPROMIDS_PARAALM/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

	StartAddress = EEPROMBUF_PARARUN;
	Length = EEPROMIDS_PARARUN/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}

	StartAddress = EEPROMBUF_PARACODE;
	Length = EEPROMIDS_PARACODE/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j; 
	}


	//psParaArm_Ctrl=(tsPARAARMCTRL_BUF *)psEepromPara_Dsp->ArmCTRL;
	//psEepromPara_Dsp=(tsEepromParaBuf *)sSHMAddress.pEepromDSP;

	memcpy((INT8U *)sSHMAddress.pEepromARM,(INT8U *)sSHMAddress.pEepromDSP,128*1024);
	
}



void Parameter_Initial_SHMDSP(void)
{	
	Int32	i,j;
	Uint32	StartAddress=0,Length=0;
	//Int8	data8;
	Int32	data32;
	Uint32	*UINT32UPointer1,*UINT32UPointer2;
	Int32	*INT32UPointer1,*INT32UPointer2;

	memset((INT8U *)sSHMAddress.pEepromDSP,0,128*1024);

	//for test
	StartAddress = EEPROMBUF_PARASIGN;
	Length = EEPROMIDS_PARASIGN/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];						
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	StartAddress = EEPROMBUF_PARAARM_CFG;
	Length = EEPROMIDS_PARAARM_CFG;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];						
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	#if 0
	StartAddress = EEPROMBUF_PARAARM_CTRL;
	Length = EEPROMIDS_PARAARM_CTRL*4;
	UINT32UPointer1 = sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = 0;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}
	#endif
	Parameter_Initial_SHMDSP_CTRL();

	StartAddress = EEPROMBUF_PARACOORD;
	Length = EEPROMIDS_PARACOORD;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	StartAddress = EEPROMBUF_PARAMACRO;
	Length = EEPROMIDS_PARAMACRO;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	StartAddress = EEPROMBUF_PARAALM;
	Length = EEPROMIDS_PARAALM/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	StartAddress = EEPROMBUF_PARARUN;
	Length = EEPROMIDS_PARARUN/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}

	StartAddress = EEPROMBUF_PARACODE;
	Length = EEPROMIDS_PARACODE/4;
	UINT32UPointer1 = &sSHMAddress.pEepromDSP[StartAddress];
	for(j=0;j<Length;j++)
	{	
		*UINT32UPointer1++ = j;
		//sSHMAddress.pEepromARM[j+StartAddress]=j;
		//sSHMAddress.pEepromDSP[j+StartAddress]=j;	
	}


	//psParaArm_Ctrl=(tsPARAARMCTRL_BUF *)psEepromPara_Dsp->ArmCTRL;
	//psEepromPara_Dsp=(tsEepromParaBuf *)sSHMAddress.pEepromDSP;

	memcpy((INT8U *)sSHMAddress.pEepromARM,(INT8U *)sSHMAddress.pEepromDSP,128*1024);
	
}

void Parameter_Initial_SHMDSP_CTRL_2(void)
{	
	psParaDsp_Ctrl->Tsample=10;	//psParaDsp_Ctrl->Tsample;					//0
	psParaDsp_Ctrl->ServoStationsSub=23;	//psParaDsp_Ctrl->ServoStationsSub;				//1
	psParaDsp_Ctrl->TrackRunOutRangeSQR=10000;	//psParaDsp_Ctrl->TrackRunOutRangeSQR;	//2
	//psParaDsp_Ctrl->a_SET=2000;	//psParaDsp_Ctrl->a_SET;
	psParaDsp_Ctrl->a_SET=2000;	//psParaDsp_Ctrl->a_SET;//3
	psParaDsp_Ctrl->a_SET_2=2000;	//psParaDsp_Ctrl->a_SET_2;					//4
	psParaDsp_Ctrl->VeerDeltaV=30;	//psParaDsp_Ctrl->VeerDeltaV;					//5	
	psParaDsp_Ctrl->NicetyVeerDeltaV=5;	//psParaDsp_Ctrl->NicetyVeerDeltaV;				//6
	psParaDsp_Ctrl->VeerDeltaT=30;	//psParaDsp_Ctrl->VeerDeltaT;					//7
	psParaDsp_Ctrl->NicetyVeerDeltaT=5;	//psParaDsp_Ctrl->NicetyVeerDeltaT;				//8
	psParaDsp_Ctrl->REFStopVariable=0;	//psParaDsp_Ctrl->REFStopVariable;			//9
	
	psParaDsp_Ctrl->EncoderCheckAxis=0xffff;	//psParaDsp_Ctrl->EncoderCheckAxis				//10	
	psParaDsp_Ctrl->RefDirAxis=0xffff;	//psParaDsp_Ctrl->RefDirAxis					//11
	psParaDsp_Ctrl->AxisUnitInLinear=1000;		//psParaDsp_Ctrl->AxisUnitInLinear				//12
	psParaDsp_Ctrl->AxisUnitOutLinear=1000;		//psParaDsp_Ctrl->AxisUnitOutLinear				//13	
	psParaDsp_Ctrl->G0Speed=6000;	//psParaDsp_Ctrl->G0Speed					//14
	psParaDsp_Ctrl->G0Speed_2=6000;	//psParaDsp_Ctrl->G0Speed_2					//15
	psParaDsp_Ctrl->G0Speed_Module1=100;	//psParaDsp_Ctrl->G0Speed_Module1		//16
	psParaDsp_Ctrl->G0Speed_Module2=100;	//psParaDsp_Ctrl->G0Speed_Module2		//17
	psParaDsp_Ctrl->G0Speed_Module3=100;	//psParaDsp_Ctrl->G0Speed_Module3		//18
	psParaDsp_Ctrl->G0Speed_Module4=100;	//psParaDsp_Ctrl->G0Speed_Module4		//19	

	psParaDsp_Ctrl->G0Speed_Module5=100;	//psParaDsp_Ctrl->G0Speed_Module5;		//20	
	psParaDsp_Ctrl->G0Speed_Module6=100;	//psParaDsp_Ctrl->G0Speed_Module6;		//21	
	psParaDsp_Ctrl->G0Speed_Module7=100;	//psParaDsp_Ctrl->G0Speed_Module7;		//22
	psParaDsp_Ctrl->G0Speed_Module8=100;	//psParaDsp_Ctrl->G0Speed_Module8;		//23
	psParaDsp_Ctrl->G0Speed_Module9=100;	//psParaDsp_Ctrl->G0Speed_Module9;		//24
	psParaDsp_Ctrl->G0Speed_Module10=100;	//psParaDsp_Ctrl->G0Speed_Module10;	//25	
	psParaDsp_Ctrl->G0Speed_Module11=100;	//psParaDsp_Ctrl->G0Speed_Module11;	//26	
	psParaDsp_Ctrl->G0Speed_Module12=100;	//psParaDsp_Ctrl->G0Speed_Module12;	//27	

	psParaDsp_Ctrl->G1Speed=600;	//psParaDsp_Ctrl->G1Speed					//28
	psParaDsp_Ctrl->G1Speed_2=600;	//psParaDsp_Ctrl->G1Speed_2					//29
	psParaDsp_Ctrl->G1Speed_Module1=10;	//psParaDsp_Ctrl->G1Speed_Module1;		//30
	psParaDsp_Ctrl->G1Speed_Module2=10;	//psParaDsp_Ctrl->G1Speed_Module2;		//31
	psParaDsp_Ctrl->G1Speed_Module3=10;	//psParaDsp_Ctrl->G1Speed_Module3;		//32	
	psParaDsp_Ctrl->G1Speed_Module4=10;	//psParaDsp_Ctrl->G1Speed_Module4;		//33	
	
	psParaDsp_Ctrl->G1Speed_Module5=10;		//psParaDsp_Ctrl->G1Speed_Module5;	//34			
	psParaDsp_Ctrl->G1Speed_Module6=10;		//psParaDsp_Ctrl->G1Speed_Module6;	//35		
	psParaDsp_Ctrl->G1Speed_Module7=10;		//psParaDsp_Ctrl->G1Speed_Module7;	//36			
	psParaDsp_Ctrl->G1Speed_Module8=10;		//psParaDsp_Ctrl->G1Speed_Module8;	//37		
	psParaDsp_Ctrl->G1Speed_Module9=10;		//psParaDsp_Ctrl->G1Speed_Module5;	//38			
	psParaDsp_Ctrl->G1Speed_Module10=10;		//psParaDsp_Ctrl->G1Speed_Module6;	//39		
	psParaDsp_Ctrl->G1Speed_Module11=10;		//psParaDsp_Ctrl->G1Speed_Module7;	//40		
	psParaDsp_Ctrl->G1Speed_Module12=10;		//psParaDsp_Ctrl->G1Speed_Module8;	//41		

	psParaDsp_Ctrl->SRefSpeed=2000;	//psParaDsp_Ctrl->SRefSpeed					//42
	psParaDsp_Ctrl->SRefSpeedBack=200;	//psParaDsp_Ctrl->SRefSpeedBack			//43	
	psParaDsp_Ctrl->SRefBack=20;	//psParaDsp_Ctrl->SRefBack						//44
	psParaDsp_Ctrl->SRefSpeed_2=400;	//psParaDsp_Ctrl->SRefSpeed_2			//45
	psParaDsp_Ctrl->SRefSpeedBack_2=50;	//psParaDsp_Ctrl->SRefSpeedBack_2		//46
	psParaDsp_Ctrl->SRefBack_2=1;	//psParaDsp_Ctrl->SRefBack_2					//47
	psParaDsp_Ctrl->Reserved1 = 0;					// 48
	psParaDsp_Ctrl->Reserved2 = 0;					// 49

	psParaDsp_Ctrl->MotorChangeDirAxis1=1;	//psParaDsp_Ctrl->MotorChangeDirAxis1;		//50
	psParaDsp_Ctrl->MotorChangeDirAxis2=0;	//psParaDsp_Ctrl->MotorChangeDirAxis2;		//51
	psParaDsp_Ctrl->MotorChangeDirAxis3=1;	//psParaDsp_Ctrl->MotorChangeDirAxis3;		//52
	psParaDsp_Ctrl->MotorChangeDirAxis4=1;	//psParaDsp_Ctrl->MotorChangeDirAxis4;		//53
	psParaDsp_Ctrl->MotorChangeDirAxis5=0;	//psParaDsp_Ctrl->MotorChangeDirAxis5;		//54
	psParaDsp_Ctrl->MotorChangeDirAxis6=1;	//psParaDsp_Ctrl->MotorChangeDirAxis6;		//55
	psParaDsp_Ctrl->MotorChangeDirAxis7=0;	//psParaDsp_Ctrl->MotorChangeDirAxis7;		//56
	psParaDsp_Ctrl->MotorChangeDirAxis8=0;	//psParaDsp_Ctrl->MotorChangeDirAxis8;		//57
	psParaDsp_Ctrl->MotorChangeDirAxis9=0;	//psParaDsp_Ctrl->MotorChangeDirAxis9;		//58
	psParaDsp_Ctrl->MotorChangeDirAxis10=0;	//psParaDsp_Ctrl->MotorChangeDirAxis10;		//59	
	psParaDsp_Ctrl->MotorChangeDirAxis11=0;	//psParaDsp_Ctrl->MotorChangeDirAxis11;		//60
	psParaDsp_Ctrl->MotorChangeDirAxis12=1;	//psParaDsp_Ctrl->MotorChangeDirAxis12;		//61	
	psParaDsp_Ctrl->MotorChangeDirAxis13=0;	//psParaDsp_Ctrl->MotorChangeDirAxis13;		//62
	psParaDsp_Ctrl->MotorChangeDirAxis14=0;	//psParaDsp_Ctrl->MotorChangeDirAxis14;		//63
	psParaDsp_Ctrl->MotorChangeDirAxis15=0;	//psParaDsp_Ctrl->MotorChangeDirAxis15;		//64
	psParaDsp_Ctrl->MotorChangeDirAxis16=0;	//psParaDsp_Ctrl->MotorChangeDirAxis16;		//65
	psParaDsp_Ctrl->MotorChangeDirAxis17=0;	//psParaDsp_Ctrl->MotorChangeDirAxis17;		//66
	psParaDsp_Ctrl->MotorChangeDirAxis18=0;	//psParaDsp_Ctrl->MotorChangeDirAxis18;		//67
	psParaDsp_Ctrl->MotorChangeDirAxis19=0;	//psParaDsp_Ctrl->MotorChangeDirAxis19;		//68
	psParaDsp_Ctrl->MotorChangeDirAxis20=0;	//psParaDsp_Ctrl->MotorChangeDirAxis20;		//69
	psParaDsp_Ctrl->MotorChangeDirAxis21=0;	//psParaDsp_Ctrl->MotorChangeDirAxis21;		//70
	psParaDsp_Ctrl->MotorChangeDirAxis22=0;	//psParaDsp_Ctrl->MotorChangeDirAxis22;		//71
	psParaDsp_Ctrl->MotorChangeDirAxis23=0;	//psParaDsp_Ctrl->MotorChangeDirAxis23;		//72
	psParaDsp_Ctrl->MotorChangeDirAxis24=0;	//psParaDsp_Ctrl->MotorChangeDirAxis24;		//73
	psParaDsp_Ctrl->MotorChangeDirAxis25=0;	//psParaDsp_Ctrl->MotorChangeDirAxis25;		//74
	
	psParaDsp_Ctrl->EncoderRDDir1=0;	//psParaDsp_Ctrl->EncoderRDDir1;		//75	
	psParaDsp_Ctrl->EncoderRDDir2=0;	//psParaDsp_Ctrl->EncoderRDDir2;		//76
	psParaDsp_Ctrl->EncoderRDDir3=0;	//psParaDsp_Ctrl->EncoderRDDir3;		//77
	psParaDsp_Ctrl->EncoderRDDir4=0;	//psParaDsp_Ctrl->EncoderRDDir4;		//78
	psParaDsp_Ctrl->EncoderRDDir5=0;	//psParaDsp_Ctrl->EncoderRDDir5;		//79
	psParaDsp_Ctrl->EncoderRDDir6=0;	//psParaDsp_Ctrl->EncoderRDDir6;		//80
	psParaDsp_Ctrl->EncoderRDDir7=1;	//psParaDsp_Ctrl->EncoderRDDir7;		//81
	psParaDsp_Ctrl->EncoderRDDir8=1;	//psParaDsp_Ctrl->EncoderRDDir8;		//82
	psParaDsp_Ctrl->EncoderRDDir9=0;	//psParaDsp_Ctrl->EncoderRDDir9;		//83
	psParaDsp_Ctrl->EncoderRDDir10=0;	//psParaDsp_Ctrl->EncoderRDDir10;		//84
	psParaDsp_Ctrl->EncoderRDDir11=1;	//psParaDsp_Ctrl->EncoderRDDir11;		//85
	psParaDsp_Ctrl->EncoderRDDir12=0;	//psParaDsp_Ctrl->EncoderRDDir12;		//86
	psParaDsp_Ctrl->EncoderRDDir13=0;	//psParaDsp_Ctrl->EncoderRDDir13;		//87
	psParaDsp_Ctrl->EncoderRDDir14=0;	//psParaDsp_Ctrl->EncoderRDDir14;		//88
	psParaDsp_Ctrl->EncoderRDDir15=0;	//psParaDsp_Ctrl->EncoderRDDir15;		//89	
	psParaDsp_Ctrl->EncoderRDDir16=0;	//psParaDsp_Ctrl->EncoderRDDir16;		//90
	psParaDsp_Ctrl->EncoderRDDir17=0;	//psParaDsp_Ctrl->EncoderRDDir17;		//91
	psParaDsp_Ctrl->EncoderRDDir18=0;	//psParaDsp_Ctrl->EncoderRDDir18;		//92
	psParaDsp_Ctrl->EncoderRDDir19=0;	//psParaDsp_Ctrl->EncoderRDDir19;		//93
	psParaDsp_Ctrl->EncoderRDDir20=0;	//psParaDsp_Ctrl->EncoderRDDir20;		//94
	psParaDsp_Ctrl->EncoderRDDir21=0;	//psParaDsp_Ctrl->EncoderRDDir21;		//95
	psParaDsp_Ctrl->EncoderRDDir22=0;	//psParaDsp_Ctrl->EncoderRDDir22;		//96
	psParaDsp_Ctrl->EncoderRDDir23=0;	//psParaDsp_Ctrl->EncoderRDDir23;		//97
	psParaDsp_Ctrl->EncoderRDDir24=0;	//psParaDsp_Ctrl->EncoderRDDir24;		//98
	psParaDsp_Ctrl->EncoderRDDir25=0;	//psParaDsp_Ctrl->EncoderRDDir25;		//99
	
	psParaDsp_Ctrl->AxisResolutionAxis1=1000;		//psParaDsp_Ctrl->AxisResolutionAxis1;	//100
	psParaDsp_Ctrl->AxisResolutionAxis2=1000;		//psParaDsp_Ctrl->AxisResolutionAxis2;	//101
	psParaDsp_Ctrl->AxisResolutionAxis3=1000;		//psParaDsp_Ctrl->AxisResolutionAxis3;	//102
	psParaDsp_Ctrl->AxisResolutionAxis4=1000;		//psParaDsp_Ctrl->AxisResolutionAxis4;	//103
	psParaDsp_Ctrl->AxisResolutionAxis5=1000;		//psParaDsp_Ctrl->AxisResolutionAxis5;	//104
	psParaDsp_Ctrl->AxisResolutionAxis6=1000;	//psParaDsp_Ctrl->AxisResolutionAxis6;	//105
	psParaDsp_Ctrl->AxisResolutionAxis7=1000;	//psParaDsp_Ctrl->AxisResolutionAxis7;	//106
	psParaDsp_Ctrl->AxisResolutionAxis8=1000;	//psParaDsp_Ctrl->AxisResolutionAxis8;	//107
	psParaDsp_Ctrl->AxisResolutionAxis9=1000;	//psParaDsp_Ctrl->AxisResolutionAxis9;	//108
	psParaDsp_Ctrl->AxisResolutionAxis10=1000;	//psParaDsp_Ctrl->AxisResolutionAxis10;		//109
	psParaDsp_Ctrl->AxisResolutionAxis11=1000;	//psParaDsp_Ctrl->AxisResolutionAxis11;		//110
	psParaDsp_Ctrl->AxisResolutionAxis12=1000;	//psParaDsp_Ctrl->AxisResolutionAxis12;		//111
	psParaDsp_Ctrl->AxisResolutionAxis13=1000;	//psParaDsp_Ctrl->AxisResolutionAxis13;		//112
	psParaDsp_Ctrl->AxisResolutionAxis14=1000;	//psParaDsp_Ctrl->AxisResolutionAxis14;		//113
	psParaDsp_Ctrl->AxisResolutionAxis15=800;	//psParaDsp_Ctrl->AxisResolutionAxis15;		//114
	psParaDsp_Ctrl->AxisResolutionAxis16=800;	//psParaDsp_Ctrl->AxisResolutionAxis16;		//115
	psParaDsp_Ctrl->AxisResolutionAxis17=800;	//psParaDsp_Ctrl->AxisResolutionAxis17;		//116
	psParaDsp_Ctrl->AxisResolutionAxis18=800;	//psParaDsp_Ctrl->AxisResolutionAxis18;		//117
	psParaDsp_Ctrl->AxisResolutionAxis19=1000;	//psParaDsp_Ctrl->AxisResolutionAxis19;		//118
	psParaDsp_Ctrl->AxisResolutionAxis20=800;	//psParaDsp_Ctrl->AxisResolutionAxis20;		//119	
	psParaDsp_Ctrl->AxisResolutionAxis21=800;		//psParaDsp_Ctrl->AxisResolutionAxis21;		//120
	psParaDsp_Ctrl->AxisResolutionAxis22=800;		//psParaDsp_Ctrl->AxisResolutionAxis22;		//121
	psParaDsp_Ctrl->AxisResolutionAxis23=1000;		//psParaDsp_Ctrl->AxisResolutionAxis23;		//122
	psParaDsp_Ctrl->AxisResolutionAxis24=1000;		//psParaDsp_Ctrl->AxisResolutionAxis24;		//123
	psParaDsp_Ctrl->AxisResolutionAxis25=1000;		//psParaDsp_Ctrl->AxisResolutionAxis25;		//124

	
	psParaDsp_Ctrl->CoordORGAxis1=0;		//psParaDsp_Ctrl->CoordORGAxis1;		//125
	psParaDsp_Ctrl->CoordORGAxis2=0;		//psParaDsp_Ctrl->CoordORGAxis2;		//126	
	psParaDsp_Ctrl->CoordORGAxis3=0;		//psParaDsp_Ctrl->CoordORGAxis3;		//127
	psParaDsp_Ctrl->CoordORGAxis4=0;		//psParaDsp_Ctrl->CoordORGAxis4;		//128
	psParaDsp_Ctrl->CoordORGAxis5=0;		//psParaDsp_Ctrl->CoordORGAxis5;		//129
	psParaDsp_Ctrl->CoordORGAxis6=0;		//psParaDsp_Ctrl->CoordORGAxis6;		//130
	psParaDsp_Ctrl->CoordORGAxis7=0;		//psParaDsp_Ctrl->CoordORGAxis7;		//131
	psParaDsp_Ctrl->CoordORGAxis8=0;		//psParaDsp_Ctrl->CoordORGAxis8;		//132	
	psParaDsp_Ctrl->CoordORGAxis9=0;		//psParaDsp_Ctrl->CoordORGAxis9;		//133
	psParaDsp_Ctrl->CoordORGAxis10=0;		//psParaDsp_Ctrl->CoordORGAxis10;		//134	
	psParaDsp_Ctrl->CoordORGAxis11=0;		//psParaDsp_Ctrl->CoordORGAxis11;		//135
	psParaDsp_Ctrl->CoordORGAxis12=0;		//psParaDsp_Ctrl->CoordORGAxis12;		//136
	psParaDsp_Ctrl->CoordORGAxis13=0;		//psParaDsp_Ctrl->CoordORGAxis13;		//137
	psParaDsp_Ctrl->CoordORGAxis14=0;		//psParaDsp_Ctrl->CoordORGAxis14;		//138
	psParaDsp_Ctrl->CoordORGAxis15=0;		//psParaDsp_Ctrl->CoordORGAxis15;		//139
	psParaDsp_Ctrl->CoordORGAxis16=0;		//psParaDsp_Ctrl->CoordORGAxis16;		//140
	psParaDsp_Ctrl->CoordORGAxis17=0;		//psParaDsp_Ctrl->CoordORGAxis17;		//141
	psParaDsp_Ctrl->CoordORGAxis18=0;		//psParaDsp_Ctrl->CoordORGAxis18;		//142
	psParaDsp_Ctrl->CoordORGAxis19=0;		//psParaDsp_Ctrl->CoordORGAxis19;		//143
	psParaDsp_Ctrl->CoordORGAxis20=0;		//psParaDsp_Ctrl->CoordORGAxis20;		//144
	psParaDsp_Ctrl->CoordORGAxis21=0;		//psParaDsp_Ctrl->CoordORGAxis21;		//145
	psParaDsp_Ctrl->CoordORGAxis22=0;		//psParaDsp_Ctrl->CoordORGAxis22;		//146
	psParaDsp_Ctrl->CoordORGAxis23=0;		//psParaDsp_Ctrl->CoordORGAxis23;		//147	
	psParaDsp_Ctrl->CoordORGAxis24=0;		//psParaDsp_Ctrl->CoordORGAxis24;		//148
	psParaDsp_Ctrl->CoordORGAxis25=0;		//psParaDsp_Ctrl->CoordORGAxis25;		//149
	
	psParaDsp_Ctrl->FeedMaxAxis1=30;		//psParaDsp_Ctrl->FeedMaxAxis1;			//150
	psParaDsp_Ctrl->FeedMaxAxis2=100;		//psParaDsp_Ctrl->FeedMaxAxis2;			//151
	psParaDsp_Ctrl->FeedMaxAxis3=100;		//psParaDsp_Ctrl->FeedMaxAxis3;			//152
	psParaDsp_Ctrl->FeedMaxAxis4=100;		//psParaDsp_Ctrl->FeedMaxAxis4;			//153
	psParaDsp_Ctrl->FeedMaxAxis5=100;		//psParaDsp_Ctrl->FeedMaxAxis5;			//154
	psParaDsp_Ctrl->FeedMaxAxis6=100;		//psParaDsp_Ctrl->FeedMaxAxis6;			//155
	psParaDsp_Ctrl->FeedMaxAxis7=100;		//psParaDsp_Ctrl->FeedMaxAxis7;			//156	
	psParaDsp_Ctrl->FeedMaxAxis8=100;		//psParaDsp_Ctrl->FeedMaxAxis8;			//157
	psParaDsp_Ctrl->FeedMaxAxis9=100;		//psParaDsp_Ctrl->FeedMaxAxis9;			//158		
	psParaDsp_Ctrl->FeedMaxAxis10=100;		//psParaDsp_Ctrl->FeedMaxAxis10;			//159
	psParaDsp_Ctrl->FeedMaxAxis11=100;		//psParaDsp_Ctrl->FeedMaxAxis11;			//160
	psParaDsp_Ctrl->FeedMaxAxis12=100;		//psParaDsp_Ctrl->FeedMaxAxis12;			//161
	psParaDsp_Ctrl->FeedMaxAxis13=100;		//psParaDsp_Ctrl->FeedMaxAxis13;			//162
	psParaDsp_Ctrl->FeedMaxAxis14=100;		//psParaDsp_Ctrl->FeedMaxAxis14;			//163
	psParaDsp_Ctrl->FeedMaxAxis15=100;		//psParaDsp_Ctrl->FeedMaxAxis15;			//164
	psParaDsp_Ctrl->FeedMaxAxis16=100;		//psParaDsp_Ctrl->FeedMaxAxis16;			//165
	psParaDsp_Ctrl->FeedMaxAxis17=100;		//psParaDsp_Ctrl->FeedMaxAxis17;			//166
	psParaDsp_Ctrl->FeedMaxAxis18=100;		//psParaDsp_Ctrl->FeedMaxAxis18;			//167
	psParaDsp_Ctrl->FeedMaxAxis19=100;		//psParaDsp_Ctrl->FeedMaxAxis19;			//168
	psParaDsp_Ctrl->FeedMaxAxis20=100;		//psParaDsp_Ctrl->FeedMaxAxis20;			//169
	psParaDsp_Ctrl->FeedMaxAxis21=100;		//psParaDsp_Ctrl->FeedMaxAxis21;			//170
	psParaDsp_Ctrl->FeedMaxAxis22=100;		//psParaDsp_Ctrl->FeedMaxAxis22;			//171
	psParaDsp_Ctrl->FeedMaxAxis23=100;		//psParaDsp_Ctrl->FeedMaxAxis23;			//172
	psParaDsp_Ctrl->FeedMaxAxis24=100;		//psParaDsp_Ctrl->FeedMaxAxis24;			//173
	psParaDsp_Ctrl->FeedMaxAxis25=100;		//psParaDsp_Ctrl->FeedMaxAxis25;			//174
	
	psParaDsp_Ctrl->REFStopAxis1=0;		//psParaDsp_Ctrl->REFStopAxis1;			//175
	psParaDsp_Ctrl->REFStopAxis2=0;		//psParaDsp_Ctrl->REFStopAxis2;			//176
	psParaDsp_Ctrl->REFStopAxis3=0;		//psParaDsp_Ctrl->REFStopAxis3;			//177
	psParaDsp_Ctrl->REFStopAxis4=0;		//psParaDsp_Ctrl->REFStopAxis4;			//178		
	psParaDsp_Ctrl->REFStopAxis5=0;		//psParaDsp_Ctrl->REFStopAxis5;			//179
	psParaDsp_Ctrl->REFStopAxis6=0;		//psParaDsp_Ctrl->REFStopAxis6;			//180
	psParaDsp_Ctrl->REFStopAxis7=0;		//psParaDsp_Ctrl->REFStopAxis7;			//181
	psParaDsp_Ctrl->REFStopAxis8=0;		//psParaDsp_Ctrl->REFStopAxis8;			//182
	psParaDsp_Ctrl->REFStopAxis9=0;		//psParaDsp_Ctrl->REFStopAxis9;			//183
	psParaDsp_Ctrl->REFStopAxis10=0;		//psParaDsp_Ctrl->REFStopAxis10;			//184
	psParaDsp_Ctrl->REFStopAxis11=0;		//psParaDsp_Ctrl->REFStopAxis11;			//185
	psParaDsp_Ctrl->REFStopAxis12=0;		//psParaDsp_Ctrl->REFStopAxis12;			//186
	psParaDsp_Ctrl->REFStopAxis13=0;		//psParaDsp_Ctrl->REFStopAxis13;			//187
	psParaDsp_Ctrl->REFStopAxis14=0;		//psParaDsp_Ctrl->REFStopAxis14;			//188
	psParaDsp_Ctrl->REFStopAxis15=0;		//psParaDsp_Ctrl->REFStopAxis15;			//189
	psParaDsp_Ctrl->REFStopAxis16=0;		//psParaDsp_Ctrl->REFStopAxis16;			//190	
	psParaDsp_Ctrl->REFStopAxis17=0;		//psParaDsp_Ctrl->REFStopAxis17;			//191
	psParaDsp_Ctrl->REFStopAxis18=0;		//psParaDsp_Ctrl->REFStopAxis18;			//192
	psParaDsp_Ctrl->REFStopAxis19=0;		//psParaDsp_Ctrl->REFStopAxis19;			//193
	psParaDsp_Ctrl->REFStopAxis20=0;		//psParaDsp_Ctrl->REFStopAxis20;			//194
	psParaDsp_Ctrl->REFStopAxis21=0;		//psParaDsp_Ctrl->REFStopAxis21;				//195
	psParaDsp_Ctrl->REFStopAxis22=0;		//psParaDsp_Ctrl->REFStopAxis22;				//196
	psParaDsp_Ctrl->REFStopAxis23=0;		//psParaDsp_Ctrl->REFStopAxis23;				//197
	psParaDsp_Ctrl->REFStopAxis24=0;		//psParaDsp_Ctrl->REFStopAxis24;				//198
	psParaDsp_Ctrl->REFStopAxis25=0;		//psParaDsp_Ctrl->REFStopAxis25;			//199

	psParaDsp_Ctrl->SLimitPosAxis1=30000;		//psParaDsp_Ctrl->SLimitPosAxis1;			//200
	psParaDsp_Ctrl->SLimitPosAxis2=25000;		//psParaDsp_Ctrl->SLimitPosAxis2;			//201	
	psParaDsp_Ctrl->SLimitPosAxis3=110000;		//psParaDsp_Ctrl->SLimitPosAxis3;			//202
	psParaDsp_Ctrl->SLimitPosAxis4=110000;		//psParaDsp_Ctrl->SLimitPosAxis4;			//203
	psParaDsp_Ctrl->SLimitPosAxis5=1000000;		//psParaDsp_Ctrl->SLimitPosAxis5;			//204
	psParaDsp_Ctrl->SLimitPosAxis6=363000;		//psParaDsp_Ctrl->SLimitPosAxis6;			//205
	psParaDsp_Ctrl->SLimitPosAxis7=788500;		//psParaDsp_Ctrl->SLimitPosAxis7;			//206	
	psParaDsp_Ctrl->SLimitPosAxis8=385000;		//psParaDsp_Ctrl->SLimitPosAxis8;			//207
	psParaDsp_Ctrl->SLimitPosAxis9=1000000;		//psParaDsp_Ctrl->SLimitPosAxis9;			//208
	psParaDsp_Ctrl->SLimitPosAxis10=1000000;		//psParaDsp_Ctrl->SLimitPosAxis10;		//209
	psParaDsp_Ctrl->SLimitPosAxis11=385000;		//psParaDsp_Ctrl->SLimitPosAxis11;		//210
	psParaDsp_Ctrl->SLimitPosAxis12=779000;		//psParaDsp_Ctrl->SLimitPosAxis12;		//211
	psParaDsp_Ctrl->SLimitPosAxis13=1000000;		//psParaDsp_Ctrl->SLimitPosAxis13;		//212
	psParaDsp_Ctrl->SLimitPosAxis14=1000000;		//psParaDsp_Ctrl->SLimitPosAxis14;		//213
	psParaDsp_Ctrl->SLimitPosAxis15=656000;		//psParaDsp_Ctrl->SLimitPosAxis15;		//214
	psParaDsp_Ctrl->SLimitPosAxis16=651000;		//psParaDsp_Ctrl->SLimitPosAxis16;		//215
	psParaDsp_Ctrl->SLimitPosAxis17=165000;		//psParaDsp_Ctrl->SLimitPosAxis17;		//216
	psParaDsp_Ctrl->SLimitPosAxis18=315000;		//psParaDsp_Ctrl->SLimitPosAxis18;		//217
	psParaDsp_Ctrl->SLimitPosAxis19=25000;		//psParaDsp_Ctrl->SLimitPosAxis19;		//218
	psParaDsp_Ctrl->SLimitPosAxis20=355000;		//psParaDsp_Ctrl->SLimitPosAxis20;		//219	
	psParaDsp_Ctrl->SLimitPosAxis21=3000000;		//psParaDsp_Ctrl->SLimitPosAxis21;		//220	
	psParaDsp_Ctrl->SLimitPosAxis22=1000000;		//psParaDsp_Ctrl->SLimitPosAxis22;		//221
	psParaDsp_Ctrl->SLimitPosAxis23=1000000;		//psParaDsp_Ctrl->SLimitPosAxis23;		//222
	psParaDsp_Ctrl->SLimitPosAxis24=1000000;		//psParaDsp_Ctrl->SLimitPosAxis24;		//223
	psParaDsp_Ctrl->SLimitPosAxis25=1000000;		//psParaDsp_Ctrl->SLimitPosAxis25;		//224

	psParaDsp_Ctrl->SLimitNegAxis1=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis1;			//225
	psParaDsp_Ctrl->SLimitNegAxis2=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis2;			//226
	psParaDsp_Ctrl->SLimitNegAxis3=-1000;		//psParaDsp_Ctrl->SLimitNegAxis3;			//227
	psParaDsp_Ctrl->SLimitNegAxis4=-1000;		//psParaDsp_Ctrl->SLimitNegAxis4;			//228
	psParaDsp_Ctrl->SLimitNegAxis5=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis5;			//229
	psParaDsp_Ctrl->SLimitNegAxis6=-4000;		//psParaDsp_Ctrl->SLimitNegAxis6;			//230
	psParaDsp_Ctrl->SLimitNegAxis7=-5000;		//psParaDsp_Ctrl->SLimitNegAxis7;			//231
	psParaDsp_Ctrl->SLimitNegAxis8=-1000;		//psParaDsp_Ctrl->SLimitNegAxis8;			//232
	psParaDsp_Ctrl->SLimitNegAxis9=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis9;			//233
	psParaDsp_Ctrl->SLimitNegAxis10=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis10;		//234
	psParaDsp_Ctrl->SLimitNegAxis11=-1000;		//psParaDsp_Ctrl->SLimitNegAxis11;		//235	
	psParaDsp_Ctrl->SLimitNegAxis12=-5000;		//psParaDsp_Ctrl->SLimitNegAxis12;		//236
	psParaDsp_Ctrl->SLimitNegAxis13=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis13;		//237
	psParaDsp_Ctrl->SLimitNegAxis14=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis14;		//238	
	psParaDsp_Ctrl->SLimitNegAxis15=-5000;		//psParaDsp_Ctrl->SLimitNegAxis15;		//239	
	psParaDsp_Ctrl->SLimitNegAxis16=-5000;		//psParaDsp_Ctrl->SLimitNegAxis16;		//240
	psParaDsp_Ctrl->SLimitNegAxis17=4000;		//psParaDsp_Ctrl->SLimitNegAxis17;		//241
	psParaDsp_Ctrl->SLimitNegAxis18=-5000;		//psParaDsp_Ctrl->SLimitNegAxis18;		//242
	psParaDsp_Ctrl->SLimitNegAxis19=-30000;		//psParaDsp_Ctrl->SLimitNegAxis19;		//243
	psParaDsp_Ctrl->SLimitNegAxis20=-5000;		//psParaDsp_Ctrl->SLimitNegAxis20;		//244
	psParaDsp_Ctrl->SLimitNegAxis21=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis21;		//245
	psParaDsp_Ctrl->SLimitNegAxis22=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis22;		//246
	psParaDsp_Ctrl->SLimitNegAxis23=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis23;		//247
	psParaDsp_Ctrl->SLimitNegAxis24=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis24;		//248
	psParaDsp_Ctrl->SLimitNegAxis25=-1000000;		//psParaDsp_Ctrl->SLimitNegAxis25;		//249

	psParaDsp_Ctrl->FunctionSelect01=0;			//psParaDsp_Ctrl->FunctionSelect01;	//250
	psParaDsp_Ctrl->FunctionSelect02=0;			//psParaDsp_Ctrl->FunctionSelect02;	//251
	psParaDsp_Ctrl->FunctionSelect03=0;			//psParaDsp_Ctrl->FunctionSelect03;	//252
	psParaDsp_Ctrl->FunctionSelect04=0;			//psParaDsp_Ctrl->FunctionSelect04;	//253
	psParaDsp_Ctrl->FunctionSelect05=0;			//psParaDsp_Ctrl->FunctionSelect05;	//254
	psParaDsp_Ctrl->FunctionSelect06=0;			//psParaDsp_Ctrl->FunctionSelect06;	//255
	psParaDsp_Ctrl->FunctionSelect07=0;			//psParaDsp_Ctrl->FunctionSelect07;	//256
	psParaDsp_Ctrl->FunctionSelect08=0;			//psParaDsp_Ctrl->FunctionSelect08;	//257
	psParaDsp_Ctrl->FunctionSelect09=0;			//psParaDsp_Ctrl->FunctionSelect09;	//258
	psParaDsp_Ctrl->FunctionSelect10=0;			//psParaDsp_Ctrl->FunctionSelect10;	//259

	psParaArm_Ctrl->LEDFlashTime1 = 500;
	psParaArm_Ctrl->LEDFlashTime2 = 500;

	//for test	
	psParaDsp_Ctrl->Reserved3=0xAA;		//psParaDsp_Ctrl->Reserved3;				//260
	psParaDsp_Ctrl->Reserved4=0xBB;		//psParaDsp_Ctrl->Reserved4;				//261
	psParaDsp_Ctrl->Reserved5=0xCC;		//psParaDsp_Ctrl->Reserved5;				//262
	psParaDsp_Ctrl->Reserved6=0xDD;		//psParaDsp_Ctrl->Reserved6;				//263
	psParaDsp_Ctrl->Reserved7=0xEE;		//psParaDsp_Ctrl->Reserved7;				//264
	psParaDsp_Ctrl->Reserved8=0xFF;		//psParaDsp_Ctrl->Reserved8;				//265
	psParaDsp_Ctrl->Reserved9=0x99; 		//psParaDsp_Ctrl->Reserved3;				//266
	psParaDsp_Ctrl->Reserved10=0x88; 		//psParaDsp_Ctrl->Reserved4;				//267
	psParaDsp_Ctrl->Reserved11=0x77; 		//psParaDsp_Ctrl->Reserved5;				//268
	psParaDsp_Ctrl->Reserved12=0x66; 		//psParaDsp_Ctrl->Reserved6;				//269	
}

void Parameter_Initial_SHMDSP_CTRL(void)
{	
	Int32	i,j;
	Uint32	StartAddress=0,Length=0;
	//Int8	data8;
	Int32	data32;
	Uint32	*UINT32UPointer1,*UINT32UPointer2;
	Int32	*INT32UPointer1,*INT32UPointer2;

	
	StartAddress=EEPROMBUF_PARAARM_CTRL;
	Length=EEPROMIDS_PARAARM_CTRL;
	UINT32UPointer1=&sSHMAddress.pEepromDSP[StartAddress];


	*UINT32UPointer1++=10;	//Tsample;					//0
	*UINT32UPointer1++=23;	//ServoStationsSub;				//1
	*UINT32UPointer1++=10000;	//TrackRunOutRangeSQR;	//2
	*UINT32UPointer1++=2000;	//a_SET;						//3
	*UINT32UPointer1++=300;	//a_SET_2;					//4
	*UINT32UPointer1++=30;	//VeerDeltaV;					//5	
	*UINT32UPointer1++=5;	//NicetyVeerDeltaV;				//6
	*UINT32UPointer1++=30;	//VeerDeltaT;					//7
	*UINT32UPointer1++=5;	//NicetyVeerDeltaT;				//8
	*UINT32UPointer1++=0;	//REFStopVariable;			//9
	
	*UINT32UPointer1++=0xffff;	//EncoderCheckAxis;				//10	
	*UINT32UPointer1++=0xffff;	//RefDirAxis;					//11
	*UINT32UPointer1++=1000;	//AxisUnitInLinear;				//12
	*UINT32UPointer1++=1000;	//AxisUnitOutLinear;				//13	
	*UINT32UPointer1++=6000;	//G0Speed;					//14
	*UINT32UPointer1++=6000;	//G0Speed_2;					//15
	*UINT32UPointer1++=100;	//G0Speed_Module1;		//16
	*UINT32UPointer1++=100;	//G0Speed_Module2;		//17
	*UINT32UPointer1++=100;	//G0Speed_Module3;		//18
	*UINT32UPointer1++=100;	//G0Speed_Module4;		//19	

	*UINT32UPointer1++=100;	//G0Speed_Module5;		//20	
	*UINT32UPointer1++=100;	//G0Speed_Module6;		//21	
	*UINT32UPointer1++=100;	//G0Speed_Module7;		//22
	*UINT32UPointer1++=100;	//G0Speed_Module8;		//23
	*UINT32UPointer1++=100;	//G0Speed_Module9;		//24	
	*UINT32UPointer1++=100;	//G0Speed_Module10;		//25	
	*UINT32UPointer1++=100;	//G0Speed_Module11;		//26
	*UINT32UPointer1++=100;	//G0Speed_Module12;		//27
	*UINT32UPointer1++=600;	//G1Speed;			//28			
	*UINT32UPointer1++=600;	//G1Speed_2;			//29			
	*UINT32UPointer1++=10;	//G1Speed_Module1;		//30		
	*UINT32UPointer1++=10;	//G1Speed_Module2;		//31		
	*UINT32UPointer1++=10;	//G1Speed_Module3;		//32	
	*UINT32UPointer1++=10;	//G1Speed_Module4;		//33	
	
	*UINT32UPointer1++=10;	//G1Speed_Module5;		//34	
	*UINT32UPointer1++=10;	//G1Speed_Module6;		//35
	*UINT32UPointer1++=10;	//G1Speed_Module7;		//36	
	*UINT32UPointer1++=10;	//G1Speed_Module8;		//37
	*UINT32UPointer1++=10;	//G1Speed_Module5;		//38
	*UINT32UPointer1++=10;	//G1Speed_Module6;		//39
	*UINT32UPointer1++=10;	//G1Speed_Module7;		//40
	*UINT32UPointer1++=10;	//G1Speed_Module8;		//41

	*UINT32UPointer1++=2000;	//SRefSpeed;			//42		
	*UINT32UPointer1++=200;	//SRefSpeedBack;		//43		
	*UINT32UPointer1++=20;	//SRefBack;			//44		
	*UINT32UPointer1++=400;	//SRefSpeed_2;		//45		
	*UINT32UPointer1++=50;	//SRefSpeedBack_2;		//46	
	*UINT32UPointer1++=1;	//SRefBack_2;			//47	
	*UINT32UPointer1++=0xABCD;//Reserved1			//48
	*UINT32UPointer1++=0xEEFF;//Reserved2			//49
	
	*UINT32UPointer1++=0;	//MotorChangeDirAxis1;		//50
	*UINT32UPointer1++=0;	//MotorChangeDirAxis2;		//51
	*UINT32UPointer1++=0;	//MotorChangeDirAxis3;		//52
	*UINT32UPointer1++=0;	//MotorChangeDirAxis4;		//53
	*UINT32UPointer1++=0;	//MotorChangeDirAxis5;		//54
	*UINT32UPointer1++=0;	//MotorChangeDirAxis6;		//55
	*UINT32UPointer1++=0;	//MotorChangeDirAxis7;		//56
	*UINT32UPointer1++=0;	//MotorChangeDirAxis8;		//57
	*UINT32UPointer1++=0;	//MotorChangeDirAxis9;		//58
	*UINT32UPointer1++=0;	//MotorChangeDirAxis10;		//59	
	*UINT32UPointer1++=0;	//MotorChangeDirAxis11;		//60
	*UINT32UPointer1++=0;	//MotorChangeDirAxis12;		//61	
	*UINT32UPointer1++=0;	//MotorChangeDirAxis13;		//62
	*UINT32UPointer1++=0;	//MotorChangeDirAxis14;		//63
	*UINT32UPointer1++=0;	//MotorChangeDirAxis15;		//64
	*UINT32UPointer1++=0;	//MotorChangeDirAxis16;		//65
	*UINT32UPointer1++=0;	//MotorChangeDirAxis17;		//66
	*UINT32UPointer1++=0;	//MotorChangeDirAxis18;		//67
	*UINT32UPointer1++=0;	//MotorChangeDirAxis19;		//68
	*UINT32UPointer1++=0;	//MotorChangeDirAxis20;		//69
	*UINT32UPointer1++=0;	//MotorChangeDirAxis21;		//70
	*UINT32UPointer1++=0;	//MotorChangeDirAxis22;		//71
	*UINT32UPointer1++=0;	//MotorChangeDirAxis23;		//72
	*UINT32UPointer1++=0;	//MotorChangeDirAxis24;		//73
	*UINT32UPointer1++=0;	//MotorChangeDirAxis25;		//74
	
	*UINT32UPointer1++=0;	//EncoderRDDir1;		//75	
	*UINT32UPointer1++=0;	//EncoderRDDir2;		//76
	*UINT32UPointer1++=0;	//EncoderRDDir3;		//77
	*UINT32UPointer1++=0;	//EncoderRDDir4;		//78
	*UINT32UPointer1++=0;	//EncoderRDDir5;		//79
	*UINT32UPointer1++=0;	//EncoderRDDir6;		//80
	*UINT32UPointer1++=0;	//EncoderRDDir7;		//81
	*UINT32UPointer1++=0;	//EncoderRDDir8;		//82
	*UINT32UPointer1++=0;	//EncoderRDDir9;		//83
	*UINT32UPointer1++=0;	//EncoderRDDir10;		//84
	*UINT32UPointer1++=0;	//EncoderRDDir11;		//85
	*UINT32UPointer1++=0;	//EncoderRDDir12;		//86
	*UINT32UPointer1++=0;	//EncoderRDDir13;		//87
	*UINT32UPointer1++=0;	//EncoderRDDir14;		//88
	*UINT32UPointer1++=0;	//EncoderRDDir15;		//89	
	*UINT32UPointer1++=0;	//EncoderRDDir16;		//90
	*UINT32UPointer1++=0;	//EncoderRDDir17;		//91
	*UINT32UPointer1++=0;	//EncoderRDDir18;		//92
	*UINT32UPointer1++=0;	//EncoderRDDir19;		//93
	*UINT32UPointer1++=0;	//EncoderRDDir20;		//94
	*UINT32UPointer1++=0;	//EncoderRDDir21;		//95
	*UINT32UPointer1++=0;	//EncoderRDDir22;		//96
	*UINT32UPointer1++=0;	//EncoderRDDir23;		//97
	*UINT32UPointer1++=0;	//EncoderRDDir24;		//98
	*UINT32UPointer1++=0;	//EncoderRDDir25;		//99
	
	*INT32UPointer1++=1000;		//AxisResolutionAxis1;	//100
	*INT32UPointer1++=1000;		//AxisResolutionAxis2;	//101
	*INT32UPointer1++=1000;		//AxisResolutionAxis3;	//102
	*INT32UPointer1++=1000;		//AxisResolutionAxis4;	//103
	*INT32UPointer1++=1000;		//AxisResolutionAxis5;	//104
	*UINT32UPointer1++=1000;	//AxisResolutionAxis6;	//105
	*UINT32UPointer1++=1000;	//AxisResolutionAxis7;	//106
	*UINT32UPointer1++=1000;	//AxisResolutionAxis8;	//107
	*UINT32UPointer1++=1000;	//AxisResolutionAxis9;	//108
	*UINT32UPointer1++=1000;	//AxisResolutionAxis10;		//109
	*UINT32UPointer1++=1000;	//AxisResolutionAxis11;		//110
	*UINT32UPointer1++=1000;	//AxisResolutionAxis12;		//111
	*UINT32UPointer1++=1000;	//AxisResolutionAxis13;		//112
	*UINT32UPointer1++=1000;	//AxisResolutionAxis14;		//113
	*UINT32UPointer1++=1000;	//AxisResolutionAxis15;		//114
	*UINT32UPointer1++=1000;	//AxisResolutionAxis16;		//115
	*UINT32UPointer1++=1000;	//AxisResolutionAxis17;		//116
	*UINT32UPointer1++=1000;	//AxisResolutionAxis18;		//117
	*UINT32UPointer1++=1000;	//AxisResolutionAxis19;		//118
	*UINT32UPointer1++=1000;	//AxisResolutionAxis20;		//119	
	*INT32UPointer1++=1000;		//AxisResolutionAxis21;		//120
	*INT32UPointer1++=1000;		//AxisResolutionAxis22;		//121
	*INT32UPointer1++=1000;		//AxisResolutionAxis23;		//122
	*INT32UPointer1++=1000;		//AxisResolutionAxis24;		//123
	*INT32UPointer1++=1000;		//AxisResolutionAxis25;		//124

	
	*INT32UPointer1++=0;		//CoordORGAxis1;		//125
	*INT32UPointer1++=0;		//CoordORGAxis2;		//126	
	*INT32UPointer1++=0;		//CoordORGAxis3;		//127
	*INT32UPointer1++=0;		//CoordORGAxis4;		//128
	*INT32UPointer1++=0;		//CoordORGAxis5;		//129
	*INT32UPointer1++=0;		//CoordORGAxis6;		//130
	*INT32UPointer1++=0;		//CoordORGAxis7;		//131
	*INT32UPointer1++=0;		//CoordORGAxis8;		//132	
	*INT32UPointer1++=0;		//CoordORGAxis9;		//133
	*INT32UPointer1++=0;		//CoordORGAxis10;		//134	
	*INT32UPointer1++=0;		//CoordORGAxis11;		//135
	*INT32UPointer1++=0;		//CoordORGAxis12;		//136
	*INT32UPointer1++=0;		//CoordORGAxis13;		//137
	*INT32UPointer1++=0;		//CoordORGAxis14;		//138
	*INT32UPointer1++=0;		//CoordORGAxis15;		//139
	*INT32UPointer1++=0;		//CoordORGAxis16;		//140
	*INT32UPointer1++=0;		//CoordORGAxis17;		//141
	*INT32UPointer1++=0;		//CoordORGAxis18;		//142
	*INT32UPointer1++=0;		//CoordORGAxis19;		//143
	*INT32UPointer1++=0;		//CoordORGAxis20;		//144
	*INT32UPointer1++=0;		//CoordORGAxis21;		//145
	*INT32UPointer1++=0;		//CoordORGAxis22;		//146
	*INT32UPointer1++=0;		//CoordORGAxis23;		//147	
	*INT32UPointer1++=0;		//CoordORGAxis24;		//148
	*INT32UPointer1++=0;		//CoordORGAxis25;		//149
	
	*INT32UPointer1++=100;		//FeedMaxAxis1;			//150
	*INT32UPointer1++=100;		//FeedMaxAxis2;			//151
	*INT32UPointer1++=100;		//FeedMaxAxis3;			//152
	*INT32UPointer1++=100;		//FeedMaxAxis4;			//153
	*INT32UPointer1++=100;		//FeedMaxAxis5;			//154
	*INT32UPointer1++=100;		//FeedMaxAxis6;			//155
	*INT32UPointer1++=100;		//FeedMaxAxis7;			//156	
	*INT32UPointer1++=100;		//FeedMaxAxis8;			//157
	*INT32UPointer1++=100;		//FeedMaxAxis9;			//158		
	*INT32UPointer1++=100;		//FeedMaxAxis10;			//159
	*INT32UPointer1++=100;		//FeedMaxAxis11;			//160
	*INT32UPointer1++=100;		//FeedMaxAxis12;			//161
	*INT32UPointer1++=100;		//FeedMaxAxis13;			//162
	*INT32UPointer1++=100;		//FeedMaxAxis14;			//163
	*INT32UPointer1++=100;		//FeedMaxAxis15;			//164
	*INT32UPointer1++=100;		//FeedMaxAxis16;			//165
	*INT32UPointer1++=100;		//FeedMaxAxis17;			//166
	*INT32UPointer1++=100;		//FeedMaxAxis18;			//167
	*INT32UPointer1++=100;		//FeedMaxAxis19;			//168
	*INT32UPointer1++=100;		//FeedMaxAxis20;			//169
	*INT32UPointer1++=100;		//FeedMaxAxis21;			//170
	*INT32UPointer1++=100;		//FeedMaxAxis22;			//171
	*INT32UPointer1++=100;		//FeedMaxAxis23;			//172
	*INT32UPointer1++=100;		//FeedMaxAxis24;			//173
	*INT32UPointer1++=100;		//FeedMaxAxis25;			//174
	
	*INT32UPointer1++=0;		//REFStopAxis1;			//175
	*INT32UPointer1++=0;		//REFStopAxis2;			//176
	*INT32UPointer1++=0;		//REFStopAxis3;			//177
	*INT32UPointer1++=0;		//REFStopAxis4;			//178		
	*INT32UPointer1++=0;		//REFStopAxis5;			//179
	*INT32UPointer1++=0;		//REFStopAxis6;			//180
	*INT32UPointer1++=0;		//REFStopAxis7;			//181
	*INT32UPointer1++=0;		//REFStopAxis8;			//182
	*INT32UPointer1++=0;		//REFStopAxis9;			//183
	*INT32UPointer1++=0;		//REFStopAxis10;			//184
	*INT32UPointer1++=0;		//REFStopAxis11;			//185
	*INT32UPointer1++=0;		//REFStopAxis12;			//186
	*INT32UPointer1++=0;		//REFStopAxis13;			//187
	*INT32UPointer1++=0;		//REFStopAxis14;			//188
	*INT32UPointer1++=0;		//REFStopAxis15;			//189
	*INT32UPointer1++=0;		//REFStopAxis16;			//190	
	*INT32UPointer1++=0;		//REFStopAxis17;			//191
	*INT32UPointer1++=0;		//REFStopAxis18;			//192
	*INT32UPointer1++=0;		//REFStopAxis19;			//193
	*INT32UPointer1++=0;		//REFStopAxis20;			//194
	*INT32UPointer1++=0;		//REFStopAxis21;				//195
	*INT32UPointer1++=0;		//REFStopAxis22;				//196
	*INT32UPointer1++=0;		//REFStopAxis23;				//197
	*INT32UPointer1++=0;		//REFStopAxis24;				//198
	*INT32UPointer1++=0;		//REFStopAxis25;			//199

	*INT32UPointer1++=1000000;		//SLimitPosAxis1;			//200
	*INT32UPointer1++=1000000;		//SLimitPosAxis2;			//201	
	*INT32UPointer1++=1000000;		//SLimitPosAxis3;			//202
	*INT32UPointer1++=1000000;		//SLimitPosAxis4;			//203
	*INT32UPointer1++=1000000;		//SLimitPosAxis5;			//204
	*INT32UPointer1++=1000000;		//SLimitPosAxis6;			//205
	*INT32UPointer1++=1000000;		//SLimitPosAxis7;			//206	
	*INT32UPointer1++=1000000;		//SLimitPosAxis8;			//207
	*INT32UPointer1++=1000000;		//SLimitPosAxis9;			//208
	*INT32UPointer1++=1000000;		//SLimitPosAxis10;		//209
	*INT32UPointer1++=1000000;		//SLimitPosAxis11;		//210
	*INT32UPointer1++=1000000;		//SLimitPosAxis12;		//211
	*INT32UPointer1++=1000000;		//SLimitPosAxis13;		//212
	*INT32UPointer1++=1000000;		//SLimitPosAxis14;		//213
	*INT32UPointer1++=1000000;		//SLimitPosAxis15;		//214
	*INT32UPointer1++=1000000;		//SLimitPosAxis16;		//215
	*INT32UPointer1++=1000000;		//SLimitPosAxis17;		//216
	*INT32UPointer1++=1000000;		//SLimitPosAxis18;		//217
	*INT32UPointer1++=1000000;		//SLimitPosAxis19;		//218
	*INT32UPointer1++=1000000;		//SLimitPosAxis20;		//219	
	*INT32UPointer1++=1000000;		//SLimitPosAxis21;		//220	
	*INT32UPointer1++=1000000;		//SLimitPosAxis22;		//221
	*INT32UPointer1++=1000000;		//SLimitPosAxis23;		//222
	*INT32UPointer1++=1000000;		//SLimitPosAxis24;		//223
	*INT32UPointer1++=1000000;		//SLimitPosAxis25;		//224

	*INT32UPointer1++=-1000000;		//SLimitNegAxis1;			//225
	*INT32UPointer1++=-1000000;		//SLimitNegAxis2;			//226
	*INT32UPointer1++=-1000000;		//SLimitNegAxis3;			//227
	*INT32UPointer1++=-1000000;		//SLimitNegAxis4;			//228
	*INT32UPointer1++=-1000000;		//SLimitNegAxis5;			//229
	*INT32UPointer1++=-1000000;		//SLimitNegAxis6;			//230
	*INT32UPointer1++=-1000000;		//SLimitNegAxis7;			//231
	*INT32UPointer1++=-1000000;		//SLimitNegAxis8;			//232
	*INT32UPointer1++=-1000000;		//SLimitNegAxis9;			//233
	*INT32UPointer1++=-1000000;		//SLimitNegAxis10;		//234
	*INT32UPointer1++=-1000000;		//SLimitNegAxis11;		//235	
	*INT32UPointer1++=-1000000;		//SLimitNegAxis12;		//236
	*INT32UPointer1++=-1000000;		//SLimitNegAxis13;		//237
	*INT32UPointer1++=-1000000;		//SLimitNegAxis14;		//238	
	*INT32UPointer1++=-1000000;		//SLimitNegAxis15;		//239	
	*INT32UPointer1++=-1000000;		//SLimitNegAxis16;		//240
	*INT32UPointer1++=-1000000;		//SLimitNegAxis17;		//241
	*INT32UPointer1++=-1000000;		//SLimitNegAxis18;		//242
	*INT32UPointer1++=-1000000;		//SLimitNegAxis19;		//243
	*INT32UPointer1++=-1000000;		//SLimitNegAxis20;		//244
	*INT32UPointer1++=-1000000;		//SLimitNegAxis21;		//245
	*INT32UPointer1++=-1000000;		//SLimitNegAxis22;		//246
	*INT32UPointer1++=-1000000;		//SLimitNegAxis23;		//247
	*INT32UPointer1++=-1000000;		//SLimitNegAxis24;		//248
	*INT32UPointer1++=-1000000;		//SLimitNegAxis25;		//249

	*UINT32UPointer1++=0;	//FunctionSelect01;				//250
	*UINT32UPointer1++=0;	//FunctionSelect02;				//251
	*UINT32UPointer1++=0;	//FunctionSelect03;				//252
	*UINT32UPointer1++=0;	//FunctionSelect04;				//253
	*UINT32UPointer1++=0;	//FunctionSelect05;				//254
	*UINT32UPointer1++=0;	//FunctionSelect06;				//255
	*UINT32UPointer1++=0;	//FunctionSelect07;				//256
	*UINT32UPointer1++=0;	//FunctionSelect08;				//257
	*UINT32UPointer1++=0;	//FunctionSelect09;				//258
	*UINT32UPointer1++=0;	//FunctionSelect10;				//259
		
	*INT32UPointer1++=0xAA;		//Reserved3;				//260
	*INT32UPointer1++=0xBB;		//Reserved4;				//261
	*INT32UPointer1++=0xCC;		//Reserved5;				//262
	*INT32UPointer1++=0xDD;		//Reserved6;				//263
	*INT32UPointer1++=0xEE;		//Reserved7;				//264
	*INT32UPointer1++=0xFF;		//Reserved8;				//265
	*INT32UPointer1++=0x99;		//Reserved9;				//266
	*INT32UPointer1++=0x88;		//Reserved10;				//267
	*INT32UPointer1++=0x77;		//Reserved11;				//268
	*INT32UPointer1++=0x66;		//Reserved12;				//269
}


void Dsp_Com_PacketIn_Initial(void)
{
	psDSPPacketIn->Head=0xaa;
	psDSPPacketIn->Length=sizeof(tsDSP_PACKETIN)-8;
	psDSPPacketIn->Port = 0xb0; 		
	psDSPPacketIn->Protocal = 0x01; 	
	psDSPPacketIn->Command = 0xb0;		
	psDSPPacketIn->Packet = 0x00;

	psDSPPacketIn->GradeG0 = 0; 	
	psDSPPacketIn->GradeG1 = 0;

	psDSPPacketIn->LEDFlashCtr1 = 0;	
	psDSPPacketIn->LEDFlashCtr2 = 0;	
	psDSPPacketIn->LEDFlashTime1 = 0;	
	psDSPPacketIn->LEDFlashTime2 = 0;	
	
	psDSPPacketIn->JOGAxis = 0; 
	psDSPPacketIn->JOGDir = 0;
	psDSPPacketIn->JOGDistance = 0; 
	psDSPPacketIn->JOGFeedrate =  100;
	
	psDSPPacketIn->InInfo1 = 0;	//for test	
	psDSPPacketIn->InInfo2 = 0;	//for test	
	psDSPPacketIn->OutInfo1 = 0;	
	psDSPPacketIn->OutInfo2 = 0;	

	psDSPPacketIn_InfoEvent->STDInit = 0;
	psDSPPacketIn_InfoEvent->SPCInit = 0;
	//psDSPPacketIn_InfoEvent->ServoOn = 1;
	psDSPPacketIn_InfoEvent->DSPPositionGet = 0;
	psDSPPacketIn_InfoEvent->NCModeType = 0;
		
	//psDSPPacketIn_InfoEvent->FunctionTest1 = 0;
	psDSPPacketIn->FunctionSelectionSwitch = 0;	
}


//RETURN	 0-ERROR 1-OK
Uint16 AxisCompensation_check_ini(void)
{
	Uint16 i;
	
	for(i=0;i<COMPBUFFERLENGTH;i++)
	{
		AxisCompBuffer[i].Value_Linear.Axis1= 12345+i;
		AxisCompBuffer[i].Value_Linear.Axis2 = 12345+2*i;
		AxisCompBuffer[i].Value_Linear.Axis3= 12345+3*i;
		AxisCompBuffer[i].Value_Linear.Axis4 = 12345+4*i;
		AxisCompBuffer[i].Value_Linear.Axis5 = 12345+5*i;
		AxisCompBuffer[i].Value_Linear.Axis6= 12345+6*i;
		AxisCompBuffer[i].Value_Linear.Axis7 = 12345+7*i;
		AxisCompBuffer[i].Value_Linear.Axis8= 12345+8*i;
		AxisCompBuffer[i].Value_Linear.Axis9 = 12345+9*i;
		AxisCompBuffer[i].Value_Linear.Axis10 = 12345+10*i;
		AxisCompBuffer[i].Value_Linear.Axis11= 12345+11*i;
		AxisCompBuffer[i].Value_Linear.Axis12 = 12345+12*i;
		AxisCompBuffer[i].Value_Linear.Axis13= 12345+13*i;
		AxisCompBuffer[i].Value_Linear.Axis14 = 12345+14*i;
		AxisCompBuffer[i].Value_Linear.Axis15 = 12345+15*i;
		AxisCompBuffer[i].Value_Linear.Axis16= 12345+16*i;
		AxisCompBuffer[i].Value_Linear.Axis17 = 12345+17*i;
		AxisCompBuffer[i].Value_Linear.Axis18= 12345+18*i;
		AxisCompBuffer[i].Value_Linear.Axis19 = 12345+19*i;
		AxisCompBuffer[i].Value_Linear.Axis20 = 12345+20*i;
		AxisCompBuffer[i].Value_Linear.Axis21= 12345+21*i;
		AxisCompBuffer[i].Value_Linear.Axis22 = 12345+22*i;
		AxisCompBuffer[i].Value_Linear.Axis23= 12345+23*i;
		AxisCompBuffer[i].Value_Linear.Axis24 = 12345+24*i;
		AxisCompBuffer[i].Value_Linear.Axis25 = 12345+25*i;
		
		AxisCompBuffer[i].Value_Laser.Axis1= 12345+i;
		AxisCompBuffer[i].Value_Laser.Axis2 = 12345+2*i;
		AxisCompBuffer[i].Value_Laser.Axis3= 12345+3*i;
		AxisCompBuffer[i].Value_Laser.Axis4 = 12345+4*i;
		AxisCompBuffer[i].Value_Laser.Axis5 = 12345+5*i;
		AxisCompBuffer[i].Value_Laser.Axis6= 12345+6*i;
		AxisCompBuffer[i].Value_Laser.Axis7 = 12345+7*i;
		AxisCompBuffer[i].Value_Laser.Axis8= 12345+8*i;
		AxisCompBuffer[i].Value_Laser.Axis9 = 12345+9*i;
		AxisCompBuffer[i].Value_Laser.Axis10 = 12345+10*i;
		AxisCompBuffer[i].Value_Laser.Axis11= 12345+11*i;
		AxisCompBuffer[i].Value_Laser.Axis12 = 12345+12*i;
		AxisCompBuffer[i].Value_Laser.Axis13= 12345+13*i;
		AxisCompBuffer[i].Value_Laser.Axis14 = 12345+14*i;
		AxisCompBuffer[i].Value_Laser.Axis15 = 12345+15*i;
		AxisCompBuffer[i].Value_Laser.Axis16= 12345+16*i;
		AxisCompBuffer[i].Value_Laser.Axis17 = 12345+17*i;
		AxisCompBuffer[i].Value_Laser.Axis18= 12345+18*i;
		AxisCompBuffer[i].Value_Laser.Axis19 = 12345+19*i;
		AxisCompBuffer[i].Value_Laser.Axis20 = 12345+20*i;
		AxisCompBuffer[i].Value_Laser.Axis21= 12345+21*i;
		AxisCompBuffer[i].Value_Laser.Axis22 = 12345+22*i;
		AxisCompBuffer[i].Value_Laser.Axis23= 12345+23*i;
		AxisCompBuffer[i].Value_Laser.Axis24 = 12345+24*i;
		AxisCompBuffer[i].Value_Laser.Axis25 = 12345+25*i;

	}

	for(i=0;i<COMPBUFFERLENGTH;i++)
	{
		if(AxisCompBuffer[i].Value_Linear.Axis1 != (12345+i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis2 != (12345+2*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis3 != (12345+3*i))
		{
			return 0;
		}		
		if(AxisCompBuffer[i].Value_Linear.Axis4 != (12345+4*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis5 != (12345+5*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis6 != (12345+6*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis7 != (12345+7*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis8 != (12345+8*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis9 != (12345+9*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis10 != (12345+10*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis11 != (12345+11*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis12 != (12345+12*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis13 != (12345+13*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis14 != (12345+14*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis15 != (12345+15*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis16 != (12345+16*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis17 != (12345+17*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis18 != (12345+18*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis19 != (12345+19*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis20 != (12345+20*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis21 != (12345+21*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis22 != (12345+22*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis23 != (12345+23*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis24 != (12345+24*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Linear.Axis25 != (12345+25*i))
		{
			return 0;
		}		

		if(AxisCompBuffer[i].Value_Laser.Axis1 != (12345+i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis2 != (12345+2*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis3 != (12345+3*i))
		{
			return 0;
		}		
		if(AxisCompBuffer[i].Value_Laser.Axis4 != (12345+4*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis5 != (12345+5*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis6 != (12345+6*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis7 != (12345+7*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis8 != (12345+8*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis9 != (12345+9*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis10 != (12345+10*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis11 != (12345+11*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis12 != (12345+12*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis13 != (12345+13*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis14 != (12345+14*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis15 != (12345+15*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis16 != (12345+16*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis17 != (12345+17*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis18 != (12345+18*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis19 != (12345+19*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis20 != (12345+20*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis21 != (12345+21*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis22 != (12345+22*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis23 != (12345+23*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis24 != (12345+24*i))
		{
			return 0;
		}
		if(AxisCompBuffer[i].Value_Laser.Axis25 != (12345+25*i))
		{
			return 0;
		}
	}

	// �����Բ���������ʼ��
	System.CompensationDirection.Axis1 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis2 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis3 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis4 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis5 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis6 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis7 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis8 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis9 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis10 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis11 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis12 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis13 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis14 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis15 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis16 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis17 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis18 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis19 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis20 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis21 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis22 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis23 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis24 = 0;	//NEG COMPENSATION
	System.CompensationDirection.Axis25 = 0;	//NEG COMPENSATION

	for(i=0;i<COMPBUFFERLENGTH;i++)
	{
		AxisCompBuffer[i].Value_Linear.Axis1 = 0;
		AxisCompBuffer[i].Value_Linear.Axis2 = 0;
		AxisCompBuffer[i].Value_Linear.Axis3 = 0;
		AxisCompBuffer[i].Value_Linear.Axis4= 0;
		AxisCompBuffer[i].Value_Linear.Axis5 = 0;
		AxisCompBuffer[i].Value_Linear.Axis6 = 0;
		AxisCompBuffer[i].Value_Linear.Axis7 = 0;
		AxisCompBuffer[i].Value_Linear.Axis8 = 0;
		AxisCompBuffer[i].Value_Linear.Axis9= 0;
		AxisCompBuffer[i].Value_Linear.Axis10 = 0;
		AxisCompBuffer[i].Value_Linear.Axis11 = 0;
		AxisCompBuffer[i].Value_Linear.Axis12 = 0;
		AxisCompBuffer[i].Value_Linear.Axis13 = 0;
		AxisCompBuffer[i].Value_Linear.Axis14= 0;
		AxisCompBuffer[i].Value_Linear.Axis15 = 0;
		AxisCompBuffer[i].Value_Linear.Axis16 = 0;
		AxisCompBuffer[i].Value_Linear.Axis17 = 0;
		AxisCompBuffer[i].Value_Linear.Axis18 = 0;
		AxisCompBuffer[i].Value_Linear.Axis19= 0;
		AxisCompBuffer[i].Value_Linear.Axis20 = 0;
		AxisCompBuffer[i].Value_Linear.Axis21 = 0;
		AxisCompBuffer[i].Value_Linear.Axis22 = 0;
		AxisCompBuffer[i].Value_Linear.Axis23 = 0;
		AxisCompBuffer[i].Value_Linear.Axis24= 0;
		AxisCompBuffer[i].Value_Linear.Axis25 = 0;

		AxisCompBuffer[i].Value_Laser.Axis1 = 0;
		AxisCompBuffer[i].Value_Laser.Axis2 = 0;
		AxisCompBuffer[i].Value_Laser.Axis3 = 0;
		AxisCompBuffer[i].Value_Laser.Axis4= 0;
		AxisCompBuffer[i].Value_Laser.Axis5 = 0;
		AxisCompBuffer[i].Value_Laser.Axis6 = 0;
		AxisCompBuffer[i].Value_Laser.Axis7 = 0;
		AxisCompBuffer[i].Value_Laser.Axis8 = 0;
		AxisCompBuffer[i].Value_Laser.Axis9= 0;
		AxisCompBuffer[i].Value_Laser.Axis10 = 0;
		AxisCompBuffer[i].Value_Laser.Axis11 = 0;
		AxisCompBuffer[i].Value_Laser.Axis12 = 0;
		AxisCompBuffer[i].Value_Laser.Axis13 = 0;
		AxisCompBuffer[i].Value_Laser.Axis14= 0;
		AxisCompBuffer[i].Value_Laser.Axis15 = 0;
		AxisCompBuffer[i].Value_Laser.Axis16 = 0;
		AxisCompBuffer[i].Value_Laser.Axis17 = 0;
		AxisCompBuffer[i].Value_Laser.Axis18 = 0;
		AxisCompBuffer[i].Value_Laser.Axis19= 0;
		AxisCompBuffer[i].Value_Laser.Axis20 = 0;
		AxisCompBuffer[i].Value_Laser.Axis21 = 0;
		AxisCompBuffer[i].Value_Laser.Axis22 = 0;
		AxisCompBuffer[i].Value_Laser.Axis23 = 0;
		AxisCompBuffer[i].Value_Laser.Axis24= 0;
		AxisCompBuffer[i].Value_Laser.Axis25 = 0;	
	}

	AxisCompBuffer[1].Value_Linear.Axis1 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis2 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis3 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis4= -100000;	
	AxisCompBuffer[1].Value_Linear.Axis5 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis6 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis7 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis8 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis9= -100000;	
	AxisCompBuffer[1].Value_Linear.Axis10 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis11 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis12 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis13 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis14= -100000;	
	AxisCompBuffer[1].Value_Linear.Axis15 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis16 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis17 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis18 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis19= -100000;	
	AxisCompBuffer[1].Value_Linear.Axis20 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis21 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis22 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis23 = -100000;	
	AxisCompBuffer[1].Value_Linear.Axis24= -100000;	
	AxisCompBuffer[1].Value_Linear.Axis25 = -100000;		

	AxisCompBuffer[1].Value_Laser.Axis1 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis2 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis3 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis4= -100000;	
	AxisCompBuffer[1].Value_Laser.Axis5 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis6 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis7 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis8 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis9= -100000;	
	AxisCompBuffer[1].Value_Laser.Axis10 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis11 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis12 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis13 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis14= -100000;	
	AxisCompBuffer[1].Value_Laser.Axis15 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis16 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis17 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis18 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis19= -100000;	
	AxisCompBuffer[1].Value_Laser.Axis20 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis21 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis22 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis23 = -100000;	
	AxisCompBuffer[1].Value_Laser.Axis24= -100000;	
	AxisCompBuffer[1].Value_Laser.Axis25 = -100000;		

	AxisCompBuffer[2].Value_Linear.Axis1 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis2 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis3 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis4= -200000;	
	AxisCompBuffer[2].Value_Linear.Axis5 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis6 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis7 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis8 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis9= -200000;	
	AxisCompBuffer[2].Value_Linear.Axis10 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis11 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis12 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis13 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis14= -200000;	
	AxisCompBuffer[2].Value_Linear.Axis15 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis16 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis17 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis18 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis19= -200000;	
	AxisCompBuffer[2].Value_Linear.Axis20 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis21 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis22 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis23 = -200000;	
	AxisCompBuffer[2].Value_Linear.Axis24= -200000;	
	AxisCompBuffer[2].Value_Linear.Axis25 = -200000;		

	AxisCompBuffer[2].Value_Laser.Axis1 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis2 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis3 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis4= -200000;	
	AxisCompBuffer[2].Value_Laser.Axis5 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis6 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis7 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis8 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis9= -200000;	
	AxisCompBuffer[2].Value_Laser.Axis10 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis11 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis12 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis13 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis14= -200000;	
	AxisCompBuffer[2].Value_Laser.Axis15 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis16 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis17 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis18 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis19= -200000;	
	AxisCompBuffer[2].Value_Laser.Axis20 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis21 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis22 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis23 = -200000;	
	AxisCompBuffer[2].Value_Laser.Axis24= -200000;	
	AxisCompBuffer[2].Value_Laser.Axis25 = -200000;		


	OverallSign.EncoderCompPt.Axis1= 1;
	OverallSign.EncoderCompPt.Axis2 = 1;
	OverallSign.EncoderCompPt.Axis3 = 1;
	OverallSign.EncoderCompPt.Axis4 = 1;
	OverallSign.EncoderCompPt.Axis5 = 1;
	OverallSign.EncoderCompPt.Axis6= 1;
	OverallSign.EncoderCompPt.Axis7 = 1;
	OverallSign.EncoderCompPt.Axis8 = 1;
	OverallSign.EncoderCompPt.Axis9 = 1;
	OverallSign.EncoderCompPt.Axis10 = 1;
	OverallSign.EncoderCompPt.Axis11= 1;
	OverallSign.EncoderCompPt.Axis12 = 1;
	OverallSign.EncoderCompPt.Axis13 = 1;
	OverallSign.EncoderCompPt.Axis14 = 1;
	OverallSign.EncoderCompPt.Axis15 = 1;
	OverallSign.EncoderCompPt.Axis16= 1;
	OverallSign.EncoderCompPt.Axis17 = 1;
	OverallSign.EncoderCompPt.Axis18 = 1;
	OverallSign.EncoderCompPt.Axis19 = 1;
	OverallSign.EncoderCompPt.Axis20 = 1;
	OverallSign.EncoderCompPt.Axis21= 1;
	OverallSign.EncoderCompPt.Axis22 = 1;
	OverallSign.EncoderCompPt.Axis23 = 1;
	OverallSign.EncoderCompPt.Axis24 = 1;
	OverallSign.EncoderCompPt.Axis25 = 1;

	OverallSign.PulseOutCompPt.Axis1 = 1;
	OverallSign.PulseOutCompPt.Axis2 = 1;
	OverallSign.PulseOutCompPt.Axis3 = 1;
	OverallSign.PulseOutCompPt.Axis4 = 1;
	OverallSign.PulseOutCompPt.Axis5 = 1;
	OverallSign.PulseOutCompPt.Axis6 = 1;
	OverallSign.PulseOutCompPt.Axis7 = 1;
	OverallSign.PulseOutCompPt.Axis8 = 1;
	OverallSign.PulseOutCompPt.Axis9 = 1;
	OverallSign.PulseOutCompPt.Axis10 = 1;	
	OverallSign.PulseOutCompPt.Axis11 = 1;
	OverallSign.PulseOutCompPt.Axis12 = 1;
	OverallSign.PulseOutCompPt.Axis13 = 1;
	OverallSign.PulseOutCompPt.Axis14 = 1;
	OverallSign.PulseOutCompPt.Axis15 = 1;
	OverallSign.PulseOutCompPt.Axis16 = 1;
	OverallSign.PulseOutCompPt.Axis17 = 1;
	OverallSign.PulseOutCompPt.Axis18 = 1;
	OverallSign.PulseOutCompPt.Axis19 = 1;
	OverallSign.PulseOutCompPt.Axis20 = 1;	
	OverallSign.PulseOutCompPt.Axis21 = 1;
	OverallSign.PulseOutCompPt.Axis22 = 1;
	OverallSign.PulseOutCompPt.Axis23 = 1;
	OverallSign.PulseOutCompPt.Axis24 = 1;
	OverallSign.PulseOutCompPt.Axis25 = 1;

	//for test
	//Axis15
	System.CompensationDirection.Axis15 = 1;	//POS COMPENSATION
	AxisCompBuffer[0].Value_Linear.Axis15  = -20000;
	AxisCompBuffer[1].Value_Linear.Axis15  = -10000;
	AxisCompBuffer[2].Value_Linear.Axis15  = 0;
	AxisCompBuffer[3].Value_Linear.Axis15  = 10000;
	AxisCompBuffer[4].Value_Linear.Axis15  = 20000;
	AxisCompBuffer[5].Value_Linear.Axis15  = 30000;
	AxisCompBuffer[6].Value_Linear.Axis15  = 40000;
	AxisCompBuffer[7].Value_Linear.Axis15  = 50000;
	AxisCompBuffer[8].Value_Linear.Axis15  = 60000;
	AxisCompBuffer[9].Value_Linear.Axis15  = 70000;
	AxisCompBuffer[10].Value_Linear.Axis15 = 80000;
	AxisCompBuffer[11].Value_Linear.Axis15 = 90000;
	AxisCompBuffer[12].Value_Linear.Axis15 = 100000;
	AxisCompBuffer[13].Value_Linear.Axis15 = 110000;
	AxisCompBuffer[14].Value_Linear.Axis15 = 120000;
	AxisCompBuffer[15].Value_Linear.Axis15 = 130000;
	AxisCompBuffer[16].Value_Linear.Axis15 = 140000;
	AxisCompBuffer[17].Value_Linear.Axis15 = 150000;
	AxisCompBuffer[18].Value_Linear.Axis15 = 160000;
	AxisCompBuffer[19].Value_Linear.Axis15 = 170000;
	AxisCompBuffer[20].Value_Linear.Axis15 = 180000;
	AxisCompBuffer[21].Value_Linear.Axis15 = 1000000;	

	AxisCompBuffer[0].Value_Laser.Axis15  = -20000;
	AxisCompBuffer[1].Value_Laser.Axis15  = -9987;
	AxisCompBuffer[2].Value_Laser.Axis15  = 25;
	AxisCompBuffer[3].Value_Laser.Axis15  = 10040;
	AxisCompBuffer[4].Value_Laser.Axis15  = 20056;
	AxisCompBuffer[5].Value_Laser.Axis15  = 30073;
	AxisCompBuffer[6].Value_Laser.Axis15  = 40089;
	AxisCompBuffer[7].Value_Laser.Axis15  = 50106;
	AxisCompBuffer[8].Value_Laser.Axis15  = 60123;
	AxisCompBuffer[9].Value_Laser.Axis15  = 70139;
	AxisCompBuffer[10].Value_Laser.Axis15 = 80158;
	AxisCompBuffer[11].Value_Laser.Axis15 = 90175;
	AxisCompBuffer[12].Value_Laser.Axis15 = 100190;
	AxisCompBuffer[13].Value_Laser.Axis15 = 110204;
	AxisCompBuffer[14].Value_Laser.Axis15 = 120217;
	AxisCompBuffer[15].Value_Laser.Axis15 = 130231;
	AxisCompBuffer[16].Value_Laser.Axis15 = 140245;
	AxisCompBuffer[17].Value_Laser.Axis15 = 150259;
	AxisCompBuffer[18].Value_Laser.Axis15 = 160271;
	AxisCompBuffer[19].Value_Laser.Axis15 = 170282;
	AxisCompBuffer[20].Value_Laser.Axis15 = 180293;
	AxisCompBuffer[21].Value_Laser.Axis15 = 1000000;	
	
	//Axis16
	System.CompensationDirection.Axis16 = 1;	//POS COMPENSATION
	AxisCompBuffer[0].Value_Linear.Axis16  = -20000;
	AxisCompBuffer[1].Value_Linear.Axis16  = 262000;
	AxisCompBuffer[2].Value_Linear.Axis16  = 272000;
	AxisCompBuffer[3].Value_Linear.Axis16  = 282000;
	AxisCompBuffer[4].Value_Linear.Axis16  = 292000;
	AxisCompBuffer[5].Value_Linear.Axis16  = 302000;
	AxisCompBuffer[6].Value_Linear.Axis16  = 312000;
	AxisCompBuffer[7].Value_Linear.Axis16  = 322000;
	AxisCompBuffer[8].Value_Linear.Axis16  = 332000;
	AxisCompBuffer[9].Value_Linear.Axis16  = 342000;
	AxisCompBuffer[10].Value_Linear.Axis16 = 352000;
	AxisCompBuffer[11].Value_Linear.Axis16 = 362000;
	AxisCompBuffer[12].Value_Linear.Axis16 = 372000;
	AxisCompBuffer[13].Value_Linear.Axis16 = 382000;
	AxisCompBuffer[14].Value_Linear.Axis16 = 392000;
	AxisCompBuffer[15].Value_Linear.Axis16 = 402000;
	AxisCompBuffer[16].Value_Linear.Axis16 = 412000;
	AxisCompBuffer[17].Value_Linear.Axis16 = 422000;
	AxisCompBuffer[18].Value_Linear.Axis16 = 432000;
	AxisCompBuffer[19].Value_Linear.Axis16 = 442000;
	AxisCompBuffer[20].Value_Linear.Axis16 = 452000;
	AxisCompBuffer[21].Value_Linear.Axis16 = 462000;
	AxisCompBuffer[22].Value_Linear.Axis16 = 472000;
	AxisCompBuffer[23].Value_Linear.Axis16 = 482000;
	AxisCompBuffer[24].Value_Linear.Axis16 = 492000;
	AxisCompBuffer[25].Value_Linear.Axis16 = 502000;
	AxisCompBuffer[26].Value_Linear.Axis16 = 1000000;

	AxisCompBuffer[0].Value_Laser.Axis16  = -20000;
	AxisCompBuffer[1].Value_Laser.Axis16  = 262000;
	AxisCompBuffer[2].Value_Laser.Axis16  = 271996;
	AxisCompBuffer[3].Value_Laser.Axis16  = 281997;
	AxisCompBuffer[4].Value_Laser.Axis16  = 292002;
	AxisCompBuffer[5].Value_Laser.Axis16  = 302002;
	AxisCompBuffer[6].Value_Laser.Axis16  = 312000;
	AxisCompBuffer[7].Value_Laser.Axis16  = 321994;
	AxisCompBuffer[8].Value_Laser.Axis16  = 331990;
	AxisCompBuffer[9].Value_Laser.Axis16  = 341990;
	AxisCompBuffer[10].Value_Laser.Axis16 = 351991;
	AxisCompBuffer[11].Value_Laser.Axis16 = 361991;
	AxisCompBuffer[12].Value_Laser.Axis16 = 371985;
	AxisCompBuffer[13].Value_Laser.Axis16 = 381978;
	AxisCompBuffer[14].Value_Laser.Axis16 = 391971;
	AxisCompBuffer[15].Value_Laser.Axis16 = 401967;
	AxisCompBuffer[16].Value_Laser.Axis16 = 411971;
	AxisCompBuffer[17].Value_Laser.Axis16 = 421971;
	AxisCompBuffer[18].Value_Laser.Axis16 = 431967;
	AxisCompBuffer[19].Value_Laser.Axis16 = 441961;
	AxisCompBuffer[20].Value_Laser.Axis16 = 451957;
	AxisCompBuffer[21].Value_Laser.Axis16 = 461955;
	AxisCompBuffer[22].Value_Laser.Axis16 = 471956;
	AxisCompBuffer[23].Value_Laser.Axis16 = 481957;
	AxisCompBuffer[24].Value_Laser.Axis16 = 491954;
	AxisCompBuffer[25].Value_Laser.Axis16 = 501949;	
	AxisCompBuffer[26].Value_Laser.Axis16 = 1000000;

	return 1;
}


INT16U GCODE_check(void)
{
	INT16U i;
	GCODE GCodeCheck;

	GCodeCheck.SendCount = 12345;
	GCodeCheck.CMDMain = 12345;
	GCodeCheck.SubCMD1 = 12345;
	GCodeCheck.SubCMD2 = 12345;

	GCodeCheck.StartPoint.Axis1 = 123456;
	GCodeCheck.StartPoint.Axis2 = 123456;
	GCodeCheck.StartPoint.Axis3 = 123456;
	GCodeCheck.StartPoint.Axis4 = 123456;
	GCodeCheck.StartPoint.Axis5 = 123456;
	GCodeCheck.StartPoint.Axis6 = 123456;
	GCodeCheck.StartPoint.Axis7 = 123456;
	GCodeCheck.StartPoint.Axis8 = 123456;
	GCodeCheck.StartPoint.Axis9 = 123456;
	GCodeCheck.StartPoint.Axis10 = 123456;
	GCodeCheck.StartPoint.Axis11 = 123456;
	GCodeCheck.StartPoint.Axis12 = 123456;
	GCodeCheck.StartPoint.Axis13 = 123456;
	GCodeCheck.StartPoint.Axis14 = 123456;
	GCodeCheck.StartPoint.Axis15 = 123456;
	GCodeCheck.StartPoint.Axis16 = 123456;
	GCodeCheck.StartPoint.Axis17 = 123456;
	GCodeCheck.StartPoint.Axis18 = 123456;
	GCodeCheck.StartPoint.Axis19 = 123456;
	GCodeCheck.StartPoint.Axis20 = 123456;
	GCodeCheck.StartPoint.Axis21 = 123456;
	GCodeCheck.StartPoint.Axis22 = 123456;
	GCodeCheck.StartPoint.Axis23 = 123456;

	GCodeCheck.EndPoint.Axis1 = 123456;
	GCodeCheck.EndPoint.Axis2 = 123456;
	GCodeCheck.EndPoint.Axis3 = 123456;
	GCodeCheck.EndPoint.Axis4 = 123456;
	GCodeCheck.EndPoint.Axis5 = 123456;
	GCodeCheck.EndPoint.Axis6 = 123456;	
	GCodeCheck.EndPoint.Axis7 = 123456;
	GCodeCheck.EndPoint.Axis8 = 123456;
	GCodeCheck.EndPoint.Axis9 = 123456;	
	GCodeCheck.EndPoint.Axis10 = 123456;
	GCodeCheck.EndPoint.Axis11 = 123456;
	GCodeCheck.EndPoint.Axis12 = 123456;
	GCodeCheck.EndPoint.Axis13 = 123456;
	GCodeCheck.EndPoint.Axis14 = 123456;
	GCodeCheck.EndPoint.Axis15 = 123456;
	GCodeCheck.EndPoint.Axis16 = 123456;	
	GCodeCheck.EndPoint.Axis17 = 123456;
	GCodeCheck.EndPoint.Axis18 = 123456;
	GCodeCheck.EndPoint.Axis19 = 123456;	
	GCodeCheck.EndPoint.Axis20 = 123456;
	GCodeCheck.EndPoint.Axis21 = 123456;
	GCodeCheck.EndPoint.Axis22 = 123456;
	GCodeCheck.EndPoint.Axis23 = 123456;

	for(i=0;i<STDGCODE_MOD;i++)
	{
		GCodeBuffer[i] = 	GCodeCheck;	
	}
	Error.OutsideRAMCheckError = 0;
	
	for(i=0;i<STDGCODE_MOD;i++)
	{
		if(GCodeBuffer[i].SendCount != 12345)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].CMDMain != 12345)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].SubCMD1 != 12345)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].SubCMD2 != 12345)
		{
			return FALSE;
		}		
		if(GCodeBuffer[i].StartPoint.Axis1 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis2 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis3 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis4 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis5 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis6 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis7 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis8 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis9 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis10 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis11 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis12 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis13 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis14 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis15 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis16 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis17 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis18 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis19 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis20 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis21 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis22 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].StartPoint.Axis23 != 123456)
		{
			return FALSE;
		}

		
		if(GCodeBuffer[i].EndPoint.Axis1 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis2 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis3 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis4 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis5 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis6 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis7 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis8 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis9 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis10 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis11 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis12 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis13 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis14 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis15 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis16 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis17 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis18 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis19 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis20 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis21 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis22 != 123456)
		{
			return FALSE;
		}
		if(GCodeBuffer[i].EndPoint.Axis23 != 123456)
		{
			return FALSE;
		}
		
	}
	return TRUE;
}

void SDRAM_check(void)
{
	INT16U CheckReturn;
	Error.InsideRAMCheckError = 0;
	
	CheckReturn = AxisCompensation_check_ini();
	if(CheckReturn == 0)
	{
		Error.InsideRAMCheckError = 1;
	}	
	CheckReturn = GCODE_check();
	if(CheckReturn == 0)
	{
		Error.InsideRAMCheckError = 1;
	}

}

Uint16 JumpError=0;
Uint16 high_error(void)
{
	if(JumpError)
	{
		return 0;
	}
	Error.MainErrorSign = 1;	
	Error.HaveGetErrorDataSign = 1;
	Error.RealRunCount = STDNCSign.RealRunCount;
	NC_AREAControl_Error = NC_AREAControl;
	CPLD_ServoOff();
	return 1;
}



