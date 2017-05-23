#ifndef __STRUCT_H__
#define __STRUCT_H__

#include "datatype.h"
#include "..\\include\\Structcode.h"
#include "..\\include\\Structpacket.h"
#include "..\\include\\Structpara.h"
#include "..\\include\\Structpoint.h"
#include "..\\include\\DEFEEprom.h"

extern tsEepromParaBuf *psEepromPara_Arm;
extern tsEepromParaBuf *psEepromPara_Dsp;

#if 0
extern tsPARASERVO_BUF *psParaServo_Axis1;
extern tsPARASERVO_BUF *psParaServo_Axis2;
extern tsPARASERVO_BUF *psParaServo_Axis3;
extern tsPARASERVO_BUF *psParaServo_Axis4;
extern tsPARASERVO_BUF *psParaServo_Axis5;
extern tsPARASERVO_BUF *psParaServo_Axis6;
extern tsPARASERVO_BUF *psParaServo_Axis7;
extern tsPARASERVO_BUF *psParaServo_Axis8;
extern tsPARASERVO_BUF *psParaServo_Axis9;
extern tsPARASERVO_BUF *psParaServo_Axis10;
extern tsPARASERVO_BUF *psParaServo_Axis11;
extern tsPARASERVO_BUF *psParaServo_Axis12;
extern tsPARASERVO_BUF *psParaServo_Axis13;
extern tsPARASERVO_BUF *psParaServo_Axis14;
extern tsPARASERVO_BUF *psParaServo_Axis15;
extern tsPARASERVO_BUF *psParaServo_Axis16;
extern tsPARASERVO_BUF *psParaServo_Axis17;
extern tsPARASERVO_BUF *psParaServo_Axis18;
extern tsPARASERVO_BUF *psParaServo_Axis19;
extern tsPARASERVO_BUF *psParaServo_Axis20;
extern tsPARASERVO_BUF *psParaServo_Axis21;
extern tsPARASERVO_BUF *psParaServo_Axis22;
extern tsPARASERVO_BUF *psParaServo_Axis23;

extern tsPARAPP_BUF *psParaPP_Mode1;
extern tsPARAPP_BUF *psParaPP_Mode2;
extern tsPARAPP_BUF *psParaPP_Mode3;
extern tsPARAPP_BUF *psParaPP_Mode4;
extern tsPARAPP_BUF *psParaPP_Mode5;
extern tsPARAPP_BUF *psParaPP_Mode6;
#endif

extern tsPARAARMCFG_BUF *psParaArm_Cfg;
extern tsPARAARMCTRL_BUF *psParaArm_Ctrl;
extern tsPARAARMCTRL_BUF *psParaDsp_Ctrl;

#if 0
extern tsPARAARMIO_BUF *psParaArm_Io;
extern tsPARAARMTOOL_BUF *psParaArm_Tool;
extern tsPARAARMATC_BUF *psParaARM_Atc;

extern tsbDSP_JOGGRADE *psDSPJogGrade;
extern tsbDSP_HANDGRADE *psDSPHandGrade;
#endif

extern tsDSP_PACKETIN_INFOEVENT *psDSPPacketIn_InfoEvent;
extern tsDSP_PACKETOUT_INFOEVENT *psDSPPacketOut_InfoEvent;

extern tsDSP_PACKETIN *psDSPPacketIn;             
extern tsDSP_PACKETOUT *psDSPPacketOut;

extern tsDSP_CODESTD *psDSPCodeSTD;
extern tsDSP_CODESPC *psDSPCodeSPC;
extern tsDSP_CODESPC_SUB *psDSPCodeSPCSub;

extern tsDSP_INFODGN *psDSPInfoDgn;

#if 0
extern tsSPD_PACKETIN *psSPDPacketIn;             
extern tsSPD_PACKETOUT *psSPDPacketOut;     

extern tsKEY_PACKETINP *psKEYPacketIn;
extern tsKEY_PACKETVERSION *psKEYPacketVersion;
extern tsKEY_PACKETOUTP *psKEYPacketOut;

extern tsIO_PACKETIN *psIOPacketIn;
extern tsIO_PACKETVERSION *psIOPacketVersion;
extern tsIO_PACKETOUT *psIOPacketOut;
#endif

#if 0
extern tsPARASERVO_BUF sParaServo_Axis1;
extern tsPARASERVO_BUF sParaServo_Axis2;
extern tsPARASERVO_BUF sParaServo_Axis3;
extern tsPARASERVO_BUF sParaServo_Axis4;
extern tsPARASERVO_BUF sParaServo_Axis5;
extern tsPARASERVO_BUF sParaServo_Axis6;
extern tsPARASERVO_BUF sParaServo_Axis7;
extern tsPARASERVO_BUF sParaServo_Axis8;
extern tsPARASERVO_BUF sParaServo_Axis9;
extern tsPARASERVO_BUF sParaServo_Axis10;
extern tsPARASERVO_BUF sParaServo_Axis11;
extern tsPARASERVO_BUF sParaServo_Axis12;
extern tsPARASERVO_BUF sParaServo_Axis13;
extern tsPARASERVO_BUF sParaServo_Axis14;
extern tsPARASERVO_BUF sParaServo_Axis15;
extern tsPARASERVO_BUF sParaServo_Axis16;
extern tsPARASERVO_BUF sParaServo_Axis17;
extern tsPARASERVO_BUF sParaServo_Axis18;
extern tsPARASERVO_BUF sParaServo_Axis19;
extern tsPARASERVO_BUF sParaServo_Axis20;
extern tsPARASERVO_BUF sParaServo_Axis21;
extern tsPARASERVO_BUF sParaServo_Axis22;
extern tsPARASERVO_BUF sParaServo_Axis23;


extern tsPARAPP_BUF sParaPP_Mode1;
extern tsPARAPP_BUF sParaPP_Mode2;
extern tsPARAPP_BUF sParaPP_Mode3;
extern tsPARAPP_BUF sParaPP_Mode4;
extern tsPARAPP_BUF sParaPP_Mode5;
extern tsPARAPP_BUF sParaPP_Mode6;
#endif

extern tsPARAARMCFG_BUF sParaArm_Cfg;
extern tsPARAARMCTRL_BUF sParaArm_Ctrl;
extern tsPARAARMCTRL_BUF sParaDsp_Ctrl;

#if 0
extern tsPARAARMIO_BUF sParaArm_Io;
extern tsPARAARMTOOL_BUF sParaArm_Tool;
extern tsPARAARMATC_BUF sParaARM_Atc;
#endif

extern tsDSP_PACKETIN sDSPPacketIn;             
extern tsDSP_PACKETOUT sDSPPacketOut;

extern tsSPD_PACKETIN sSPDPackerIn;             
extern tsSPD_PACKETOUT sSPDPacketOut;     

#if 0
extern tsKEY_PACKETINP sKEYPacketIn;
extern tsKEY_PACKETVERSION sKEYPacketVersion;
extern tsKEY_PACKETOUTP sKEYPacketOut;

extern tsIO_PACKETIN sIOPacketIn;
extern tsIO_PACKETVERSION sIOPacketVersion;
extern tsIO_PACKETOUT sIOPacketOut;

extern tsbDSP_JOGGRADE sbDSPJogGrade;        
extern tsbDSP_HANDGRADE sbDSPHandGrade;
#endif

#endif
