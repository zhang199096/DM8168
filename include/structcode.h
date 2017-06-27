#ifndef _STRUCT_CODE_H_
#define _STRUCT_CODE_H_

#include "datatype.h"


typedef struct
{
	INT16U Length;		
	INT32U SendCountD;	
	INT32U SendCountA;
	
	INT32S CMDMain;
	INT32S SubCMD1;
	INT32S SubCMD2;
	INT32S HoldTime;

	INT32S Axis1;
	INT32S Axis2;
	INT32S Axis3;
	INT32S Axis4;
	INT32S Axis5;
	INT32S Axis6;	
	INT32S Axis7;
	INT32S Axis8;
	INT32S Axis9;
	INT32S Axis10;
	INT32S Axis11;
	INT32S Axis12;
	INT32S Axis13;
	INT32S Axis14;
	INT32S Axis15;
	INT32S Axis16;	
	INT32S Axis17;
	INT32S Axis18;
	INT32S Axis19;
	INT32S Axis20;
	INT32S Axis21;
	INT32S Axis22;
	INT32S Axis23;
	INT32S Axis24;
	INT32S Axis25;
	
	INT32S Feedrate_1;
	INT32S Feedrate_2;
	INT32S Feedrate_3;
	INT32S Feedrate_4;
	INT32S Feedrate_5;
	INT32S Feedrate_6;
	INT32S Feedrate_7;
	INT32S Feedrate_8;
	INT32S Feedrate_9;
	INT32S Feedrate_10;
	INT32S Feedrate_11;
	INT32S Feedrate_12;
	INT32S Feedrate_13;
	INT32S Feedrate_14;
	INT32S Feedrate_15;
	INT32S Feedrate_16;
	INT32S Feedrate_17;
	INT32S Feedrate_18;
	INT32S Feedrate_19;
	INT32S Feedrate_20;
	INT32S Feedrate_21;
	INT32S Feedrate_22;
	INT32S Feedrate_23;
	INT32S Feedrate_24;
	INT32S Feedrate_25;

	INT32U PacketCheckCode_B3;
}tsCODE_G00;
#endif
