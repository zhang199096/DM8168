#include "..\\include\\struct.h"
#include "..\\include\\sci.h"
#include "..\\include\\sharemem.h"
#include "..\\include\\cputimers.h"
#include "..\\include\\common.h"
#include "..\\include\\uart.h"
#include "..\\include\\cpld.h"
#include "..\\include\\run_nc.h"
#include "..\\include\\area_control.h"
#include "..\\include\\main_dsp.h"

#include <string.h>

#define mbWHEEL_AXIS	(0x1F)
#define mbWHEEL_GRADE	(0x1E0)

#define SCI_SPCDEBUG	0

#if SCI_SPCDEBUG
INT32U SPCFeedIndex;
INT32U SPCFeedBuf[512];
INT32U SPCPulseBuf[512];
INT32U SPCTimeBuf[512];
INT32U SPCRunBuf[512];
#endif		

Uint32 SCIA_TxBuf[4096];	//SCIAÊµï¿½Ê·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
Uint32 SCIA_RxBuf[4096];	//SCIAÊµï¿½Ê½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
INT32S LensCompensationBuffer[8000];	
Uint16 LensCompensationBufferCnt = 0;
Uint16 ServoOn2Counter=0;

tsSCIPARA sSCIPara;

INT8U KeyBD3203JCoderList[] = { 0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9,
		11, 10, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 19, 18, 23, 22, 20, 21 };

INT16U halfCRC[16] = { 0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306,
		0x7387, 0x8408, 0x9489, 0xa50a, 0xb58b, 0xc60c, 0xd68d, 0xe70e, 0xf78f };

INT16U Packet_CRCGet(INT8U *ptr) {
	INT16U len;
	INT16U crc;
	INT8U da;
	crc = 0;
	len = *ptr + (*(ptr + 1) << 8);
	while (len--) {
		da = (crc & 0xf);
		crc >>= 4;
		crc ^= halfCRC[da ^ (*ptr & 0x0f)];
		da = (crc & 0xf);
		crc >>= 4;
		crc ^= halfCRC[da ^ (*ptr >> 4)];
		ptr++;
	}
	return crc;
}

void Packet_CRCMake(INT8U *ptr) {
	INT16U len;
	INT16U crc;
	INT8U da;
	crc = 0;
	len = *ptr + (*(ptr + 1) << 8);
	while (len--) {
		da = (crc & 0xf);
		crc >>= 4;
		crc ^= halfCRC[da ^ (*ptr & 0x0f)];
		da = (crc & 0xf);
		crc >>= 4;
		crc ^= halfCRC[da ^ (*ptr >> 4)];
		ptr++;
	}
	*ptr++ = crc;
	*ptr++ = crc >> 8;
	*ptr++ = 0xBB;
}

INT16U Packet_CRC16Get(INT8U *pData, INT16U Length) {
	INT16U i;
	INT16U CRCResult = 0xffff;
	while (Length--) {
		CRCResult ^= *pData++;
		for (i = 0; i < 8; i++) {
			if (CRCResult & 0x01)
				CRCResult = (CRCResult >> 1) ^ 0xa001;
			else
				CRCResult = CRCResult >> 1;
		}
	}
	return (CRCResult);
}

void Dsp_Init(void) {
	Struct_Init();
}

void InitCOM232_1(void)
{
	static Uint8 TestCOM1Sign=0;
	#if 0
	if(TestCOM1Sign == 0)
	{
		* COM_OUT_FIFO_Clear = 0;
		* COM_OUT_FIFO_Clear = 0xffff;
		* COM_IN_FIFO_Clear = 0;
		* COM_IN_FIFO_Clear = 0xffff;
	}
	
	//57 02 AA 50(0x0D,0x0a)
	//57 03 AA 50
	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x02;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;
	
	*WRITE_RS232_2 = 0x57;
	*WRITE_RS232_2 = 0x02;
	*WRITE_RS232_2 = 0xAA;
	*WRITE_RS232_2 = 0x50;

	*WRITE_RS232_3 = 0x57;
	*WRITE_RS232_3 = 0x02;
	*WRITE_RS232_3 = 0xAA;
	*WRITE_RS232_3 = 0x50;

	*WRITE_RS232_4 = 0x57;
	*WRITE_RS232_4 = 0x02;
	*WRITE_RS232_4 = 0xAA;
	*WRITE_RS232_4 = 0x50;
	
	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x03;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;	

	*WRITE_RS232_2 = 0x57;
	*WRITE_RS232_2 = 0x03;
	*WRITE_RS232_2 = 0xAA;
	*WRITE_RS232_2 = 0x50;	

	*WRITE_RS232_3 = 0x57;
	*WRITE_RS232_3 = 0x03;
	*WRITE_RS232_3 = 0xAA;
	*WRITE_RS232_3 = 0x50;

	*WRITE_RS232_4 = 0x57;
	*WRITE_RS232_4 = 0x03;
	*WRITE_RS232_4 = 0xAA;
	*WRITE_RS232_4 = 0x50;
	
	*WRITE_GPIO_UART0 = 0x57;
	*WRITE_GPIO_UART0 = 0x02;
	*WRITE_GPIO_UART0 = 0xAA;
	*WRITE_GPIO_UART0 = 0x50;	
	*WRITE_GPIO_UART0 = 0x57;
	*WRITE_GPIO_UART0 = 0x03;
	*WRITE_GPIO_UART0 = 0xAA;
	*WRITE_GPIO_UART0 = 0x50;	

	*WRITE_GPIO_UART1 = 0x57;
	*WRITE_GPIO_UART1 = 0x02;
	*WRITE_GPIO_UART1 = 0xAA;
	*WRITE_GPIO_UART1 = 0x50;	
	*WRITE_GPIO_UART1 = 0x57;
	*WRITE_GPIO_UART1 = 0x03;
	*WRITE_GPIO_UART1 = 0xAA;
	*WRITE_GPIO_UART1 = 0x50;	

	*WRITE_GPIO_UART2 = 0x57;
	*WRITE_GPIO_UART2 = 0x02;
	*WRITE_GPIO_UART2 = 0xAA;
	*WRITE_GPIO_UART2 = 0x50;	
	*WRITE_GPIO_UART2 = 0x57;
	*WRITE_GPIO_UART2 = 0x03;
	*WRITE_GPIO_UART2 = 0xAA;
	*WRITE_GPIO_UART2 = 0x50;	

	*WRITE_GPIO_UART3 = 0x57;
	*WRITE_GPIO_UART3 = 0x02;
	*WRITE_GPIO_UART3 = 0xAA;
	*WRITE_GPIO_UART3 = 0x50;	
	*WRITE_GPIO_UART3 = 0x57;
	*WRITE_GPIO_UART3 = 0x03;
	*WRITE_GPIO_UART3 = 0xAA;
	*WRITE_GPIO_UART3 = 0x50;	

	*WRITE_GPIO_UART4 = 0x57;
	*WRITE_GPIO_UART4 = 0x02;
	*WRITE_GPIO_UART4 = 0xAA;
	*WRITE_GPIO_UART4 = 0x50;	
	*WRITE_GPIO_UART4 = 0x57;
	*WRITE_GPIO_UART4 = 0x03;
	*WRITE_GPIO_UART4 = 0xAA;
	*WRITE_GPIO_UART4 = 0x50;	

	*WRITE_GPIO_UART5 = 0x57;
	*WRITE_GPIO_UART5 = 0x02;
	*WRITE_GPIO_UART5 = 0xAA;
	*WRITE_GPIO_UART5 = 0x50;	
	*WRITE_GPIO_UART5 = 0x57;
	*WRITE_GPIO_UART5 = 0x03;
	*WRITE_GPIO_UART5 = 0xAA;
	*WRITE_GPIO_UART5 = 0x50;	

	*WRITE_GPIO_UART6 = 0x57;
	*WRITE_GPIO_UART6 = 0x02;
	*WRITE_GPIO_UART6 = 0xAA;
	*WRITE_GPIO_UART6 = 0x50;	
	*WRITE_GPIO_UART6 = 0x57;
	*WRITE_GPIO_UART6 = 0x03;
	*WRITE_GPIO_UART6 = 0xAA;
	*WRITE_GPIO_UART6 = 0x50;	

	*WRITE_GPIO_UART7 = 0x57;
	*WRITE_GPIO_UART7 = 0x02;
	*WRITE_GPIO_UART7 = 0xAA;
	*WRITE_GPIO_UART7 = 0x50;	
	*WRITE_GPIO_UART7 = 0x57;
	*WRITE_GPIO_UART7 = 0x03;
	*WRITE_GPIO_UART7 = 0xAA;
	*WRITE_GPIO_UART7 = 0x50;
	#endif
}

void InitCOM232_2(void)
{
	#if 0
	* COM_OUT_FIFO_Clear = 0;
	* COM_OUT_FIFO_Clear = 0xffff;
	* COM_IN_FIFO_Clear = 0;
	* COM_IN_FIFO_Clear = 0xffff;

	//57 02 AA 50(0x0D,0x0a)
	//57 03 AA 50
	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x02;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;

	*WRITE_RS232_2 = 0x57;
	*WRITE_RS232_2 = 0x02;
	*WRITE_RS232_2 = 0xAA;
	*WRITE_RS232_2 = 0x50;

	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x03;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;	

	*WRITE_RS232_2 = 0x57;
	*WRITE_RS232_2 = 0x03;
	*WRITE_RS232_2 = 0xAA;
	*WRITE_RS232_2 = 0x50;	
	#endif
}

Uint16 COM232_1(void) 
{	//Fluorescence control
	/************************
	Sub_CMD1 value name match with color.
	0XFF(255)- Disables All.
	0XFE(254)- Cy5/Cy5.5 AlexaFluor 660
	0XFD(253)- ECy3/Alexa Fluor 555 
	0XFB(251)- GFP/FITC
	0XF7(247)- DAPI
	0XEF(239)- TexRed/mCherry
	0XDF(223)- CFP
	0XBF(191)- AlexaFluor 514 
	********************************/

	* COM_OUT_FIFO_Clear = 0;
	* COM_OUT_FIFO_Clear = 0xffff;
	* COM_IN_FIFO_Clear = 0;
	* COM_IN_FIFO_Clear = 0xffff;

	//57 02 AA 50(0x0D,0x0a)
	//57 03 AA 50
	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x02;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;

	*WRITE_RS232_1 = 0x57;
	*WRITE_RS232_1 = 0x03;
	*WRITE_RS232_1 = 0xAA;
	*WRITE_RS232_1 = 0x50;	

	*WRITE_RS232_1 = 0x4F;
	*WRITE_RS232_1 = 0xFF;
	*WRITE_RS232_1 = 0x50;
	//return 1;
}


Uint16 COM232_2(void) 
{	//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)
	//DAC 0V 
	//*WRITE_RS232_2 = 0x08;
	//*WRITE_RS232_2 = 0x00;
	//VOLTAGE_OUT = (1000 - 220)*2048/1000 = 0x06 0x3D;
	* COM_OUT_FIFO_Clear = 0;
	* COM_OUT_FIFO_Clear = 0xffff;
	
	*WRITE_RS232_2 = 0x06;
	*WRITE_RS232_2 = 0x3D;

	//DAC 0V
	*WRITE_RS232_3 = 0x08;
	*WRITE_RS232_3 = 0x00;

	//DAC 0V
	*WRITE_RS232_4 = 0x08;
	*WRITE_RS232_4 = 0x00;

	*DA_Convert_CS = 1;
}


void ParaInit_ServoSub(SERVO *psServo, tsPARASERVO_BUF *psParaServo) {
#if 0
	psServo->S001=psParaServo->S001;	//ä¼ºæœé©±åŠ¨å™¨åž‹å·
	psServo->S003=psParaServo->S003;//åˆ‡æ¢ç”µæœºæ–¹å‘

	//psServo->S037=psParaServo->S037;	//PGåˆ†é¢‘æ¯”(è„‰å†²å•ä½) [Pn212]
	//psServo->S038=psParaServo->S038;	//ç”µå­é½¿è½®æ¯”(åˆ†å­) [Pn20E]
	//psServo->S039=psParaServo->S039;	//ç”µå­é½¿è½®æ¯”(åˆ†æ¯) [Pn210]

	psServo->S106=psParaServo->S106;//ç¼–ç å™¨çº¿æ•°
	psServo->S107=psParaServo->S107;//ç”µæœºä¸€è½¬å¯¹åº”ç›´çº¿è·ç¦»æˆ–æ—‹è½¬è§’åº¦unit:nm or nd
	psServo->S108=psParaServo->S108;//å‡é€Ÿæ¯”åˆ†å­
	psServo->S109=psParaServo->S109;//å‡é€Ÿæ¯”åˆ†æ¯

	psServo->Resolution = 1.0;
	psServo->AxisOutUnitEQU = 1.0;

	//é¿å…é™¤æ³•å‡ºçŽ°ç¨‹åºå¼‚å¸¸20130924 change by rex chen
	if((System.LinearAxisOutUnitEQU) && (System.LinearAxisMinUnit))
	if(((System.ParameterWriteOpenSign&0X0F))&&(psServo->S037)&&(psServo->S038)&&(psServo->S039)
			&&(psServo->S106)&&(psServo->S107)&&(psServo->S108)&&(psServo->S109))
	{
		//åˆ†å¼€è®¡ç®—ï¼Œé˜²æ­¢æµ®ç‚¹è¿ç®—å‡ºé”™
		psServo->Resolution = divdp(psServo->S107 , System.LinearAxisMinUnit);
		psServo->Resolution = divdp(psServo->Resolution , (4*psServo->S037));
		psServo->Resolution = psServo->Resolution * divdp(psServo->S109 , psServo->S108);
		if(fabs(psServo->Resolution) < 1e-6)
		{	//å…¼å®¹è€ç‰ˆæœ¬(è®¾ç½®ç”µå­é½¿è½®æ¯”)
			psServo->Resolution = 1.0;
		}
		//åˆ†å¼€è®¡ç®—ï¼Œé˜²æ­¢æµ®ç‚¹è¿ç®—å‡ºé”™
		psServo->AxisOutUnitEQU = divdp(psServo->S107 , System.LinearAxisOutUnitEQU);
		psServo->AxisOutUnitEQU = divdp((powdp(2 , psServo->S106)) , psServo->AxisOutUnitEQU);
		psServo->AxisOutUnitEQU = psServo->AxisOutUnitEQU * divdp(psServo->S108 , psServo->S109);
		psServo->AxisOutUnitEQU = psServo->AxisOutUnitEQU * psServo->S039;
		psServo->AxisOutUnitEQU = divdp(psServo->AxisOutUnitEQU , psServo->S038);
		if(fabs(psServo->AxisOutUnitEQU) < 1e-6)
		{
			//å…¼å®¹è€ç‰ˆæœ¬(è®¾ç½®ç”µå­é½¿è½®æ¯”)
			psServo->AxisOutUnitEQU = 1.0;
		}
	}

	//æé«˜æ‰§è¡Œæ•ˆçŽ‡ï¼Œé¿å…æ’è¡¥ç‚¹ä¿¡æ¯åˆ¤æ–­çš„æµ®ç‚¹æ•°è¿ç®—
	if(System.LinearAxisOutUnitEQU)
	{
		psServo->AxisOutUnitEQUSp=divdp(5000000.0,System.LinearAxisOutUnitEQU)*psServo->AxisOutUnitEQU;
	}
	else
	{
		psServo->AxisOutUnitEQUSp=5000.0;
	}
	//æ–°å¢žè½¯é™ä½ä¿¡æ¯å¤„ç†
	//if(psDSPPacketTemp->Command==0x50)
	if(psServo==&X_Servo)
	{
		System.XLimitPos=psParaServo->S118;
		System.XLimitNeg=psParaServo->S119;
	}
	else if(psServo==&Y_Servo)	//if(psDSPPacketTemp->Command==0x51)
	{
		System.YLimitPos=psParaServo->S118;
		System.YLimitNeg=psParaServo->S119;
	}
	else if(psServo==&Z_Servo)	//if(psDSPPacketTemp->Command==0x52)
	{
		System.ZLimitPos=psParaServo->S118;
		System.ZLimitNeg=psParaServo->S119;
	}
#endif
}

void ParaInit_Servo(void) {
#if 0
	ParaInit_ServoSub(&Servo_Axis1, &sParaServo_Axis1);
	ParaInit_ServoSub(&Servo_Axis2, &sParaServo_Axis2);
	ParaInit_ServoSub(&Servo_Axis3, &sParaServo_Axis3);
	ParaInit_ServoSub(&Servo_Axis4, &sParaServo_Axis4);
	ParaInit_ServoSub(&Servo_Axis5, &sParaServo_Axis5);
	ParaInit_ServoSub(&Servo_Axis6, &sParaServo_Axis6);
	ParaInit_ServoSub(&Servo_Axis7, &sParaServo_Axis7);
	ParaInit_ServoSub(&Servo_Axis8, &sParaServo_Axis8);
	ParaInit_ServoSub(&Servo_Axis9, &sParaServo_Axis9);
	ParaInit_ServoSub(&Servo_Axis10, &sParaServo_Axis10);
	ParaInit_ServoSub(&Servo_Axis11, &sParaServo_Axis11);
	ParaInit_ServoSub(&Servo_Axis12, &sParaServo_Axis12);
	ParaInit_ServoSub(&Servo_Axis13, &sParaServo_Axis13);
	ParaInit_ServoSub(&Servo_Axis14, &sParaServo_Axis14);
	ParaInit_ServoSub(&Servo_Axis15, &sParaServo_Axis15);
	ParaInit_ServoSub(&Servo_Axis16, &sParaServo_Axis16);
	ParaInit_ServoSub(&Servo_Axis17, &sParaServo_Axis17);
	ParaInit_ServoSub(&Servo_Axis18, &sParaServo_Axis18);
	ParaInit_ServoSub(&Servo_Axis19, &sParaServo_Axis19);
	ParaInit_ServoSub(&Servo_Axis20, &sParaServo_Axis20);
	ParaInit_ServoSub(&Servo_Axis21, &sParaServo_Axis21);
	ParaInit_ServoSub(&Servo_Axis22, &sParaServo_Axis22);
	ParaInit_ServoSub(&Servo_Axis23, &sParaServo_Axis23);
#endif
}

void ParaInit_ArmCFG(void)
{

}
void ParaInit_ArmCtrl(void) 
{
	System.Tsample = divdp(psParaArm_Ctrl->Tsample, 10000.0);	// Uint: 0.1ms
	System.Tsample1 = System.Tsample;
	System.PWM_PeriodRegister_ZeroPeriod = FPGA_MAX_F * 2 * System.Tsample;

	System.SlaveMAX = psParaArm_Ctrl->ServoStationsSub;
	System.TrackRunOutRangeSQR = powdp(psParaArm_Ctrl->TrackRunOutRangeSQR, 2);
	System.a_SET_Module.Module1 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module2 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module3 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module4 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module5 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module6 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module7 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module8 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module9 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module10 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module11 = psParaArm_Ctrl->a_SET;
	System.a_SET_Module.Module12 = psParaArm_Ctrl->a_SET;
	System.a_A4_SET_Module.Module1 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module2 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module3 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module4 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module5 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module6 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module7 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module8 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module9 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module10 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module11 = psParaArm_Ctrl->a_SET_2;
	System.a_A4_SET_Module.Module12 = psParaArm_Ctrl->a_SET_2;

	System.VeerDeltaV = divdp(psParaArm_Ctrl->VeerDeltaV, 10.0);
	System.NicetyVeerDeltaV = divdp(psParaArm_Ctrl->NicetyVeerDeltaV, 10.0);
	System.VeerDeltaT = divdp(psParaArm_Ctrl->VeerDeltaT, 20000.0);
	System.NicetyVeerDeltaT = divdp(psParaArm_Ctrl->NicetyVeerDeltaT, 20000.0);

	//System.REFStopVariable = psParaArm_Ctrl->REFStopVariable;
	System.REFStopVariable.Axis1 = (psParaArm_Ctrl->REFStopVariable & BIT0)?1:0;
	System.REFStopVariable.Axis2 = (psParaArm_Ctrl->REFStopVariable & BIT1)?1:0;
	System.REFStopVariable.Axis3 = (psParaArm_Ctrl->REFStopVariable & BIT2)?1:0;
	System.REFStopVariable.Axis4 = (psParaArm_Ctrl->REFStopVariable & BIT3)?1:0;
	System.REFStopVariable.Axis5 = (psParaArm_Ctrl->REFStopVariable & BIT4)?1:0;
	System.REFStopVariable.Axis6 = (psParaArm_Ctrl->REFStopVariable & BIT5)?1:0;
	System.REFStopVariable.Axis7 = (psParaArm_Ctrl->REFStopVariable & BIT6)?1:0;
	System.REFStopVariable.Axis8 = (psParaArm_Ctrl->REFStopVariable & BIT7)?1:0;
	System.REFStopVariable.Axis9 = (psParaArm_Ctrl->REFStopVariable & BIT8)?1:0;
	System.REFStopVariable.Axis10 = (psParaArm_Ctrl->REFStopVariable & BIT9)?1:0;
	System.REFStopVariable.Axis11 = (psParaArm_Ctrl->REFStopVariable & BIT10)?1:0;
	System.REFStopVariable.Axis12 = (psParaArm_Ctrl->REFStopVariable & BIT11)?1:0;
	System.REFStopVariable.Axis13 = (psParaArm_Ctrl->REFStopVariable & BIT12)?1:0;
	System.REFStopVariable.Axis15 = (psParaArm_Ctrl->REFStopVariable & BIT13)?1:0;
	System.REFStopVariable.Axis15 = (psParaArm_Ctrl->REFStopVariable & BIT14)?1:0;
	System.REFStopVariable.Axis16 = (psParaArm_Ctrl->REFStopVariable & BIT15)?1:0;
	System.REFStopVariable.Axis17 = (psParaArm_Ctrl->REFStopVariable & BIT16)?1:0;
	System.REFStopVariable.Axis18 = (psParaArm_Ctrl->REFStopVariable & BIT17)?1:0;
	System.REFStopVariable.Axis19 = (psParaArm_Ctrl->REFStopVariable & BIT18)?1:0;
	System.REFStopVariable.Axis20 = (psParaArm_Ctrl->REFStopVariable & BIT19)?1:0;
	System.REFStopVariable.Axis21 = (psParaArm_Ctrl->REFStopVariable & BIT20)?1:0;
	System.REFStopVariable.Axis22 = (psParaArm_Ctrl->REFStopVariable & BIT21)?1:0;
	System.REFStopVariable.Axis23 = (psParaArm_Ctrl->REFStopVariable & BIT22)?1:0;
	System.REFStopVariable.Axis24 = (psParaArm_Ctrl->REFStopVariable & BIT23)?1:0;
	System.REFStopVariable.Axis25 = (psParaArm_Ctrl->REFStopVariable & BIT24)?1:0;

	System.RefDir.Axis1 = (psParaArm_Ctrl->RefDirAxis & BIT0)?1:-1;
	System.RefDir.Axis2 = (psParaArm_Ctrl->RefDirAxis & BIT1)?1:-1;
	System.RefDir.Axis3 = (psParaArm_Ctrl->RefDirAxis & BIT2)?1:-1;
	System.RefDir.Axis4 = (psParaArm_Ctrl->RefDirAxis & BIT3)?1:-1;
	System.RefDir.Axis5 = (psParaArm_Ctrl->RefDirAxis & BIT4)?1:-1;
	System.RefDir.Axis6 = (psParaArm_Ctrl->RefDirAxis & BIT5)?1:-1;
	System.RefDir.Axis7 = (psParaArm_Ctrl->RefDirAxis & BIT6)?1:-1;
	System.RefDir.Axis8 = (psParaArm_Ctrl->RefDirAxis & BIT7)?1:-1;
	System.RefDir.Axis9 = (psParaArm_Ctrl->RefDirAxis & BIT8)?1:-1;
	System.RefDir.Axis10 = (psParaArm_Ctrl->RefDirAxis & BIT9)?1:-1;
	System.RefDir.Axis11 = (psParaArm_Ctrl->RefDirAxis & BIT10)?1:-1;
	System.RefDir.Axis12 = (psParaArm_Ctrl->RefDirAxis & BIT11)?1:-1;
	System.RefDir.Axis13 = (psParaArm_Ctrl->RefDirAxis & BIT12)?1:-1;
	System.RefDir.Axis15 = (psParaArm_Ctrl->RefDirAxis & BIT13)?1:-1;
	System.RefDir.Axis15 = (psParaArm_Ctrl->RefDirAxis & BIT14)?1:-1;
	System.RefDir.Axis16 = (psParaArm_Ctrl->RefDirAxis & BIT15)?1:-1;
	System.RefDir.Axis17 = (psParaArm_Ctrl->RefDirAxis & BIT16)?1:-1;
	System.RefDir.Axis18 = (psParaArm_Ctrl->RefDirAxis & BIT17)?1:-1;
	System.RefDir.Axis19 = (psParaArm_Ctrl->RefDirAxis & BIT18)?1:-1;
	System.RefDir.Axis20 = (psParaArm_Ctrl->RefDirAxis & BIT19)?1:-1;
	System.RefDir.Axis21 = (psParaArm_Ctrl->RefDirAxis & BIT20)?1:-1;
	System.RefDir.Axis22 = (psParaArm_Ctrl->RefDirAxis & BIT21)?1:-1;
	System.RefDir.Axis23 = (psParaArm_Ctrl->RefDirAxis & BIT22)?1:-1;
	System.RefDir.Axis24 = (psParaArm_Ctrl->RefDirAxis & BIT23)?1:-1;
	System.RefDir.Axis25 = (psParaArm_Ctrl->RefDirAxis & BIT24)?1:-1;


	System.EncoderCheckChoose.Axis1 = (psParaArm_Ctrl->EncoderCheckAxis & BIT0)?1:0;
	System.EncoderCheckChoose.Axis2 = (psParaArm_Ctrl->EncoderCheckAxis & BIT1)?1:0;
	System.EncoderCheckChoose.Axis3 = (psParaArm_Ctrl->EncoderCheckAxis & BIT2)?1:0;
	System.EncoderCheckChoose.Axis4 = (psParaArm_Ctrl->EncoderCheckAxis & BIT3)?1:0;
	System.EncoderCheckChoose.Axis5 = (psParaArm_Ctrl->EncoderCheckAxis & BIT4)?1:0;
	System.EncoderCheckChoose.Axis6 = (psParaArm_Ctrl->EncoderCheckAxis & BIT5)?1:0;
	System.EncoderCheckChoose.Axis7 = (psParaArm_Ctrl->EncoderCheckAxis & BIT6)?1:0;
	System.EncoderCheckChoose.Axis8 = (psParaArm_Ctrl->EncoderCheckAxis & BIT7)?1:0;
	System.EncoderCheckChoose.Axis9 = (psParaArm_Ctrl->EncoderCheckAxis & BIT8)?1:0;
	System.EncoderCheckChoose.Axis10 = (psParaArm_Ctrl->EncoderCheckAxis & BIT9)?1:0;
	System.EncoderCheckChoose.Axis11 = (psParaArm_Ctrl->EncoderCheckAxis & BIT10)?1:0;
	System.EncoderCheckChoose.Axis12 = (psParaArm_Ctrl->EncoderCheckAxis & BIT11)?1:0;
	System.EncoderCheckChoose.Axis13 = (psParaArm_Ctrl->EncoderCheckAxis & BIT12)?1:0;
	System.EncoderCheckChoose.Axis15 = (psParaArm_Ctrl->EncoderCheckAxis & BIT13)?1:0;
	System.EncoderCheckChoose.Axis15 = (psParaArm_Ctrl->EncoderCheckAxis & BIT14)?1:0;
	System.EncoderCheckChoose.Axis16 = (psParaArm_Ctrl->EncoderCheckAxis & BIT15)?1:0;
	System.EncoderCheckChoose.Axis17 = (psParaArm_Ctrl->EncoderCheckAxis & BIT16)?1:0;
	System.EncoderCheckChoose.Axis18 = (psParaArm_Ctrl->EncoderCheckAxis & BIT17)?1:0;
	System.EncoderCheckChoose.Axis19 = (psParaArm_Ctrl->EncoderCheckAxis & BIT18)?1:0;
	System.EncoderCheckChoose.Axis20 = (psParaArm_Ctrl->EncoderCheckAxis & BIT19)?1:0;
	System.EncoderCheckChoose.Axis21 = (psParaArm_Ctrl->EncoderCheckAxis & BIT20)?1:0;
	System.EncoderCheckChoose.Axis22 = (psParaArm_Ctrl->EncoderCheckAxis & BIT21)?1:0;
	System.EncoderCheckChoose.Axis23 = (psParaArm_Ctrl->EncoderCheckAxis & BIT22)?1:0;
	System.EncoderCheckChoose.Axis24 = (psParaArm_Ctrl->EncoderCheckAxis & BIT23)?1:0;
	System.EncoderCheckChoose.Axis25 = (psParaArm_Ctrl->EncoderCheckAxis & BIT24)?1:0;

	System.LinearAxisMinUnit = psParaArm_Ctrl->AxisUnitInLinear;
	System.UnitTo_mm = divdp(System.LinearAxisMinUnit, 1000000.0);
	System.LinearAxisOutUnitEQU = psParaArm_Ctrl->AxisUnitOutLinear;

	System.G0Speed = psParaArm_Ctrl->G0Speed;
	System.G0Speed_2 = psParaArm_Ctrl->G0Speed_2;

	System.G0Speed_Module.Module1 = System.G0Speed;
	System.G0Speed_Module.Module2 = System.G0Speed;
	System.G0Speed_Module.Module3 = System.G0Speed;
	System.G0Speed_Module.Module4 = System.G0Speed;
	System.G0Speed_Module.Module5 = System.G0Speed;
	System.G0Speed_Module.Module6 = System.G0Speed;
	System.G0Speed_Module.Module7 = System.G0Speed;
	System.G0Speed_Module.Module8 = System.G0Speed;
	System.G0Speed_Module.Module9 = System.G0Speed;
	System.G0Speed_Module.Module10 = System.G0Speed;
	System.G0Speed_Module.Module11 = System.G0Speed;
	System.G0Speed_Module.Module12 = System.G0Speed;

	System.G1Speed = psParaArm_Ctrl->G1Speed;
	System.G1Speed_2 = psParaArm_Ctrl->G1Speed_2;

	System.G1Speed_Module.Module1 = System.G1Speed;
	System.G1Speed_Module.Module2 = System.G1Speed;
	System.G1Speed_Module.Module3 = System.G1Speed;
	System.G1Speed_Module.Module4 = System.G1Speed;
	System.G1Speed_Module.Module5 = System.G1Speed;
	System.G1Speed_Module.Module6 = System.G1Speed;
	System.G1Speed_Module.Module7 = System.G1Speed;
	System.G1Speed_Module.Module8 = System.G1Speed;
	System.G1Speed_Module.Module9 = System.G1Speed;
	System.G1Speed_Module.Module10 = System.G1Speed;
	System.G1Speed_Module.Module11 = System.G1Speed;
	System.G1Speed_Module.Module12 = System.G1Speed;

	System.SRefSpeed = psParaArm_Ctrl->SRefSpeed;
	System.SRefSpeedBack = psParaArm_Ctrl->SRefSpeedBack;
	System.SRefBack = psParaArm_Ctrl->SRefBack;

	System.SRefSpeed_2 = psParaArm_Ctrl->SRefSpeed_2;
	System.SRefSpeedBack_2 = psParaArm_Ctrl->SRefSpeedBack_2;
	System.SRefBack_2 = System.SRefBack_2;

	System.MotorChangeDir.Axis1 = psParaArm_Ctrl->MotorChangeDirAxis1;
	System.MotorChangeDir.Axis2 = psParaArm_Ctrl->MotorChangeDirAxis2;
	System.MotorChangeDir.Axis3 = psParaArm_Ctrl->MotorChangeDirAxis3;
	System.MotorChangeDir.Axis4 = psParaArm_Ctrl->MotorChangeDirAxis4;
	System.MotorChangeDir.Axis5 = psParaArm_Ctrl->MotorChangeDirAxis5;
	System.MotorChangeDir.Axis6 = psParaArm_Ctrl->MotorChangeDirAxis6;
	System.MotorChangeDir.Axis7 = psParaArm_Ctrl->MotorChangeDirAxis7;
	System.MotorChangeDir.Axis8 = psParaArm_Ctrl->MotorChangeDirAxis8;
	System.MotorChangeDir.Axis9 = psParaArm_Ctrl->MotorChangeDirAxis9;
	System.MotorChangeDir.Axis10 = psParaArm_Ctrl->MotorChangeDirAxis10;
	System.MotorChangeDir.Axis11 = psParaArm_Ctrl->MotorChangeDirAxis11;
	System.MotorChangeDir.Axis12 = psParaArm_Ctrl->MotorChangeDirAxis12;
	System.MotorChangeDir.Axis13 = psParaArm_Ctrl->MotorChangeDirAxis13;
	System.MotorChangeDir.Axis14 = psParaArm_Ctrl->MotorChangeDirAxis14;
	System.MotorChangeDir.Axis15 = psParaArm_Ctrl->MotorChangeDirAxis15;
	System.MotorChangeDir.Axis16 = psParaArm_Ctrl->MotorChangeDirAxis16;
	System.MotorChangeDir.Axis17 = psParaArm_Ctrl->MotorChangeDirAxis17;
	System.MotorChangeDir.Axis18 = psParaArm_Ctrl->MotorChangeDirAxis18;
	System.MotorChangeDir.Axis19 = psParaArm_Ctrl->MotorChangeDirAxis19;
	System.MotorChangeDir.Axis20 = psParaArm_Ctrl->MotorChangeDirAxis20;
	System.MotorChangeDir.Axis21 = psParaArm_Ctrl->MotorChangeDirAxis21;
	System.MotorChangeDir.Axis22 = psParaArm_Ctrl->MotorChangeDirAxis22;
	System.MotorChangeDir.Axis23 = psParaArm_Ctrl->MotorChangeDirAxis23;
	System.MotorChangeDir.Axis24 = psParaArm_Ctrl->MotorChangeDirAxis24;
	System.MotorChangeDir.Axis25 = psParaArm_Ctrl->MotorChangeDirAxis25;

	System.EncoderRDDir.Axis1 = psParaArm_Ctrl->EncoderRDDir1;
	System.EncoderRDDir.Axis2 = psParaArm_Ctrl->EncoderRDDir2;
	System.EncoderRDDir.Axis3 = psParaArm_Ctrl->EncoderRDDir3;
	System.EncoderRDDir.Axis4 = psParaArm_Ctrl->EncoderRDDir4;
	System.EncoderRDDir.Axis5 = psParaArm_Ctrl->EncoderRDDir5;
	System.EncoderRDDir.Axis6 = psParaArm_Ctrl->EncoderRDDir6;
	System.EncoderRDDir.Axis7 = psParaArm_Ctrl->EncoderRDDir7;
	System.EncoderRDDir.Axis8 = psParaArm_Ctrl->EncoderRDDir8;
	System.EncoderRDDir.Axis9 = psParaArm_Ctrl->EncoderRDDir9;
	System.EncoderRDDir.Axis10 = psParaArm_Ctrl->EncoderRDDir10;
	System.EncoderRDDir.Axis11 = psParaArm_Ctrl->EncoderRDDir11;
	System.EncoderRDDir.Axis12 = psParaArm_Ctrl->EncoderRDDir12;
	System.EncoderRDDir.Axis13 = psParaArm_Ctrl->EncoderRDDir13;
	System.EncoderRDDir.Axis14 = psParaArm_Ctrl->EncoderRDDir14;
	System.EncoderRDDir.Axis15 = psParaArm_Ctrl->EncoderRDDir15;
	System.EncoderRDDir.Axis16 = psParaArm_Ctrl->EncoderRDDir16;
	System.EncoderRDDir.Axis17 = psParaArm_Ctrl->EncoderRDDir17;
	System.EncoderRDDir.Axis18 = psParaArm_Ctrl->EncoderRDDir18;
	System.EncoderRDDir.Axis19 = psParaArm_Ctrl->EncoderRDDir19;
	System.EncoderRDDir.Axis20 = psParaArm_Ctrl->EncoderRDDir20;
	System.EncoderRDDir.Axis21 = psParaArm_Ctrl->EncoderRDDir21;
	System.EncoderRDDir.Axis22 = psParaArm_Ctrl->EncoderRDDir22;
	System.EncoderRDDir.Axis23 = psParaArm_Ctrl->EncoderRDDir23;
	System.EncoderRDDir.Axis24 = psParaArm_Ctrl->EncoderRDDir24;
	System.EncoderRDDir.Axis25 = psParaArm_Ctrl->EncoderRDDir25;

	Encoder_Axis1.EncoderDIR = System.EncoderRDDir.Axis1;
	Encoder_Axis2.EncoderDIR = System.EncoderRDDir.Axis2;
	Encoder_Axis3.EncoderDIR = System.EncoderRDDir.Axis3;
	Encoder_Axis4.EncoderDIR = System.EncoderRDDir.Axis4;
	Encoder_Axis5.EncoderDIR = System.EncoderRDDir.Axis5;
	Encoder_Axis6.EncoderDIR = System.EncoderRDDir.Axis6;
	Encoder_Axis7.EncoderDIR = System.EncoderRDDir.Axis7;
	Encoder_Axis8.EncoderDIR = System.EncoderRDDir.Axis8;
	Encoder_Axis9.EncoderDIR = System.EncoderRDDir.Axis9;
	Encoder_Axis10.EncoderDIR = System.EncoderRDDir.Axis10;
	Encoder_Axis11.EncoderDIR = System.EncoderRDDir.Axis11;
	Encoder_Axis12.EncoderDIR = System.EncoderRDDir.Axis12;
	Encoder_Axis13.EncoderDIR = System.EncoderRDDir.Axis13;
	Encoder_Axis14.EncoderDIR = System.EncoderRDDir.Axis15;
	Encoder_Axis15.EncoderDIR = System.EncoderRDDir.Axis15;
	Encoder_Axis16.EncoderDIR = System.EncoderRDDir.Axis16;
	Encoder_Axis17.EncoderDIR = System.EncoderRDDir.Axis17;
	Encoder_Axis18.EncoderDIR = System.EncoderRDDir.Axis18;
	Encoder_Axis19.EncoderDIR = System.EncoderRDDir.Axis19;
	Encoder_Axis20.EncoderDIR = System.EncoderRDDir.Axis20;
	Encoder_Axis21.EncoderDIR = System.EncoderRDDir.Axis21;
	Encoder_Axis22.EncoderDIR = System.EncoderRDDir.Axis22;
	Encoder_Axis23.EncoderDIR = System.EncoderRDDir.Axis23;

	System.AxisResolution.Axis1 = psParaArm_Ctrl->AxisResolutionAxis1;
	System.AxisResolution.Axis2 = psParaArm_Ctrl->AxisResolutionAxis2;
	System.AxisResolution.Axis3 = psParaArm_Ctrl->AxisResolutionAxis3;
	System.AxisResolution.Axis4 = psParaArm_Ctrl->AxisResolutionAxis4;
	System.AxisResolution.Axis5 = psParaArm_Ctrl->AxisResolutionAxis5;
	System.AxisResolution.Axis6 = psParaArm_Ctrl->AxisResolutionAxis6;
	System.AxisResolution.Axis7 = psParaArm_Ctrl->AxisResolutionAxis7;
	System.AxisResolution.Axis8 = psParaArm_Ctrl->AxisResolutionAxis8;
	System.AxisResolution.Axis9 = psParaArm_Ctrl->AxisResolutionAxis9;
	System.AxisResolution.Axis10 = psParaArm_Ctrl->AxisResolutionAxis10;
	System.AxisResolution.Axis11 = psParaArm_Ctrl->AxisResolutionAxis11;
	System.AxisResolution.Axis12 = psParaArm_Ctrl->AxisResolutionAxis12;
	System.AxisResolution.Axis13 = psParaArm_Ctrl->AxisResolutionAxis13;
	System.AxisResolution.Axis14 = psParaArm_Ctrl->AxisResolutionAxis14;
	System.AxisResolution.Axis15 = psParaArm_Ctrl->AxisResolutionAxis15;
	System.AxisResolution.Axis16 = psParaArm_Ctrl->AxisResolutionAxis16;
	System.AxisResolution.Axis17 = psParaArm_Ctrl->AxisResolutionAxis17;
	System.AxisResolution.Axis18 = psParaArm_Ctrl->AxisResolutionAxis18;
	System.AxisResolution.Axis19 = psParaArm_Ctrl->AxisResolutionAxis19;
	System.AxisResolution.Axis20 = psParaArm_Ctrl->AxisResolutionAxis20;
	System.AxisResolution.Axis21 = psParaArm_Ctrl->AxisResolutionAxis21;
	System.AxisResolution.Axis22 = psParaArm_Ctrl->AxisResolutionAxis22;
	System.AxisResolution.Axis23 = psParaArm_Ctrl->AxisResolutionAxis23;
	System.AxisResolution.Axis24 = psParaArm_Ctrl->AxisResolutionAxis24;
	System.AxisResolution.Axis25 = psParaArm_Ctrl->AxisResolutionAxis25;

	Encoder_Axis1.Resolution = divdp(System.AxisResolution.Axis1,
			System.LinearAxisMinUnit);
	Encoder_Axis2.Resolution = divdp(System.AxisResolution.Axis2,
			System.LinearAxisMinUnit);
	Encoder_Axis3.Resolution = divdp(System.AxisResolution.Axis3,
			System.LinearAxisMinUnit);
	Encoder_Axis4.Resolution = divdp(System.AxisResolution.Axis4,
			System.LinearAxisMinUnit);
	Encoder_Axis5.Resolution = divdp(System.AxisResolution.Axis5,
			System.LinearAxisMinUnit);
	Encoder_Axis6.Resolution = divdp(System.AxisResolution.Axis6,
			System.LinearAxisMinUnit);
	Encoder_Axis7.Resolution = divdp(System.AxisResolution.Axis7,
			System.LinearAxisMinUnit);
	Encoder_Axis8.Resolution = divdp(System.AxisResolution.Axis8,
			System.LinearAxisMinUnit);
	Encoder_Axis9.Resolution = divdp(System.AxisResolution.Axis9,
			System.LinearAxisMinUnit);
	Encoder_Axis10.Resolution = divdp(System.AxisResolution.Axis10,
			System.LinearAxisMinUnit);
	Encoder_Axis11.Resolution = divdp(System.AxisResolution.Axis11,
			System.LinearAxisMinUnit);
	Encoder_Axis12.Resolution = divdp(System.AxisResolution.Axis12,
			System.LinearAxisMinUnit);
	Encoder_Axis13.Resolution = divdp(System.AxisResolution.Axis13,
			System.LinearAxisMinUnit);
	Encoder_Axis14.Resolution = divdp(System.AxisResolution.Axis14,
			System.LinearAxisMinUnit);
	Encoder_Axis15.Resolution = divdp(System.AxisResolution.Axis15,
			System.LinearAxisMinUnit);
	Encoder_Axis16.Resolution = divdp(System.AxisResolution.Axis16,
			System.LinearAxisMinUnit);
	Encoder_Axis17.Resolution = divdp(System.AxisResolution.Axis17,
			System.LinearAxisMinUnit);
	Encoder_Axis18.Resolution = divdp(System.AxisResolution.Axis18,
			System.LinearAxisMinUnit);
	Encoder_Axis19.Resolution = divdp(System.AxisResolution.Axis19,
			System.LinearAxisMinUnit);
	Encoder_Axis20.Resolution = divdp(System.AxisResolution.Axis20,
			System.LinearAxisMinUnit);
	Encoder_Axis21.Resolution = divdp(System.AxisResolution.Axis21,
			System.LinearAxisMinUnit);
	Encoder_Axis22.Resolution = divdp(System.AxisResolution.Axis22,
			System.LinearAxisMinUnit);
	Encoder_Axis23.Resolution = divdp(System.AxisResolution.Axis23,
			System.LinearAxisMinUnit);

	System.CoordORG.Axis1 = psParaArm_Ctrl->CoordORGAxis1;
	System.CoordORG.Axis2 = psParaArm_Ctrl->CoordORGAxis2;
	System.CoordORG.Axis3 = psParaArm_Ctrl->CoordORGAxis3;
	System.CoordORG.Axis4 = psParaArm_Ctrl->CoordORGAxis4;
	System.CoordORG.Axis5 = psParaArm_Ctrl->CoordORGAxis5;
	System.CoordORG.Axis6 = psParaArm_Ctrl->CoordORGAxis6;
	System.CoordORG.Axis7 = psParaArm_Ctrl->CoordORGAxis7;
	System.CoordORG.Axis8 = psParaArm_Ctrl->CoordORGAxis8;
	System.CoordORG.Axis9 = psParaArm_Ctrl->CoordORGAxis9;
	System.CoordORG.Axis10 = psParaArm_Ctrl->CoordORGAxis10;
	System.CoordORG.Axis11 = psParaArm_Ctrl->CoordORGAxis11;
	System.CoordORG.Axis12 = psParaArm_Ctrl->CoordORGAxis12;
	System.CoordORG.Axis13 = psParaArm_Ctrl->CoordORGAxis13;
	System.CoordORG.Axis14 = psParaArm_Ctrl->CoordORGAxis14;
	System.CoordORG.Axis15 = psParaArm_Ctrl->CoordORGAxis15;
	System.CoordORG.Axis16 = psParaArm_Ctrl->CoordORGAxis16;
	System.CoordORG.Axis17 = psParaArm_Ctrl->CoordORGAxis17;
	System.CoordORG.Axis18 = psParaArm_Ctrl->CoordORGAxis18;
	System.CoordORG.Axis19 = psParaArm_Ctrl->CoordORGAxis19;
	System.CoordORG.Axis20 = psParaArm_Ctrl->CoordORGAxis20;
	System.CoordORG.Axis21 = psParaArm_Ctrl->CoordORGAxis21;
	System.CoordORG.Axis22 = psParaArm_Ctrl->CoordORGAxis22;
	System.CoordORG.Axis23 = psParaArm_Ctrl->CoordORGAxis23;
	System.CoordORG.Axis24 = psParaArm_Ctrl->CoordORGAxis24;
	System.CoordORG.Axis25 = psParaArm_Ctrl->CoordORGAxis25;

	OverallSign.ABSORG_M_Coordinate.Axis1 = System.CoordORG.Axis1;
	OverallSign.ABSORG_M_Coordinate.Axis2 = System.CoordORG.Axis2;
	OverallSign.ABSORG_M_Coordinate.Axis3 = System.CoordORG.Axis3;
	OverallSign.ABSORG_M_Coordinate.Axis4 = System.CoordORG.Axis4;
	OverallSign.ABSORG_M_Coordinate.Axis5 = System.CoordORG.Axis5;
	OverallSign.ABSORG_M_Coordinate.Axis6 = System.CoordORG.Axis6;
	OverallSign.ABSORG_M_Coordinate.Axis7 = System.CoordORG.Axis7;
	OverallSign.ABSORG_M_Coordinate.Axis8 = System.CoordORG.Axis8;
	OverallSign.ABSORG_M_Coordinate.Axis9 = System.CoordORG.Axis9;
	OverallSign.ABSORG_M_Coordinate.Axis10 = System.CoordORG.Axis10;
	OverallSign.ABSORG_M_Coordinate.Axis11 = System.CoordORG.Axis11;
	OverallSign.ABSORG_M_Coordinate.Axis12 = System.CoordORG.Axis12;
	OverallSign.ABSORG_M_Coordinate.Axis13 = System.CoordORG.Axis13;
	OverallSign.ABSORG_M_Coordinate.Axis14 = System.CoordORG.Axis14;
	OverallSign.ABSORG_M_Coordinate.Axis15 = System.CoordORG.Axis15;
	OverallSign.ABSORG_M_Coordinate.Axis16 = System.CoordORG.Axis16;
	OverallSign.ABSORG_M_Coordinate.Axis17 = System.CoordORG.Axis17;
	OverallSign.ABSORG_M_Coordinate.Axis18 = System.CoordORG.Axis18;
	OverallSign.ABSORG_M_Coordinate.Axis19 = System.CoordORG.Axis19;
	OverallSign.ABSORG_M_Coordinate.Axis20 = System.CoordORG.Axis20;
	OverallSign.ABSORG_M_Coordinate.Axis21 = System.CoordORG.Axis21;
	OverallSign.ABSORG_M_Coordinate.Axis22 = System.CoordORG.Axis22;
	OverallSign.ABSORG_M_Coordinate.Axis23 = System.CoordORG.Axis23;
	OverallSign.ABSORG_M_Coordinate.Axis24 = System.CoordORG.Axis24;
	OverallSign.ABSORG_M_Coordinate.Axis25 = System.CoordORG.Axis25;

	System.OffsetCoordinate.Axis1 = psParaArm_Ctrl->CoordOffsetAxis1;
	System.OffsetCoordinate.Axis2 = psParaArm_Ctrl->CoordOffsetAxis2;
	System.OffsetCoordinate.Axis3 = psParaArm_Ctrl->CoordOffsetAxis3;
	System.OffsetCoordinate.Axis4 = psParaArm_Ctrl->CoordOffsetAxis4;
	System.OffsetCoordinate.Axis5 = psParaArm_Ctrl->CoordOffsetAxis5;
	System.OffsetCoordinate.Axis6 = psParaArm_Ctrl->CoordOffsetAxis6;
	System.OffsetCoordinate.Axis7 = psParaArm_Ctrl->CoordOffsetAxis7;
	System.OffsetCoordinate.Axis8 = psParaArm_Ctrl->CoordOffsetAxis8;
	System.OffsetCoordinate.Axis9 = psParaArm_Ctrl->CoordOffsetAxis9;
	System.OffsetCoordinate.Axis10 = psParaArm_Ctrl->CoordOffsetAxis10;
	System.OffsetCoordinate.Axis11 = psParaArm_Ctrl->CoordOffsetAxis11;
	System.OffsetCoordinate.Axis12 = psParaArm_Ctrl->CoordOffsetAxis12;
	System.OffsetCoordinate.Axis13 = psParaArm_Ctrl->CoordOffsetAxis13;
	System.OffsetCoordinate.Axis14 = psParaArm_Ctrl->CoordOffsetAxis14;
	System.OffsetCoordinate.Axis15 = psParaArm_Ctrl->CoordOffsetAxis15;
	System.OffsetCoordinate.Axis16 = psParaArm_Ctrl->CoordOffsetAxis16;
	System.OffsetCoordinate.Axis17 = psParaArm_Ctrl->CoordOffsetAxis17;
	System.OffsetCoordinate.Axis18 = psParaArm_Ctrl->CoordOffsetAxis18;
	System.OffsetCoordinate.Axis19 = psParaArm_Ctrl->CoordOffsetAxis19;
	System.OffsetCoordinate.Axis20 = psParaArm_Ctrl->CoordOffsetAxis20;
	System.OffsetCoordinate.Axis21 = psParaArm_Ctrl->CoordOffsetAxis21;
	System.OffsetCoordinate.Axis22 = psParaArm_Ctrl->CoordOffsetAxis22;
	System.OffsetCoordinate.Axis23 = psParaArm_Ctrl->CoordOffsetAxis23;
	System.OffsetCoordinate.Axis24 = psParaArm_Ctrl->CoordOffsetAxis24;
	System.OffsetCoordinate.Axis25 = psParaArm_Ctrl->CoordOffsetAxis25;

	System.SafeCoordinate.Axis1 = psParaArm_Ctrl->CoordSafeAxis1;
	System.SafeCoordinate.Axis2 = psParaArm_Ctrl->CoordSafeAxis2;
	System.SafeCoordinate.Axis3 = psParaArm_Ctrl->CoordSafeAxis3;
	System.SafeCoordinate.Axis4 = psParaArm_Ctrl->CoordSafeAxis4;
	System.SafeCoordinate.Axis5 = psParaArm_Ctrl->CoordSafeAxis5;
	System.SafeCoordinate.Axis6 = psParaArm_Ctrl->CoordSafeAxis6;
	System.SafeCoordinate.Axis7 = psParaArm_Ctrl->CoordSafeAxis7;
	System.SafeCoordinate.Axis8 = psParaArm_Ctrl->CoordSafeAxis8;
	System.SafeCoordinate.Axis9 = psParaArm_Ctrl->CoordSafeAxis9;
	System.SafeCoordinate.Axis10 = psParaArm_Ctrl->CoordSafeAxis10;
	System.SafeCoordinate.Axis11 = psParaArm_Ctrl->CoordSafeAxis11;
	System.SafeCoordinate.Axis12 = psParaArm_Ctrl->CoordSafeAxis12;
	System.SafeCoordinate.Axis13 = psParaArm_Ctrl->CoordSafeAxis13;
	System.SafeCoordinate.Axis14 = psParaArm_Ctrl->CoordSafeAxis14;
	System.SafeCoordinate.Axis15 = psParaArm_Ctrl->CoordSafeAxis15;
	System.SafeCoordinate.Axis16 = psParaArm_Ctrl->CoordSafeAxis16;
	System.SafeCoordinate.Axis17 = psParaArm_Ctrl->CoordSafeAxis17;
	System.SafeCoordinate.Axis18 = psParaArm_Ctrl->CoordSafeAxis18;
	System.SafeCoordinate.Axis19 = psParaArm_Ctrl->CoordSafeAxis19;
	System.SafeCoordinate.Axis20 = psParaArm_Ctrl->CoordSafeAxis20;
	System.SafeCoordinate.Axis21 = psParaArm_Ctrl->CoordSafeAxis21;
	System.SafeCoordinate.Axis22 = psParaArm_Ctrl->CoordSafeAxis22;
	System.SafeCoordinate.Axis23 = psParaArm_Ctrl->CoordSafeAxis23;
	System.SafeCoordinate.Axis24 = psParaArm_Ctrl->CoordSafeAxis24;
	System.SafeCoordinate.Axis25 = psParaArm_Ctrl->CoordSafeAxis25;

	System.BackCoordinate.Axis1 = psParaArm_Ctrl->CoordBackAxis1;
	System.BackCoordinate.Axis2 = psParaArm_Ctrl->CoordBackAxis2;
	System.BackCoordinate.Axis3 = psParaArm_Ctrl->CoordBackAxis3;
	System.BackCoordinate.Axis4 = psParaArm_Ctrl->CoordBackAxis4;
	System.BackCoordinate.Axis5 = psParaArm_Ctrl->CoordBackAxis5;
	System.BackCoordinate.Axis6 = psParaArm_Ctrl->CoordBackAxis6;
	System.BackCoordinate.Axis7 = psParaArm_Ctrl->CoordBackAxis7;
	System.BackCoordinate.Axis8 = psParaArm_Ctrl->CoordBackAxis8;
	System.BackCoordinate.Axis9 = psParaArm_Ctrl->CoordBackAxis9;
	System.BackCoordinate.Axis10 = psParaArm_Ctrl->CoordBackAxis10;
	System.BackCoordinate.Axis11 = psParaArm_Ctrl->CoordBackAxis11;
	System.BackCoordinate.Axis12 = psParaArm_Ctrl->CoordBackAxis12;
	System.BackCoordinate.Axis13 = psParaArm_Ctrl->CoordBackAxis13;
	System.BackCoordinate.Axis14 = psParaArm_Ctrl->CoordBackAxis14;
	System.BackCoordinate.Axis15 = psParaArm_Ctrl->CoordBackAxis15;
	System.BackCoordinate.Axis16 = psParaArm_Ctrl->CoordBackAxis16;
	System.BackCoordinate.Axis17 = psParaArm_Ctrl->CoordBackAxis17;
	System.BackCoordinate.Axis18 = psParaArm_Ctrl->CoordBackAxis18;
	System.BackCoordinate.Axis19 = psParaArm_Ctrl->CoordBackAxis19;
	System.BackCoordinate.Axis20 = psParaArm_Ctrl->CoordBackAxis20;
	System.BackCoordinate.Axis21 = psParaArm_Ctrl->CoordBackAxis21;
	System.BackCoordinate.Axis22 = psParaArm_Ctrl->CoordBackAxis22;
	System.BackCoordinate.Axis23 = psParaArm_Ctrl->CoordBackAxis23;
	System.BackCoordinate.Axis24 = psParaArm_Ctrl->CoordBackAxis24;
	System.BackCoordinate.Axis25 = psParaArm_Ctrl->CoordBackAxis25;

	System.PositionCoordinate1.Axis1 = psParaArm_Ctrl->PositionCoordinate1Axis1;
	System.PositionCoordinate1.Axis2 = psParaArm_Ctrl->PositionCoordinate1Axis2;
	System.PositionCoordinate1.Axis3 = psParaArm_Ctrl->PositionCoordinate1Axis3;
	System.PositionCoordinate1.Axis4 = psParaArm_Ctrl->PositionCoordinate1Axis4;
	System.PositionCoordinate1.Axis5 = psParaArm_Ctrl->PositionCoordinate1Axis5;
	System.PositionCoordinate1.Axis6 = psParaArm_Ctrl->PositionCoordinate1Axis6;
	System.PositionCoordinate1.Axis7 = psParaArm_Ctrl->PositionCoordinate1Axis7;
	System.PositionCoordinate1.Axis8 = psParaArm_Ctrl->PositionCoordinate1Axis8;
	System.PositionCoordinate1.Axis9 = psParaArm_Ctrl->PositionCoordinate1Axis9;
	System.PositionCoordinate1.Axis10 = psParaArm_Ctrl->PositionCoordinate1Axis10;
	System.PositionCoordinate1.Axis11 = psParaArm_Ctrl->PositionCoordinate1Axis11;
	System.PositionCoordinate1.Axis12 = psParaArm_Ctrl->PositionCoordinate1Axis12;
	System.PositionCoordinate1.Axis13 = psParaArm_Ctrl->PositionCoordinate1Axis13;
	System.PositionCoordinate1.Axis14 = psParaArm_Ctrl->PositionCoordinate1Axis14;
	System.PositionCoordinate1.Axis15 = psParaArm_Ctrl->PositionCoordinate1Axis15;
	System.PositionCoordinate1.Axis16 = psParaArm_Ctrl->PositionCoordinate1Axis16;
	System.PositionCoordinate1.Axis17 = psParaArm_Ctrl->PositionCoordinate1Axis17;
	System.PositionCoordinate1.Axis18 = psParaArm_Ctrl->PositionCoordinate1Axis18;
	System.PositionCoordinate1.Axis19 = psParaArm_Ctrl->PositionCoordinate1Axis19;
	System.PositionCoordinate1.Axis20 = psParaArm_Ctrl->PositionCoordinate1Axis20;
	System.PositionCoordinate1.Axis21 = psParaArm_Ctrl->PositionCoordinate1Axis21;
	System.PositionCoordinate1.Axis22 = psParaArm_Ctrl->PositionCoordinate1Axis22;
	System.PositionCoordinate1.Axis23 = psParaArm_Ctrl->PositionCoordinate1Axis23;
	System.PositionCoordinate1.Axis24 = psParaArm_Ctrl->PositionCoordinate1Axis24;
	System.PositionCoordinate1.Axis25 = psParaArm_Ctrl->PositionCoordinate1Axis25;

	System.PositionCoordinate2.Axis1 = psParaArm_Ctrl->PositionCoordinate2Axis1;
	System.PositionCoordinate2.Axis2 = psParaArm_Ctrl->PositionCoordinate2Axis2;
	System.PositionCoordinate2.Axis3 = psParaArm_Ctrl->PositionCoordinate2Axis3;
	System.PositionCoordinate2.Axis4 = psParaArm_Ctrl->PositionCoordinate2Axis4;
	System.PositionCoordinate2.Axis5 = psParaArm_Ctrl->PositionCoordinate2Axis5;
	System.PositionCoordinate2.Axis6 = psParaArm_Ctrl->PositionCoordinate2Axis6;
	System.PositionCoordinate2.Axis7 = psParaArm_Ctrl->PositionCoordinate2Axis7;
	System.PositionCoordinate2.Axis8 = psParaArm_Ctrl->PositionCoordinate2Axis8;
	System.PositionCoordinate2.Axis9 = psParaArm_Ctrl->PositionCoordinate2Axis9;
	System.PositionCoordinate2.Axis10 = psParaArm_Ctrl->PositionCoordinate2Axis10;
	System.PositionCoordinate2.Axis11 = psParaArm_Ctrl->PositionCoordinate2Axis11;
	System.PositionCoordinate2.Axis12 = psParaArm_Ctrl->PositionCoordinate2Axis12;
	System.PositionCoordinate2.Axis13 = psParaArm_Ctrl->PositionCoordinate2Axis13;
	System.PositionCoordinate2.Axis14 = psParaArm_Ctrl->PositionCoordinate2Axis14;
	System.PositionCoordinate2.Axis15 = psParaArm_Ctrl->PositionCoordinate2Axis15;
	System.PositionCoordinate2.Axis16 = psParaArm_Ctrl->PositionCoordinate2Axis16;
	System.PositionCoordinate2.Axis17 = psParaArm_Ctrl->PositionCoordinate2Axis17;
	System.PositionCoordinate2.Axis18 = psParaArm_Ctrl->PositionCoordinate2Axis18;
	System.PositionCoordinate2.Axis19 = psParaArm_Ctrl->PositionCoordinate2Axis19;
	System.PositionCoordinate2.Axis20 = psParaArm_Ctrl->PositionCoordinate2Axis20;
	System.PositionCoordinate2.Axis21 = psParaArm_Ctrl->PositionCoordinate2Axis21;
	System.PositionCoordinate2.Axis22 = psParaArm_Ctrl->PositionCoordinate2Axis22;
	System.PositionCoordinate2.Axis23 = psParaArm_Ctrl->PositionCoordinate2Axis23;
	System.PositionCoordinate2.Axis24 = psParaArm_Ctrl->PositionCoordinate2Axis24;
	System.PositionCoordinate2.Axis25 = psParaArm_Ctrl->PositionCoordinate2Axis25;

	System.MAXSpeed.Axis1 = psParaArm_Ctrl->FeedMaxAxis1;
	System.MAXSpeed.Axis2 = psParaArm_Ctrl->FeedMaxAxis2;
	System.MAXSpeed.Axis3 = psParaArm_Ctrl->FeedMaxAxis3;
	System.MAXSpeed.Axis4 = psParaArm_Ctrl->FeedMaxAxis4;
	System.MAXSpeed.Axis5 = psParaArm_Ctrl->FeedMaxAxis5;
	System.MAXSpeed.Axis6 = psParaArm_Ctrl->FeedMaxAxis6;
	System.MAXSpeed.Axis7 = psParaArm_Ctrl->FeedMaxAxis7;
	System.MAXSpeed.Axis8 = psParaArm_Ctrl->FeedMaxAxis8;
	System.MAXSpeed.Axis9 = psParaArm_Ctrl->FeedMaxAxis9;
	System.MAXSpeed.Axis10 = psParaArm_Ctrl->FeedMaxAxis10;
	System.MAXSpeed.Axis11 = psParaArm_Ctrl->FeedMaxAxis11;
	System.MAXSpeed.Axis12 = psParaArm_Ctrl->FeedMaxAxis12;
	System.MAXSpeed.Axis13 = psParaArm_Ctrl->FeedMaxAxis13;
	System.MAXSpeed.Axis14 = psParaArm_Ctrl->FeedMaxAxis14;
	System.MAXSpeed.Axis15 = psParaArm_Ctrl->FeedMaxAxis15;
	System.MAXSpeed.Axis16 = psParaArm_Ctrl->FeedMaxAxis16;
	System.MAXSpeed.Axis17 = psParaArm_Ctrl->FeedMaxAxis17;
	System.MAXSpeed.Axis18 = psParaArm_Ctrl->FeedMaxAxis18;
	System.MAXSpeed.Axis19 = psParaArm_Ctrl->FeedMaxAxis19;
	System.MAXSpeed.Axis20 = psParaArm_Ctrl->FeedMaxAxis20;
	System.MAXSpeed.Axis21 = psParaArm_Ctrl->FeedMaxAxis21;
	System.MAXSpeed.Axis22 = psParaArm_Ctrl->FeedMaxAxis22;
	System.MAXSpeed.Axis23 = psParaArm_Ctrl->FeedMaxAxis23;
	System.MAXSpeed.Axis24 = psParaArm_Ctrl->FeedMaxAxis24;
	System.MAXSpeed.Axis25 = psParaArm_Ctrl->FeedMaxAxis25;

	System.REFStopPosition.Axis1 = psParaArm_Ctrl->REFStopAxis1;
	System.REFStopPosition.Axis2 = psParaArm_Ctrl->REFStopAxis2;
	System.REFStopPosition.Axis3 = psParaArm_Ctrl->REFStopAxis3;
	System.REFStopPosition.Axis4 = psParaArm_Ctrl->REFStopAxis4;
	System.REFStopPosition.Axis5 = psParaArm_Ctrl->REFStopAxis5;
	System.REFStopPosition.Axis6 = psParaArm_Ctrl->REFStopAxis6;
	System.REFStopPosition.Axis7 = psParaArm_Ctrl->REFStopAxis7;
	System.REFStopPosition.Axis8 = psParaArm_Ctrl->REFStopAxis8;
	System.REFStopPosition.Axis9 = psParaArm_Ctrl->REFStopAxis9;
	System.REFStopPosition.Axis10 = psParaArm_Ctrl->REFStopAxis10;
	System.REFStopPosition.Axis11 = psParaArm_Ctrl->REFStopAxis11;
	System.REFStopPosition.Axis12 = psParaArm_Ctrl->REFStopAxis12;
	System.REFStopPosition.Axis13 = psParaArm_Ctrl->REFStopAxis13;
	System.REFStopPosition.Axis14 = psParaArm_Ctrl->REFStopAxis14;
	System.REFStopPosition.Axis15 = psParaArm_Ctrl->REFStopAxis15;
	System.REFStopPosition.Axis16 = psParaArm_Ctrl->REFStopAxis16;
	System.REFStopPosition.Axis17 = psParaArm_Ctrl->REFStopAxis17;
	System.REFStopPosition.Axis18 = psParaArm_Ctrl->REFStopAxis18;
	System.REFStopPosition.Axis19 = psParaArm_Ctrl->REFStopAxis19;
	System.REFStopPosition.Axis20 = psParaArm_Ctrl->REFStopAxis20;
	System.REFStopPosition.Axis21 = psParaArm_Ctrl->REFStopAxis21;
	System.REFStopPosition.Axis22 = psParaArm_Ctrl->REFStopAxis22;
	System.REFStopPosition.Axis23 = psParaArm_Ctrl->REFStopAxis23;
	System.REFStopPosition.Axis24 = psParaArm_Ctrl->REFStopAxis24;
	System.REFStopPosition.Axis25 = psParaArm_Ctrl->REFStopAxis25;

	System.SLimitPos.Axis1 = psParaArm_Ctrl->SLimitPosAxis1;
	System.SLimitPos.Axis2 = psParaArm_Ctrl->SLimitPosAxis2;
	System.SLimitPos.Axis3 = psParaArm_Ctrl->SLimitPosAxis3;
	System.SLimitPos.Axis4 = psParaArm_Ctrl->SLimitPosAxis4;
	System.SLimitPos.Axis5 = psParaArm_Ctrl->SLimitPosAxis5;
	System.SLimitPos.Axis6 = psParaArm_Ctrl->SLimitPosAxis6;
	System.SLimitPos.Axis7 = psParaArm_Ctrl->SLimitPosAxis7;
	System.SLimitPos.Axis8 = psParaArm_Ctrl->SLimitPosAxis8;
	System.SLimitPos.Axis9 = psParaArm_Ctrl->SLimitPosAxis9;
	System.SLimitPos.Axis10 = psParaArm_Ctrl->SLimitPosAxis10;
	System.SLimitPos.Axis11 = psParaArm_Ctrl->SLimitPosAxis11;
	System.SLimitPos.Axis12 = psParaArm_Ctrl->SLimitPosAxis12;
	System.SLimitPos.Axis13 = psParaArm_Ctrl->SLimitPosAxis13;
	System.SLimitPos.Axis14 = psParaArm_Ctrl->SLimitPosAxis14;
	System.SLimitPos.Axis15 = psParaArm_Ctrl->SLimitPosAxis15;
	System.SLimitPos.Axis16 = psParaArm_Ctrl->SLimitPosAxis16;
	System.SLimitPos.Axis17 = psParaArm_Ctrl->SLimitPosAxis17;
	System.SLimitPos.Axis18 = psParaArm_Ctrl->SLimitPosAxis18;
	System.SLimitPos.Axis19 = psParaArm_Ctrl->SLimitPosAxis19;
	System.SLimitPos.Axis20 = psParaArm_Ctrl->SLimitPosAxis20;
	System.SLimitPos.Axis21 = psParaArm_Ctrl->SLimitPosAxis21;
	System.SLimitPos.Axis22 = psParaArm_Ctrl->SLimitPosAxis22;
	System.SLimitPos.Axis23 = psParaArm_Ctrl->SLimitPosAxis23;
	System.SLimitPos.Axis24 = psParaArm_Ctrl->SLimitPosAxis24;
	System.SLimitPos.Axis25 = psParaArm_Ctrl->SLimitPosAxis25;

	System.SLimitNeg.Axis1 = psParaArm_Ctrl->SLimitNegAxis1;
	System.SLimitNeg.Axis2 = psParaArm_Ctrl->SLimitNegAxis2;
	System.SLimitNeg.Axis3 = psParaArm_Ctrl->SLimitNegAxis3;
	System.SLimitNeg.Axis4 = psParaArm_Ctrl->SLimitNegAxis4;
	System.SLimitNeg.Axis5 = psParaArm_Ctrl->SLimitNegAxis5;
	System.SLimitNeg.Axis6 = psParaArm_Ctrl->SLimitNegAxis6;
	System.SLimitNeg.Axis7 = psParaArm_Ctrl->SLimitNegAxis7;
	System.SLimitNeg.Axis8 = psParaArm_Ctrl->SLimitNegAxis8;
	System.SLimitNeg.Axis9 = psParaArm_Ctrl->SLimitNegAxis9;
	System.SLimitNeg.Axis10 = psParaArm_Ctrl->SLimitNegAxis10;
	System.SLimitNeg.Axis11 = psParaArm_Ctrl->SLimitNegAxis11;
	System.SLimitNeg.Axis12 = psParaArm_Ctrl->SLimitNegAxis12;
	System.SLimitNeg.Axis13 = psParaArm_Ctrl->SLimitNegAxis13;
	System.SLimitNeg.Axis14 = psParaArm_Ctrl->SLimitNegAxis14;
	System.SLimitNeg.Axis15 = psParaArm_Ctrl->SLimitNegAxis15;
	System.SLimitNeg.Axis16 = psParaArm_Ctrl->SLimitNegAxis16;
	System.SLimitNeg.Axis17 = psParaArm_Ctrl->SLimitNegAxis17;
	System.SLimitNeg.Axis18 = psParaArm_Ctrl->SLimitNegAxis18;
	System.SLimitNeg.Axis19 = psParaArm_Ctrl->SLimitNegAxis19;
	System.SLimitNeg.Axis20 = psParaArm_Ctrl->SLimitNegAxis20;
	System.SLimitNeg.Axis21 = psParaArm_Ctrl->SLimitNegAxis21;
	System.SLimitNeg.Axis22 = psParaArm_Ctrl->SLimitNegAxis22;
	System.SLimitNeg.Axis23 = psParaArm_Ctrl->SLimitNegAxis23;
	System.SLimitNeg.Axis24 = psParaArm_Ctrl->SLimitNegAxis24;
	System.SLimitNeg.Axis25 = psParaArm_Ctrl->SLimitNegAxis25;

	System.FunctionSelect01 = psParaArm_Ctrl->FunctionSelect01;//default 1
	System.FunctionSelect02 = psParaArm_Ctrl->FunctionSelect02;//default 1
	System.FunctionSelect03 = psParaArm_Ctrl->FunctionSelect03;//default 1
	System.FunctionSelect04 = psParaArm_Ctrl->FunctionSelect04;
	System.FunctionSelect05 = psParaArm_Ctrl->FunctionSelect05;
	System.FunctionSelect06 = psParaArm_Ctrl->FunctionSelect06;
	System.FunctionSelect07 = psParaArm_Ctrl->FunctionSelect07;
	System.FunctionSelect08 = psParaArm_Ctrl->FunctionSelect08;
	System.FunctionSelect09 = psParaArm_Ctrl->FunctionSelect09;
	System.FunctionSelect10 = psParaArm_Ctrl->FunctionSelect10;

	//for refresh LED flash time
	//System.LEDFlashTime1 = psParaArm_Ctrl->LEDFlashTime1;
	//System.LEDFlashTime2 = psParaArm_Ctrl->LEDFlashTime2;

	System.RunState = 0;
	//System.RunState = 1;//for test

	System.ReferenceFrameMode = REF_XYZ;

	System_Initial();

	psDSPPacketOut->PacketCheckCodeBuf[1] = psParaArm_Ctrl->PacketCheckCode_B1;
}

void ParaInit_Linear(void) 
{
	Uint32 temp_count;
	Int16	i;
//	Int8	data8;
	Int32	data32;
	Uint32	MicroEDataBaseStartCnt,MicroEDataBaseEndCnt,MicroEDataBaseTotalCnt;
	
	temp_count = psEepromPara_Arm->Macro[0];//SCIA_RxBuf[7]

	if(temp_count != OverallSign.MicroEDataBaseStartCnt)
	{
		OverallSign.MicroEDataBaseStartCnt = OverallSign.MicroEDataBaseStartCnt;
		return;
	}

	temp_count = psEepromPara_Arm->Macro[1];//SCIA_RxBuf[8]
	if(temp_count == OverallSign.MicroEDataBaseEndCnt)
	{
		OverallSign.MicroEDataBaseEndCnt = OverallSign.MicroEDataBaseEndCnt;
		return;
	}	
	/*
	temp_count = psEepromPara_Arm->Macro[2];//SCIA_RxBuf[9]
	if(temp_count == OverallSign.MicroEDataBaseTotalCnt)
	{
		OverallSign.MicroEDataBaseTotalCnt = OverallSign.MicroEDataBaseTotalCnt;
		return;
	}
	*/
	
	OverallSign.MicroEDataBaseStartCnt = psEepromPara_Arm->Macro[0];//SCIA_RxBuf[7];
	OverallSign.MicroEDataBaseEndCnt = psEepromPara_Arm->Macro[1];//SCIA_RxBuf[8];
	OverallSign.MicroEDataBaseTotalCnt = psEepromPara_Arm->Macro[2];//SCIA_RxBuf[9];

	if(OverallSign.MicroEDataBaseStartCnt == 0)
	{
		LensCompensationBufferCnt = 0;
		memset(LensCompensationBuffer,0,sizeof(LensCompensationBuffer));
	}
	
	for(temp_count=0;temp_count<(OverallSign.MicroEDataBaseEndCnt-OverallSign.
MicroEDataBaseStartCnt);temp_count++)
	{
		LensCompensationBuffer[LensCompensationBufferCnt++] = psEepromPara_Arm->Macro[3+temp_count];//SCIA_RxBuf[temp_count+10];
		if(LensCompensationBufferCnt > 8000)
		{
			LensCompensationBufferCnt = 0;
		}
	}
	OverallSign.MicroEDataBaseStartCnt = OverallSign.MicroEDataBaseEndCnt;
	psEepromPara_Arm->Macro[0] = psEepromPara_Arm->Macro[1];//OverallSign.MicroEDataBaseStartCnt = OverallSign.MicroEDataBaseEndCnt;
	psDSPPacketOut->MicroEDataBaseStartCnt = psEepromPara_Arm->Macro[0];
	psDSPPacketOut->PacketCheckCodeBuf[2] = psEepromPara_Arm->Macro[212];
	return 1;
}

void ParaInit(void) {
	ParaInit_ArmCtrl();

#if 0
	ParaInit_Servo();
	ParaInit_Linear();
#endif
}

INT8U Dsp_Com_HandJudge(void) 
{
	return OK;
}

void Dsp_Com_Hand(void) 
{

}

INT8U Dsp_Com_JogJudge(void) 
{
}

void Dsp_Com_Jog(void) 
{

}

void Dsp_Com_PacketIn(void) 
{
	static Uint16 ServoOn2CounterRefValue = 200;
		
	sDSPPacketIn = *psDSPPacketIn;

	if((psDSPPacketIn_InfoEvent->ServoOn))// 1
	{
		if (Error.MainErrorSign == 0) 
		{
			//CPLD_ServoOn();
			CPLD_ServoOn2();
			ServoOn2Counter++;
			if(ServoOn2Counter >= ServoOn2CounterRefValue)
			{
				ServoOn2Counter = ServoOn2CounterRefValue;
				CPLD_ServoOn1();
			}
			
		} 
		else
		{
			CPLD_ServoOff();
		}
	} 
	else 
	{
		CPLD_ServoOff();
		SPC_Initial();
		STD_Initial();
	}

	OverallSign.NCSign = psDSPPacketIn_InfoEvent->NCModeType;// 2
	if(OverallSign.NCSign != NOCODERUN)
	{	//for Debug break test
		OverallSign.NCSign = OverallSign.NCSign;
	}
	
	OverallSign.RapidFeedrateOverride = psDSPPacketIn->GradeG0;// 3
	OverallSign.FeedrateOverride = psDSPPacketIn->GradeG1;// 4

	#if 0
	//OverallSign.LEDFlashCtr1 = psDSPPacketIn->LEDFlashCtr1;// 5
	//OverallSign.LEDFlashCtr2 = psDSPPacketIn->LEDFlashCtr2;// 6
	OverallSign.LEDFlashTime1 = psDSPPacketIn->LEDFlashTime1;// 7
	OverallSign.LEDFlashTime2 = psDSPPacketIn->LEDFlashTime2;// 8

	OverallSign.LEDFlashCtr1 = 0;
	OverallSign.LEDFlashCtr2 = 0;
	if(psDSPPacketIn->LEDFlashCtr1 == 1)
	{
		OverallSign.LEDFlashCtr1 = 0x01; 
		OverallSign.LEDFlashCtr2 = 0x01; 
		Write_LED_OUT_CONTROL();
		Write_LED_OUT_TIME1();
		psDSPPacketIn->LEDFlashCtr1 = 0;
		
	}
	if(psDSPPacketIn->LEDFlashCtr2 == 1)
	{
		OverallSign.LEDFlashCtr1 = 0x02;
		OverallSign.LEDFlashCtr2 = 0x02; 
		Write_LED_OUT_CONTROL();
		Write_LED_OUT_TIME2();
		psDSPPacketIn->LEDFlashCtr2 = 0;
	}
	#endif
	if((psDSPPacketIn->LEDFlashCtr1 != 0)||(psDSPPacketIn->LEDFlashCtr2 != 0))
	{
		Write_LEDFlash_OUT((psDSPPacketIn->LEDFlashCtr1 | psDSPPacketIn->LEDFlashCtr2 << 1),psDSPPacketIn->LEDFlashTime1,psDSPPacketIn->LEDFlashTime2);
		psDSPPacketIn->LEDFlashCtr1 = 0;
		psDSPPacketIn->LEDFlashCtr2 = 0;		
	}
	OverallSign.JOGAxis = psDSPPacketIn->JOGAxis;// 9
	OverallSign.JOGDir = psDSPPacketIn->JOGDir;// 10
	//OverallSign.JOGDistance = divdp(psDSPPacketIn->JOGDistance,
	//		(1000 * System.UnitTo_mm));// 11
	OverallSign.JOGDistance = psDSPPacketIn->JOGDistance;//20170301
	OverallSign.JOGFeedrate = psDSPPacketIn->JOGFeedrate;// 12
	SPCNCSign.F = OverallSign.JOGFeedrate;
	if( (OverallSign.JOGDir != 0)&&(SPCNCSign.CodeRun_Over == 1))
	{
		SPC_JOG_Code();		
		psDSPPacketIn->JOGDir = 0;
	}

	// 13
	if (psDSPPacketIn_InfoEvent->STDInit) 
	{
		STD_Initial();
		psDSPPacketIn_InfoEvent->STDInit = 0;
	}
	if (psDSPPacketIn_InfoEvent->SPCInit) 
	{
		SPC_Initial();
		psDSPPacketIn_InfoEvent->SPCInit = 0;
	}
	if (psDSPPacketIn_InfoEvent->DSPPositionGet) 
	{
		STDNCSign.GetPositionSign = 1;
		SPCNCSign.GetPositionSign = 1;
		Timer0.GetPositionCounter = Timer0.GenericCounter;
		psDSPPacketIn_InfoEvent->DSPPositionGet = 0;
	}

	// 14
	OverallSign.CameraScanSign 			= (psDSPPacketIn->InInfo1 & 0x0001)?(1):(0);
	OverallSign.CameraStaticCaptureSign 		= (psDSPPacketIn->InInfo1 & 0x0002)?(1):(0);
	OverallSign.MicroEScanSign 			= (psDSPPacketIn->InInfo1 & 0x0004)?(1):(0);
	OverallSign.CameraSoftTriggerLedFlash 	= (psDSPPacketIn->InInfo1 & 0x0008)?(1):(0);

	//15
	if(((psDSPPacketIn->NeedFindAxisSign) & 0x80000000) &&
		((psDSPPacketIn->NeedFindAxisSign) & 0x007fffff))
	{
		if (Reference.SearchRefSign == 0) 
		{
			Reference.SearchRefSign = 1;
			Reference.RefStep = 0;
			Reference.FindRefSign.Axis1 = (System.REFStopVariable.Axis1)?0:1;
			Reference.FindRefSign.Axis2 = (System.REFStopVariable.Axis2)?0:1;
			Reference.FindRefSign.Axis3 = (System.REFStopVariable.Axis3)?0:1;
			Reference.FindRefSign.Axis4 = (System.REFStopVariable.Axis4)?0:1;
			Reference.FindRefSign.Axis5 = (System.REFStopVariable.Axis5)?0:1;
			Reference.FindRefSign.Axis6 = (System.REFStopVariable.Axis6)?0:1;
			Reference.FindRefSign.Axis7 = (System.REFStopVariable.Axis7)?0:1;
			Reference.FindRefSign.Axis8 = (System.REFStopVariable.Axis8)?0:1;
			Reference.FindRefSign.Axis9 = (System.REFStopVariable.Axis9)?0:1;
			Reference.FindRefSign.Axis10 = (System.REFStopVariable.Axis10)?0:1;
			Reference.FindRefSign.Axis11 = (System.REFStopVariable.Axis11)?0:1;
			Reference.FindRefSign.Axis12 = (System.REFStopVariable.Axis12)?0:1;
			Reference.FindRefSign.Axis13 = (System.REFStopVariable.Axis13)?0:1;
			Reference.FindRefSign.Axis14 = (System.REFStopVariable.Axis14)?0:1;
			Reference.FindRefSign.Axis15 = (System.REFStopVariable.Axis15)?0:1;
			Reference.FindRefSign.Axis16 = (System.REFStopVariable.Axis16)?0:1;
			Reference.FindRefSign.Axis17 = (System.REFStopVariable.Axis17)?0:1;
			Reference.FindRefSign.Axis18 = (System.REFStopVariable.Axis18)?0:1;
			Reference.FindRefSign.Axis19 = (System.REFStopVariable.Axis19)?0:1;
			Reference.FindRefSign.Axis20 = (System.REFStopVariable.Axis20)?0:1;
			Reference.FindRefSign.Axis21 = (System.REFStopVariable.Axis21)?0:1;
			Reference.FindRefSign.Axis22 = (System.REFStopVariable.Axis22)?0:1;
			Reference.FindRefSign.Axis23 = (System.REFStopVariable.Axis23)?0:1;
			Reference.FindRefSign.Axis24 = (System.REFStopVariable.Axis24)?0:1;
			Reference.FindRefSign.Axis25 = (System.REFStopVariable.Axis25)?0:1;

			//Servo_Encoder_Clear1_Out_Register.all = 0;
			//Write_Servo_Encoder_Clear1_Out();
			//Servo_Encoder_Clear2_Out_Register.all = 0;
			//Write_Servo_Encoder_Clear2_Out();
		}
	} 
	else 
	{
		Reference.SearchRefSign = 0;
	}

	// 15
	Reference.NeedFindAxisSign.Axis1 = ((psDSPPacketIn->NeedFindAxisSign)& 0x1)?(1):(0);
	Reference.NeedFindAxisSign.Axis2 = ((psDSPPacketIn->NeedFindAxisSign)& 0x2)?(1):(0);
	Reference.NeedFindAxisSign.Axis3 = ((psDSPPacketIn->NeedFindAxisSign)& 0x4)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis4 = ((psDSPPacketIn->NeedFindAxisSign)& 0x8)?(0):(0);//Abstract Encoder

	Reference.NeedFindAxisSign.Axis5 = ((psDSPPacketIn->NeedFindAxisSign)& 0x10)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis6 = ((psDSPPacketIn->NeedFindAxisSign)& 0x20)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis7 = ((psDSPPacketIn->NeedFindAxisSign)& 0x40)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis8 = ((psDSPPacketIn->NeedFindAxisSign)& 0x80)?(0):(0);//Abstract Encoder

	Reference.NeedFindAxisSign.Axis9 = ((psDSPPacketIn->NeedFindAxisSign)& 0x100)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis10 = ((psDSPPacketIn->NeedFindAxisSign)& 0x200)?(1):(0);
	Reference.NeedFindAxisSign.Axis11 = ((psDSPPacketIn->NeedFindAxisSign)& 0x400)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis12 = ((psDSPPacketIn->NeedFindAxisSign)& 0x800)?(0):(0);//Abstract Encoder

	Reference.NeedFindAxisSign.Axis13 = ((psDSPPacketIn->NeedFindAxisSign)& 0x1000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis14 = ((psDSPPacketIn->NeedFindAxisSign)& 0x2000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis15 = ((psDSPPacketIn->NeedFindAxisSign)& 0x4000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis16 = ((psDSPPacketIn->NeedFindAxisSign)& 0x8000)?(0):(0);//Abstract Encoder

	Reference.NeedFindAxisSign.Axis17 = ((psDSPPacketIn->NeedFindAxisSign)& 0x10000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis18 = ((psDSPPacketIn->NeedFindAxisSign)& 0x20000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis19 = ((psDSPPacketIn->NeedFindAxisSign)& 0x40000)?(1):(0);//Incrument Encoder
	Reference.NeedFindAxisSign.Axis20 = ((psDSPPacketIn->NeedFindAxisSign)& 0x80000)?(0):(0);//Abstract Encoder

	Reference.NeedFindAxisSign.Axis21 = ((psDSPPacketIn->NeedFindAxisSign)& 0x100000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis22 = ((psDSPPacketIn->NeedFindAxisSign)& 0x200000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis23 = ((psDSPPacketIn->NeedFindAxisSign)& 0x400000)?(0):(0);//Abstract Encoder
	Reference.NeedFindAxisSign.Axis24 = 0;
	Reference.NeedFindAxisSign.Axis25 = 0;

	MainCommand_ForTestSign.all = 0xFFFF;
	//for test alpha
	MainCommand_ForTestSign.all = (psDSPPacketIn->FunctionSelectionSwitch & BIT0)?(0):(MainCommand_ForTestSign.all);//for test alpha
	
	if(System.FunctionSelect05 == True)
	{
		if(psDSPPacketIn->FunctionSelectionSwitch == 0xABCD)
		{
			//for test plus
			MainCommand_ForTestSign.all = 0xABCD;
		}
	}
	//for test
	//MainCommand_ForTestSign.all = 0xABCD;

	psDSPPacketOut->PacketCheckCodeBuf[0] = psDSPPacketIn->PacketCheckCode_B0;
		
}

void Dsp_Com_PacketOut(void) 
{
	psDSPPacketOut->Head = 0xaa;
	psDSPPacketOut->Length = sizeof(tsDSP_PACKETOUT) - 8;
	psDSPPacketOut->Port = 0xb0;
	psDSPPacketOut->Protocal = 0x0f;
	psDSPPacketOut->Command = 0xb0;
	psDSPPacketOut->Packet = 0x00;
	psDSPPacketOut->STDRunCount =(STDNCSign.RealRunCount) ?(GCodeBuffer[(STDNCSign.RealRunCount - 1) % STDGCODE_MOD].SendCount) :(0);
	psDSPPacketOut->SPCRunCount =(SPCNCSign.RealRunCount) ?(SPCGCodeBuffer[(SPCNCSign.RealRunCount - 1) % SPCGCODE_MOD].SendCount) :(0);
	psDSPPacketOut->STDSendCount =(STDNCSign.RealSendCount) ?(GCodeBuffer[(STDNCSign.RealSendCount - 1) % STDGCODE_MOD].SendCount) :(0);
	psDSPPacketOut->SPCSendCount =(SPCNCSign.RealSendCount) ?(SPCGCodeBuffer[(SPCNCSign.RealSendCount - 1) % SPCGCODE_MOD].SendCount) :(0);
	//psDSPPacketOut->MicroEDataBaseStartCnt = 
		
	strcpy((char *) psDSPPacketOut->DspVersionBuf, "2017-03-17 V4.0.2");
	strcpy((char *) psDSPPacketOut->KeyVersionBuf, "2017-01-05 V4.0.1");
	strcpy((char *) psDSPPacketOut->IOVersionBuf, "2017-01-05 V4.0.0");
	//strcpy((char *) psDSPPacketOut->SPDVersionBuf, "2016-01-19 V3.0.0");

	psDSPPacketOut->FeedSet = (OverallSign.NCSign)?(NC_AREAControl.Vm):(0);

	psDSPPacketOut->FeedReal.Module1 = NC_AREAControl.VNow_Module.Module1;
	psDSPPacketOut->FeedReal.Module2 = NC_AREAControl.VNow_Module.Module2;
	psDSPPacketOut->FeedReal.Module3 = NC_AREAControl.VNow_Module.Module3;
	psDSPPacketOut->FeedReal.Module4 = NC_AREAControl.VNow_Module.Module4;
	psDSPPacketOut->FeedReal.Module5 = NC_AREAControl.VNow_Module.Module5;
	psDSPPacketOut->FeedReal.Module6 = NC_AREAControl.VNow_Module.Module6;
	psDSPPacketOut->FeedReal.Module7 = NC_AREAControl.VNow_Module.Module7;
	psDSPPacketOut->FeedReal.Module8 = NC_AREAControl.VNow_Module.Module8;
	psDSPPacketOut->FeedReal.Module9 = NC_AREAControl.VNow_Module.Module9;
	psDSPPacketOut->FeedReal.Module10 = NC_AREAControl.VNow_Module.Module10;
	psDSPPacketOut->FeedReal.Module11 = NC_AREAControl.VNow_Module.Module11;
	psDSPPacketOut->FeedReal.Module12 = NC_AREAControl.VNow_Module.Module12;
	
	psDSPPacketOut->ABSMachineCoordinateAxis1 = OverallSign.AbsoluteEncoderValue_PSO.Axis1;
	psDSPPacketOut->ABSMachineCoordinateAxis2 = OverallSign.AbsoluteEncoderValue_PSO.Axis2;
	psDSPPacketOut->ABSMachineCoordinateAxis3 = OverallSign.AbsoluteEncoderValue_PSO.Axis3;
	psDSPPacketOut->ABSMachineCoordinateAxis4 = OverallSign.AbsoluteEncoderValue_PSO.Axis4;
	psDSPPacketOut->ABSMachineCoordinateAxis5 = OverallSign.AbsoluteEncoderValue_PSO.Axis5;
	psDSPPacketOut->ABSMachineCoordinateAxis6 = OverallSign.AbsoluteEncoderValue_PSO.Axis6;
	psDSPPacketOut->ABSMachineCoordinateAxis7 = OverallSign.AbsoluteEncoderValue_PSO.Axis7;
	psDSPPacketOut->ABSMachineCoordinateAxis8 = OverallSign.AbsoluteEncoderValue_PSO.Axis8;
	psDSPPacketOut->ABSMachineCoordinateAxis9 = OverallSign.AbsoluteEncoderValue_PSO.Axis9;
	psDSPPacketOut->ABSMachineCoordinateAxis10 = OverallSign.AbsoluteEncoderValue_PSO.Axis10;
	psDSPPacketOut->ABSMachineCoordinateAxis11 = OverallSign.AbsoluteEncoderValue_PSO.Axis11;
	psDSPPacketOut->ABSMachineCoordinateAxis12 = OverallSign.AbsoluteEncoderValue_PSO.Axis12;
	psDSPPacketOut->ABSMachineCoordinateAxis13 = OverallSign.AbsoluteEncoderValue_PSO.Axis13;
	psDSPPacketOut->ABSMachineCoordinateAxis14 = OverallSign.AbsoluteEncoderValue_PSO.Axis14;
	psDSPPacketOut->ABSMachineCoordinateAxis15 = OverallSign.AbsoluteEncoderValue_PSO.Axis15;
	psDSPPacketOut->ABSMachineCoordinateAxis16 = OverallSign.AbsoluteEncoderValue_PSO.Axis16;
	psDSPPacketOut->ABSMachineCoordinateAxis17 = OverallSign.AbsoluteEncoderValue_PSO.Axis17;
	psDSPPacketOut->ABSMachineCoordinateAxis18 = OverallSign.AbsoluteEncoderValue_PSO.Axis18;
	psDSPPacketOut->ABSMachineCoordinateAxis19 = OverallSign.AbsoluteEncoderValue_PSO.Axis19;
	psDSPPacketOut->ABSMachineCoordinateAxis20 = OverallSign.AbsoluteEncoderValue_PSO.Axis20;
	psDSPPacketOut->ABSMachineCoordinateAxis21 = OverallSign.AbsoluteEncoderValue_PSO.Axis21;
	psDSPPacketOut->ABSMachineCoordinateAxis22 = OverallSign.AbsoluteEncoderValue_PSO.Axis22;
	psDSPPacketOut->ABSMachineCoordinateAxis23 = OverallSign.AbsoluteEncoderValue_PSO.Axis23;
	psDSPPacketOut->ABSMachineCoordinateAxis24 = 0;
	psDSPPacketOut->ABSMachineCoordinateAxis25 = 0;

	psDSPPacketOut->MachineCoordinateAxis1 = OverallSign.ABS_Coordinate.Axis1;
	psDSPPacketOut->MachineCoordinateAxis2 = OverallSign.ABS_Coordinate.Axis2;
	psDSPPacketOut->MachineCoordinateAxis3 = OverallSign.ABS_Coordinate.Axis3;
	psDSPPacketOut->MachineCoordinateAxis4 = OverallSign.ABS_Coordinate.Axis4;
	psDSPPacketOut->MachineCoordinateAxis5 = OverallSign.ABS_Coordinate.Axis5;
	psDSPPacketOut->MachineCoordinateAxis6 = OverallSign.ABS_Coordinate.Axis6;
	psDSPPacketOut->MachineCoordinateAxis7 = OverallSign.ABS_Coordinate.Axis7;
	psDSPPacketOut->MachineCoordinateAxis8 = OverallSign.ABS_Coordinate.Axis8;
	psDSPPacketOut->MachineCoordinateAxis9 = OverallSign.ABS_Coordinate.Axis9;
	psDSPPacketOut->MachineCoordinateAxis10 = OverallSign.ABS_Coordinate.Axis10;
	psDSPPacketOut->MachineCoordinateAxis11 = OverallSign.ABS_Coordinate.Axis11;
	psDSPPacketOut->MachineCoordinateAxis12 = OverallSign.ABS_Coordinate.Axis12;
	psDSPPacketOut->MachineCoordinateAxis13 = OverallSign.ABS_Coordinate.Axis13;
	psDSPPacketOut->MachineCoordinateAxis14 = OverallSign.ABS_Coordinate.Axis14;
	psDSPPacketOut->MachineCoordinateAxis15 = OverallSign.ABS_Coordinate.Axis15;
	psDSPPacketOut->MachineCoordinateAxis16 = OverallSign.ABS_Coordinate.Axis16;
	psDSPPacketOut->MachineCoordinateAxis17 = OverallSign.ABS_Coordinate.Axis17;
	psDSPPacketOut->MachineCoordinateAxis18 = OverallSign.ABS_Coordinate.Axis18;
	psDSPPacketOut->MachineCoordinateAxis19 = OverallSign.ABS_Coordinate.Axis19;
	psDSPPacketOut->MachineCoordinateAxis20 = OverallSign.ABS_Coordinate.Axis20;
	psDSPPacketOut->MachineCoordinateAxis21 = OverallSign.ABS_Coordinate.Axis21;
	psDSPPacketOut->MachineCoordinateAxis22 = OverallSign.ABS_Coordinate.Axis22;
	psDSPPacketOut->MachineCoordinateAxis23 = OverallSign.ABS_Coordinate.Axis23;
	psDSPPacketOut->MachineCoordinateAxis24 = 0;
	psDSPPacketOut->MachineCoordinateAxis25 = Pmt_Counter1.AverageValue_G114;

	#if 0
	psDSPPacketOut->MachineCoordinateAxis1 = Encoder_Axis1.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis2 = Encoder_Axis2.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis3 = Encoder_Axis3.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis4 = Encoder_Axis4.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis5 = Encoder_Axis5.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis6 = Encoder_Axis6.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis7 = Encoder_Axis7.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis8 = Encoder_Axis8.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis9 = Encoder_Axis9.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis10 = Encoder_Axis10.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis11 = Encoder_Axis11.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis12 = Encoder_Axis12.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis13 = Encoder_Axis13.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis14 = Encoder_Axis14.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis15 = Encoder_Axis15.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis16 = Encoder_Axis16.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis17 = Encoder_Axis17.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis18 = Encoder_Axis18.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis19 = Encoder_Axis19.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis20 = Encoder_Axis20.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis21 = Encoder_Axis21.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis22 = Encoder_Axis22.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis23 = Encoder_Axis23.MACH_PositionValue;
	psDSPPacketOut->MachineCoordinateAxis24 = 0;
	psDSPPacketOut->MachineCoordinateAxis25 = 0;
	#endif
	psDSPPacketOut->IOInInfo1 = GPIO_In1_Register.all + ((GPIO_In2_Register.all & 0xffff) << 16);
	psDSPPacketOut->IOInInfo2 = GPIO_In3_Register.all + ((GPIO_In4_Register.all & 0xffff) << 16);
	psDSPPacketOut->IOOutInfo1 = GPIO_Out1_Register.all + ((GPIO_Out2_Register.all & 0xffff) << 16);
	psDSPPacketOut->IOOutInfo2 = 0;

	
	psDSPPacketOut_InfoEvent->SPCSendEnable = FAIL;
	if (OverallSign.NCSign == SPCCODERUN) 
	{
		if (((SPCNCSign.RealRunCount + SPCGCODE_MOD - 2)> SPCNCSign.RealSendCount) && (SPCNCSign.GetPositionSign == 0))
		{
			psDSPPacketOut_InfoEvent->SPCSendEnable = OK;
		}
	}
	
	psDSPPacketOut_InfoEvent->STDSendEnable = FAIL;
	if (OverallSign.NCSign == STDCODERUN) 
	{
		if (((STDNCSign.RealRunCount + STDGCODE_MOD - 80)> STDNCSign.RealSendCount) && (STDNCSign.GetPositionSign == 0))
		{
			psDSPPacketOut_InfoEvent->STDSendEnable = OK;
		}
	}

	psDSPPacketOut_InfoEvent->DSPInAlarm = (Error.MainErrorSign);

	System.InPositionSign = (((OverallSign.NCSign == STDCODERUN) && (STDNCSign.GetPositionSign == 0))
							||((OverallSign.NCSign == SPCCODERUN)&& (SPCNCSign.GetPositionSign == 0))) ?
						(OK):(FAIL);
	
	psDSPPacketOut_InfoEvent->DSPLocateOver = System.InPositionSign;
	psDSPPacketOut_InfoEvent->DSPRunOver = (OverallSign.LastCodeOverSign);
	psDSPPacketOut_InfoEvent->ParameterWriteCompleteSign = OverallSign.ParameterReadCompleteSign;

	psDSPPacketOut_InfoEvent->HLimitPosAxis1 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis2 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2 );
 	psDSPPacketOut_InfoEvent->HLimitPosAxis3 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis4 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis5 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis6 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis7 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis8 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis9 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis10 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis11 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis12 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis13 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis14 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis15 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis16 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis17 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis18 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis19 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis20 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis21 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis22 = (Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22 );
	psDSPPacketOut_InfoEvent->HLimitPosAxis23 = (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23 );

	psDSPPacketOut_InfoEvent->HLimitNegAxis1 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis2 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis3 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis4 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis5 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis6 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis7 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis8 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis9 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis10 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis11 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis12 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis13 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis14 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis15 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis16 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis17 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis18 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis19 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis20 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis21 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis22 =(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22 );
	psDSPPacketOut_InfoEvent->HLimitNegAxis23 =(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23 );

	psDSPPacketOut_InfoEvent->AxisRunOverAxis1 =(Servo_Coin1_In_Register.bit.COIN_1== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis2 =(Servo_Coin1_In_Register.bit.COIN_2== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis3 =(Servo_Coin1_In_Register.bit.COIN_3== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis4 =(Servo_Coin1_In_Register.bit.COIN_4== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis5 =(Servo_Coin1_In_Register.bit.COIN_5== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis6 =(Servo_Coin2_In_Register.bit.COIN_6== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis7 =(Servo_Coin2_In_Register.bit.COIN_7== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis8 =(Servo_Coin2_In_Register.bit.COIN_8== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis9 =(Servo_Coin2_In_Register.bit.COIN_9== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis10 =(Servo_Coin2_In_Register.bit.COIN_10== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis11 =(Servo_Coin1_In_Register.bit.COIN_11== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis12 =(Servo_Coin1_In_Register.bit.COIN_12== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis13 =(Servo_Coin1_In_Register.bit.COIN_13== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis14 =(Servo_Coin1_In_Register.bit.COIN_14== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis15 =(Servo_Coin1_In_Register.bit.COIN_15== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis16 =(Servo_Coin1_In_Register.bit.COIN_16== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis17 =(Servo_Coin1_In_Register.bit.COIN_17== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis18 =(Servo_Coin1_In_Register.bit.COIN_18== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis19 =(Servo_Coin2_In_Register.bit.COIN_19== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis20 =(Servo_Coin2_In_Register.bit.COIN_20== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis21 =(Servo_Coin2_In_Register.bit.COIN_21== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis22 =(Servo_Coin2_In_Register.bit.COIN_22== 0);
	psDSPPacketOut_InfoEvent->AxisRunOverAxis23 =(Servo_Coin1_In_Register.bit.COIN_23== 0);

	psDSPPacketOut_InfoEvent->AxisRunOverValid1 = (OverallSign.EveryAxisCoinValid1 == 0);
	psDSPPacketOut_InfoEvent->AxisRunOverValid2 = (OverallSign.EveryAxisCoinValid2 == 0);
	psDSPPacketOut_InfoEvent->AxisRunOverValid3 = (OverallSign.EveryAxisCoinValid3 == 0);
	psDSPPacketOut_InfoEvent->AxisRunOverValid4 = (OverallSign.EveryAxisCoinValid4 == 0);
	psDSPPacketOut_InfoEvent->AxisRunOverValidAll = OverallSign.EveryAxisCoinValid;
	if(psDSPPacketOut_InfoEvent->AxisRunOverValidAll == 0)
	{	//for Debug break
		psDSPPacketOut_InfoEvent->AxisRunOverValidAll = psDSPPacketOut_InfoEvent->AxisRunOverValidAll ;
	}

	psDSPPacketOut_InfoEvent->AxisRefOverAxis1 =(Reference.FindRefSign.Axis1);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis2 =(Reference.FindRefSign.Axis2);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis3 =(Reference.FindRefSign.Axis3);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis4 =(Reference.FindRefSign.Axis4);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis5 =(Reference.FindRefSign.Axis5);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis6 =(Reference.FindRefSign.Axis6);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis7 =(Reference.FindRefSign.Axis7);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis8 =(Reference.FindRefSign.Axis8);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis9 =(Reference.FindRefSign.Axis9);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis10 =(Reference.FindRefSign.Axis10);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis11 =(Reference.FindRefSign.Axis11);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis12 =(Reference.FindRefSign.Axis12);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis13 =(Reference.FindRefSign.Axis13);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis14 =(Reference.FindRefSign.Axis14);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis15 =(Reference.FindRefSign.Axis15);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis16 =(Reference.FindRefSign.Axis16);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis17 =(Reference.FindRefSign.Axis17);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis18 =(Reference.FindRefSign.Axis18);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis19 =(Reference.FindRefSign.Axis19);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis20 =(Reference.FindRefSign.Axis20);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis21 =(Reference.FindRefSign.Axis21);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis22 =(Reference.FindRefSign.Axis22);
	psDSPPacketOut_InfoEvent->AxisRefOverAxis23 =(Reference.FindRefSign.Axis23);
	psDSPPacketOut_InfoEvent->AxisRefOverValidAll =(Reference.RefStep == 20);//for test

	psDSPPacketOut_InfoEvent->NCModeType = OverallSign.NCSign & 0x03;

	psDSPPacketOut_InfoEvent->AlmServoAxis1 = (Error.ServoAlarm.Axis1);
	psDSPPacketOut_InfoEvent->AlmServoAxis2 = (Error.ServoAlarm.Axis2);
	psDSPPacketOut_InfoEvent->AlmServoAxis3 = (Error.ServoAlarm.Axis3);
	psDSPPacketOut_InfoEvent->AlmServoAxis4 = (Error.ServoAlarm.Axis4);
	psDSPPacketOut_InfoEvent->AlmServoAxis5 = (Error.ServoAlarm.Axis5);
	psDSPPacketOut_InfoEvent->AlmServoAxis6 = (Error.ServoAlarm.Axis6);
	psDSPPacketOut_InfoEvent->AlmServoAxis7 = (Error.ServoAlarm.Axis7);
	psDSPPacketOut_InfoEvent->AlmServoAxis8 = (Error.ServoAlarm.Axis8);
	psDSPPacketOut_InfoEvent->AlmServoAxis9 = (Error.ServoAlarm.Axis9);
	psDSPPacketOut_InfoEvent->AlmServoAxis10 = (Error.ServoAlarm.Axis10);
	psDSPPacketOut_InfoEvent->AlmServoAxis11 = (Error.ServoAlarm.Axis11);
	psDSPPacketOut_InfoEvent->AlmServoAxis12 = (Error.ServoAlarm.Axis12);
	psDSPPacketOut_InfoEvent->AlmServoAxis13 = (Error.ServoAlarm.Axis13);
	psDSPPacketOut_InfoEvent->AlmServoAxis14 = (Error.ServoAlarm.Axis14);
	psDSPPacketOut_InfoEvent->AlmServoAxis15 = (Error.ServoAlarm.Axis15);
	psDSPPacketOut_InfoEvent->AlmServoAxis16 = (Error.ServoAlarm.Axis16);
	psDSPPacketOut_InfoEvent->AlmServoAxis17 = (Error.ServoAlarm.Axis17);
	psDSPPacketOut_InfoEvent->AlmServoAxis18 = (Error.ServoAlarm.Axis18);
	psDSPPacketOut_InfoEvent->AlmServoAxis19 = (Error.ServoAlarm.Axis19);
	psDSPPacketOut_InfoEvent->AlmServoAxis20 = (Error.ServoAlarm.Axis20);
	psDSPPacketOut_InfoEvent->AlmServoAxis21 = (Error.ServoAlarm.Axis21);
	psDSPPacketOut_InfoEvent->AlmServoAxis22 = (Error.ServoAlarm.Axis22);
	psDSPPacketOut_InfoEvent->AlmServoAxis23 = (Error.ServoAlarm.Axis23);

	psDSPPacketOut_InfoEvent->AlmEncoderAxis1 = (Error.EncoderError.Axis1);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis2 = (Error.EncoderError.Axis2);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis3 = (Error.EncoderError.Axis3);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis4 = (Error.EncoderError.Axis4);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis5 = (Error.EncoderError.Axis5);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis6 = (Error.EncoderError.Axis6);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis7 = (Error.EncoderError.Axis7);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis8 = (Error.EncoderError.Axis8);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis9 = (Error.EncoderError.Axis9);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis10 = (Error.EncoderError.Axis10);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis11 = (Error.EncoderError.Axis11);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis12 = (Error.EncoderError.Axis12);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis13 = (Error.EncoderError.Axis13);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis14 = (Error.EncoderError.Axis14);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis15 = (Error.EncoderError.Axis15);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis16 = (Error.EncoderError.Axis16);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis17 = (Error.EncoderError.Axis17);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis18 = (Error.EncoderError.Axis18);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis19 = (Error.EncoderError.Axis19);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis20 = (Error.EncoderError.Axis20);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis21 = (Error.EncoderError.Axis21);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis22 = (Error.EncoderError.Axis22);
	psDSPPacketOut_InfoEvent->AlmEncoderAxis23 = (Error.EncoderError.Axis23);

	psDSPPacketOut_InfoEvent->AlmDSPRamIn = (Error.InsideRAMCheckError);
	psDSPPacketOut_InfoEvent->AlmDSPRamOut = (Error.OutsideRAMCheckError);
	psDSPPacketOut_InfoEvent->AlmDSPCode = (Error.NCCodeBreak);
	psDSPPacketOut_InfoEvent->AlmDSPTrack = (Error.TrackRunOutError);
	psDSPPacketOut_InfoEvent->AlmDSPInterpolate = (Error.InterpolationOver);

	psDSPPacketOut_InfoEvent->AlmHLimitPos = Error.AlmHLimitPos;
	psDSPPacketOut_InfoEvent->AlmHLimitNeg = Error.AlmHLimitNeg;
	psDSPPacketOut_InfoEvent->AlmSLimitPos = Error.AlmSLimitPos;
	psDSPPacketOut_InfoEvent->AlmSLimitNeg = Error.AlmSLimitNeg;
	psDSPPacketOut_InfoEvent->AlmCompDataCheck = Error.CompDataCheckError;
	psDSPPacketOut_InfoEvent->AlmOpticalFlashTriggerBreak = Error.OpticalFlashTriggerBreakError;
	psDSPPacketOut_InfoEvent->AlmCameraShutterTriggerBreak = Error.CameraShutterTriggerBreakError;
	psDSPPacketOut_InfoEvent->AlmLensPositionBreak = Error.LensPositionBreakError;
	
	psDSPPacketOut->TrackRunOver = Error.TrackRunOutError;
	psDSPPacketOut->InterpolationOver = Error.InterpolationOver;
	psDSPPacketOut->CompensationDataCheckError = Error.CompDataCheckError;
	#if 0
	if(Error.TrackRunOutError == False)
	{
		psDSPPacketOut->TrackRunOver = 0;
	}
	if(Error.InterpolationOver == False)
	{
		psDSPPacketOut->InterpolationOver = 0;
	}
	if(Error.CompDataCheckError == False)
	{
		psDSPPacketOut->CompensationDataCheckError = 0;
	}
	#endif

	//Packet_CRCMake((INT8U *)&psDSPPacketOut->Length);
}

void Dsp_Com_EepromARM(void) 
{
	#if 0
	sParaServo_Axis1=*psParaServo_Axis1;
	sParaServo_Axis2=*psParaServo_Axis2;
	sParaServo_Axis3=*psParaServo_Axis3;
	sParaServo_Axis4=*psParaServo_Axis4;
	sParaServo_Axis5=*psParaServo_Axis5;
	sParaServo_Axis6=*psParaServo_Axis6;
	sParaServo_Axis7=*psParaServo_Axis7;
	sParaServo_Axis8=*psParaServo_Axis8;
	sParaServo_Axis9=*psParaServo_Axis9;
	sParaServo_Axis10=*psParaServo_Axis10;
	sParaServo_Axis11=*psParaServo_Axis11;
	sParaServo_Axis12=*psParaServo_Axis12;
	sParaServo_Axis13=*psParaServo_Axis13;
	sParaServo_Axis14=*psParaServo_Axis14;
	sParaServo_Axis15=*psParaServo_Axis15;
	sParaServo_Axis16=*psParaServo_Axis16;
	sParaServo_Axis17=*psParaServo_Axis17;
	sParaServo_Axis18=*psParaServo_Axis18;
	sParaServo_Axis19=*psParaServo_Axis19;
	sParaServo_Axis20=*psParaServo_Axis20;
	sParaServo_Axis21=*psParaServo_Axis21;
	sParaServo_Axis22=*psParaServo_Axis22;
	sParaServo_Axis23=*psParaServo_Axis23;

	sParaPP_Mode1=*psParaPP_Mode1;
	sParaPP_Mode2=*psParaPP_Mode2;
	sParaPP_Mode3=*psParaPP_Mode3;
	sParaPP_Mode4=*psParaPP_Mode4;
	sParaPP_Mode5=*psParaPP_Mode5;
	sParaPP_Mode6=*psParaPP_Mode6;
	#endif

	#if 0
	sParaArm_Cfg=*psParaArm_Cfg;
	#endif

	sParaArm_Ctrl = *psParaArm_Ctrl;

	#if 0
	sParaArm_Io=*psParaArm_Io;
	sParaArm_Tool=*psParaArm_Tool;
	sParaARM_Atc=*psParaARM_Atc;
	#endif

	ParaInit();

}

void Dsp_Com_EepromDSP(void) 
{
	SPI_Read(0, (INT8U *) sSHMAddress.pEepromDSP, 128 * 1024);
}

void Dsp_Com_CodeSPC(void) 
{
	INT32S i, SendCount;
	GCODE *psNowCode;
	static int In = 5000;

	OverallSign.LastCodeOverSign = 0;

	psDSPCodeSPCSub = &psDSPCodeSPC->sInfoBuf[i];
	psDSPPacketOut->PacketCheckCodeBuf[4] = psDSPCodeSPCSub->PacketCheckCode_B4;

	if (psDSPCodeSPCSub->CountID == SPCNCSign.RealSendCount)
	{
		return;
	}

	SPCNCSign.RealSendCount = psDSPCodeSPCSub->CountID;

	psNowCode = &SPCGCodeBuffer[(SPCNCSign.RealSendCount - 1) % SPCGCODE_MOD];

	psNowCode->SendCount = SPCNCSign.RealSendCount;

	if (psNowCode->SendCount > 1) 
	{
		if (labs(psNowCode->SendCount - SPCGCodeBuffer[(SPCNCSign.RealSendCount - 2)% SPCGCODE_MOD].SendCount) >= 2) 
		{
			Error.NCCodeBreak = 1;
			return;
		}
	}

	psNowCode->CMDMain = 1;
	psNowCode->SubCMD1 = 0;
	psNowCode->SubCMD2 = 0;

	psNowCode->StartPoint = OverallSign.SPCLastMoveGCode.EndPoint;
	psNowCode->EndPoint.Axis1 = psDSPCodeSPCSub->PosAxis1;
	psNowCode->EndPoint.Axis2 = psDSPCodeSPCSub->PosAxis2;
	psNowCode->EndPoint.Axis3 = psDSPCodeSPCSub->PosAxis3;
	psNowCode->EndPoint.Axis4 = psDSPCodeSPCSub->PosAxis4;
	psNowCode->EndPoint.Axis5 = psDSPCodeSPCSub->PosAxis5;
	psNowCode->EndPoint.Axis6 = psDSPCodeSPCSub->PosAxis6;
	psNowCode->EndPoint.Axis7 = psDSPCodeSPCSub->PosAxis7;
	psNowCode->EndPoint.Axis8 = psDSPCodeSPCSub->PosAxis8;
	psNowCode->EndPoint.Axis9 = psDSPCodeSPCSub->PosAxis9;
	psNowCode->EndPoint.Axis10 = psDSPCodeSPCSub->PosAxis10;
	psNowCode->EndPoint.Axis11 = psDSPCodeSPCSub->PosAxis11;
	psNowCode->EndPoint.Axis12 = psDSPCodeSPCSub->PosAxis12;
	psNowCode->EndPoint.Axis13 = psDSPCodeSPCSub->PosAxis13;
	psNowCode->EndPoint.Axis14 = psDSPCodeSPCSub->PosAxis14;
	psNowCode->EndPoint.Axis15 = psDSPCodeSPCSub->PosAxis15;
	psNowCode->EndPoint.Axis16 = psDSPCodeSPCSub->PosAxis16;
	psNowCode->EndPoint.Axis17 = psDSPCodeSPCSub->PosAxis17;
	psNowCode->EndPoint.Axis18 = psDSPCodeSPCSub->PosAxis18;
	psNowCode->EndPoint.Axis19 = psDSPCodeSPCSub->PosAxis19;
	psNowCode->EndPoint.Axis20 = psDSPCodeSPCSub->PosAxis20;
	psNowCode->EndPoint.Axis21 = psDSPCodeSPCSub->PosAxis21;
	psNowCode->EndPoint.Axis22 = psDSPCodeSPCSub->PosAxis22;
	psNowCode->EndPoint.Axis23 = psDSPCodeSPCSub->PosAxis23;
	psNowCode->EndPoint.Axis24 = psDSPCodeSPCSub->PosAxis24;
	psNowCode->EndPoint.Axis25 = psDSPCodeSPCSub->PosAxis25;

	//SPC_JudgeSoftLimit(psNowCode);

	SPCNCSign.F = psDSPCodeSPCSub->Feed;
	psNowCode->Feedrate = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module1 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module2 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module3 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module4 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module5 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module6 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module7 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module8 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module9 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module10 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module11 = SPCNCSign.F;
	psNowCode->Feedrate_Module.Module12 = SPCNCSign.F;

	SPC_Deal_Line(psNowCode);

	OverallSign.SPCLastMoveGCode.EndPoint = psNowCode->EndPoint;

	psNowCode->VeCalSign = 0;
	psNowCode->MoveCodeSign = 0;
	
	SPCNCSign.LastRealSendCount = SPCNCSign.RealSendCount;
	
	if(psNowCode->CMDMain<=3)
	{
		psNowCode->MoveCodeSign = 1;
		NCAreaGCodeBuffer[ (NCRunSign.RealSendCount++)%STDGCODE_MOD ] = *psNowCode;
	}
}

void Dsp_Com_CodeSTD(void) 
{
	INT16U i;
	tsCODE_G00 *psCodeInfo;

	psCodeInfo = (tsCODE_G00 *) &psDSPCodeSTD->CodeBuf[0];

	psDSPPacketOut->PacketCheckCodeBuf[3] = psCodeInfo->PacketCheckCode_B3;

	STDNCSign.LastRealSendCount = STDNCSign.RealSendCount;

	STD_Rec_All(psCodeInfo);

	if (Error.NCCodeBreak) 
	{
		return;
	}

	if (STDNCSign.ResendGcodeSign) 
	{
		STDNCSign.ResendGcodeSign = 0;
	}
	else 
	{
		GCODE *psNowCode;
		psNowCode = &GCodeBuffer[(STDNCSign.RealSendCount - 1)% STDGCODE_MOD];

		if (STDNCSign.RealSendCount > 1) 
		{
			if (labs(psNowCode->SendCount - GCodeBuffer[(STDNCSign.RealSendCount - 2)%STDGCODE_MOD].SendCount) > 1)
			{
				Error.NCCodeBreak = 1;
				Error.MainErrorSign = 1;
				high_error();
				return;
			}
		}
		
		STDNCSign.LastRealSendCount = STDNCSign.RealSendCount;

		psNowCode->VeCalSign = 0;

		if (psNowCode->CMDMain < 4)
		{
			psNowCode->MoveCodeSign = 1;
			NCAreaGCodeBuffer[(NCRunSign.RealSendCount++) % STDGCODE_MOD] = *psNowCode;
		} 
		else 
		{
			psNowCode->MoveCodeSign = 0;
		}

	}

}


void Dsp_Com_Compensation(void)
{
	Uint16 ArrayN;
	Int32 data32;
	Int16 i,j;
	//Uint16	temp_PointerNumber = 1024;
	Uint16	temp_PointerNumber = 0;
	Int32	*INT32UPointer1;
	Int32 PacketCheckCode_B5_Pointer;

	PacketCheckCode_B5_Pointer = psEepromPara_Arm->Macro[temp_PointerNumber+1]*2+3;
	psDSPPacketOut->PacketCheckCodeBuf[5] = psEepromPara_Arm->Macro[PacketCheckCode_B5_Pointer];
	
	INT32UPointer1 = &psEepromPara_Arm->Macro[temp_PointerNumber+2];	//²¹³¥Êý×éÆðÊ¼Ö¸Õë
	
	if(psEepromPara_Arm->Macro[temp_PointerNumber] == 1)//Axis1
	{	//  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis1 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 1;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis1= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis1 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis1 < AxisCompBuffer[0].Value_Linear.Axis1)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis1 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis1 > AxisCompBuffer[0].Value_Linear.Axis1)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis1 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 2)//Axis2
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis2 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0002;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis2= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis2 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis2 < AxisCompBuffer[0].Value_Linear.Axis2)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis2 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis2 > AxisCompBuffer[0].Value_Linear.Axis2)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis2 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 3)//Axis3
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis3 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0004;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis3= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis3 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis3 < AxisCompBuffer[0].Value_Linear.Axis3)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis3 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis3 > AxisCompBuffer[0].Value_Linear.Axis3)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis3 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 4)//Axis4
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis4 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0008;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis4= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis4 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis4 < AxisCompBuffer[0].Value_Linear.Axis4)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis4 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis4 > AxisCompBuffer[0].Value_Linear.Axis4)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis4 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 5)//Axis5
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis5 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0010;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis5= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis5 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis5 < AxisCompBuffer[0].Value_Linear.Axis5)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis5 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis5 > AxisCompBuffer[0].Value_Linear.Axis5)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis5 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 6)//Axis6
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis6 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0020;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis6= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis6 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis6 < AxisCompBuffer[0].Value_Linear.Axis6)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis6 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis6 > AxisCompBuffer[0].Value_Linear.Axis6)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis6 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 7)//Axis7
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis7 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0040;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis7= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis7 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis7 < AxisCompBuffer[0].Value_Linear.Axis7)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis7 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis7 > AxisCompBuffer[0].Value_Linear.Axis7)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis7 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 8)//Axis8
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis8 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0080;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis8= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis8 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis8 < AxisCompBuffer[0].Value_Linear.Axis8)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis8 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis8 > AxisCompBuffer[0].Value_Linear.Axis8)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis8 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 9)//Axis9
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis9 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0100;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis9= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis9 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis9 < AxisCompBuffer[0].Value_Linear.Axis9)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis9 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis9 > AxisCompBuffer[0].Value_Linear.Axis9)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis9 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 10)//Axis10
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis10 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0200;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis10= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis10 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis10 < AxisCompBuffer[0].Value_Linear.Axis10)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis10 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis10 > AxisCompBuffer[0].Value_Linear.Axis10)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis10 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 11)//Axis11
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis11 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0400;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis11= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis11 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis11 < AxisCompBuffer[0].Value_Linear.Axis11)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis11 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis11 > AxisCompBuffer[0].Value_Linear.Axis11)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis11 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 12)//Axis12
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis12 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x0800;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis12= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis12 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis12 < AxisCompBuffer[0].Value_Linear.Axis12)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis12 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis12 > AxisCompBuffer[0].Value_Linear.Axis12)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis12 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 13)//Axis13
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis13 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x1000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis13= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis13 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis13 < AxisCompBuffer[0].Value_Linear.Axis13)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis13 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis13 > AxisCompBuffer[0].Value_Linear.Axis13)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis13 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 14)//Axis14
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis14 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x2000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis14= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis14 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis14 < AxisCompBuffer[0].Value_Linear.Axis14)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis14 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis14 > AxisCompBuffer[0].Value_Linear.Axis14)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis14 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 15)//Axis15
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis15 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x4000;		
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis15= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis15 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis15 < AxisCompBuffer[0].Value_Linear.Axis15)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis15 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis15 > AxisCompBuffer[0].Value_Linear.Axis15)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis15 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 16)//Axis16
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis16 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x8000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis16= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis16 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis16 < AxisCompBuffer[0].Value_Linear.Axis16)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis16 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis16 > AxisCompBuffer[0].Value_Linear.Axis16)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis16 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 17)//Axis17
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis17 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00010000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis17= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis17 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis17 < AxisCompBuffer[0].Value_Linear.Axis17)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis17 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis17 > AxisCompBuffer[0].Value_Linear.Axis17)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis17 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 18)//Axis18
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis18 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00020000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis18= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis18 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis18 < AxisCompBuffer[0].Value_Linear.Axis18)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis18 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis18 > AxisCompBuffer[0].Value_Linear.Axis18)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis18 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 19)//Axis19
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis19 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00040000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis19= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis19 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis19 < AxisCompBuffer[0].Value_Linear.Axis19)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis19 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis19 > AxisCompBuffer[0].Value_Linear.Axis19)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis19 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 20)//Axis20
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis20 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00080000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis20= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis20 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis20 < AxisCompBuffer[0].Value_Linear.Axis20)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis20 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis20 > AxisCompBuffer[0].Value_Linear.Axis20)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis20 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 21)//Axis21
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis21 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00100000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis21= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis21 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis21 < AxisCompBuffer[0].Value_Linear.Axis21)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis21 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis21 > AxisCompBuffer[0].Value_Linear.Axis21)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis21 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 22)//Axis22
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis22 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00200000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis22= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis22 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis22 < AxisCompBuffer[0].Value_Linear.Axis22)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis22 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis22 > AxisCompBuffer[0].Value_Linear.Axis22)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis22 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	else if(psEepromPara_Arm->Macro[temp_PointerNumber] == 23)//Axis23
	{	//X  ·ÇÏßÐÔ²¹³¥Êý×é
		ArrayN = psEepromPara_Arm->Macro[temp_PointerNumber+1]; 		//Êý×é³¤¶È
		System.AxisCompMaxL.Axis23 = ArrayN-1;		//Êµ¼Ê²¹³¥ÇøÓòÊýÎªÊý×é³¤¶È¼õ1

		if((ArrayN > COMPBUFFERLENGTH) || (ArrayN < 2))
		{	//·¶Î§[2,COMPBUFFERLENGTH]
			Error.CompDataCheckError = 1;
			psDSPPacketOut->CompensationDataCheckError = 0x00400000;
			return 0;
		}

		for(i=0;i<ArrayN;i++)
		{
			AxisCompBuffer[i].Value_Linear.Axis23= *INT32UPointer1++;	
			AxisCompBuffer[i].Value_Laser.Axis23 = *INT32UPointer1++;	
		}
		
		//Í¨¹ýL[0]ºÍL[1]ÅÐ¶Ï²¹³¥·½Ïò£¬ÈçÏàµÈÔò±¨¾¯¡£
		if(AxisCompBuffer[1].Value_Linear.Axis23 < AxisCompBuffer[0].Value_Linear.Axis23)
		{	//¸ºÏò²¹³¥
			System.CompensationDirection.Axis23 = 0;
		}
		else if(AxisCompBuffer[1].Value_Linear.Axis23 > AxisCompBuffer[0].Value_Linear.Axis23)
		{	//ÕýÏò²¹³¥
			System.CompensationDirection.Axis23 = 1;
		}
		else
		{
			Error.CompDataCheckError = 1;
		}
		return 1;
	}
	return 1;
}




void Dsp_Com_InfoDgn(void)
{
	INT32U StartCount, TotalCount;
	GCODE *psNowCode;
	tsDSP_B6SUB *psB6Sub;
	tsDSP_B5SUB *psB5Sub;
	/*
	 psB5Sub=&psDSPInfoDgn->sInfoB5Buf[0];
	 psB6Sub=&psDSPInfoDgn->sInfoB6;

	 psDSPInfoDgn->ComID=0;
	 psDSPInfoDgn->CodeTotal=15;

	 psB6Sub->N = Error.RealRunCount;
	 psB6Sub->Pointer_StartCode = NC_AREAControl_Error.Pointer_StartCode;
	 psB6Sub->Pointer_EndCode = NC_AREAControl_Error.Pointer_EndCode;
	 psB6Sub->Pointer_FirstInflexionCode = NC_AREAControl_Error.Pointer_FirstInflexionCode;
	 psB6Sub->Pointer_SecondInflexionCode = NC_AREAControl_Error.Pointer_SecondInflexionCode;
	 psB6Sub->Ss = NC_AREAControl_Error.Ss;
	 psB6Sub->Stotal = divdp( NC_AREAControl_Error.Stotal, System.UnitTo_mm );
	 //psB6Sub->CurrentStotal = divdp( NC_AREAControl_Error.CurrentStotal, System.UnitTo_mm );
	 //psB6Sub->CurrentStotalLast = divdp( NC_AREAControl_Error.CurrentStotalLast, System.UnitTo_mm );
	 psB6Sub->Snow = divdp( NC_AREAControl_Error.Snow, System.UnitTo_mm );
	 psB6Sub->DeltaSnow = divdp( NC_AREAControl_Error.DeltaSnow, System.UnitTo_mm );
	 psB6Sub->I = NC_AREAControl_Error.I;
	 psB6Sub->TotalN = NC_AREAControl_Error.TotalN;
	 psB6Sub->Vs = NC_AREAControl_Error.Vs;
	 psB6Sub->Vm = NC_AREAControl_Error.Vm;
	 psB6Sub->VmReal = NC_AREAControl_Error.VmReal;
	 psB6Sub->Ve = NC_AREAControl_Error.Ve;
	 psB6Sub->VNow = NC_AREAControl_Error.VNow;
	 psB6Sub->VNowOut = NC_AREAControl_Error.VNowOut;
	 psB6Sub->Vmin = NC_AREAControl_Error.VMin;
	 psB6Sub->VChangeSign = NC_AREAControl_Error.VChangeSign;
	 psB6Sub->CurveClassNumber = 0;
	 psB6Sub->StotalReal= 0;
	 psB6Sub->StotalLast = NC_AREAControl_Error.SnowLast;
	 psB6Sub->Sac = NC_AREAControl_Error.Sac;
	 psB6Sub->Sm = NC_AREAControl_Error.Sm;
	 psB6Sub->Sde = NC_AREAControl_Error.Sde;
	 psB6Sub->SA = 0;
	 psB6Sub->S1 = NC_AREAControl_Error.S1;
	 psB6Sub->S2 = NC_AREAControl_Error.S2;
	 psB6Sub->S3 = NC_AREAControl_Error.S3;
	 psB6Sub->S4 = NC_AREAControl_Error.S4;
	 psB6Sub->S5 = NC_AREAControl_Error.S5;
	 psB6Sub->T1 = NC_AREAControl_Error.T1;
	 psB6Sub->T2 = NC_AREAControl_Error.T2;
	 psB6Sub->T3 = NC_AREAControl_Error.T3;
	 psB6Sub->T4= NC_AREAControl_Error.T4;
	 //psB6Sub->T5 = NC_AREAControl_Error.T5;
	 //psB6Sub->DeltaT1 = NC_AREAControl_Error.DeltaT1;
	 //psB6Sub->DeltaT2 = NC_AREAControl_Error.DeltaT2;
	 //psB6Sub->DeltaT3= NC_AREAControl_Error.DeltaT3;
	 //psB6Sub->DeltaT4 = NC_AREAControl_Error.DeltaT4;
	 //psB6Sub->DeltaT5 = NC_AREAControl_Error.DeltaT5;
	 //psB6Sub->a_ac = NC_AREAControl_Error.a_ac;
	 //psB6Sub->a_de = NC_AREAControl_Error.a_de;
	 //psB6Sub->CurrentT = NC_AREAControl_Error.CurrentT;
	 psB6Sub->sNowValue.Axis1 = NC_AREAControl_Error.NowValue.Axis1;
	 psB6Sub->sNowValue.Axis2 = NC_AREAControl_Error.NowValue.Axis2;
	 psB6Sub->sNowValue.Axis3 = NC_AREAControl_Error.NowValue.Axis3;
	 psB6Sub->sNowValue.Axis4 = 0;
	 psB6Sub->sNowValue.Axis5= 0;
	 psB6Sub->sNowValue.Axis6 = 0;
	 psB6Sub->sOldValue.Axis1 = NC_AREAControl_Error.DeltaStep.Axis1;
	 psB6Sub->sOldValue.Axis2 = NC_AREAControl_Error.DeltaStep.Axis2;
	 psB6Sub->sOldValue.Axis3 = NC_AREAControl_Error.DeltaStep.Axis3;
	 psB6Sub->sOldValue.Axis4 = 0;
	 psB6Sub->sOldValue.Axis5 = 0;
	 psB6Sub->sOldValue.Axis6 = 0;
	 psB6Sub->sNowPointMACPulse.Axis6 = 0;
	 psB6Sub->sNowPointMACPulseOut.Axis1 = NC_AREAControl_Error.NowPointMACHPulseOut.Axis1;
	 psB6Sub->sNowPointMACPulseOut.Axis2 = NC_AREAControl_Error.NowPointMACHPulseOut.Axis2;
	 psB6Sub->sNowPointMACPulseOut.Axis3 = NC_AREAControl_Error.NowPointMACHPulseOut.Axis3;
	 psB6Sub->sNowPointMACPulseOut.Axis4 = 0;
	 psB6Sub->sNowPointMACPulseOut.Axis5 = 0;
	 psB6Sub->sNowPointMACPulseOut.Axis6 = 0;
	 psB6Sub->sEndPointMACPulseOut.Axis1 = NC_AREAControl_Error.EndPointMachPulseOut.Axis1;
	 psB6Sub->sEndPointMACPulseOut.Axis2 = NC_AREAControl_Error.EndPointMachPulseOut.Axis2;
	 psB6Sub->sEndPointMACPulseOut.Axis3 = NC_AREAControl_Error.EndPointMachPulseOut.Axis3;
	 psB6Sub->sServoUnit.Axis1 = Encoder_Axis1.Resolution;
	 psB6Sub->sServoUnit.Axis2 = Encoder_Axis2.Resolution;
	 psB6Sub->sServoUnit.Axis3 = Encoder_Axis3.Resolution;
	 psB6Sub->sServoUnit.Axis4 = 0;
	 psB6Sub->sServoUnit.Axis5 = 0;
	 psB6Sub->sServoUnit.Axis6 =0;
	 psB6Sub->sSerVoPosition.Axis1 = Encoder_Axis1.MACH_PositionValue;
	 psB6Sub->sSerVoPosition.Axis2 = Encoder_Axis2.MACH_PositionValue;
	 psB6Sub->sSerVoPosition.Axis3 = Encoder_Axis3.MACH_PositionValue;
	 psB6Sub->sSerVoPosition.Axis4 = 0;
	 psB6Sub->sSerVoPosition.Axis5 = 0;
	 psB6Sub->sSerVoPosition.Axis6 = 0;
	 memset(psB6Sub->Bak,0x0,56*4);

	 STDNCSign.RealRunCount = Error.RealRunCount;
	 StartCount = (STDNCSign.RealRunCount>10)?(STDNCSign.RealRunCount-10):(0);
	 TotalCount=StartCount+psDSPInfoDgn->CodeTotal;
	 while(StartCount < TotalCount)
	 {
	 if(StartCount)
	 psNowCode= &GCodeBuffer[(StartCount-1)%STDGCODE_MOD];
	 else
	 psNowCode= &GCodeBuffer[0];
	 psB5Sub->SendCount=psNowCode->SendCount;
	 psB5Sub->CMDMain=psNowCode->CMDMain;
	 psB5Sub->CMDSub=psNowCode->SubCMD1;
	 psB5Sub->sPointS.Axis1=psNowCode->StartPoint.Axis1;
	 psB5Sub->sPointS.Axis2=psNowCode->StartPoint.Axis2;
	 psB5Sub->sPointS.Axis3=psNowCode->StartPoint.Axis3;
	 psB5Sub->sPointS.Axis4=0;
	 psB5Sub->sPointS.Axis5=0;
	 psB5Sub->sPointE.Axis1=psNowCode->EndPoint.Axis1;
	 psB5Sub->sPointE.Axis2=psNowCode->EndPoint.Axis2;
	 psB5Sub->sPointE.Axis3=psNowCode->EndPoint.Axis3;
	 psB5Sub->sPointE.Axis4=0;
	 psB5Sub->sPointE.Axis5=0;
	 psB5Sub->Stotal = divdp( psNowCode->Stotal, System.UnitTo_mm );
	 psB5Sub->FeedRate = psNowCode->Feedrate;
	 psB5Sub->Vm = psNowCode->Vm;
	 psB5Sub->Ve = psNowCode->Ve;

	 StartCount++;
	 psB5Sub++;
	 }
	 */

}

void Dsp_Com_InfoInterpolate(void) 
{

}

void Dsp_Com_AbsCoordinateInitial(void) 
{	
	//for test alpha	
	OverallSign.ABSORG_Coordinate.Axis1 = OverallSign.AbsoluteEncoderValue_PSO.Axis1;
	if(System.FunctionSelect05 == False)
	{
		OverallSign.ABSORG_Coordinate.Axis1 = 0;
	}

	OverallSign.ABSORG_Coordinate.Axis2 = OverallSign.AbsoluteEncoderValue_PSO.Axis2;
	OverallSign.ABSORG_Coordinate.Axis3 = OverallSign.AbsoluteEncoderValue_PSO.Axis3;
	OverallSign.ABSORG_Coordinate.Axis4 = OverallSign.AbsoluteEncoderValue_PSO.Axis4;
	OverallSign.ABSORG_Coordinate.Axis5 = OverallSign.AbsoluteEncoderValue_PSO.Axis5;
	OverallSign.ABSORG_Coordinate.Axis6 = OverallSign.AbsoluteEncoderValue_PSO.Axis6;
	OverallSign.ABSORG_Coordinate.Axis7 = OverallSign.AbsoluteEncoderValue_PSO.Axis7;
	OverallSign.ABSORG_Coordinate.Axis8 = OverallSign.AbsoluteEncoderValue_PSO.Axis8;
	OverallSign.ABSORG_Coordinate.Axis9 = OverallSign.AbsoluteEncoderValue_PSO.Axis9;
	OverallSign.ABSORG_Coordinate.Axis10 = OverallSign.AbsoluteEncoderValue_PSO.Axis10;
	OverallSign.ABSORG_Coordinate.Axis11 = OverallSign.AbsoluteEncoderValue_PSO.Axis11;
	OverallSign.ABSORG_Coordinate.Axis12 = OverallSign.AbsoluteEncoderValue_PSO.Axis12;
	OverallSign.ABSORG_Coordinate.Axis13 = OverallSign.AbsoluteEncoderValue_PSO.Axis13;
	OverallSign.ABSORG_Coordinate.Axis14 = OverallSign.AbsoluteEncoderValue_PSO.Axis14;
	OverallSign.ABSORG_Coordinate.Axis15 = OverallSign.AbsoluteEncoderValue_PSO.Axis15;
	OverallSign.ABSORG_Coordinate.Axis16 = OverallSign.AbsoluteEncoderValue_PSO.Axis16;
	OverallSign.ABSORG_Coordinate.Axis17 = OverallSign.AbsoluteEncoderValue_PSO.Axis17;
	OverallSign.ABSORG_Coordinate.Axis18 = OverallSign.AbsoluteEncoderValue_PSO.Axis18;
	OverallSign.ABSORG_Coordinate.Axis19 = OverallSign.AbsoluteEncoderValue_PSO.Axis19;
	OverallSign.ABSORG_Coordinate.Axis20 = OverallSign.AbsoluteEncoderValue_PSO.Axis20;
	OverallSign.ABSORG_Coordinate.Axis21 = OverallSign.AbsoluteEncoderValue_PSO.Axis21;
	OverallSign.ABSORG_Coordinate.Axis22 = OverallSign.AbsoluteEncoderValue_PSO.Axis22;
	OverallSign.ABSORG_Coordinate.Axis23 = OverallSign.AbsoluteEncoderValue_PSO.Axis23;
	
}

void Dsp_Com(void) 
{
	if (psSHMEvent->PacketARM) 
	{
		#if 0
		Dsp_Com_PacketIn_Initial();
		#endif

		Dsp_Com_PacketIn();
		psSHMEvent->PacketARM = 0;
		Dsp_Com_PacketOut();
		psSHMEvent->PacketARM = 1;
	}

	if (psSHMEvent->EepromARM) 
	{
		Dsp_Com_EepromARM();
		psSHMEvent->EepromARM = 0;
	}

	if (psSHMEvent->GCodeSPC)
	{
		Dsp_Com_CodeSPC();
		psSHMEvent->GCodeSPC = 0;
	}

	if (psSHMEvent->GCodeSTD) 
	{
		Dsp_Com_CodeSTD();
		psSHMEvent->GCodeSTD = 0;
	}

	if (psSHMEvent->AbsCoordinateInitial==2) 
	{
		Dsp_Com_AbsCoordinateInitial();
		psSHMEvent->AbsCoordinateInitial = 3;
		//psDSPPacketIn_InfoEvent->ServoOn = 1;
	}

	if (psSHMEvent->LensCompensationBuffer) 
	{
		static INT8U LensCompensationSign = 1;
		INT32U LensCompensationtimerDelta;
		
		LensCompensationtimerDelta = get_delta_time(Timer0.LensCompensationDeltaCounter);
		//Log_info1("LensCompensationtimerDelta = %d\n", LensCompensationtimerDelta);
			
		ParaInit_Linear();
		
		Timer0.LensCompensationDeltaCounter=Timer0.GenericCounter;
		
		psSHMEvent->LensCompensationBuffer = 0;
	}

	if (psSHMEvent->PositionCompensation) 
	{
		Dsp_Com_Compensation();
		psSHMEvent->PositionCompensation = 0;
	}	
}

void SCI_Init(void) 
{
	#if 0
	//Uart_Init();
	#endif

	Dsp_Init();
}

