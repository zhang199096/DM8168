/*
**************************************************************
* file: sharemem.h
* author: rex chen
* description: share memory management functions;mostly cycle buf functions
* version: 2014/3/25 first edition
***************************************************************
*/
#ifndef _SHAREMEM_H_
#define _SHAREMEM_H_

#include "datatype.h"
#include "..\\include\\Struct.h"

typedef struct
{
	INT8U PacketARM;
	INT8U PacketDSP;
	INT8U EepromARM;
	INT8U EepromDSP;
	INT8U GCodeSPC;
	INT8U GCodeSTD;
	INT8U PositionCompensation;	
	INT8U InfoDgn;
	INT8U InfoInterpolate;
	INT8U LensCompensationBuffer;
	INT8U AbsCoordinateInitial;
}tsSHMEVENT;

typedef struct
{
	INT32U* pHead; 		// 8m  share memory
	INT32U* pEvent;
	INT32U* pPacketARM;
	INT32U* pPacketDSP;
	INT32U* pEepromARM;
	INT32U* pEepromDSP;
	INT32U* pCodeSPC;
	INT32U* pCodeSTD;
	INT32U* pInfoDgn;
	INT32U* pInfoInterpolate;
}tsSHMADDRESS;

#define mSHM_ADDRESS 			0X98000000
#define mSHM_SIZE       			1024*1024*16  //16M

#define mSHM_OFFSET_EVENT			0x0
#define mSHM_OFFSET_ARM			0x1000
#define mSHM_OFFSET_DSP			0x2000
#define mSHM_OFFSET_EEPROMARM	0x10000
#define mSHM_OFFSET_EEPROMDSP	0x30000
#define mSHM_OFFSET_SPC			0x100000
#define mSHM_OFFSET_STD			0x180000
#define mSHM_OFFSET_DGN			0x200000
#define mSHM_OFFSET_INP			0x400000

extern tsSHMEVENT *psSHMEvent;
extern tsSHMADDRESS sSHMAddress;

INT32S SHM_Init(void);
INT32S Struct_Init(void);

#endif
