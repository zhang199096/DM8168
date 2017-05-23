#ifndef _SCI_H
#define _SCI_H

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	INT16U	SPD_SendRight;
	INT16U	SPD_SendCycle;	
	INT16U	SPD_SendSign;			
	INT16U	SPD_ReceiveSign;		
	INT32U	SPD_RecCount;
	INT32U	SPD_SendCount;
	INT32U	SPD_ResendCount;
	INT32U	SPD_TotalResendCount;
	INT32U	SPD_TotalRightCount;
	INT32U	SPD_ErrorCount;
	INT8U	SPD_VersionBuf[32];

	INT16U	KEY_SendRight;
	INT16U	KEY_SendCycle;	
	INT32U	KEY_RecCount;
	INT32U	KEY_SendCount;
	INT32U	KEY_ResendCount;
	INT32U	KEY_TotalResendCount;
	INT32U	KEY_TotalRightCount;
	INT8U	KEY_VersionBuf[32];

	INT16U	IO_SendRight;
	INT16U	IO_SendCycle;	
	INT32U	IO_RecCount;
	INT32U	IO_SendCount;
	INT32U	IO_ResendCount;
	INT32U	IO_TotalResendCount;
	INT32U	IO_TotalRightCount;
	INT8U	IO_VersionBuf[32];

	INT32U	DSP_GradeCount;
	INT8U	DSP_VersionBuf[32];
}tsSCIPARA;

extern void InitCOM232_1(void);
extern void InitCOM232_2(void);
extern Uint16 COM232_1(void); 
extern Uint16 COM232_2(void); 

extern void SCI_Init(void);
extern void SPD_ComVT(void);
extern void Dsp_Com(void);
extern void Key_Com(void);
extern void IO_Com(void);

extern INT32U 	SCIA_TxBuf[4096];	//SCIA实际发送数据
extern INT32U 	SCIA_RxBuf[4096];	//SCIA实际接收数据

extern Uint16	ServoOn2Counter;

//extern INT16U Packet_CRC16Get(INT8U *pData, INT16U Length);

#ifdef __cplusplus
}
#endif 	

#endif
