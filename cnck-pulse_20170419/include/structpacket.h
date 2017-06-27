#ifndef _STRUCT_PACKET_H_
#define _STRUCT_PACKET_H_

#include "datatype.h"
#include "StructPoint.h"
#include "common.h"

typedef struct
{
	INT16U VSub:1;		// 1
	INT16U VAdd:1;		// 2
	INT16U IVSub:1;		// 3
	INT16U ZSub:1;		// 4
	INT16U YSub:1;		// 5
	INT16U XAdd:1;		// 6
	INT16U Grade:1;		// 7	
	INT16U XSub:1;		// 8
	INT16U YAdd:1;		// 9
	INT16U ZAdd:1;		//10
	INT16U IVAdd:1;		//11
	INT16U Rsd12:1;		//12
	INT16U Rsd13:1;		//13
	INT16U Rsd14:1;		//14
	INT16U Rsd15:1;		//15
	INT16U Rsd16:1;		//16
}tsbDSP_JOGGRADE;	
typedef union
{
	INT16U all;
	tsbDSP_JOGGRADE sbit;
}tsuDSP_JOGGRADE;

typedef struct
{
	INT16U X:1;			// 1
	INT16U Y:1;			// 2
	INT16U Z:1;			// 3
	INT16U IV:1;			// 4
	INT16U V:1;			// 5
	INT16U G1:1;			// 6
	INT16U G10:1;		// 7	
	INT16U G100:1;		// 8
	INT16U G1000:1;		// 9
	INT16U Rsd10:1;		//10
	INT16U Rsd11:1;		//11
	INT16U Rsd12:1;		//12
	INT16U Rsd13:1;		//13
	INT16U Rsd14:1;		//14
	INT16U Rsd15:1;		//15
	INT16U Rsd16:1;		//16
}tsbDSP_HANDGRADE;
typedef union
{
	INT16U all;
	tsbDSP_HANDGRADE sbit;
}tsuDSP_HANDGRADE;

typedef struct
//typedef __packed struct	
{
	INT8U STDInit;
	INT8U SPCInit;
	INT8U STDMode;
	INT8U SPCMode;
	INT8U NCModeType;
	INT8U ServoOn;
	INT8U DSPPositionGet;
	INT8U FunctionTest1;
	INT8U FunctionTest2;
	INT8U FunctionTest3;
	INT8U FunctionTest4;
	INT8U FunctionTest5;
	INT8U FunctionTest6;
	INT8U FunctionTest7;
	INT8U FunctionTest8;
	INT8U FunctionTest9;
	INT8U FunctionTest10;	
}tsDSP_PACKETIN_INFOEVENT;

typedef struct
//typedef __packed struct	
{
	INT16U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U Packet;
	
	INT8U GradeG0;
	INT8U GradeG1;
	INT8U LEDFlashCtr1;
	INT8U LEDFlashCtr2;
	INT32U	LEDFlashTime1;
	INT32U	LEDFlashTime2;
	INT32U	JOGAxis;
	INT32U	JOGDir;
	INT32S	JOGDistance;
	INT32U	JOGFeedrate;
	
	INT32U InInfo1;
	INT32U InInfo2;
	INT32U OutInfo1;
	INT32U OutInfo2;

	INT32U	NeedFindAxisSign;
    	INT32U  FunctionSelectionSwitch;

	INT32U Reserved1;
	INT32U Reserved2;
	INT32U Reserved3;
	INT32U Reserved4;
	INT32U Reserved5;
	INT32U Reserved6;
	INT32U Reserved7;
	INT32U Reserved8;
	INT32U Reserved9;
	INT32U Reserved10;


	INT8U InfoBuf[48];	//事件结构体
	
	INT32U ComID;
	INT32U PacketCheckCode_B0;
	INT16U CRC;
	INT16U End;
}tsDSP_PACKETIN;

typedef struct
//typedef __packed struct
{
	INT8U SPCSendEnable;
	INT8U STDSendEnable;
	INT8U DSPInAlarm;
	INT8U DSPSingleOver;
	INT8U DSPLocateOver;
	INT8U DSPRunOver;
	INT8U ParameterWriteCompleteSign;
	INT8U HLimitPos;
	
	INT8U HLimitPosAxis1;
	INT8U HLimitPosAxis2;
	INT8U HLimitPosAxis3;
	INT8U HLimitPosAxis4;
	INT8U HLimitPosAxis5;
	INT8U HLimitPosAxis6;
	INT8U HLimitPosAxis7;
	INT8U HLimitPosAxis8;
	INT8U HLimitPosAxis9;
	INT8U HLimitPosAxis10;
	INT8U HLimitPosAxis11;
	INT8U HLimitPosAxis12;
	INT8U HLimitPosAxis13;
	INT8U HLimitPosAxis14;
	INT8U HLimitPosAxis15;
	INT8U HLimitPosAxis16;
	INT8U HLimitPosAxis17;
	INT8U HLimitPosAxis18;
	INT8U HLimitPosAxis19;
	INT8U HLimitPosAxis20;
	INT8U HLimitPosAxis21;
	INT8U HLimitPosAxis22;
	INT8U HLimitPosAxis23;

	INT8U HLimitNeg;
	INT8U HLimitNegAxis1;
	INT8U HLimitNegAxis2;
	INT8U HLimitNegAxis3;
	INT8U HLimitNegAxis4;
	INT8U HLimitNegAxis5;
	INT8U HLimitNegAxis6;
	INT8U HLimitNegAxis7;
	INT8U HLimitNegAxis8;
	INT8U HLimitNegAxis9;
	INT8U HLimitNegAxis10;
	INT8U HLimitNegAxis11;
	INT8U HLimitNegAxis12;
	INT8U HLimitNegAxis13;
	INT8U HLimitNegAxis14;
	INT8U HLimitNegAxis15;
	INT8U HLimitNegAxis16;
	INT8U HLimitNegAxis17;
	INT8U HLimitNegAxis18;
	INT8U HLimitNegAxis19;
	INT8U HLimitNegAxis20;
	INT8U HLimitNegAxis21;
	INT8U HLimitNegAxis22;
	INT8U HLimitNegAxis23;

	INT8U SLimitPos;
	INT8U SLimitPosAxis1;
	INT8U SLimitPosAxis2;
	INT8U SLimitPosAxis3;
	INT8U SLimitPosAxis4;
	INT8U SLimitPosAxis5;
	INT8U SLimitPosAxis6;
	INT8U SLimitPosAxis7;
	INT8U SLimitPosAxis8;
	INT8U SLimitPosAxis9;
	INT8U SLimitPosAxis10;
	INT8U SLimitPosAxis11;
	INT8U SLimitPosAxis12;
	INT8U SLimitPosAxis13;
	INT8U SLimitPosAxis14;
	INT8U SLimitPosAxis15;
	INT8U SLimitPosAxis16;
	INT8U SLimitPosAxis17;
	INT8U SLimitPosAxis18;
	INT8U SLimitPosAxis19;
	INT8U SLimitPosAxis20;
	INT8U SLimitPosAxis21;
	INT8U SLimitPosAxis22;
	INT8U SLimitPosAxis23;

	INT8U SLimitNeg;
	INT8U SLimitNegAxis1;
	INT8U SLimitNegAxis2;
	INT8U SLimitNegAxis3;
	INT8U SLimitNegAxis4;
	INT8U SLimitNegAxis5;
	INT8U SLimitNegAxis6;
	INT8U SLimitNegAxis7;
	INT8U SLimitNegAxis8;
	INT8U SLimitNegAxis9;
	INT8U SLimitNegAxis10;
	INT8U SLimitNegAxis11;
	INT8U SLimitNegAxis12;
	INT8U SLimitNegAxis13;
	INT8U SLimitNegAxis14;
	INT8U SLimitNegAxis15;
	INT8U SLimitNegAxis16;
	INT8U SLimitNegAxis17;
	INT8U SLimitNegAxis18;
	INT8U SLimitNegAxis19;
	INT8U SLimitNegAxis20;
	INT8U SLimitNegAxis21;
	INT8U SLimitNegAxis22;
	INT8U SLimitNegAxis23;

	INT8U AxisRunOverAxis1;
	INT8U AxisRunOverAxis2;
	INT8U AxisRunOverAxis3;
	INT8U AxisRunOverAxis4;
	INT8U AxisRunOverAxis5;
	INT8U AxisRunOverAxis6;
	INT8U AxisRunOverAxis7;
	INT8U AxisRunOverAxis8;
	INT8U AxisRunOverAxis9;
	INT8U AxisRunOverAxis10;
	INT8U AxisRunOverAxis11;
	INT8U AxisRunOverAxis12;
	INT8U AxisRunOverAxis13;
	INT8U AxisRunOverAxis14;
	INT8U AxisRunOverAxis15;
	INT8U AxisRunOverAxis16;
	INT8U AxisRunOverAxis17;
	INT8U AxisRunOverAxis18;
	INT8U AxisRunOverAxis19;
	INT8U AxisRunOverAxis20;	
	INT8U AxisRunOverAxis21;
	INT8U AxisRunOverAxis22;
	INT8U AxisRunOverAxis23;
	INT8U AxisRunOverValid1;
	INT8U AxisRunOverValid2;
	INT8U AxisRunOverValid3;
	INT8U AxisRunOverValid4;
	INT8U AxisRunOverValidAll;
		
	INT8U AxisRefOverAxis1;
	INT8U AxisRefOverAxis2;
	INT8U AxisRefOverAxis3;
	INT8U AxisRefOverAxis4;
	INT8U AxisRefOverAxis5;
	INT8U AxisRefOverAxis6;
	INT8U AxisRefOverAxis7;
	INT8U AxisRefOverAxis8;
	INT8U AxisRefOverAxis9;
	INT8U AxisRefOverAxis10;
	INT8U AxisRefOverAxis11;
	INT8U AxisRefOverAxis12;
	INT8U AxisRefOverAxis13;
	INT8U AxisRefOverAxis14;
	INT8U AxisRefOverAxis15;
	INT8U AxisRefOverAxis16;
	INT8U AxisRefOverAxis17;
	INT8U AxisRefOverAxis18;
	INT8U AxisRefOverAxis19;
	INT8U AxisRefOverAxis20;
	INT8U AxisRefOverAxis21;
	INT8U AxisRefOverAxis22;
	INT8U AxisRefOverAxis23;
	INT8U AxisRefOverValidAll;

	INT8U AxisLoadAxis1;
	INT8U AxisLoadAxis2;
	INT8U AxisLoadAxis3;
	INT8U AxisLoadAxis4;
	INT8U AxisLoadAxis5;
	INT8U AxisLoadAxis6;
	INT8U AxisLoadAxis7;
	INT8U AxisLoadAxis8;
	INT8U AxisLoadAxis9;
	INT8U AxisLoadAxis10;
	INT8U AxisLoadAxis11;
	INT8U AxisLoadAxis12;
	INT8U AxisLoadAxis13;
	INT8U AxisLoadAxis14;
	INT8U AxisLoadAxis15;
	INT8U AxisLoadAxis16;
	INT8U AxisLoadAxis17;
	INT8U AxisLoadAxis18;
	INT8U AxisLoadAxis19;
	INT8U AxisLoadAxis20;	
	INT8U AxisLoadAxis21;
	INT8U AxisLoadAxis22;
	INT8U AxisLoadAxis23;

	INT8U NCModeType;

	INT8U AlmServoAxis1;
	INT8U AlmServoAxis2;
	INT8U AlmServoAxis3;
	INT8U AlmServoAxis4;
	INT8U AlmServoAxis5;
	INT8U AlmServoAxis6;
	INT8U AlmServoAxis7;
	INT8U AlmServoAxis8;
	INT8U AlmServoAxis9;
	INT8U AlmServoAxis10;
	INT8U AlmServoAxis11;
	INT8U AlmServoAxis12;
	INT8U AlmServoAxis13;
	INT8U AlmServoAxis14;
	INT8U AlmServoAxis15;
	INT8U AlmServoAxis16;
	INT8U AlmServoAxis17;
	INT8U AlmServoAxis18;
	INT8U AlmServoAxis19;
	INT8U AlmServoAxis20;
	INT8U AlmServoAxis21;
	INT8U AlmServoAxis22;
	INT8U AlmServoAxis23;

	INT8U AlmEncoderAxis1;
	INT8U AlmEncoderAxis2;
	INT8U AlmEncoderAxis3;
	INT8U AlmEncoderAxis4;
	INT8U AlmEncoderAxis5;
	INT8U AlmEncoderAxis6;
	INT8U AlmEncoderAxis7;
	INT8U AlmEncoderAxis8;
	INT8U AlmEncoderAxis9;
	INT8U AlmEncoderAxis10;
	INT8U AlmEncoderAxis11;
	INT8U AlmEncoderAxis12;
	INT8U AlmEncoderAxis13;
	INT8U AlmEncoderAxis14;
	INT8U AlmEncoderAxis15;
	INT8U AlmEncoderAxis16;
	INT8U AlmEncoderAxis17;
	INT8U AlmEncoderAxis18;
	INT8U AlmEncoderAxis19;
	INT8U AlmEncoderAxis20;	
	INT8U AlmEncoderAxis21;
	INT8U AlmEncoderAxis22;
	INT8U AlmEncoderAxis23;
	
	INT8U AlmDSPRamIn;
	INT8U AlmDSPRamOut;
	INT8U AlmDSPCode;
	INT8U AlmDSPTrack;
	INT8U AlmDSPInterpolate;
	
	INT8U AlmHLimitPos;
	INT8U AlmHLimitNeg;
	INT8U AlmSLimitPos;
	INT8U AlmSLimitNeg;
	INT8U AlmCompDataCheck;
	INT8U AlmOpticalFlashTriggerBreak;
	INT8U AlmCameraShutterTriggerBreak;
	INT8U AlmLensPositionBreak;	
}tsDSP_PACKETOUT_INFOEVENT;

typedef struct
//typedef __packed struct
{
	INT16U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U Packet;
	INT32U STDRunCount;
	INT32U SPCRunCount;
	INT32U STDSendCount;//
    	INT32U SPCSendCount;//
    	INT32U MicroEDataBaseStartCnt;
	UINT40MODULE FeedReal;
	INT32U FeedSet;

	INT32S ABSMachineCoordinateAxis1;
	INT32S ABSMachineCoordinateAxis2;
	INT32S ABSMachineCoordinateAxis3;
	INT32S ABSMachineCoordinateAxis4;
	INT32S ABSMachineCoordinateAxis5;
	INT32S ABSMachineCoordinateAxis6;
	INT32S ABSMachineCoordinateAxis7;
	INT32S ABSMachineCoordinateAxis8;
	INT32S ABSMachineCoordinateAxis9;
	INT32S ABSMachineCoordinateAxis10;
	INT32S ABSMachineCoordinateAxis11;
	INT32S ABSMachineCoordinateAxis12;
	INT32S ABSMachineCoordinateAxis13;
	INT32S ABSMachineCoordinateAxis14;
	INT32S ABSMachineCoordinateAxis15;
	INT32S ABSMachineCoordinateAxis16;
	INT32S ABSMachineCoordinateAxis17;
	INT32S ABSMachineCoordinateAxis18;
	INT32S ABSMachineCoordinateAxis19;
	INT32S ABSMachineCoordinateAxis20;
	INT32S ABSMachineCoordinateAxis21;
	INT32S ABSMachineCoordinateAxis22;
	INT32S ABSMachineCoordinateAxis23;
	INT32S ABSMachineCoordinateAxis24;
	INT32S ABSMachineCoordinateAxis25;

	INT32S MachineCoordinateAxis1;
	INT32S MachineCoordinateAxis2;
	INT32S MachineCoordinateAxis3;
	INT32S MachineCoordinateAxis4;
	INT32S MachineCoordinateAxis5;
	INT32S MachineCoordinateAxis6;
	INT32S MachineCoordinateAxis7;
	INT32S MachineCoordinateAxis8;
	INT32S MachineCoordinateAxis9;
	INT32S MachineCoordinateAxis10;
	INT32S MachineCoordinateAxis11;
	INT32S MachineCoordinateAxis12;
	INT32S MachineCoordinateAxis13;
	INT32S MachineCoordinateAxis14;
	INT32S MachineCoordinateAxis15;
	INT32S MachineCoordinateAxis16;
	INT32S MachineCoordinateAxis17;
	INT32S MachineCoordinateAxis18;
	INT32S MachineCoordinateAxis19;
	INT32S MachineCoordinateAxis20;
	INT32S MachineCoordinateAxis21;
	INT32S MachineCoordinateAxis22;
	INT32S MachineCoordinateAxis23;
	INT32S MachineCoordinateAxis24;
	INT32S MachineCoordinateAxis25;

	INT8U DspVersionBuf[20];
	INT8U KeyVersionBuf[20];
	INT8U IOVersionBuf[20];
	INT32U PacketCheckCodeBuf[10];
	
	INT32U HandGrade;	
	INT32U HandPulse;	

	INT32U IOInInfo1;
	INT32U IOInInfo2;
	INT32U IOOutInfo1;
	INT32U IOOutInfo2;
	INT32U IOComPass;
	INT32U IOComFail;

	INT32U MainCmd1;
	INT32U MainCmd2;
	INT32U MainStatus1;
	INT32U MainStatus2;
	
	INT32U TrackRunOver;	
	INT32U InterpolationOver;
	INT32U CompensationDataCheckError;
	INT32U Reserved1;
	
	INT32U Reserved2;
	INT32U Reserved3;
	INT32U Reserved4;
	INT32U Reserved5;
	INT32U Reserved6;
	INT32U Reserved7;
	INT32U Reserved8;
	INT32U Reserved9;
	INT32U Reserved10;
	INT32U Reserved11;	
	
	INT32U SPDComPass;
	INT32U SPDComFail;	
	
	INT8U InfoBuf[256];		//事件结构体
	//INT8U AlmInfo[8];
	
	INT32U ComID;
	INT16U CRC;
	INT16U End;
}tsDSP_PACKETOUT;

typedef struct
//typedef __packed struct
{		 
	INT32U ComID;
	INT32S CodeTotal;
	INT32S CodeBuf[128*16];	 
}tsDSP_CODESTD;

typedef struct
//typedef __packed struct
{	
	INT32S PosAxis1;
	INT32S PosAxis2;
	INT32S PosAxis3;
	INT32S PosAxis4;
	INT32S PosAxis5;
	INT32S PosAxis6;
	INT32S PosAxis7;
	INT32S PosAxis8;
	INT32S PosAxis9;
	INT32S PosAxis10;
	INT32S PosAxis11;
	INT32S PosAxis12;
	INT32S PosAxis13;
	INT32S PosAxis14;
	INT32S PosAxis15;
	INT32S PosAxis16;
	INT32S PosAxis17;
	INT32S PosAxis18;
	INT32S PosAxis19;
	INT32S PosAxis20;
	INT32S PosAxis21;
	INT32S PosAxis22;
	INT32S PosAxis23;
	INT32S PosAxis24;
	INT32S PosAxis25;
	
	INT32U Feed;
	INT32U CountID;
	INT32U PacketCheckCode_B4;

	INT8U Event1;
	INT8U Event2;
	INT8U Event3;
	INT8U Event4;
}tsDSP_CODESPC_SUB;

typedef struct
//typedef __packed struct
{		 
	INT32U ComID;
	INT32S CodeTotal;
	tsDSP_CODESPC_SUB sInfoBuf[64];
	//INT32S CodeBuf[64][16];	 
}tsDSP_CODESPC;

typedef struct
//typedef __packed struct
{		 
	INT32S SendCount;
	INT32S CMDMain;
	INT32S CMDSub;	
	tsPOINT5A sPointS;
	tsPOINT5A sPointE;
	tsPOINT3A sPointC;
	INT32S AngleS;
	INT32S AngleE;
	INT32S Stotal;
	INT32S FeedRate;
	INT32S Vm;
	INT32S Ve;
}tsDSP_B5SUB;

typedef struct
//typedef __packed struct
{
	/*
	INT32S N;
	INT32S Pointer_StartCode;
	INT32S Pointer_EndCode;
	INT32S Pointer_FirstInflexionCode;
	INT32S Pointer_SecondInflexionCode;
	INT32S Ss;
	INT32S Stotal;
	INT32S CurrentStotal;
	INT32S CurrentStotalLast;
	INT32S Snow;
	INT32S DeltaSnow;
	INT32S I;
	INT32S TotalN;
	INT32S Vs;
	INT32S Vm;
	INT32S VmReal;
	INT32S Ve;
	INT32S VNow;
	INT32S Vmin;
	INT32S VChangeSign;
	INT32S CurveClassNumber;
	INT32S StotalReal;
	INT32S StotalLast;
	INT32S Sac;
	INT32S Sm;
	INT32S Sde;
	INT32S SA;
	INT32S S1;
	INT32S S2;
	INT32S S3;
	INT32S S4;
	INT32S S5;
	INT32S T1;
	INT32S T2;
	INT32S T3;
	INT32S T4;
	INT32S T5;
	INT32S DeltaT1;
	INT32S DeltaT2;
	INT32S DeltaT3;
	INT32S DeltaT4;
	INT32S DeltaT5;
	INT32S a_ac;
	INT32S a_de;
	INT32S CurrentT;
	tsPOINT6A sNowValue;
	tsPOINT6A sOldValue;
	tsPOINT6A sNowPointMACPulse;
	tsPOINT6A sNowPointMACPulseOut;
	tsPOINT6A sEndPointMACPulseOut;
	INT32S RealSendCount;
	INT32S RealRunCount;
	tsPOINT6A sServoUnit;
	tsPOINT6A sSerVoPosition;
	INT32S Bak[56];
	*/
}tsDSP_B6SUB;

typedef struct
//typedef __packed struct
{
	INT32U ComID;
	INT32S CodeTotal;
	tsDSP_B6SUB sInfoB6;
	tsDSP_B5SUB sInfoB5Buf[32];
}tsDSP_INFODGN;

typedef struct
//typedef __packed struct
{	
	INT8U Fix;
	INT8U Port;			
	INT8U Protocal;		
	INT8U Function;		
	INT8U Command;		
	INT8U Speed1;
	INT16U Speed23;
	INT16U Angle;
	INT16U Torque;
	INT16U Check;	
}tsSPD_PACKETIN;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Port;			
	INT8U Protocal;		
	INT8U Function;		
	INT8U Command;		
	INT8U Speed1;
	INT16U Speed23;
	INT16U Angle;
	INT8U Rsd9;
	INT8U Rsd10;
	INT16U Check;	
}tsSPD_PACKETOUT;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U ToolType;
	INT8U ToolTotal;
	INT8U ToolTurn;
	INT8U ToolDelayR;
	INT8U ToolDelayT;
	INT32U SignIn1;
	INT32U SignIn2;
	INT32U SignOut1;
	INT32U SignOut2;
	INT32U OutInfo1;
	INT32U OutInfo2;
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsIO_PACKETOUT;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U State;
	INT8U Location;
	INT8U Step;
	INT8U Rsd1;
	INT8U Rsd2;
	INT32U InInfo1;
	INT32U InInfo2;
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsIO_PACKETIN;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;	
	INT8U Version[21];
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsIO_PACKETVERSION;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U KeyID;
	INT16U JogID;
	INT8U GradeG0;
	INT8U GradeG1;
	INT8U GradeS;
	INT8U KeyPress;
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsKEY_PACKETINP;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;
	INT8U Version[21];
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsKEY_PACKETVERSION;

typedef struct
//typedef __packed struct
{
	INT8U Fix;
	INT8U Head;
	INT16U Length;
	INT8U Port;
	INT8U Protocal;
	INT8U Command;	
	INT8U KeyID;
	INT16U LedMode;
	INT16U LedOut;
	INT16U LedState;
	INT32U ComID;
	INT16U CRC;
	INT8U End;
}tsKEY_PACKETOUTP;


//and envent is a hint to tell others to receive message

//#define DSP_EVENT_KEEPALIVE 0
//#define DSP_EVENT_KEY    1
//#define DSP_EVENT_PKGB0  2


#endif
