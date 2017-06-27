#ifndef COMMON_H
#define COMMON_H



#ifndef DATA_TYPES
#define DATA_TYPES
typedef unsigned char    Uint8;
typedef unsigned short   Uint16;
typedef unsigned int     Uint32;
typedef unsigned long    Uint40;
typedef short            Int16;
typedef int              Int32;
typedef float           	float32;
typedef double	     	float64;
#endif

#define FALSE  0
#define False  0
#define false  0

#define TRUE  1
#define True  1
#define true  1

#define NOCODERUN		0
#define STDCODERUN		1
#define SPCCODERUN		2

#define Over_Signal_Invalid	0
#define Over_Signal_Valid	1
#define Alarm_Signal_Invalid	0
#define Alarm_Signal_Valid	1
#define Inposition_Signal_Invalid	1
#define Inposition_Signal_Valid	0
#define HardLimit_Signal_Invalid	0
#define HardLimit_Signal_Valid	1
#define FindRefSign_Invalid	0
#define FindRefSign_Valid		1


#define	GPIO_IN_Valid 1
#define	GPIO_IN_Invalid 0

#define	GPIO_OUT_OPEN 0
#define	GPIO_OUT_CLOSE 1

#define LensMotionNormal 0
#define LensMotionCompensationRun 1
#define LensMotionCompensationOver 2

#define IntXX Int32
#define UintXX Uint32

#define  RealTimeRun		0		// ÊµÊ±ÔËÐÐ
#define  SimulateRun		1		// ·ÂÕæÔËÐÐ

#define 		STDGCODE_MOD	1000
#define 		SPCGCODE_MOD	11
#define			ADDGCODEINCUBATOR_MOD	21
#define 		COMPBUFFERLENGTH	1000


#define 	VNowMIN		0.0001	// 当前实际运行速度最小值. (mm/s)
#define	ShortLineLengh	0.0055	// 异常小线段值定义unit:mm
#define	MovePositive	1		// 正向移动
#define	MoveNegtive	0		// 负向移动

//CH0603
#define 	StaticTrackLimit		0.5		//静态轨迹判定界限0.5mm  CH0603
#define 	Max_StaticErrorTimes 1000	//静态轨迹判定周期约1s

#define  REF_XYZ			0	//×ø±êÏµÎª3 ×ø±ê
#define  REF_XYZA		1	//×ø±êÏµÎª3+A ×ø±ê
#define  REF_XYZB		2	//×ø±êÏµÎª3+B ×ø±ê
#define  REF_XYZC		3	//×ø±êÏµÎª3+C ×ø±ê


#define DA_CH1	1
#define DA_CH2	2
#define DA_CH3	3
#define DA_CH4	4
#define DA_CH5	5
#define DA_CH6	6
#define DA_CH7	7
#define DA_CH8	8
#define DA_CH9	9


#define  	INTERPOLATIONBUFLENGTH 100
#define  	SPC_INTERPOLATIONBUFLENGTH 40
  
typedef struct
{
	Int32 Axis1;
	Int32 Axis2;
	Int32 Axis3;
	Int32 Axis4;
	Int32 Axis5;
	Int32 Axis6;	
	Int32 Axis7;
	Int32 Axis8;
	Int32 Axis9;
	Int32 Axis10;
	Int32 Axis11;
	Int32 Axis12;
	Int32 Axis13;
	Int32 Axis14;
	Int32 Axis15;
	Int32 Axis16;	
	Int32 Axis17;
	Int32 Axis18;
	Int32 Axis19;
	Int32 Axis20;
	Int32 Axis21;
	Int32 Axis22;
	Int32 Axis23;
	Int32 Axis24;
	Int32 Axis25;
}INT40AXIS;  


typedef struct
{
	Uint32 Axis1;
	Uint32 Axis2;
	Uint32 Axis3;
	Uint32 Axis4;
	Uint32 Axis5;
	Uint32 Axis6;	
	Uint32 Axis7;
	Uint32 Axis8;
	Uint32 Axis9;
	Uint32 Axis10;
	Uint32 Axis11;
	Uint32 Axis12;
	Uint32 Axis13;
	Uint32 Axis14;
	Uint32 Axis15;
	Uint32 Axis16;	
	Uint32 Axis17;
	Uint32 Axis18;
	Uint32 Axis19;
	Uint32 Axis20;
	Uint32 Axis21;
	Uint32 Axis22;
	Uint32 Axis23;
	Uint32 Axis24;
	Uint32 Axis25;
}UINT40AXIS;     

typedef struct
{
	Uint16 Axis1;
	Uint16 Axis2;
	Uint16 Axis3;
	Uint16 Axis4;
	Uint16 Axis5;
	Uint16 Axis6;	
	Uint16 Axis7;
	Uint16 Axis8;
	Uint16 Axis9;
	Uint16 Axis10;
	Uint16 Axis11;
	Uint16 Axis12;
	Uint16 Axis13;
	Uint16 Axis14;
	Uint16 Axis15;
	Uint16 Axis16;	
	Uint16 Axis17;
	Uint16 Axis18;
	Uint16 Axis19;
	Uint16 Axis20;
	Uint16 Axis21;
	Uint16 Axis22;
	Uint16 Axis23;
	Uint16 Axis24;
	Uint16 Axis25;
}UINT16AXIS;     

typedef struct
{
	Int16 Axis1;
	Int16 Axis2;
	Int16 Axis3;
	Int16 Axis4;
	Int16 Axis5;
	Int16 Axis6;	
	Int16 Axis7;
	Int16 Axis8;
	Int16 Axis9;
	Int16 Axis10;
	Int16 Axis11;
	Int16 Axis12;
	Int16 Axis13;
	Int16 Axis14;
	Int16 Axis15;
	Int16 Axis16;	
	Int16 Axis17;
	Int16 Axis18;
	Int16 Axis19;
	Int16 Axis20;
	Int16 Axis21;
	Int16 Axis22;
	Int16 Axis23;
	Int16 Axis24;
	Int16 Axis25;
}INT16AXIS;     

typedef struct
{
	Uint8 Axis1;
	Uint8 Axis2;
	Uint8 Axis3;
	Uint8 Axis4;
	Uint8 Axis5;
	Uint8 Axis6;	
	Uint8 Axis7;
	Uint8 Axis8;
	Uint8 Axis9;
	Uint8 Axis10;
	Uint8 Axis11;
	Uint8 Axis12;
	Uint8 Axis13;
	Uint8 Axis14;
	Uint8 Axis15;
	Uint8 Axis16;	
	Uint8 Axis17;
	Uint8 Axis18;
	Uint8 Axis19;
	Uint8 Axis20;
	Uint8 Axis21;
	Uint8 Axis22;
	Uint8 Axis23;
	Uint8 Axis24;
	Uint8 Axis25;
}UINT8AXIS;   

typedef struct
{
	float64 Axis1;
	float64 Axis2;
	float64 Axis3;
	float64 Axis4;
	float64 Axis5;
	float64 Axis6;	
	float64 Axis7;
	float64 Axis8;
	float64 Axis9;
	float64 Axis10;
	float64 Axis11;
	float64 Axis12;
	float64 Axis13;
	float64 Axis14;
	float64 Axis15;
	float64 Axis16;	
	float64 Axis17;
	float64 Axis18;
	float64 Axis19;
	float64 Axis20;
	float64 Axis21;
	float64 Axis22;
	float64 Axis23;
	float64 Axis24;
	float64 Axis25;
}float64AXIS;     

typedef struct
{
	float64 Axis1;
	float64 Axis2;
	float64 Axis3;
	float64 Axis4;
	float64 Axis5;
	float64 Axis6;	
	float64 Axis7;
	float64 Axis8;
	float64 Axis9;
	float64 Axis10;
	float64 Axis11;
	float64 Axis12;
	float64 Axis13;
	float64 Axis14;
	float64 Axis15;
	float64 Axis16;	
	float64 Axis17;
	float64 Axis18;
	float64 Axis19;
	float64 Axis20;
	float64 Axis21;
	float64 Axis22;
	float64 Axis23;
	float64 Axis24;
	float64 Axis25;
}VECTOR;    
typedef struct
{
	Int32 Module1;
	Int32 Module2;
	Int32 Module3;
	Int32 Module4;
	Int32 Module5;
	Int32 Module6;	
	Int32 Module7;
	Int32 Module8;
	Int32 Module9;
	Int32 Module10;	
	Int32 Module11;
	Int32 Module12;
}INT40MODULE;  


typedef struct
{
	Uint32 Module1;
	Uint32 Module2;
	Uint32 Module3;
	Uint32 Module4;
	Uint32 Module5;
	Uint32 Module6;	
	Uint32 Module7;
	Uint32 Module8;
	Uint32 Module9;
	Uint32 Module10;	
	Uint32 Module11;
	Uint32 Module12;
}UINT40MODULE;     

typedef struct
{
	Uint16 Module1;
	Uint16 Module2;
	Uint16 Module3;
	Uint16 Module4;
	Uint16 Module5;
	Uint16 Module6; 
	Uint16 Module7;
	Uint16 Module8;
	Uint16 Module9;
	Uint16 Module10; 
	Uint16 Module11;
	Uint16 Module12;
}UINT16MODULE;     

typedef struct
{
	Int16 Module1;
	Int16 Module2;
	Int16 Module3;
	Int16 Module4;
	Int16 Module5;
	Int16 Module6; 
	Int16 Module7;
	Int16 Module8;
	Int16 Module9;
	Int16 Module10; 
	Int16 Module11;
	Int16 Module12;
}INT16MODULE;     

typedef struct
{
	Uint8 Module1;
	Uint8 Module2;
	Uint8 Module3;
	Uint8 Module4;
	Uint8 Module5;
	Uint8 Module6; 
	Uint8 Module7;
	Uint8 Module8;
	Uint8 Module9;
	Uint8 Module10; 
	Uint8 Module11;
	Uint8 Module12;
}
UINT8MODULE;   

typedef struct
{
	float64 Module1;
	float64 Module2;
	float64 Module3;
	float64 Module4;
	float64 Module5;
	float64 Module6; 
	float64 Module7;
	float64 Module8;
	float64 Module9;
	float64 Module10;
	float64 Module11;
	float64 Module12;
}float64MODULE;     

typedef struct
{
	float64 Module1;
	float64 Module2;
	float64 Module3;
	float64 Module4;
	float64 Module5;
	float64 Module6; 
	float64 Module7;
	float64 Module8;
	float64 Module9;
	float64 Module10; 
	float64 Module11;
	float64 Module12;
}VECTOR_MODULE;     

typedef struct
{
	Uint32	Pointer_StartCode;								//分解代码，区域起始代码段指针
	Uint32	Pointer_EndCode;								//分解代码，区域终止代码段指针
	Uint32	Pointer_FirstInflexionCode;				//分解代码，区域一次拐点代码段指针
	Uint32	Pointer_SecondInflexionCode;		//分解代码，区域二次拐点代码段指针
	float64MODULE	Ss_Module;		
	float64	Stotal;						//区域内所有代码总运行长度 mm
	float64MODULE 	Stotal_Module;
	float64MODULE 	CurrentStotal_Module;
	float64MODULE 	CurrentStotalLast_Module;
	float64MODULE 	Snow_Module;
	float64MODULE 	DeltaSnow_Module;
	Uint32	I;		//插补序号 [1,+无穷)
	UINT40MODULE I_Module;

	UINT40MODULE TotalN_Module;	//×Ü²å²¹Êý	TotalN=T5/Tsample
	float64MODULE	Vs_Module;		//µ±Ç°ÔËÐÐG´úÂëÆðµãÔËÐÐËÙ¶È. (mm/s)
	float64	Vm;		// 当前运行G代码设定运行速度. (mm/s)
	float64MODULE	Vm_Module;		// µ±Ç°ÔËÐÐG´úÂëÉè¶¨ÔËÐÐËÙ¶È. (mm/s)
	float64MODULE	VmReal_Module;	// µ±Ç°ÔËÐÐG´úÂëÊµ¼Ê×î¸ßÔËÐÐËÙ¶È. (mm/s)
	float64MODULE	Ve_Module;			// µ±Ç°ÔËÐÐG´úÂëÖÕµãÔËÐÐËÙ¶È. (mm/s)
	float64MODULE 	VNow_Module;		// µ±Ç°Êµ¼ÊÔËÐÐËÙ¶È. (mm/s)
	float64 			VMin;		// µ±Ç°×îÐ¡ÔËÐÐËÙ¶È. (mm/s)
	float64MODULE 	VMin_Module;		// µ±Ç°×îÐ¡ÔËÐÐËÙ¶È. (mm/s)
	INT16U  			VChangeSign;	//ËÙ¶Èµµ±ä»¯±êÖ¾£¬ÐèÖØÐÂ¼ÆËã¼Ó¼õËÙÇúÏß
	float64MODULE 	SnowLast_Module;		//µ±Ç°´úÂëÉÏ´Î²å²¹×ÜÔËÐÐ³¤¶È mm
	float64MODULE	Sac_Module;				//µ±Ç°´úÂë¼ÓËÙ¹ý³ÌËùÐè¾àÀë(mm)
	float64MODULE	Sm_Module;					//µ±Ç°´úÂëÎÈËÙ¹ý³ÌËùÐè¾àÀë(mm)
	float64MODULE	Sde_Module;				//µ±Ç°´úÂë¼õËÙ¹ý³ÌËùÐè¾àÀë(mm)
	float64MODULE	SA_Module;					//ÆðµãÒÑÓÐµÄ²å²¹¾àÀë mm     ***Í£ÓÃ
	float64MODULE	S1_Module;					//Å×ÎïÏßAB»ý·ÖÃæ»ý mm
	float64MODULE	S2_Module;					//Å×ÎïÏßBC»ý·ÖÃæ»ý mm
	float64MODULE	S3_Module;					//Ö±ÏßCD»ý·ÖÃæ»ý mm
	float64MODULE	S4_Module;					//Å×ÎïÏßDE»ý·ÖÃæ»ý mm
	float64MODULE	S5_Module;					//Å×ÎïÏßEF»ý·ÖÃæ»ý mm
	float64MODULE	T1_Module;					//´ÓAµ½BËùÓÃÊ±¼ä	s
	float64MODULE	T2_Module;					//´ÓAµ½CËùÓÃÊ±¼ä	s
	float64MODULE	T3_Module;					//´ÓAµ½DËùÓÃÊ±¼ä	s
	float64MODULE	T4_Module;					//´ÓAµ½EËùÓÃÊ±¼ä	s
	float64MODULE	T5_Module;					//´ÓAµ½FËùÓÃÊ±¼ä	s
	float64MODULE	DeltaT1_Module;			//ABËùÓÃÊ±¼ä	s
	float64MODULE	DeltaT2_Module;			//BCËùÓÃÊ±¼ä	s
	float64MODULE	DeltaT3_Module;			//CDËùÓÃÊ±¼ä	s
	float64MODULE	DeltaT4_Module;			//DEËùÓÃÊ±¼ä	s
	float64MODULE	DeltaT5_Module;			//EFËùÓÃÊ±¼ä	s

	float64MODULE 	a_ac_Module;				
	float64MODULE 	a_de_Module;
	
	float64MODULE 	CurrentT_Module;
	float64			CurrentT;
	VECTOR	Cosine;				// ²å²¹Öá·½Ïò¼Ð½ÇÓàÏÒ
	INT40AXIS	DeltaStep;			// Ö±ÏßÖÐÃ¿¶ÎxÓ¦×ß¾àÀë  unit:pulse(Êä³öÖ¸Áîµ¥Î»)
	INT40AXIS	NowValue;			// µ±Ç°²å²¹µã×ø±êÎ»ÖÃÖµ--×îÐ¡ÊäÈëµ¥Î»
	INT40AXIS	OldValue;			// Ç°Ò»²å²¹µã×ø±êÎ»ÖÃÖµ--×îÐ¡ÊäÈëµ¥Î»
	INT40AXIS	NowPointMACHPulseOut;
	// 前一插补点MACH 坐标逆补偿输出位置值(输出指令单位)
	INT40AXIS	OldPointMACHPulseOut;	
	INT40AXIS	EndPointMachPulseOut;

	UINT16MODULE SpeedState;	//区域速度状态
	Uint16		ri;						//200 缓冲区运行取模指针
	Uint16		ZeroPulseSign;				//零脉冲标志,1为零脉冲
}AREA_CONTROL;

//记忆代码的移动方向，用于反方向减速处理
typedef struct
{
UINT16AXIS	MoveSign;				// 1-positive 0-negtive 
INT40AXIS	Delta;					// 代码变化量um
float64		TotalLength;			// 代码总长mm
}PRECALMOVESIGN;

//CH0512
typedef struct
{
	Uint32 		SendCount;		//DSP当前运行原代码位置指针，返回给ARM用于显示及其它用途
	Uint32 				CMDMain,SubCMD1,SubCMD2;		
	INT40AXIS	StartPoint,EndPoint; //起点,终点,圆心点  单位：线性轴最小输入单位
	float64AXIS	DeltaPoint;		//起点与终点的相对距离 单位: mm  
	float64 	Stotal;			//当前G代码长度  单位:mm   正值
	float64MODULE 		Stotal_Module;			
	float64 	Feedrate;				//进给速度,单位:mm/s；IV轴:degree/s
	float64MODULE 		Feedrate_Module;				
	float64 	Vm;				//根据档位确定的理论进给速度,单位:mm/s
	float64MODULE 		Vm_Module;			
	float64 	Ve;							//终点允许速度,单位:mm/s
	float64MODULE 		Ve_Module;						
	float64MODULE 		Vveer_Module;	

	VECTOR  				StPointVector;		
	VECTOR  				EndPointVector;	
	INT16U				MoveCodeSign;
	Uint16				VeCalSign;		
	Uint16				VmCalSign;		
	Uint8				PositionStep;
}GCODE;

//总体标志和变量用结构体
typedef struct
{
	Uint16 	NCSign;  
	// 0:通常状态  1:标准代码；2:特殊代码-JOG HANDLE REF；INI=0
	
	//速度控制相关参数
	Uint16	LastFeedrateOverride;		//上次进给速度档位
	Uint16	LastRapidFeedrateOverride;	//上次快进速度档位
	Uint16 	FeedrateOverride;			//进给档位	[0-20]
	Uint16  RapidFeedrateOverride;		//G0快速进给档位	[0-20]
	//CH0506 GCODE	LastMoveGCode;			//暂存上条发送运动G代码结构内容
	GCODE	STDLastMoveGCode;			//暂存上条发送运动G代码结构内容
	GCODE	SPCLastMoveGCode;			//暂存上条发送运动G代码结构内容

	VECTOR	LastMoveCodeEndVector;	//上一条运动G代码终点方向矢量(代码预处理用)
	Uint16	EveryAxisCoinValid;		// 各轴COIN 都有效标志
	Uint16	EveryAxisCoinValid1;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid2;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid3;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid4;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisOverValid;		// 各轴OVER 都有效标志
	Uint16	EveryAxisOverValid1;		// ¸÷ÖáOVER ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisOverValid2;		// ¸÷ÖáOVER ¶¼ÓÐÐ§±êÖ¾
	
	// 坐标跟踪相关参数
	Uint16 	GetMachCoordinateRight;	//  1:获得机床坐标正确

INT40AXIS	AbsoluteEncoderValue_PSO;	
INT40AXIS	ABS_Coordinate;			// 当前绝对坐标值unit:最小输入单位
INT40AXIS	ABSORG_M_Coordinate;	// 当前绝对坐标原点的机床坐标值unit:最小输入单位
INT40AXIS	ABSORG_Coordinate;	// µ±Ç°¾ø¶Ô×ø±êÔ­µãµÄ»ú´²×ø±êÖµunit:×îÐ¡ÊäÈëµ¥Î»
UINT16AXIS	MoveSign;				// 当前运动方向1-positive 0-negtive 
UINT16AXIS	TheoryMoveSign;			// 当前代码理论方向1-positive 0-negtive 

Uint16	LastCodeOverSign;			//最后一条代码走完标志  1－走完
	GCODE		*LastGCodePointer;
	//非线性误差补偿	CH0701

	Uint16		Int4IdleSign_1;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾
	Uint16		Int4IdleSign_2;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾
	//Uint32		OverideCnt;
	Uint32		InterpolationSendCnt_1;		//缓冲区发送条数
	Uint32		InterpolationRunCnt_1;		//»º³åÇøÔËÐÐÌõÊý
	Uint32		InterpolationSendCnt_2;		//»º³åÇø·¢ËÍÌõÊý
	Uint32		InterpolationRunCnt_2;		//»º³åÇøÔËÐÐÌõÊý

	Uint8		ParameterReadCompleteSign;
	Uint8		CameraNeedCaptureSign;			//Ϡ??ШҪńՕ?ꖾ

	Uint8		CameraScanSign;			//Ϡ??ɨè?ꖾ
	Uint8		CameraStaticCaptureSign;			//Ϡ????ֹńՕ?ꖾ
	Uint8		MicroEScanSign;			//MicroEɨè?ꖾ
	Uint8		CameraSoftTriggerLedFlash;			
	Uint8		OpticalFlashSign;			//Ɂ?ⵆɁ?ⱪ־
	Uint8		LensMotionAverageSign;
		
	Uint32		MicroEDataBaseStartCnt;
	Uint32		MicroEDataBaseEndCnt;
	Uint32		MicroEDataBaseTotalCnt;	

	INT8U	LEDFlashCtr1;
	INT8U 	LEDFlashCtr2;
	INT32U	LEDFlashTime1;
	INT32U	LEDFlashTime2;

	float64		JOGDistance;	
	float64		JOGFeedrate;
	Uint8		JOGDir;
	Uint8		JOGAxis;

	Uint8		LensMotionOverSign;
	
	UINT16AXIS	EncoderCompPt;				//?«Ʒ?ǏߐԲ????Ƌ㊽שλփָի
	UINT16AXIS	PulseOutCompPt;				//¶?劤??ߐԲ????Ƌ㊽שλփָի
}OVERALLSIGN;

typedef struct
{	
	float64		Tsample;					// 0				
	float64		Tsample1;					
	Uint8		SlaveMAX;					// 1
	float64		TrackRunOutRangeSQR;		// 2
	float64MODULE a_SET_Module;				// 3
	float64MODULE a_A4_SET_Module;			// 4
	float64 VeerDeltaV;					// 5	
	float64 NicetyVeerDeltaV;				// 6
	float64 VeerDeltaT;					// 7
	float64 NicetyVeerDeltaT;				// 8
	UINT16AXIS REFStopVariable;				// 9

	UINT16AXIS	EncoderCheckChoose;		// 10 (bit0~31)
	INT16AXIS	RefDir;					// 11 (bit0~31)
	INT32U LinearAxisMinUnit;				// 12 
	INT32U LinearAxisOutUnitEQU;			// 13	
	float64 	UnitTo_mm;					//mm/unit
	float64 G0Speed;						// 14	//mm/s
	float64 G0Speed_2;					// 15	//mm/s
	float64MODULE G0Speed_Module;		// 16-23	//mm/s
	float64 G1Speed;						// 24	//mm/s
	float64 G1Speed_2;					// 25	//mm/s
	float64MODULE G1Speed_Module;		// 26-33	//mm/s

	float64 SRefSpeed;					// 34	//mm/s
	float64 SRefSpeedBack;				// 35	//mm/s
	float64 SRefBack;						// 36
	float64 SRefSpeed_2;					// 37	//mm/s
	float64 SRefSpeedBack_2;				// 38	//mm/s
	float64 SRefBack_2;					// 39

	INT32U FunctionSelect01;				// 40
	INT32U FunctionSelect02;				// 41
	INT32U FunctionSelect03;				// 42
	INT32U FunctionSelect04;				// 43
	INT32U FunctionSelect05;				// 44
	INT32U FunctionSelect06;				// 45
	INT32U FunctionSelect07;				// 46
	INT32U FunctionSelect08;				// 47
	INT32U FunctionSelect09;				// 48
	INT32U FunctionSelect10;				// 49

	INT40AXIS	MotorChangeDir;
	INT40AXIS	EncoderRDDir;
	INT40AXIS	AxisResolution;
	INT40AXIS	CoordORG;
	float64AXIS	MAXSpeed;
	float64AXIS	REFStopPosition;
	INT40AXIS	SLimitPos;
	INT40AXIS 	SLimitNeg;
		
	Uint16	RunState;	
	Uint8	InPositionSign;
	Uint16	PULSE_PER_UM;	//[1-100]  每um相当脉冲数   pulse/um
	Uint16	PWM_PeriodRegister_MIN;			// PWM  最小写入值
	Uint16	PWM_PeriodRegister_MAX;			// PWM  最大写入值
	Uint16	PWM_PeriodRegister_ZeroPeriod;	// LPM为0时PWM  写入值
	
	Uint16	ReferenceFrameMode;		//坐标系模型

	INT40AXIS PositionCoordinate1; 
	INT40AXIS PositionCoordinate2; 
	INT40AXIS PositionCoordinate3; 
	INT40AXIS PositionCoordinate4; 
	INT40AXIS PositionCoordinate5; 
	INT40AXIS PositionCoordinate6; 
	INT40AXIS PositionCoordinate7; 
	INT40AXIS PositionCoordinate8; 
	INT40AXIS PositionCoordinate9; 
	INT40AXIS PositionCoordinate10; 	

	INT40AXIS SafeCoordinate;
	INT40AXIS BackCoordinate;
	INT40AXIS OffsetCoordinate;

	INT32U  LEDFlashTime1;
	INT32U  LEDFlashTime2;
	
	UINT16AXIS	AxisCompMaxL;	
	UINT16AXIS	CompensationDirection;
	
	UINT40AXIS	Gap;				 			//֡?䏶ֵ,??λ: ׮Сʤȫ??λ
	UINT40AXIS	GapPulse;					//֡?䏶ֵ,??λ: ʤ??λ
}SYSTEM;	//系统参数结构组

typedef struct
{
	int			EncoderTotalPulseSingleRotation;
	int 			Pn20E;//Electronic Gear Ratio(Numerator)
	int 			Pn210;//Electronic Gear Ratio(Denominator)
	int 			Pn212;//Number of Encoder Output Pulses (P/rev)
	
	float64	PWM_PeriodRegister;				// PWM 脉冲发生器周期  CH0906
   	volatile Uint16 *PWM_Port;				//PWM 32BITS address
	
	volatile Uint16	*LPM_CounterPort;	//LPM counter 16bits address 
	Uint16 LPM_CounterRegister;			// LPM 输出计数值
	
	float64 AxisOutUnitEQU;				//UNIT: pulse/最小输出单位 	
}SERVO;

typedef struct
{
UINT16AXIS	EncoderError;		// 轴光栅尺报警   1--报警

	Uint16 NCCodeBreak;					//ARM发送G代码不连续	
	Uint16 InterpolationOver;			//插补量超5mm报警
	Uint16 TrackRunOutError;			//轨迹偏离报警
	Uint16 HaveGetErrorDataSign;		//已截取错误数据标志, when ARM ->RET
 	Uint16 RealRunCount;				//已截取错误数据运行条数
	Uint16 		InsideRAMCheckError;		// 内部RAM 校验错误
	Uint16 		OutsideRAMCheckError;		// 外部RAM 校验错误
	Uint16 		MainErrorSign;				//总错误标志
	Uint16		AlmHLimitPos;
	Uint16		AlmHLimitNeg;
	Uint16		AlmSLimitPos;
	Uint16		AlmSLimitNeg;
	UINT16AXIS ServoAlarm;							//伺服报警		1:报警有效
	Uint16 		CompDataCheckError;				// ?ǏߐԲ???ʽ?ݐ?ѩ?펳
	Uint16		OpticalFlashTriggerBreakError; //OpticalFlashTriggerBreak
	Uint16		CameraShutterTriggerBreakError; //CameraShutterTriggerBreak
	Uint16		LensPositionBreakError;
}ERROR;		//报警结构组


typedef struct
{
	Uint16	CodeRun_Over;               	//  1--完成当前G代码XYZ轴运行
	UINT16MODULE CodeRun_Over_Module;
	Uint16	PrecalCode;					// 1--已预先计算G代码 
	Uint16 	GetPositionSign;			//1--需获得稳定反馈坐标值标志
	Uint16	NewAreaSign;				//新区域标志
	float64    	F;			//G代码设定进给速度  (mm/s)
	float64   	FG0;		//G0代码设定进给速度 (mm/s)
	Uint32 	RealSendCount;		//ARM实际发送分解代码位置指针
	Uint32 	RealRunCount;		//DSP实际运行分解代码位置指针
	Uint32	LastRealSendCount;	//打入公共区域时及单段时使用
	Uint32	Information32[3];	// SPC代码32位信息
	Uint32	EndCodeCount;		//速度为0的代码条
	Uint32	LastEndCodeCount;	//
	Uint32	VeVmCalCount;		//上次计算过Ve,Vm的代码条		
	Uint16	ResendGcodeSign;		//Arm重发代码标志.1:重发.
	UINT16AXIS	NeedBack;					//0:  1:positive compensation 2:negtive compensation 
}NC_RUN_SIGN;

typedef struct
{
	Uint16		SearchRefSign;			// 要求搜索参考点标志
	Uint16		RefStep;				//参考点搜索步骤
	UINT16AXIS	NeedFindAxisSign;		// 需要搜索轴的标志
	UINT16AXIS	FindRefSign;			// 已找到某轴参考点标志
	Uint16		PreSearchRefSign;		// 搜索参考点前初始化标志
}REFERENCE;

//²å²¹Êä³ö»º³åÇø  CH0902
typedef struct
{
	INT40AXIS	DeltaPulse;			//»º³åÇø:  (µ¥Î»: ²å²¹¶ÎÊä³öÖ¸Áîµ¥Î»)
	INT40AXIS	MACHPosition;		//»º³åÇø:  (µ¥Î»: ²å²¹Î»ÖÃÊä³öÖ¸Áîµ¥Î»)
	UINT16AXIS 	PWM_PeriodRegister;			                                     
	UINT16AXIS 	SIGN;                                      
}INTERPOLATION_BUFFER;

typedef struct 			//±àÂëÆ÷Êý¾Ý½á¹¹Ìå
{
	INT16U	OldMACHValue_Low;
	INT16S	OldMACHValue_High;
	INT16U	NowMACHValue_Low;
	INT16S	NowMACHValue_High;

	Int32 	OldEncoderValue;			// ���������������ֵ
	Int32 	NowEncoderValue;			// ���������������ֵ
	Int32   	MACH_PositionValue;			// �����������ʵ��λ��ֵ   UNIT:��С���뵥λ(A4/A5/Spindle)
    	Int32   	MACH_PositionValueout;		// �����������ʵ����� λ��ֵ
	Uint16	EncoderDIR;							// ��������������  0:����
	float64	Resolution;							// �ֱ���UNIT: ��С���뵥λ/pulse
	volatile Int32 *LPM_CounterPort_Low;	// LPM ����������ڵ�ַ
	volatile Int32 *LPM_CounterPort_High;	// LPM ����������ڵ�ַ
}ENCODER;

typedef struct 
{
	Int32 	OldValue;			
	Int32 	NowValue;	

	Int32 	OldValue_G114;			
	Int32 	NowValue_G114;	
	float64 	AverageValue_G114;
	float64	Resolution;					
	volatile Uint16 *PMT_CounterPort_Low;
	volatile Uint16 *PMT_CounterPort_High;
}PMT;


typedef struct
{
	float64AXIS Value_Linear;		//?╤?ߗ??ꖵ   		??λ: ׮Сʤȫ??λ
	float64AXIS Value_Laser ;		//???⊵?◸?ꖵ  	??λ: ׮Сʤȫ??λ
}AxisCompensation;   


typedef struct
{ 
	Uint16 DA_CS; 	
	Uint16 DA_ADDR;
	Uint16 DA_Data;
	
	Uint16 AutoSign; // 1
	Uint16 Voltage_Max;  //uint:0.01V
	Uint16 Voltage_Start; //uint:0.01V
	Uint16 CycleNum; //num
	
	Uint16 RiseCount; //uint:0.1ms
	Uint16 FallCount;  //uint:0.1ms
	Uint16 HKeepCount;  //uint:0.1ms
	Uint16 LKeepCount; //uint:0.1ms
	Uint16 HIncrument; 
	Uint16 LIncrument; 
	Uint16 HDividedNum; 
	Uint16 LDividedNum; 
	
	Uint16 PWMHighCount; //uint:0.1us
	Uint16 PWMLowCount;  //uint:0.1us
	Uint16 PWMPhase;  //uint:0.1us
}DACONVERT; 


#endif  // end of DSP6713_COMMON_H definition


