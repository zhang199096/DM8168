#include "..\\include\\struct.h"

tsEepromParaBuf *psEepromPara_Arm;
tsEepromParaBuf *psEepromPara_Dsp;
#if 0
tsPARASERVO_BUF *psParaServo_Axis1;
tsPARASERVO_BUF *psParaServo_Axis2;
tsPARASERVO_BUF *psParaServo_Axis3;
tsPARASERVO_BUF *psParaServo_Axis4;
tsPARASERVO_BUF *psParaServo_Axis5;
tsPARASERVO_BUF *psParaServo_Axis6;
tsPARASERVO_BUF *psParaServo_Axis7;
tsPARASERVO_BUF *psParaServo_Axis8;
tsPARASERVO_BUF *psParaServo_Axis9;
tsPARASERVO_BUF *psParaServo_Axis10;
tsPARASERVO_BUF *psParaServo_Axis11;
tsPARASERVO_BUF *psParaServo_Axis12;
tsPARASERVO_BUF *psParaServo_Axis13;
tsPARASERVO_BUF *psParaServo_Axis14;
tsPARASERVO_BUF *psParaServo_Axis15;
tsPARASERVO_BUF *psParaServo_Axis16;
tsPARASERVO_BUF *psParaServo_Axis17;
tsPARASERVO_BUF *psParaServo_Axis18;
tsPARASERVO_BUF *psParaServo_Axis19;
tsPARASERVO_BUF *psParaServo_Axis20;
tsPARASERVO_BUF *psParaServo_Axis21;
tsPARASERVO_BUF *psParaServo_Axis22;
tsPARASERVO_BUF *psParaServo_Axis23;

tsPARAPP_BUF *psParaPP_Mode1;
tsPARAPP_BUF *psParaPP_Mode2;
tsPARAPP_BUF *psParaPP_Mode3;
tsPARAPP_BUF *psParaPP_Mode4;
tsPARAPP_BUF *psParaPP_Mode5;
tsPARAPP_BUF *psParaPP_Mode6;
#endif

tsPARAARMCFG_BUF *psParaArm_Cfg;
tsPARAARMCTRL_BUF *psParaArm_Ctrl;
tsPARAARMCTRL_BUF *psParaDsp_Ctrl;


#if 0
tsPARAARMIO_BUF *psParaArm_Io;
tsPARAARMTOOL_BUF *psParaArm_Tool;
tsPARAARMATC_BUF *psParaARM_Atc;

tsbDSP_JOGGRADE *psDSPJogGrade;
tsbDSP_HANDGRADE *psDSPHandGrade;
#endif

tsDSP_PACKETIN_INFOEVENT *psDSPPacketIn_InfoEvent;
tsDSP_PACKETOUT_INFOEVENT *psDSPPacketOut_InfoEvent;

tsDSP_PACKETIN *psDSPPacketIn;             
tsDSP_PACKETOUT *psDSPPacketOut;

tsDSP_CODESTD *psDSPCodeSTD;
tsDSP_CODESPC *psDSPCodeSPC;
tsDSP_CODESPC_SUB *psDSPCodeSPCSub;

tsDSP_INFODGN *psDSPInfoDgn;

#if 0
tsSPD_PACKETIN *psSPDPacketIn;             
tsSPD_PACKETOUT *psSPDPacketOut;     

tsKEY_PACKETINP *psKEYPacketIn;
tsKEY_PACKETVERSION *psKEYPacketVersion;
tsKEY_PACKETOUTP *psKEYPacketOut;

tsIO_PACKETIN *psIOPacketIn;
tsIO_PACKETVERSION *psIOPacketVersion;
tsIO_PACKETOUT *psIOPacketOut;
#endif

#if 0
tsPARASERVO_BUF sParaServo_Axis1;
tsPARASERVO_BUF sParaServo_Axis2;
tsPARASERVO_BUF sParaServo_Axis3;
tsPARASERVO_BUF sParaServo_Axis4;
tsPARASERVO_BUF sParaServo_Axis5;
tsPARASERVO_BUF sParaServo_Axis6;
tsPARASERVO_BUF sParaServo_Axis7;
tsPARASERVO_BUF sParaServo_Axis8;
tsPARASERVO_BUF sParaServo_Axis9;
tsPARASERVO_BUF sParaServo_Axis10;
tsPARASERVO_BUF sParaServo_Axis11;
tsPARASERVO_BUF sParaServo_Axis12;
tsPARASERVO_BUF sParaServo_Axis13;
tsPARASERVO_BUF sParaServo_Axis14;
tsPARASERVO_BUF sParaServo_Axis15;
tsPARASERVO_BUF sParaServo_Axis16;
tsPARASERVO_BUF sParaServo_Axis17;
tsPARASERVO_BUF sParaServo_Axis18;
tsPARASERVO_BUF sParaServo_Axis19;
tsPARASERVO_BUF sParaServo_Axis20;
tsPARASERVO_BUF sParaServo_Axis21;
tsPARASERVO_BUF sParaServo_Axis22;
tsPARASERVO_BUF sParaServo_Axis23;

tsPARAPP_BUF sParaPP_Mode1;
tsPARAPP_BUF sParaPP_Mode2;
tsPARAPP_BUF sParaPP_Mode3;
tsPARAPP_BUF sParaPP_Mode4;
tsPARAPP_BUF sParaPP_Mode5;
tsPARAPP_BUF sParaPP_Mode6;
#endif

tsPARAARMCFG_BUF sParaArm_Cfg;
tsPARAARMCTRL_BUF sParaArm_Ctrl;

#if 0
tsPARAARMIO_BUF sParaArm_Io;
tsPARAARMTOOL_BUF sParaArm_Tool;
tsPARAARMATC_BUF sParaARM_Atc;
#endif

tsDSP_PACKETIN sDSPPacketIn;             
tsDSP_PACKETOUT sDSPPacketOut;

#if 0
tsSPD_PACKETIN sSPDPackerIn;             
tsSPD_PACKETOUT sSPDPacketOut;     

tsKEY_PACKETINP sKEYPacketIn;
tsKEY_PACKETVERSION sKEYPacketVersion;
tsKEY_PACKETOUTP sKEYPacketOut;

tsIO_PACKETIN sIOPacketIn;
tsIO_PACKETVERSION sIOPacketVersion;
tsIO_PACKETOUT sIOPacketOut;

tsbDSP_JOGGRADE sbDSPJogGrade;        
tsbDSP_HANDGRADE sbDSPHandGrade;
#endif


