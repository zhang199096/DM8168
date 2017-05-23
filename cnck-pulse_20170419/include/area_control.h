#ifndef DSP6713_AREA_CONTROL_H
#define DSP6713_AREA_CONTROL_H

//区域计算区域

#define NCAREA_STABLE_STATE			0		//区域稳速状态
#define NCAREA_ACCELERATING_STATE	1		//区域上升状态
#define NCAREA_DECELERATING_STATE	2		//区域下降状态
#define NCAREA_SINGLE_STATE			3		//区域单条代码状态
#define NCAREA_SPECIAL_STATE		4		//区域特殊状态

extern AREA_CONTROL	NC_AREAControl;
extern AREA_CONTROL NC_AREAControl_Error;

extern INTERPOLATION_BUFFER InterpolationBuffer_1[INTERPOLATIONBUFLENGTH+1];
extern INTERPOLATION_BUFFER InterpolationBuffer_2[INTERPOLATIONBUFLENGTH+1];
extern INTERPOLATION_BUFFER NewInterpolationData;

extern Uint16  NCAreaMonitorBuffer(float64 Buffer1, float64 Buffer2, float64 Buffer3,float64 Buffer4);
extern Uint16 NcAreaCameraShutter(AREA_CONTROL *AREAControl,NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NcAreaOpticalFlash(AREA_CONTROL *AREAControl,NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NcAreaLensPosition(AREA_CONTROL *AREAControl,NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NcAreaRunStateCheck(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NCAreaPrePulseToServo(AREA_CONTROL *AREAControl);
extern Uint16 NCAreaPulseToServo_LPMEN1(void);
extern Uint16 NCAreaPulseToServo_LPMEN2(void);
extern Uint16 NCAreaFeedrateOverride(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign);
extern float64 NCAreaVm(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign, Uint32 i,Uint8 ModuleSel);
extern float64 NCArea_Cal_i1_i2_Vveer(float64 a_de, GCODE * AreaGcodeBufferPointer , Uint32 i1,Uint32 i2,Uint8 ModuleSel);
extern Uint16 NCAreaHyperbolaPrecalStandardCurve(AREA_CONTROL *AREAControl, Uint8 ModuleSel);//预计算标准曲线参数
extern Uint16 NCAreaGetEndCodeCount(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NCAreaCalSs(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern Uint16 NCAreaCalVeVm(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern Uint16 NCAreaFirstInflexion(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern Uint16 NCAreaSecondInflexion(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern Uint16 NCAreaRunPreCal(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern Uint16 NCAreaControl(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NCAreaRunPre(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer , NC_RUN_SIGN *AreaRunNCSign);
extern Uint16 NCAreaHyperbolaRun(AREA_CONTROL *AREAControl,Uint8 ModuleSel);
extern Uint16 NCAreaCalNowValue(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel);
extern void NCAreaEnsureDirection(AREA_CONTROL *AREAControl, NC_RUN_SIGN *AreaRunNCSign);
extern void NCArea_check_point_direction(AREA_CONTROL *AREAControl);
extern void NCArea_check_endpoint_value(AREA_CONTROL *AREAControl);	//检查插补点有无超出终点范围
extern void NCAreaJudgeLimit(AREA_CONTROL *AREAControl);//判断有无到极限位，并做相应处理
extern void NCAreaCal_V_tpr(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module1(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module2(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module3(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module4(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module5(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module6(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module7(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module8(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module9(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module10(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module11(AREA_CONTROL *AREAControl);
extern void NCAreaCal_V_tpr_Module12(AREA_CONTROL *AREAControl);
extern Uint16 NCAreaWriteInterpolationBuffer(AREA_CONTROL *AREAControl);
extern Uint16  NCAreaRun(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign);

#endif  // end of DSP6713_AREA_CONTROL_H definition


