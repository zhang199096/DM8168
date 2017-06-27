/*
********************************************************************
* file: sharemem.h
* author: vinge
* description: share memory management functions;
               mostly cycle buf functions
* version: 2014/3/25 first edition
********************************************************************
*/
#include "../include/sharemem.h"
#include "../include/struct.h"

tsSHMEVENT *psSHMEvent;
tsSHMADDRESS sSHMAddress;

INT32S SHM_Init(void)
{
	sSHMAddress.pHead=(INT32U*)mSHM_ADDRESS;
	sSHMAddress.pEvent=sSHMAddress.pHead+(mSHM_OFFSET_EVENT>>2);
	sSHMAddress.pPacketARM=sSHMAddress.pHead+(mSHM_OFFSET_ARM>>2);
	sSHMAddress.pPacketDSP=sSHMAddress.pHead+(mSHM_OFFSET_DSP>>2);
	sSHMAddress.pEepromARM=sSHMAddress.pHead+(mSHM_OFFSET_EEPROMARM>>2);
	sSHMAddress.pEepromDSP=sSHMAddress.pHead+(mSHM_OFFSET_EEPROMDSP>>2);
	sSHMAddress.pCodeSPC=sSHMAddress.pHead+(mSHM_OFFSET_SPC>>2);
	sSHMAddress.pCodeSTD=sSHMAddress.pHead+(mSHM_OFFSET_STD>>2);
	sSHMAddress.pInfoDgn=sSHMAddress.pHead+(mSHM_OFFSET_DGN>>2);
	sSHMAddress.pInfoInterpolate=sSHMAddress.pHead+(mSHM_OFFSET_INP>>2);	
	psSHMEvent=(tsSHMEVENT *)sSHMAddress.pEvent;
	memset(psSHMEvent,0x0,sizeof(tsSHMEVENT));	
	return 1;
	//SPI_Read(0, (INT8U *)sSHMAddress.pEepromDSP, 128*1024);
}

INT32S Struct_Init(void)
{
	SHM_Init();
	
	psEepromPara_Arm=(tsEepromParaBuf *)sSHMAddress.pEepromARM;
	psEepromPara_Dsp=(tsEepromParaBuf *)sSHMAddress.pEepromDSP;

	#if 0
	psParaServo_Axis1=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis1;
	psParaServo_Axis2=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis2;
	psParaServo_Axis3=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis3;
	psParaServo_Axis4=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis4;
	psParaServo_Axis5=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis5;
	psParaServo_Axis6=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis6;
	psParaServo_Axis7=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis7;
	psParaServo_Axis8=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis8;
	psParaServo_Axis9=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis9;
	psParaServo_Axis10=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis10;
	psParaServo_Axis11=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis11;
	psParaServo_Axis12=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis12;
	psParaServo_Axis13=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis13;
	psParaServo_Axis14=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis14;
	psParaServo_Axis15=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis15;
	psParaServo_Axis16=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis16;
	psParaServo_Axis17=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis17;
	psParaServo_Axis18=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis18;
	psParaServo_Axis19=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis19;
	psParaServo_Axis20=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis20;
	psParaServo_Axis21=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis21;
	psParaServo_Axis22=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis22;
	psParaServo_Axis23=(tsPARASERVO_BUF *)psEepromPara_Arm->DspServoAxis23;

	psParaPP_Mode1=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode1;
	psParaPP_Mode2=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode2;
	psParaPP_Mode3=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode3;
	psParaPP_Mode4=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode4;
	psParaPP_Mode5=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode5;
	psParaPP_Mode6=(tsPARAPP_BUF *)psEepromPara_Arm->PPMode6;
	#endif
	
	psParaArm_Cfg=(tsPARAARMCFG_BUF *)psEepromPara_Arm->ArmCFG;	
	psParaArm_Ctrl=(tsPARAARMCTRL_BUF *)psEepromPara_Arm->ArmCTRL;
	
	psParaDsp_Ctrl=(tsPARAARMCTRL_BUF *)psEepromPara_Dsp->ArmCTRL;
	#if 0
	psParaArm_Io=(tsPARAARMIO_BUF *)psEepromPara_Arm->ArmIO;
	psParaArm_Tool=(tsPARAARMTOOL_BUF *)psEepromPara_Arm->ArmTOOL;
	psParaARM_Atc=(tsPARAARMATC_BUF *)psEepromPara_Arm->ArmATC;
	#endif

	psDSPPacketIn=(tsDSP_PACKETIN *)sSHMAddress.pPacketARM;
	psDSPPacketIn_InfoEvent=(tsDSP_PACKETIN_INFOEVENT *)psDSPPacketIn->InfoBuf;
	
	psDSPPacketOut=(tsDSP_PACKETOUT *)sSHMAddress.pPacketDSP;
	psDSPPacketOut_InfoEvent=(tsDSP_PACKETOUT_INFOEVENT *)psDSPPacketOut->InfoBuf;

	psDSPCodeSTD=(tsDSP_CODESTD *)sSHMAddress.pCodeSTD;
	psDSPCodeSPC=(tsDSP_CODESPC *)sSHMAddress.pCodeSPC;	

	psDSPInfoDgn=(tsDSP_INFODGN *)sSHMAddress.pInfoDgn;

	return 1;
}

