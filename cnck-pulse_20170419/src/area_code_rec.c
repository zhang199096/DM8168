#include "..\\include\\main_dsp.h"

GCODE  *psGCodeBuffer;

void STD_Deal_DeltaPoint(GCODE  *psNowCode)
{	
	psNowCode->DeltaPoint.Axis15 = (psNowCode->EndPoint.Axis15-psNowCode->StartPoint.Axis15)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis16 = (psNowCode->EndPoint.Axis16-psNowCode->StartPoint.Axis16)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis1 = (psNowCode->EndPoint.Axis1-psNowCode->StartPoint.Axis1)*System.UnitTo_mm;//mm
	//psNowCode->DeltaPoint.Axis2 = (psNowCode->EndPoint.Axis2-psNowCode->StartPoint.Axis2)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis15,psNowCode->DeltaPoint.Axis16,psNowCode->DeltaPoint.Axis1,0);
	psNowCode->Stotal_Module.Module1 = psNowCode->Stotal;
		
	psNowCode->DeltaPoint.Axis17 = (psNowCode->EndPoint.Axis17-psNowCode->StartPoint.Axis17)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis18 = (psNowCode->EndPoint.Axis18-psNowCode->StartPoint.Axis18)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis17,psNowCode->DeltaPoint.Axis18,0,0);
	psNowCode->Stotal_Module.Module2 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis3 = (psNowCode->EndPoint.Axis3-psNowCode->StartPoint.Axis3)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis4 = (psNowCode->EndPoint.Axis4-psNowCode->StartPoint.Axis4)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis5 = (psNowCode->EndPoint.Axis5-psNowCode->StartPoint.Axis5)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis3,psNowCode->DeltaPoint.Axis4,psNowCode->DeltaPoint.Axis5,0);
	psNowCode->Stotal_Module.Module3 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis8 = (psNowCode->EndPoint.Axis8-psNowCode->StartPoint.Axis8)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis7 = (psNowCode->EndPoint.Axis7-psNowCode->StartPoint.Axis7)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis8,psNowCode->DeltaPoint.Axis7,0,0);
	psNowCode->Stotal_Module.Module4 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis6 = (psNowCode->EndPoint.Axis6-psNowCode->StartPoint.Axis6)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis20 = (psNowCode->EndPoint.Axis20-psNowCode->StartPoint.Axis20)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis6,psNowCode->DeltaPoint.Axis20,0,0);
	psNowCode->Stotal_Module.Module5 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis11 = (psNowCode->EndPoint.Axis11-psNowCode->StartPoint.Axis11)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis12 = (psNowCode->EndPoint.Axis12-psNowCode->StartPoint.Axis12)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis11,psNowCode->DeltaPoint.Axis12,0,0);
	psNowCode->Stotal_Module.Module6 = psNowCode->Stotal;


	psNowCode->DeltaPoint.Axis21 = (psNowCode->EndPoint.Axis21-psNowCode->StartPoint.Axis21)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis22 = (psNowCode->EndPoint.Axis22-psNowCode->StartPoint.Axis22)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis9 = (psNowCode->EndPoint.Axis9-psNowCode->StartPoint.Axis9)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis21,psNowCode->DeltaPoint.Axis22,psNowCode->DeltaPoint.Axis9,0);
	psNowCode->Stotal_Module.Module7 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis2 = (psNowCode->EndPoint.Axis2-psNowCode->StartPoint.Axis2)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis2,0,0,0);
	psNowCode->Stotal_Module.Module8 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis10 = (psNowCode->EndPoint.Axis10-psNowCode->StartPoint.Axis10)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis10,0,0,0);
	psNowCode->Stotal_Module.Module9 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis13 = (psNowCode->EndPoint.Axis13-psNowCode->StartPoint.Axis13)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis14 = (psNowCode->EndPoint.Axis14-psNowCode->StartPoint.Axis14)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis13,psNowCode->DeltaPoint.Axis14,0,0);
	psNowCode->Stotal_Module.Module10 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis19 = (psNowCode->EndPoint.Axis19-psNowCode->StartPoint.Axis19)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis19,0,0,0);
	psNowCode->Stotal_Module.Module11 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis23 = (psNowCode->EndPoint.Axis23-psNowCode->StartPoint.Axis23)*System.UnitTo_mm;//mm
	FourAxisInterpolatAlgorithm(psNowCode,psNowCode->DeltaPoint.Axis23,0,0,0);
	psNowCode->Stotal_Module.Module12 = psNowCode->Stotal;

	psNowCode->DeltaPoint.Axis24 = (psNowCode->EndPoint.Axis24-psNowCode->StartPoint.Axis24)*System.UnitTo_mm;//mm
	psNowCode->DeltaPoint.Axis25 = (psNowCode->EndPoint.Axis25-psNowCode->StartPoint.Axis25)*System.UnitTo_mm;//mm


	psNowCode->Stotal = psNowCode->Stotal_Module.Module1+psNowCode->Stotal_Module.Module2+psNowCode->Stotal_Module.Module3+
						psNowCode->Stotal_Module.Module4+psNowCode->Stotal_Module.Module5+psNowCode->Stotal_Module.Module6+
						psNowCode->Stotal_Module.Module7+psNowCode->Stotal_Module.Module8+psNowCode->Stotal_Module.Module9+
						psNowCode->Stotal_Module.Module10+psNowCode->Stotal_Module.Module11+psNowCode->Stotal_Module.Module12
						;

}

// æ­¤å¤„è®¡ç®—NowCode->StPointVector ,NowCode->EndPointVectorä¸º é€šç”¨é€”
// 1.ç›´çº¿æˆ–åœ†å¼§èµ·ç‚¹ä½™å¼¦çŸ¢é‡å€¼,ç”¨äºŽä¸Šæ¡è¿åŠ¨ä»£ç ç»ˆç‚¹å…è®¸é€Ÿåº¦
// 2.ç›´çº¿æˆ–åœ†å¼§ç»ˆç‚¹ä½™å¼¦çŸ¢é‡å€¼,ç”¨äºŽä¸‹æ¡è¿åŠ¨ä»£ç èµ·ç‚¹å…è®¸é€Ÿåº¦
void STD_Deal_Vveer(GCODE *psNowCode)
{
	Uint16 ReturnValue;
	ReturnValue = 0;
	
	if(psNowCode->Stotal < 0.00001)
	{
		psNowCode->Stotal  = 0;
		psNowCode->Vveer_Module.Module1 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module2 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module3 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module4 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module5 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module6 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module7 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module8 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module9 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module10 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module11 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module12 = psNowCode->Feedrate*2;
		psNowCode->CMDMain = 0x64;
		psNowCode->SubCMD1 = 0xff;		
		psNowCode->MoveCodeSign = 0;
	}

	if((psNowCode->Stotal_Module.Module1 < 0.001)&&
		(psNowCode->Stotal_Module.Module2 < 0.001)&&
		(psNowCode->Stotal_Module.Module3 < 0.001)&&
		(psNowCode->Stotal_Module.Module4 < 0.001)&&
		(psNowCode->Stotal_Module.Module5 < 0.001)&&
		(psNowCode->Stotal_Module.Module6 < 0.001)&&
		(psNowCode->Stotal_Module.Module7 < 0.001)	&&
		(psNowCode->Stotal_Module.Module8 < 0.001)&&
		(psNowCode->Stotal_Module.Module9 < 0.001)&&
		(psNowCode->Stotal_Module.Module10 < 0.001)&&
		(psNowCode->Stotal_Module.Module11 < 0.001)&&
		(psNowCode->Stotal_Module.Module12 < 0.001)		
		)
	{
		psNowCode->Stotal  = 0;
		psNowCode->Vveer_Module.Module1 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module2 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module3 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module4 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module5 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module6 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module7 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module8 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module9 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module10 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module11 = psNowCode->Feedrate*2;
		psNowCode->Vveer_Module.Module12 = psNowCode->Feedrate*2;

		psNowCode->CMDMain = 0x64;
		psNowCode->SubCMD1 = 0xff;		
		psNowCode->MoveCodeSign = 0;
	}
	
	psNowCode->StPointVector = OverallSign.LastMoveCodeEndVector;

	if(psNowCode->Stotal_Module.Module1 > 0.0001)
	{
		psNowCode->StPointVector.Axis15 = divdp(psNowCode->DeltaPoint.Axis15, psNowCode->Stotal_Module.Module1);
		psNowCode->StPointVector.Axis16 = divdp(psNowCode->DeltaPoint.Axis16, psNowCode->Stotal_Module.Module1);
		psNowCode->StPointVector.Axis1 = divdp(psNowCode->DeltaPoint.Axis1, psNowCode->Stotal_Module.Module1);
		//psNowCode->StPointVector.Axis2 = divdp(psNowCode->DeltaPoint.Axis2, psNowCode->Stotal_Module.Module1);
	}
	else
	{
		psNowCode->Stotal_Module.Module1 = 0;
	}

	
	if(psNowCode->Stotal_Module.Module2 > 0.0001)
	{
		psNowCode->StPointVector.Axis17 = divdp(psNowCode->DeltaPoint.Axis17, psNowCode->Stotal_Module.Module2);
		psNowCode->StPointVector.Axis18 = divdp(psNowCode->DeltaPoint.Axis18, psNowCode->Stotal_Module.Module2);
	}
	else
	{
		psNowCode->Stotal_Module.Module2 = 0;
	}
	
	if(psNowCode->Stotal_Module.Module3 > 0.0001)
	{	
		psNowCode->StPointVector.Axis3 = divdp(psNowCode->DeltaPoint.Axis3, psNowCode->Stotal_Module.Module3);
		psNowCode->StPointVector.Axis4 = divdp(psNowCode->DeltaPoint.Axis4, psNowCode->Stotal_Module.Module3);
		psNowCode->StPointVector.Axis5 = divdp(psNowCode->DeltaPoint.Axis5, psNowCode->Stotal_Module.Module3);
	}
	else
	{
		psNowCode->Stotal_Module.Module3 = 0;
	}

	if(psNowCode->Stotal_Module.Module4 > 0.0001)
	{
		psNowCode->StPointVector.Axis8 = divdp(psNowCode->DeltaPoint.Axis8, psNowCode->Stotal_Module.Module4);
		psNowCode->StPointVector.Axis7 = divdp(psNowCode->DeltaPoint.Axis7, psNowCode->Stotal_Module.Module4);
	}
	else
	{
		psNowCode->Stotal_Module.Module4 = 0;
	}

	if(psNowCode->Stotal_Module.Module5 > 0.0001)
	{
		psNowCode->StPointVector.Axis6 = divdp(psNowCode->DeltaPoint.Axis6, psNowCode->Stotal_Module.Module5);
		psNowCode->StPointVector.Axis20 = divdp(psNowCode->DeltaPoint.Axis20, psNowCode->Stotal_Module.Module5);
	}
	else
	{
		psNowCode->Stotal_Module.Module5 = 0;
	}

	if(psNowCode->Stotal_Module.Module6 > 0.0001)
	{	
		psNowCode->StPointVector.Axis11 = divdp(psNowCode->DeltaPoint.Axis11, psNowCode->Stotal_Module.Module6);
		psNowCode->StPointVector.Axis12 = divdp(psNowCode->DeltaPoint.Axis12, psNowCode->Stotal_Module.Module6);
	}
	else
	{
		psNowCode->Stotal_Module.Module6 = 0;
	}

	
	if(psNowCode->Stotal_Module.Module7 > 0.0001)
	{
		psNowCode->StPointVector.Axis21 = divdp(psNowCode->DeltaPoint.Axis21, psNowCode->Stotal_Module.Module7);
		psNowCode->StPointVector.Axis22 = divdp(psNowCode->DeltaPoint.Axis22, psNowCode->Stotal_Module.Module7);
		psNowCode->StPointVector.Axis9 = divdp(psNowCode->DeltaPoint.Axis9, psNowCode->Stotal_Module.Module7);
	}
	else
	{
		psNowCode->Stotal_Module.Module7 = 0;
	}

	if(psNowCode->Stotal_Module.Module8 > 0.0001)
	{
		psNowCode->StPointVector.Axis2 = divdp(psNowCode->DeltaPoint.Axis2, psNowCode->Stotal_Module.Module8);
	}
	else
	{
		psNowCode->Stotal_Module.Module8 = 0;
	}

	if(psNowCode->Stotal_Module.Module9 > 0.0001)
	{
		psNowCode->StPointVector.Axis10 = divdp(psNowCode->DeltaPoint.Axis10, psNowCode->Stotal_Module.Module9);
	}
	else
	{
		psNowCode->Stotal_Module.Module9 = 0;
	}

	if(psNowCode->Stotal_Module.Module10 > 0.0001)
	{
		psNowCode->StPointVector.Axis13 = divdp(psNowCode->DeltaPoint.Axis13, psNowCode->Stotal_Module.Module10);
		psNowCode->StPointVector.Axis14 = divdp(psNowCode->DeltaPoint.Axis14, psNowCode->Stotal_Module.Module10);
	}
	else
	{
		psNowCode->Stotal_Module.Module10 = 0;
	}

	if(psNowCode->Stotal_Module.Module11 > 0.0001)
	{
		psNowCode->StPointVector.Axis19 = divdp(psNowCode->DeltaPoint.Axis19, psNowCode->Stotal_Module.Module8);
	}
	else
	{
		psNowCode->Stotal_Module.Module11 = 0;
	}

	
	if(psNowCode->Stotal_Module.Module12 > 0.0001)
	{
		psNowCode->StPointVector.Axis23 = divdp(psNowCode->DeltaPoint.Axis23, psNowCode->Stotal_Module.Module8);
	}
	else
	{
		psNowCode->Stotal_Module.Module12 = 0;
	}

	psNowCode->EndPointVector = psNowCode->StPointVector; 
	OverallSign.LastMoveCodeEndVector = psNowCode->StPointVector;
	OverallSign.STDLastMoveGCode.EndPoint = psNowCode->EndPoint;
	
	psNowCode->StPointVector.Axis24 = 0.575;
	psNowCode->StPointVector.Axis25 = 0.575;
	psNowCode->EndPointVector.Axis24 = 0.575; 
	psNowCode->EndPointVector.Axis25 = 0.575;
	OverallSign.LastMoveCodeEndVector.Axis24 = 0.575;
	OverallSign.LastMoveCodeEndVector.Axis25 = 0.575;
	OverallSign.STDLastMoveGCode.EndPoint.Axis24 = 0.575;
	OverallSign.STDLastMoveGCode.EndPoint.Axis25 = 0.575;		
}

void STD_Rec_G00(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)
{	
	Int32 Feedrate;
	
	GCodeBuffer->EndPoint.Axis15 = psCodeG00->Axis15;
	GCodeBuffer->EndPoint.Axis16 = psCodeG00->Axis16;
	GCodeBuffer->EndPoint.Axis1 = psCodeG00->Axis1;
	//GCodeBuffer->EndPoint.Axis2 = psCodeG00->Axis2;

	GCodeBuffer->EndPoint.Axis17 = psCodeG00->Axis17;
	GCodeBuffer->EndPoint.Axis18 = psCodeG00->Axis18;

	GCodeBuffer->EndPoint.Axis3 = psCodeG00->Axis3;
	GCodeBuffer->EndPoint.Axis4 = psCodeG00->Axis4;
	GCodeBuffer->EndPoint.Axis5 = psCodeG00->Axis5;

	GCodeBuffer->EndPoint.Axis8 = psCodeG00->Axis8;
	GCodeBuffer->EndPoint.Axis7 = psCodeG00->Axis7;

	GCodeBuffer->EndPoint.Axis6 = psCodeG00->Axis6;
	GCodeBuffer->EndPoint.Axis20 = psCodeG00->Axis20;

	GCodeBuffer->EndPoint.Axis11 = psCodeG00->Axis11;
	GCodeBuffer->EndPoint.Axis12 = psCodeG00->Axis12;

	GCodeBuffer->EndPoint.Axis21 = psCodeG00->Axis21;
	GCodeBuffer->EndPoint.Axis22 = psCodeG00->Axis22;
	GCodeBuffer->EndPoint.Axis9 = psCodeG00->Axis9;
	
	GCodeBuffer->EndPoint.Axis2 = psCodeG00->Axis2;

	GCodeBuffer->EndPoint.Axis10 = psCodeG00->Axis10;

	GCodeBuffer->EndPoint.Axis13 = psCodeG00->Axis13;
	GCodeBuffer->EndPoint.Axis14 = psCodeG00->Axis14;
	
	GCodeBuffer->EndPoint.Axis19 = psCodeG00->Axis19;
	
	GCodeBuffer->EndPoint.Axis23 = psCodeG00->Axis23;
	
	GCodeBuffer->EndPoint.Axis24 = psCodeG00->Axis24;
	GCodeBuffer->EndPoint.Axis25 = psCodeG00->Axis25;
	
	GCodeBuffer->StartPoint = OverallSign.STDLastMoveGCode.EndPoint;		
	
	//GCodeBuffer->Feedrate = (GCodeBuffer->CMDMain ==0)?(STDNCSign.FG0):(STDNCSign.F);

	Feedrate = 0;
	Feedrate = (psCodeG00->Feedrate_15>psCodeG00->Feedrate_16)?(psCodeG00->Feedrate_15):(psCodeG00->Feedrate_16);
	Feedrate = (Feedrate > psCodeG00->Feedrate_1)?(Feedrate):(psCodeG00->Feedrate_1);		
	GCodeBuffer->Feedrate_Module.Module1 = Feedrate;//15/16/1
	
	GCodeBuffer->Feedrate_Module.Module2 = (psCodeG00->Feedrate_17>psCodeG00->Feedrate_18)?(psCodeG00->Feedrate_17):(psCodeG00->Feedrate_18);//17/18

	Feedrate = 0;
	Feedrate = (psCodeG00->Feedrate_3>psCodeG00->Feedrate_4)?(psCodeG00->Feedrate_3):(psCodeG00->Feedrate_4);
	Feedrate = (Feedrate > psCodeG00->Feedrate_5)?(Feedrate):(psCodeG00->Feedrate_5);		
	GCodeBuffer->Feedrate_Module.Module3 = Feedrate;//3/4/5

	GCodeBuffer->Feedrate_Module.Module4 =(psCodeG00->Feedrate_8>psCodeG00->Feedrate_7)?(psCodeG00->Feedrate_8):(psCodeG00->Feedrate_7);//8/7

	GCodeBuffer->Feedrate_Module.Module5 = (psCodeG00->Feedrate_6>psCodeG00->Feedrate_20)?(psCodeG00->Feedrate_6):(psCodeG00->Feedrate_20);//6/20
	GCodeBuffer->Feedrate_Module.Module6 = (psCodeG00->Feedrate_11>psCodeG00->Feedrate_12)?(psCodeG00->Feedrate_11):(psCodeG00->Feedrate_12);//11/12

	Feedrate = 0;
	Feedrate = (psCodeG00->Feedrate_21>psCodeG00->Feedrate_22)?(psCodeG00->Feedrate_21):(psCodeG00->Feedrate_22);
	Feedrate = (Feedrate > psCodeG00->Feedrate_9)?(Feedrate):(psCodeG00->Feedrate_9);		
	GCodeBuffer->Feedrate_Module.Module7 = Feedrate;//21/22/9 
	
	GCodeBuffer->Feedrate_Module.Module8 = psCodeG00->Feedrate_2;// 2
	GCodeBuffer->Feedrate_Module.Module9 = psCodeG00->Feedrate_10;// 10
	
	GCodeBuffer->Feedrate_Module.Module10 = (psCodeG00->Feedrate_13>psCodeG00->Feedrate_14)?(psCodeG00->Feedrate_13):(psCodeG00->Feedrate_14);//13/14
	
	GCodeBuffer->Feedrate_Module.Module11 = psCodeG00->Feedrate_19;//19
	GCodeBuffer->Feedrate_Module.Module12 = psCodeG00->Feedrate_23;//23

	STD_Deal_DeltaPoint(GCodeBuffer);	
		
	STD_Deal_Vveer(GCodeBuffer);

	if(GCodeBuffer->CMDMain == 0x64)
	{
		return ;	
	}
	
	OverallSign.LastMoveCodeEndVector = GCodeBuffer->EndPointVector;	
	OverallSign.STDLastMoveGCode.EndPoint = GCodeBuffer->EndPoint;
}

void STD_Rec_G04(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)		
{	
	//psGCodeBuffer->StartPoint.Axis1 = psCodeG00->P*System.UnitTo_mm*1000;	//store G04DelayT...temporary use system.UintTo_mm as conversion...}	
	STD_Rec_single_code(GCodeBuffer);	
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}


void STD_Rec_G100(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G101(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G102(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G103(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G104(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G105(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	
	if((psCodeG00->SubCMD1 == 1)||(psCodeG00->SubCMD1 == 2))
	{	//CMDMain = 105
		//SubCMD1 = 1 Ê±¸üÐÂÆðÊ¼×ø±ê
		//SubCMD1 = 2 Ê±¸üÐÂ³ÉÏñÉ¨Ãè²½½øÖµ(²»Í¬¾µÍ·Í¼Æ¬´óÐ¡²»Í¬)
		GCodeBuffer->StartPoint.Axis17 = psCodeG00->Axis17;
		GCodeBuffer->StartPoint.Axis18 = psCodeG00->Axis18;
	}
}

void STD_Rec_G106(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->Axis1;
	GCodeBuffer->StartPoint.Axis2 = psCodeG00->Axis2;
	GCodeBuffer->StartPoint.Axis3 = psCodeG00->Axis3;
	GCodeBuffer->StartPoint.Axis4 = psCodeG00->Axis4;
	GCodeBuffer->StartPoint.Axis5 = psCodeG00->Axis5;
	GCodeBuffer->StartPoint.Axis6 = psCodeG00->Axis6;
	GCodeBuffer->StartPoint.Axis7 = psCodeG00->Axis7;
	GCodeBuffer->StartPoint.Axis8 = psCodeG00->Axis8;
	GCodeBuffer->StartPoint.Axis9 = psCodeG00->Axis9;
	GCodeBuffer->StartPoint.Axis10 = psCodeG00->Axis10;
	GCodeBuffer->StartPoint.Axis11 = psCodeG00->Axis11;
	GCodeBuffer->StartPoint.Axis12 = psCodeG00->Axis12;
	GCodeBuffer->StartPoint.Axis13 = psCodeG00->Axis13;
	GCodeBuffer->StartPoint.Axis13 = psCodeG00->Axis14;
	GCodeBuffer->StartPoint.Axis13 = psCodeG00->Axis15;
	GCodeBuffer->StartPoint.Axis13 = psCodeG00->Axis16;
}

void STD_Rec_G107(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G108(GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
}

void STD_Rec_G109(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	//GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}

void STD_Rec_G110(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}

void STD_Rec_G111(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}

void STD_Rec_G112(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}

void STD_Rec_G113(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)	
{
	STD_Rec_single_code(GCodeBuffer);
	
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->Axis1;
	GCodeBuffer->StartPoint.Axis2 = psCodeG00->Axis2;
	GCodeBuffer->StartPoint.Axis3 = psCodeG00->Axis3;
	GCodeBuffer->StartPoint.Axis4 = psCodeG00->Axis4;
	GCodeBuffer->StartPoint.Axis5 = psCodeG00->Axis5;
}

void STD_Rec_G114(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer)		
{	
	STD_Rec_single_code(GCodeBuffer);	
	GCodeBuffer->StartPoint.Axis1 = psCodeG00->HoldTime;
}


void STD_Rec_single_code(GCODE  *GCodeBuffer)	
{	
	GCodeBuffer->Stotal_Module.Module1 = 0;
	GCodeBuffer->Stotal_Module.Module2 = 0;
	GCodeBuffer->Stotal_Module.Module3 = 0;
	GCodeBuffer->Stotal_Module.Module4 = 0;
	GCodeBuffer->Stotal_Module.Module5 = 0;
	GCodeBuffer->Stotal_Module.Module6 = 0;
	GCodeBuffer->Stotal_Module.Module7 = 0;
	GCodeBuffer->Stotal_Module.Module8 = 0;
	GCodeBuffer->Stotal_Module.Module9 = 0;
	GCodeBuffer->Stotal_Module.Module10 = 0;
	GCodeBuffer->Stotal_Module.Module11 = 0;
	GCodeBuffer->Stotal_Module.Module12 = 0;
	
	GCodeBuffer->StartPoint = OverallSign.STDLastMoveGCode.EndPoint;
	GCodeBuffer->EndPoint = OverallSign.STDLastMoveGCode.EndPoint;	//ÓÃÓÚµ¥¶Î¹¦ÄÜ
}

void STD_Rec_All(tsCODE_G00 *psCode)
{	
	if(psCode->SendCountD <= STDNCSign.RealSendCount) 
	{	
		STDNCSign.ResendGcodeSign = 1;		//Arm send gcode again sign.
		return ;
	}	
	STDNCSign.RealSendCount = psCode->SendCountD;
	psGCodeBuffer = &GCodeBuffer[(STDNCSign.RealSendCount-1) % STDGCODE_MOD];	
	psGCodeBuffer->CMDMain = psCode->CMDMain;
	psGCodeBuffer->SubCMD1 = psCode->SubCMD1;	
	psGCodeBuffer->SubCMD2 = psCode->SubCMD2;		
	psGCodeBuffer->SendCount = psCode->SendCountD;	
	
	if(psCode->CMDMain>3)
	{
		psGCodeBuffer->Stotal = 0;
		NC_AREAControl.Vm = 0;
		psGCodeBuffer->EndPoint = OverallSign.STDLastMoveGCode.EndPoint; 	//ç”¨äºŽå•æ®µåŠŸèƒ½
	}	
	
	switch(psCode->CMDMain)
	{
		case  0: STD_Rec_G00((tsCODE_G00 *)psCode,psGCodeBuffer);break;		
		case  1: STD_Rec_G00((tsCODE_G00 *)psCode,psGCodeBuffer);break;
		case  4: STD_Rec_G04((tsCODE_G00 *)psCode,psGCodeBuffer);break; 
		case 100: 	STD_Rec_G100(psGCodeBuffer);break;		
		case 101: 	STD_Rec_G101(psGCodeBuffer);break;
		case 102: 	STD_Rec_G102(psGCodeBuffer);break;
		case 103: 	STD_Rec_G103(psGCodeBuffer);break;
		case 104: 	STD_Rec_G104(psGCodeBuffer);break;			
		case 105: 	STD_Rec_G105((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 106: 	STD_Rec_G106((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 107: 	STD_Rec_G107(psGCodeBuffer);break;	
		case 108: 	STD_Rec_G108(psGCodeBuffer);break;	
		case 109: 	STD_Rec_G109((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 110: 	STD_Rec_G110((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 111: 	STD_Rec_G111((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 112: 	STD_Rec_G112((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 113: 	STD_Rec_G113((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		case 114: 	STD_Rec_G114((tsCODE_G00 *)psCode,psGCodeBuffer);break;	
		
		case 0xff: 	STD_Rec_single_code(psGCodeBuffer);break;
		default:	break;
	}
}

void SPC_Deal_Line(GCODE *psNowCode)
{
	char ReturnValue;
	ReturnValue = 0;

	STD_Deal_DeltaPoint(psNowCode);
		
	psNowCode->StPointVector = OverallSign.LastMoveCodeEndVector;
	
	if(psNowCode->Stotal_Module.Module1<0.00001)
	{
		psNowCode->Stotal_Module.Module1 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis15 = divdp(psNowCode->DeltaPoint.Axis15, psNowCode->Stotal_Module.Module1);
		psNowCode->StPointVector.Axis16 = divdp(psNowCode->DeltaPoint.Axis16, psNowCode->Stotal_Module.Module1);
		psNowCode->StPointVector.Axis1 = divdp(psNowCode->DeltaPoint.Axis1, psNowCode->Stotal_Module.Module1);
		//psNowCode->StPointVector.Axis2 = divdp(psNowCode->DeltaPoint.Axis2, psNowCode->Stotal_Module.Module1);
	}
	
	if(psNowCode->Stotal_Module.Module2<0.00001)
	{
		psNowCode->Stotal_Module.Module2 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis17 = divdp(psNowCode->DeltaPoint.Axis17, psNowCode->Stotal_Module.Module2);
		psNowCode->StPointVector.Axis18 = divdp(psNowCode->DeltaPoint.Axis18, psNowCode->Stotal_Module.Module2);
	}

	if(psNowCode->Stotal_Module.Module3<0.00001)
	{
		psNowCode->Stotal_Module.Module3 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis3 = divdp(psNowCode->DeltaPoint.Axis3, psNowCode->Stotal_Module.Module3);
		psNowCode->StPointVector.Axis4 = divdp(psNowCode->DeltaPoint.Axis4, psNowCode->Stotal_Module.Module3);
		psNowCode->StPointVector.Axis5 = divdp(psNowCode->DeltaPoint.Axis5, psNowCode->Stotal_Module.Module3);
	}

	if(psNowCode->Stotal_Module.Module4<0.00001)
	{
		psNowCode->Stotal_Module.Module4 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis8 = divdp(psNowCode->DeltaPoint.Axis8, psNowCode->Stotal_Module.Module4);
		psNowCode->StPointVector.Axis7 = divdp(psNowCode->DeltaPoint.Axis7, psNowCode->Stotal_Module.Module4);
	}

	if(psNowCode->Stotal_Module.Module5<0.00001)
	{
		psNowCode->Stotal_Module.Module5 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis6 = divdp(psNowCode->DeltaPoint.Axis6, psNowCode->Stotal_Module.Module5);
		psNowCode->StPointVector.Axis20 = divdp(psNowCode->DeltaPoint.Axis20, psNowCode->Stotal_Module.Module5);
	}

	if(psNowCode->Stotal_Module.Module6<0.00001)
	{
		psNowCode->Stotal_Module.Module6 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis11 = divdp(psNowCode->DeltaPoint.Axis11, psNowCode->Stotal_Module.Module6);
		psNowCode->StPointVector.Axis12 = divdp(psNowCode->DeltaPoint.Axis12, psNowCode->Stotal_Module.Module6);
	}

	if(psNowCode->Stotal_Module.Module7<0.00001)
	{
		psNowCode->Stotal_Module.Module7 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis21 = divdp(psNowCode->DeltaPoint.Axis21, psNowCode->Stotal_Module.Module7);
		psNowCode->StPointVector.Axis22 = divdp(psNowCode->DeltaPoint.Axis22, psNowCode->Stotal_Module.Module7);
		psNowCode->StPointVector.Axis9 = divdp(psNowCode->DeltaPoint.Axis9, psNowCode->Stotal_Module.Module7);
	}

	if(psNowCode->Stotal_Module.Module8<0.00001)
	{
		psNowCode->Stotal_Module.Module8 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis2 = divdp(psNowCode->DeltaPoint.Axis2, psNowCode->Stotal_Module.Module8);
	}

	if(psNowCode->Stotal_Module.Module9<0.00001)
	{
		psNowCode->Stotal_Module.Module9 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis10 = divdp(psNowCode->DeltaPoint.Axis10, psNowCode->Stotal_Module.Module9);
	}

	if(psNowCode->Stotal_Module.Module10<0.00001)
	{
		psNowCode->Stotal_Module.Module10 = 0;
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis13 = divdp(psNowCode->DeltaPoint.Axis13, psNowCode->Stotal_Module.Module10);
		psNowCode->StPointVector.Axis14 = divdp(psNowCode->DeltaPoint.Axis14, psNowCode->Stotal_Module.Module10);
	}

	if(psNowCode->Stotal_Module.Module11<0.00001)
	{
		psNowCode->Stotal_Module.Module11 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis19 = divdp(psNowCode->DeltaPoint.Axis19, psNowCode->Stotal_Module.Module11);
	}

	if(psNowCode->Stotal_Module.Module12<0.00001)
	{
		psNowCode->Stotal_Module.Module12 = 0;		
		ReturnValue++;
	}
	else
	{
		psNowCode->StPointVector.Axis23 = divdp(psNowCode->DeltaPoint.Axis23, psNowCode->Stotal_Module.Module12);
	}		
	psNowCode->EndPointVector = psNowCode->StPointVector; 
	OverallSign.LastMoveCodeEndVector = psNowCode->StPointVector;
	OverallSign.STDLastMoveGCode.EndPoint = psNowCode->EndPoint;
	
	psNowCode->StPointVector.Axis24 = 0.575;
	psNowCode->StPointVector.Axis25 = 0.575;
	psNowCode->EndPointVector.Axis24 = 0.575; 
	psNowCode->EndPointVector.Axis25 = 0.575;
	OverallSign.LastMoveCodeEndVector.Axis24 = 0.575;
	OverallSign.LastMoveCodeEndVector.Axis25 = 0.575;
	OverallSign.SPCLastMoveGCode.EndPoint.Axis24 = 0.575;
	OverallSign.SPCLastMoveGCode.EndPoint.Axis25 = 0.575;		

	

	psNowCode->Stotal = psNowCode->Stotal_Module.Module1+psNowCode->Stotal_Module.Module2+psNowCode->Stotal_Module.Module3+
						psNowCode->Stotal_Module.Module4+psNowCode->Stotal_Module.Module5+psNowCode->Stotal_Module.Module6+
						psNowCode->Stotal_Module.Module7+psNowCode->Stotal_Module.Module8+psNowCode->Stotal_Module.Module9+
						psNowCode->Stotal_Module.Module10+psNowCode->Stotal_Module.Module11+psNowCode->Stotal_Module.Module12;



	if(ReturnValue == 12)
	{
		return ;		
	}

	NowPrecalMoveSign.Axis1 = (psNowCode->DeltaPoint.Axis1>0)?(1):(0);
	NowPrecalMoveSign.Axis2 = (psNowCode->DeltaPoint.Axis2>0)?(1):(0);
	NowPrecalMoveSign.Axis3 = (psNowCode->DeltaPoint.Axis3>0)?(1):(0);	
	NowPrecalMoveSign.Axis4 = (psNowCode->DeltaPoint.Axis4>0)?(1):(0);
	NowPrecalMoveSign.Axis5 = (psNowCode->DeltaPoint.Axis5>0)?(1):(0);
	NowPrecalMoveSign.Axis6 = (psNowCode->DeltaPoint.Axis6>0)?(1):(0);	
	NowPrecalMoveSign.Axis7 = (psNowCode->DeltaPoint.Axis7>0)?(1):(0);
	NowPrecalMoveSign.Axis8 = (psNowCode->DeltaPoint.Axis8>0)?(1):(0);
	NowPrecalMoveSign.Axis9 = (psNowCode->DeltaPoint.Axis9>0)?(1):(0);	
	NowPrecalMoveSign.Axis10 = (psNowCode->DeltaPoint.Axis10>0)?(1):(0);
	NowPrecalMoveSign.Axis11 = (psNowCode->DeltaPoint.Axis11>0)?(1):(0);
	NowPrecalMoveSign.Axis12 = (psNowCode->DeltaPoint.Axis12>0)?(1):(0);
	NowPrecalMoveSign.Axis13 = (psNowCode->DeltaPoint.Axis13>0)?(1):(0);	
	NowPrecalMoveSign.Axis14 = (psNowCode->DeltaPoint.Axis14>0)?(1):(0);
	NowPrecalMoveSign.Axis15 = (psNowCode->DeltaPoint.Axis15>0)?(1):(0);
	NowPrecalMoveSign.Axis16 = (psNowCode->DeltaPoint.Axis16>0)?(1):(0);	
	NowPrecalMoveSign.Axis17 = (psNowCode->DeltaPoint.Axis17>0)?(1):(0);
	NowPrecalMoveSign.Axis18 = (psNowCode->DeltaPoint.Axis18>0)?(1):(0);
	NowPrecalMoveSign.Axis19 = (psNowCode->DeltaPoint.Axis19>0)?(1):(0);	
	NowPrecalMoveSign.Axis20 = (psNowCode->DeltaPoint.Axis20>0)?(1):(0);
	NowPrecalMoveSign.Axis21 = (psNowCode->DeltaPoint.Axis21>0)?(1):(0);
	NowPrecalMoveSign.Axis22 = (psNowCode->DeltaPoint.Axis22>0)?(1):(0);
	NowPrecalMoveSign.Axis23 = (psNowCode->DeltaPoint.Axis23>0)?(1):(0);	
	NowPrecalMoveSign.Axis24 = (psNowCode->DeltaPoint.Axis24>0)?(1):(0);
	NowPrecalMoveSign.Axis25 = (psNowCode->DeltaPoint.Axis25>0)?(1):(0);

	LastPrecalMoveSign = NowPrecalMoveSign;
}


/***************************************************************************/
/*  Function name: SPC_JOG_Code()                                   */
/*  Return value:		0:not complete ;1:Complete  */
/*  Function: DSP add  code if from STD to SPC  when stay single mode                                       */
/***************************************************************************/
Uint16 SPC_JOG_Code(void)
{
	Uint16 i,j,k,ParaLength;
	Uint32 si,temp_count;			//¶ÔSPCNCSign.RealSendCountÇó20Ä£µÄÓàÊý=»º³åÇøÖ¸Õë
	GCODE  *GCodeSIPointer;
	Uint32	a,b,c;
	Uint32	testcnt = 3;

	OverallSign.LastCodeOverSign = 0;		//CH0512
	OverallSign.NCSign = SPCCODERUN;
	SPCNCSign.RealSendCount++;
	
	si = (SPCNCSign.RealSendCount-1) % SPCGCODE_MOD;
	GCodeSIPointer = &SPCGCodeBuffer[si];

	GCodeSIPointer->SendCount = SPCNCSign.RealSendCount; 
	
	GCodeSIPointer->CMDMain = 1;
	GCodeSIPointer->SubCMD1 = 0;
	GCodeSIPointer->SubCMD2 = 0;
	GCodeSIPointer->StartPoint = OverallSign.SPCLastMoveGCode.EndPoint;
	GCodeSIPointer->EndPoint = GCodeSIPointer->StartPoint;

	if(OverallSign.JOGAxis == 1)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis1+= OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis1 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis1 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 2)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis2 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis2 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis2 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 3)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis3 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis3 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis3 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 4)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis4 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis4 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis4 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 5)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis5 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis5 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis5 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 6)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis6 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis6 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis6 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 7)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis7 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis7 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis7 += 0;
		}
	}	
	else if(OverallSign.JOGAxis == 8)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis8 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis8 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis8 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 9)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis9 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis9 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis9 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 10)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis10 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis10 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis10 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 11)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis11+= OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis11 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis11 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 12)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis12 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis12 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis12 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 13)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis13 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis13 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis13 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 14)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis14 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis14 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis14 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 15)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis15 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis15 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis15 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 16)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis16 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis16 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis16 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 17)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis17 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis17 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis17 += 0;
		}
	}	
	else if(OverallSign.JOGAxis == 18)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis18 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis18 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis18 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 19)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis19 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis19 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis19 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 20)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis20 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis20 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis20 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 21)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis21+= OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis21 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis21 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 22)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis22 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis22 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis22 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 23)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis23 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis23 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis23 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 24)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis24 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis24 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis24 += 0;
		}
	}
	else if(OverallSign.JOGAxis == 25)
	{
		if(OverallSign.JOGDir == 1)
		{
			GCodeSIPointer->EndPoint.Axis25 += OverallSign.JOGDistance;
		}
		else if(OverallSign.JOGDir == 2)
		{
			GCodeSIPointer->EndPoint.Axis25 -= OverallSign.JOGDistance;
		}
		else
		{
			GCodeSIPointer->EndPoint.Axis25 += 0;
		}
	}
	
	GCodeSIPointer->Feedrate = SPCNCSign.F;  //mm/s  or degree/s
	GCodeSIPointer->Feedrate_Module.Module1 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module2 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module3 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module4 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module5 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module6 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module7 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module8 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module9 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module10 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module11 = SPCNCSign.F;
	GCodeSIPointer->Feedrate_Module.Module12 = SPCNCSign.F;

	SPC_Deal_Line(GCodeSIPointer);
	
	OverallSign.SPCLastMoveGCode.EndPoint = GCodeSIPointer->EndPoint;

	GCodeSIPointer->VeCalSign = 0;
	GCodeSIPointer->MoveCodeSign = 0;
	GCodeSIPointer->MoveCodeSign = 1;
	
	SPCNCSign.LastRealSendCount = SPCNCSign.RealSendCount;
	
	NCAreaGCodeBuffer[ (NCRunSign.RealSendCount++)%STDGCODE_MOD ] = *GCodeSIPointer;

	return 1;
}

void SPC_JudgeSoftLimit(GCODE *psNowCode)
{}



//ï¿½ï¿½ï¿½ï¿½å²¹ï¿½ã·¨added by Qing  20100311
Uint16 FourAxisInterpolatAlgorithm(GCODE *FourAxisGcodePointer,float64 DeltaPointX,float64 DeltaPointY,float64 DeltaPointZ,float64 DeltaPointA)
{
	#if 0
	if(FourAxisGcodePointer->SubCMD1 == 1)
	{		
		FourAxisGcodePointer->Stotal = fabs(DeltaPointA); //only ï¿½ï¿½ï¿½ï¿½ï¿½:A ï¿½áµ¥ï¿½ï¿½ï¿½ï¿½
	}
	else
	{
		FourAxisGcodePointer->Stotal = DeltaPointX*DeltaPointX+DeltaPointY*DeltaPointY+DeltaPointZ*DeltaPointZ; 

		if(FourAxisGcodePointer->Stotal > 0)
		{
			FourAxisGcodePointer->Stotal = sqrtdp(FourAxisGcodePointer->Stotal);
		}
	}
	#endif

	FourAxisGcodePointer->Stotal = DeltaPointX*DeltaPointX+DeltaPointY*DeltaPointY+DeltaPointZ*DeltaPointZ; 

	if(FourAxisGcodePointer->Stotal > 0)
	{
		FourAxisGcodePointer->Stotal = sqrtdp(FourAxisGcodePointer->Stotal);
	}
	
	return 1;

}



