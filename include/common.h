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
#define		ADDGCODEINCUBATOR_MOD	21
#define 		COMPBUFFERLENGTH	1000

#define 	VNowMIN		0.0001	// µ±Ç°Êµ¼ÊÔËÐÐËÙ¶È×îÐ¡Öµ. (mm/s)
#define	ShortLineLengh	0.0055	// Òì³£Ð¡Ïß¶ÎÖµ¶¨Òåunit:mm
#define	MovePositive	1		// ÕýÏòÒÆ¶¯
#define	MoveNegtive	0		// ¸ºÏòÒÆ¶¯

//CH0603
#define 	StaticTrackLimit		0.5		//¾²Ì¬¹ì¼£ÅÐ¶¨½çÏÞ0.5mm  CH0603
#define 	Max_StaticErrorTimes 1000	//¾²Ì¬¹ì¼£ÅÐ¶¨ÖÜÆÚÔ¼1s

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
	Uint32	Pointer_StartCode;								//·Ö½â´úÂë£¬ÇøÓòÆðÊ¼´úÂë¶ÎÖ¸Õë
	Uint32	Pointer_EndCode;								//·Ö½â´úÂë£¬ÇøÓòÖÕÖ¹´úÂë¶ÎÖ¸Õë
	Uint32	Pointer_FirstInflexionCode;				//·Ö½â´úÂë£¬ÇøÓòÒ»´Î¹Õµã´úÂë¶ÎÖ¸Õë
	Uint32	Pointer_SecondInflexionCode;		//·Ö½â´úÂë£¬ÇøÓò¶þ´Î¹Õµã´úÂë¶ÎÖ¸Õë
	float64MODULE	Ss_Module;		
	float64	Stotal;						//ÇøÓòÄÚËùÓÐ´úÂë×ÜÔËÐÐ³¤¶È mm
	float64MODULE 	Stotal_Module;
	float64MODULE 	CurrentStotal_Module;
	float64MODULE 	CurrentStotalLast_Module;
	float64MODULE 	Snow_Module;
	float64MODULE 	DeltaSnow_Module;
	Uint32	I;		//²å²¹ÐòºÅ [1,+ÎÞÇî)
	UINT40MODULE I_Module;

	UINT40MODULE TotalN_Module;	//×Ü²å²¹Êý	TotalN=T5/Tsample
	float64MODULE	Vs_Module;		//µ±Ç°ÔËÐÐG´úÂëÆðµãÔËÐÐËÙ¶È. (mm/s)
	float64			Vm;
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
	INT40AXIS	OldPointMACHPulseOut;	
	INT40AXIS	EndPointMachPulseOut;

	UINT16MODULE SpeedState;		//ÇøÓòËÙ¶È×´Ì¬
	Uint16		ri;						//200 »º³åÇøÔËÐÐÈ¡Ä£Ö¸Õë
	Uint16		ZeroPulseSign;				//ÁãÂö³å±êÖ¾,1ÎªÁãÂö³å
}AREA_CONTROL;

//¼ÇÒä´úÂëµÄÒÆ¶¯·½Ïò£¬ÓÃÓÚ·´·½Ïò¼õËÙ´¦Àí
typedef struct
{
UINT16AXIS	MoveSign;				// 1-positive 0-negtive 
INT40AXIS	Delta;					// ´úÂë±ä»¯Á¿um
float64		TotalLength;			// ´úÂë×Ü³¤mm
}PRECALMOVESIGN;

//CH0512
typedef struct
{
	Uint32 				SendCount;		
	Uint32 				CMDMain,SubCMD1,SubCMD2;		
	INT40AXIS			StartPoint,EndPoint; 
	float64AXIS			DeltaPoint;		
	float64 				Stotal;			
	float64MODULE 		Stotal_Module;			
	float64 				Feedrate;			
	float64MODULE 		Feedrate_Module;				
	float64 				Vm;				
	float64MODULE 		Vm_Module;			
	float64 				Ve;						
	float64MODULE 		Ve_Module;						
	float64MODULE 		Vveer_Module;	

	VECTOR  				StPointVector;		
	VECTOR  				EndPointVector;	
	INT16U				MoveCodeSign;
	Uint16				VeCalSign;		
	Uint16				VmCalSign;		
	Uint8				PositionStep;
}GCODE;

//×ÜÌå±êÖ¾ºÍ±äÁ¿ÓÃ½á¹¹Ìå
typedef struct
{
	Uint16 	NCSign;  
	// 0:Í¨³£×´Ì¬  1:±ê×¼´úÂë£»2:ÌØÊâ´úÂë-JOG HANDLE REF£»INI=0
	
	//ËÙ¶È¿ØÖÆÏà¹Ø²ÎÊý
	Uint16	LastFeedrateOverride;		//ÉÏ´Î½ø¸øËÙ¶ÈµµÎ»
	Uint16	LastRapidFeedrateOverride;	//ÉÏ´Î¿ì½øËÙ¶ÈµµÎ»
	Uint16 	FeedrateOverride;			//½ø¸øµµÎ»	[0-20]
	Uint16  RapidFeedrateOverride;		//G0¿ìËÙ½ø¸øµµÎ»	[0-20]
	//CH0506 GCODE	LastMoveGCode;			//ÔÝ´æÉÏÌõ·¢ËÍÔË¶¯G´úÂë½á¹¹ÄÚÈÝ
	GCODE	STDLastMoveGCode;			//ÔÝ´æÉÏÌõ·¢ËÍÔË¶¯G´úÂë½á¹¹ÄÚÈÝ
	GCODE	SPCLastMoveGCode;			//ÔÝ´æÉÏÌõ·¢ËÍÔË¶¯G´úÂë½á¹¹ÄÚÈÝ

	VECTOR	LastMoveCodeEndVector;	//ÉÏÒ»ÌõÔË¶¯G´úÂëÖÕµã·½ÏòÊ¸Á¿(´úÂëÔ¤´¦ÀíÓÃ)
	Uint16	EveryAxisCoinValid;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid1;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid2;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid3;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisCoinValid4;		// ¸÷ÖáCOIN ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisOverValid;		// ¸÷ÖáOVER ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisOverValid1;		// ¸÷ÖáOVER ¶¼ÓÐÐ§±êÖ¾
	Uint16	EveryAxisOverValid2;		// ¸÷ÖáOVER ¶¼ÓÐÐ§±êÖ¾
	
	Uint16 	GetMachCoordinateRight;	//  1:»ñµÃ»ú´²×ø±êÕýÈ·

INT40AXIS	AbsoluteEncoderValue_PSO;	
INT40AXIS	ABS_Coordinate;			// µ±Ç°¾ø¶Ô×ø±êÖµunit:×îÐ¡ÊäÈëµ¥Î»
INT40AXIS	ABSORG_M_Coordinate;	// µ±Ç°¾ø¶Ô×ø±êÔ­µãµÄ»ú´²×ø±êÖµunit:×îÐ¡ÊäÈëµ¥Î»
INT40AXIS	ABSORG_Coordinate;	// µ±Ç°¾ø¶Ô×ø±êÔ­µãµÄ»ú´²×ø±êÖµunit:×îÐ¡ÊäÈëµ¥Î»
UINT16AXIS	MoveSign;				// µ±Ç°ÔË¶¯·½Ïò1-positive 0-negtive
UINT16AXIS	TheoryMoveSign;			// µ±Ç°´úÂëÀíÂÛ·½Ïò1-positive 0-negtive

Uint16	LastCodeOverSign;			//×îºóÒ»Ìõ´úÂë×ßÍê±êÖ¾  1£­×ßÍê
	GCODE		*LastGCodePointer;

	Uint16		Int4IdleSign_1;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾
	Uint16		Int4IdleSign_2;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾
	Uint16		Int4IdleSign_3;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾
	Uint16		Int4IdleSign_4;						//ÖÐ¶Ï4 ¿ÕÏÐ±êÖ¾

	//Uint32		OverideCnt;
	Uint32		InterpolationSendCnt_1;		//»º³åÇø·¢ËÍÌõÊý
	Uint32		InterpolationRunCnt_1;		//»º³åÇøÔËÐÐÌõÊý
	Uint32		InterpolationSendCnt_2;		//»º³åÇø·¢ËÍÌõÊý
	Uint32		InterpolationRunCnt_2;		//»º³åÇøÔËÐÐÌõÊý

	Uint8		ParameterReadCompleteSign;
	Uint8		CameraNeedCaptureSign;			//�����Ҫ���ձ�־

	Uint8		CameraScanSign;			//���ɨ���־
	Uint8		CameraStaticCaptureSign;			//���ֹ���ձ�־
	Uint8		MicroEScanSign;			//MicroEɨ���־
	Uint8		CameraSoftTriggerLedFlash;			
	Uint8		OpticalFlashSign;			//����������־
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
	
	UINT16AXIS	EncoderCompPt;				//�����������Բ�����������λ��ָ��
	UINT16AXIS	PulseOutCompPt;				//������������Բ�����������λ��ָ��
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
	Uint16	PULSE_PER_UM;	//[1-100]  Ã¿umÏàµ±Âö³åÊý   pulse/um
	Uint16	PWM_PeriodRegister_MIN;			// PWM  ×îÐ¡Ð´ÈëÖµ
	Uint16	PWM_PeriodRegister_MAX;			// PWM  ×î´óÐ´ÈëÖµ
	Uint16	PWM_PeriodRegister_ZeroPeriod;	// LPMÎª0Ê±PWM  Ð´ÈëÖµ
	
	Uint16	ReferenceFrameMode;		//×ø±êÏµÄ£ÐÍ

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
	
	UINT40AXIS	Gap;				 			//���϶ֵ,��λ: ��С���뵥λ
	UINT40AXIS	GapPulse;					//���϶ֵ,��λ: ���ָ�λ
}SYSTEM;	

typedef struct
{
	int			EncoderTotalPulseSingleRotation;
	int 			Pn20E;//Electronic Gear Ratio(Numerator)
	int 			Pn210;//Electronic Gear Ratio(Denominator)
	int 			Pn212;//Number of Encoder Output Pulses (P/rev)
	
	float64	PWM_PeriodRegister;				// PWM Âö³å·¢ÉúÆ÷ÖÜÆÚ  CH0906
   	volatile Uint16 *PWM_Port;				//PWM 32BITS address
	
	volatile Uint16	*LPM_CounterPort;	//LPM counter 16bits address 
	Uint16 LPM_CounterRegister;			// LPM Êä³ö¼ÆÊýÖµ
	
	float64 AxisOutUnitEQU;				//UNIT: pulse/×îÐ¡Êä³öµ¥Î»
}SERVO;

typedef struct
{
UINT16AXIS	EncoderError;		// Öá¹âÕ¤³ß±¨¾¯   1--±¨¾¯

	Uint16 NCCodeBreak;					//ARM·¢ËÍG´úÂë²»Á¬Ðø
	Uint16 InterpolationOver;			//²å²¹Á¿³¬5mm±¨¾¯
	Uint16 TrackRunOutError;			//¹ì¼£Æ«Àë±¨¾¯
	Uint16 HaveGetErrorDataSign;		//ÒÑ½ØÈ¡´íÎóÊý¾Ý±êÖ¾, when ARM ->RET
 	Uint16 RealRunCount;				//ÒÑ½ØÈ¡´íÎóÊý¾ÝÔËÐÐÌõÊý
	Uint16 		InsideRAMCheckError;		// ÄÚ²¿RAM Ð£Ñé´íÎó
	Uint16 		OutsideRAMCheckError;		// Íâ²¿RAM Ð£Ñé´íÎó
	Uint16 		MainErrorSign;				//×Ü´íÎó±êÖ¾
	Uint16		AlmHLimitPos;
	Uint16		AlmHLimitNeg;
	Uint16		AlmSLimitPos;
	Uint16		AlmSLimitNeg;
	UINT16AXIS ServoAlarm;							//ËÅ·þ±¨¾¯		1:±¨¾¯ÓÐÐ§
	Uint16 		CompDataCheckError;				// �����Բ������У�����
	Uint16		OpticalFlashTriggerBreakError; //OpticalFlashTriggerBreak
	Uint16		CameraShutterTriggerBreakError; //CameraShutterTriggerBreak
	Uint16		LensPositionBreakError;
}ERROR;		//±¨¾¯½á¹¹×é


typedef struct
{
	Uint16	CodeRun_Over;               	// 1--Íê³Éµ±Ç°G´úÂëXYZÖáÔËÐÐ
	UINT16MODULE CodeRun_Over_Module;
	Uint16	PrecalCode;					// 1--ÒÑÔ¤ÏÈ¼ÆËãG´úÂë
	Uint16 	GetPositionSign;			//1--Ðè»ñµÃÎÈ¶¨·´À¡×ø±êÖµ±êÖ¾
	Uint16	NewAreaSign;				//ÐÂÇøÓò±êÖ¾
	float64    	F;			//G´úÂëÉè¶¨½ø¸øËÙ¶È  (mm/s)
	float64   	FG0;		//G0´úÂëÉè¶¨½ø¸øËÙ¶È (mm/s)
	Uint32 	RealSendCount;		//ARMÊµ¼Ê·¢ËÍ·Ö½â´úÂëÎ»ÖÃÖ¸Õë
	Uint32 	RealRunCount;		//DSPÊµ¼ÊÔËÐÐ·Ö½â´úÂëÎ»ÖÃÖ¸Õë
	Uint32	LastRealSendCount;	//´òÈë¹«¹²ÇøÓòÊ±¼°µ¥¶ÎÊ±Ê¹ÓÃ
	Uint32	Information32[3];	// SPC´úÂë32Î»ÐÅÏ¢
	Uint32	EndCodeCount;		//ËÙ¶ÈÎª0µÄ´úÂëÌõ
	Uint32	LastEndCodeCount;	//
	Uint32	VeVmCalCount;		//ÉÏ´Î¼ÆËã¹ýVe,VmµÄ´úÂëÌõ
	Uint16	ResendGcodeSign;		//ArmÖØ·¢´úÂë±êÖ¾.1:ÖØ·¢.
	UINT16AXIS	NeedBack;					//0:  1:positive compensation 2:negtive compensation 
}NC_RUN_SIGN;

typedef struct
{
	Uint16		SearchRefSign;			// ÒªÇóËÑË÷²Î¿¼µã±êÖ¾
	Uint16		RefStep;				//²Î¿¼µãËÑË÷²½Öè
	UINT16AXIS	NeedFindAxisSign;		// ÐèÒªËÑË÷ÖáµÄ±êÖ¾
	UINT16AXIS	FindRefSign;			// ÒÑÕÒµ½Ä³Öá²Î¿¼µã±êÖ¾
	Uint16		PreSearchRefSign;		// ËÑË÷²Î¿¼µãÇ°³õÊ¼»¯±êÖ¾
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
	float64AXIS Value_Linear;		//��դ�����ֵ   		��λ: ��С���뵥λ
	float64AXIS Value_Laser ;		//����ʵ�����ֵ  	��λ: ��С���뵥λ
}AxisCompensation;   

/*
typedef struct
{ 
	Uint16 DA_CS; 	
	Uint16 DA_ADDR;
	Uint32 DA_ADDR_Sync;
	Uint16 DA_Data;	
	Uint16 DA_Data_Sync;	
	Uint16 AutoSign; // 1	single channel // 2~5 sync modle...2:aotf normal open;3:aotf pwm ctr;4:pockel normal open ; 5:pockel pwm ctr.

	Uint32 StartOffsetCnt;//uint:received data unit is 100us.  so multiple*2 change to cnt basic 50us interrupt time
	Uint32 EndOffsetCnt;//uint:received data unit is 100us.  so multiple*2 change to cnt basic 50us interrupt time

	Uint16 PWMHighCount; //uint:0.1us
	Uint16 PWMLowCount;  //uint:0.1us
	Uint16 PWMPhase;  //uint:0.1us
	
}DACONVERT; 
PC 端结构体
 typedef struct
{
	Uint32 VotageChannel;
	Uint32 VotageChannel_Sync;
	Uint32 VotageValue;

	Uint32 AutoSign; 	// 1
	Uint32 Voltage_Max; //uint:0.01V
	Uint32 Voltage_Start; //uint:0.01V
	Uint32 Voltage_Max2; //uint:0.01V
	Uint32 Voltage_Start2; //uint:0.01V
	Uint32 CycleNum; //num

	Uint32 RiseTime; //uint:0.1ms
	Uint32 FallTime;  //uint:0.1ms
	Uint32 HighKeepTime; //uint:0.1ms
	Uint32 LowKeepTime; //uint:0.1ms

	Uint32 StartOffsetTime;//uint:0.1ms
	Uint32 EndOffsetTime;//uint:0.1ms

	Uint32 PWMHighCount; //uint:0.1us
	Uint32 PWMLowCount; //uint:0.1us
	Uint32 PWMPhase; //uint:0.1us
}DACONVERT;
*/
typedef struct
{
	Uint16 DA_CS;
	Uint16 DA_ADDR;
	Uint32 DA_ADDR_Sync;
	Uint16 DA_Data;
	Uint16 DA_Data_Sync;
	Uint16 AutoSign; // 1	single channel // 2~5 sync modle...2:aotf normal open;3:aotf pwm ctr;4:pockel normal open ; 5:pockel pwm ctr.
	Uint32 WaveSelect; //0:triangle;1:sin;2:cos;3:atan
	Uint32 Voltage_Max; //uint:0.01V
	Uint32 Voltage_Start; //uint:0.01V
	Uint32 Voltage_Max2; //uint:0.01V
	Uint32 Voltage_Start2; //uint:0.01V
	Uint32 CycleNum; //num

	Uint32 RiseTime; //uint:0.1ms
	Uint32 FallTime;  //uint:0.1ms
	Uint32 HighKeepTime; //uint:0.1ms
	Uint32 LowKeepTime; //uint:0.1ms

	Uint32 StartOffsetCnt;//uint:received data unit is 100us.  so multiple*2 change to cnt basic 50us interrupt time
	Uint32 EndOffsetCnt;//uint:received data unit is 100us.  so multiple*2 change to cnt basic 50us interrupt time

	Uint16 PWMHighCount; //uint:0.1us
	Uint16 PWMLowCount;  //uint:0.1us
	Uint16 PWMPhase;  //uint:0.1us

	Uint16 GetDataCount; //0-2000;
}DACONVERT;

#endif  // end of DSP6713_COMMON_H definition


