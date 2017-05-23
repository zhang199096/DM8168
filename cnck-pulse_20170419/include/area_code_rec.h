#ifndef _AREA_CODE_REC_H
#define _AREA_CODE_REC_H

extern void STD_Deal_DeltaPoint(GCODE  *psNowCode);
extern void STD_Deal_ArcStotal(GCODE  *psNowCode);
extern void STD_Deal_Vveer(GCODE *NowCode);
extern void STD_Rec_G00(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G04(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G100(GCODE  *GCodeBuffer);
extern void STD_Rec_G101(GCODE  *GCodeBuffer);
extern void STD_Rec_G102(GCODE  *GCodeBuffer);
extern void STD_Rec_G103(GCODE  *GCodeBuffer);
extern void STD_Rec_G104(GCODE  *GCodeBuffer);
extern void STD_Rec_G105(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G106(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G107(GCODE  *GCodeBuffer);
extern void STD_Rec_G108(GCODE  *GCodeBuffer);
extern void STD_Rec_G109(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G110(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G111(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G112(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G113(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);
extern void STD_Rec_G114(tsCODE_G00 *psCodeG00,GCODE  *GCodeBuffer);

extern void STD_Rec_single_code(GCODE  *GCodeBuffer);
extern void STD_Rec_All(tsCODE_G00 * psCode);
extern void SPC_Deal_Line(GCODE * psNowCode);
extern Uint16 SPC_JOG_Code(void);
extern void SPC_JudgeSoftLimit(GCODE *psNowCode);
extern Uint16 FourAxisInterpolatAlgorithm(GCODE *FourAxisGcodePointer,float64 DeltaPointX,float64 DeltaPointY,float64 DeltaPointZ,float64 DeltaPointA);

#endif 
