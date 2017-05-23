#include "..\\include\\main_dsp.h"

#define RESOLUTION_LINEARMOTOR 0.0625/0.05
INT16AXIS CheckSoftLimit_cycle = {0};

extern Uint16 MonitorBuffer1Cnt, MonitorBuffer2Cnt, MonitorBuffer3Cnt,MonitorBuffer4Cnt;
Uint32 AverageCounter;

INT40AXIS PositionValue_CameraCapture;
float64	PositionValueDivideOffsetRef=100.0;

extern Uint16 LensCompensationBufferCnt;

AD5315DA  Ad_Convert;
AD5315DA  Ad_Convert_2;
AD5315DA  Ad_Convert_3;

extern float64 		CameraScanStepperTotalNumber_X;
extern Int32		PositionPointerCnt,PositionPointerCnt_Bak;
extern Int32		OpticalFlashPointerCnt,OpticalFlashPointerCnt_Bak;
extern Int32		DeltaStepAxis19_Bak;
extern Int32		DeltaStepAxis19_CPS;

Int32 LensPositionOverCounter = 0;
Int32 LensPositionOverCounter_2 = 0;
Int32 LensMotionOverCounter = 0;
Int32 LensMotionOverCounter_2 = 0;


volatile INT16U * ENCODER_Axis1_Low = (INT16U *) mCPLDADDR_ENCODER_Axis1_Low; //AD×ª»»0
volatile INT16U * ENCODER_Axis1_High = (INT16U *) mCPLDADDR_ENCODER_Axis1_High; //AD×ª»»0
volatile INT16U * ENCODER_Axis2_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis2_Low;
volatile INT16U * ENCODER_Axis2_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis2_High;
volatile INT16U * ENCODER_Axis3_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis3_Low; 
volatile INT16U * ENCODER_Axis3_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis3_High; 
volatile INT16U * ENCODER_Axis4_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis4_Low; 
volatile INT16U * ENCODER_Axis4_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis4_High; 
volatile INT16U * ENCODER_Axis5_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis5_Low; 
volatile INT16U * ENCODER_Axis5_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis5_High; 
volatile INT16U * ENCODER_Axis6_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis6_Low; 
volatile INT16U * ENCODER_Axis6_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis6_High; 
volatile INT16U * ENCODER_Axis7_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis7_Low; 
volatile INT16U * ENCODER_Axis7_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis7_High; 
volatile INT16U * ENCODER_Axis8_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis8_Low; 
volatile INT16U * ENCODER_Axis8_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis8_High; 
volatile INT16U * ENCODER_Axis9_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis9_Low; 
volatile INT16U * ENCODER_Axis9_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis9_High; 
volatile INT16U * ENCODER_Axis10_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis10_Low; 
volatile INT16U * ENCODER_Axis10_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis10_High; 
volatile INT16U * ENCODER_Axis11_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis11_Low; 
volatile INT16U * ENCODER_Axis11_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis11_High; 
volatile INT16U * ENCODER_Axis12_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis12_Low; 
volatile INT16U * ENCODER_Axis12_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis12_High; 
volatile INT16U * ENCODER_Axis13_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis13_Low; 
volatile INT16U * ENCODER_Axis13_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis13_High; 
volatile INT16U * ENCODER_Axis14_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis14_Low; 
volatile INT16U * ENCODER_Axis14_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis14_High; 
volatile INT16U * ENCODER_Axis15_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis15_Low; 
volatile INT16U * ENCODER_Axis15_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis15_High;  
volatile INT16U * ENCODER_Axis16_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis16_Low;  
volatile INT16U * ENCODER_Axis16_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis16_High;  
volatile INT16U * ENCODER_Axis17_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis17_Low;  
volatile INT16U * ENCODER_Axis17_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis17_High;  
volatile INT16U * ENCODER_Axis18_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis18_Low;  
volatile INT16U * ENCODER_Axis18_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis18_High;  
volatile INT16U * ENCODER_Axis19_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis19_Low;  
volatile INT16U * ENCODER_Axis19_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis19_High;  
volatile INT16U * ENCODER_Axis20_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis20_Low;  
volatile INT16U * ENCODER_Axis20_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis20_High;  
volatile INT16U * ENCODER_Axis21_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis21_Low;  
volatile INT16U * ENCODER_Axis21_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis21_High;  
volatile INT16U * ENCODER_Axis22_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis22_Low;  
volatile INT16U * ENCODER_Axis22_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis22_High;  
volatile INT16U * ENCODER_Axis23_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis23_Low;  
volatile INT16U * ENCODER_Axis23_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis23_High;  
volatile INT16U * ENCODER_Axis24_Low  	= (INT16U *) mCPLDADDR_ENCODER_Axis24_Low;  
volatile INT16U * ENCODER_Axis24_High  	= (INT16U *) mCPLDADDR_ENCODER_Axis24_High ; 

volatile INT16U * SERVO_OVER1  	= (INT16U *) mCPLDADDR_SERVO_OVER1;  
volatile INT16U * SERVO_OVER2  	= (INT16U *) mCPLDADDR_SERVO_OVER2 ; 
volatile INT16U * SERVO_ALARM1  	= (INT16U *) mCPLDADDR_SERVO_ALARM1;  
volatile INT16U * SERVO_ALARM2  	= (INT16U *) mCPLDADDR_SERVO_ALARM2 ; 
volatile INT16U * SERVO_COIN1  	= (INT16U *) mCPLDADDR_SERVO_COIN1;  
volatile INT16U * SERVO_COIN2  	= (INT16U *) mCPLDADDR_SERVO_COIN2;  
volatile INT16U * HARDLIMIT_POSITIVE1  	= (INT16U *) mCPLDADDR_HARDLIMIT_POSITIVE1;  
volatile INT16U * HARDLIMIT_POSITIVE2  	= (INT16U *) mCPLDADDR_HARDLIMIT_POSITIVE2 ; 
volatile INT16U * HARDLIMIT_NEGATIVE1  	= (INT16U *) mCPLDADDR_HARDLIMIT_NEGATIVE1;  
volatile INT16U * HARDLIMIT_NEGATIVE2  	= (INT16U *) mCPLDADDR_HARDLIMIT_NEGATIVE2 ; 
volatile INT16U * ENCODER1_REF  	= (INT16U *) mCPLDADDR_ENCODER1_REF ;
volatile INT16U * ENCODER2_REF  	= (INT16U *) mCPLDADDR_ENCODER2_REF ;

volatile INT16U *  AD0  = (INT16U *) mCPLDADDR_AD0 ; 
volatile INT16U *  AD1  = (INT16U *) mCPLDADDR_AD1 ; 
volatile INT16U *  AD2  = (INT16U *) mCPLDADDR_AD2 ; 
volatile INT16U *  AD3  = (INT16U *) mCPLDADDR_AD3 ; 
volatile INT16U *  AD4  = (INT16U *) mCPLDADDR_AD4 ; 
volatile INT16U *  AD5  = (INT16U *) mCPLDADDR_AD5 ; 
volatile INT16U *  AD6  = (INT16U *) mCPLDADDR_AD6 ; 
volatile INT16U *  AD7  = (INT16U *) mCPLDADDR_AD7 ; 

volatile INT16U * IO_IN1  	= (INT16U *) mCPLDADDR_IO_IN1 ; 
volatile INT16U * IO_IN2  	= (INT16U *) mCPLDADDR_IO_IN2 ; 
volatile INT16U * IO_IN3  	= (INT16U *) mCPLDADDR_IO_IN3 ; 
volatile INT16U * IO_IN4  	= (INT16U *) mCPLDADDR_IO_IN4 ; 


volatile INT16U * READ_GPIO_UART0  	= (INT16U *) mCPLDADDR_READ_GPIO_UART0 ; 
volatile INT16U * READ_GPIO_UART1  	= (INT16U *) mCPLDADDR_READ_GPIO_UART1 ; 
volatile INT16U * READ_GPIO_UART2  	= (INT16U *) mCPLDADDR_READ_GPIO_UART2 ; 
volatile INT16U * READ_GPIO_UART3  	= (INT16U *) mCPLDADDR_READ_GPIO_UART3 ; 
volatile INT16U * READ_GPIO_UART4  	= (INT16U *) mCPLDADDR_READ_GPIO_UART4 ; 
volatile INT16U * READ_GPIO_UART5  	= (INT16U *) mCPLDADDR_READ_GPIO_UART5 ; 
volatile INT16U * READ_GPIO_UART6  	= (INT16U *) mCPLDADDR_READ_GPIO_UART6 ; 
volatile INT16U * READ_GPIO_UART7  	= (INT16U *) mCPLDADDR_READ_GPIO_UART7 ; 

volatile INT16U * READ_RS232_1  	= (INT16U *) mCPLDADDR_READ_RS232_1 ;
volatile INT16U * READ_RS232_2  	= (INT16U *) mCPLDADDR_READ_RS232_2 ;
volatile INT16U * READ_RS232_3  	= (INT16U *) mCPLDADDR_READ_RS232_3 ;
volatile INT16U * READ_RS232_4  	= (INT16U *) mCPLDADDR_READ_RS232_4 ;
volatile INT16U * READ_RS485_1  	= (INT16U *) mCPLDADDR_READ_RS485_1 ;
volatile INT16U * READ_RS485_2  	= (INT16U *) mCPLDADDR_READ_RS485_2 ;
volatile INT16U * READ_RS490_1  	= (INT16U *) mCPLDADDR_READ_RS490_1 ;
volatile INT16U * READ_RS490_2  	= (INT16U *) mCPLDADDR_READ_RS490_2 ;


volatile INT16U * READ_COM_ReadSign  	= (INT16U *) mCPLDADDR_READ_COM_ReadSign ;

volatile INT16U * READ_STM32_Data  	= (INT16U *) mCPLDADDR_READ_STM32_Data ;
volatile INT16U * READ_STM32_ReadSign  	= (INT16U *) mCPLDADDR_READ_STM32_ReadSign ;

volatile INT16U * READ_Baudrate_Uart_1_2  	= (INT16U *) mCPLDADDR_READ_Baudrate_Uart_1_2 ;
volatile INT16U * READ_Baudrate_Uart_3_4 	= (INT16U *) mCPLDADDR_READ_Baudrate_Uart_3_4 ;
volatile INT16U * READ_Baudrate_Uart_5_6  	= (INT16U *) mCPLDADDR_READ_Baudrate_Uart_5_6 ;
volatile INT16U * READ_Baudrate_Uart_7_8  	= (INT16U *) mCPLDADDR_READ_Baudrate_Uart_7_8 ;
volatile INT16U * READ_Baudrate_RS232_1_2  	= (INT16U *) mCPLDADDR_READ_Baudrate_RS232_1_2 ;
volatile INT16U * READ_Baudrate_RS232_3_4  	= (INT16U *) mCPLDADDR_READ_Baudrate_RS232_3_4 ;
volatile INT16U * READ_Baudrate_RS485_1_2  	= (INT16U *) mCPLDADDR_READ_Baudrate_RS485_1_2 ;
volatile INT16U * READ_Baudrate_RS490_1_2  	= (INT16U *) mCPLDADDR_READ_Baudrate_RS490_1_2 ;

//= (INT16U *) mCPLDADDR__184) reserved

volatile INT16U * ABSENCODER_Dir_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis1 ;   //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis1;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis1;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis1;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis1;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar0_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar1_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis1; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar6_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis1;	
volatile INT16U * ABSENCODER_RingValueChar7_Axis1  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis1;	

volatile INT16U * ABSENCODER_Dir_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis2 ;   //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis2;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis2;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis2; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis2;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis2; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis2; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis2;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis2;
volatile INT16U * ABSENCODER_RingValueChar3_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis2;
volatile INT16U * ABSENCODER_RingValueChar4_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis2;
volatile INT16U * ABSENCODER_RingValueChar5_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis2;
volatile INT16U * ABSENCODER_RingValueChar6_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis2;
volatile INT16U * ABSENCODER_RingValueChar7_Axis2  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis2;


volatile INT16U * ABSENCODER_Dir_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis3;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis3;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis3; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis3; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis3;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar0_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar1_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar4_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis3; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis3;	
volatile INT16U * ABSENCODER_RingValueChar7_Axis3  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis3;	


volatile INT16U * ABSENCODER_Dir_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis4 ;   //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis4;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis4;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis4; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis4;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis4;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis4;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis4; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis4  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis4; 	

volatile INT16U * ABSENCODER_Dir_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis5;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis5; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis5; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis5;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis5;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis5; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis5; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis5; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis5;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis5;	
volatile INT16U * ABSENCODER_RingValueChar4_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis5; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis5;	
volatile INT16U * ABSENCODER_RingValueChar6_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis5; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis5  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis5;	

volatile INT16U * ABSENCODER_Dir_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis6;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis6; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis6; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis6;
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis6; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis6; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar1_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis6;
volatile INT16U * ABSENCODER_RingValueChar3_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar4_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar6_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis6;	
volatile INT16U * ABSENCODER_RingValueChar7_Axis6  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis6;	

volatile INT16U * ABSENCODER_Dir_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis7;     //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis7; 
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis7;  	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis7;  	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis7; 
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis7; 
volatile INT16U * ABSENCODER_RingValueChar0_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis7;  	
volatile INT16U * ABSENCODER_RingValueChar1_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis7; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis7; 	
volatile INT16U * ABSENCODER_RingValueChar3_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis7;  	
volatile INT16U * ABSENCODER_RingValueChar4_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis7; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis7;  	
volatile INT16U * ABSENCODER_RingValueChar6_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis7;  	
volatile INT16U * ABSENCODER_RingValueChar7_Axis7  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis7; 	

volatile INT16U * ABSENCODER_Dir_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis8;     //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis8;  	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis8; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis8; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis8; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis8; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis8; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis8;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis8;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis8;	
volatile INT16U * ABSENCODER_RingValueChar4_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis8;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis8; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis8; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis8  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis8; 	

volatile INT16U * ABSENCODER_Dir_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis9;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis9;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis9; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis9; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis9; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis9;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis9;	
volatile INT16U * ABSENCODER_RingValueChar6_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis9; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis9  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis9; 	

volatile INT16U * ABSENCODER_Dir_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis10;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis10; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis10; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis10; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis10; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis10; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis10; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis10; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis10;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis10; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis10;	
volatile INT16U * ABSENCODER_RingValueChar5_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis10;	
volatile INT16U * ABSENCODER_RingValueChar6_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis10; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis10  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis10; 	

volatile INT16U * ABSENCODER_Dir_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis11;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis11; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis11; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis11;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis11; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis11;	
volatile INT16U * ABSENCODER_RingValueChar1_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar3_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis11; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis11  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis11; 	

volatile INT16U * ABSENCODER_Dir_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis12;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis12; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis12; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis12; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis12; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis12;	
volatile INT16U * ABSENCODER_RingValueChar0_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis12; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis12;	
volatile INT16U * ABSENCODER_RingValueChar2_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis12;	
volatile INT16U * ABSENCODER_RingValueChar3_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis12; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis12; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis12; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis12; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis12  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis12; 	

volatile INT16U * ABSENCODER_Dir_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis13;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis13; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis13; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis13; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis13; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar3_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar4_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis13; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis13  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis13; 	


volatile INT16U * ABSENCODER_Dir_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_Dir_Axis14;    //low bit 0-7
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar0_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar0_Axis14;	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar1_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar1_Axis14; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar2_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar2_Axis14; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar3_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar3_Axis14; 	
volatile INT16U * ABSENCODER_RotateRingTotalNuberChar4_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RotateRingTotalNuberChar4_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar0_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar0_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar1_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar1_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar2_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar2_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar3_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar3_Axis14;	
volatile INT16U * ABSENCODER_RingValueChar4_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar4_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar5_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar5_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar6_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar6_Axis14; 	
volatile INT16U * ABSENCODER_RingValueChar7_Axis14  	= (INT16U *) mCPLDADDR_ABSENCODER_RingValueChar7_Axis14; 	

volatile INT16U * PMT1_Low = (INT16U *) mCPLDADDR_READ_PMT1_Low; 
volatile INT16U * PMT1_High = (INT16U *) mCPLDADDR_READ_PMT1_High;
volatile INT16U * PMT2_Low  = (INT16U *) mCPLDADDR_READ_PMT2_Low;
volatile INT16U * PMT2_High  = (INT16U *) mCPLDADDR_READ_PMT2_High;

//Write address as fllows
volatile INT16U * PWM_Axis1  	= (INT16U *) mCPLDADDR_PWM_Axis1;	
volatile INT16U * PWM_Axis2  	= (INT16U *) mCPLDADDR_PWM_Axis2;
volatile INT16U * PWM_Axis3  	= (INT16U *) mCPLDADDR_PWM_Axis3;
volatile INT16U * PWM_Axis4  	= (INT16U *) mCPLDADDR_PWM_Axis4;
volatile INT16U * PWM_Axis5  	= (INT16U *) mCPLDADDR_PWM_Axis5;
volatile INT16U * PWM_Axis6  	= (INT16U *) mCPLDADDR_PWM_Axis6;
volatile INT16U * PWM_Axis7  	= (INT16U *) mCPLDADDR_PWM_Axis7;
volatile INT16U * PWM_Axis8  	= (INT16U *) mCPLDADDR_PWM_Axis8;
volatile INT16U * PWM_Axis9  	= (INT16U *) mCPLDADDR_PWM_Axis9;
volatile INT16U * PWM_Axis10  	= (INT16U *) mCPLDADDR_PWM_Axis10;
volatile INT16U * PWM_Axis11  	= (INT16U *) mCPLDADDR_PWM_Axis11;
volatile INT16U * PWM_Axis12  	= (INT16U *) mCPLDADDR_PWM_Axis12;
volatile INT16U * PWM_Axis13  	= (INT16U *) mCPLDADDR_PWM_Axis13;
volatile INT16U * PWM_Axis14  	= (INT16U *) mCPLDADDR_PWM_Axis14;
volatile INT16U * PWM_Axis15	= (INT16U *) mCPLDADDR_PWM_Axis15;
volatile INT16U * PWM_Axis16  	= (INT16U *) mCPLDADDR_PWM_Axis16;
volatile INT16U * PWM_Axis17  	= (INT16U *) mCPLDADDR_PWM_Axis17;
volatile INT16U * PWM_Axis18  	= (INT16U *) mCPLDADDR_PWM_Axis18;
volatile INT16U * PWM_Axis19  	= (INT16U *) mCPLDADDR_PWM_Axis19;
volatile INT16U * PWM_Axis20  	= (INT16U *) mCPLDADDR_PWM_Axis20;
volatile INT16U * PWM_Axis21  	= (INT16U *) mCPLDADDR_PWM_Axis21;
volatile INT16U * PWM_Axis22  	= (INT16U *) mCPLDADDR_PWM_Axis22;
volatile INT16U * PWM_Axis23  	= (INT16U *) mCPLDADDR_PWM_Axis23;

volatile INT16U * LPM_Axis1  	= (INT16U *) mCPLDADDR_LPM_Axis1;
volatile INT16U * LPM_Axis2  	= (INT16U *) mCPLDADDR_LPM_Axis2;
volatile INT16U * LPM_Axis3  	= (INT16U *) mCPLDADDR_LPM_Axis3;
volatile INT16U * LPM_Axis4	= (INT16U *) mCPLDADDR_LPM_Axis4;
volatile INT16U * LPM_Axis5  	= (INT16U *) mCPLDADDR_LPM_Axis5;
volatile INT16U * LPM_Axis6  	= (INT16U *) mCPLDADDR_LPM_Axis6;
volatile INT16U * LPM_Axis7  	= (INT16U *) mCPLDADDR_LPM_Axis7;
volatile INT16U * LPM_Axis8	= (INT16U *) mCPLDADDR_LPM_Axis8;
volatile INT16U * LPM_Axis9  	= (INT16U *) mCPLDADDR_LPM_Axis9;
volatile INT16U * LPM_Axis10  	= (INT16U *) mCPLDADDR_LPM_Axis10;
volatile INT16U * LPM_Axis11 	= (INT16U *) mCPLDADDR_LPM_Axis11;
volatile INT16U * LPM_Axis12	= (INT16U *) mCPLDADDR_LPM_Axis12;
volatile INT16U * LPM_Axis13  	= (INT16U *) mCPLDADDR_LPM_Axis13;
volatile INT16U * LPM_Axis14  	= (INT16U *) mCPLDADDR_LPM_Axis14;
volatile INT16U * LPM_Axis15  	= (INT16U *) mCPLDADDR_LPM_Axis15;
volatile INT16U * LPM_Axis16	= (INT16U *) mCPLDADDR_LPM_Axis16;
volatile INT16U * LPM_Axis17	= (INT16U *) mCPLDADDR_LPM_Axis17;
volatile INT16U * LPM_Axis18  	= (INT16U *) mCPLDADDR_LPM_Axis18;
volatile INT16U * LPM_Axis19  	= (INT16U *) mCPLDADDR_LPM_Axis19;
volatile INT16U * LPM_Axis20	= (INT16U *) mCPLDADDR_LPM_Axis20;
volatile INT16U * LPM_Axis21  	= (INT16U *) mCPLDADDR_LPM_Axis21;	
volatile INT16U * LPM_Axis22  	= (INT16U *) mCPLDADDR_LPM_Axis22;
volatile INT16U * LPM_Axis23  	= (INT16U *) mCPLDADDR_LPM_Axis23;

volatile INT16U * LPM_Enable 	= (INT16U *) mCPLDADDR_LPM_Enable;

volatile INT16U * SERVO_ON1 	= (INT16U *) mCPLDADDR_SERVO_ON1;	
volatile INT16U * SERVO_ON2 	= (INT16U *) mCPLDADDR_SERVO_ON2;

volatile INT16U * SERVO_Dir1 	= (INT16U *) mCPLDADDR_SERVO_Dir1;
volatile INT16U * SERVO_Dir2 	= (INT16U *) mCPLDADDR_SERVO_Dir2;

volatile INT16U * ENCODER_Clear1 	= (INT16U *) mCPLDADDR_ENCODER_Clear1;	
volatile INT16U * ENCODER_Clear2 	= (INT16U *) mCPLDADDR_ENCODER_Clear2;	

volatile INT16U * IO_OUT1  	= (INT16U *) mCPLDADDR_IO_OUT1;	//W
volatile INT16U * IO_OUT2  	= (INT16U *) mCPLDADDR_IO_OUT2;	//W
volatile INT16U * IO_OUT3  	= (INT16U *) mCPLDADDR_IO_OUT3;	//W
volatile INT16U * IO_OUT4  	= (INT16U *) mCPLDADDR_IO_OUT4;	//W

volatile INT16U * LED_OUT1  	= (INT16U *) mCPLDADDR_LED_OUT1; 	//W bit 0~7
volatile INT16U * LED_OUT2  	= (INT16U *) mCPLDADDR_LED_OUT2; 	//W bit 0~1

volatile INT16U * ABSENCODER_SEN  	= (INT16U *) mCPLDADDR_ABSENCODER_SEN ;//0x8E4	¾ø¶Ô±àÂëÆ÷ËÅ·þÐÅºÅSENÊä³ö	Öá14~1¶ÔÓ¦Î»13~0

volatile INT16U * WRITE_Baudrate_Uart_1_2  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_Uart_1_2;
volatile INT16U * WRITE_Baudrate_Uart_3_4 	= (INT16U *) mCPLDADDR_WRITE_Baudrate_Uart_3_4;
volatile INT16U * WRITE_Baudrate_Uart_5_6  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_Uart_5_6;
volatile INT16U * WRITE_Baudrate_Uart_7_8  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_Uart_7_8;
volatile INT16U * WRITE_Baudrate_RS232_1_2  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_RS232_1_2;
volatile INT16U * WRITE_Baudrate_RS232_3_4  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_RS232_3_4;
volatile INT16U * WRITE_Baudrate_RS485_1_2  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_RS485_1_2;
volatile INT16U * WRITE_Baudrate_RS490_1_2  	= (INT16U *) mCPLDADDR_WRITE_Baudrate_RS490_1_2;

volatile INT16U * WRITE_GPIO_UART0  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART0; 	//W  data=bit0~7  
volatile INT16U * WRITE_GPIO_UART1  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART1; 	//W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART2  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART2;	//W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART3  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART3; 	//W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART4  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART4; 	//W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART5  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART5; 	//W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART6  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART6; //W  data=bit0~7
volatile INT16U * WRITE_GPIO_UART7  	= (INT16U *) mCPLDADDR_WRITE_GPIO_UART7; 	//W  data=bit0~7

volatile INT16U * WRITE_RS232_1  	= (INT16U *) mCPLDADDR_WRITE_RS232_1; 	//W  data=bit0~7
volatile INT16U * WRITE_RS232_2  	= (INT16U *) mCPLDADDR_WRITE_RS232_2; 	//W  data=bit0~7
volatile INT16U * WRITE_RS232_3  	= (INT16U *) mCPLDADDR_WRITE_RS232_3; 	//W  data=bit0~7
volatile INT16U * WRITE_RS232_4  	= (INT16U *) mCPLDADDR_WRITE_RS232_4; 	//W  data=bit0~7
volatile INT16U * WRITE_RS485_1  	= (INT16U *) mCPLDADDR_WRITE_RS485_1; 	//W  data=bit0~7
volatile INT16U * WRITE_RS485_2  	= (INT16U *) mCPLDADDR_WRITE_RS485_2; 	//W  data=bit0~7
volatile INT16U * WRITE_RS485_3  	= (INT16U *) mCPLDADDR_WRITE_RS485_3; 	//W  data=bit0~7
volatile INT16U * WRITE_RS485_4 	= (INT16U *) mCPLDADDR_WRITE_RS485_4; 	//W  data=bit0~7
volatile INT16U * WRITE_RS490_1  	= (INT16U *) mCPLDADDR_WRITE_RS490_1; 	//W  data=bit0~7
volatile INT16U * WRITE_RS490_2  	= (INT16U *) mCPLDADDR_WRITE_RS490_2; 	//W  data=bit0~7
volatile INT16U * WRITE_I2C_1  	= (INT16U *) mCPLDADDR_WRITE_I2C_1; 	//W  data=bit0~7
volatile INT16U * WRITE_I2C_2  	= (INT16U *) mCPLDADDR_WRITE_I2C_2; 	//W  data=bit0~7
volatile INT16U * WRITE_I2C_3  	= (INT16U *) mCPLDADDR_WRITE_I2C_3; 	//W  data=bit0~7
volatile INT16U * WRITE_I2C_4  	= (INT16U *) mCPLDADDR_WRITE_I2C_4; 	//W  data=bit0~7


volatile INT16U * COM_IN_FIFO_Clear  	= (INT16U *) mCPLDADDR_COM_IN_FIFO_Clear ;	//W
volatile INT16U * COM_OUT_FIFO_Clear  	= (INT16U *) mCPLDADDR_COM_OUT_FIFO_Clear ;	//W
volatile INT16U * COM_IN_FIFO_Clear_2  	= (INT16U *) mCPLDADDR_COM_IN_FIFO_Clear_2 ;	//W
volatile INT16U * COM_OUT_FIFO_Clear_2  	= (INT16U *) mCPLDADDR_COM_OUT_FIFO_Clear_2 ;	//W

volatile INT16U * WRITE_STM32_Data  	= (INT16U *) mCPLDADDR_WRITE_STM32_Data; 	//W  data=bit0~7
volatile INT16U * STM32_IN_FIFO_Clear  	= (INT16U *) mCPLDADDR_STM32_IN_FIFO_Clear; 	//W 0x954	stmÊäÈëFIFOÇå¿Õ	0Î»ÓÐÐ§	Ð´1´Î0Çå¿Õ

volatile INT16U * LED1_OUT_Time 	= (INT16U *) mCPLDADDR_LED1_OUT_Time; //0x958	led1µÆ¿ØÊ±¼ä	16Î»	µ¥Î»¡¡£±us
volatile INT16U * LED2_OUT_Time  	= (INT16U *) mCPLDADDR_LED2_OUT_Time; //0x95C	led2µÆ¿ØÊ±¼ä	16Î»
volatile INT16U * LED_OUT_Ctrl  	= (INT16U *) mCPLDADDR_LED_OUT_Ctrl; //0x960	LEDµÆ¿Ø´¥·¢ bit0=1 µÆ1; bit1=1 µÆ2...	(Ð´1¼´ÄÜ´¥·¢Ò»´Î)

volatile INT16U * DA_Convert_Ctrl  	= (INT16U *) mCPLDADDR_DA_Convert; //0x964		
volatile INT16U * DA_Convert_CS  	= (INT16U *) mCPLDADDR_DA_CS; //0x968	

volatile INT16U * DA_Convert_RiseCount  		= (INT16U *) mCPLDADDR_DA_RiseCount; //0x96C	
volatile INT16U * DA_Convert_FallCount  		= (INT16U *) mCPLDADDR_DA_FallCount; //0x70
volatile INT16U * DA_Convert_HKeepCount  		= (INT16U *) mCPLDADDR_DA_HKeepCount; //0x974		
volatile INT16U * DA_Convert_LKeepCount  		= (INT16U *) mCPLDADDR_DA_LKeepCount; //0x978	
volatile INT16U * DA_Convert_HIncrument  		= (INT16U *) mCPLDADDR_DA_HIncrument; //0x97C	

volatile INT16U * DA_Convert_Ctrl_2  	= (INT16U *) mCPLDADDR_DA2_Convert; //0x990		
volatile INT16U * DA_Convert_CS_2  	= (INT16U *) mCPLDADDR_DA2_CS; //0x994	

volatile INT16U * DA_Convert_AutoSign  	= (INT16U *) mCPLDADDR_DA2_AutoSign; //0x998		
volatile INT16U * DA_Convert_Vmax  		= (INT16U *) mCPLDADDR_DA2_Vmax; //0x99C	
volatile INT16U * DA_Convert_Vs  		= (INT16U *) mCPLDADDR_DA2_Vs; //0x9A0		
volatile INT16U * DA_Convert_Cycle  		= (INT16U *) mCPLDADDR_DA2_Cycle; //0x9A4
volatile INT16U * DA_Convert_LIncrument  		= (INT16U *) mCPLDADDR_DA_LIncrument; //0x9A8	


volatile INT16U * AOTF_HighCount 	= (INT16U *) mCPLDADDR_AOTF_HighCount; //0x9D0		
volatile INT16U * AOTF_LowCount 	= (INT16U *) mCPLDADDR_AOTF_LowCount; //0x9D4	
volatile INT16U * AOTF_Phase		= (INT16U *) mCPLDADDR_AOTF_Phase; //0x9D8		
volatile INT16U * AOTF_Enable		= (INT16U *) mCPLDADDR_AOTF_Enable; //0x9DC

volatile INT16U * Pockels_HighCount 	= (INT16U *) mCPLDADDR_Pockels_HighCount; //0x9E0		
volatile INT16U * Pockels_LowCount 	= (INT16U *) mCPLDADDR_Pockels_LowCount; //0x9E4	
volatile INT16U * Pockels_Phase		= (INT16U *) mCPLDADDR_Pockels_Phase; //0x9E8		
volatile INT16U * Pockels_Enable		= (INT16U *) mCPLDADDR_Pockels_Enable; //0x9EC

volatile INT16U * PWM_HighCount 	= (INT16U *) mCPLDADDR_PWM_HighCount; //0x9F0		
volatile INT16U * PWM_LowCount 	= (INT16U *) mCPLDADDR_PWM_LowCount; //0x9F4	
volatile INT16U * PWM_Phase		= (INT16U *) mCPLDADDR_PWM_Phase; //0x9F8		
volatile INT16U * PWM_Enable		= (INT16U *) mCPLDADDR_PWM_Enable; //0x9FC

volatile INT16U * PWM2_HighCount 	= (INT16U *) mCPLDADDR_PWM2_HighCount; //0xA00		
volatile INT16U * PWM2_LowCount 	= (INT16U *) mCPLDADDR_PWM2_LowCount; //0xA04	
volatile INT16U * PWM2_Phase		= (INT16U *) mCPLDADDR_PWM2_Phase; //0xA08		
volatile INT16U * PWM2_Enable		= (INT16U *) mCPLDADDR_PWM2_Enable; //0xA0C

volatile INT16U * DA_Convert_Ctrl_3  	= (INT16U *) mCPLDADDR_DA3_Convert; //0xA10		
volatile INT16U * DA_Convert_CS_3  	= (INT16U *) mCPLDADDR_DA3_CS; //0xA14	

volatile INT16U * DA_Convert_HDividedNum  	= (INT16U *) mCPLDADDR_DA_HDividedNum; //0xA18		
volatile INT16U * DA_Convert_LDividedNum  	= (INT16U *) mCPLDADDR_DA_LDividedNum; //0xA1C	

//GPIO_IRQ
#if 0
const int GPIO0_BASEADDR = 0x48032000;
volatile int *GPIO0_IRQSTATUS_0_REG = (int *) (GPIO0_BASEADDR + 0x2C);
volatile int *GPIO0_IRQSTATUS_1_REG = (int *) (GPIO0_BASEADDR + 0x30);
const int GPIO1_BASEADDR = 0x4804C000;
volatile int *GPIO1_IRQSTATUS_0_REG = (int *) (GPIO1_BASEADDR + 0x2C);
volatile int *GPIO1_IRQSTATUS_1_REG = (int *) (GPIO1_BASEADDR + 0x30);
#endif

volatile int *GPIO0_IRQSTATUS_RAW_0_REG = (int *) (0x48032000 + 0x24);
volatile int *GPIO0_IRQSTATUS_RAW_1_REG = (int *) (0x48032000 + 0x28);
volatile int *GPIO1_IRQSTATUS_RAW_0_REG = (int *) (0x4804C000 + 0x24);
volatile int *GPIO1_IRQSTATUS_RAW_1_REG = (int *) (0x4804C000 + 0x28);


volatile int *GPIO0_IRQSTATUS_0_REG = (int *) (0x48032000 + 0x2C);
volatile int *GPIO0_IRQSTATUS_1_REG = (int *) (0x48032000 + 0x30);
volatile int *GPIO1_IRQSTATUS_0_REG = (int *) (0x4804C000 + 0x2C);
volatile int *GPIO1_IRQSTATUS_1_REG = (int *) (0x4804C000 + 0x30);


union SERVO_OVER1_IN_REG	Servo_Over1_In_Register;
union SERVO_OVER2_IN_REG	Servo_Over2_In_Register;
union SERVO_ALARM1_IN_REG	Servo_Alarm1_In_Register;
union SERVO_ALARM2_IN_REG	Servo_Alarm2_In_Register;
union SERVO_COIN1_IN_REG	Servo_Coin1_In_Register;
union SERVO_COIN2_IN_REG	Servo_Coin2_In_Register;
union SERVO_HLIMIT_POS1_IN_REG	Servo_HardLimit_Positive1_In_Register;
union SERVO_HLIMIT_POS2_IN_REG	Servo_HardLimit_Positive2_In_Register;
union SERVO_HLIMIT_NEG1_IN_REG	Servo_HardLimit_Negative1_In_Register;
union SERVO_HLIMIT_NEG2_IN_REG	Servo_HardLimit_Negative2_In_Register;
union SERVO_ENCODER_R1_IN_REG	Servo_Encoder_Reference1_In_Register;
union SERVO_ENCODER_R2_IN_REG	Servo_Encoder_Reference2_In_Register;
union GPIO_IN1_REG	GPIO_In1_Register;
union GPIO_IN2_REG	GPIO_In2_Register;
union GPIO_IN3_REG	GPIO_In3_Register;
union GPIO_IN4_REG	GPIO_In4_Register;
union SERVO_COM_READSIGN_REG	Servo_Com_ReadSign_Register;
union SERVO_LPMEN_REG	Servo_LPMEN_Register;
union SERVO_ON1_OUT_REG	Servo_On1_Out_Register;
union SERVO_ON2_OUT_REG	Servo_On2_Out_Register;
union SERVO_SIGN1_OUT_REG	Servo_Sign1_Out_Register;
union SERVO_SIGN2_OUT_REG	Servo_Sign2_Out_Register;
union SERVO_ENCODER_CLEAR1_OUT_REG	Servo_Encoder_Clear1_Out_Register;
union SERVO_ENCODER_CLEAR2_OUT_REG	Servo_Encoder_Clear2_Out_Register;
union GPIO_OUT1_REG		GPIO_Out1_Register;
union GPIO_OUT2_REG		GPIO_Out2_Register;
union GPIO_OUT3_REG		GPIO_Out3_Register;
union GPIO_OUT4_REG		GPIO_Out4_Register;
union LEDTest_OUT1_REG	LEDTest_Out1_Register;
union LEDTest_OUT2_REG	LEDTest_Out2_Register;
union SERVO_COM_FIFO_IN_CLEAR_REG	Servo_Com_FIFO_In_Clear_Register;
union SERVO_COM_FIFO_OUT_CLEAR_REG	Servo_Com_FIFO_Out_Clear_Register;

union MAINCOMMANDSIGN_REG ABSENCODER_SEN_Register;


union MAINCOMMANDSIGN_REG GPIO0_IRQSTATUS_RAW_0_Register;
union MAINCOMMANDSIGN_REG GPIO0_IRQSTATUS_RAW_1_Register;
union MAINCOMMANDSIGN_REG GPIO1_IRQSTATUS_RAW_0_Register;
union MAINCOMMANDSIGN_REG GPIO1_IRQSTATUS_RAW_1_Register;

union MAINCOMMANDSIGN_REG GPIO0_IRQSTATUS_0_Register;
union MAINCOMMANDSIGN_REG GPIO0_IRQSTATUS_1_Register;
union MAINCOMMANDSIGN_REG GPIO1_IRQSTATUS_0_Register;
union MAINCOMMANDSIGN_REG GPIO1_IRQSTATUS_1_Register;


union MAINCOMMANDSIGN_REG MainCommand_Register;
union MAINCOMMANDSIGN_REG Output_Register;
union MAINSTATUSSIGN_REG MainStatus_Register;

 ENCODER Encoder_Axis1 ;
 ENCODER Encoder_Axis2 ;
 ENCODER Encoder_Axis3 ;
 ENCODER Encoder_Axis4 ;
 ENCODER Encoder_Axis5 ;
 ENCODER Encoder_Axis6 ;
 ENCODER Encoder_Axis7 ;
 ENCODER Encoder_Axis8 ;
 ENCODER Encoder_Axis9 ;
 ENCODER Encoder_Axis10 ;
 ENCODER Encoder_Axis11 ;
 ENCODER Encoder_Axis12 ;
 ENCODER Encoder_Axis13 ;
 ENCODER Encoder_Axis14 ;
 ENCODER Encoder_Axis15 ;
 ENCODER Encoder_Axis16 ;
 ENCODER Encoder_Axis17 ;
 ENCODER Encoder_Axis18 ;
 ENCODER Encoder_Axis19;
 ENCODER Encoder_Axis20 ;
 ENCODER Encoder_Axis21 ;
 ENCODER Encoder_Axis22 ;
 ENCODER Encoder_Axis23 ;

 PMT Pmt_Counter1;
 PMT Pmt_Counter2;

 SERVO Servo_Axis1;
 SERVO Servo_Axis2;
 SERVO Servo_Axis3;
 SERVO Servo_Axis4;
 SERVO Servo_Axis5;
 SERVO Servo_Axis6;
 SERVO Servo_Axis7;
 SERVO Servo_Axis8;
 SERVO Servo_Axis9;
 SERVO Servo_Axis10;
 SERVO Servo_Axis11;
 SERVO Servo_Axis12;
 SERVO Servo_Axis13;
 SERVO Servo_Axis14;
 SERVO Servo_Axis15;
 SERVO Servo_Axis16;
 SERVO Servo_Axis17;
 SERVO Servo_Axis18;
 SERVO Servo_Axis19;
 SERVO Servo_Axis20;
 SERVO Servo_Axis21;
 SERVO Servo_Axis22;
 SERVO Servo_Axis23;

GPIO_IRQSTATUS GPIO_IRQStatus;

//for test
union MAINCOMMANDSIGN_REG MainCommand_ForTestSign;

void Read_Servo_Over_In(void)
{
	Read_Servo_Over1_In();
	Read_Servo_Over2_In();
}


void Read_Servo_Over1_In(void)
{
	Servo_Over1_In_Register.all = *SERVO_OVER1;

	if(System.RunState == SimulateRun)
	{	
		Servo_Over1_In_Register.bit.OVER_1=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_2=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_3=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_4=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_5=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_15=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_16=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_17=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_18=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_11=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_12=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_13=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_14=Over_Signal_Valid;
		Servo_Over1_In_Register.bit.OVER_23=Over_Signal_Valid;
	}	

	if(System.FunctionSelect10 == True)
	{
		OverallSign.EveryAxisOverValid1 = (Servo_Over1_In_Register.bit.OVER_1 & 
										Servo_Over1_In_Register.bit.OVER_2 & 
										Servo_Over1_In_Register.bit.OVER_3 &
										Servo_Over1_In_Register.bit.OVER_4 &
										Servo_Over1_In_Register.bit.OVER_5
										);
	}
	else
	{
		OverallSign.EveryAxisOverValid1 = (Servo_Over1_In_Register.bit.OVER_1 & 
										Servo_Over1_In_Register.bit.OVER_2 & 
										Servo_Over1_In_Register.bit.OVER_3 &
										Servo_Over1_In_Register.bit.OVER_4 &
										Servo_Over1_In_Register.bit.OVER_5 & 
										Servo_Over1_In_Register.bit.OVER_15 & 
										Servo_Over1_In_Register.bit.OVER_16 &
										Servo_Over1_In_Register.bit.OVER_17 &
										Servo_Over1_In_Register.bit.OVER_18 &
										Servo_Over1_In_Register.bit.OVER_11 & 
										Servo_Over1_In_Register.bit.OVER_12 & 
										Servo_Over1_In_Register.bit.OVER_13 &
										Servo_Over1_In_Register.bit.OVER_14 &
										Servo_Over1_In_Register.bit.OVER_23
										);
	}
	
}

void Read_Servo_Over2_In(void)
{
	Servo_Over2_In_Register.all = *SERVO_OVER2;

	if(System.RunState == SimulateRun)
	{	
		Servo_Over2_In_Register.bit.OVER_6=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_7=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_8=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_9=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_10=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_19=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_20=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_21=Over_Signal_Valid;
		Servo_Over2_In_Register.bit.OVER_22=Over_Signal_Valid;
	}

	if(System.FunctionSelect10 == True)
	{
		OverallSign.EveryAxisOverValid2 = (Servo_Over2_In_Register.bit.OVER_6);
	}
	else
	{
		OverallSign.EveryAxisOverValid2 = (Servo_Over2_In_Register.bit.OVER_6 & 
										Servo_Over2_In_Register.bit.OVER_7 & 
										Servo_Over2_In_Register.bit.OVER_8 &
										Servo_Over2_In_Register.bit.OVER_19 &
										Servo_Over2_In_Register.bit.OVER_20 &
										Servo_Over2_In_Register.bit.OVER_9 & 
										Servo_Over2_In_Register.bit.OVER_10 & 
										Servo_Over2_In_Register.bit.OVER_21 &
										Servo_Over2_In_Register.bit.OVER_22
										);
	}

	OverallSign.EveryAxisOverValid = (OverallSign.EveryAxisOverValid1 || OverallSign.EveryAxisOverValid2);
	
}



void Read_Servo_Alarm1_In(void)
{		
	Servo_Alarm1_In_Register.all = *SERVO_ALARM1;

	if(MainCommand_ForTestSign.bit.SIGN1)
	{		
		//Servo_Alarm1_In_Register.bit.ALM_1=0;
		if(System.FunctionSelect05 == False)
		{	//for test alpha machine
			Servo_Alarm1_In_Register.bit.ALM_1=Alarm_Signal_Invalid;
			Servo_Alarm1_In_Register.bit.ALM_15=Alarm_Signal_Invalid;
			Servo_Alarm1_In_Register.bit.ALM_16=Alarm_Signal_Invalid;
			Servo_Alarm1_In_Register.bit.ALM_17=Alarm_Signal_Invalid;
			Servo_Alarm1_In_Register.bit.ALM_18=Alarm_Signal_Invalid;
		}
		if(System.FunctionSelect05 == True)
		{	//for test plus machine
			Servo_Alarm1_In_Register.bit.ALM_2=Alarm_Signal_Invalid;
		}
		//Servo_Alarm1_In_Register.bit.ALM_3=Alarm_Signal_Invalid;
		//Servo_Alarm1_In_Register.bit.ALM_4=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_5=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_15=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_16=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_17=Alarm_Signal_Invalid;//for test beta
		Servo_Alarm1_In_Register.bit.ALM_18=Alarm_Signal_Invalid;
		//Servo_Alarm1_In_Register.bit.ALM_11=Alarm_Signal_Invalid;
		//Servo_Alarm1_In_Register.bit.ALM_12=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_13=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_14=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_23=Alarm_Signal_Invalid;
	}	

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_Alarm1_In_Register.bit.ALM_1=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_2=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_3=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_4=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_5=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_15=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_16=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_17=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_18=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_11=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_12=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_13=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_14=Alarm_Signal_Invalid;
		Servo_Alarm1_In_Register.bit.ALM_23=Alarm_Signal_Invalid;
	}
	
	Error.ServoAlarm.Axis1 = Servo_Alarm1_In_Register.bit.ALM_1?1:0;
	Error.ServoAlarm.Axis2 = Servo_Alarm1_In_Register.bit.ALM_2?1:0;
	Error.ServoAlarm.Axis3 = Servo_Alarm1_In_Register.bit.ALM_3?1:0;
	Error.ServoAlarm.Axis4 = Servo_Alarm1_In_Register.bit.ALM_4?1:0;
	Error.ServoAlarm.Axis5 = Servo_Alarm1_In_Register.bit.ALM_5?1:0;
	Error.ServoAlarm.Axis11 = Servo_Alarm1_In_Register.bit.ALM_11?1:0;
	Error.ServoAlarm.Axis12 = Servo_Alarm1_In_Register.bit.ALM_12?1:0;
	Error.ServoAlarm.Axis13 = Servo_Alarm1_In_Register.bit.ALM_13?1:0;
	Error.ServoAlarm.Axis14 = Servo_Alarm1_In_Register.bit.ALM_14?1:0;
	Error.ServoAlarm.Axis15 = Servo_Alarm1_In_Register.bit.ALM_15?1:0;
	Error.ServoAlarm.Axis16 = Servo_Alarm1_In_Register.bit.ALM_16?1:0;
	Error.ServoAlarm.Axis17 = Servo_Alarm1_In_Register.bit.ALM_17?1:0;
	Error.ServoAlarm.Axis18 = Servo_Alarm1_In_Register.bit.ALM_18?1:0;
	Error.ServoAlarm.Axis23 = Servo_Alarm1_In_Register.bit.ALM_23?1:0;
	
	if( Error.ServoAlarm.Axis1 || Error.ServoAlarm.Axis2 || Error.ServoAlarm.Axis3 ||
		Error.ServoAlarm.Axis4 || Error.ServoAlarm.Axis5 || Error.ServoAlarm.Axis11 ||
		Error.ServoAlarm.Axis12 || Error.ServoAlarm.Axis13 || Error.ServoAlarm.Axis14 ||
		Error.ServoAlarm.Axis15 || Error.ServoAlarm.Axis16 || Error.ServoAlarm.Axis17 ||
		Error.ServoAlarm.Axis18 || Error.ServoAlarm.Axis23
	)
	{	
		Error.MainErrorSign 	= True;
		Reference.FindRefSign.Axis1 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis2 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis3 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis4 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis5 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis11 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis12 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis13 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis14 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis15 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis16 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis17 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis18 =FindRefSign_Invalid;
		Reference.FindRefSign.Axis23 =FindRefSign_Invalid;	
	}	
	
}

void Read_Servo_Alarm2_In(void)
{		
	Servo_Alarm2_In_Register.all = *SERVO_ALARM2;

	if(MainCommand_ForTestSign.bit.SIGN2)
	{	
		//Servo_Alarm2_In_Register.bit.ALM_6=0;
		//Servo_Alarm2_In_Register.bit.ALM_7=0;
		//Servo_Alarm2_In_Register.bit.ALM_8=0;		
		//Servo_Alarm2_In_Register.bit.ALM_9=0;
		if(System.FunctionSelect05 == True)
		{
			Servo_Alarm2_In_Register.bit.ALM_10=Alarm_Signal_Invalid;
			
			if(Servo_Alarm2_In_Register.bit.ALM_19 == Alarm_Signal_Valid)
			{	//for test Alpha 0x19=L18_Slave//always on
				Servo_On2_Out_Register.bit.SERVO_ON_19 = 1;
				Write_Servo_On2_Out();
			}
		}
		if(System.FunctionSelect05 == False)
		{	
			Servo_Alarm2_In_Register.bit.ALM_19=Alarm_Signal_Invalid;//for test Alpha ===>Beta L18_Slave Alarm
			Servo_Alarm2_In_Register.bit.ALM_20=Alarm_Signal_Invalid;//for test Alpha
			Servo_Alarm2_In_Register.bit.ALM_21=Alarm_Signal_Invalid;
			Servo_Alarm2_In_Register.bit.ALM_22=Alarm_Signal_Invalid;
			Servo_Alarm2_In_Register.bit.ALM_9=Alarm_Signal_Invalid;//for test Alpha
		}
		
		Servo_Alarm2_In_Register.bit.ALM_19=Alarm_Signal_Invalid;//for test Alpha ===>Beta L18_Slave Alarm
		Servo_Alarm2_In_Register.bit.ALM_20=Alarm_Signal_Invalid;//for test Alpha
		Servo_Alarm2_In_Register.bit.ALM_21=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_22=Alarm_Signal_Invalid;

	}

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_Alarm2_In_Register.bit.ALM_6=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_7=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_8=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_9=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_10=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_19=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_20=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_21=Alarm_Signal_Invalid;
		Servo_Alarm2_In_Register.bit.ALM_22=Alarm_Signal_Invalid;
	}
	
	Error.ServoAlarm.Axis6 = Servo_Alarm2_In_Register.bit.ALM_6?1:0;
	Error.ServoAlarm.Axis7 = Servo_Alarm2_In_Register.bit.ALM_7?1:0;
	Error.ServoAlarm.Axis8 = Servo_Alarm2_In_Register.bit.ALM_8?1:0;
	Error.ServoAlarm.Axis9 = Servo_Alarm2_In_Register.bit.ALM_9?1:0;
	Error.ServoAlarm.Axis10 = Servo_Alarm2_In_Register.bit.ALM_10?1:0;
	Error.ServoAlarm.Axis19 = Servo_Alarm2_In_Register.bit.ALM_19?1:0;
	Error.ServoAlarm.Axis20 = Servo_Alarm2_In_Register.bit.ALM_20?1:0;
	Error.ServoAlarm.Axis21 = Servo_Alarm2_In_Register.bit.ALM_21?1:0;
	Error.ServoAlarm.Axis22 = Servo_Alarm2_In_Register.bit.ALM_22?1:0;

	if( Error.ServoAlarm.Axis6 || Error.ServoAlarm.Axis7 || Error.ServoAlarm.Axis8 ||
		Error.ServoAlarm.Axis9 || Error.ServoAlarm.Axis10 || Error.ServoAlarm.Axis19 ||
		Error.ServoAlarm.Axis20 || Error.ServoAlarm.Axis21 || Error.ServoAlarm.Axis22 
	)
	{	
		Error.MainErrorSign 	= True;
		Reference.FindRefSign.Axis6 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis7 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis8 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis9 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis10 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis19 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis20 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis21 = FindRefSign_Invalid;
		Reference.FindRefSign.Axis22 = FindRefSign_Invalid;	
	}	

}


void Read_Servo_Coin1_In(void)
{		
	Servo_Coin1_In_Register.all = *SERVO_COIN1;

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_Coin1_In_Register.bit.COIN_1=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_2=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_3=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_4=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_5=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_15=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_16=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_17=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_18=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_11=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_12=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_13=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_14=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_23=Inposition_Signal_Valid;
	}
	
	if(MainCommand_ForTestSign.bit.SIGN3)
	{	
		if(System.FunctionSelect05 == False)
		{	//for test
			Servo_Coin1_In_Register.bit.COIN_1=Inposition_Signal_Valid;
		}
		Servo_Coin1_In_Register.bit.COIN_2=Inposition_Signal_Valid;
		//Servo_Coin1_In_Register.bit.COIN_3=Inposition_Signal_Valid;
		//Servo_Coin1_In_Register.bit.COIN_4=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_5=Inposition_Signal_Valid;
		//if(System.FunctionSelect05 == False)
		{
			Servo_Coin1_In_Register.bit.COIN_15=Inposition_Signal_Valid;
			Servo_Coin1_In_Register.bit.COIN_16=Inposition_Signal_Valid;
			Servo_Coin1_In_Register.bit.COIN_17=Inposition_Signal_Valid;
			Servo_Coin1_In_Register.bit.COIN_18=Inposition_Signal_Valid;
		}
		//Servo_Coin1_In_Register.bit.COIN_11=Inposition_Signal_Valid;
		//Servo_Coin1_In_Register.bit.COIN_12=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_13=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_14=Inposition_Signal_Valid;
		Servo_Coin1_In_Register.bit.COIN_23=Inposition_Signal_Valid;
	}
	
	OverallSign.EveryAxisCoinValid1 = (Servo_Coin1_In_Register.bit.COIN_1 | 
									Servo_Coin1_In_Register.bit.COIN_2 | 
									Servo_Coin1_In_Register.bit.COIN_3 |
									Servo_Coin1_In_Register.bit.COIN_4 |
									Servo_Coin1_In_Register.bit.COIN_5 | 
									Servo_Coin1_In_Register.bit.COIN_15 | 
									Servo_Coin1_In_Register.bit.COIN_16 |
									Servo_Coin1_In_Register.bit.COIN_17 |
									Servo_Coin1_In_Register.bit.COIN_18
									);

	OverallSign.EveryAxisCoinValid2 = (Servo_Coin1_In_Register.bit.COIN_11 | 
									Servo_Coin1_In_Register.bit.COIN_12 | 
									Servo_Coin1_In_Register.bit.COIN_13 |
									Servo_Coin1_In_Register.bit.COIN_14 |
									Servo_Coin1_In_Register.bit.COIN_23
									);	
}

void Read_Servo_Coin2_In(void)
{
	Servo_Coin2_In_Register.all = *SERVO_COIN2;

	if(MainCommand_ForTestSign.bit.SIGN4)
	{	
		//Servo_Coin2_In_Register.bit.COIN_6=Inposition_Signal_Valid;
		//Servo_Coin2_In_Register.bit.COIN_7=Inposition_Signal_Valid;
		//Servo_Coin2_In_Register.bit.COIN_8=Inposition_Signal_Valid;
		//Servo_Coin2_In_Register.bit.COIN_9=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_10=Inposition_Signal_Valid;
		//if(System.FunctionSelect05 == False)
		{	
			Servo_Coin2_In_Register.bit.COIN_19=Inposition_Signal_Valid;//for test Beta ===>L18_Slave Coin.
			Servo_Coin2_In_Register.bit.COIN_20=Inposition_Signal_Valid;//for test
			Servo_Coin2_In_Register.bit.COIN_21=Inposition_Signal_Valid;
			Servo_Coin2_In_Register.bit.COIN_22=Inposition_Signal_Valid;
		}
	}

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_Coin2_In_Register.bit.COIN_6=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_7=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_8=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_9=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_10=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_19=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_20=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_21=Inposition_Signal_Valid;
		Servo_Coin2_In_Register.bit.COIN_22=Inposition_Signal_Valid;
	}
	
	OverallSign.EveryAxisCoinValid3 = (Servo_Coin2_In_Register.bit.COIN_6 | 
									Servo_Coin2_In_Register.bit.COIN_7 | 
									Servo_Coin2_In_Register.bit.COIN_8 |
									Servo_Coin2_In_Register.bit.COIN_19 |
									Servo_Coin2_In_Register.bit.COIN_20
									);

	OverallSign.EveryAxisCoinValid4 = (Servo_Coin2_In_Register.bit.COIN_9 | 
									Servo_Coin2_In_Register.bit.COIN_10 | 
									Servo_Coin2_In_Register.bit.COIN_21 |
									Servo_Coin2_In_Register.bit.COIN_22
									);
	
	OverallSign.EveryAxisCoinValid = (OverallSign.EveryAxisCoinValid1 + 
								OverallSign.EveryAxisCoinValid2 +
								OverallSign.EveryAxisCoinValid3 +
								OverallSign.EveryAxisCoinValid4) ? (0):(1);
	
}

void Read_Servo_HardLimit_Positive1_In(void)
{
	Servo_HardLimit_Positive1_In_Register.all = *HARDLIMIT_POSITIVE1;

	if(MainCommand_ForTestSign.bit.SIGN5)
	{	
		//Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23=HardLimit_Signal_Invalid;
		#if 0
		if(System.FunctionSelect05 == True)
		{	//for test Beta
			Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17=HardLimit_Signal_Invalid;
			Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18=HardLimit_Signal_Invalid;
		}
		#endif
	}
	
	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23=HardLimit_Signal_Invalid;
	}
}


void Read_Servo_HardLimit_Positive2_In(void)
{
	Servo_HardLimit_Positive2_In_Register.all = *HARDLIMIT_POSITIVE2;

	if(MainCommand_ForTestSign.bit.SIGN6)
	{
		if(System.FunctionSelect05 == True)
		{	//for test Beta, Limit22===>Limit10
			Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22=Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10;		
		}
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22=HardLimit_Signal_Invalid;
	}	

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21=HardLimit_Signal_Invalid;
		Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22=HardLimit_Signal_Invalid;
	}
	
	if((Servo_HardLimit_Positive1_In_Register.all != HardLimit_Signal_Invalid)||(Servo_HardLimit_Positive2_In_Register.all != HardLimit_Signal_Invalid))
	{
		Error.AlmHLimitPos = True;
	}
	else
	{
		Error.AlmHLimitPos = False;
	}	
}


void Read_Servo_HardLimit_Negative1_In(void)
{
	Servo_HardLimit_Negative1_In_Register.all = *HARDLIMIT_NEGATIVE1;

	if(MainCommand_ForTestSign.bit.SIGN7)
	{	
		//Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17=HardLimit_Signal_Invalid;
		if(System.FunctionSelect05 == False)
		{
			Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18=HardLimit_Signal_Invalid;//for test alpha
		}
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23=HardLimit_Signal_Invalid;

		#if 0
		if(System.FunctionSelect05 == True)
		{	//for test Beta
			Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17=0;
			Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18=0;
		}
		#endif
	}	
	
	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18=HardLimit_Signal_Invalid;//for test
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23=HardLimit_Signal_Invalid;
	}

}


void Read_Servo_HardLimit_Negative2_In(void)
{
	Servo_HardLimit_Negative2_In_Register.all = *HARDLIMIT_NEGATIVE2;

	if(MainCommand_ForTestSign.bit.SIGN8)
	{	
		if(System.FunctionSelect05 == True)
		{	//for test Beta, Limit22===>Limit10
			Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22=Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10;		
		}
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21=HardLimit_Signal_Invalid;
		//Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22=HardLimit_Signal_Invalid;	
	}	

	if((System.RunState == SimulateRun)||(MainCommand_ForTestSign.all == 0xABCD))
	{	
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21=HardLimit_Signal_Invalid;
		Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22=HardLimit_Signal_Invalid;
	}	

	if((Servo_HardLimit_Negative1_In_Register.all != HardLimit_Signal_Invalid)||(Servo_HardLimit_Negative2_In_Register.all != HardLimit_Signal_Invalid))
	{
		Error.AlmHLimitNeg = True;
	}
	else
	{
		Error.AlmHLimitNeg = False;
	}	
}


void Read_Servo_Encoder_Reference1_In(void)
{
	Servo_Encoder_Reference1_In_Register.all = *ENCODER1_REF;
}


void Read_Servo_Encoder_Reference2_In(void)
{
	Servo_Encoder_Reference2_In_Register.all = *ENCODER2_REF;
}


void Read_AD_In(void)		//��ȡAD ת��
{
	Int16 AD0_IN_test,AD1_IN_test,AD2_IN_test,AD3_IN_test,AD4_IN_test,AD5_IN_test,AD6_IN_test;
	float64 V0_IN_test,V1_IN_test,V2_IN_test,V3_IN_test,V4_IN_test,V5_IN_test,V6_IN_test;
	
	AD0_IN_test = *AD0;
	AD1_IN_test = *AD1;
	AD2_IN_test = *AD2;
	AD3_IN_test = *AD3;
	AD4_IN_test = *AD4;
	AD5_IN_test = *AD5;
	AD6_IN_test = *AD6;

	V0_IN_test = divdp(AD0_IN_test,0x7fff)*2.5;
	V1_IN_test = divdp(AD1_IN_test,0x7fff)*2.5;
	V2_IN_test = divdp(AD2_IN_test,0x7fff)*2.5;
	V3_IN_test = divdp(AD3_IN_test,0x7fff)*2.5;
	V4_IN_test = divdp(AD4_IN_test,0x7fff)*2.5;
	V5_IN_test = divdp(AD5_IN_test,0x7fff)*2.5;
	V6_IN_test = divdp(AD6_IN_test,0x7fff)*2.5;

	//printf("V0_IN_test%.3f\n",V0_IN_test);
	//printf("V0_IN_test\n");
	//LOG_printf("V0_IN_test%.3f\n",V0_IN_test);
}


void Read_GPIO_In1(void)
{
	GPIO_In1_Register.all = *IO_IN1;
	if (GPIO_In1_Register.bit.GPIOIN12 == GPIO_IN_Valid)
	{
		GPIO_In1_Register.bit.GPIOIN12 = GPIO_In1_Register.bit.GPIOIN12;
	}
}


void Read_GPIO_In2(void)
{
	GPIO_In2_Register.all = *IO_IN2;
	if(GPIO_In2_Register.all == 0xFFFF)
	{
		GPIO_In2_Register.all = *IO_IN2;
	}
}


void Read_GPIO_In3(void)
{
	GPIO_In3_Register.all = *IO_IN3;
	if(GPIO_In3_Register.all == 0xFFFF)
	{
		GPIO_In3_Register.all = *IO_IN3;
	}
}


void Read_GPIO_In4(void)
{
	GPIO_In4_Register.all = *IO_IN4;
	if(GPIO_In4_Register.all == 0xFFFF)
	{
		GPIO_In4_Register.all = *IO_IN4;
	}
}


void Read_Serial_Com_ReadSign(void)
{
	Servo_Com_ReadSign_Register.all = *READ_COM_ReadSign;
}

void Read_AbsoluteEncoderValue_PSO_Axis(void)
{
	Read_AbsoluteEncoderValue_PSO_Axis1();
	Read_AbsoluteEncoderValue_PSO_Axis2();
	Read_AbsoluteEncoderValue_PSO_Axis3();
	Read_AbsoluteEncoderValue_PSO_Axis4();
	Read_AbsoluteEncoderValue_PSO_Axis5();
	Read_AbsoluteEncoderValue_PSO_Axis6();
	Read_AbsoluteEncoderValue_PSO_Axis7();
	Read_AbsoluteEncoderValue_PSO_Axis8();
	Read_AbsoluteEncoderValue_PSO_Axis9();
	Read_AbsoluteEncoderValue_PSO_Axis10();
	Read_AbsoluteEncoderValue_PSO_Axis11();
	Read_AbsoluteEncoderValue_PSO_Axis12();
	Read_AbsoluteEncoderValue_PSO_Axis13();
	Read_AbsoluteEncoderValue_PSO_Axis14();
	Read_AbsoluteEncoderValue_PSO_Axis15();
	Read_AbsoluteEncoderValue_PSO_Axis16();
	Read_AbsoluteEncoderValue_PSO_Axis17();
	Read_AbsoluteEncoderValue_PSO_Axis18();
	Read_AbsoluteEncoderValue_PSO_Axis19();
	Read_AbsoluteEncoderValue_PSO_Axis20();
	Read_AbsoluteEncoderValue_PSO_Axis21();
	Read_AbsoluteEncoderValue_PSO_Axis22();
	Read_AbsoluteEncoderValue_PSO_Axis23();	
}

void Read_AbsoluteEncoderValue_PSO_Axis1(void)
{
	char MultiTurnBuffer[7];	
	int MultiTurnBufferVulue_Axis1;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis1;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis1)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis1)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis1)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis1)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis1)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis1)& 0xff;
	MultiTurnBuffer[6]=0x0d;
	MultiTurnBufferVulue_Axis1 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis1)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis1)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis1)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis1)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis1)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis1)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis1)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis1)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis1 = atoi(PulseInSingleRotationBuffer);

	//for test Plus and Beta
	if(System.FunctionSelect05 == True)
	{
		Servo_Axis1.Pn210 = 4000;//pitch
		Servo_Axis1.Pn212 = 4000;
	}

	OverallSign.AbsoluteEncoderValue_PSO.Axis1 = -1*(MultiTurnBufferVulue_Axis1*Servo_Axis1.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis1,Servo_Axis1.Pn20E)*Servo_Axis1.Pn210);

	if(System.FunctionSelect05 == False)
	{	//for alpha
		OverallSign.AbsoluteEncoderValue_PSO.Axis1 = OverallSign.ABS_Coordinate.Axis1;
	}
}

void Read_AbsoluteEncoderValue_PSO_Axis2(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis2;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis2;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis2)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis2)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis2)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis2)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis2)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis2)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis2 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis2)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis2)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis2)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis2)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis2)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis2)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis2)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis2)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis2 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis2 = MultiTurnBufferVulue_Axis2*Servo_Axis2.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis2,Servo_Axis2.Pn20E)*Servo_Axis2.Pn210;
}

void Read_AbsoluteEncoderValue_PSO_Axis3(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis3;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis3;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis3)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis3)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis3)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis3)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis3)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis3)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis3 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis3)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis3)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis3)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis3)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis3)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis3)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis3)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis3)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis3 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis3 = -1*(MultiTurnBufferVulue_Axis3*Servo_Axis3.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis3,Servo_Axis3.Pn20E)*Servo_Axis3.Pn210);

}

void Read_AbsoluteEncoderValue_PSO_Axis4(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis4;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis4;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis4)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis4)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis4)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis4)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis4)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis4)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis4 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis4)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis4)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis4)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis4)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis4)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis4)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis4)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis4)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis4 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis4 = -1*(MultiTurnBufferVulue_Axis4*Servo_Axis4.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis4,Servo_Axis4.Pn20E)*Servo_Axis4.Pn210);

}

void Read_AbsoluteEncoderValue_PSO_Axis5(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis5;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis5;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis5)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis5)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis5)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis5)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis5)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis5)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis5 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis5)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis5)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis5)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis5)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis5)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis5)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis5)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis5)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis5 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis5 = MultiTurnBufferVulue_Axis5*Servo_Axis5.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis5,Servo_Axis5.Pn20E)*Servo_Axis5.Pn210;
}

void Read_AbsoluteEncoderValue_PSO_Axis6(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis6;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis6;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis6)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis6)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis6)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis6)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis6)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis6)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis6 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis6)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis6)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis6)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis6)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis6)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis6)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis6)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis6)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis6 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis6 = -1*(MultiTurnBufferVulue_Axis6*Servo_Axis6.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis6,Servo_Axis6.Pn20E)*Servo_Axis6.Pn210);

	if(OverallSign.AbsoluteEncoderValue_PSO.Axis6  < -100)
	{
		OverallSign.AbsoluteEncoderValue_PSO.Axis6 = OverallSign.AbsoluteEncoderValue_PSO.Axis6;
	}
}

void Read_AbsoluteEncoderValue_PSO_Axis7(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis7;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis7;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis7)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis7)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis7)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis7)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis7)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis7)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis7 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis7)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis7)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis7)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis7)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis7)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis7)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis7)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis7)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis7 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis7 = MultiTurnBufferVulue_Axis7*Servo_Axis7.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis7,Servo_Axis7.Pn20E)*Servo_Axis7.Pn210;

}

void Read_AbsoluteEncoderValue_PSO_Axis8(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis8;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis8;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis8)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis8)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis8)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis8)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis8)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis8)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis8 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis8)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis8)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis8)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis8)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis8)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis8)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis8)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis8)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis8 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis8 = MultiTurnBufferVulue_Axis8*Servo_Axis8.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis8,Servo_Axis8.Pn20E)*Servo_Axis8.Pn210;
}

void Read_AbsoluteEncoderValue_PSO_Axis9(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis9;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis9;

	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis9)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis9)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis9)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis9)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis9)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis9)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis9 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis9)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis9)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis9)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis9)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis9)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis9)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis9)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis9)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis9 = atoi(PulseInSingleRotationBuffer);
		
	if(System.FunctionSelect05 == False)
	{	//for alpha
		Servo_Axis9.Pn210 = 6000;//pitch
		Servo_Axis9.Pn212 = 6000;
		OverallSign.AbsoluteEncoderValue_PSO.Axis9 = -1.0*(MultiTurnBufferVulue_Axis9*Servo_Axis9.Pn212 + 
			divdp(PulseInSingleRotationValue_Axis9,Servo_Axis9.Pn20E)*Servo_Axis9.Pn210);
	}
	else
	{	//for beta
		Servo_Axis9.Pn210 = 10000;//pitch
		Servo_Axis9.Pn212 = 10000;
		OverallSign.AbsoluteEncoderValue_PSO.Axis9 = MultiTurnBufferVulue_Axis9*Servo_Axis9.Pn212 + 
			divdp(PulseInSingleRotationValue_Axis9,Servo_Axis9.Pn20E)*Servo_Axis9.Pn210;
	}
}

void Read_AbsoluteEncoderValue_PSO_Axis10(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis10;

	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis10;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis10)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis10)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis10)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis10)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis10)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis10)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis10 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis10)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis10)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis10)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis10)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis10)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis10)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis10)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis10)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis10 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis10 = MultiTurnBufferVulue_Axis10*Servo_Axis10.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis10,Servo_Axis10.Pn20E)*Servo_Axis10.Pn210;

}

void Read_AbsoluteEncoderValue_PSO_Axis11(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis11;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis11;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis11)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis11)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis11)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis11)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis11)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis11)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis11 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis11)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis11)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis11)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis11)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis11)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis11)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis11)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis11)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis11 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis11 = MultiTurnBufferVulue_Axis11*Servo_Axis11.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis11,Servo_Axis11.Pn20E)*Servo_Axis11.Pn210;

}

void Read_AbsoluteEncoderValue_PSO_Axis12(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis12;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis12;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis12)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis12)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis12)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis12)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis12)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis12)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis12 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis12)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis12)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis12)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis12)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis12)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis12)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis12)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis12)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis12 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis12 = -1.0*(MultiTurnBufferVulue_Axis12*Servo_Axis12.Pn212 +
		divdp(PulseInSingleRotationValue_Axis12,Servo_Axis12.Pn20E)*Servo_Axis12.Pn210);

}

void Read_AbsoluteEncoderValue_PSO_Axis13(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis13;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis13;
	
	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis13)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis13)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis13)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis13)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis13)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis13)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis13 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis13)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis13)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis13)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis13)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis13)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis13)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis13)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis13)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis13 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis13 = MultiTurnBufferVulue_Axis13*Servo_Axis13.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis13,Servo_Axis13.Pn20E)*Servo_Axis13.Pn210;

}

void Read_AbsoluteEncoderValue_PSO_Axis14(void)
{
	char MultiTurnBuffer[7];
	int MultiTurnBufferVulue_Axis14;
	
	char PulseInSingleRotationBuffer[9];
	Uint32 PulseInSingleRotationValue_Axis14;

	MultiTurnBuffer[0]=(* ABSENCODER_Dir_Axis14)& 0xff;
	MultiTurnBuffer[1]=(* ABSENCODER_RotateRingTotalNuberChar0_Axis14)& 0xff;
	MultiTurnBuffer[2]=(* ABSENCODER_RotateRingTotalNuberChar1_Axis14)& 0xff;
	MultiTurnBuffer[3]=(* ABSENCODER_RotateRingTotalNuberChar2_Axis14)& 0xff;
	MultiTurnBuffer[4]=(* ABSENCODER_RotateRingTotalNuberChar3_Axis14)& 0xff;
	MultiTurnBuffer[5]=(* ABSENCODER_RotateRingTotalNuberChar4_Axis14)& 0xff;
	MultiTurnBuffer[6]=0x0d;

	MultiTurnBufferVulue_Axis14 = atoi(MultiTurnBuffer);

	PulseInSingleRotationBuffer[0]=(* ABSENCODER_RingValueChar0_Axis14)& 0xff;
	PulseInSingleRotationBuffer[1]=(* ABSENCODER_RingValueChar1_Axis14)& 0xff;
	PulseInSingleRotationBuffer[2]=(* ABSENCODER_RingValueChar2_Axis14)& 0xff;
	PulseInSingleRotationBuffer[3]=(* ABSENCODER_RingValueChar3_Axis14)& 0xff;
	PulseInSingleRotationBuffer[4]=(* ABSENCODER_RingValueChar4_Axis14)& 0xff;
	PulseInSingleRotationBuffer[5]=(* ABSENCODER_RingValueChar5_Axis14)& 0xff;
	PulseInSingleRotationBuffer[6]=(* ABSENCODER_RingValueChar6_Axis14)& 0xff;
	PulseInSingleRotationBuffer[7]=(* ABSENCODER_RingValueChar7_Axis14)& 0xff;
	PulseInSingleRotationBuffer[8]=0x0d;

	PulseInSingleRotationValue_Axis14 = atoi(PulseInSingleRotationBuffer);

	OverallSign.AbsoluteEncoderValue_PSO.Axis14 = MultiTurnBufferVulue_Axis14*Servo_Axis14.Pn212 + 
		divdp(PulseInSingleRotationValue_Axis14,Servo_Axis14.Pn20E)*Servo_Axis14.Pn210;
}

void Read_AbsoluteEncoderValue_PSO_Axis15(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis15 = psDSPPacketOut->ABSMachineCoordinateAxis15;
}
void Read_AbsoluteEncoderValue_PSO_Axis16(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis16 = psDSPPacketOut->ABSMachineCoordinateAxis16;
}
void Read_AbsoluteEncoderValue_PSO_Axis17(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis17 = psDSPPacketOut->ABSMachineCoordinateAxis17;
}
void Read_AbsoluteEncoderValue_PSO_Axis18(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis18 = psDSPPacketOut->ABSMachineCoordinateAxis18;
}
void Read_AbsoluteEncoderValue_PSO_Axis19(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis19 = psDSPPacketOut->ABSMachineCoordinateAxis19;	
	//OverallSign.AbsoluteEncoderValue_PSO.Axis19 = OverallSign.ABS_Coordinate.Axis19;
}
void Read_AbsoluteEncoderValue_PSO_Axis20(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis20 = psDSPPacketOut->ABSMachineCoordinateAxis20;
}
void Read_AbsoluteEncoderValue_PSO_Axis21(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis21 = psDSPPacketOut->ABSMachineCoordinateAxis21;
}
void Read_AbsoluteEncoderValue_PSO_Axis22(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis22 = psDSPPacketOut->ABSMachineCoordinateAxis22;
}
void Read_AbsoluteEncoderValue_PSO_Axis23(void)
{
	OverallSign.AbsoluteEncoderValue_PSO.Axis23 = Encoder_Axis23.MACH_PositionValue;
}



void Write_Servo_LPMEN(void)
{
	*LPM_Enable = Servo_LPMEN_Register.all;
}


void Write_Servo_On1_Out(void)
{
	*SERVO_ON1 = Servo_On1_Out_Register.all;
}


void Write_Servo_On2_Out(void)
{
	*SERVO_ON2 = Servo_On2_Out_Register.all;
}


void Write_Servo_Sign1_Out(void)
{
	*SERVO_Dir1 = Servo_Sign1_Out_Register.all;
}


void Write_Servo_Sign2_Out(void)
{
	*SERVO_Dir2 = Servo_Sign2_Out_Register.all;
}


void Write_Servo_Encoder_Clear1_Out(void)
{
	*ENCODER_Clear1 = Servo_Encoder_Clear1_Out_Register.all;
}


void Write_Servo_Encoder_Clear2_Out(void)
{
	*ENCODER_Clear2 = Servo_Encoder_Clear2_Out_Register.all;
}


void Write_GPIO_Out1(void)
{
	*IO_OUT1 = GPIO_Out1_Register.all;
}


void Write_GPIO_Out2(void)
{
	*IO_OUT2 = GPIO_Out2_Register.all;
}

void Write_GPIO_Out3(void)
{
	*IO_OUT3 = GPIO_Out3_Register.all;
}


void Write_GPIO_Out4(void)
{
	*IO_OUT4 = GPIO_Out4_Register.all;
}


void Write_LEDTest_Out1(void)
{
	*LED_OUT1 = LEDTest_Out1_Register.all;
}


void Write_LEDTest_Out2(void)
{
	*LED_OUT2 = LEDTest_Out2_Register.all;
}

void Write_ABSENCODER_SEN(void)
{
	* ABSENCODER_SEN = ABSENCODER_SEN_Register.all;
}

void Write_Servo_Com_FIFO_In_Clear(void)
{
	*COM_IN_FIFO_Clear = Servo_Com_FIFO_In_Clear_Register.all;
}

void Write_Servo_Com_FIFO_Out_Clear(void)
{
	*COM_OUT_FIFO_Clear = Servo_Com_FIFO_Out_Clear_Register.all;
}

void Write_LED_OUT_TIME1(void)
{
	*LED1_OUT_Time = OverallSign.LEDFlashTime1;
}

void Write_LED_OUT_TIME2(void)
{
	*LED2_OUT_Time = OverallSign.LEDFlashTime2;
}

void Write_LED_OUT_CONTROL(void)
{
	*LED_OUT_Ctrl = OverallSign.LEDFlashCtr1;
}

void Write_LEDFlash_OUT(Uint16 LEDFlash_TRIGGER_OUT,Uint16 LEDFlash_DATA_OUT_1,Uint16 LEDFlash_DATA_OUT_2)
{
	//if (LEDFlash_DATA_OUT_1 == 0xffff) the Flash1 always on. 
	//if (LEDFlash_DATA_OUT_2 == 0xffff) the Flash2 always on.
	//*LED1_DATA_OUT = LEDFlash_DATA_OUT_1;
	//*LED2_DATA_OUT = LEDFlash_DATA_OUT_2;
	*LED1_OUT_Time = LEDFlash_DATA_OUT_1;
	*LED2_OUT_Time = LEDFlash_DATA_OUT_2;
	*LED_OUT_Ctrl = LEDFlash_TRIGGER_OUT;
	LEDFlash_TRIGGER_OUT = LEDFlash_TRIGGER_OUT;//for break
}


void Write_DA_CONVERT_CONTROL(Uint16 DA_Data,
											Uint16 DA_ADDR,
											Uint16 DA_CS)
{
	Ad_Convert.OutData = 0;
	Ad_Convert.AD5315Str.Data = DA_Data;
	
	Ad_Convert.AD5315Str.Addr1 = ((DA_ADDR & 0x01) == 0x01)?(1):(0);
	Ad_Convert.AD5315Str.Addr2 = ((DA_ADDR & 0x02) == 0x02)?(1):(0);
	Ad_Convert.AD5315Str.Addr3 = ((DA_ADDR & 0x04) == 0x04)?(1):(0);
	Ad_Convert.AD5315Str.Addr4 = ((DA_ADDR & 0x08) == 0x08)?(1):(0);
	
	*DA_Convert_Ctrl = Ad_Convert.OutData;
	*DA_Convert_CS = DA_CS;
}



void Write_DA_CONVERT_CONTROL_2(DACONVERT  *DA_WavePara)
{
	 
	if(DA_WavePara->AutoSign != 0)
	{
		//AutoSign = 1; //=0; for sync test
		DA_WavePara->Voltage_Max = (DA_WavePara->Voltage_Max > 0x3FF)?(0x3FF):(DA_WavePara->Voltage_Max);
		DA_WavePara->Voltage_Start = (DA_WavePara->Voltage_Start > 0x3FF)?(0x3FF):(DA_WavePara->Voltage_Start);

		DA_WavePara->RiseCount = (DA_WavePara->RiseCount < 0x01) ? (0x01):(DA_WavePara->RiseCount);
		DA_WavePara->FallCount = (DA_WavePara->FallCount < 0x01) ? (0x01):(DA_WavePara->FallCount);
		DA_WavePara->HKeepCount = (DA_WavePara->HKeepCount < 0x01) ? (0x01):(DA_WavePara->HKeepCount);
		DA_WavePara->LKeepCount = (DA_WavePara->LKeepCount < 0x01) ? (0x01):(DA_WavePara->LKeepCount);

		DA_WavePara->HIncrument = (DA_WavePara->HIncrument < 1) ? (1):(DA_WavePara->HIncrument);
		DA_WavePara->LIncrument = (DA_WavePara->LIncrument < 1) ? (1):(DA_WavePara->LIncrument);
		DA_WavePara->HDividedNum = (DA_WavePara->HDividedNum < 1) ? (0):(DA_WavePara->HDividedNum);
		DA_WavePara->LDividedNum = (DA_WavePara->LDividedNum < 1) ? (0):(DA_WavePara->LDividedNum);
		
		*DA_Convert_AutoSign = DA_WavePara->AutoSign;
		*DA_Convert_Vmax = DA_WavePara->Voltage_Max;
		*DA_Convert_Vs = DA_WavePara->Voltage_Start;
		*DA_Convert_Cycle = DA_WavePara->CycleNum;

		*DA_Convert_RiseCount = DA_WavePara->RiseCount;
		*DA_Convert_FallCount = DA_WavePara->FallCount;
		*DA_Convert_HKeepCount = DA_WavePara->HKeepCount;
		*DA_Convert_LKeepCount = DA_WavePara->LKeepCount;
		*DA_Convert_HIncrument = DA_WavePara->HIncrument;
		*DA_Convert_LIncrument = DA_WavePara->LIncrument;
		*DA_Convert_HDividedNum = DA_WavePara->HDividedNum;
		*DA_Convert_LDividedNum = DA_WavePara->LDividedNum;

		if(DA_WavePara->DA_ADDR == 1)//DA channel 5
		{
			if(DA_WavePara->AutoSign == 0x03)
			{
				*AOTF_HighCount = DA_WavePara->PWMHighCount;
				*AOTF_LowCount = DA_WavePara->PWMLowCount;
				*AOTF_Phase = DA_WavePara->PWMPhase;
				*AOTF_Enable = 0;				
			}
			else if(DA_WavePara->AutoSign == 0x05)
			{
				*Pockels_HighCount = DA_WavePara->PWMHighCount;
				*Pockels_LowCount = DA_WavePara->PWMLowCount;
				*Pockels_Phase = DA_WavePara->PWMPhase;
				*Pockels_Enable = 0;								
			}
		}
	}
	else
	{
		*DA_Convert_AutoSign = 0;
		*DA_Convert_Vmax = 0;
		*DA_Convert_Vs = 0;
		*DA_Convert_Cycle = 0;

		*DA_Convert_RiseCount = 0;
		*DA_Convert_FallCount = 0;
		*DA_Convert_HKeepCount = 0;
		*DA_Convert_LKeepCount = 0;
		*DA_Convert_HIncrument = 0;
		*DA_Convert_LIncrument = 0;
		*DA_Convert_HDividedNum = 0;
		*DA_Convert_LDividedNum = 0;
	}
	
	Ad_Convert_2.OutData = 0;
	Ad_Convert_2.AD5315Str.Data = DA_WavePara->DA_Data;

	Ad_Convert_2.AD5315Str.Addr1 = ((DA_WavePara->DA_ADDR & 0x01) == 0x01)?(1):(0);
	Ad_Convert_2.AD5315Str.Addr2 = ((DA_WavePara->DA_ADDR & 0x02) == 0x02)?(1):(0);
	Ad_Convert_2.AD5315Str.Addr3 = ((DA_WavePara->DA_ADDR & 0x04) == 0x04)?(1):(0);
	Ad_Convert_2.AD5315Str.Addr4 = ((DA_WavePara->DA_ADDR & 0x08) == 0x08)?(1):(0);

	*DA_Convert_Ctrl_2 = Ad_Convert_2.OutData;
	*DA_Convert_CS_2 = DA_WavePara->DA_CS;
}


void Write_DA_CONVERT_CONTROL_3(Uint16 DA_Data,
											Uint16 DA_ADDR,
											Uint16 DA_CS)
{	
	Ad_Convert_3.OutData = 0;
	Ad_Convert_3.AD5315Str.Data = DA_Data;
	
	Ad_Convert_3.AD5315Str.Addr1 = ((DA_ADDR & 0x01) == 0x01)?(1):(0);
	Ad_Convert_3.AD5315Str.Addr2 = ((DA_ADDR & 0x02) == 0x02)?(1):(0);
	Ad_Convert_3.AD5315Str.Addr3 = ((DA_ADDR & 0x04) == 0x04)?(1):(0);
	Ad_Convert_3.AD5315Str.Addr4 = ((DA_ADDR & 0x08) == 0x08)?(1):(0);

	*DA_Convert_Ctrl_3 = Ad_Convert_3.OutData;
	*DA_Convert_CS_3 = DA_CS;
}



void Write_AOTF_CONTROL(Uint16 HighCount,Uint16 LowCount,Uint16 Phase,Uint16 Enable)
{
	*AOTF_HighCount = HighCount;
	*AOTF_LowCount = LowCount;
	*AOTF_Phase = Phase;
	*AOTF_Enable = Enable;
}

void Write_Pockels_CONTROL(Uint16 HighCount,Uint16 LowCount,Uint16 Phase,Uint16 Enable)
{
	*Pockels_HighCount = HighCount;
	*Pockels_LowCount = LowCount;
	*Pockels_Phase = Phase;
	*Pockels_Enable = Enable;
}

void Write_PWM1_CONTROL(Uint16 HighCount,Uint16 LowCount,Uint16 Phase,Uint16 Enable)
{
	*PWM_HighCount = HighCount;
	*PWM_LowCount = LowCount;
	*PWM_Phase = Phase;
	*PWM_Enable = Enable;
}

void Write_PWM2_CONTROL(Uint16 HighCount,Uint16 LowCount,Uint16 Phase,Uint16 Enable)
{
	*PWM2_HighCount = HighCount;
	*PWM2_LowCount = LowCount;
	*PWM2_Phase = Phase;
	*PWM2_Enable = Enable;
}


void Read_GPIO_IRQSTATUS(void)
{
	Read_GPIO0_IRQSTATUS_0();
	Read_GPIO0_IRQSTATUS_1();
	Read_GPIO1_IRQSTATUS_0();
	Read_GPIO1_IRQSTATUS_1();
}
void Read_GPIO0_IRQSTATUS_0(void)
{
	GPIO0_IRQSTATUS_0_Register.all = *GPIO0_IRQSTATUS_0_REG;
}
void Read_GPIO0_IRQSTATUS_1(void)
{
	GPIO0_IRQSTATUS_1_Register.all = *GPIO0_IRQSTATUS_1_REG;
}
void Read_GPIO1_IRQSTATUS_0(void)
{
	GPIO1_IRQSTATUS_0_Register.all = *GPIO1_IRQSTATUS_0_REG;
}
void Read_GPIO1_IRQSTATUS_1(void)
{
	GPIO1_IRQSTATUS_1_Register.all = *GPIO1_IRQSTATUS_1_REG;
}

void Read_GPIO_IRQSTATUS_RAW(void)
{
	Read_GPIO0_IRQSTATUS_RAW_0();
	Read_GPIO0_IRQSTATUS_RAW_1();
	Read_GPIO1_IRQSTATUS_RAW_0();
	Read_GPIO1_IRQSTATUS_RAW_1();
}
void Read_GPIO0_IRQSTATUS_RAW_0(void)
{
	GPIO0_IRQSTATUS_RAW_0_Register.all = *GPIO0_IRQSTATUS_RAW_0_REG;
}
void Read_GPIO0_IRQSTATUS_RAW_1(void)
{
	GPIO0_IRQSTATUS_RAW_1_Register.all = *GPIO0_IRQSTATUS_RAW_1_REG;
}
void Read_GPIO1_IRQSTATUS_RAW_0(void)
{
	GPIO1_IRQSTATUS_RAW_0_Register.all = *GPIO1_IRQSTATUS_RAW_0_REG;
}
void Read_GPIO1_IRQSTATUS_RAW_1(void)
{
	GPIO1_IRQSTATUS_RAW_1_Register.all = *GPIO1_IRQSTATUS_RAW_1_REG;
}



void CPLD_EncoderResetAllAxis(void)
{
	CPLD_EncoderResetAxis1();
	CPLD_EncoderResetAxis2();
	CPLD_EncoderResetAxis3();
	CPLD_EncoderResetAxis4();
	CPLD_EncoderResetAxis5();
	CPLD_EncoderResetAxis6();
	CPLD_EncoderResetAxis7();
	CPLD_EncoderResetAxis8();
	CPLD_EncoderResetAxis9();
	CPLD_EncoderResetAxis10();
	CPLD_EncoderResetAxis11();
	CPLD_EncoderResetAxis12();
	CPLD_EncoderResetAxis13();
	CPLD_EncoderResetAxis14();
	CPLD_EncoderResetAxis15();
	CPLD_EncoderResetAxis16();
	CPLD_EncoderResetAxis17();
	CPLD_EncoderResetAxis18();
	CPLD_EncoderResetAxis19();
	CPLD_EncoderResetAxis20();
	CPLD_EncoderResetAxis21();
	CPLD_EncoderResetAxis22();
	CPLD_EncoderResetAxis23();
}

void CPLD_EncoderResetAxis1(void)	
{
	Encoder_Axis1.OldMACHValue_Low = 0;
	Encoder_Axis1.OldMACHValue_High = 0;
	Encoder_Axis1.NowMACHValue_Low = 0;
	Encoder_Axis1.NowMACHValue_High = 0;
	Encoder_Axis1.OldEncoderValue = 0;
	Encoder_Axis1.NowEncoderValue = 0;
	Encoder_Axis1.MACH_PositionValue = 0;		
	Encoder_Axis1.MACH_PositionValueout = 0;
	Encoder_Axis1.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis1 = 0;
	
	Encoder_Axis1.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis1_Low;
	Encoder_Axis1.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis1_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_1= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_1 = 1;
}

void CPLD_EncoderResetAxis2(void)	
{
	Encoder_Axis2.OldMACHValue_Low = 0;
	Encoder_Axis2.OldMACHValue_High = 0;
	Encoder_Axis2.NowMACHValue_Low = 0;
	Encoder_Axis2.NowMACHValue_High = 0;
	Encoder_Axis2.OldEncoderValue = 0;
	Encoder_Axis2.NowEncoderValue = 0;
	Encoder_Axis2.MACH_PositionValue = 0;		
	Encoder_Axis2.MACH_PositionValueout = 0;
	Encoder_Axis2.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis2 = 0;

	Encoder_Axis2.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis2_Low;
	Encoder_Axis2.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis2_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_2= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_2 = 1;
}



void CPLD_EncoderResetAxis3(void)	
{
	Encoder_Axis3.OldMACHValue_Low = 0;
	Encoder_Axis3.OldMACHValue_High = 0;
	Encoder_Axis3.NowMACHValue_Low = 0;
	Encoder_Axis3.NowMACHValue_High = 0;
	Encoder_Axis3.OldEncoderValue = 0;
	Encoder_Axis3.NowEncoderValue = 0;
	Encoder_Axis3.MACH_PositionValue = 0;		
	Encoder_Axis3.MACH_PositionValueout = 0;
	Encoder_Axis3.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis3 = 0;

	Encoder_Axis3.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis3_Low;
	Encoder_Axis3.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis3_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_3= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_3 = 1;
}


void CPLD_EncoderResetAxis4(void)	
{
	Encoder_Axis4.OldMACHValue_Low = 0;
	Encoder_Axis4.OldMACHValue_High = 0;
	Encoder_Axis4.NowMACHValue_Low = 0;
	Encoder_Axis4.NowMACHValue_High = 0;
	Encoder_Axis4.OldEncoderValue = 0;
	Encoder_Axis4.NowEncoderValue = 0;
	Encoder_Axis4.MACH_PositionValue = 0;		
	Encoder_Axis4.MACH_PositionValueout = 0;
	Encoder_Axis4.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis4 = 0;

	Encoder_Axis4.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis4_Low;
	Encoder_Axis4.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis4_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_4= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_4 = 1;
}


void CPLD_EncoderResetAxis5(void)	
{
	Encoder_Axis5.OldMACHValue_Low = 0;
	Encoder_Axis5.OldMACHValue_High = 0;
	Encoder_Axis5.NowMACHValue_Low = 0;
	Encoder_Axis5.NowMACHValue_High = 0;
	Encoder_Axis5.OldEncoderValue = 0;
	Encoder_Axis5.NowEncoderValue = 0;
	Encoder_Axis5.MACH_PositionValue = 0;		
	Encoder_Axis5.MACH_PositionValueout = 0;
	Encoder_Axis5.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis5 = 0;

	Encoder_Axis5.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis5_Low;
	Encoder_Axis5.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis5_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_5= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_5 = 1;
}


void CPLD_EncoderResetAxis6(void)	
{
	Encoder_Axis6.OldMACHValue_Low = 0;
	Encoder_Axis6.OldMACHValue_High = 0;
	Encoder_Axis6.NowMACHValue_Low = 0;
	Encoder_Axis6.NowMACHValue_High = 0;
	Encoder_Axis6.OldEncoderValue = 0;
	Encoder_Axis6.NowEncoderValue = 0;
	Encoder_Axis6.MACH_PositionValue = 0;		
	Encoder_Axis6.MACH_PositionValueout = 0;
	Encoder_Axis6.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis6 = 0;

	Encoder_Axis6.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis6_Low;
	Encoder_Axis6.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis6_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_6= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_6 = 1;
}


void CPLD_EncoderResetAxis7(void)	
{
	Encoder_Axis7.OldMACHValue_Low = 0;
	Encoder_Axis7.OldMACHValue_High = 0;
	Encoder_Axis7.NowMACHValue_Low = 0;
	Encoder_Axis7.NowMACHValue_High = 0;
	Encoder_Axis7.OldEncoderValue = 0;
	Encoder_Axis7.NowEncoderValue = 0;
	Encoder_Axis7.MACH_PositionValue = 0;		
	Encoder_Axis7.MACH_PositionValueout = 0;
	Encoder_Axis7.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis7 = 0;

	Encoder_Axis7.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis7_Low;
	Encoder_Axis7.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis7_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_7= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_7 = 1;
}


void CPLD_EncoderResetAxis8(void)	
{
	Encoder_Axis8.OldMACHValue_Low = 0;
	Encoder_Axis8.OldMACHValue_High = 0;
	Encoder_Axis8.NowMACHValue_Low = 0;
	Encoder_Axis8.NowMACHValue_High = 0;
	Encoder_Axis8.OldEncoderValue = 0;
	Encoder_Axis8.NowEncoderValue = 0;
	Encoder_Axis8.MACH_PositionValue = 0;		
	Encoder_Axis8.MACH_PositionValueout = 0;
	Encoder_Axis8.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis8 = 0;

	Encoder_Axis8.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis8_Low;
	Encoder_Axis8.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis8_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_8= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_8 = 1;
}


void CPLD_EncoderResetAxis9(void)	
{
	Encoder_Axis9.OldMACHValue_Low = 0;
	Encoder_Axis9.OldMACHValue_High = 0;
	Encoder_Axis9.NowMACHValue_Low = 0;
	Encoder_Axis9.NowMACHValue_High = 0;
	Encoder_Axis9.OldEncoderValue = 0;
	Encoder_Axis9.NowEncoderValue = 0;
	Encoder_Axis9.MACH_PositionValue = 0;		
	Encoder_Axis9.MACH_PositionValueout = 0;
	Encoder_Axis9.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis9 = 0;

	Encoder_Axis9.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis9_Low;
	Encoder_Axis9.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis9_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_9= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_9 = 1;
}


void CPLD_EncoderResetAxis10(void)	
{
	Encoder_Axis10.OldMACHValue_Low = 0;
	Encoder_Axis10.OldMACHValue_High = 0;
	Encoder_Axis10.NowMACHValue_Low = 0;
	Encoder_Axis10.NowMACHValue_High = 0;
	Encoder_Axis10.OldEncoderValue = 0;
	Encoder_Axis10.NowEncoderValue = 0;
	Encoder_Axis10.MACH_PositionValue = 0;		
	Encoder_Axis10.MACH_PositionValueout = 0;
	Encoder_Axis10.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis10 = 0;

	Encoder_Axis10.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis10_Low;
	Encoder_Axis10.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis10_High;

	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_10= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_10 = 1;
}


void CPLD_EncoderResetAxis11(void)	
{
	Encoder_Axis11.OldMACHValue_Low = 0;
	Encoder_Axis11.OldMACHValue_High = 0;
	Encoder_Axis11.NowMACHValue_Low = 0;
	Encoder_Axis11.NowMACHValue_High = 0;
	Encoder_Axis11.OldEncoderValue = 0;
	Encoder_Axis11.NowEncoderValue = 0;
	Encoder_Axis11.MACH_PositionValue = 0;		
	Encoder_Axis11.MACH_PositionValueout = 0;
	Encoder_Axis11.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis11 = 0;

	Encoder_Axis11.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis11_Low;
	Encoder_Axis11.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis11_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_11= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_11 = 1;
}


void CPLD_EncoderResetAxis12(void)	
{
	Encoder_Axis12.OldMACHValue_Low = 0;
	Encoder_Axis12.OldMACHValue_High = 0;
	Encoder_Axis12.NowMACHValue_Low = 0;
	Encoder_Axis12.NowMACHValue_High = 0;
	Encoder_Axis12.OldEncoderValue = 0;
	Encoder_Axis12.NowEncoderValue = 0;
	Encoder_Axis12.MACH_PositionValue = 0;		
	Encoder_Axis12.MACH_PositionValueout = 0;
	Encoder_Axis12.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis12 = 0;

	Encoder_Axis12.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis12_Low;
	Encoder_Axis12.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis12_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_12= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_12 = 1;
}


void CPLD_EncoderResetAxis13(void)	
{
	Encoder_Axis13.OldMACHValue_Low = 0;
	Encoder_Axis13.OldMACHValue_High = 0;
	Encoder_Axis13.NowMACHValue_Low = 0;
	Encoder_Axis13.NowMACHValue_High = 0;
	Encoder_Axis13.OldEncoderValue = 0;
	Encoder_Axis13.NowEncoderValue = 0;
	Encoder_Axis13.MACH_PositionValue = 0;		
	Encoder_Axis13.MACH_PositionValueout = 0;
	Encoder_Axis13.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis13 = 0;

	Encoder_Axis13.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis13_Low;
	Encoder_Axis13.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis13_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_13= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_13 = 1;
}


void CPLD_EncoderResetAxis14(void)	
{
	Encoder_Axis14.OldMACHValue_Low = 0;
	Encoder_Axis14.OldMACHValue_High = 0;
	Encoder_Axis14.NowMACHValue_Low = 0;
	Encoder_Axis14.NowMACHValue_High = 0;
	Encoder_Axis14.OldEncoderValue = 0;
	Encoder_Axis14.NowEncoderValue = 0;
	Encoder_Axis14.MACH_PositionValue = 0;		
	Encoder_Axis14.MACH_PositionValueout = 0;
	Encoder_Axis14.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis14 = 0;

	Encoder_Axis14.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis14_Low;
	Encoder_Axis14.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis14_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_14= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_14 = 1;
}


void CPLD_EncoderResetAxis15(void)	
{
	Encoder_Axis15.OldMACHValue_Low = 0;
	Encoder_Axis15.OldMACHValue_High = 0;
	Encoder_Axis15.NowMACHValue_Low = 0;
	Encoder_Axis15.NowMACHValue_High = 0;
	Encoder_Axis15.OldEncoderValue = 0;
	Encoder_Axis15.NowEncoderValue = 0;
	Encoder_Axis15.MACH_PositionValue = 0;		
	Encoder_Axis15.MACH_PositionValueout = 0;
	//Encoder_Axis15.Resolution = 0.1;
	//Encoder_Axis15.Resolution = 1.0;
	Encoder_Axis15.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis15 = 0;

	Encoder_Axis15.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis15_Low;
	Encoder_Axis15.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis15_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_15= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_15 = 1;
}


void CPLD_EncoderResetAxis16(void)	
{
	Encoder_Axis16.OldMACHValue_Low = 0;
	Encoder_Axis16.OldMACHValue_High = 0;
	Encoder_Axis16.NowMACHValue_Low = 0;
	Encoder_Axis16.NowMACHValue_High = 0;
	Encoder_Axis16.OldEncoderValue = 0;
	Encoder_Axis16.NowEncoderValue = 0;
	Encoder_Axis16.MACH_PositionValue = 0;		
	Encoder_Axis16.MACH_PositionValueout = 0;
	//Encoder_Axis16.Resolution = 0.1;
	//Encoder_Axis16.Resolution = 1.0;
	Encoder_Axis16.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis16 = 0;

	Encoder_Axis16.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis16_Low;
	Encoder_Axis16.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis16_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_16= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_16 = 1;
}


void CPLD_EncoderResetAxis17(void)	
{
	Encoder_Axis17.OldMACHValue_Low = 0;
	Encoder_Axis17.OldMACHValue_High = 0;
	Encoder_Axis17.NowMACHValue_Low = 0;
	Encoder_Axis17.NowMACHValue_High = 0;
	Encoder_Axis17.OldEncoderValue = 0;
	Encoder_Axis17.NowEncoderValue = 0;
	Encoder_Axis17.MACH_PositionValue = 0;		
	Encoder_Axis17.MACH_PositionValueout = 0;
	//Encoder_Axis17.Resolution = 0.1;
	//Encoder_Axis17.Resolution = 1.0;
	Encoder_Axis17.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis17 = 0;

	Encoder_Axis17.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis17_Low;
	Encoder_Axis17.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis17_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_17= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_17 = 1;
}


void CPLD_EncoderResetAxis18(void)	
{
	Encoder_Axis18.OldMACHValue_Low = 0;
	Encoder_Axis18.OldMACHValue_High = 0;
	Encoder_Axis18.NowMACHValue_Low = 0;
	Encoder_Axis18.NowMACHValue_High = 0;
	Encoder_Axis18.OldEncoderValue = 0;
	Encoder_Axis18.NowEncoderValue = 0;
	Encoder_Axis18.MACH_PositionValue = 0;		
	Encoder_Axis18.MACH_PositionValueout = 0;
	//Encoder_Axis18.Resolution = 0.1;
	//Encoder_Axis18.Resolution = 1.0;
	Encoder_Axis18.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis18 = 0;

	Encoder_Axis18.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis18_Low;
	Encoder_Axis18.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis18_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_18= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_18 = 1;
}


void CPLD_EncoderResetAxis19(void)	
{
	Encoder_Axis19.OldMACHValue_Low = 0;
	Encoder_Axis19.OldMACHValue_High = 0;
	Encoder_Axis19.NowMACHValue_Low = 0;
	Encoder_Axis19.NowMACHValue_High = 0;
	Encoder_Axis19.OldEncoderValue = 0;
	Encoder_Axis19.NowEncoderValue = 0;
	Encoder_Axis19.MACH_PositionValue = 0;		
	Encoder_Axis19.MACH_PositionValueout = 0;
	//Encoder_Axis19.Resolution = 0.1;
	//Encoder_Axis19.Resolution = 1.0;
	//Encoder_Axis19.Resolution = RESOLUTION_LINEARMOTOR;
	Encoder_Axis19.Resolution = 0.5;
	psDSPPacketOut->ABSMachineCoordinateAxis19 = 0;//for test
	OverallSign.AbsoluteEncoderValue_PSO.Axis19 = 0;

	Encoder_Axis19.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis19_Low;
	Encoder_Axis19.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis19_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_19= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_19 = 1;
}


void CPLD_EncoderResetAxis20(void)	
{
	Encoder_Axis20.OldMACHValue_Low = 0;
	Encoder_Axis20.OldMACHValue_High = 0;
	Encoder_Axis20.NowMACHValue_Low = 0;
	Encoder_Axis20.NowMACHValue_High = 0;
	Encoder_Axis20.OldEncoderValue = 0;
	Encoder_Axis20.NowEncoderValue = 0;
	Encoder_Axis20.MACH_PositionValue = 0;		
	Encoder_Axis20.MACH_PositionValueout = 0;
	//Encoder_Axis20.Resolution = 0.1;
	//Encoder_Axis20.Resolution = 1.0;
	Encoder_Axis20.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis20 = 0;

	Encoder_Axis20.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis20_Low;
	Encoder_Axis20.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis20_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_20= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_20 = 1;
}


void CPLD_EncoderResetAxis21(void)	
{
	Encoder_Axis21.OldMACHValue_Low = 0;
	Encoder_Axis21.OldMACHValue_High = 0;
	Encoder_Axis21.NowMACHValue_Low = 0;
	Encoder_Axis21.NowMACHValue_High = 0;
	Encoder_Axis21.OldEncoderValue = 0;
	Encoder_Axis21.NowEncoderValue = 0;
	Encoder_Axis21.MACH_PositionValue = 0;		
	Encoder_Axis21.MACH_PositionValueout = 0;
	//Encoder_Axis21.Resolution = 0.1;
	//Encoder_Axis21.Resolution = 1.0;
	Encoder_Axis21.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis21 = 0;

	Encoder_Axis21.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis21_Low;
	Encoder_Axis21.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis21_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_21= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_21 = 1;
}


void CPLD_EncoderResetAxis22(void)	
{
	Encoder_Axis22.OldMACHValue_Low = 0;
	Encoder_Axis22.OldMACHValue_High = 0;
	Encoder_Axis22.NowMACHValue_Low = 0;
	Encoder_Axis22.NowMACHValue_High = 0;
	Encoder_Axis22.OldEncoderValue = 0;
	Encoder_Axis22.NowEncoderValue = 0;
	Encoder_Axis22.MACH_PositionValue = 0;		
	Encoder_Axis22.MACH_PositionValueout = 0;
	//Encoder_Axis22.Resolution = 0.1;
	//Encoder_Axis22.Resolution = 1.0;
	Encoder_Axis22.Resolution = RESOLUTION_LINEARMOTOR;
	OverallSign.AbsoluteEncoderValue_PSO.Axis22 = 0;

	Encoder_Axis22.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis22_Low;
	Encoder_Axis22.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis22_High;
	
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_22= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear2_Out();
	Servo_Encoder_Clear2_Out_Register.bit.ENCODER_CLEAR_22 = 1;
}


void CPLD_EncoderResetAxis23(void)	
{
	Encoder_Axis23.OldMACHValue_Low = 0;
	Encoder_Axis23.OldMACHValue_High = 0;
	Encoder_Axis23.NowMACHValue_Low = 0;
	Encoder_Axis23.NowMACHValue_High = 0;
	Encoder_Axis23.OldEncoderValue = 0;
	Encoder_Axis23.NowEncoderValue = 0;
	Encoder_Axis23.MACH_PositionValue = 0;		
	Encoder_Axis23.MACH_PositionValueout = 0;
	//Encoder_Axis23.Resolution = 0.1;
	Encoder_Axis23.Resolution = 1.0;
	OverallSign.AbsoluteEncoderValue_PSO.Axis23 = 0;

	Encoder_Axis23.LPM_CounterPort_Low = (INT16U *) mCPLDADDR_ENCODER_Axis23_Low;
	Encoder_Axis23.LPM_CounterPort_High = (INT16U *) mCPLDADDR_ENCODER_Axis23_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_23= 0;  //Ö»ÐèÒªÐ´Ò»´Î0£¬FPGA¼´¸´Î»
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.ENCODER_CLEAR_23 = 1;
}



void CPLD_EncoderJudge(void)		
{
	if(Error.MainErrorSign)
		return ;
}

//added 20170407 for PMT counter
void CPLD_ResetPMTAll(void)
{
	CPLD_ResetPMT1();
	CPLD_ResetPMT2();
}
void CPLD_ResetPMT1(void)
{
	Pmt_Counter1.OldValue = 0;
	Pmt_Counter1.NowValue = 0;

	Pmt_Counter1.OldValue_G114 = 0;
	Pmt_Counter1.NowValue_G114 = 0;
	Pmt_Counter1.AverageValue_G114 = 0;

	Pmt_Counter1.Resolution = 1.0;

	Pmt_Counter1.PMT_CounterPort_Low = (INT16U *) mCPLDADDR_READ_PMT1_Low;
	Pmt_Counter1.PMT_CounterPort_High = (INT16U *) mCPLDADDR_READ_PMT1_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_1= 0; 
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_1 = 1;
}

void CPLD_ResetPMT2(void)
{
	Pmt_Counter2.OldValue = 0;
	Pmt_Counter2.NowValue = 0;

	Pmt_Counter2.OldValue_G114 = 0;
	Pmt_Counter2.NowValue_G114 = 0;
	Pmt_Counter2.AverageValue_G114 = 0;
	
	Pmt_Counter2.Resolution = 1.0;

	Pmt_Counter2.PMT_CounterPort_Low = (INT16U *) mCPLDADDR_READ_PMT2_Low;
	Pmt_Counter2.PMT_CounterPort_High = (INT16U *) mCPLDADDR_READ_PMT2_High;
	
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_2= 0; 
	Write_Servo_Encoder_Clear1_Out();
	Servo_Encoder_Clear1_Out_Register.bit.PMT_CLEAR_2 = 1;
}



void CPLD_ServoInit(void)
{
	Servo_Axis1.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis1.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis1.Pn210 = 5000;//pitch
	Servo_Axis1.Pn212 = 5000;
	Servo_Axis1.PWM_Port = PWM_Axis1;
	Servo_Axis1.LPM_CounterPort = LPM_Axis1;	
	Servo_Axis1.PWM_PeriodRegister = 1000;
	*(Servo_Axis1.PWM_Port) = Servo_Axis1.PWM_PeriodRegister;
	*(Servo_Axis1.LPM_CounterPort) = 0;
	Servo_Axis1.AxisOutUnitEQU = 1.0;

	Servo_Axis2.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis2.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis2.Pn210 = 5000;//pitch
	Servo_Axis2.Pn212 = 5000;
	//Servo_Axis2.Pn210 = 50000;//pitch
	//Servo_Axis2.Pn212 = 50000;
	Servo_Axis2.PWM_Port = (INT16U *)PWM_Axis2;
	Servo_Axis2.LPM_CounterPort = (INT16U *)LPM_Axis2;	
	Servo_Axis2.PWM_PeriodRegister = 1000;
	*(Servo_Axis2.PWM_Port) = Servo_Axis2.PWM_PeriodRegister;
	*(Servo_Axis2.LPM_CounterPort) = 0;	
	Servo_Axis2.AxisOutUnitEQU = 1.0;
	//Servo_Axis2.AxisOutUnitEQU = 10.0;

	Servo_Axis3.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis3.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis3.Pn210 = 1000;//pitch
	Servo_Axis3.Pn212 = 1000;
	Servo_Axis3.PWM_Port = (INT16U *)PWM_Axis3;
	Servo_Axis3.LPM_CounterPort = (INT16U *)LPM_Axis3;	
	Servo_Axis3.PWM_PeriodRegister = 1000;
	*(Servo_Axis3.PWM_Port) = Servo_Axis3.PWM_PeriodRegister;
	*(Servo_Axis3.LPM_CounterPort) = 0;
	Servo_Axis3.AxisOutUnitEQU = 1.0;

	Servo_Axis4.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis4.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis4.Pn210 = 1000;//pitch
	Servo_Axis4.Pn212 = 1000;
	Servo_Axis4.PWM_Port = (INT16U *)PWM_Axis4;
	Servo_Axis4.LPM_CounterPort = (INT16U *)LPM_Axis4;	
	Servo_Axis4.PWM_PeriodRegister = 1000;
	*(Servo_Axis4.PWM_Port) = Servo_Axis4.PWM_PeriodRegister;
	*(Servo_Axis4.LPM_CounterPort) = 0;
	Servo_Axis4.AxisOutUnitEQU = 1.0;

	Servo_Axis5.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis5.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis5.Pn210 = 5000;//pitch
	Servo_Axis5.Pn212 = 5000;
	Servo_Axis5.PWM_Port = (INT16U *)PWM_Axis5;
	Servo_Axis5.LPM_CounterPort = (INT16U *)LPM_Axis5;	
	Servo_Axis5.PWM_PeriodRegister = 1000;
	*(Servo_Axis5.PWM_Port) = Servo_Axis5.PWM_PeriodRegister;
	*(Servo_Axis5.LPM_CounterPort) = 0;
	Servo_Axis5.AxisOutUnitEQU = 1.0;

	Servo_Axis6.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis6.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis6.Pn210 = 10000;//pitch
	Servo_Axis6.Pn212 = 10000;
	Servo_Axis6.PWM_Port = (INT16U *)PWM_Axis6;
	Servo_Axis6.LPM_CounterPort = (INT16U *)LPM_Axis6;	
	Servo_Axis6.PWM_PeriodRegister = 1000;
	*(Servo_Axis6.PWM_Port) = Servo_Axis6.PWM_PeriodRegister;
	*(Servo_Axis6.LPM_CounterPort) = 0;
	Servo_Axis6.AxisOutUnitEQU = 1.0;

	Servo_Axis7.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis7.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis7.Pn210 = 80000;//pitch
	Servo_Axis7.Pn212 = 80000;
	Servo_Axis7.PWM_Port = (INT16U *)PWM_Axis7;
	Servo_Axis7.LPM_CounterPort = (INT16U *)LPM_Axis7;	
	Servo_Axis7.PWM_PeriodRegister = 1000;
	*(Servo_Axis7.PWM_Port) = Servo_Axis7.PWM_PeriodRegister;
	*(Servo_Axis7.LPM_CounterPort) = 0;
	Servo_Axis7.AxisOutUnitEQU = 1.0;

	Servo_Axis8.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis8.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis8.Pn210 = 5000;//pitch
	Servo_Axis8.Pn212 = 5000;
	Servo_Axis8.PWM_Port = (INT16U *)PWM_Axis8;
	Servo_Axis8.LPM_CounterPort = (INT16U *)LPM_Axis8;	
	Servo_Axis8.PWM_PeriodRegister = 1000;
	*(Servo_Axis8.PWM_Port) = Servo_Axis8.PWM_PeriodRegister;
	*(Servo_Axis8.LPM_CounterPort) = 0;
	Servo_Axis8.AxisOutUnitEQU = 1.0;

	Servo_Axis9.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis9.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis9.Pn210 = 10000;//pitch
	Servo_Axis9.Pn212 = 10000;
	Servo_Axis9.PWM_Port = (INT16U *)PWM_Axis9;
	Servo_Axis9.LPM_CounterPort = (INT16U *)LPM_Axis9;	
	Servo_Axis9.PWM_PeriodRegister = 1000;
	*(Servo_Axis9.PWM_Port) = Servo_Axis9.PWM_PeriodRegister;
	*(Servo_Axis9.LPM_CounterPort) = 0;
	Servo_Axis9.AxisOutUnitEQU = 1.0;
	
	Servo_Axis10.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis10.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis10.Pn210 = 5000;//pitch
	Servo_Axis10.Pn212 = 5000;
	Servo_Axis10.PWM_Port = (INT16U *)PWM_Axis10;
	Servo_Axis10.LPM_CounterPort = (INT16U *)LPM_Axis10;	
	Servo_Axis10.PWM_PeriodRegister = 1000;
	*(Servo_Axis10.PWM_Port) = Servo_Axis10.PWM_PeriodRegister;
	*(Servo_Axis10.LPM_CounterPort) = 0;
	Servo_Axis10.AxisOutUnitEQU = 1.0;

	Servo_Axis11.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis11.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis11.Pn210 = 5000;//pitch
	Servo_Axis11.Pn212 = 5000;
	Servo_Axis11.PWM_Port = PWM_Axis11;
	Servo_Axis11.LPM_CounterPort = LPM_Axis11;	
	Servo_Axis11.PWM_PeriodRegister = 1000;
	*(Servo_Axis11.PWM_Port) = Servo_Axis11.PWM_PeriodRegister;
	*(Servo_Axis11.LPM_CounterPort) = 0;
	Servo_Axis11.AxisOutUnitEQU = 1.0;

	Servo_Axis12.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis12.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis12.Pn210 = 80000;//pitch
	Servo_Axis12.Pn212 = 80000;
	Servo_Axis12.PWM_Port = (INT16U *)PWM_Axis12;
	Servo_Axis12.LPM_CounterPort = (INT16U *)LPM_Axis12;	
	Servo_Axis12.PWM_PeriodRegister = 1000;
	*(Servo_Axis12.PWM_Port) = Servo_Axis12.PWM_PeriodRegister;
	*(Servo_Axis12.LPM_CounterPort) = 0;	
	Servo_Axis12.AxisOutUnitEQU = 1.0;

	Servo_Axis13.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis13.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis13.Pn210 = 5000;//pitch
	Servo_Axis13.Pn212 = 5000;
	Servo_Axis13.PWM_Port = (INT16U *)PWM_Axis13;
	Servo_Axis13.LPM_CounterPort = (INT16U *)LPM_Axis13;	
	Servo_Axis13.PWM_PeriodRegister = 1000;
	*(Servo_Axis13.PWM_Port) = Servo_Axis13.PWM_PeriodRegister;
	*(Servo_Axis13.LPM_CounterPort) = 0;
	Servo_Axis13.AxisOutUnitEQU = 1.0;

	Servo_Axis14.EncoderTotalPulseSingleRotation = 0x1000000;//2^24 = 16777216;
	Servo_Axis14.Pn20E= 0x1000000;//2^24 = 16777216;
	Servo_Axis14.Pn210 = 5000;//pitch
	Servo_Axis14.Pn212 = 5000;
	Servo_Axis14.PWM_Port = (INT16U *)PWM_Axis14;
	Servo_Axis14.LPM_CounterPort = (INT16U *)LPM_Axis14;	
	Servo_Axis14.PWM_PeriodRegister = 1000;
	*(Servo_Axis14.PWM_Port) = Servo_Axis14.PWM_PeriodRegister;
	*(Servo_Axis14.LPM_CounterPort) = 0;
	Servo_Axis14.AxisOutUnitEQU = 1.0;

	Servo_Axis15.PWM_Port = (INT16U *)PWM_Axis15;
	Servo_Axis15.LPM_CounterPort = (INT16U *)LPM_Axis15;	
	Servo_Axis15.PWM_PeriodRegister = 1000;
	*(Servo_Axis15.PWM_Port) = Servo_Axis15.PWM_PeriodRegister;
	*(Servo_Axis15.LPM_CounterPort) = 0;
	Servo_Axis15.AxisOutUnitEQU = 1.0;
	//Servo_Axis15.AxisOutUnitEQU = 10.0;

	Servo_Axis16.PWM_Port = (INT16U *)PWM_Axis16;
	Servo_Axis16.LPM_CounterPort = (INT16U *)LPM_Axis16;	
	Servo_Axis16.PWM_PeriodRegister = 1000;
	*(Servo_Axis16.PWM_Port) = Servo_Axis16.PWM_PeriodRegister;
	*(Servo_Axis16.LPM_CounterPort) = 0;
	Servo_Axis16.AxisOutUnitEQU = 1.0;
	//Servo_Axis16.AxisOutUnitEQU = 10.0;

	Servo_Axis17.PWM_Port = (INT16U *)PWM_Axis17;
	Servo_Axis17.LPM_CounterPort = (INT16U *)LPM_Axis17;	
	Servo_Axis17.PWM_PeriodRegister = 1000;
	*(Servo_Axis17.PWM_Port) = Servo_Axis17.PWM_PeriodRegister;
	*(Servo_Axis17.LPM_CounterPort) = 0;
	Servo_Axis17.AxisOutUnitEQU = 1.0;
	//Servo_Axis17.AxisOutUnitEQU = 10.0;

	Servo_Axis18.PWM_Port = (INT16U *)PWM_Axis18;
	Servo_Axis18.LPM_CounterPort = (INT16U *)LPM_Axis18;	
	Servo_Axis18.PWM_PeriodRegister = 1000;
	*(Servo_Axis18.PWM_Port) = Servo_Axis18.PWM_PeriodRegister;
	*(Servo_Axis18.LPM_CounterPort) = 0;
	Servo_Axis18.AxisOutUnitEQU = 1.0;
	//Servo_Axis18.AxisOutUnitEQU = 10.0;

	Servo_Axis19.PWM_Port = (INT16U *)PWM_Axis19;
	Servo_Axis19.LPM_CounterPort = (INT16U *)LPM_Axis19;	
	Servo_Axis19.PWM_PeriodRegister = 1000;
	*(Servo_Axis19.PWM_Port) = Servo_Axis19.PWM_PeriodRegister;
	*(Servo_Axis19.LPM_CounterPort) = 0;
	//Servo_Axis19.AxisOutUnitEQU = 1.0;
	//Servo_Axis19.AxisOutUnitEQU = 10.0;	
	//Servo_Axis19.AxisOutUnitEQU = 20.0;//for MSTEP=10000;SW5=OFF,SW6=ON,SW7=OFF,SW8=OFF.
	Servo_Axis19.AxisOutUnitEQU = 2.0;//for MSTEP=1000;SW5=ON,SW6=ON,SW7=ON,SW8=OFF.

	Servo_Axis20.PWM_Port = (INT16U *)PWM_Axis20;
	Servo_Axis20.LPM_CounterPort = (INT16U *)LPM_Axis20;	
	Servo_Axis20.PWM_PeriodRegister = 1000;
	*(Servo_Axis20.PWM_Port) = Servo_Axis20.PWM_PeriodRegister;
	*(Servo_Axis20.LPM_CounterPort) = 0;
	Servo_Axis20.AxisOutUnitEQU = 1.0;
	//Servo_Axis20.AxisOutUnitEQU = 10.0;
	
	Servo_Axis21.PWM_Port = PWM_Axis21;
	Servo_Axis21.LPM_CounterPort = LPM_Axis21;	
	Servo_Axis21.PWM_PeriodRegister = 1000;
	*(Servo_Axis21.PWM_Port) = Servo_Axis21.PWM_PeriodRegister;
	*(Servo_Axis21.LPM_CounterPort) = 0;
	Servo_Axis21.AxisOutUnitEQU = 1.0;
	//Servo_Axis21.AxisOutUnitEQU = 10.0;

	Servo_Axis22.PWM_Port = (INT16U *)PWM_Axis22;
	Servo_Axis22.LPM_CounterPort = (INT16U *)LPM_Axis22;	
	Servo_Axis22.PWM_PeriodRegister = 1000;
	*(Servo_Axis22.PWM_Port) = Servo_Axis22.PWM_PeriodRegister;
	*(Servo_Axis22.LPM_CounterPort) = 0;	
	Servo_Axis22.AxisOutUnitEQU = 1.0;
	//Servo_Axis22.AxisOutUnitEQU = 10.0;

	Servo_Axis23.PWM_Port = (INT16U *)PWM_Axis23;
	Servo_Axis23.LPM_CounterPort = (INT16U *)LPM_Axis23;	
	Servo_Axis23.PWM_PeriodRegister = 1000;
	*(Servo_Axis23.PWM_Port) = Servo_Axis23.PWM_PeriodRegister;
	*(Servo_Axis23.LPM_CounterPort) = 0;
	Servo_Axis23.AxisOutUnitEQU = 1.0;	
	//Servo_Axis23.AxisOutUnitEQU = 10.0;
}

void CPLD_ServoOff(void)
{
	//Servo_On1_Out_Register.all = 0xFFFF;
	//0x02=R2;//always on 0xFE1D
	//Servo_On1_Out_Register.all = 0xFE1F;
	Servo_On1_Out_Register.all = 0xFE1D;
	Write_Servo_On1_Out();
	//Servo_On2_Out_Register.all = 0xFFFF;

	//Servo_On2_Out_Register.all = 0xFE1F;
	//0x10=R10 Motor //always on FE0F
	Servo_On2_Out_Register.all = 0xFE0F;
	//0x19=L18_Slave//always on 0xFE2F
	//Servo_On2_Out_Register.all = 0xFE2F;
	Write_Servo_On2_Out();
	ServoOn2Counter = 0;
}

void CPLD_ServoOn(void)
{
	CPLD_ServoOn1();
	CPLD_ServoOn2();
}

void CPLD_ServoOn1(void)
{
	//Servo_On1_Out_Register.all = ;
	//0x02=R2;
	Servo_On1_Out_Register.all = 0x1e0;//0x20=L15;0x40=L16;0x80=L17;0x100=L18
	Write_Servo_On1_Out();
}

void CPLD_ServoOn2(void)
{
	//Servo_On2_Out_Register.all = 0;
	//0x10=R10
	Servo_On2_Out_Register.all = 0x1e0;//0x20=L19;0x40=L20;0x80=L21;0x100=L22
	Write_Servo_On2_Out();
}


void CPLD_Servo_INRead(void)
{
	Read_Servo_Alarm1_In();
	Read_Servo_Alarm2_In();
	Read_Servo_Coin1_In();
	Read_Servo_Coin2_In();
	Read_Servo_HardLimit_Positive1_In();
	Read_Servo_HardLimit_Positive2_In();
	Read_Servo_HardLimit_Negative1_In();
	Read_Servo_HardLimit_Negative2_In();
}

void CPLD_Servo_OUTWrite(void)
{

}



void CPLD_GPIO_INRead(void)
{
	Read_GPIO_In1();
	Read_GPIO_In2();
	Read_GPIO_In3();
	Read_GPIO_In4();	
}
void CPLD_GPIO_OUTwrite(void)
{
	Write_GPIO_Out1();
	Write_GPIO_Out2();
}

void CPLD_GPIOinit(void)
{	
	GPIO_In1_Register.all=0;
	GPIO_In2_Register.all=0;
	GPIO_In3_Register.all=0;
	GPIO_In4_Register.all=0;
		
	GPIO_Out1_Register.all=0xFFFF;
	GPIO_Out2_Register.all=0xFFFF;
	
	CPLD_GPIO_INRead();
	CPLD_GPIO_OUTwrite();
}

void CPLD_GPIO_IRQStatusInit()
{
	GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0 = GPIO0_IRQSTATUS_RAW_0_REG;
	//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = 0;
	GPIO_IRQStatus.GPIO0_IRQStatus_RAW_1 = GPIO0_IRQSTATUS_RAW_1_REG;
	//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_1) = 0;
	
	GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0 = GPIO1_IRQSTATUS_RAW_0_REG;
	//*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0) = 0;
	GPIO_IRQStatus.GPIO1_IRQStatus_RAW_1 = GPIO1_IRQSTATUS_RAW_1_REG;
	//*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_1) = 0;

	GPIO_IRQStatus.GPIO0_IRQStatus_0 = GPIO0_IRQSTATUS_0_REG;
	//*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = 0;
	GPIO_IRQStatus.GPIO0_IRQStatus_1 = GPIO0_IRQSTATUS_1_REG;
	//*(GPIO_IRQStatus.GPIO0_IRQStatus_1) = 0;
	
	GPIO_IRQStatus.GPIO1_IRQStatus_0 = GPIO1_IRQSTATUS_0_REG;
	//*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = 0;
	GPIO_IRQStatus.GPIO1_IRQStatus_1 = GPIO1_IRQSTATUS_1_REG;
	//*(GPIO_IRQStatus.GPIO1_IRQStatus_1) = 0;
}


Uint16 pre_get_MACH_counter(ENCODER *Encoder)
{
	Int16	NowMACHValue_Low_1,NowMACHValue_Low_2;
	Int16	NowMACHValue_High_1,NowMACHValue_High_2;

	NowMACHValue_Low_1 = *Encoder->LPM_CounterPort_Low;
	NowMACHValue_Low_2 = *Encoder->LPM_CounterPort_Low;
	if(abs(NowMACHValue_Low_1-NowMACHValue_Low_2) > 10000)
	{
		NowMACHValue_Low_1 = *Encoder->LPM_CounterPort_Low;
		NowMACHValue_Low_2 = *Encoder->LPM_CounterPort_Low;
		if(abs(NowMACHValue_Low_1-NowMACHValue_Low_2) > 10000)
		{
			OverallSign.GetMachCoordinateRight = 0;
			return 0;
		}
	}

	NowMACHValue_High_1 = *Encoder->LPM_CounterPort_High;
	NowMACHValue_High_2 = *Encoder->LPM_CounterPort_High;

	if(abs(NowMACHValue_High_1-NowMACHValue_High_2) > 100)
	{
		NowMACHValue_High_1 = *Encoder->LPM_CounterPort_High;
		NowMACHValue_High_2 = *Encoder->LPM_CounterPort_High;
		if(abs(NowMACHValue_High_1-NowMACHValue_High_2) > 100)
		{
			OverallSign.GetMachCoordinateRight = 0;
			return 0;
		}
	}
	
	Encoder->NowMACHValue_Low = NowMACHValue_Low_2;
	Encoder->NowMACHValue_High = NowMACHValue_High_2;

	#if 0
	if((Encoder->OldMACHValue_Low>60000) && (Encoder->NowMACHValue_Low<10000))
	{

		Encoder->NowMACHValue_High++;
	}
	
	if((Encoder->OldMACHValue_Low<10000) && (Encoder->NowMACHValue_Low>60000))
	{
		Encoder->NowMACHValue_High --;
	}

	if(NowMACHValue_High_2 != Encoder->NowMACHValue_High)
	{
		Encoder->NowMACHValue_High = Encoder->NowMACHValue_High;
	}
	#endif

	 return 1;
}

Uint16 get_MACH_value(ENCODER *Encoder)
{
	Encoder->MACH_PositionValueout=((Encoder->NowMACHValue_Low+65536*Encoder->NowMACHValue_High)*Encoder->Resolution);
	Encoder->OldMACHValue_Low= Encoder->NowMACHValue_Low;
	Encoder->OldMACHValue_High= Encoder->NowMACHValue_High;
	Encoder->MACH_PositionValue =(Encoder->EncoderDIR)?((-1)*Encoder->MACH_PositionValueout):(Encoder->MACH_PositionValueout);	
}

void get_PMT_counter(PMT *Pmt)
{
	static Uint32 AverageNumber = 20;//10ms=500us*20
	float64 	AverageValue;
	Uint16	NowValue_Low_1,NowValue_Low_2;
	Uint16	NowValue_High_1,NowValue_High_2;

	AverageCounter++;
	if(AverageCounter >10000)
	{
		AverageCounter = 1;
	}
	
	if((AverageCounter%AverageNumber != 0))
	{
		return;
	}
	NowValue_Low_1 = *Pmt->PMT_CounterPort_Low;
	NowValue_Low_2 = *Pmt->PMT_CounterPort_Low;
	
	NowValue_High_1 = *Pmt->PMT_CounterPort_High;
	NowValue_High_2 = *Pmt->PMT_CounterPort_High;

	if(NowValue_High_2 == 0xFFFF)
	{	//for monitor
		Pmt->NowValue = Pmt->NowValue;
	}
	
	Pmt->NowValue =((NowValue_Low_2+NowValue_High_2 * 0x10000) * Pmt->Resolution);
	AverageValue = divdp((Pmt->NowValue - Pmt->OldValue)*2,AverageNumber);
	Pmt->OldValue = Pmt->NowValue;

	MonitorBuffer4[MonitorBuffer4Cnt] = AverageValue;

	MonitorBuffer4Cnt++;
	if(MonitorBuffer4Cnt > 1000)
	{
		MonitorBuffer4Cnt = 0;
	}

	 return 1;
}

void get_PMT_Value(void)
{
	get_PMT_counter(&Pmt_Counter1);
	//get_PMT_counter(&Pmt_Counter2);
}


void nonlinear_mach_value(INT40AXIS Axis_MACHValue_In)
{
	//��Ӧ��Out
	nonlinear_Axis1_mach_value(Axis_MACHValue_In.Axis1);	
	nonlinear_Axis2_mach_value(Axis_MACHValue_In.Axis2);
	nonlinear_Axis3_mach_value(Axis_MACHValue_In.Axis3);
	nonlinear_Axis4_mach_value(Axis_MACHValue_In.Axis4);
	nonlinear_Axis5_mach_value(Axis_MACHValue_In.Axis5);
	nonlinear_Axis6_mach_value(Axis_MACHValue_In.Axis6);
	nonlinear_Axis7_mach_value(Axis_MACHValue_In.Axis7);
	nonlinear_Axis8_mach_value(Axis_MACHValue_In.Axis8);
	nonlinear_Axis9_mach_value(Axis_MACHValue_In.Axis9);
	nonlinear_Axis10_mach_value(Axis_MACHValue_In.Axis10);
	nonlinear_Axis11_mach_value(Axis_MACHValue_In.Axis11);
	nonlinear_Axis12_mach_value(Axis_MACHValue_In.Axis12);
	nonlinear_Axis13_mach_value(Axis_MACHValue_In.Axis13);
	nonlinear_Axis14_mach_value(Axis_MACHValue_In.Axis14);
	nonlinear_Axis15_mach_value(Axis_MACHValue_In.Axis15);
	nonlinear_Axis16_mach_value(Axis_MACHValue_In.Axis16);
	nonlinear_Axis17_mach_value(Axis_MACHValue_In.Axis17);
	nonlinear_Axis18_mach_value(Axis_MACHValue_In.Axis18);
	nonlinear_Axis19_mach_value(Axis_MACHValue_In.Axis19);
	nonlinear_Axis20_mach_value(Axis_MACHValue_In.Axis20);
	nonlinear_Axis21_mach_value(Axis_MACHValue_In.Axis21);
	nonlinear_Axis22_mach_value(Axis_MACHValue_In.Axis22);
	nonlinear_Axis23_mach_value(Axis_MACHValue_In.Axis23);
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis1_mach_value(Int32 Axis1_MACHValue)
{
	Uint32 Axis1_ECPT;
	Int32   Axis1_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis1 <= 2)
	{
		return; 
	}
	
	Axis1_ECPT = OverallSign.EncoderCompPt.Axis1;	

	if(Axis1_ECPT <= 0)
	{
		Axis1_ECPT = 1;
	}
	else if(Axis1_ECPT >=(System.AxisCompMaxL.Axis1+1))
	{
		Axis1_ECPT = System.AxisCompMaxL.Axis1;
	}

	if(System.CompensationDirection.Axis1)
	{	//���򲹳�
		do
		{
			if(Axis1_MACHValue > AxisCompBuffer[Axis1_ECPT].Value_Linear.Axis1)
			{	
				Axis1_ECPT++;
			}
			else if(Axis1_MACHValue < AxisCompBuffer[Axis1_ECPT-1].Value_Linear.Axis1)
			{
				Axis1_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis1_ECPT!=0) && (Axis1_ECPT!=(System.AxisCompMaxL.Axis1+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis1_MACHValue < AxisCompBuffer[Axis1_ECPT].Value_Linear.Axis1)
			{	
				Axis1_ECPT++;
			}
			else if(Axis1_MACHValue > AxisCompBuffer[Axis1_ECPT-1].Value_Linear.Axis1)
			{
				Axis1_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis1_ECPT!=0) && (Axis1_ECPT!=(System.AxisCompMaxL.Axis1+1)));	
	}

	if(Axis1_ECPT == 0)
	{
		Axis1_CompValue = AxisCompBuffer[0].Value_Laser.Axis1+
			(Axis1_MACHValue-AxisCompBuffer[0].Value_Linear.Axis1);
	}
	else if(Axis1_ECPT == (System.AxisCompMaxL.Axis1+1))
	{
		Axis1_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis1].Value_Laser.Axis1+
			(Axis1_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis1].Value_Linear.Axis1);
	}
	else
	{
		Axis1_CompValue = AxisCompBuffer[Axis1_ECPT-1].Value_Laser.Axis1+
			(Axis1_MACHValue-AxisCompBuffer[Axis1_ECPT-1].Value_Linear.Axis1)*
			divdp( (AxisCompBuffer[Axis1_ECPT].Value_Laser.Axis1-AxisCompBuffer[Axis1_ECPT-1].Value_Laser.Axis1), 
			(AxisCompBuffer[Axis1_ECPT].Value_Linear.Axis1-AxisCompBuffer[Axis1_ECPT-1].Value_Linear.Axis1) );
	}		

	OverallSign.EncoderCompPt.Axis1 = Axis1_ECPT;
	
	OverallSign.ABS_Coordinate.Axis1 = Axis1_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis2_mach_value(Int32 Axis2_MACHValue)
{
	Uint32 Axis2_ECPT;
	Int32   Axis2_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis2 <= 2)
	{
		return; 
	}
	
	Axis2_ECPT = OverallSign.EncoderCompPt.Axis2;	

	if(Axis2_ECPT <= 0)
	{
		Axis2_ECPT = 1;
	}
	else if(Axis2_ECPT >=(System.AxisCompMaxL.Axis2+1))
	{
		Axis2_ECPT = System.AxisCompMaxL.Axis2;
	}

	if(System.CompensationDirection.Axis2)
	{	//���򲹳�
		do
		{
			if(Axis2_MACHValue > AxisCompBuffer[Axis2_ECPT].Value_Linear.Axis2)
			{	
				Axis2_ECPT++;
			}
			else if(Axis2_MACHValue < AxisCompBuffer[Axis2_ECPT-1].Value_Linear.Axis2)
			{
				Axis2_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis2_ECPT!=0) && (Axis2_ECPT!=(System.AxisCompMaxL.Axis2+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis2_MACHValue < AxisCompBuffer[Axis2_ECPT].Value_Linear.Axis2)
			{	
				Axis2_ECPT++;
			}
			else if(Axis2_MACHValue > AxisCompBuffer[Axis2_ECPT-1].Value_Linear.Axis2)
			{
				Axis2_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis2_ECPT!=0) && (Axis2_ECPT!=(System.AxisCompMaxL.Axis2+1)));	
	}

	if(Axis2_ECPT == 0)
	{
		Axis2_CompValue = AxisCompBuffer[0].Value_Laser.Axis2+
			(Axis2_MACHValue-AxisCompBuffer[0].Value_Linear.Axis2);
	}
	else if(Axis2_ECPT == (System.AxisCompMaxL.Axis2+1))
	{
		Axis2_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis2].Value_Laser.Axis2+
			(Axis2_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis2].Value_Linear.Axis2);
	}
	else
	{
		Axis2_CompValue = AxisCompBuffer[Axis2_ECPT-1].Value_Laser.Axis2+
			(Axis2_MACHValue-AxisCompBuffer[Axis2_ECPT-1].Value_Linear.Axis2)*
			divdp( (AxisCompBuffer[Axis2_ECPT].Value_Laser.Axis2-AxisCompBuffer[Axis2_ECPT-1].Value_Laser.Axis2), 
			(AxisCompBuffer[Axis2_ECPT].Value_Linear.Axis2-AxisCompBuffer[Axis2_ECPT-1].Value_Linear.Axis2) );
	}		

	OverallSign.EncoderCompPt.Axis2 = Axis2_ECPT;
	
	OverallSign.ABS_Coordinate.Axis2 = Axis2_CompValue;
	
}




//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis3_mach_value(Int32 Axis3_MACHValue)
{
	Uint32 Axis3_ECPT;
	Int32   Axis3_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis3 <= 2)
	{
		return; 
	}

	Axis3_ECPT = OverallSign.EncoderCompPt.Axis3;	

	if(Axis3_ECPT <= 0)
	{
		Axis3_ECPT = 1;
	}
	else if(Axis3_ECPT >=(System.AxisCompMaxL.Axis3+1))
	{
		Axis3_ECPT = System.AxisCompMaxL.Axis3;
	}

	if(System.CompensationDirection.Axis3)
	{	//���򲹳�
		do
		{
			if(Axis3_MACHValue > AxisCompBuffer[Axis3_ECPT].Value_Linear.Axis3)
			{	
				Axis3_ECPT++;
			}
			else if(Axis3_MACHValue < AxisCompBuffer[Axis3_ECPT-1].Value_Linear.Axis3)
			{
				Axis3_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis3_ECPT!=0) && (Axis3_ECPT!=(System.AxisCompMaxL.Axis3+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis3_MACHValue < AxisCompBuffer[Axis3_ECPT].Value_Linear.Axis3)
			{	
				Axis3_ECPT++;
			}
			else if(Axis3_MACHValue > AxisCompBuffer[Axis3_ECPT-1].Value_Linear.Axis3)
			{
				Axis3_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis3_ECPT!=0) && (Axis3_ECPT!=(System.AxisCompMaxL.Axis3+1)));	
	}

	if(Axis3_ECPT == 0)
	{
		Axis3_CompValue = AxisCompBuffer[0].Value_Laser.Axis3+
			(Axis3_MACHValue-AxisCompBuffer[0].Value_Linear.Axis3);
	}
	else if(Axis3_ECPT == (System.AxisCompMaxL.Axis3+1))
	{
		Axis3_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis3].Value_Laser.Axis3+
			(Axis3_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis3].Value_Linear.Axis3);
	}
	else
	{
		Axis3_CompValue = AxisCompBuffer[Axis3_ECPT-1].Value_Laser.Axis3+
			(Axis3_MACHValue-AxisCompBuffer[Axis3_ECPT-1].Value_Linear.Axis3)*
			divdp( (AxisCompBuffer[Axis3_ECPT].Value_Laser.Axis3-AxisCompBuffer[Axis3_ECPT-1].Value_Laser.Axis3), 
			(AxisCompBuffer[Axis3_ECPT].Value_Linear.Axis3-AxisCompBuffer[Axis3_ECPT-1].Value_Linear.Axis3) );
	}		

	OverallSign.EncoderCompPt.Axis3 = Axis3_ECPT;
	
	OverallSign.ABS_Coordinate.Axis3 = Axis3_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis4_mach_value(Int32 Axis4_MACHValue)
{
	Uint32 Axis4_ECPT;
	Int32   Axis4_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis4 <= 2)
	{
		return; 
	}

	Axis4_ECPT = OverallSign.EncoderCompPt.Axis4;	

	if(Axis4_ECPT <= 0)
	{
		Axis4_ECPT = 1;
	}
	else if(Axis4_ECPT >=(System.AxisCompMaxL.Axis4+1))
	{
		Axis4_ECPT = System.AxisCompMaxL.Axis4;
	}

	if(System.CompensationDirection.Axis4)
	{	//���򲹳�
		do
		{
			if(Axis4_MACHValue > AxisCompBuffer[Axis4_ECPT].Value_Linear.Axis4)
			{	
				Axis4_ECPT++;
			}
			else if(Axis4_MACHValue < AxisCompBuffer[Axis4_ECPT-1].Value_Linear.Axis4)
			{
				Axis4_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis4_ECPT!=0) && (Axis4_ECPT!=(System.AxisCompMaxL.Axis4+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis4_MACHValue < AxisCompBuffer[Axis4_ECPT].Value_Linear.Axis4)
			{	
				Axis4_ECPT++;
			}
			else if(Axis4_MACHValue > AxisCompBuffer[Axis4_ECPT-1].Value_Linear.Axis4)
			{
				Axis4_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis4_ECPT!=0) && (Axis4_ECPT!=(System.AxisCompMaxL.Axis4+1)));	
	}

	if(Axis4_ECPT == 0)
	{
		Axis4_CompValue = AxisCompBuffer[0].Value_Laser.Axis4+
			(Axis4_MACHValue-AxisCompBuffer[0].Value_Linear.Axis4);
	}
	else if(Axis4_ECPT == (System.AxisCompMaxL.Axis4+1))
	{
		Axis4_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis4].Value_Laser.Axis4+
			(Axis4_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis4].Value_Linear.Axis4);
	}
	else
	{
		Axis4_CompValue = AxisCompBuffer[Axis4_ECPT-1].Value_Laser.Axis4+
			(Axis4_MACHValue-AxisCompBuffer[Axis4_ECPT-1].Value_Linear.Axis4)*
			divdp( (AxisCompBuffer[Axis4_ECPT].Value_Laser.Axis4-AxisCompBuffer[Axis4_ECPT-1].Value_Laser.Axis4), 
			(AxisCompBuffer[Axis4_ECPT].Value_Linear.Axis4-AxisCompBuffer[Axis4_ECPT-1].Value_Linear.Axis4) );
	}		

	OverallSign.EncoderCompPt.Axis4 = Axis4_ECPT;
	
	OverallSign.ABS_Coordinate.Axis4 = Axis4_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis5_mach_value(Int32 Axis5_MACHValue)
{
	Uint32 Axis5_ECPT;
	Int32   Axis5_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis5 <= 2)
	{
		return; 
	}
	
	Axis5_ECPT = OverallSign.EncoderCompPt.Axis5;	

	if(Axis5_ECPT <= 0)
	{
		Axis5_ECPT = 1;
	}
	else if(Axis5_ECPT >=(System.AxisCompMaxL.Axis5+1))
	{
		Axis5_ECPT = System.AxisCompMaxL.Axis5;
	}

	if(System.CompensationDirection.Axis5)
	{	//���򲹳�
		do
		{
			if(Axis5_MACHValue > AxisCompBuffer[Axis5_ECPT].Value_Linear.Axis5)
			{	
				Axis5_ECPT++;
			}
			else if(Axis5_MACHValue < AxisCompBuffer[Axis5_ECPT-1].Value_Linear.Axis5)
			{
				Axis5_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis5_ECPT!=0) && (Axis5_ECPT!=(System.AxisCompMaxL.Axis5+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis5_MACHValue < AxisCompBuffer[Axis5_ECPT].Value_Linear.Axis5)
			{	
				Axis5_ECPT++;
			}
			else if(Axis5_MACHValue > AxisCompBuffer[Axis5_ECPT-1].Value_Linear.Axis5)
			{
				Axis5_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis5_ECPT!=0) && (Axis5_ECPT!=(System.AxisCompMaxL.Axis5+1)));	
	}

	if(Axis5_ECPT == 0)
	{
		Axis5_CompValue = AxisCompBuffer[0].Value_Laser.Axis5+
			(Axis5_MACHValue-AxisCompBuffer[0].Value_Linear.Axis5);
	}
	else if(Axis5_ECPT == (System.AxisCompMaxL.Axis5+1))
	{
		Axis5_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis5].Value_Laser.Axis5+
			(Axis5_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis5].Value_Linear.Axis5);
	}
	else
	{
		Axis5_CompValue = AxisCompBuffer[Axis5_ECPT-1].Value_Laser.Axis5+
			(Axis5_MACHValue-AxisCompBuffer[Axis5_ECPT-1].Value_Linear.Axis5)*
			divdp( (AxisCompBuffer[Axis5_ECPT].Value_Laser.Axis5-AxisCompBuffer[Axis5_ECPT-1].Value_Laser.Axis5), 
			(AxisCompBuffer[Axis5_ECPT].Value_Linear.Axis5-AxisCompBuffer[Axis5_ECPT-1].Value_Linear.Axis5) );
	}		

	OverallSign.EncoderCompPt.Axis5 = Axis5_ECPT;
	
	OverallSign.ABS_Coordinate.Axis5 = Axis5_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis6_mach_value(Int32 Axis6_MACHValue)
{
	Uint32 Axis6_ECPT;
	Int32   Axis6_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis6 <= 2)
	{
		return; 
	}
	
	Axis6_ECPT = OverallSign.EncoderCompPt.Axis6;	

	if(Axis6_ECPT <= 0)
	{
		Axis6_ECPT = 1;
	}
	else if(Axis6_ECPT >=(System.AxisCompMaxL.Axis6+1))
	{
		Axis6_ECPT = System.AxisCompMaxL.Axis6;
	}

	if(System.CompensationDirection.Axis6)
	{	//���򲹳�
		do
		{
			if(Axis6_MACHValue > AxisCompBuffer[Axis6_ECPT].Value_Linear.Axis6)
			{	
				Axis6_ECPT++;
			}
			else if(Axis6_MACHValue < AxisCompBuffer[Axis6_ECPT-1].Value_Linear.Axis6)
			{
				Axis6_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis6_ECPT!=0) && (Axis6_ECPT!=(System.AxisCompMaxL.Axis6+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis6_MACHValue < AxisCompBuffer[Axis6_ECPT].Value_Linear.Axis6)
			{	
				Axis6_ECPT++;
			}
			else if(Axis6_MACHValue > AxisCompBuffer[Axis6_ECPT-1].Value_Linear.Axis6)
			{
				Axis6_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis6_ECPT!=0) && (Axis6_ECPT!=(System.AxisCompMaxL.Axis6+1)));	
	}

	if(Axis6_ECPT == 0)
	{
		Axis6_CompValue = AxisCompBuffer[0].Value_Laser.Axis6+
			(Axis6_MACHValue-AxisCompBuffer[0].Value_Linear.Axis6);
	}
	else if(Axis6_ECPT == (System.AxisCompMaxL.Axis6+1))
	{
		Axis6_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis6].Value_Laser.Axis6+
			(Axis6_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis6].Value_Linear.Axis6);
	}
	else
	{
		Axis6_CompValue = AxisCompBuffer[Axis6_ECPT-1].Value_Laser.Axis6+
			(Axis6_MACHValue-AxisCompBuffer[Axis6_ECPT-1].Value_Linear.Axis6)*
			divdp( (AxisCompBuffer[Axis6_ECPT].Value_Laser.Axis6-AxisCompBuffer[Axis6_ECPT-1].Value_Laser.Axis6), 
			(AxisCompBuffer[Axis6_ECPT].Value_Linear.Axis6-AxisCompBuffer[Axis6_ECPT-1].Value_Linear.Axis6) );
	}		

	OverallSign.EncoderCompPt.Axis6 = Axis6_ECPT;
	
	OverallSign.ABS_Coordinate.Axis6 = Axis6_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis7_mach_value(Int32 Axis7_MACHValue)
{
	Uint32 Axis7_ECPT;
	Int32   Axis7_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis7 <= 2)
	{
		return; 
	}
	
	Axis7_ECPT = OverallSign.EncoderCompPt.Axis7;	

	if(Axis7_ECPT <= 0)
	{
		Axis7_ECPT = 1;
	}
	else if(Axis7_ECPT >=(System.AxisCompMaxL.Axis7+1))
	{
		Axis7_ECPT = System.AxisCompMaxL.Axis7;
	}

	if(System.CompensationDirection.Axis7)
	{	//���򲹳�
		do
		{
			if(Axis7_MACHValue > AxisCompBuffer[Axis7_ECPT].Value_Linear.Axis7)
			{	
				Axis7_ECPT++;
			}
			else if(Axis7_MACHValue < AxisCompBuffer[Axis7_ECPT-1].Value_Linear.Axis7)
			{
				Axis7_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis7_ECPT!=0) && (Axis7_ECPT!=(System.AxisCompMaxL.Axis7+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis7_MACHValue < AxisCompBuffer[Axis7_ECPT].Value_Linear.Axis7)
			{	
				Axis7_ECPT++;
			}
			else if(Axis7_MACHValue > AxisCompBuffer[Axis7_ECPT-1].Value_Linear.Axis7)
			{
				Axis7_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis7_ECPT!=0) && (Axis7_ECPT!=(System.AxisCompMaxL.Axis7+1)));	
	}

	if(Axis7_ECPT == 0)
	{
		Axis7_CompValue = AxisCompBuffer[0].Value_Laser.Axis7+
			(Axis7_MACHValue-AxisCompBuffer[0].Value_Linear.Axis7);
	}
	else if(Axis7_ECPT == (System.AxisCompMaxL.Axis7+1))
	{
		Axis7_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis7].Value_Laser.Axis7+
			(Axis7_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis7].Value_Linear.Axis7);
	}
	else
	{
		Axis7_CompValue = AxisCompBuffer[Axis7_ECPT-1].Value_Laser.Axis7+
			(Axis7_MACHValue-AxisCompBuffer[Axis7_ECPT-1].Value_Linear.Axis7)*
			divdp( (AxisCompBuffer[Axis7_ECPT].Value_Laser.Axis7-AxisCompBuffer[Axis7_ECPT-1].Value_Laser.Axis7), 
			(AxisCompBuffer[Axis7_ECPT].Value_Linear.Axis7-AxisCompBuffer[Axis7_ECPT-1].Value_Linear.Axis7) );
	}		

	OverallSign.EncoderCompPt.Axis7 = Axis7_ECPT;
	
	OverallSign.ABS_Coordinate.Axis7 = Axis7_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis8_mach_value(Int32 Axis8_MACHValue)
{
	Uint32 Axis8_ECPT;
	Int32   Axis8_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis8 <= 2)
	{
		return; 
	}
	
	Axis8_ECPT = OverallSign.EncoderCompPt.Axis8;	

	if(Axis8_ECPT <= 0)
	{
		Axis8_ECPT = 1;
	}
	else if(Axis8_ECPT >=(System.AxisCompMaxL.Axis8+1))
	{
		Axis8_ECPT = System.AxisCompMaxL.Axis8;
	}

	if(System.CompensationDirection.Axis8)
	{	//���򲹳�
		do
		{
			if(Axis8_MACHValue > AxisCompBuffer[Axis8_ECPT].Value_Linear.Axis8)
			{	
				Axis8_ECPT++;
			}
			else if(Axis8_MACHValue < AxisCompBuffer[Axis8_ECPT-1].Value_Linear.Axis8)
			{
				Axis8_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis8_ECPT!=0) && (Axis8_ECPT!=(System.AxisCompMaxL.Axis8+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis8_MACHValue < AxisCompBuffer[Axis8_ECPT].Value_Linear.Axis8)
			{	
				Axis8_ECPT++;
			}
			else if(Axis8_MACHValue > AxisCompBuffer[Axis8_ECPT-1].Value_Linear.Axis8)
			{
				Axis8_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis8_ECPT!=0) && (Axis8_ECPT!=(System.AxisCompMaxL.Axis8+1)));	
	}

	if(Axis8_ECPT == 0)
	{
		Axis8_CompValue = AxisCompBuffer[0].Value_Laser.Axis8+
			(Axis8_MACHValue-AxisCompBuffer[0].Value_Linear.Axis8);
	}
	else if(Axis8_ECPT == (System.AxisCompMaxL.Axis8+1))
	{
		Axis8_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis8].Value_Laser.Axis8+
			(Axis8_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis8].Value_Linear.Axis8);
	}
	else
	{
		Axis8_CompValue = AxisCompBuffer[Axis8_ECPT-1].Value_Laser.Axis8+
			(Axis8_MACHValue-AxisCompBuffer[Axis8_ECPT-1].Value_Linear.Axis8)*
			divdp( (AxisCompBuffer[Axis8_ECPT].Value_Laser.Axis8-AxisCompBuffer[Axis8_ECPT-1].Value_Laser.Axis8), 
			(AxisCompBuffer[Axis8_ECPT].Value_Linear.Axis8-AxisCompBuffer[Axis8_ECPT-1].Value_Linear.Axis8) );
	}		

	OverallSign.EncoderCompPt.Axis8 = Axis8_ECPT;
	
	OverallSign.ABS_Coordinate.Axis8 = Axis8_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis9_mach_value(Int32 Axis9_MACHValue)
{
	Uint32 Axis9_ECPT;
	Int32   Axis9_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis9 <= 2)
	{
		return; 
	}
	
	Axis9_ECPT = OverallSign.EncoderCompPt.Axis9;	

	if(Axis9_ECPT <= 0)
	{
		Axis9_ECPT = 1;
	}
	else if(Axis9_ECPT >=(System.AxisCompMaxL.Axis9+1))
	{
		Axis9_ECPT = System.AxisCompMaxL.Axis9;
	}

	if(System.CompensationDirection.Axis9)
	{	//���򲹳�
		do
		{
			if(Axis9_MACHValue > AxisCompBuffer[Axis9_ECPT].Value_Linear.Axis9)
			{	
				Axis9_ECPT++;
			}
			else if(Axis9_MACHValue < AxisCompBuffer[Axis9_ECPT-1].Value_Linear.Axis9)
			{
				Axis9_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis9_ECPT!=0) && (Axis9_ECPT!=(System.AxisCompMaxL.Axis9+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis9_MACHValue < AxisCompBuffer[Axis9_ECPT].Value_Linear.Axis9)
			{	
				Axis9_ECPT++;
			}
			else if(Axis9_MACHValue > AxisCompBuffer[Axis9_ECPT-1].Value_Linear.Axis9)
			{
				Axis9_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis9_ECPT!=0) && (Axis9_ECPT!=(System.AxisCompMaxL.Axis9+1)));	
	}

	if(Axis9_ECPT == 0)
	{
		Axis9_CompValue = AxisCompBuffer[0].Value_Laser.Axis9+
			(Axis9_MACHValue-AxisCompBuffer[0].Value_Linear.Axis9);
	}
	else if(Axis9_ECPT == (System.AxisCompMaxL.Axis9+1))
	{
		Axis9_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis9].Value_Laser.Axis9+
			(Axis9_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis9].Value_Linear.Axis9);
	}
	else
	{
		Axis9_CompValue = AxisCompBuffer[Axis9_ECPT-1].Value_Laser.Axis9+
			(Axis9_MACHValue-AxisCompBuffer[Axis9_ECPT-1].Value_Linear.Axis9)*
			divdp( (AxisCompBuffer[Axis9_ECPT].Value_Laser.Axis9-AxisCompBuffer[Axis9_ECPT-1].Value_Laser.Axis9), 
			(AxisCompBuffer[Axis9_ECPT].Value_Linear.Axis9-AxisCompBuffer[Axis9_ECPT-1].Value_Linear.Axis9) );
	}		

	OverallSign.EncoderCompPt.Axis9 = Axis9_ECPT;
	
	OverallSign.ABS_Coordinate.Axis9 = Axis9_CompValue;
	
}



//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis10_mach_value(Int32 Axis10_MACHValue)
{
	Uint32 Axis10_ECPT;
	Int32   Axis10_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis10 <= 2)
	{
		return; 
	}
	
	Axis10_ECPT = OverallSign.EncoderCompPt.Axis10;	

	if(Axis10_ECPT <= 0)
	{
		Axis10_ECPT = 1;
	}
	else if(Axis10_ECPT >=(System.AxisCompMaxL.Axis10+1))
	{
		Axis10_ECPT = System.AxisCompMaxL.Axis10;
	}

	if(System.CompensationDirection.Axis10)
	{	//���򲹳�
		do
		{
			if(Axis10_MACHValue > AxisCompBuffer[Axis10_ECPT].Value_Linear.Axis10)
			{	
				Axis10_ECPT++;
			}
			else if(Axis10_MACHValue < AxisCompBuffer[Axis10_ECPT-1].Value_Linear.Axis10)
			{
				Axis10_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis10_ECPT!=0) && (Axis10_ECPT!=(System.AxisCompMaxL.Axis10+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis10_MACHValue < AxisCompBuffer[Axis10_ECPT].Value_Linear.Axis10)
			{	
				Axis10_ECPT++;
			}
			else if(Axis10_MACHValue > AxisCompBuffer[Axis10_ECPT-1].Value_Linear.Axis10)
			{
				Axis10_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis10_ECPT!=0) && (Axis10_ECPT!=(System.AxisCompMaxL.Axis10+1)));	
	}

	if(Axis10_ECPT == 0)
	{
		Axis10_CompValue = AxisCompBuffer[0].Value_Laser.Axis10+
			(Axis10_MACHValue-AxisCompBuffer[0].Value_Linear.Axis10);
	}
	else if(Axis10_ECPT == (System.AxisCompMaxL.Axis10+1))
	{
		Axis10_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis10].Value_Laser.Axis10+
			(Axis10_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis10].Value_Linear.Axis10);
	}
	else
	{
		Axis10_CompValue = AxisCompBuffer[Axis10_ECPT-1].Value_Laser.Axis10+
			(Axis10_MACHValue-AxisCompBuffer[Axis10_ECPT-1].Value_Linear.Axis10)*
			divdp( (AxisCompBuffer[Axis10_ECPT].Value_Laser.Axis10-AxisCompBuffer[Axis10_ECPT-1].Value_Laser.Axis10), 
			(AxisCompBuffer[Axis10_ECPT].Value_Linear.Axis10-AxisCompBuffer[Axis10_ECPT-1].Value_Linear.Axis10) );
	}		

	OverallSign.EncoderCompPt.Axis10 = Axis10_ECPT;
	
	OverallSign.ABS_Coordinate.Axis10 = Axis10_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis11_mach_value(Int32 Axis11_MACHValue)
{
	Uint32 Axis11_ECPT;
	Int32   Axis11_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis11 <= 2)
	{
		return; 
	}
	
	Axis11_ECPT = OverallSign.EncoderCompPt.Axis11;	

	if(Axis11_ECPT <= 0)
	{
		Axis11_ECPT = 1;
	}
	else if(Axis11_ECPT >=(System.AxisCompMaxL.Axis11+1))
	{
		Axis11_ECPT = System.AxisCompMaxL.Axis11;
	}

	if(System.CompensationDirection.Axis11)
	{	//���򲹳�
		do
		{
			if(Axis11_MACHValue > AxisCompBuffer[Axis11_ECPT].Value_Linear.Axis11)
			{	
				Axis11_ECPT++;
			}
			else if(Axis11_MACHValue < AxisCompBuffer[Axis11_ECPT-1].Value_Linear.Axis11)
			{
				Axis11_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis11_ECPT!=0) && (Axis11_ECPT!=(System.AxisCompMaxL.Axis11+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis11_MACHValue < AxisCompBuffer[Axis11_ECPT].Value_Linear.Axis11)
			{	
				Axis11_ECPT++;
			}
			else if(Axis11_MACHValue > AxisCompBuffer[Axis11_ECPT-1].Value_Linear.Axis11)
			{
				Axis11_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis11_ECPT!=0) && (Axis11_ECPT!=(System.AxisCompMaxL.Axis11+1)));	
	}

	if(Axis11_ECPT == 0)
	{
		Axis11_CompValue = AxisCompBuffer[0].Value_Laser.Axis11+
			(Axis11_MACHValue-AxisCompBuffer[0].Value_Linear.Axis11);
	}
	else if(Axis11_ECPT == (System.AxisCompMaxL.Axis11+1))
	{
		Axis11_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis11].Value_Laser.Axis11+
			(Axis11_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis11].Value_Linear.Axis11);
	}
	else
	{
		Axis11_CompValue = AxisCompBuffer[Axis11_ECPT-1].Value_Laser.Axis11+
			(Axis11_MACHValue-AxisCompBuffer[Axis11_ECPT-1].Value_Linear.Axis11)*
			divdp( (AxisCompBuffer[Axis11_ECPT].Value_Laser.Axis11-AxisCompBuffer[Axis11_ECPT-1].Value_Laser.Axis11), 
			(AxisCompBuffer[Axis11_ECPT].Value_Linear.Axis11-AxisCompBuffer[Axis11_ECPT-1].Value_Linear.Axis11) );
	}		

	OverallSign.EncoderCompPt.Axis11 = Axis11_ECPT;
	
	OverallSign.ABS_Coordinate.Axis11 = Axis11_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis12_mach_value(Int32 Axis12_MACHValue)
{
	Uint32 Axis12_ECPT;
	Int32   Axis12_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis12 <= 2)
	{
		return; 
	}
	
	Axis12_ECPT = OverallSign.EncoderCompPt.Axis12;	

	if(Axis12_ECPT <= 0)
	{
		Axis12_ECPT = 1;
	}
	else if(Axis12_ECPT >=(System.AxisCompMaxL.Axis12+1))
	{
		Axis12_ECPT = System.AxisCompMaxL.Axis12;
	}

	if(System.CompensationDirection.Axis12)
	{	//���򲹳�
		do
		{
			if(Axis12_MACHValue > AxisCompBuffer[Axis12_ECPT].Value_Linear.Axis12)
			{	
				Axis12_ECPT++;
			}
			else if(Axis12_MACHValue < AxisCompBuffer[Axis12_ECPT-1].Value_Linear.Axis12)
			{
				Axis12_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis12_ECPT!=0) && (Axis12_ECPT!=(System.AxisCompMaxL.Axis12+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis12_MACHValue < AxisCompBuffer[Axis12_ECPT].Value_Linear.Axis12)
			{	
				Axis12_ECPT++;
			}
			else if(Axis12_MACHValue > AxisCompBuffer[Axis12_ECPT-1].Value_Linear.Axis12)
			{
				Axis12_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis12_ECPT!=0) && (Axis12_ECPT!=(System.AxisCompMaxL.Axis12+1)));	
	}

	if(Axis12_ECPT == 0)
	{
		Axis12_CompValue = AxisCompBuffer[0].Value_Laser.Axis12+
			(Axis12_MACHValue-AxisCompBuffer[0].Value_Linear.Axis12);
	}
	else if(Axis12_ECPT == (System.AxisCompMaxL.Axis12+1))
	{
		Axis12_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis12].Value_Laser.Axis12+
			(Axis12_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis12].Value_Linear.Axis12);
	}
	else
	{
		Axis12_CompValue = AxisCompBuffer[Axis12_ECPT-1].Value_Laser.Axis12+
			(Axis12_MACHValue-AxisCompBuffer[Axis12_ECPT-1].Value_Linear.Axis12)*
			divdp( (AxisCompBuffer[Axis12_ECPT].Value_Laser.Axis12-AxisCompBuffer[Axis12_ECPT-1].Value_Laser.Axis12), 
			(AxisCompBuffer[Axis12_ECPT].Value_Linear.Axis12-AxisCompBuffer[Axis12_ECPT-1].Value_Linear.Axis12) );
	}		

	OverallSign.EncoderCompPt.Axis12 = Axis12_ECPT;
	
	OverallSign.ABS_Coordinate.Axis12 = Axis12_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis13_mach_value(Int32 Axis13_MACHValue)
{
	Uint32 Axis13_ECPT;
	Int32   Axis13_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis13 <= 2)
	{
		return; 
	}
	
	Axis13_ECPT = OverallSign.EncoderCompPt.Axis13;	

	if(Axis13_ECPT <= 0)
	{
		Axis13_ECPT = 1;
	}
	else if(Axis13_ECPT >=(System.AxisCompMaxL.Axis13+1))
	{
		Axis13_ECPT = System.AxisCompMaxL.Axis13;
	}

	if(System.CompensationDirection.Axis13)
	{	//���򲹳�
		do
		{
			if(Axis13_MACHValue > AxisCompBuffer[Axis13_ECPT].Value_Linear.Axis13)
			{	
				Axis13_ECPT++;
			}
			else if(Axis13_MACHValue < AxisCompBuffer[Axis13_ECPT-1].Value_Linear.Axis13)
			{
				Axis13_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis13_ECPT!=0) && (Axis13_ECPT!=(System.AxisCompMaxL.Axis13+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis13_MACHValue < AxisCompBuffer[Axis13_ECPT].Value_Linear.Axis13)
			{	
				Axis13_ECPT++;
			}
			else if(Axis13_MACHValue > AxisCompBuffer[Axis13_ECPT-1].Value_Linear.Axis13)
			{
				Axis13_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis13_ECPT!=0) && (Axis13_ECPT!=(System.AxisCompMaxL.Axis13+1)));	
	}

	if(Axis13_ECPT == 0)
	{
		Axis13_CompValue = AxisCompBuffer[0].Value_Laser.Axis13+
			(Axis13_MACHValue-AxisCompBuffer[0].Value_Linear.Axis13);
	}
	else if(Axis13_ECPT == (System.AxisCompMaxL.Axis13+1))
	{
		Axis13_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis13].Value_Laser.Axis13+
			(Axis13_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis13].Value_Linear.Axis13);
	}
	else
	{
		Axis13_CompValue = AxisCompBuffer[Axis13_ECPT-1].Value_Laser.Axis13+
			(Axis13_MACHValue-AxisCompBuffer[Axis13_ECPT-1].Value_Linear.Axis13)*
			divdp( (AxisCompBuffer[Axis13_ECPT].Value_Laser.Axis13-AxisCompBuffer[Axis13_ECPT-1].Value_Laser.Axis13), 
			(AxisCompBuffer[Axis13_ECPT].Value_Linear.Axis13-AxisCompBuffer[Axis13_ECPT-1].Value_Linear.Axis13) );
	}		

	OverallSign.EncoderCompPt.Axis13 = Axis13_ECPT;
	
	OverallSign.ABS_Coordinate.Axis13 = Axis13_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis14_mach_value(Int32 Axis14_MACHValue)
{
	Uint32 Axis14_ECPT;
	Int32   Axis14_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis14 <= 2)
	{
		return; 
	}
	
	Axis14_ECPT = OverallSign.EncoderCompPt.Axis14;	

	if(Axis14_ECPT <= 0)
	{
		Axis14_ECPT = 1;
	}
	else if(Axis14_ECPT >=(System.AxisCompMaxL.Axis14+1))
	{
		Axis14_ECPT = System.AxisCompMaxL.Axis14;
	}

	if(System.CompensationDirection.Axis14)
	{	//���򲹳�
		do
		{
			if(Axis14_MACHValue > AxisCompBuffer[Axis14_ECPT].Value_Linear.Axis14)
			{	
				Axis14_ECPT++;
			}
			else if(Axis14_MACHValue < AxisCompBuffer[Axis14_ECPT-1].Value_Linear.Axis14)
			{
				Axis14_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis14_ECPT!=0) && (Axis14_ECPT!=(System.AxisCompMaxL.Axis14+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis14_MACHValue < AxisCompBuffer[Axis14_ECPT].Value_Linear.Axis14)
			{	
				Axis14_ECPT++;
			}
			else if(Axis14_MACHValue > AxisCompBuffer[Axis14_ECPT-1].Value_Linear.Axis14)
			{
				Axis14_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis14_ECPT!=0) && (Axis14_ECPT!=(System.AxisCompMaxL.Axis14+1)));	
	}

	if(Axis14_ECPT == 0)
	{
		Axis14_CompValue = AxisCompBuffer[0].Value_Laser.Axis14+
			(Axis14_MACHValue-AxisCompBuffer[0].Value_Linear.Axis14);
	}
	else if(Axis14_ECPT == (System.AxisCompMaxL.Axis14+1))
	{
		Axis14_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis14].Value_Laser.Axis14+
			(Axis14_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis14].Value_Linear.Axis14);
	}
	else
	{
		Axis14_CompValue = AxisCompBuffer[Axis14_ECPT-1].Value_Laser.Axis14+
			(Axis14_MACHValue-AxisCompBuffer[Axis14_ECPT-1].Value_Linear.Axis14)*
			divdp( (AxisCompBuffer[Axis14_ECPT].Value_Laser.Axis14-AxisCompBuffer[Axis14_ECPT-1].Value_Laser.Axis14), 
			(AxisCompBuffer[Axis14_ECPT].Value_Linear.Axis14-AxisCompBuffer[Axis14_ECPT-1].Value_Linear.Axis14) );
	}		

	OverallSign.EncoderCompPt.Axis14 = Axis14_ECPT;
	
	OverallSign.ABS_Coordinate.Axis14 = Axis14_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis15_mach_value(Int32 Axis15_MACHValue)
{
	Uint32 Axis15_ECPT;
	Int32   Axis15_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis15 <= 2)
	{
		return; 
	}
	
	Axis15_ECPT = OverallSign.EncoderCompPt.Axis15;	

	if(Axis15_ECPT <= 0)
	{
		Axis15_ECPT = 1;
	}
	else if(Axis15_ECPT >=(System.AxisCompMaxL.Axis15+1))
	{
		Axis15_ECPT = System.AxisCompMaxL.Axis15;
	}

	if(System.CompensationDirection.Axis15)
	{	//���򲹳�
		do
		{
			if(Axis15_MACHValue > AxisCompBuffer[Axis15_ECPT].Value_Linear.Axis15)
			{	
				Axis15_ECPT++;
			}
			else if(Axis15_MACHValue < AxisCompBuffer[Axis15_ECPT-1].Value_Linear.Axis15)
			{
				Axis15_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis15_ECPT!=0) && (Axis15_ECPT!=(System.AxisCompMaxL.Axis15+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis15_MACHValue < AxisCompBuffer[Axis15_ECPT].Value_Linear.Axis15)
			{	
				Axis15_ECPT++;
			}
			else if(Axis15_MACHValue > AxisCompBuffer[Axis15_ECPT-1].Value_Linear.Axis15)
			{
				Axis15_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis15_ECPT!=0) && (Axis15_ECPT!=(System.AxisCompMaxL.Axis15+1)));	
	}

	if(Axis15_ECPT == 0)
	{
		Axis15_CompValue = AxisCompBuffer[0].Value_Laser.Axis15+
			(Axis15_MACHValue-AxisCompBuffer[0].Value_Linear.Axis15);
	}
	else if(Axis15_ECPT == (System.AxisCompMaxL.Axis15+1))
	{
		Axis15_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis15].Value_Laser.Axis15+
			(Axis15_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis15].Value_Linear.Axis15);
	}
	else
	{
		Axis15_CompValue = AxisCompBuffer[Axis15_ECPT-1].Value_Laser.Axis15+
			(Axis15_MACHValue-AxisCompBuffer[Axis15_ECPT-1].Value_Linear.Axis15)*
			divdp( (AxisCompBuffer[Axis15_ECPT].Value_Laser.Axis15-AxisCompBuffer[Axis15_ECPT-1].Value_Laser.Axis15), 
			(AxisCompBuffer[Axis15_ECPT].Value_Linear.Axis15-AxisCompBuffer[Axis15_ECPT-1].Value_Linear.Axis15) );
	}		

	OverallSign.EncoderCompPt.Axis15 = Axis15_ECPT;
	
	OverallSign.ABS_Coordinate.Axis15 = Axis15_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis16_mach_value(Int32 Axis16_MACHValue)
{
	Uint32 Axis16_ECPT;
	Int32   Axis16_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis16 <= 2)
	{
		return; 
	}
	
	Axis16_ECPT = OverallSign.EncoderCompPt.Axis16;	

	if(Axis16_ECPT <= 0)
	{
		Axis16_ECPT = 1;
	}
	else if(Axis16_ECPT >=(System.AxisCompMaxL.Axis16+1))
	{
		Axis16_ECPT = System.AxisCompMaxL.Axis16;
	}

	if(System.CompensationDirection.Axis16)
	{	//���򲹳�
		do
		{
			if(Axis16_MACHValue > AxisCompBuffer[Axis16_ECPT].Value_Linear.Axis16)
			{	
				Axis16_ECPT++;
			}
			else if(Axis16_MACHValue < AxisCompBuffer[Axis16_ECPT-1].Value_Linear.Axis16)
			{
				Axis16_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis16_ECPT!=0) && (Axis16_ECPT!=(System.AxisCompMaxL.Axis16+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis16_MACHValue < AxisCompBuffer[Axis16_ECPT].Value_Linear.Axis16)
			{	
				Axis16_ECPT++;
			}
			else if(Axis16_MACHValue > AxisCompBuffer[Axis16_ECPT-1].Value_Linear.Axis16)
			{
				Axis16_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis16_ECPT!=0) && (Axis16_ECPT!=(System.AxisCompMaxL.Axis16+1)));	
	}

	if(Axis16_ECPT == 0)
	{
		Axis16_CompValue = AxisCompBuffer[0].Value_Laser.Axis16+
			(Axis16_MACHValue-AxisCompBuffer[0].Value_Linear.Axis16);
	}
	else if(Axis16_ECPT == (System.AxisCompMaxL.Axis16+1))
	{
		Axis16_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis16].Value_Laser.Axis16+
			(Axis16_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis16].Value_Linear.Axis16);
	}
	else
	{
		Axis16_CompValue = AxisCompBuffer[Axis16_ECPT-1].Value_Laser.Axis16+
			(Axis16_MACHValue-AxisCompBuffer[Axis16_ECPT-1].Value_Linear.Axis16)*
			divdp( (AxisCompBuffer[Axis16_ECPT].Value_Laser.Axis16-AxisCompBuffer[Axis16_ECPT-1].Value_Laser.Axis16), 
			(AxisCompBuffer[Axis16_ECPT].Value_Linear.Axis16-AxisCompBuffer[Axis16_ECPT-1].Value_Linear.Axis16) );
	}		

	OverallSign.EncoderCompPt.Axis16 = Axis16_ECPT;
	
	OverallSign.ABS_Coordinate.Axis16 = Axis16_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis17_mach_value(Int32 Axis17_MACHValue)
{
	Uint32 Axis17_ECPT;
	Int32   Axis17_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis17 <= 2)
	{
		return; 
	}
		
	Axis17_ECPT = OverallSign.EncoderCompPt.Axis17;	

	if(Axis17_ECPT <= 0)
	{
		Axis17_ECPT = 1;
	}
	else if(Axis17_ECPT >=(System.AxisCompMaxL.Axis17+1))
	{
		Axis17_ECPT = System.AxisCompMaxL.Axis17;
	}

	if(System.CompensationDirection.Axis17)
	{	//���򲹳�
		do
		{
			if(Axis17_MACHValue > AxisCompBuffer[Axis17_ECPT].Value_Linear.Axis17)
			{	
				Axis17_ECPT++;
			}
			else if(Axis17_MACHValue < AxisCompBuffer[Axis17_ECPT-1].Value_Linear.Axis17)
			{
				Axis17_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis17_ECPT!=0) && (Axis17_ECPT!=(System.AxisCompMaxL.Axis17+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis17_MACHValue < AxisCompBuffer[Axis17_ECPT].Value_Linear.Axis17)
			{	
				Axis17_ECPT++;
			}
			else if(Axis17_MACHValue > AxisCompBuffer[Axis17_ECPT-1].Value_Linear.Axis17)
			{
				Axis17_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis17_ECPT!=0) && (Axis17_ECPT!=(System.AxisCompMaxL.Axis17+1)));	
	}

	if(Axis17_ECPT == 0)
	{
		Axis17_CompValue = AxisCompBuffer[0].Value_Laser.Axis17+
			(Axis17_MACHValue-AxisCompBuffer[0].Value_Linear.Axis17);
	}
	else if(Axis17_ECPT == (System.AxisCompMaxL.Axis17+1))
	{
		Axis17_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis17].Value_Laser.Axis17+
			(Axis17_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis17].Value_Linear.Axis17);
	}
	else
	{
		Axis17_CompValue = AxisCompBuffer[Axis17_ECPT-1].Value_Laser.Axis17+
			(Axis17_MACHValue-AxisCompBuffer[Axis17_ECPT-1].Value_Linear.Axis17)*
			divdp( (AxisCompBuffer[Axis17_ECPT].Value_Laser.Axis17-AxisCompBuffer[Axis17_ECPT-1].Value_Laser.Axis17), 
			(AxisCompBuffer[Axis17_ECPT].Value_Linear.Axis17-AxisCompBuffer[Axis17_ECPT-1].Value_Linear.Axis17) );
	}		

	OverallSign.EncoderCompPt.Axis17 = Axis17_ECPT;
	
	OverallSign.ABS_Coordinate.Axis17 = Axis17_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis18_mach_value(Int32 Axis18_MACHValue)
{
	Uint32 Axis18_ECPT;
	Int32   Axis18_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis18 <= 2)
	{
		return; 
	}
	
	Axis18_ECPT = OverallSign.EncoderCompPt.Axis18;	

	if(Axis18_ECPT <= 0)
	{
		Axis18_ECPT = 1;
	}
	else if(Axis18_ECPT >=(System.AxisCompMaxL.Axis18+1))
	{
		Axis18_ECPT = System.AxisCompMaxL.Axis18;
	}

	if(System.CompensationDirection.Axis18)
	{	//���򲹳�
		do
		{
			if(Axis18_MACHValue > AxisCompBuffer[Axis18_ECPT].Value_Linear.Axis18)
			{	
				Axis18_ECPT++;
			}
			else if(Axis18_MACHValue < AxisCompBuffer[Axis18_ECPT-1].Value_Linear.Axis18)
			{
				Axis18_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis18_ECPT!=0) && (Axis18_ECPT!=(System.AxisCompMaxL.Axis18+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis18_MACHValue < AxisCompBuffer[Axis18_ECPT].Value_Linear.Axis18)
			{	
				Axis18_ECPT++;
			}
			else if(Axis18_MACHValue > AxisCompBuffer[Axis18_ECPT-1].Value_Linear.Axis18)
			{
				Axis18_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis18_ECPT!=0) && (Axis18_ECPT!=(System.AxisCompMaxL.Axis18+1)));	
	}

	if(Axis18_ECPT == 0)
	{
		Axis18_CompValue = AxisCompBuffer[0].Value_Laser.Axis18+
			(Axis18_MACHValue-AxisCompBuffer[0].Value_Linear.Axis18);
	}
	else if(Axis18_ECPT == (System.AxisCompMaxL.Axis18+1))
	{
		Axis18_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis18].Value_Laser.Axis18+
			(Axis18_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis18].Value_Linear.Axis18);
	}
	else
	{
		Axis18_CompValue = AxisCompBuffer[Axis18_ECPT-1].Value_Laser.Axis18+
			(Axis18_MACHValue-AxisCompBuffer[Axis18_ECPT-1].Value_Linear.Axis18)*
			divdp( (AxisCompBuffer[Axis18_ECPT].Value_Laser.Axis18-AxisCompBuffer[Axis18_ECPT-1].Value_Laser.Axis18), 
			(AxisCompBuffer[Axis18_ECPT].Value_Linear.Axis18-AxisCompBuffer[Axis18_ECPT-1].Value_Linear.Axis18) );
	}		

	OverallSign.EncoderCompPt.Axis18 = Axis18_ECPT;
	
	OverallSign.ABS_Coordinate.Axis18 = Axis18_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis19_mach_value(Int32 Axis19_MACHValue)
{
	Uint32 Axis19_ECPT;
	Int32   Axis19_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis19 <= 2)
	{
		return; 
	}
	
	Axis19_ECPT = OverallSign.EncoderCompPt.Axis19;	

	if(Axis19_ECPT <= 0)
	{
		Axis19_ECPT = 1;
	}
	else if(Axis19_ECPT >=(System.AxisCompMaxL.Axis19+1))
	{
		Axis19_ECPT = System.AxisCompMaxL.Axis19;
	}

	if(System.CompensationDirection.Axis19)
	{	//���򲹳�
		do
		{
			if(Axis19_MACHValue > AxisCompBuffer[Axis19_ECPT].Value_Linear.Axis19)
			{	
				Axis19_ECPT++;
			}
			else if(Axis19_MACHValue < AxisCompBuffer[Axis19_ECPT-1].Value_Linear.Axis19)
			{
				Axis19_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis19_ECPT!=0) && (Axis19_ECPT!=(System.AxisCompMaxL.Axis19+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis19_MACHValue < AxisCompBuffer[Axis19_ECPT].Value_Linear.Axis19)
			{	
				Axis19_ECPT++;
			}
			else if(Axis19_MACHValue > AxisCompBuffer[Axis19_ECPT-1].Value_Linear.Axis19)
			{
				Axis19_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis19_ECPT!=0) && (Axis19_ECPT!=(System.AxisCompMaxL.Axis19+1)));	
	}

	if(Axis19_ECPT == 0)
	{
		Axis19_CompValue = AxisCompBuffer[0].Value_Laser.Axis19+
			(Axis19_MACHValue-AxisCompBuffer[0].Value_Linear.Axis19);
	}
	else if(Axis19_ECPT == (System.AxisCompMaxL.Axis19+1))
	{
		Axis19_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis19].Value_Laser.Axis19+
			(Axis19_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis19].Value_Linear.Axis19);
	}
	else
	{
		Axis19_CompValue = AxisCompBuffer[Axis19_ECPT-1].Value_Laser.Axis19+
			(Axis19_MACHValue-AxisCompBuffer[Axis19_ECPT-1].Value_Linear.Axis19)*
			divdp( (AxisCompBuffer[Axis19_ECPT].Value_Laser.Axis19-AxisCompBuffer[Axis19_ECPT-1].Value_Laser.Axis19), 
			(AxisCompBuffer[Axis19_ECPT].Value_Linear.Axis19-AxisCompBuffer[Axis19_ECPT-1].Value_Linear.Axis19) );
	}		

	OverallSign.EncoderCompPt.Axis19 = Axis19_ECPT;
	
	OverallSign.ABS_Coordinate.Axis19 = Axis19_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis20_mach_value(Int32 Axis20_MACHValue)
{
	Uint32 Axis20_ECPT;
	Int32   Axis20_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis20 <= 2)
	{
		return; 
	}
		
	Axis20_ECPT = OverallSign.EncoderCompPt.Axis20;	

	if(Axis20_ECPT <= 0)
	{
		Axis20_ECPT = 1;
	}
	else if(Axis20_ECPT >=(System.AxisCompMaxL.Axis20+1))
	{
		Axis20_ECPT = System.AxisCompMaxL.Axis20;
	}

	if(System.CompensationDirection.Axis20)
	{	//���򲹳�
		do
		{
			if(Axis20_MACHValue > AxisCompBuffer[Axis20_ECPT].Value_Linear.Axis20)
			{	
				Axis20_ECPT++;
			}
			else if(Axis20_MACHValue < AxisCompBuffer[Axis20_ECPT-1].Value_Linear.Axis20)
			{
				Axis20_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis20_ECPT!=0) && (Axis20_ECPT!=(System.AxisCompMaxL.Axis20+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis20_MACHValue < AxisCompBuffer[Axis20_ECPT].Value_Linear.Axis20)
			{	
				Axis20_ECPT++;
			}
			else if(Axis20_MACHValue > AxisCompBuffer[Axis20_ECPT-1].Value_Linear.Axis20)
			{
				Axis20_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis20_ECPT!=0) && (Axis20_ECPT!=(System.AxisCompMaxL.Axis20+1)));	
	}

	if(Axis20_ECPT == 0)
	{
		Axis20_CompValue = AxisCompBuffer[0].Value_Laser.Axis20+
			(Axis20_MACHValue-AxisCompBuffer[0].Value_Linear.Axis20);
	}
	else if(Axis20_ECPT == (System.AxisCompMaxL.Axis20+1))
	{
		Axis20_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis20].Value_Laser.Axis20+
			(Axis20_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis20].Value_Linear.Axis20);
	}
	else
	{
		Axis20_CompValue = AxisCompBuffer[Axis20_ECPT-1].Value_Laser.Axis20+
			(Axis20_MACHValue-AxisCompBuffer[Axis20_ECPT-1].Value_Linear.Axis20)*
			divdp( (AxisCompBuffer[Axis20_ECPT].Value_Laser.Axis20-AxisCompBuffer[Axis20_ECPT-1].Value_Laser.Axis20), 
			(AxisCompBuffer[Axis20_ECPT].Value_Linear.Axis20-AxisCompBuffer[Axis20_ECPT-1].Value_Linear.Axis20) );
	}		

	OverallSign.EncoderCompPt.Axis20 = Axis20_ECPT;
	
	OverallSign.ABS_Coordinate.Axis20 = Axis20_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis21_mach_value(Int32 Axis21_MACHValue)
{
	Uint32 Axis21_ECPT;
	Int32   Axis21_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis21 <= 2)
	{
		return; 
	}
	
	Axis21_ECPT = OverallSign.EncoderCompPt.Axis21;	

	if(Axis21_ECPT <= 0)
	{
		Axis21_ECPT = 1;
	}
	else if(Axis21_ECPT >=(System.AxisCompMaxL.Axis21+1))
	{
		Axis21_ECPT = System.AxisCompMaxL.Axis21;
	}

	if(System.CompensationDirection.Axis21)
	{	//���򲹳�
		do
		{
			if(Axis21_MACHValue > AxisCompBuffer[Axis21_ECPT].Value_Linear.Axis21)
			{	
				Axis21_ECPT++;
			}
			else if(Axis21_MACHValue < AxisCompBuffer[Axis21_ECPT-1].Value_Linear.Axis21)
			{
				Axis21_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis21_ECPT!=0) && (Axis21_ECPT!=(System.AxisCompMaxL.Axis21+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis21_MACHValue < AxisCompBuffer[Axis21_ECPT].Value_Linear.Axis21)
			{	
				Axis21_ECPT++;
			}
			else if(Axis21_MACHValue > AxisCompBuffer[Axis21_ECPT-1].Value_Linear.Axis21)
			{
				Axis21_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis21_ECPT!=0) && (Axis21_ECPT!=(System.AxisCompMaxL.Axis21+1)));	
	}

	if(Axis21_ECPT == 0)
	{
		Axis21_CompValue = AxisCompBuffer[0].Value_Laser.Axis21+
			(Axis21_MACHValue-AxisCompBuffer[0].Value_Linear.Axis21);
	}
	else if(Axis21_ECPT == (System.AxisCompMaxL.Axis21+1))
	{
		Axis21_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis21].Value_Laser.Axis21+
			(Axis21_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis21].Value_Linear.Axis21);
	}
	else
	{
		Axis21_CompValue = AxisCompBuffer[Axis21_ECPT-1].Value_Laser.Axis21+
			(Axis21_MACHValue-AxisCompBuffer[Axis21_ECPT-1].Value_Linear.Axis21)*
			divdp( (AxisCompBuffer[Axis21_ECPT].Value_Laser.Axis21-AxisCompBuffer[Axis21_ECPT-1].Value_Laser.Axis21), 
			(AxisCompBuffer[Axis21_ECPT].Value_Linear.Axis21-AxisCompBuffer[Axis21_ECPT-1].Value_Linear.Axis21) );
	}		

	OverallSign.EncoderCompPt.Axis21 = Axis21_ECPT;
	
	OverallSign.ABS_Coordinate.Axis21 = Axis21_CompValue;
	
}

//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis22_mach_value(Int32 Axis22_MACHValue)
{
	Uint32 Axis22_ECPT;
	Int32   Axis22_CompValue;		//����������ֵ
	
	if(System.AxisCompMaxL.Axis22 <= 2)
	{
		return; 
	}
	
	Axis22_ECPT = OverallSign.EncoderCompPt.Axis22;	

	if(Axis22_ECPT <= 0)
	{
		Axis22_ECPT = 1;
	}
	else if(Axis22_ECPT >=(System.AxisCompMaxL.Axis22+1))
	{
		Axis22_ECPT = System.AxisCompMaxL.Axis22;
	}

	if(System.CompensationDirection.Axis22)
	{	//���򲹳�
		do
		{
			if(Axis22_MACHValue > AxisCompBuffer[Axis22_ECPT].Value_Linear.Axis22)
			{	
				Axis22_ECPT++;
			}
			else if(Axis22_MACHValue < AxisCompBuffer[Axis22_ECPT-1].Value_Linear.Axis22)
			{
				Axis22_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis22_ECPT!=0) && (Axis22_ECPT!=(System.AxisCompMaxL.Axis22+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis22_MACHValue < AxisCompBuffer[Axis22_ECPT].Value_Linear.Axis22)
			{	
				Axis22_ECPT++;
			}
			else if(Axis22_MACHValue > AxisCompBuffer[Axis22_ECPT-1].Value_Linear.Axis22)
			{
				Axis22_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis22_ECPT!=0) && (Axis22_ECPT!=(System.AxisCompMaxL.Axis22+1)));	
	}

	if(Axis22_ECPT == 0)
	{
		Axis22_CompValue = AxisCompBuffer[0].Value_Laser.Axis22+
			(Axis22_MACHValue-AxisCompBuffer[0].Value_Linear.Axis22);
	}
	else if(Axis22_ECPT == (System.AxisCompMaxL.Axis22+1))
	{
		Axis22_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis22].Value_Laser.Axis22+
			(Axis22_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis22].Value_Linear.Axis22);
	}
	else
	{
		Axis22_CompValue = AxisCompBuffer[Axis22_ECPT-1].Value_Laser.Axis22+
			(Axis22_MACHValue-AxisCompBuffer[Axis22_ECPT-1].Value_Linear.Axis22)*
			divdp( (AxisCompBuffer[Axis22_ECPT].Value_Laser.Axis22-AxisCompBuffer[Axis22_ECPT-1].Value_Laser.Axis22), 
			(AxisCompBuffer[Axis22_ECPT].Value_Linear.Axis22-AxisCompBuffer[Axis22_ECPT-1].Value_Linear.Axis22) );
	}		

	OverallSign.EncoderCompPt.Axis22 = Axis22_ECPT;
	
	OverallSign.ABS_Coordinate.Axis22 = Axis22_CompValue;
	
}


//����Ϊ���߲�����
//����:���� ��1�����ķ���������ֵ
//���: δ�����Ļ����ֵ
//����: ������Ļ����ֵ����Ӧ�������ֵ
//��λ:  ��С���뵥λ
void nonlinear_Axis23_mach_value(Int32 Axis23_MACHValue)
{
	Uint32 Axis23_ECPT;
	Int32   Axis23_CompValue;		//����������ֵ

	if(System.AxisCompMaxL.Axis23 <= 2)
	{
		return; 
	}
	
	Axis23_ECPT = OverallSign.EncoderCompPt.Axis23;	

	if(Axis23_ECPT <= 0)
	{
		Axis23_ECPT = 1;
	}
	else if(Axis23_ECPT >=(System.AxisCompMaxL.Axis23+1))
	{
		Axis23_ECPT = System.AxisCompMaxL.Axis23;
	}

	if(System.CompensationDirection.Axis23)
	{	//���򲹳�
		do
		{
			if(Axis23_MACHValue > AxisCompBuffer[Axis23_ECPT].Value_Linear.Axis23)
			{	
				Axis23_ECPT++;
			}
			else if(Axis23_MACHValue < AxisCompBuffer[Axis23_ECPT-1].Value_Linear.Axis23)
			{
				Axis23_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis23_ECPT!=0) && (Axis23_ECPT!=(System.AxisCompMaxL.Axis23+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis23_MACHValue < AxisCompBuffer[Axis23_ECPT].Value_Linear.Axis23)
			{	
				Axis23_ECPT++;
			}
			else if(Axis23_MACHValue > AxisCompBuffer[Axis23_ECPT-1].Value_Linear.Axis23)
			{
				Axis23_ECPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis23_ECPT!=0) && (Axis23_ECPT!=(System.AxisCompMaxL.Axis23+1)));	
	}

	if(Axis23_ECPT == 0)
	{
		Axis23_CompValue = AxisCompBuffer[0].Value_Laser.Axis23+
			(Axis23_MACHValue-AxisCompBuffer[0].Value_Linear.Axis23);
	}
	else if(Axis23_ECPT == (System.AxisCompMaxL.Axis23+1))
	{
		Axis23_CompValue = AxisCompBuffer[System.AxisCompMaxL.Axis23].Value_Laser.Axis23+
			(Axis23_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis23].Value_Linear.Axis23);
	}
	else
	{
		Axis23_CompValue = AxisCompBuffer[Axis23_ECPT-1].Value_Laser.Axis23+
			(Axis23_MACHValue-AxisCompBuffer[Axis23_ECPT-1].Value_Linear.Axis23)*
			divdp( (AxisCompBuffer[Axis23_ECPT].Value_Laser.Axis23-AxisCompBuffer[Axis23_ECPT-1].Value_Laser.Axis23), 
			(AxisCompBuffer[Axis23_ECPT].Value_Linear.Axis23-AxisCompBuffer[Axis23_ECPT-1].Value_Linear.Axis23) );
	}		

	OverallSign.EncoderCompPt.Axis23 = Axis23_ECPT;
	
	OverallSign.ABS_Coordinate.Axis23 = Axis23_CompValue;
	
}

INT40AXIS nonlinear_pulse_out(INT40AXIS Axis_MACHValue_Out)
{
	INT40AXIS Axis_CompValue_Out;
	//��Ӧ��In
	Axis_CompValue_Out.Axis1 = nonlinear_Axis1_pulse_out(Axis_MACHValue_Out.Axis1);
	Axis_CompValue_Out.Axis2 = nonlinear_Axis2_pulse_out(Axis_MACHValue_Out.Axis2);
	Axis_CompValue_Out.Axis3 = nonlinear_Axis3_pulse_out(Axis_MACHValue_Out.Axis3);	
	Axis_CompValue_Out.Axis4 = nonlinear_Axis4_pulse_out(Axis_MACHValue_Out.Axis4);
	Axis_CompValue_Out.Axis5 = nonlinear_Axis5_pulse_out(Axis_MACHValue_Out.Axis5);
	Axis_CompValue_Out.Axis6 = nonlinear_Axis6_pulse_out(Axis_MACHValue_Out.Axis6);
	Axis_CompValue_Out.Axis7 = nonlinear_Axis7_pulse_out(Axis_MACHValue_Out.Axis7);
	Axis_CompValue_Out.Axis8 = nonlinear_Axis8_pulse_out(Axis_MACHValue_Out.Axis8);	
	Axis_CompValue_Out.Axis9 = nonlinear_Axis9_pulse_out(Axis_MACHValue_Out.Axis9);
	Axis_CompValue_Out.Axis10 = nonlinear_Axis10_pulse_out(Axis_MACHValue_Out.Axis10);
	Axis_CompValue_Out.Axis11 = nonlinear_Axis11_pulse_out(Axis_MACHValue_Out.Axis11);
	Axis_CompValue_Out.Axis12 = nonlinear_Axis12_pulse_out(Axis_MACHValue_Out.Axis12);
	Axis_CompValue_Out.Axis13 = nonlinear_Axis13_pulse_out(Axis_MACHValue_Out.Axis13);	
	Axis_CompValue_Out.Axis14 = nonlinear_Axis14_pulse_out(Axis_MACHValue_Out.Axis14);
	Axis_CompValue_Out.Axis15 = nonlinear_Axis15_pulse_out(Axis_MACHValue_Out.Axis15);
	Axis_CompValue_Out.Axis16 = nonlinear_Axis16_pulse_out(Axis_MACHValue_Out.Axis16);
	Axis_CompValue_Out.Axis17 = nonlinear_Axis17_pulse_out(Axis_MACHValue_Out.Axis17);
	Axis_CompValue_Out.Axis18 = nonlinear_Axis18_pulse_out(Axis_MACHValue_Out.Axis18);	
	Axis_CompValue_Out.Axis19 = nonlinear_Axis19_pulse_out(Axis_MACHValue_Out.Axis19);
	Axis_CompValue_Out.Axis20 = nonlinear_Axis20_pulse_out(Axis_MACHValue_Out.Axis20);
	Axis_CompValue_Out.Axis21 = nonlinear_Axis21_pulse_out(Axis_MACHValue_Out.Axis21);
	Axis_CompValue_Out.Axis22 = nonlinear_Axis22_pulse_out(Axis_MACHValue_Out.Axis22);
	Axis_CompValue_Out.Axis23 = nonlinear_Axis23_pulse_out(Axis_MACHValue_Out.Axis23);	
	return Axis_CompValue_Out;
}



//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis1_pulse_out(Int32 Axis1_MACHValue)
{
	Uint16 Axis1_POCPT;
	Int32   Axis1_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis1 <= 2)
	{
		Axis1_CompValue = Axis1_MACHValue;
		return Axis1_CompValue; 
	}
	
	Axis1_POCPT = OverallSign.PulseOutCompPt.Axis1;
	
	if(Axis1_POCPT <= 0)
	{
		Axis1_POCPT = 1;
	}
	else if(Axis1_POCPT >=(System.AxisCompMaxL.Axis1+1))
	{
		Axis1_POCPT = System.AxisCompMaxL.Axis1;
	}

	if(System.CompensationDirection.Axis1)
	{	//���򲹳�
		do
		{
			if(Axis1_MACHValue > AxisCompBuffer[Axis1_POCPT].Value_Laser.Axis1)
			{	
				Axis1_POCPT++;
			}
			else if(Axis1_MACHValue < AxisCompBuffer[Axis1_POCPT-1].Value_Laser.Axis1)
			{
				Axis1_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis1_POCPT!=0) && (Axis1_POCPT!=(System.AxisCompMaxL.Axis1+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis1_MACHValue < AxisCompBuffer[Axis1_POCPT].Value_Laser.Axis1)
			{	
				Axis1_POCPT++;
			}
			else if(Axis1_MACHValue > AxisCompBuffer[Axis1_POCPT-1].Value_Laser.Axis1)
			{
				Axis1_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis1_POCPT!=0) && (Axis1_POCPT!=(System.AxisCompMaxL.Axis1+1)));	
	}

	if(Axis1_POCPT == 0)
	{
		Axis1_CompValue = Axis1_MACHValue-AxisCompBuffer[0].Value_Laser.Axis1+AxisCompBuffer[0].Value_Linear.Axis1;
	}
	else if(Axis1_POCPT == (System.AxisCompMaxL.Axis1+1))
	{
		Axis1_CompValue = Axis1_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis1].Value_Laser.Axis1+
			AxisCompBuffer[System.AxisCompMaxL.Axis1].Value_Linear.Axis1;
	}
	else
	{
		Axis1_CompValue = AxisCompBuffer[Axis1_POCPT-1].Value_Linear.Axis1 + (Axis1_MACHValue-AxisCompBuffer[Axis1_POCPT-1].Value_Laser.Axis1)*
			divdp((AxisCompBuffer[Axis1_POCPT].Value_Linear.Axis1-AxisCompBuffer[Axis1_POCPT-1].Value_Linear.Axis1) , (AxisCompBuffer[Axis1_POCPT].Value_Laser.Axis1-AxisCompBuffer[Axis1_POCPT-1].Value_Laser.Axis1));
	}		

	OverallSign.PulseOutCompPt.Axis1 = Axis1_POCPT;

	// �任���뵥λΪ���������λ
	//Axis1_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis1.AxisOutUnitEQU;
	
	return Axis1_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis2_pulse_out(Int32 Axis2_MACHValue)
{
	Uint16 Axis2_POCPT;
	Int32   Axis2_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis2 <= 2)
	{
		Axis2_CompValue = Axis2_MACHValue;
		return Axis2_CompValue; 
	}
	
	Axis2_POCPT = OverallSign.PulseOutCompPt.Axis2;
	
	if(Axis2_POCPT <= 0)
	{
		Axis2_POCPT = 1;
	}
	else if(Axis2_POCPT >=(System.AxisCompMaxL.Axis2+1))
	{
		Axis2_POCPT = System.AxisCompMaxL.Axis2;
	}

	if(System.CompensationDirection.Axis2)
	{	//���򲹳�
		do
		{
			if(Axis2_MACHValue > AxisCompBuffer[Axis2_POCPT].Value_Laser.Axis2)
			{	
				Axis2_POCPT++;
			}
			else if(Axis2_MACHValue < AxisCompBuffer[Axis2_POCPT-1].Value_Laser.Axis2)
			{
				Axis2_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis2_POCPT!=0) && (Axis2_POCPT!=(System.AxisCompMaxL.Axis2+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis2_MACHValue < AxisCompBuffer[Axis2_POCPT].Value_Laser.Axis2)
			{	
				Axis2_POCPT++;
			}
			else if(Axis2_MACHValue > AxisCompBuffer[Axis2_POCPT-1].Value_Laser.Axis2)
			{
				Axis2_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis2_POCPT!=0) && (Axis2_POCPT!=(System.AxisCompMaxL.Axis2+1)));	
	}

	if(Axis2_POCPT == 0)
	{
		Axis2_CompValue = Axis2_MACHValue-AxisCompBuffer[0].Value_Laser.Axis2+AxisCompBuffer[0].Value_Linear.Axis2;
	}
	else if(Axis2_POCPT == (System.AxisCompMaxL.Axis2+1))
	{
		Axis2_CompValue = Axis2_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis2].Value_Laser.Axis2+
			AxisCompBuffer[System.AxisCompMaxL.Axis2].Value_Linear.Axis2;
	}
	else
	{
		Axis2_CompValue = AxisCompBuffer[Axis2_POCPT-1].Value_Linear.Axis2 + (Axis2_MACHValue-AxisCompBuffer[Axis2_POCPT-1].Value_Laser.Axis2)*
			divdp((AxisCompBuffer[Axis2_POCPT].Value_Linear.Axis2-AxisCompBuffer[Axis2_POCPT-1].Value_Linear.Axis2) , (AxisCompBuffer[Axis2_POCPT].Value_Laser.Axis2-AxisCompBuffer[Axis2_POCPT-1].Value_Laser.Axis2));
	}		

	OverallSign.PulseOutCompPt.Axis2 = Axis2_POCPT;

	// �任���뵥λΪ���������λ
	//Axis2_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis2.AxisOutUnitEQU;
	
	return Axis2_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis3_pulse_out(Int32 Axis3_MACHValue)
{
	Uint16 Axis3_POCPT;
	Int32   Axis3_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis3 <= 2)
	{
		Axis3_CompValue = Axis3_MACHValue;
		return Axis3_CompValue; 
	}
	
	Axis3_POCPT = OverallSign.PulseOutCompPt.Axis3;
	
	if(Axis3_POCPT <= 0)
	{
		Axis3_POCPT = 1;
	}
	else if(Axis3_POCPT >=(System.AxisCompMaxL.Axis3+1))
	{
		Axis3_POCPT = System.AxisCompMaxL.Axis3;
	}

	if(System.CompensationDirection.Axis3)
	{	//���򲹳�
		do
		{
			if(Axis3_MACHValue > AxisCompBuffer[Axis3_POCPT].Value_Laser.Axis3)
			{	
				Axis3_POCPT++;
			}
			else if(Axis3_MACHValue < AxisCompBuffer[Axis3_POCPT-1].Value_Laser.Axis3)
			{
				Axis3_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis3_POCPT!=0) && (Axis3_POCPT!=(System.AxisCompMaxL.Axis3+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis3_MACHValue < AxisCompBuffer[Axis3_POCPT].Value_Laser.Axis3)
			{	
				Axis3_POCPT++;
			}
			else if(Axis3_MACHValue > AxisCompBuffer[Axis3_POCPT-1].Value_Laser.Axis3)
			{
				Axis3_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis3_POCPT!=0) && (Axis3_POCPT!=(System.AxisCompMaxL.Axis3+1)));	
	}

	if(Axis3_POCPT == 0)
	{
		Axis3_CompValue = Axis3_MACHValue-AxisCompBuffer[0].Value_Laser.Axis3+AxisCompBuffer[0].Value_Linear.Axis3;
	}
	else if(Axis3_POCPT == (System.AxisCompMaxL.Axis3+1))
	{
		Axis3_CompValue = Axis3_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis3].Value_Laser.Axis3+
			AxisCompBuffer[System.AxisCompMaxL.Axis3].Value_Linear.Axis3;
	}
	else
	{
		Axis3_CompValue = AxisCompBuffer[Axis3_POCPT-1].Value_Linear.Axis3 + (Axis3_MACHValue-AxisCompBuffer[Axis3_POCPT-1].Value_Laser.Axis3)*
			divdp((AxisCompBuffer[Axis3_POCPT].Value_Linear.Axis3-AxisCompBuffer[Axis3_POCPT-1].Value_Linear.Axis3) , (AxisCompBuffer[Axis3_POCPT].Value_Laser.Axis3-AxisCompBuffer[Axis3_POCPT-1].Value_Laser.Axis3));
	}		

	OverallSign.PulseOutCompPt.Axis3 = Axis3_POCPT;

	// �任���뵥λΪ���������λ
	//Axis3_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis3.AxisOutUnitEQU;
	
	return Axis3_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis4_pulse_out(Int32 Axis4_MACHValue)
{
	Uint16 Axis4_POCPT;
	Int32   Axis4_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis4 <= 2)
	{
		Axis4_CompValue = Axis4_MACHValue;
		return Axis4_CompValue; 
	}
	
	Axis4_POCPT = OverallSign.PulseOutCompPt.Axis4;
	
	if(Axis4_POCPT <= 0)
	{
		Axis4_POCPT = 1;
	}
	else if(Axis4_POCPT >=(System.AxisCompMaxL.Axis4+1))
	{
		Axis4_POCPT = System.AxisCompMaxL.Axis4;
	}

	if(System.CompensationDirection.Axis4)
	{	//���򲹳�
		do
		{
			if(Axis4_MACHValue > AxisCompBuffer[Axis4_POCPT].Value_Laser.Axis4)
			{	
				Axis4_POCPT++;
			}
			else if(Axis4_MACHValue < AxisCompBuffer[Axis4_POCPT-1].Value_Laser.Axis4)
			{
				Axis4_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis4_POCPT!=0) && (Axis4_POCPT!=(System.AxisCompMaxL.Axis4+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis4_MACHValue < AxisCompBuffer[Axis4_POCPT].Value_Laser.Axis4)
			{	
				Axis4_POCPT++;
			}
			else if(Axis4_MACHValue > AxisCompBuffer[Axis4_POCPT-1].Value_Laser.Axis4)
			{
				Axis4_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis4_POCPT!=0) && (Axis4_POCPT!=(System.AxisCompMaxL.Axis4+1)));	
	}

	if(Axis4_POCPT == 0)
	{
		Axis4_CompValue = Axis4_MACHValue-AxisCompBuffer[0].Value_Laser.Axis4+AxisCompBuffer[0].Value_Linear.Axis4;
	}
	else if(Axis4_POCPT == (System.AxisCompMaxL.Axis4+1))
	{
		Axis4_CompValue = Axis4_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis4].Value_Laser.Axis4+
			AxisCompBuffer[System.AxisCompMaxL.Axis4].Value_Linear.Axis4;
	}
	else
	{
		Axis4_CompValue = AxisCompBuffer[Axis4_POCPT-1].Value_Linear.Axis4 + (Axis4_MACHValue-AxisCompBuffer[Axis4_POCPT-1].Value_Laser.Axis4)*
			divdp((AxisCompBuffer[Axis4_POCPT].Value_Linear.Axis4-AxisCompBuffer[Axis4_POCPT-1].Value_Linear.Axis4) , (AxisCompBuffer[Axis4_POCPT].Value_Laser.Axis4-AxisCompBuffer[Axis4_POCPT-1].Value_Laser.Axis4));
	}		

	OverallSign.PulseOutCompPt.Axis4 = Axis4_POCPT;

	// �任���뵥λΪ���������λ
	//Axis4_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis4.AxisOutUnitEQU;
	
	return Axis4_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis5_pulse_out(Int32 Axis5_MACHValue)
{
	Uint16 Axis5_POCPT;
	Int32   Axis5_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis5 <= 2)
	{
		Axis5_CompValue = Axis5_MACHValue;
		return Axis5_CompValue; 
	}
	
	Axis5_POCPT = OverallSign.PulseOutCompPt.Axis5;
	
	if(Axis5_POCPT <= 0)
	{
		Axis5_POCPT = 1;
	}
	else if(Axis5_POCPT >=(System.AxisCompMaxL.Axis5+1))
	{
		Axis5_POCPT = System.AxisCompMaxL.Axis5;
	}

	if(System.CompensationDirection.Axis5)
	{	//���򲹳�
		do
		{
			if(Axis5_MACHValue > AxisCompBuffer[Axis5_POCPT].Value_Laser.Axis5)
			{	
				Axis5_POCPT++;
			}
			else if(Axis5_MACHValue < AxisCompBuffer[Axis5_POCPT-1].Value_Laser.Axis5)
			{
				Axis5_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis5_POCPT!=0) && (Axis5_POCPT!=(System.AxisCompMaxL.Axis5+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis5_MACHValue < AxisCompBuffer[Axis5_POCPT].Value_Laser.Axis5)
			{	
				Axis5_POCPT++;
			}
			else if(Axis5_MACHValue > AxisCompBuffer[Axis5_POCPT-1].Value_Laser.Axis5)
			{
				Axis5_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis5_POCPT!=0) && (Axis5_POCPT!=(System.AxisCompMaxL.Axis5+1)));	
	}

	if(Axis5_POCPT == 0)
	{
		Axis5_CompValue = Axis5_MACHValue-AxisCompBuffer[0].Value_Laser.Axis5+AxisCompBuffer[0].Value_Linear.Axis5;
	}
	else if(Axis5_POCPT == (System.AxisCompMaxL.Axis5+1))
	{
		Axis5_CompValue = Axis5_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis5].Value_Laser.Axis5+
			AxisCompBuffer[System.AxisCompMaxL.Axis5].Value_Linear.Axis5;
	}
	else
	{
		Axis5_CompValue = AxisCompBuffer[Axis5_POCPT-1].Value_Linear.Axis5 + (Axis5_MACHValue-AxisCompBuffer[Axis5_POCPT-1].Value_Laser.Axis5)*
			divdp((AxisCompBuffer[Axis5_POCPT].Value_Linear.Axis5-AxisCompBuffer[Axis5_POCPT-1].Value_Linear.Axis5) , (AxisCompBuffer[Axis5_POCPT].Value_Laser.Axis5-AxisCompBuffer[Axis5_POCPT-1].Value_Laser.Axis5));
	}		

	OverallSign.PulseOutCompPt.Axis5 = Axis5_POCPT;

	// �任���뵥λΪ���������λ
	//Axis5_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis5.AxisOutUnitEQU;
	
	return Axis5_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis6_pulse_out(Int32 Axis6_MACHValue)
{
	Uint16 Axis6_POCPT;
	Int32   Axis6_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis6 <= 2)
	{
		Axis6_CompValue = Axis6_MACHValue;
		return Axis6_CompValue; 
	}
	
	Axis6_POCPT = OverallSign.PulseOutCompPt.Axis6;
	
	if(Axis6_POCPT <= 0)
	{
		Axis6_POCPT = 1;
	}
	else if(Axis6_POCPT >=(System.AxisCompMaxL.Axis6+1))
	{
		Axis6_POCPT = System.AxisCompMaxL.Axis6;
	}

	if(System.CompensationDirection.Axis6)
	{	//���򲹳�
		do
		{
			if(Axis6_MACHValue > AxisCompBuffer[Axis6_POCPT].Value_Laser.Axis6)
			{	
				Axis6_POCPT++;
			}
			else if(Axis6_MACHValue < AxisCompBuffer[Axis6_POCPT-1].Value_Laser.Axis6)
			{
				Axis6_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis6_POCPT!=0) && (Axis6_POCPT!=(System.AxisCompMaxL.Axis6+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis6_MACHValue < AxisCompBuffer[Axis6_POCPT].Value_Laser.Axis6)
			{	
				Axis6_POCPT++;
			}
			else if(Axis6_MACHValue > AxisCompBuffer[Axis6_POCPT-1].Value_Laser.Axis6)
			{
				Axis6_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis6_POCPT!=0) && (Axis6_POCPT!=(System.AxisCompMaxL.Axis6+1)));	
	}

	if(Axis6_POCPT == 0)
	{
		Axis6_CompValue = Axis6_MACHValue-AxisCompBuffer[0].Value_Laser.Axis6+AxisCompBuffer[0].Value_Linear.Axis6;
	}
	else if(Axis6_POCPT == (System.AxisCompMaxL.Axis6+1))
	{
		Axis6_CompValue = Axis6_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis6].Value_Laser.Axis6+
			AxisCompBuffer[System.AxisCompMaxL.Axis6].Value_Linear.Axis6;
	}
	else
	{
		Axis6_CompValue = AxisCompBuffer[Axis6_POCPT-1].Value_Linear.Axis6 + (Axis6_MACHValue-AxisCompBuffer[Axis6_POCPT-1].Value_Laser.Axis6)*
			divdp((AxisCompBuffer[Axis6_POCPT].Value_Linear.Axis6-AxisCompBuffer[Axis6_POCPT-1].Value_Linear.Axis6) , (AxisCompBuffer[Axis6_POCPT].Value_Laser.Axis6-AxisCompBuffer[Axis6_POCPT-1].Value_Laser.Axis6));
	}		

	OverallSign.PulseOutCompPt.Axis6 = Axis6_POCPT;

	// �任���뵥λΪ���������λ
	//Axis6_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis6.AxisOutUnitEQU;
	
	return Axis6_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis7_pulse_out(Int32 Axis7_MACHValue)
{
	Uint16 Axis7_POCPT;
	Int32   Axis7_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis7 <= 2)
	{
		Axis7_CompValue = Axis7_MACHValue;
		return Axis7_CompValue; 
	}
	
	Axis7_POCPT = OverallSign.PulseOutCompPt.Axis7;
	
	if(Axis7_POCPT <= 0)
	{
		Axis7_POCPT = 1;
	}
	else if(Axis7_POCPT >=(System.AxisCompMaxL.Axis7+1))
	{
		Axis7_POCPT = System.AxisCompMaxL.Axis7;
	}

	if(System.CompensationDirection.Axis7)
	{	//���򲹳�
		do
		{
			if(Axis7_MACHValue > AxisCompBuffer[Axis7_POCPT].Value_Laser.Axis7)
			{	
				Axis7_POCPT++;
			}
			else if(Axis7_MACHValue < AxisCompBuffer[Axis7_POCPT-1].Value_Laser.Axis7)
			{
				Axis7_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis7_POCPT!=0) && (Axis7_POCPT!=(System.AxisCompMaxL.Axis7+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis7_MACHValue < AxisCompBuffer[Axis7_POCPT].Value_Laser.Axis7)
			{	
				Axis7_POCPT++;
			}
			else if(Axis7_MACHValue > AxisCompBuffer[Axis7_POCPT-1].Value_Laser.Axis7)
			{
				Axis7_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis7_POCPT!=0) && (Axis7_POCPT!=(System.AxisCompMaxL.Axis7+1)));	
	}

	if(Axis7_POCPT == 0)
	{
		Axis7_CompValue = Axis7_MACHValue-AxisCompBuffer[0].Value_Laser.Axis7+AxisCompBuffer[0].Value_Linear.Axis7;
	}
	else if(Axis7_POCPT == (System.AxisCompMaxL.Axis7+1))
	{
		Axis7_CompValue = Axis7_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis7].Value_Laser.Axis7+
			AxisCompBuffer[System.AxisCompMaxL.Axis7].Value_Linear.Axis7;
	}
	else
	{
		Axis7_CompValue = AxisCompBuffer[Axis7_POCPT-1].Value_Linear.Axis7 + (Axis7_MACHValue-AxisCompBuffer[Axis7_POCPT-1].Value_Laser.Axis7)*
			divdp((AxisCompBuffer[Axis7_POCPT].Value_Linear.Axis7-AxisCompBuffer[Axis7_POCPT-1].Value_Linear.Axis7) , (AxisCompBuffer[Axis7_POCPT].Value_Laser.Axis7-AxisCompBuffer[Axis7_POCPT-1].Value_Laser.Axis7));
	}		

	OverallSign.PulseOutCompPt.Axis7 = Axis7_POCPT;

	// �任���뵥λΪ���������λ
	//Axis7_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis7.AxisOutUnitEQU;
	
	return Axis7_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis8_pulse_out(Int32 Axis8_MACHValue)
{
	Uint16 Axis8_POCPT;
	Int32   Axis8_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis8 <= 2)
	{
		Axis8_CompValue = Axis8_MACHValue;
		return Axis8_CompValue; 
	}
	
	Axis8_POCPT = OverallSign.PulseOutCompPt.Axis8;
	
	if(Axis8_POCPT <= 0)
	{
		Axis8_POCPT = 1;
	}
	else if(Axis8_POCPT >=(System.AxisCompMaxL.Axis8+1))
	{
		Axis8_POCPT = System.AxisCompMaxL.Axis8;
	}

	if(System.CompensationDirection.Axis8)
	{	//���򲹳�
		do
		{
			if(Axis8_MACHValue > AxisCompBuffer[Axis8_POCPT].Value_Laser.Axis8)
			{	
				Axis8_POCPT++;
			}
			else if(Axis8_MACHValue < AxisCompBuffer[Axis8_POCPT-1].Value_Laser.Axis8)
			{
				Axis8_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis8_POCPT!=0) && (Axis8_POCPT!=(System.AxisCompMaxL.Axis8+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis8_MACHValue < AxisCompBuffer[Axis8_POCPT].Value_Laser.Axis8)
			{	
				Axis8_POCPT++;
			}
			else if(Axis8_MACHValue > AxisCompBuffer[Axis8_POCPT-1].Value_Laser.Axis8)
			{
				Axis8_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis8_POCPT!=0) && (Axis8_POCPT!=(System.AxisCompMaxL.Axis8+1)));	
	}

	if(Axis8_POCPT == 0)
	{
		Axis8_CompValue = Axis8_MACHValue-AxisCompBuffer[0].Value_Laser.Axis8+AxisCompBuffer[0].Value_Linear.Axis8;
	}
	else if(Axis8_POCPT == (System.AxisCompMaxL.Axis8+1))
	{
		Axis8_CompValue = Axis8_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis8].Value_Laser.Axis8+
			AxisCompBuffer[System.AxisCompMaxL.Axis8].Value_Linear.Axis8;
	}
	else
	{
		Axis8_CompValue = AxisCompBuffer[Axis8_POCPT-1].Value_Linear.Axis8 + (Axis8_MACHValue-AxisCompBuffer[Axis8_POCPT-1].Value_Laser.Axis8)*
			divdp((AxisCompBuffer[Axis8_POCPT].Value_Linear.Axis8-AxisCompBuffer[Axis8_POCPT-1].Value_Linear.Axis8) , (AxisCompBuffer[Axis8_POCPT].Value_Laser.Axis8-AxisCompBuffer[Axis8_POCPT-1].Value_Laser.Axis8));
	}		

	OverallSign.PulseOutCompPt.Axis8 = Axis8_POCPT;

	// �任���뵥λΪ���������λ
	//Axis8_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis8.AxisOutUnitEQU;
	
	return Axis8_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis9_pulse_out(Int32 Axis9_MACHValue)
{
	Uint16 Axis9_POCPT;
	Int32   Axis9_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis9 <= 2)
	{
		Axis9_CompValue = Axis9_MACHValue;
		return Axis9_CompValue; 
	}
	
	Axis9_POCPT = OverallSign.PulseOutCompPt.Axis9;
	
	if(Axis9_POCPT <= 0)
	{
		Axis9_POCPT = 1;
	}
	else if(Axis9_POCPT >=(System.AxisCompMaxL.Axis9+1))
	{
		Axis9_POCPT = System.AxisCompMaxL.Axis9;
	}

	if(System.CompensationDirection.Axis9)
	{	//���򲹳�
		do
		{
			if(Axis9_MACHValue > AxisCompBuffer[Axis9_POCPT].Value_Laser.Axis9)
			{	
				Axis9_POCPT++;
			}
			else if(Axis9_MACHValue < AxisCompBuffer[Axis9_POCPT-1].Value_Laser.Axis9)
			{
				Axis9_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis9_POCPT!=0) && (Axis9_POCPT!=(System.AxisCompMaxL.Axis9+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis9_MACHValue < AxisCompBuffer[Axis9_POCPT].Value_Laser.Axis9)
			{	
				Axis9_POCPT++;
			}
			else if(Axis9_MACHValue > AxisCompBuffer[Axis9_POCPT-1].Value_Laser.Axis9)
			{
				Axis9_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis9_POCPT!=0) && (Axis9_POCPT!=(System.AxisCompMaxL.Axis9+1)));	
	}

	if(Axis9_POCPT == 0)
	{
		Axis9_CompValue = Axis9_MACHValue-AxisCompBuffer[0].Value_Laser.Axis9+AxisCompBuffer[0].Value_Linear.Axis9;
	}
	else if(Axis9_POCPT == (System.AxisCompMaxL.Axis9+1))
	{
		Axis9_CompValue = Axis9_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis9].Value_Laser.Axis9+
			AxisCompBuffer[System.AxisCompMaxL.Axis9].Value_Linear.Axis9;
	}
	else
	{
		Axis9_CompValue = AxisCompBuffer[Axis9_POCPT-1].Value_Linear.Axis9 + (Axis9_MACHValue-AxisCompBuffer[Axis9_POCPT-1].Value_Laser.Axis9)*
			divdp((AxisCompBuffer[Axis9_POCPT].Value_Linear.Axis9-AxisCompBuffer[Axis9_POCPT-1].Value_Linear.Axis9) , (AxisCompBuffer[Axis9_POCPT].Value_Laser.Axis9-AxisCompBuffer[Axis9_POCPT-1].Value_Laser.Axis9));
	}		

	OverallSign.PulseOutCompPt.Axis9 = Axis9_POCPT;

	// �任���뵥λΪ���������λ
	//Axis9_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis9.AxisOutUnitEQU;
	
	return Axis9_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis10_pulse_out(Int32 Axis10_MACHValue)
{
	Uint16 Axis10_POCPT;
	Int32   Axis10_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis10 <= 2)
	{
		Axis10_CompValue = Axis10_MACHValue;
		return Axis10_CompValue; 
	}
	
	Axis10_POCPT = OverallSign.PulseOutCompPt.Axis10;
	
	if(Axis10_POCPT <= 0)
	{
		Axis10_POCPT = 1;
	}
	else if(Axis10_POCPT >=(System.AxisCompMaxL.Axis10+1))
	{
		Axis10_POCPT = System.AxisCompMaxL.Axis10;
	}

	if(System.CompensationDirection.Axis10)
	{	//���򲹳�
		do
		{
			if(Axis10_MACHValue > AxisCompBuffer[Axis10_POCPT].Value_Laser.Axis10)
			{	
				Axis10_POCPT++;
			}
			else if(Axis10_MACHValue < AxisCompBuffer[Axis10_POCPT-1].Value_Laser.Axis10)
			{
				Axis10_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis10_POCPT!=0) && (Axis10_POCPT!=(System.AxisCompMaxL.Axis10+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis10_MACHValue < AxisCompBuffer[Axis10_POCPT].Value_Laser.Axis10)
			{	
				Axis10_POCPT++;
			}
			else if(Axis10_MACHValue > AxisCompBuffer[Axis10_POCPT-1].Value_Laser.Axis10)
			{
				Axis10_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis10_POCPT!=0) && (Axis10_POCPT!=(System.AxisCompMaxL.Axis10+1)));	
	}

	if(Axis10_POCPT == 0)
	{
		Axis10_CompValue = Axis10_MACHValue-AxisCompBuffer[0].Value_Laser.Axis10+AxisCompBuffer[0].Value_Linear.Axis10;
	}
	else if(Axis10_POCPT == (System.AxisCompMaxL.Axis10+1))
	{
		Axis10_CompValue = Axis10_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis10].Value_Laser.Axis10+
			AxisCompBuffer[System.AxisCompMaxL.Axis10].Value_Linear.Axis10;
	}
	else
	{
		Axis10_CompValue = AxisCompBuffer[Axis10_POCPT-1].Value_Linear.Axis10 + (Axis10_MACHValue-AxisCompBuffer[Axis10_POCPT-1].Value_Laser.Axis10)*
			divdp((AxisCompBuffer[Axis10_POCPT].Value_Linear.Axis10-AxisCompBuffer[Axis10_POCPT-1].Value_Linear.Axis10) , (AxisCompBuffer[Axis10_POCPT].Value_Laser.Axis10-AxisCompBuffer[Axis10_POCPT-1].Value_Laser.Axis10));
	}		

	OverallSign.PulseOutCompPt.Axis10 = Axis10_POCPT;

	// �任���뵥λΪ���������λ
	//Axis10_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis10.AxisOutUnitEQU;
	
	return Axis10_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis11_pulse_out(Int32 Axis11_MACHValue)
{
	Uint16 Axis11_POCPT;
	Int32   Axis11_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis11 <= 2)
	{
		Axis11_CompValue = Axis11_MACHValue;
		return Axis11_CompValue; 
	}
	
	Axis11_POCPT = OverallSign.PulseOutCompPt.Axis11;
	
	if(Axis11_POCPT <= 0)
	{
		Axis11_POCPT = 1;
	}
	else if(Axis11_POCPT >=(System.AxisCompMaxL.Axis11+1))
	{
		Axis11_POCPT = System.AxisCompMaxL.Axis11;
	}

	if(System.CompensationDirection.Axis11)
	{	//���򲹳�
		do
		{
			if(Axis11_MACHValue > AxisCompBuffer[Axis11_POCPT].Value_Laser.Axis11)
			{	
				Axis11_POCPT++;
			}
			else if(Axis11_MACHValue < AxisCompBuffer[Axis11_POCPT-1].Value_Laser.Axis11)
			{
				Axis11_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis11_POCPT!=0) && (Axis11_POCPT!=(System.AxisCompMaxL.Axis11+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis11_MACHValue < AxisCompBuffer[Axis11_POCPT].Value_Laser.Axis11)
			{	
				Axis11_POCPT++;
			}
			else if(Axis11_MACHValue > AxisCompBuffer[Axis11_POCPT-1].Value_Laser.Axis11)
			{
				Axis11_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis11_POCPT!=0) && (Axis11_POCPT!=(System.AxisCompMaxL.Axis11+1)));	
	}

	if(Axis11_POCPT == 0)
	{
		Axis11_CompValue = Axis11_MACHValue-AxisCompBuffer[0].Value_Laser.Axis11+AxisCompBuffer[0].Value_Linear.Axis11;
	}
	else if(Axis11_POCPT == (System.AxisCompMaxL.Axis11+1))
	{
		Axis11_CompValue = Axis11_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis11].Value_Laser.Axis11+
			AxisCompBuffer[System.AxisCompMaxL.Axis11].Value_Linear.Axis11;
	}
	else
	{
		Axis11_CompValue = AxisCompBuffer[Axis11_POCPT-1].Value_Linear.Axis11 + (Axis11_MACHValue-AxisCompBuffer[Axis11_POCPT-1].Value_Laser.Axis11)*
			divdp((AxisCompBuffer[Axis11_POCPT].Value_Linear.Axis11-AxisCompBuffer[Axis11_POCPT-1].Value_Linear.Axis11) , (AxisCompBuffer[Axis11_POCPT].Value_Laser.Axis11-AxisCompBuffer[Axis11_POCPT-1].Value_Laser.Axis11));
	}		

	OverallSign.PulseOutCompPt.Axis11 = Axis11_POCPT;

	// �任���뵥λΪ���������λ
	//Axis11_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis11.AxisOutUnitEQU;
	
	return Axis11_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis12_pulse_out(Int32 Axis12_MACHValue)
{
	Uint16 Axis12_POCPT;
	Int32   Axis12_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis12 <= 2)
	{
		Axis12_CompValue = Axis12_MACHValue;
		return Axis12_CompValue; 
	}
	
	Axis12_POCPT = OverallSign.PulseOutCompPt.Axis12;
	
	if(Axis12_POCPT <= 0)
	{
		Axis12_POCPT = 1;
	}
	else if(Axis12_POCPT >=(System.AxisCompMaxL.Axis12+1))
	{
		Axis12_POCPT = System.AxisCompMaxL.Axis12;
	}

	if(System.CompensationDirection.Axis12)
	{	//���򲹳�
		do
		{
			if(Axis12_MACHValue > AxisCompBuffer[Axis12_POCPT].Value_Laser.Axis12)
			{	
				Axis12_POCPT++;
			}
			else if(Axis12_MACHValue < AxisCompBuffer[Axis12_POCPT-1].Value_Laser.Axis12)
			{
				Axis12_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis12_POCPT!=0) && (Axis12_POCPT!=(System.AxisCompMaxL.Axis12+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis12_MACHValue < AxisCompBuffer[Axis12_POCPT].Value_Laser.Axis12)
			{	
				Axis12_POCPT++;
			}
			else if(Axis12_MACHValue > AxisCompBuffer[Axis12_POCPT-1].Value_Laser.Axis12)
			{
				Axis12_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis12_POCPT!=0) && (Axis12_POCPT!=(System.AxisCompMaxL.Axis12+1)));	
	}

	if(Axis12_POCPT == 0)
	{
		Axis12_CompValue = Axis12_MACHValue-AxisCompBuffer[0].Value_Laser.Axis12+AxisCompBuffer[0].Value_Linear.Axis12;
	}
	else if(Axis12_POCPT == (System.AxisCompMaxL.Axis12+1))
	{
		Axis12_CompValue = Axis12_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis12].Value_Laser.Axis12+
			AxisCompBuffer[System.AxisCompMaxL.Axis12].Value_Linear.Axis12;
	}
	else
	{
		Axis12_CompValue = AxisCompBuffer[Axis12_POCPT-1].Value_Linear.Axis12 + (Axis12_MACHValue-AxisCompBuffer[Axis12_POCPT-1].Value_Laser.Axis12)*
			divdp((AxisCompBuffer[Axis12_POCPT].Value_Linear.Axis12-AxisCompBuffer[Axis12_POCPT-1].Value_Linear.Axis12) , (AxisCompBuffer[Axis12_POCPT].Value_Laser.Axis12-AxisCompBuffer[Axis12_POCPT-1].Value_Laser.Axis12));
	}		

	OverallSign.PulseOutCompPt.Axis12 = Axis12_POCPT;

	// �任���뵥λΪ���������λ
	//Axis12_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis12.AxisOutUnitEQU;
	
	return Axis12_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis13_pulse_out(Int32 Axis13_MACHValue)
{
	Uint16 Axis13_POCPT;
	Int32   Axis13_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis13 <= 2)
	{
		Axis13_CompValue = Axis13_MACHValue;
		return Axis13_CompValue; 
	}
	
	Axis13_POCPT = OverallSign.PulseOutCompPt.Axis13;
	
	if(Axis13_POCPT <= 0)
	{
		Axis13_POCPT = 1;
	}
	else if(Axis13_POCPT >=(System.AxisCompMaxL.Axis13+1))
	{
		Axis13_POCPT = System.AxisCompMaxL.Axis13;
	}

	if(System.CompensationDirection.Axis13)
	{	//���򲹳�
		do
		{
			if(Axis13_MACHValue > AxisCompBuffer[Axis13_POCPT].Value_Laser.Axis13)
			{	
				Axis13_POCPT++;
			}
			else if(Axis13_MACHValue < AxisCompBuffer[Axis13_POCPT-1].Value_Laser.Axis13)
			{
				Axis13_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis13_POCPT!=0) && (Axis13_POCPT!=(System.AxisCompMaxL.Axis13+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis13_MACHValue < AxisCompBuffer[Axis13_POCPT].Value_Laser.Axis13)
			{	
				Axis13_POCPT++;
			}
			else if(Axis13_MACHValue > AxisCompBuffer[Axis13_POCPT-1].Value_Laser.Axis13)
			{
				Axis13_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis13_POCPT!=0) && (Axis13_POCPT!=(System.AxisCompMaxL.Axis13+1)));	
	}

	if(Axis13_POCPT == 0)
	{
		Axis13_CompValue = Axis13_MACHValue-AxisCompBuffer[0].Value_Laser.Axis13+AxisCompBuffer[0].Value_Linear.Axis13;
	}
	else if(Axis13_POCPT == (System.AxisCompMaxL.Axis13+1))
	{
		Axis13_CompValue = Axis13_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis13].Value_Laser.Axis13+
			AxisCompBuffer[System.AxisCompMaxL.Axis13].Value_Linear.Axis13;
	}
	else
	{
		Axis13_CompValue = AxisCompBuffer[Axis13_POCPT-1].Value_Linear.Axis13 + (Axis13_MACHValue-AxisCompBuffer[Axis13_POCPT-1].Value_Laser.Axis13)*
			divdp((AxisCompBuffer[Axis13_POCPT].Value_Linear.Axis13-AxisCompBuffer[Axis13_POCPT-1].Value_Linear.Axis13) , (AxisCompBuffer[Axis13_POCPT].Value_Laser.Axis13-AxisCompBuffer[Axis13_POCPT-1].Value_Laser.Axis13));
	}		

	OverallSign.PulseOutCompPt.Axis13 = Axis13_POCPT;

	// �任���뵥λΪ���������λ
	//Axis13_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis13.AxisOutUnitEQU;
	
	return Axis13_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis14_pulse_out(Int32 Axis14_MACHValue)
{
	Uint16 Axis14_POCPT;
	Int32   Axis14_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis14 <= 2)
	{
		Axis14_CompValue = Axis14_MACHValue;
		return Axis14_CompValue; 
	}
	
	Axis14_POCPT = OverallSign.PulseOutCompPt.Axis14;
	
	if(Axis14_POCPT <= 0)
	{
		Axis14_POCPT = 1;
	}
	else if(Axis14_POCPT >=(System.AxisCompMaxL.Axis14+1))
	{
		Axis14_POCPT = System.AxisCompMaxL.Axis14;
	}

	if(System.CompensationDirection.Axis14)
	{	//���򲹳�
		do
		{
			if(Axis14_MACHValue > AxisCompBuffer[Axis14_POCPT].Value_Laser.Axis14)
			{	
				Axis14_POCPT++;
			}
			else if(Axis14_MACHValue < AxisCompBuffer[Axis14_POCPT-1].Value_Laser.Axis14)
			{
				Axis14_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis14_POCPT!=0) && (Axis14_POCPT!=(System.AxisCompMaxL.Axis14+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis14_MACHValue < AxisCompBuffer[Axis14_POCPT].Value_Laser.Axis14)
			{	
				Axis14_POCPT++;
			}
			else if(Axis14_MACHValue > AxisCompBuffer[Axis14_POCPT-1].Value_Laser.Axis14)
			{
				Axis14_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis14_POCPT!=0) && (Axis14_POCPT!=(System.AxisCompMaxL.Axis14+1)));	
	}

	if(Axis14_POCPT == 0)
	{
		Axis14_CompValue = Axis14_MACHValue-AxisCompBuffer[0].Value_Laser.Axis14+AxisCompBuffer[0].Value_Linear.Axis14;
	}
	else if(Axis14_POCPT == (System.AxisCompMaxL.Axis14+1))
	{
		Axis14_CompValue = Axis14_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis14].Value_Laser.Axis14+
			AxisCompBuffer[System.AxisCompMaxL.Axis14].Value_Linear.Axis14;
	}
	else
	{
		Axis14_CompValue = AxisCompBuffer[Axis14_POCPT-1].Value_Linear.Axis14 + (Axis14_MACHValue-AxisCompBuffer[Axis14_POCPT-1].Value_Laser.Axis14)*
			divdp((AxisCompBuffer[Axis14_POCPT].Value_Linear.Axis14-AxisCompBuffer[Axis14_POCPT-1].Value_Linear.Axis14) , (AxisCompBuffer[Axis14_POCPT].Value_Laser.Axis14-AxisCompBuffer[Axis14_POCPT-1].Value_Laser.Axis14));
	}		

	OverallSign.PulseOutCompPt.Axis14 = Axis14_POCPT;

	// �任���뵥λΪ���������λ
	//Axis14_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis14.AxisOutUnitEQU;
	
	return Axis14_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis15_pulse_out(Int32 Axis15_MACHValue)
{
	Uint16 Axis15_POCPT;
	Int32   Axis15_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis15 <= 2)
	{
		Axis15_CompValue = Axis15_MACHValue;
		return Axis15_CompValue; 
	}

	Axis15_POCPT = OverallSign.PulseOutCompPt.Axis15;
	
	if(Axis15_POCPT <= 0)
	{
		Axis15_POCPT = 1;
	}
	else if(Axis15_POCPT >=(System.AxisCompMaxL.Axis15+1))
	{
		Axis15_POCPT = System.AxisCompMaxL.Axis15;
	}

	if(System.CompensationDirection.Axis15)
	{	//���򲹳�
		do
		{
			if(Axis15_MACHValue > AxisCompBuffer[Axis15_POCPT].Value_Laser.Axis15)
			{	
				Axis15_POCPT++;
			}
			else if(Axis15_MACHValue < AxisCompBuffer[Axis15_POCPT-1].Value_Laser.Axis15)
			{
				Axis15_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis15_POCPT!=0) && (Axis15_POCPT!=(System.AxisCompMaxL.Axis15+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis15_MACHValue < AxisCompBuffer[Axis15_POCPT].Value_Laser.Axis15)
			{	
				Axis15_POCPT++;
			}
			else if(Axis15_MACHValue > AxisCompBuffer[Axis15_POCPT-1].Value_Laser.Axis15)
			{
				Axis15_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis15_POCPT!=0) && (Axis15_POCPT!=(System.AxisCompMaxL.Axis15+1)));	
	}

	if(Axis15_POCPT == 0)
	{
		Axis15_CompValue = Axis15_MACHValue-AxisCompBuffer[0].Value_Laser.Axis15+AxisCompBuffer[0].Value_Linear.Axis15;
	}
	else if(Axis15_POCPT == (System.AxisCompMaxL.Axis15+1))
	{
		Axis15_CompValue = Axis15_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis15].Value_Laser.Axis15+
			AxisCompBuffer[System.AxisCompMaxL.Axis15].Value_Linear.Axis15;
	}
	else
	{
		Axis15_CompValue = AxisCompBuffer[Axis15_POCPT-1].Value_Linear.Axis15 + (Axis15_MACHValue-AxisCompBuffer[Axis15_POCPT-1].Value_Laser.Axis15)*
			divdp((AxisCompBuffer[Axis15_POCPT].Value_Linear.Axis15-AxisCompBuffer[Axis15_POCPT-1].Value_Linear.Axis15) , (AxisCompBuffer[Axis15_POCPT].Value_Laser.Axis15-AxisCompBuffer[Axis15_POCPT-1].Value_Laser.Axis15));
	}		

	OverallSign.PulseOutCompPt.Axis15 = Axis15_POCPT;

	// �任���뵥λΪ���������λ
	//Axis15_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis15.AxisOutUnitEQU;
	
	return Axis15_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis16_pulse_out(Int32 Axis16_MACHValue)
{
	Uint16 Axis16_POCPT;
	Int32   Axis16_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis16 <= 2)
	{
		Axis16_CompValue = Axis16_MACHValue;
		return Axis16_CompValue; 
	}
	
	Axis16_POCPT = OverallSign.PulseOutCompPt.Axis16;
	
	if(Axis16_POCPT <= 0)
	{
		Axis16_POCPT = 1;
	}
	else if(Axis16_POCPT >=(System.AxisCompMaxL.Axis16+1))
	{
		Axis16_POCPT = System.AxisCompMaxL.Axis16;
	}

	if(System.CompensationDirection.Axis16)
	{	//���򲹳�
		do
		{
			if(Axis16_MACHValue > AxisCompBuffer[Axis16_POCPT].Value_Laser.Axis16)
			{	
				Axis16_POCPT++;
			}
			else if(Axis16_MACHValue < AxisCompBuffer[Axis16_POCPT-1].Value_Laser.Axis16)
			{
				Axis16_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis16_POCPT!=0) && (Axis16_POCPT!=(System.AxisCompMaxL.Axis16+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis16_MACHValue < AxisCompBuffer[Axis16_POCPT].Value_Laser.Axis16)
			{	
				Axis16_POCPT++;
			}
			else if(Axis16_MACHValue > AxisCompBuffer[Axis16_POCPT-1].Value_Laser.Axis16)
			{
				Axis16_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis16_POCPT!=0) && (Axis16_POCPT!=(System.AxisCompMaxL.Axis16+1)));	
	}

	if(Axis16_POCPT == 0)
	{
		Axis16_CompValue = Axis16_MACHValue-AxisCompBuffer[0].Value_Laser.Axis16+AxisCompBuffer[0].Value_Linear.Axis16;
	}
	else if(Axis16_POCPT == (System.AxisCompMaxL.Axis16+1))
	{
		Axis16_CompValue = Axis16_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis16].Value_Laser.Axis16+
			AxisCompBuffer[System.AxisCompMaxL.Axis16].Value_Linear.Axis16;
	}
	else
	{
		Axis16_CompValue = AxisCompBuffer[Axis16_POCPT-1].Value_Linear.Axis16 + (Axis16_MACHValue-AxisCompBuffer[Axis16_POCPT-1].Value_Laser.Axis16)*
			divdp((AxisCompBuffer[Axis16_POCPT].Value_Linear.Axis16-AxisCompBuffer[Axis16_POCPT-1].Value_Linear.Axis16) , (AxisCompBuffer[Axis16_POCPT].Value_Laser.Axis16-AxisCompBuffer[Axis16_POCPT-1].Value_Laser.Axis16));
	}		

	OverallSign.PulseOutCompPt.Axis16 = Axis16_POCPT;

	// �任���뵥λΪ���������λ
	//Axis16_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis16.AxisOutUnitEQU;
	
	return Axis16_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis17_pulse_out(Int32 Axis17_MACHValue)
{
	Uint16 Axis17_POCPT;
	Int32   Axis17_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis17 <= 2)
	{
		Axis17_CompValue = Axis17_MACHValue;
		return Axis17_CompValue; 
	}
	
	Axis17_POCPT = OverallSign.PulseOutCompPt.Axis17;
	
	if(Axis17_POCPT <= 0)
	{
		Axis17_POCPT = 1;
	}
	else if(Axis17_POCPT >=(System.AxisCompMaxL.Axis17+1))
	{
		Axis17_POCPT = System.AxisCompMaxL.Axis17;
	}

	if(System.CompensationDirection.Axis17)
	{	//���򲹳�
		do
		{
			if(Axis17_MACHValue > AxisCompBuffer[Axis17_POCPT].Value_Laser.Axis17)
			{	
				Axis17_POCPT++;
			}
			else if(Axis17_MACHValue < AxisCompBuffer[Axis17_POCPT-1].Value_Laser.Axis17)
			{
				Axis17_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis17_POCPT!=0) && (Axis17_POCPT!=(System.AxisCompMaxL.Axis17+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis17_MACHValue < AxisCompBuffer[Axis17_POCPT].Value_Laser.Axis17)
			{	
				Axis17_POCPT++;
			}
			else if(Axis17_MACHValue > AxisCompBuffer[Axis17_POCPT-1].Value_Laser.Axis17)
			{
				Axis17_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis17_POCPT!=0) && (Axis17_POCPT!=(System.AxisCompMaxL.Axis17+1)));	
	}

	if(Axis17_POCPT == 0)
	{
		Axis17_CompValue = Axis17_MACHValue-AxisCompBuffer[0].Value_Laser.Axis17+AxisCompBuffer[0].Value_Linear.Axis17;
	}
	else if(Axis17_POCPT == (System.AxisCompMaxL.Axis17+1))
	{
		Axis17_CompValue = Axis17_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis17].Value_Laser.Axis17+
			AxisCompBuffer[System.AxisCompMaxL.Axis17].Value_Linear.Axis17;
	}
	else
	{
		Axis17_CompValue = AxisCompBuffer[Axis17_POCPT-1].Value_Linear.Axis17 + (Axis17_MACHValue-AxisCompBuffer[Axis17_POCPT-1].Value_Laser.Axis17)*
			divdp((AxisCompBuffer[Axis17_POCPT].Value_Linear.Axis17-AxisCompBuffer[Axis17_POCPT-1].Value_Linear.Axis17) , (AxisCompBuffer[Axis17_POCPT].Value_Laser.Axis17-AxisCompBuffer[Axis17_POCPT-1].Value_Laser.Axis17));
	}		

	OverallSign.PulseOutCompPt.Axis17 = Axis17_POCPT;

	// �任���뵥λΪ���������λ
	//Axis17_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis17.AxisOutUnitEQU;
	
	return Axis17_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis18_pulse_out(Int32 Axis18_MACHValue)
{
	Uint16 Axis18_POCPT;
	Int32   Axis18_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis18 <= 2)
	{
		Axis18_CompValue = Axis18_MACHValue;
		return Axis18_CompValue; 
	}
	
	Axis18_POCPT = OverallSign.PulseOutCompPt.Axis18;
	
	if(Axis18_POCPT <= 0)
	{
		Axis18_POCPT = 1;
	}
	else if(Axis18_POCPT >=(System.AxisCompMaxL.Axis18+1))
	{
		Axis18_POCPT = System.AxisCompMaxL.Axis18;
	}

	if(System.CompensationDirection.Axis18)
	{	//���򲹳�
		do
		{
			if(Axis18_MACHValue > AxisCompBuffer[Axis18_POCPT].Value_Laser.Axis18)
			{	
				Axis18_POCPT++;
			}
			else if(Axis18_MACHValue < AxisCompBuffer[Axis18_POCPT-1].Value_Laser.Axis18)
			{
				Axis18_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis18_POCPT!=0) && (Axis18_POCPT!=(System.AxisCompMaxL.Axis18+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis18_MACHValue < AxisCompBuffer[Axis18_POCPT].Value_Laser.Axis18)
			{	
				Axis18_POCPT++;
			}
			else if(Axis18_MACHValue > AxisCompBuffer[Axis18_POCPT-1].Value_Laser.Axis18)
			{
				Axis18_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis18_POCPT!=0) && (Axis18_POCPT!=(System.AxisCompMaxL.Axis18+1)));	
	}

	if(Axis18_POCPT == 0)
	{
		Axis18_CompValue = Axis18_MACHValue-AxisCompBuffer[0].Value_Laser.Axis18+AxisCompBuffer[0].Value_Linear.Axis18;
	}
	else if(Axis18_POCPT == (System.AxisCompMaxL.Axis18+1))
	{
		Axis18_CompValue = Axis18_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis18].Value_Laser.Axis18+
			AxisCompBuffer[System.AxisCompMaxL.Axis18].Value_Linear.Axis18;
	}
	else
	{
		Axis18_CompValue = AxisCompBuffer[Axis18_POCPT-1].Value_Linear.Axis18 + (Axis18_MACHValue-AxisCompBuffer[Axis18_POCPT-1].Value_Laser.Axis18)*
			divdp((AxisCompBuffer[Axis18_POCPT].Value_Linear.Axis18-AxisCompBuffer[Axis18_POCPT-1].Value_Linear.Axis18) , (AxisCompBuffer[Axis18_POCPT].Value_Laser.Axis18-AxisCompBuffer[Axis18_POCPT-1].Value_Laser.Axis18));
	}		

	OverallSign.PulseOutCompPt.Axis18 = Axis18_POCPT;

	// �任���뵥λΪ���������λ
	//Axis18_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis18.AxisOutUnitEQU;
	
	return Axis18_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis19_pulse_out(Int32 Axis19_MACHValue)
{
	Uint16 Axis19_POCPT;
	Int32   Axis19_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis19 <= 2)
	{
		Axis19_CompValue = Axis19_MACHValue;
		return Axis19_CompValue; 
	}
	
	Axis19_POCPT = OverallSign.PulseOutCompPt.Axis19;
	
	if(Axis19_POCPT <= 0)
	{
		Axis19_POCPT = 1;
	}
	else if(Axis19_POCPT >=(System.AxisCompMaxL.Axis19+1))
	{
		Axis19_POCPT = System.AxisCompMaxL.Axis19;
	}

	if(System.CompensationDirection.Axis19)
	{	//���򲹳�
		do
		{
			if(Axis19_MACHValue > AxisCompBuffer[Axis19_POCPT].Value_Laser.Axis19)
			{	
				Axis19_POCPT++;
			}
			else if(Axis19_MACHValue < AxisCompBuffer[Axis19_POCPT-1].Value_Laser.Axis19)
			{
				Axis19_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis19_POCPT!=0) && (Axis19_POCPT!=(System.AxisCompMaxL.Axis19+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis19_MACHValue < AxisCompBuffer[Axis19_POCPT].Value_Laser.Axis19)
			{	
				Axis19_POCPT++;
			}
			else if(Axis19_MACHValue > AxisCompBuffer[Axis19_POCPT-1].Value_Laser.Axis19)
			{
				Axis19_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis19_POCPT!=0) && (Axis19_POCPT!=(System.AxisCompMaxL.Axis19+1)));	
	}

	if(Axis19_POCPT == 0)
	{
		Axis19_CompValue = Axis19_MACHValue-AxisCompBuffer[0].Value_Laser.Axis19+AxisCompBuffer[0].Value_Linear.Axis19;
	}
	else if(Axis19_POCPT == (System.AxisCompMaxL.Axis19+1))
	{
		Axis19_CompValue = Axis19_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis19].Value_Laser.Axis19+
			AxisCompBuffer[System.AxisCompMaxL.Axis19].Value_Linear.Axis19;
	}
	else
	{
		Axis19_CompValue = AxisCompBuffer[Axis19_POCPT-1].Value_Linear.Axis19 + (Axis19_MACHValue-AxisCompBuffer[Axis19_POCPT-1].Value_Laser.Axis19)*
			divdp((AxisCompBuffer[Axis19_POCPT].Value_Linear.Axis19-AxisCompBuffer[Axis19_POCPT-1].Value_Linear.Axis19) , (AxisCompBuffer[Axis19_POCPT].Value_Laser.Axis19-AxisCompBuffer[Axis19_POCPT-1].Value_Laser.Axis19));
	}		

	OverallSign.PulseOutCompPt.Axis19 = Axis19_POCPT;

	// �任���뵥λΪ���������λ
	//Axis19_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis19.AxisOutUnitEQU;
	
	return Axis19_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis20_pulse_out(Int32 Axis20_MACHValue)
{
	Uint16 Axis20_POCPT;
	Int32   Axis20_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis20 <= 2)
	{
		Axis20_CompValue = Axis20_MACHValue;
		return Axis20_CompValue; 
	}
	
	Axis20_POCPT = OverallSign.PulseOutCompPt.Axis20;
	
	if(Axis20_POCPT <= 0)
	{
		Axis20_POCPT = 1;
	}
	else if(Axis20_POCPT >=(System.AxisCompMaxL.Axis20+1))
	{
		Axis20_POCPT = System.AxisCompMaxL.Axis20;
	}

	if(System.CompensationDirection.Axis20)
	{	//���򲹳�
		do
		{
			if(Axis20_MACHValue > AxisCompBuffer[Axis20_POCPT].Value_Laser.Axis20)
			{	
				Axis20_POCPT++;
			}
			else if(Axis20_MACHValue < AxisCompBuffer[Axis20_POCPT-1].Value_Laser.Axis20)
			{
				Axis20_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis20_POCPT!=0) && (Axis20_POCPT!=(System.AxisCompMaxL.Axis20+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis20_MACHValue < AxisCompBuffer[Axis20_POCPT].Value_Laser.Axis20)
			{	
				Axis20_POCPT++;
			}
			else if(Axis20_MACHValue > AxisCompBuffer[Axis20_POCPT-1].Value_Laser.Axis20)
			{
				Axis20_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis20_POCPT!=0) && (Axis20_POCPT!=(System.AxisCompMaxL.Axis20+1)));	
	}

	if(Axis20_POCPT == 0)
	{
		Axis20_CompValue = Axis20_MACHValue-AxisCompBuffer[0].Value_Laser.Axis20+AxisCompBuffer[0].Value_Linear.Axis20;
	}
	else if(Axis20_POCPT == (System.AxisCompMaxL.Axis20+1))
	{
		Axis20_CompValue = Axis20_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis20].Value_Laser.Axis20+
			AxisCompBuffer[System.AxisCompMaxL.Axis20].Value_Linear.Axis20;
	}
	else
	{
		Axis20_CompValue = AxisCompBuffer[Axis20_POCPT-1].Value_Linear.Axis20 + (Axis20_MACHValue-AxisCompBuffer[Axis20_POCPT-1].Value_Laser.Axis20)*
			divdp((AxisCompBuffer[Axis20_POCPT].Value_Linear.Axis20-AxisCompBuffer[Axis20_POCPT-1].Value_Linear.Axis20) , (AxisCompBuffer[Axis20_POCPT].Value_Laser.Axis20-AxisCompBuffer[Axis20_POCPT-1].Value_Laser.Axis20));
	}		

	OverallSign.PulseOutCompPt.Axis20 = Axis20_POCPT;

	// �任���뵥λΪ���������λ
	//Axis20_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis20.AxisOutUnitEQU;
	
	return Axis20_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis21_pulse_out(Int32 Axis21_MACHValue)
{
	Uint16 Axis21_POCPT;
	Int32   Axis21_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis21 <= 2)
	{
		Axis21_CompValue = Axis21_MACHValue;
		return Axis21_CompValue; 
	}
	
	Axis21_POCPT = OverallSign.PulseOutCompPt.Axis21;
	
	if(Axis21_POCPT <= 0)
	{
		Axis21_POCPT = 1;
	}
	else if(Axis21_POCPT >=(System.AxisCompMaxL.Axis21+1))
	{
		Axis21_POCPT = System.AxisCompMaxL.Axis21;
	}

	if(System.CompensationDirection.Axis21)
	{	//���򲹳�
		do
		{
			if(Axis21_MACHValue > AxisCompBuffer[Axis21_POCPT].Value_Laser.Axis21)
			{	
				Axis21_POCPT++;
			}
			else if(Axis21_MACHValue < AxisCompBuffer[Axis21_POCPT-1].Value_Laser.Axis21)
			{
				Axis21_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis21_POCPT!=0) && (Axis21_POCPT!=(System.AxisCompMaxL.Axis21+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis21_MACHValue < AxisCompBuffer[Axis21_POCPT].Value_Laser.Axis21)
			{	
				Axis21_POCPT++;
			}
			else if(Axis21_MACHValue > AxisCompBuffer[Axis21_POCPT-1].Value_Laser.Axis21)
			{
				Axis21_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis21_POCPT!=0) && (Axis21_POCPT!=(System.AxisCompMaxL.Axis21+1)));	
	}

	if(Axis21_POCPT == 0)
	{
		Axis21_CompValue = Axis21_MACHValue-AxisCompBuffer[0].Value_Laser.Axis21+AxisCompBuffer[0].Value_Linear.Axis21;
	}
	else if(Axis21_POCPT == (System.AxisCompMaxL.Axis21+1))
	{
		Axis21_CompValue = Axis21_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis21].Value_Laser.Axis21+
			AxisCompBuffer[System.AxisCompMaxL.Axis21].Value_Linear.Axis21;
	}
	else
	{
		Axis21_CompValue = AxisCompBuffer[Axis21_POCPT-1].Value_Linear.Axis21 + (Axis21_MACHValue-AxisCompBuffer[Axis21_POCPT-1].Value_Laser.Axis21)*
			divdp((AxisCompBuffer[Axis21_POCPT].Value_Linear.Axis21-AxisCompBuffer[Axis21_POCPT-1].Value_Linear.Axis21) , (AxisCompBuffer[Axis21_POCPT].Value_Laser.Axis21-AxisCompBuffer[Axis21_POCPT-1].Value_Laser.Axis21));
	}		

	OverallSign.PulseOutCompPt.Axis21 = Axis21_POCPT;

	// �任���뵥λΪ���������λ
	//Axis21_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis21.AxisOutUnitEQU;
	
	return Axis21_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis22_pulse_out(Int32 Axis22_MACHValue)
{
	Uint16 Axis22_POCPT;
	Int32   Axis22_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis22 <= 2)
	{
		Axis22_CompValue = Axis22_MACHValue;
		return Axis22_CompValue; 
	}
	
	Axis22_POCPT = OverallSign.PulseOutCompPt.Axis22;
	
	if(Axis22_POCPT <= 0)
	{
		Axis22_POCPT = 1;
	}
	else if(Axis22_POCPT >=(System.AxisCompMaxL.Axis22+1))
	{
		Axis22_POCPT = System.AxisCompMaxL.Axis22;
	}

	if(System.CompensationDirection.Axis22)
	{	//���򲹳�
		do
		{
			if(Axis22_MACHValue > AxisCompBuffer[Axis22_POCPT].Value_Laser.Axis22)
			{	
				Axis22_POCPT++;
			}
			else if(Axis22_MACHValue < AxisCompBuffer[Axis22_POCPT-1].Value_Laser.Axis22)
			{
				Axis22_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis22_POCPT!=0) && (Axis22_POCPT!=(System.AxisCompMaxL.Axis22+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis22_MACHValue < AxisCompBuffer[Axis22_POCPT].Value_Laser.Axis22)
			{	
				Axis22_POCPT++;
			}
			else if(Axis22_MACHValue > AxisCompBuffer[Axis22_POCPT-1].Value_Laser.Axis22)
			{
				Axis22_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis22_POCPT!=0) && (Axis22_POCPT!=(System.AxisCompMaxL.Axis22+1)));	
	}

	if(Axis22_POCPT == 0)
	{
		Axis22_CompValue = Axis22_MACHValue-AxisCompBuffer[0].Value_Laser.Axis22+AxisCompBuffer[0].Value_Linear.Axis22;
	}
	else if(Axis22_POCPT == (System.AxisCompMaxL.Axis22+1))
	{
		Axis22_CompValue = Axis22_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis22].Value_Laser.Axis22+
			AxisCompBuffer[System.AxisCompMaxL.Axis22].Value_Linear.Axis22;
	}
	else
	{
		Axis22_CompValue = AxisCompBuffer[Axis22_POCPT-1].Value_Linear.Axis22 + (Axis22_MACHValue-AxisCompBuffer[Axis22_POCPT-1].Value_Laser.Axis22)*
			divdp((AxisCompBuffer[Axis22_POCPT].Value_Linear.Axis22-AxisCompBuffer[Axis22_POCPT-1].Value_Linear.Axis22) , (AxisCompBuffer[Axis22_POCPT].Value_Laser.Axis22-AxisCompBuffer[Axis22_POCPT-1].Value_Laser.Axis22));
	}		

	OverallSign.PulseOutCompPt.Axis22 = Axis22_POCPT;

	// �任���뵥λΪ���������λ
	//Axis22_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis22.AxisOutUnitEQU;
	
	return Axis22_CompValue;	
}

//���: ��ǰ�岹�����δ���򲹳�����λ��ֵ����С���뵥λ
//����: ������1�������ķ�����������򲹳�ֵ
//������ֵΪ������������λ��ֵ
Int32 nonlinear_Axis23_pulse_out(Int32 Axis23_MACHValue)
{
	Uint16 Axis23_POCPT;
	Int32   Axis23_CompValue;		//�油�����MACH λ��ֵ

	if(System.AxisCompMaxL.Axis23 <= 2)
	{
		Axis23_CompValue = Axis23_MACHValue;
		return Axis23_CompValue; 
	}
	
	Axis23_POCPT = OverallSign.PulseOutCompPt.Axis23;
	
	if(Axis23_POCPT <= 0)
	{
		Axis23_POCPT = 1;
	}
	else if(Axis23_POCPT >=(System.AxisCompMaxL.Axis23+1))
	{
		Axis23_POCPT = System.AxisCompMaxL.Axis23;
	}

	if(System.CompensationDirection.Axis23)
	{	//���򲹳�
		do
		{
			if(Axis23_MACHValue > AxisCompBuffer[Axis23_POCPT].Value_Laser.Axis23)
			{	
				Axis23_POCPT++;
			}
			else if(Axis23_MACHValue < AxisCompBuffer[Axis23_POCPT-1].Value_Laser.Axis23)
			{
				Axis23_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis23_POCPT!=0) && (Axis23_POCPT!=(System.AxisCompMaxL.Axis23+1)));	
	}
	else
	{	//���򲹳�
		do
		{
			if(Axis23_MACHValue < AxisCompBuffer[Axis23_POCPT].Value_Laser.Axis23)
			{	
				Axis23_POCPT++;
			}
			else if(Axis23_MACHValue > AxisCompBuffer[Axis23_POCPT-1].Value_Laser.Axis23)
			{
				Axis23_POCPT--;
			}
			else
			{	//���ڵ�ǰ������
				break;
			}
		}while((Axis23_POCPT!=0) && (Axis23_POCPT!=(System.AxisCompMaxL.Axis23+1)));	
	}

	if(Axis23_POCPT == 0)
	{
		Axis23_CompValue = Axis23_MACHValue-AxisCompBuffer[0].Value_Laser.Axis23+AxisCompBuffer[0].Value_Linear.Axis23;
	}
	else if(Axis23_POCPT == (System.AxisCompMaxL.Axis23+1))
	{
		Axis23_CompValue = Axis23_MACHValue-AxisCompBuffer[System.AxisCompMaxL.Axis23].Value_Laser.Axis23+
			AxisCompBuffer[System.AxisCompMaxL.Axis23].Value_Linear.Axis23;
	}
	else
	{
		Axis23_CompValue = AxisCompBuffer[Axis23_POCPT-1].Value_Linear.Axis23 + (Axis23_MACHValue-AxisCompBuffer[Axis23_POCPT-1].Value_Laser.Axis23)*
			divdp((AxisCompBuffer[Axis23_POCPT].Value_Linear.Axis23-AxisCompBuffer[Axis23_POCPT-1].Value_Linear.Axis23) , (AxisCompBuffer[Axis23_POCPT].Value_Laser.Axis23-AxisCompBuffer[Axis23_POCPT-1].Value_Laser.Axis23));
	}		

	OverallSign.PulseOutCompPt.Axis23 = Axis23_POCPT;

	// �任���뵥λΪ���������λ
	//Axis23_CompValue *= divdp(System.LinearAxisMinUnit,System.LinearAxisOutUnitEQU)*Servo_Axis23.AxisOutUnitEQU;
	
	return Axis23_CompValue;	
}


/******************CALCULATE ABSOLUTE COORDINATE****************/
Uint16 get_ABS_value(void)	
{
	INT40AXIS Point_ABS_Coordinate;

	if(OverallSign.GetMachCoordinateRight == 0)
		return 0;

	OverallSign.ABS_Coordinate.Axis1 = Encoder_Axis1.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis1;
	OverallSign.ABS_Coordinate.Axis2 = Encoder_Axis2.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis2;
	OverallSign.ABS_Coordinate.Axis3 = Encoder_Axis3.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis3;
	OverallSign.ABS_Coordinate.Axis4 = Encoder_Axis4.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis4;
	OverallSign.ABS_Coordinate.Axis5 = Encoder_Axis5.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis5;
	OverallSign.ABS_Coordinate.Axis6 = Encoder_Axis6.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis6;
	OverallSign.ABS_Coordinate.Axis7 = Encoder_Axis7.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis7;
	OverallSign.ABS_Coordinate.Axis8 = Encoder_Axis8.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis8;
	OverallSign.ABS_Coordinate.Axis9 = Encoder_Axis9.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis9;
	OverallSign.ABS_Coordinate.Axis10 = Encoder_Axis10.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis10;
	OverallSign.ABS_Coordinate.Axis11 = Encoder_Axis11.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis11;	
	OverallSign.ABS_Coordinate.Axis12 = Encoder_Axis12.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis12;
	OverallSign.ABS_Coordinate.Axis13 = Encoder_Axis13.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis13;
	OverallSign.ABS_Coordinate.Axis14 = Encoder_Axis14.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis14;
	OverallSign.ABS_Coordinate.Axis15 = Encoder_Axis15.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis15;
	OverallSign.ABS_Coordinate.Axis16 = Encoder_Axis16.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis16;
	OverallSign.ABS_Coordinate.Axis17 = Encoder_Axis17.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis17;
	OverallSign.ABS_Coordinate.Axis18 = Encoder_Axis18.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis18;
	OverallSign.ABS_Coordinate.Axis19 = Encoder_Axis19.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis19;
	OverallSign.ABS_Coordinate.Axis20 = Encoder_Axis20.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis20;
	OverallSign.ABS_Coordinate.Axis21 = Encoder_Axis21.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis21;

	if(System.FunctionSelect05 == False)
	{
		OverallSign.ABS_Coordinate.Axis21 = OverallSign.AbsoluteEncoderValue_PSO.Axis21;
	}
	
	OverallSign.ABS_Coordinate.Axis22 = Encoder_Axis22.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis22;
	OverallSign.ABS_Coordinate.Axis23 = Encoder_Axis23.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis23;

	//need add other axis
	OverallSign.ABS_Coordinate.Axis15 = (OverallSign.MoveSign.Axis15)?(OverallSign.ABS_Coordinate.Axis15):(OverallSign.ABS_Coordinate.Axis15+System.Gap.Axis15);
	OverallSign.ABS_Coordinate.Axis16 = (OverallSign.MoveSign.Axis16)?(OverallSign.ABS_Coordinate.Axis16):(OverallSign.ABS_Coordinate.Axis16+System.Gap.Axis16);
	OverallSign.ABS_Coordinate.Axis17 = (OverallSign.MoveSign.Axis17)?(OverallSign.ABS_Coordinate.Axis17):(OverallSign.ABS_Coordinate.Axis17+System.Gap.Axis17);
	OverallSign.ABS_Coordinate.Axis18 = (OverallSign.MoveSign.Axis18)?(OverallSign.ABS_Coordinate.Axis18):(OverallSign.ABS_Coordinate.Axis18+System.Gap.Axis18);

	nonlinear_mach_value(OverallSign.ABS_Coordinate);

	#if 0
	Point_ABS_Coordinate.Axis1 = Encoder_Axis1.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis1;
	Point_ABS_Coordinate.Axis2 = Encoder_Axis2.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis2;
	Point_ABS_Coordinate.Axis3 = Encoder_Axis3.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis3;
	Point_ABS_Coordinate.Axis4 = Encoder_Axis4.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis4;
	Point_ABS_Coordinate.Axis5 = Encoder_Axis5.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis5;
	Point_ABS_Coordinate.Axis6 = Encoder_Axis6.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis6;
	Point_ABS_Coordinate.Axis7 = Encoder_Axis7.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis7;
	Point_ABS_Coordinate.Axis8 = Encoder_Axis8.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis8;
	Point_ABS_Coordinate.Axis9 = Encoder_Axis9.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis9;
	Point_ABS_Coordinate.Axis10 = Encoder_Axis10.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis10;
	Point_ABS_Coordinate.Axis11 = Encoder_Axis11.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis11;
	Point_ABS_Coordinate.Axis12 = Encoder_Axis12.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis12;
	Point_ABS_Coordinate.Axis13 = Encoder_Axis13.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis13;
	Point_ABS_Coordinate.Axis14 = Encoder_Axis14.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis14;
	Point_ABS_Coordinate.Axis15 = Encoder_Axis15.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis15;
	Point_ABS_Coordinate.Axis16 = Encoder_Axis16.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis16;
	Point_ABS_Coordinate.Axis17 = Encoder_Axis17.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis17;
	Point_ABS_Coordinate.Axis18 = Encoder_Axis18.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis18;
	Point_ABS_Coordinate.Axis19 = Encoder_Axis19.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis19;
	Point_ABS_Coordinate.Axis20 = Encoder_Axis20.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis20;
	Point_ABS_Coordinate.Axis21 = Encoder_Axis21.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis21;
	Point_ABS_Coordinate.Axis22 = Encoder_Axis22.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis22;
	Point_ABS_Coordinate.Axis23 = Encoder_Axis23.MACH_PositionValue+OverallSign.ABSORG_Coordinate.Axis23;
	
	nonlinear_mach_value(Point_ABS_Coordinate);
	#endif
	
	return 1;
}

void get_ABS_coordinate(void)
{		
	// ������֤ XYZ  ���ͬ����
	OverallSign.GetMachCoordinateRight = 1;

	if(1)	//���������
	{
		if(pre_get_MACH_counter(&Encoder_Axis1))
		{
			get_MACH_value(&Encoder_Axis1);			
		}
		
		#if 0
		if(pre_get_MACH_counter(&Encoder_Axis2))
		{
			get_MACH_value(&Encoder_Axis2);
		}
		#else
		Encoder_Axis2.MACH_PositionValue = NC_AREAControl.OldValue.Axis2;
		#endif
		
		if(pre_get_MACH_counter(&Encoder_Axis3))
		{
			get_MACH_value(&Encoder_Axis3);
		}		
		if(pre_get_MACH_counter(&Encoder_Axis4))
		{
			get_MACH_value(&Encoder_Axis4);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis5))
		{
			get_MACH_value(&Encoder_Axis5);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis6))
		{
			get_MACH_value(&Encoder_Axis6);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis7))
		{
			get_MACH_value(&Encoder_Axis7);
		}
		if(pre_get_MACH_counter(&Encoder_Axis8))
		{
			get_MACH_value(&Encoder_Axis8);
		}	
		#if 0
		if(pre_get_MACH_counter(&Encoder_Axis9))
		{
			get_MACH_value(&Encoder_Axis9);			
		}
		#endif

		if (pre_get_MACH_counter(&Encoder_Axis9))
		{
			get_MACH_value(&Encoder_Axis9);
		}

		#if 0
		if(pre_get_MACH_counter(&Encoder_Axis10))
		{
			get_MACH_value(&Encoder_Axis10);			
		}
		#else
		Encoder_Axis10.MACH_PositionValue = NC_AREAControl.OldValue.Axis10;
		#endif

		if(pre_get_MACH_counter(&Encoder_Axis11))
		{
			get_MACH_value(&Encoder_Axis11);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis12))
		{
			get_MACH_value(&Encoder_Axis12);
		}
		if(pre_get_MACH_counter(&Encoder_Axis13))
		{
			get_MACH_value(&Encoder_Axis13);
		}		
		if(pre_get_MACH_counter(&Encoder_Axis14))
		{
			get_MACH_value(&Encoder_Axis14);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis15))
		{
			get_MACH_value(&Encoder_Axis15);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis16))
		{
			get_MACH_value(&Encoder_Axis16);			
		}
		if(pre_get_MACH_counter(&Encoder_Axis17))
		{
			get_MACH_value(&Encoder_Axis17);
		}
		if(pre_get_MACH_counter(&Encoder_Axis18))
		{
			get_MACH_value(&Encoder_Axis18);
		}	
		if(pre_get_MACH_counter(&Encoder_Axis19))
		{
			get_MACH_value(&Encoder_Axis19);			
		}
		
		if(pre_get_MACH_counter(&Encoder_Axis20))
		{
			get_MACH_value(&Encoder_Axis20);			
		}

		if(pre_get_MACH_counter(&Encoder_Axis21))
		{
			get_MACH_value(&Encoder_Axis21);			
		}
		
		if(pre_get_MACH_counter(&Encoder_Axis22))
		{
			get_MACH_value(&Encoder_Axis22);			
		}
		
		if(pre_get_MACH_counter(&Encoder_Axis23))
		{
			get_MACH_value(&Encoder_Axis23);
		}
	}

	if(System.RunState == 3)
	{
		Encoder_Axis1.MACH_PositionValue = NC_AREAControl.OldValue.Axis1;
		Encoder_Axis2.MACH_PositionValue = NC_AREAControl.OldValue.Axis2;
		Encoder_Axis3.MACH_PositionValue = NC_AREAControl.OldValue.Axis3;
		Encoder_Axis4.MACH_PositionValue = NC_AREAControl.OldValue.Axis4;
		Encoder_Axis5.MACH_PositionValue = NC_AREAControl.OldValue.Axis5;
		Encoder_Axis6.MACH_PositionValue = NC_AREAControl.OldValue.Axis6;
		Encoder_Axis7.MACH_PositionValue = NC_AREAControl.OldValue.Axis7;
		Encoder_Axis8.MACH_PositionValue = NC_AREAControl.OldValue.Axis8;
		Encoder_Axis9.MACH_PositionValue = NC_AREAControl.OldValue.Axis9;
		Encoder_Axis10.MACH_PositionValue = NC_AREAControl.OldValue.Axis10;
		Encoder_Axis11.MACH_PositionValue = NC_AREAControl.OldValue.Axis11;
		Encoder_Axis12.MACH_PositionValue = NC_AREAControl.OldValue.Axis12;
		Encoder_Axis13.MACH_PositionValue = NC_AREAControl.OldValue.Axis13;
		Encoder_Axis14.MACH_PositionValue = NC_AREAControl.OldValue.Axis14;
		Encoder_Axis15.MACH_PositionValue = NC_AREAControl.OldValue.Axis15;
		Encoder_Axis16.MACH_PositionValue = NC_AREAControl.OldValue.Axis16;
		Encoder_Axis17.MACH_PositionValue = NC_AREAControl.OldValue.Axis17;
		Encoder_Axis18.MACH_PositionValue = NC_AREAControl.OldValue.Axis18;
		Encoder_Axis19.MACH_PositionValue = NC_AREAControl.OldValue.Axis19;
		Encoder_Axis20.MACH_PositionValue = NC_AREAControl.OldValue.Axis20;
		Encoder_Axis21.MACH_PositionValue = NC_AREAControl.OldValue.Axis21;
		Encoder_Axis22.MACH_PositionValue = NC_AREAControl.OldValue.Axis22;
		Encoder_Axis23.MACH_PositionValue = NC_AREAControl.OldValue.Axis23;
	}
	
	get_ABS_value();
	get_ABS_coordinate_CameraScan();	
	get_ABS_coordinate_LensPosition();
	get_ABS_coordinate_LensMotionCompensation();
	//CheckMACH_PositionValueLimit();
	CheckMACH_PositionValueLimit_PSO();
}

void CheckMACH_PositionValueLimit(void)
{
	if(System.RunState == SimulateRun)
	{
		return;
	}

	if(Reference.FindRefSign.Axis1==1)
	{
		if((OverallSign.ABS_Coordinate.Axis1  > System.SLimitPos.Axis1)||(OverallSign.ABS_Coordinate.Axis1  < System.SLimitNeg.Axis1))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis2==1)
	{
		if((OverallSign.ABS_Coordinate.Axis2  > System.SLimitPos.Axis2)||(OverallSign.ABS_Coordinate.Axis2  < System.SLimitNeg.Axis2))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis3==1)
	{
		if((OverallSign.ABS_Coordinate.Axis3  > System.SLimitPos.Axis3)||(OverallSign.ABS_Coordinate.Axis3  < System.SLimitNeg.Axis3))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis4==1)
	{
		if((OverallSign.ABS_Coordinate.Axis4  > System.SLimitPos.Axis4)||(OverallSign.ABS_Coordinate.Axis4  < System.SLimitNeg.Axis4))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis5==1)
	{
		if((OverallSign.ABS_Coordinate.Axis5  > System.SLimitPos.Axis5)||(OverallSign.ABS_Coordinate.Axis5  < System.SLimitNeg.Axis5))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis6==1)
	{
		if((OverallSign.ABS_Coordinate.Axis6  > System.SLimitPos.Axis6)||(OverallSign.ABS_Coordinate.Axis6  < System.SLimitNeg.Axis6))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis7==1)
	{
		if((OverallSign.ABS_Coordinate.Axis7  > System.SLimitPos.Axis7)||(OverallSign.ABS_Coordinate.Axis7  < System.SLimitNeg.Axis7))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis8==1)
	{
		if((OverallSign.ABS_Coordinate.Axis8  > System.SLimitPos.Axis8)||(OverallSign.ABS_Coordinate.Axis8  < System.SLimitNeg.Axis8))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis9==1)
	{
		if((OverallSign.ABS_Coordinate.Axis9  > System.SLimitPos.Axis9)||(OverallSign.ABS_Coordinate.Axis9  < System.SLimitNeg.Axis9))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis10==1)
	{
		if((OverallSign.ABS_Coordinate.Axis10  > System.SLimitPos.Axis10)||(OverallSign.ABS_Coordinate.Axis10  < System.SLimitNeg.Axis10))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis11==1)
	{
		if((OverallSign.ABS_Coordinate.Axis11  > System.SLimitPos.Axis11)||(OverallSign.ABS_Coordinate.Axis11  < System.SLimitNeg.Axis11))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis12==1)
	{
		if((OverallSign.ABS_Coordinate.Axis12  > System.SLimitPos.Axis12)||(OverallSign.ABS_Coordinate.Axis12  < System.SLimitNeg.Axis12))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis13==1)
	{
		if((OverallSign.ABS_Coordinate.Axis13  > System.SLimitPos.Axis13)||(OverallSign.ABS_Coordinate.Axis13  < System.SLimitNeg.Axis13))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis14==1)
	{
		if((OverallSign.ABS_Coordinate.Axis14  > System.SLimitPos.Axis14)||(OverallSign.ABS_Coordinate.Axis14  < System.SLimitNeg.Axis14))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis15==1)
	{
		if((OverallSign.ABS_Coordinate.Axis15  > System.SLimitPos.Axis15)||(OverallSign.ABS_Coordinate.Axis15  < System.SLimitNeg.Axis15))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis16==1)
	{
		if((OverallSign.ABS_Coordinate.Axis16  > System.SLimitPos.Axis16)||(OverallSign.ABS_Coordinate.Axis16  < System.SLimitNeg.Axis16))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis17==1)
	{
		if((OverallSign.ABS_Coordinate.Axis17  > System.SLimitPos.Axis17)||(OverallSign.ABS_Coordinate.Axis17  < System.SLimitNeg.Axis17))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis18==1)
	{
		if((OverallSign.ABS_Coordinate.Axis18  > System.SLimitPos.Axis18)||(OverallSign.ABS_Coordinate.Axis18  < System.SLimitNeg.Axis18))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis19==1)
	{
		if((OverallSign.ABS_Coordinate.Axis19  > System.SLimitPos.Axis19)||(OverallSign.ABS_Coordinate.Axis19  < System.SLimitNeg.Axis19))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis20==1)
	{
		if((OverallSign.ABS_Coordinate.Axis20  > System.SLimitPos.Axis20)||(OverallSign.ABS_Coordinate.Axis20  < System.SLimitNeg.Axis20))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis21==1)
	{
		if((OverallSign.ABS_Coordinate.Axis21  > System.SLimitPos.Axis21)||(OverallSign.ABS_Coordinate.Axis21  < System.SLimitNeg.Axis21))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis22==1)
	{
		if((OverallSign.ABS_Coordinate.Axis22  > System.SLimitPos.Axis22)||(OverallSign.ABS_Coordinate.Axis22  < System.SLimitNeg.Axis22))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
	if(Reference.FindRefSign.Axis23==1)
	{
		if((OverallSign.ABS_Coordinate.Axis23  > System.SLimitPos.Axis23)||(OverallSign.ABS_Coordinate.Axis23  < System.SLimitNeg.Axis23))
		{
			Error.InterpolationOver = 1;
			high_error();		
		}
	}
}

void CheckMACH_PositionValueLimit_PSO(void)
{
	static Uint8 CheckSoftLimit_cycleCounter = 30;

	if(System.RunState == SimulateRun)
	{
		return;
	}

	if(Reference.FindRefSign.Axis1==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis1  > System.SLimitPos.Axis1)||(OverallSign.AbsoluteEncoderValue_PSO.Axis1  < System.SLimitNeg.Axis1))
		{
			CheckSoftLimit_cycle.Axis1++;
			if (CheckSoftLimit_cycle.Axis1 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis1  > System.SLimitPos.Axis1)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis1 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis1 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis1  < System.SLimitNeg.Axis1)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis1 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis1 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis1 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis1 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis1 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis1 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis1 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis1 = 0;
	}
	if(Reference.FindRefSign.Axis2==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis2  > System.SLimitPos.Axis2)||(OverallSign.AbsoluteEncoderValue_PSO.Axis2  < System.SLimitNeg.Axis2))
		{
			CheckSoftLimit_cycle.Axis2++;
			if (CheckSoftLimit_cycle.Axis2 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis2  > System.SLimitPos.Axis2)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis2 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis2 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis2  < System.SLimitNeg.Axis2)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis2 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis2 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis2 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis2 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis2 = 0;
		}

	}
	else 
	{
		CheckSoftLimit_cycle.Axis2 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis2 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis2 = 0;
	}
	if(Reference.FindRefSign.Axis3==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis3  > System.SLimitPos.Axis3)||(OverallSign.AbsoluteEncoderValue_PSO.Axis3  < System.SLimitNeg.Axis3))
		{
			CheckSoftLimit_cycle.Axis3++;
			if (CheckSoftLimit_cycle.Axis3 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis3  > System.SLimitPos.Axis3)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis3 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis3 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis3  < System.SLimitNeg.Axis3)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis3 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis3 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis3 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis3 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis3 = 0;
		}

	}
	else 
	{
		CheckSoftLimit_cycle.Axis3 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis3 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis3 = 0;
	}
	if(Reference.FindRefSign.Axis4==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis4  > System.SLimitPos.Axis4)||(OverallSign.AbsoluteEncoderValue_PSO.Axis4  < System.SLimitNeg.Axis4))
		{
			CheckSoftLimit_cycle.Axis4++;
			if (CheckSoftLimit_cycle.Axis4 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis4  > System.SLimitPos.Axis4)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis4 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis4 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis4  < System.SLimitNeg.Axis4)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis4 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis4 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis4 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis4 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis4 = 0;
		}

	}
	else 
	{
		CheckSoftLimit_cycle.Axis4 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis4 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis4 = 0;
	}
	if(Reference.FindRefSign.Axis5==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis5  > System.SLimitPos.Axis5)||(OverallSign.AbsoluteEncoderValue_PSO.Axis5  < System.SLimitNeg.Axis5))
		{
			CheckSoftLimit_cycle.Axis5++;
			if (CheckSoftLimit_cycle.Axis5 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis5  > System.SLimitPos.Axis5)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis5 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis5 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis5  < System.SLimitNeg.Axis5)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis5 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis5 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis5 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis5 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis5 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis5 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis5 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis5 = 0;
	}
	if(Reference.FindRefSign.Axis6==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis6  > System.SLimitPos.Axis6)||(OverallSign.AbsoluteEncoderValue_PSO.Axis6  < System.SLimitNeg.Axis6))
		{
			CheckSoftLimit_cycle.Axis6++;
			if (CheckSoftLimit_cycle.Axis6 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis6  > System.SLimitPos.Axis6)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis6 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis6 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis6  < System.SLimitNeg.Axis6)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis6 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis6 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis6 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis6 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis6 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis6 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis6 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis6 = 0;
	}
	if(Reference.FindRefSign.Axis7==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis7  > System.SLimitPos.Axis7)||(OverallSign.AbsoluteEncoderValue_PSO.Axis7  < System.SLimitNeg.Axis7))
		{
			CheckSoftLimit_cycle.Axis7++;
			if (CheckSoftLimit_cycle.Axis7 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis7  > System.SLimitPos.Axis7)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis7 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis7 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis7  < System.SLimitNeg.Axis7)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis7 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis7 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis7 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis7 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis7 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis7 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis7 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis7 = 0;
	}
	if(Reference.FindRefSign.Axis8==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis8  > System.SLimitPos.Axis8)||(OverallSign.AbsoluteEncoderValue_PSO.Axis8  < System.SLimitNeg.Axis8))
		{
			CheckSoftLimit_cycle.Axis8++;
			if (CheckSoftLimit_cycle.Axis8 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis8  > System.SLimitPos.Axis8)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis8 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis8 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis8  < System.SLimitNeg.Axis8)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis8 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis8 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis8 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis8 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis8 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis8 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis8 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis8 = 0;
	}
	if(Reference.FindRefSign.Axis9==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis9  > System.SLimitPos.Axis9)||(OverallSign.AbsoluteEncoderValue_PSO.Axis9  < System.SLimitNeg.Axis9))
		{
			CheckSoftLimit_cycle.Axis9++;
			if (CheckSoftLimit_cycle.Axis9 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis9  > System.SLimitPos.Axis9)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis9 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis9 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis9  < System.SLimitNeg.Axis9)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis9 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis9 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis9 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis9 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis9 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis9 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis9 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis9 = 0;
	}
	if(Reference.FindRefSign.Axis10==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis10  > System.SLimitPos.Axis10)||(OverallSign.AbsoluteEncoderValue_PSO.Axis10  < System.SLimitNeg.Axis10))
		{
			CheckSoftLimit_cycle.Axis10++;
			if (CheckSoftLimit_cycle.Axis10 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis10  > System.SLimitPos.Axis10)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis10 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis10 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis10  < System.SLimitNeg.Axis10)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis10 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis10 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis10 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis10 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis10 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis10 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis10 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis10 = 0;
	}
	if(Reference.FindRefSign.Axis11==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis11  > System.SLimitPos.Axis11)||(OverallSign.AbsoluteEncoderValue_PSO.Axis11  < System.SLimitNeg.Axis11))
		{
			CheckSoftLimit_cycle.Axis11++;
			if (CheckSoftLimit_cycle.Axis11 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis11  > System.SLimitPos.Axis11)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis11 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis11 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis11  < System.SLimitNeg.Axis11)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis11 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis11 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis11 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis11 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis11 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis11 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis11 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis11 = 0;
	}
	if(Reference.FindRefSign.Axis12==1)
	{
		if((OverallSign.ABS_Coordinate.Axis12  > System.SLimitPos.Axis12)||(OverallSign.ABS_Coordinate.Axis12  < System.SLimitNeg.Axis12))
		//if((OverallSign.AbsoluteEncoderValue_PSO.Axis12  > System.SLimitPos.Axis12)||(OverallSign.AbsoluteEncoderValue_PSO.Axis12  < System.SLimitNeg.Axis12))
		{
			CheckSoftLimit_cycle.Axis12++;
			if (CheckSoftLimit_cycle.Axis12 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis12  > System.SLimitPos.Axis12)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis12 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis12= 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis12  < System.SLimitNeg.Axis12)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis12 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis12 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis12 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis12 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis12 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis12 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis12 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis12 = 0;
	}
	if(Reference.FindRefSign.Axis13==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis13  > System.SLimitPos.Axis13)||(OverallSign.AbsoluteEncoderValue_PSO.Axis13  < System.SLimitNeg.Axis13))
		{
			CheckSoftLimit_cycle.Axis13++;
			if (CheckSoftLimit_cycle.Axis13 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis13  > System.SLimitPos.Axis13)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis13 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis13 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis13  < System.SLimitNeg.Axis13)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis13 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis13 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis13 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis13 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis13 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis13 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis13 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis13 = 0;
	}
	if(Reference.FindRefSign.Axis14==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis14  > System.SLimitPos.Axis14)||(OverallSign.AbsoluteEncoderValue_PSO.Axis14  < System.SLimitNeg.Axis14))
		{
			CheckSoftLimit_cycle.Axis14++;
			if (CheckSoftLimit_cycle.Axis14 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis14  > System.SLimitPos.Axis14)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis14 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis14 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis14  < System.SLimitNeg.Axis14)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis14 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis14 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis14 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis14 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis14 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis14 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis14 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis14 = 0;
	}
	if(Reference.FindRefSign.Axis15==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis15  > System.SLimitPos.Axis15)||(OverallSign.AbsoluteEncoderValue_PSO.Axis15  < System.SLimitNeg.Axis15))
		{
			CheckSoftLimit_cycle.Axis15++;
			if (CheckSoftLimit_cycle.Axis15 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis15  > System.SLimitPos.Axis15)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis15 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis15 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis15  < System.SLimitNeg.Axis15)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis15 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis15 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis15 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis15 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis15 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis15 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis15 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis15 = 0;
	}
	if(Reference.FindRefSign.Axis16==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis16  > System.SLimitPos.Axis16)||(OverallSign.AbsoluteEncoderValue_PSO.Axis16  < System.SLimitNeg.Axis16))
		{
			CheckSoftLimit_cycle.Axis16++;
			if (CheckSoftLimit_cycle.Axis16 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis16  > System.SLimitPos.Axis16)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis16 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis16 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis16  < System.SLimitNeg.Axis16)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis16 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis16 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis16 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis16 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis16 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis16 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis16 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis16 = 0;
	}
	if(Reference.FindRefSign.Axis17==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis17  > System.SLimitPos.Axis17)||(OverallSign.AbsoluteEncoderValue_PSO.Axis17  < System.SLimitNeg.Axis17))
		{
			CheckSoftLimit_cycle.Axis17++;
			if (CheckSoftLimit_cycle.Axis17 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis17  > System.SLimitPos.Axis17)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis17 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis17 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis17  < System.SLimitNeg.Axis17)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis17 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis17 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis17 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis17 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis17 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis17 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis17 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis17 = 0;
	}
	if(Reference.FindRefSign.Axis18==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis18  > System.SLimitPos.Axis18)||(OverallSign.AbsoluteEncoderValue_PSO.Axis18  < System.SLimitNeg.Axis18))
		{
			CheckSoftLimit_cycle.Axis18++;
			if (CheckSoftLimit_cycle.Axis18 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis18  > System.SLimitPos.Axis18)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis18 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis18 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis18  < System.SLimitNeg.Axis18)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis18 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis18 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis18 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis18 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis18 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis18 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis18 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis18 = 0;
	}
	if(Reference.FindRefSign.Axis19==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis19  > System.SLimitPos.Axis19)||(OverallSign.AbsoluteEncoderValue_PSO.Axis19  < System.SLimitNeg.Axis19))
		{
			CheckSoftLimit_cycle.Axis19++;
			if (CheckSoftLimit_cycle.Axis19 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis19  > System.SLimitPos.Axis19)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis19 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis19 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis19  < System.SLimitNeg.Axis19)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis19 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis19 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis19 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis19 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis19 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis19 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis19 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis19 = 0;
	}
	if(Reference.FindRefSign.Axis20==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis20  > System.SLimitPos.Axis20)||(OverallSign.AbsoluteEncoderValue_PSO.Axis20  < System.SLimitNeg.Axis20))
		{
			CheckSoftLimit_cycle.Axis20++;
			if (CheckSoftLimit_cycle.Axis20 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis20  > System.SLimitPos.Axis20)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis20 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis20 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis20  < System.SLimitNeg.Axis20)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis20 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis20 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis20 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis20 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis20 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis20 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis20 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis20 = 0;
	}
	if(Reference.FindRefSign.Axis21==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis21  > System.SLimitPos.Axis21)||(OverallSign.AbsoluteEncoderValue_PSO.Axis21  < System.SLimitNeg.Axis21))
		{
			CheckSoftLimit_cycle.Axis21++;
			if (CheckSoftLimit_cycle.Axis21 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis21  > System.SLimitPos.Axis21)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis21 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis21 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis21  < System.SLimitNeg.Axis21)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis21 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis21 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis21 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis21 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis21 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis21 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis21 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis21 = 0;
	}
			
	if(Reference.FindRefSign.Axis22==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis22  > System.SLimitPos.Axis22)||(OverallSign.AbsoluteEncoderValue_PSO.Axis22  < System.SLimitNeg.Axis22))
		{
			CheckSoftLimit_cycle.Axis22++;
			if (CheckSoftLimit_cycle.Axis22 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis22  > System.SLimitPos.Axis22)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis22 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis22 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis22  < System.SLimitNeg.Axis22)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis22 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis22 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis22 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis22 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis22 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis22 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis22 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis22 = 0;
	}
	
	if(Reference.FindRefSign.Axis23==1)
	{
		if((OverallSign.AbsoluteEncoderValue_PSO.Axis23  > System.SLimitPos.Axis23)||(OverallSign.AbsoluteEncoderValue_PSO.Axis23  < System.SLimitNeg.Axis23))
		{
			CheckSoftLimit_cycle.Axis23++;
			if (CheckSoftLimit_cycle.Axis23 > CheckSoftLimit_cycleCounter) 
			{
				if(OverallSign.AbsoluteEncoderValue_PSO.Axis23  > System.SLimitPos.Axis23)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis23 = 1;
					psDSPPacketOut_InfoEvent->SLimitNegAxis23 = 0;
					Error.AlmSLimitPos = 1;
					//high_error();
				}
				else if(OverallSign.AbsoluteEncoderValue_PSO.Axis23  < System.SLimitNeg.Axis23)
				{
					psDSPPacketOut_InfoEvent->SLimitPosAxis23 = 0;
					psDSPPacketOut_InfoEvent->SLimitNegAxis23 = 1;
					Error.AlmSLimitNeg = 1;
					//high_error();
				}
			}
		} 
		else 
		{
			CheckSoftLimit_cycle.Axis23 = 0;
			psDSPPacketOut_InfoEvent->SLimitPosAxis23 = 0;
			psDSPPacketOut_InfoEvent->SLimitNegAxis23 = 0;
		}
	}
	else 
	{
		CheckSoftLimit_cycle.Axis23 = 0;
		psDSPPacketOut_InfoEvent->SLimitPosAxis23 = 0;
		psDSPPacketOut_InfoEvent->SLimitNegAxis23 = 0;
	}
}


void get_ABS_coordinate_CameraScan(void)
{
	static float64 CaptureTestTime1 = 12;//10ms start open camera shutter
	static float64 CaptureTestTime2 = 13;//10ms start open camera shutter
	static float64 CaptureTestTime3 = 15;//12ms start open camera shutter rising edge trigger
	float CaptureTestTime4;
	Int32	PositionValue_Delta = 5;//5 um
	Int32	PositionValue_Delta_1 = 5;//5 um
	Int32	PositionValueModOffset;
	float64	PositionValueDivideOffset;
	static Int32 MonitorBuffer1Monitor = 288;
	static float64	DivideOffsetValue = 0.4;

	if(OverallSign.CameraScanSign == 0)
	{
		return;
	}

	if(System.FunctionSelect06 == 1)
	{
		return;
	}

	PositionValueDivideOffsetRef = divdp(NC_AREAControl.Stotal_Module.Module2*1000,System.OffsetCoordinate.Axis17)-DivideOffsetValue;

	if(PositionValueDivideOffsetRef < 2.0)
	{
		return;
	}

	if((OverallSign.NCSign ==STDCODERUN)&&(NCRunSign.RealSendCount > 1))
	{
		PositionValue_CameraCapture.Axis17 = OverallSign.ABS_Coordinate.Axis17 - System.PositionCoordinate1.Axis17;
		PositionValue_CameraCapture.Axis18 = OverallSign.ABS_Coordinate.Axis18 - System.PositionCoordinate1.Axis18;
		//PositionValue_CameraCapture.Axis17 = OverallSign.AbsoluteEncoderValue_PSO.Axis17 - System.PositionCoordinate1.Axis17;
		//PositionValue_CameraCapture.Axis18 = OverallSign.AbsoluteEncoderValue_PSO.Axis18 - System.PositionCoordinate1.Axis18;
		
		PositionValueDivideOffset = divdp(PositionValue_CameraCapture.Axis17,System.OffsetCoordinate.Axis17);

		if((PositionValueDivideOffset < 0.4)||(PositionValueDivideOffset >PositionValueDivideOffsetRef))
		{
			PositionValueDivideOffsetRef = PositionValueDivideOffsetRef;
			return;
		}
		
		CaptureTestTime4 = divdp(System.LEDFlashTime1,1000);

		PositionValueModOffset = PositionValue_CameraCapture.Axis17%System.OffsetCoordinate.Axis17;

		if(OverallSign.MoveSign.Axis17)
		{			
			if(OverallSign.CameraNeedCaptureSign == 1)
			{				
				if(PositionValueModOffset > (System.OffsetCoordinate.Axis17 - CaptureTestTime1*NC_AREAControl.VmReal_Module.Module2))
				{
					if(Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 == 0)
					{
						Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 1; //for trigger camera capture(open)
						Write_Servo_Sign1_Out();
						OverallSign.CameraNeedCaptureSign = 0;	

						MonitorBuffer2[MonitorBuffer2Cnt] = PositionValue_CameraCapture.Axis17;
						
						MonitorBuffer2Cnt++;
						if(MonitorBuffer2Cnt > 1000)
						{
							MonitorBuffer2Cnt = 0;
						}
					}
				}
			}
			else
			{
				//if(PositionValueModOffset > (System.OffsetCoordinate.Axis17 - CaptureTestTime3*NC_AREAControl.VmReal_Module.Module2))
				if((PositionValueModOffset > (System.OffsetCoordinate.Axis17 - CaptureTestTime3*NC_AREAControl.VmReal_Module.Module2))&&
					(PositionValueModOffset < (System.OffsetCoordinate.Axis17 - CaptureTestTime2*NC_AREAControl.VmReal_Module.Module2)))
				{
					if(Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 == 1)
					{							
						OverallSign.CameraNeedCaptureSign = 1;
						Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 0; //for trigger camera capture(close)
						Write_Servo_Sign1_Out();
					}
				}
			}
		}
		else if(OverallSign.MoveSign.Axis17 == 0)
		{
			if(OverallSign.CameraNeedCaptureSign == 1)
			{			
				if(PositionValueModOffset < (CaptureTestTime1*NC_AREAControl.VmReal_Module.Module2))
				{
					if(Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 == 0)
					{
						Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= 1; //for trigger camera capture(open)
						Write_Servo_Sign1_Out();
						OverallSign.CameraNeedCaptureSign = 0;
						MonitorBuffer2[MonitorBuffer2Cnt] = PositionValue_CameraCapture.Axis17;
						
						MonitorBuffer2Cnt++;
						if(MonitorBuffer2Cnt > 1000)
						{
							MonitorBuffer2Cnt = 0;
						}
					}
				}
			}
			else 
			{
				//if(PositionValueModOffset < (CaptureTestTime3*NC_AREAControl.VmReal_Module.Module2))
				if((PositionValueModOffset < (CaptureTestTime3*NC_AREAControl.VmReal_Module.Module2))&&
					(PositionValueModOffset > (CaptureTestTime2*NC_AREAControl.VmReal_Module.Module2)))
				{
					if(Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 == 1)
					{							
						OverallSign.CameraNeedCaptureSign = 1;
						Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 0; //for trigger camera capture(close)
						Write_Servo_Sign1_Out();				
					}
				}
			}
		}
	}		
}

void get_ABS_coordinate_MicroEScan(void)
{
	if(OverallSign.MicroEScanSign == 0)
	{
		return;
	}		
}


void get_ABS_coordinate_LensPosition(void)
{
	float64AXIS	LensPositionDivideStepper;
	static float64	LensPositionOffsetRatio = 0.4;
	INT40AXIS 	LensAbsCoordinate;
	Int32		LensPositionDivideStepperX_Int32,LensPositionDivideStepperY_Int32;
	Int32		DeltaStepAxis19;
	static Int32	LensPositionMaxOffset = 10;
	static Uint16 	LensPositionStaticCounter = 2;
	static Uint16 	LensPositionStaticCounter_2 = 2;

	if(System.FunctionSelect03 != 1)
	{
		DeltaStepAxis19_Bak = 0;
		return;
	}
	
	if(OverallSign.CameraScanSign == 0)
	{
		DeltaStepAxis19_Bak = 0;
		return;
	}
	if(LensCompensationBufferCnt == 0)
	{
		DeltaStepAxis19_Bak = 0;
		return;
	}
	
	if(psDSPPacketOut_InfoEvent->AxisRefOverValidAll == False)
	{
		DeltaStepAxis19_Bak = 0;
		return;
	}
	
	OverallSign.LensMotionOverSign = LensMotionNormal;
	
	if(OverallSign.NCSign ==STDCODERUN)
	{
		DeltaStepAxis19 = 0;
		
		//LensAbsCoordinate.Axis17 = Encoder_Axis17.MACH_PositionValue - System.PositionCoordinate1.Axis17;
		//LensAbsCoordinate.Axis18 = Encoder_Axis18.MACH_PositionValue - System.PositionCoordinate1.Axis18;
		LensAbsCoordinate.Axis17 = OverallSign.ABS_Coordinate.Axis17 - System.PositionCoordinate1.Axis17;
		LensAbsCoordinate.Axis18 = OverallSign.ABS_Coordinate.Axis18 - System.PositionCoordinate1.Axis18;
	
		LensPositionDivideStepper.Axis17 = divdp(LensAbsCoordinate.Axis17,System.OffsetCoordinate.Axis17);
		LensPositionDivideStepper.Axis18 = divdp(LensAbsCoordinate.Axis18,System.OffsetCoordinate.Axis18);

		CameraScanStepperTotalNumber_X = divdp(NC_AREAControl.Stotal_Module.Module2*1000,System.OffsetCoordinate.Axis17)-LensPositionOffsetRatio;

		//if(LensPositionDivideStepper.Axis17 < 0.4)
		if(LensPositionDivideStepper.Axis17 < 0.2)
		{
			DeltaStepAxis19_Bak = 0;
			return;
		}

		if(CameraScanStepperTotalNumber_X < 2.0)
		{
			DeltaStepAxis19_Bak = 0;
			return;
		}
		
		LensPositionDivideStepperX_Int32 = (Int32)(LensPositionDivideStepper.Axis17+0.9);
		LensPositionDivideStepperY_Int32 = (Int32)(LensPositionDivideStepper.Axis18+0.9);

		if(OverallSign.MoveSign.Axis17 == 1)
		{
			PositionPointerCnt = LensPositionDivideStepperX_Int32+((Int32)(CameraScanStepperTotalNumber_X))*LensPositionDivideStepperY_Int32;
		}
		else if(OverallSign.MoveSign.Axis17 == 0)
		{
			PositionPointerCnt = (((Int32)(CameraScanStepperTotalNumber_X+0.6))-LensPositionDivideStepperX_Int32)+((Int32)(CameraScanStepperTotalNumber_X))*LensPositionDivideStepperY_Int32;
		}

		if((PositionPointerCnt - PositionPointerCnt_Bak) > 1)
		{
			Error.InterpolationOver = Error.InterpolationOver;
			//DeltaStepAxis19_Bak = 0;
			//Error.LensPositionBreakError = 1;
			//high_error();		//CH0904
			return;
		}
		
		if(PositionPointerCnt < 0)
		{
			DeltaStepAxis19_Bak = 0;
			Error.LensPositionBreakError = 1;
			high_error();		//CH0904
			return;
		}
		
		if(PositionPointerCnt > (LensCompensationBufferCnt))
		{
			PositionPointerCnt = LensCompensationBufferCnt;
		}
		
		if(PositionPointerCnt>0)
		{
			//if(PositionPointerCnt_Bak >= PositionPointerCnt)
			if(PositionPointerCnt_Bak > PositionPointerCnt)
			{
				Encoder_Axis17.MACH_PositionValue = Encoder_Axis17.MACH_PositionValue;
				return;
			}
		}
		
		if(PositionPointerCnt >0)
		{
			//DeltaStepU = (LensCompensationBuffer[PositionPointerCnt-1] -Encoder_Axis23.MACH_PositionValue)*2;
			//DeltaStepAxis19 = (LensCompensationBuffer[PositionPointerCnt-1] -Encoder_Axis23.MACH_PositionValue);
			//DeltaStepAxis19 = (LensCompensationBuffer[PositionPointerCnt-1] -OverallSign.ABS_Coordinate.Axis19);
			DeltaStepAxis19 = (LensCompensationBuffer[PositionPointerCnt-1] -OverallSign.ABS_Coordinate.Axis19)*Servo_Axis19.AxisOutUnitEQU;//Servo_Axis19.AxisOutUnitEQU=2.0;1000setp

			//added 20170301
			if(fabs(DeltaStepAxis19) < LensPositionMaxOffset)
			{
				LensPositionOverCounter++;
				if(LensPositionOverCounter < LensPositionStaticCounter)
				{
					DeltaStepAxis19_Bak = 0;
					PositionPointerCnt_Bak = PositionPointerCnt;
					return;
				}	
				LensPositionOverCounter = 0;

				DeltaStepAxis19_Bak= DeltaStepAxis19;
				if(DeltaStepAxis19_Bak > 2)
				{
					DeltaStepAxis19_Bak = 2;
				}
				else if(DeltaStepAxis19_Bak < -2)
				{
					DeltaStepAxis19_Bak = -2;
				}
			}
			else
			{			
				DeltaStepAxis19_Bak= DeltaStepAxis19;
				if(DeltaStepAxis19_Bak > 4)
				{
					DeltaStepAxis19_Bak = 4;
				}
				else if(DeltaStepAxis19_Bak < -4)
				{
					DeltaStepAxis19_Bak = -4;
				}
			}
			
		}
		
		if(DeltaStepAxis19 > 0)
		{
			OverallSign.MoveSign.Axis19 = 1;
		}
		else
		{
			OverallSign.MoveSign.Axis19 = 0;
		}

		PositionPointerCnt_Bak = PositionPointerCnt;
	
		MonitorBuffer3[PositionPointerCnt_Bak%1000]=OverallSign.ABS_Coordinate.Axis19;		
	}		
}

void get_ABS_coordinate_LensMotionCompensation(void)
{
	static Uint16 LensMotionCompensationStaticCounter = 5;
	static Uint16 LensMotionCompensationStaticCounter_2 = 6;
	static Int16	LensMotionDistancePOS = 2;
	static Int16	LensMotionDistanceNEG = -2;
	static float64	AxisOutUnitEQU=2;
	static float64	AxisOutUnitEQU_2=2;
	static Int16	LensMotionDistance = 3;	
	static Int16	LensMotionMaxDistance = 15;	
	static Uint16	Int4IdleSign_1 = 6;
	
	if(System.FunctionSelect04 != 1)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}
	if(OverallSign.CameraScanSign != 0)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}
	
	#if 1
	#if 0
	if(psDSPPacketOut_InfoEvent->AxisRefOverValidAll == False)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}

	if(Reference.SearchRefSign == 1)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}
	#endif
	
	if(Reference.RefStep < 4)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}
	
	LensMotionOverCounter_2++;
	if(LensMotionOverCounter_2 < LensMotionCompensationStaticCounter_2)
	{
		DeltaStepAxis19_CPS = 0;
		return;
	}
	LensMotionOverCounter_2 = 0;

	if(OverallSign.LensMotionOverSign == LensMotionCompensationRun)
	{
		//if(OverallSign.InterpolationRunCnt_1 == OverallSign.InterpolationSendCnt_1)
		if(1)
		{	
			DeltaStepAxis19_CPS = (NC_AREAControl.NowValue.Axis19-OverallSign.ABS_Coordinate.Axis19);
			//DeltaStepAxis19_CPS = (NC_AREAControl.OldValue.Axis19-OverallSign.ABS_Coordinate.Axis19);			
				
			if(DeltaStepAxis19_CPS != 0)
			{		
				if(DeltaStepAxis19_CPS > LensMotionDistancePOS)
				{
					DeltaStepAxis19_CPS = LensMotionDistancePOS;
					LensMotionOverCounter = 0;
				}
				else if(DeltaStepAxis19_CPS < LensMotionDistanceNEG)
				{
					DeltaStepAxis19_CPS = LensMotionDistanceNEG;
					LensMotionOverCounter = 0;
				}
				else
				{
					//DeltaStepAxis19_CPS /= AxisOutUnitEQU_2;
					if(DeltaStepAxis19_CPS == 0)
					{
						LensMotionOverCounter++;
						if(LensMotionOverCounter > LensMotionCompensationStaticCounter)
						{
							OverallSign.LensMotionOverSign = LensMotionCompensationOver;
						}
					}
					else
					{
						LensMotionOverCounter = 0;
					}
				}
			}
			else
			{
				LensMotionOverCounter++;
				if(LensMotionOverCounter > LensMotionCompensationStaticCounter)
				{
					OverallSign.LensMotionOverSign = LensMotionCompensationOver;
				}			
			}
			
			if(OverallSign.Int4IdleSign_1>Int4IdleSign_1)
			{			
				IRQ_ENABLE(IRQNUM_PLUSE1);	
				IRQ_ENABLE(IRQNUM_PLUSE3);		
				//*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<22);
				//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1<<22);
				*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = (0x1<<17);
				*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0) = (0x1<<17);
				*(GPIO_IRQStatus.GPIO1_IRQStatus_1) = (0x1<<17);
				*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_1) = (0x1<<17);
			}
		}		
		else
		{
			DeltaStepAxis19_CPS = 0;
			LensMotionOverCounter = 0;
		}
	}
	else
	{
		DeltaStepAxis19_CPS = 0;
		LensMotionOverCounter = 0;
	}
	#endif
}




