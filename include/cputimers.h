//###########################################################################
//DSP6713 å®šæ—¶å™¨
// FILE:    CpuTimers.h
//

#ifndef CPU_TIMERS_H
#define CPU_TIMERS_H

typedef struct
{
	FLOAT64 SamplePeriod;		
	INT32U GenericCounter;		
	INT32U GetPositionCounter;	
	INT32U G04StartCounter;	
	INT32U G114StartCounter;	
	INT32U SPCStartCounter;		
	INT32U SMC1_Homing_Counter;		
	INT32U SMC2_Homing_Counter;		
	INT32U LedFlashCounter;
	INT32U LedFlashDeltaCounter;
	INT32U LensCompensationDeltaCounter;
}TIMER0;


//---------------------------------------------------------------------------
// Function prototypes and external definitions:
//

extern TIMER0 Timer0;	//å®šæ—¶å™¨0 ç»“æž„å¯¹è±¡

extern FLOAT64 get_delta_time(INT32U StartTime);	//è®¡ç®—ç”±StartTime èµ·çš„ç»�åŽ†æ—¶é—´


void InitCpuTimers(void);
//void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, FLOAT32 Freq, FLOAT32 Period);
extern volatile struct CPUTIMER_REGS CpuTimer0Regs;

// Start Timer:
#define StartCpuTimer0()  CpuTimer0Regs.TCR.bit.TSS = 0

// Stop Timer:
#define StopCpuTimer0()   CpuTimer0Regs.TCR.bit.TSS = 1

// Reload Timer With period Value:
#define ReloadCpuTimer0() CpuTimer0Regs.TCR.bit.TRB = 1

// Read 32-Bit Timer Value:
#define ReadCpuTimer0Counter() CpuTimer0Regs.TIM.all

// Read 32-Bit Period Value:
#define ReadCpuTimer0Period() CpuTimer0Regs.PRD.all


#endif  // end of DSP281x_CPU_TIMERS_H definition


//===========================================================================
// No more.
//===========================================================================
