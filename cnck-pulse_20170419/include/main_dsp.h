#ifndef _MAIN_DSP_H
#define  _MAIN_DSP_H

#include "datatype.h"

// xdctools header files 
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Registry.h>

// package header files 
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>
#include <ti/ipc/SharedRegion.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/mathlib/mathlib.h>

//define a interrupt enable and disable macro
#define IRQ_DISABLE(num)  Hwi_disableInterrupt(num)
#define IRQ_ENABLE(num)   Hwi_enableInterrupt(num)

#include "common.h"

#include "structcode.h"
#include "structcode.h"
#include "structpacket.h"
#include "structpara.h"
#include "structpoint.h"

#include "cputimers.h"
#include "area_code_rec.h"
#include "area_control.h"         	
#include "sci.h"            
#include "run_nc.h"	
#include "cpld.h"		
#include "spi.h"
#include "int_code.h"
#include "sharemem.h"
#include "Test.h"

#include "stdio.h"
#include "stdlib.h"
#include "Evm816x.h"
#include "norflash.h"
#include "evm816x_gpmc.h"


extern INT32S MonitorBuffer1[1024];					//éŽ´î�„å½‡é��ç‰ˆåµ�ç¼‚æ’³å•¿é�–ï¿?
extern INT32S MonitorBuffer2[1024];					//éŽ´î�„å½‡é��ç‰ˆåµ�ç¼‚æ’³å•¿é�–ï¿?
extern INT32S MonitorBuffer3[1024];					//éŽ´î�„å½‡é��ç‰ˆåµ�ç¼‚æ’³å•¿é�–ï¿?
extern INT32S MonitorBuffer4[1024];
extern INT32S GetDataBuffer[1024];					//éŽ´î�„å½‡é��ç‰ˆåµ�ç¼‚æ’³å•¿é�–ï¿?
extern INTERPOLATION_BUFFER InterpolationBuffer_1[INTERPOLATIONBUFLENGTH+1];

extern INT32S LensCompensationBuffer[8000];	

extern void System_Initial(void);
extern void Poweron_Initial(void);
extern void Parameter_Initial(void);
extern void Parameter_Initial_SHMDSP_2(void);
extern void Parameter_Initial_SHMDSP_CTRL_2(void);
extern void Parameter_Initial_SHMDSP(void);
extern void Parameter_Initial_SHMDSP_CTRL(void);
extern void Dsp_Com_PacketIn_Initial(void);
extern void SDRAM_check(void);
extern Uint16 AxisCompensation_check_ini(void);
extern void STD_Initial(void);
extern void SPC_Initial(void);	
extern Uint16 high_error(void);

#endif  // end of DSP6713_MAIN_H definition

