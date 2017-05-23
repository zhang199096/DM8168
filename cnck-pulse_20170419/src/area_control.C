// enjoy lu
//此程序为区域运动控制主程序

#include "..\\include\\main_dsp.h"

AREA_CONTROL	NC_AREAControl;
AREA_CONTROL    NC_AREAControl_Error;
INTERPOLATION_BUFFER InterpolationBuffer_1[INTERPOLATIONBUFLENGTH+1];
INTERPOLATION_BUFFER InterpolationBuffer_2[INTERPOLATIONBUFLENGTH+1];

//主循环程序与中断交互缓冲区
INTERPOLATION_BUFFER NewInterpolationData;   

// 区域曲线写插补缓冲区监控
Int16 		MonitorInterPlationBuf[1000];
Uint16 		MonitorInterPlationBufi=0, MonitorBuffer1Cnt=0, MonitorBuffer2Cnt=0, MonitorBuffer3Cnt=0,MonitorBuffer4Cnt=0;
Int32 		PositionPointerCnt=0,PositionPointerCnt_Bak=0;
float64 		CameraScanStepperTotalNumber_X=100.0;
Int32		OpticalFlashPointerCnt=0,OpticalFlashPointerCnt_Bak=0;
Int32		DeltaStepAxis19_Bak;
Int32		DeltaStepAxis19_CPS;

float64 		CameraShutterStepperTotalNumber_X=100.0;
Int32		CameraShutterPointerCnt=0,CameraShutterPointerCnt_Bak=0;

INT32S MonitorBuffer1[1024];					//NcAreaOpticalFlash
INT32S MonitorBuffer2[1024];					//NcAreaCameraShutter,get_ABS_coordinate_CameraScan
INT32S MonitorBuffer3[1024];					//OverallSign.ABS_Coordinate.Axis19 (get_ABS_coordinate_LensPosition )
INT32S MonitorBuffer4[1024];					//get_PMT_counter
INT32S GetDataBuffer[1024];					//

/***************************************************************************/
/*  Function name: NCAreaMonitorBuffer()                                   */
/*  Argument:AREA_CONTROL ,GCODE ,NC_RUN_SIGN   	                                    */
/*  Return value: */
/*  Function: NCAreaMonitorBuffer                                 */
/***************************************************************************/
Uint16  NCAreaMonitorBuffer(float64 Buffer1, float64 Buffer2, float64 Buffer3,float64 Buffer4)
{

	MonitorBuffer1[MonitorBuffer1Cnt++] = Buffer1;
	if(MonitorBuffer1Cnt > 1000)
	{
		MonitorBuffer1Cnt = 0;
	}

	MonitorBuffer2[MonitorBuffer2Cnt++] = Buffer2;
	if(MonitorBuffer2Cnt > 1000)
	{
		MonitorBuffer2Cnt = 0;
	}

	MonitorBuffer3[MonitorBuffer3Cnt++] = Buffer3;
	if(MonitorBuffer3Cnt > 1000)
	{
		MonitorBuffer3Cnt = 0;
	}

	MonitorBuffer4[MonitorBuffer4Cnt++] = Buffer4;

	if(MonitorBuffer4Cnt > 1000)
	{
		MonitorBuffer4Cnt = 0;
	}	
	
	return TRUE;
}

Uint16 NcAreaCameraShutter(AREA_CONTROL *AREAControl,NC_RUN_SIGN *AreaRunNCSign)
{
	float64AXIS	 CameraShutterDivideStepper;	
	Int32		 CameraShutterModOffset;
	float64	 	CameraShutterOffsetRatio;
	INT40AXIS 	 CameraShutterAbsCoordinate;
	Int32		 CameraShutterDivideStepperX_Int32, CameraShutterDivideStepperY_Int32;
	static float64 	CaptureShutterTestTime2 = 7;//10ms start open camera shutter
	static float64 	CaptureShutterTestTime3 = 9;//12ms start open camera shutter rising edge trigger

	AREAControl->DeltaStep.Axis14 = 0;

	if(OverallSign.CameraScanSign == 0)
	{
		return;
	}
	if(System.FunctionSelect06 == 0)
	{
		return;
	}
	if(OverallSign.NCSign ==STDCODERUN)
	{		
		 CameraShutterAbsCoordinate.Axis17 = AREAControl->NowValue.Axis17 - System.PositionCoordinate1.Axis17;
		 CameraShutterAbsCoordinate.Axis18 = AREAControl->NowValue.Axis18 - System.PositionCoordinate1.Axis18;
		
		 CameraShutterDivideStepper.Axis17 = divdp( CameraShutterAbsCoordinate.Axis17,System.OffsetCoordinate.Axis17);
		 CameraShutterDivideStepper.Axis18 = divdp( CameraShutterAbsCoordinate.Axis18,System.OffsetCoordinate.Axis18);

		#if 0
		if(( CameraShutterDivideStepper.Axis17 < 0.2)||( CameraShutterDivideStepper.Axis17 >CameraScanStepperTotalNumber_X))
		{
			AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
			return;
		}
		#endif
		if(( CameraShutterAbsCoordinate.Axis17 < (System.OffsetCoordinate.Axis17 - CaptureShutterTestTime3*AREAControl->VmReal_Module.Module2))||
			( CameraShutterAbsCoordinate.Axis17 >AREAControl->Stotal_Module.Module2*1000 - System.OffsetCoordinate.Axis17+CaptureShutterTestTime3*AREAControl->VmReal_Module.Module2))
		{
			AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
			return;
		}

		CameraShutterOffsetRatio = divdp(CaptureShutterTestTime3*AREAControl->VmReal_Module.Module2,System.OffsetCoordinate.Axis17);
		
		 CameraShutterStepperTotalNumber_X = divdp(AREAControl->Stotal_Module.Module2*1000,System.OffsetCoordinate.Axis17) - 0.4;

		if( CameraShutterStepperTotalNumber_X < 2.0)
		{
			AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
			return 0;
		}
		
		 //CameraShutterDivideStepperX_Int32 = (Int32)( CameraShutterDivideStepper.Axis17+CameraShutterOffsetRatio);
		 CameraShutterDivideStepperY_Int32 = (Int32)( CameraShutterDivideStepper.Axis18+0.2);
		
		 CameraShutterModOffset =  CameraShutterAbsCoordinate.Axis17%System.OffsetCoordinate.Axis17;

		if((OverallSign.MoveSign.Axis17 == 1)&&(AREAControl->DeltaStep.Axis18 == 0))
		{
			
			CameraShutterDivideStepperX_Int32 = (Int32)( CameraShutterDivideStepper.Axis17+CameraShutterOffsetRatio);
			 CameraShutterPointerCnt =  CameraShutterDivideStepperX_Int32+((Int32)( CameraShutterStepperTotalNumber_X))* CameraShutterDivideStepperY_Int32;

			if(( CameraShutterPointerCnt -  CameraShutterPointerCnt_Bak) > 1)
			{
				Error.CameraShutterTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if( CameraShutterPointerCnt < 0)
			{
				Error.CameraShutterTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if( CameraShutterPointerCnt>0)
			{
				if( CameraShutterPointerCnt_Bak ==  CameraShutterPointerCnt)
				{
					AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
					return 0;
				}
				else
				{
					if((CameraShutterModOffset > (System.OffsetCoordinate.Axis17 - CaptureShutterTestTime3*AREAControl->VmReal_Module.Module2))&&
						(CameraShutterModOffset < (System.OffsetCoordinate.Axis17 - CaptureShutterTestTime2*AREAControl->VmReal_Module.Module2)))
					{
						AREAControl->DeltaStep.Axis14 = 1;	
						MonitorBuffer2[MonitorBuffer2Cnt] = CameraShutterAbsCoordinate.Axis17;
						
						MonitorBuffer2Cnt++;
						if(MonitorBuffer2Cnt > 1000)
						{
							MonitorBuffer2Cnt = 0;
						}
					}
					else
					{
						AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
						return 0;
					}

				}
			}			
		}
		else if((OverallSign.MoveSign.Axis17 == 0)&&(AREAControl->DeltaStep.Axis18 == 0))
		{		
			CameraShutterDivideStepperX_Int32 = (Int32)( CameraShutterDivideStepper.Axis17+1-CameraShutterOffsetRatio);
			 CameraShutterPointerCnt = (((Int32)( CameraShutterStepperTotalNumber_X+0.6))- CameraShutterDivideStepperX_Int32)+((Int32)( CameraShutterStepperTotalNumber_X))* CameraShutterDivideStepperY_Int32;

			if(( CameraShutterPointerCnt -  CameraShutterPointerCnt_Bak) > 1)
			{
				Error.CameraShutterTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if( CameraShutterPointerCnt < 0)
			{
				Error.CameraShutterTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if( CameraShutterPointerCnt>0)
			{
				if( CameraShutterPointerCnt_Bak ==  CameraShutterPointerCnt)
				{
					AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
					return 0;
				}
				else				
				{
				
					if((CameraShutterModOffset < (CaptureShutterTestTime3*AREAControl->VmReal_Module.Module2))&&
					(CameraShutterModOffset > (CaptureShutterTestTime2*AREAControl->VmReal_Module.Module2)))
					{
						AREAControl->DeltaStep.Axis14 = 1;		

						MonitorBuffer2[MonitorBuffer2Cnt] = CameraShutterAbsCoordinate.Axis17;
						
						MonitorBuffer2Cnt++;
						if(MonitorBuffer2Cnt > 1000)
						{
							MonitorBuffer2Cnt = 0;
						}
					}
					else
					{
						AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14;
						return 0;
					}
				}
			}
		}

		 CameraShutterPointerCnt_Bak =  CameraShutterPointerCnt;
	}		
}


Uint16 NcAreaOpticalFlash(AREA_CONTROL *AREAControl,NC_RUN_SIGN *AreaRunNCSign)
{
	float64AXIS	OpticalFlashDivideStepper;	
	Int32		OpticalFlashModOffset;
	static float64	OpticalFlashOffsetRatio = 0.4;
	INT40AXIS 	CameraAbsCoordinate;
	Int32		OpticalFlashDivideStepperX_Int32,OpticalFlashDivideStepperY_Int32;
	Int32		OpticalFlashValue_Delta = 5;
	Int32		OpticalFlashValue_Delta_2 = 5;

	AREAControl->DeltaStep.Axis23 = 0;

	if(OverallSign.CameraScanSign == 0)
	{
		return;
	}
	if(System.FunctionSelect01 == 0)
	{
		return;
	}
	if(OverallSign.NCSign ==STDCODERUN)
	{		
		CameraAbsCoordinate.Axis17 = AREAControl->NowValue.Axis17 - System.PositionCoordinate1.Axis17;
		CameraAbsCoordinate.Axis18 = AREAControl->NowValue.Axis18 - System.PositionCoordinate1.Axis18;
		
		OpticalFlashDivideStepper.Axis17 = divdp(CameraAbsCoordinate.Axis17,System.OffsetCoordinate.Axis17);
		OpticalFlashDivideStepper.Axis18 = divdp(CameraAbsCoordinate.Axis18,System.OffsetCoordinate.Axis18);

		if((OpticalFlashDivideStepper.Axis17 < 0.4)||(OpticalFlashDivideStepper.Axis17 >CameraScanStepperTotalNumber_X))
		{
			AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
			return;
		}

		CameraScanStepperTotalNumber_X = divdp(AREAControl->Stotal_Module.Module2*1000,System.OffsetCoordinate.Axis17)-OpticalFlashOffsetRatio;

		if(CameraScanStepperTotalNumber_X < 2.0)
		{
			AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
			return 0;
		}
		
		OpticalFlashDivideStepperX_Int32 = (Int32)(OpticalFlashDivideStepper.Axis17+0.2);
		OpticalFlashDivideStepperY_Int32 = (Int32)(OpticalFlashDivideStepper.Axis18+0.2);
		
		OpticalFlashModOffset = CameraAbsCoordinate.Axis17%System.OffsetCoordinate.Axis17;

		if((OverallSign.MoveSign.Axis17 == 1)&&(AREAControl->DeltaStep.Axis18 == 0))
		{
			OpticalFlashPointerCnt = OpticalFlashDivideStepperX_Int32+((Int32)(CameraScanStepperTotalNumber_X))*OpticalFlashDivideStepperY_Int32;

			if((OpticalFlashPointerCnt - OpticalFlashPointerCnt_Bak) > 1)
			{
				Error.OpticalFlashTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if(OpticalFlashPointerCnt < 0)
			{
				Error.OpticalFlashTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if(OpticalFlashPointerCnt>0)
			{
				if(OpticalFlashPointerCnt_Bak == OpticalFlashPointerCnt)
				{
					AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
					return 0;
				}
				else
				{
					if(System.FunctionSelect02 == 1)
					{
						if(	//front
							(OpticalFlashModOffset < AREAControl->VmReal_Module.Module2)
							//&&(OpticalFlashModOffset >=OpticalFlashValue_Delta)
							||(OpticalFlashModOffset >= (System.OffsetCoordinate.Axis17 - OpticalFlashValue_Delta))
							)
						{
							AREAControl->DeltaStep.Axis23 = 1;	
							//MonitorBuffer1[MonitorBuffer1Cnt] = OpticalFlashModOffset;
							MonitorBuffer1[MonitorBuffer1Cnt] = CameraAbsCoordinate.Axis17;
						
							MonitorBuffer1Cnt++;
							if(MonitorBuffer1Cnt > 1000)
							{
								MonitorBuffer1Cnt = 0;
							}
						}
						else
						{
							AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
							return 0;
						}
					}
					else
					{
						if(	//behind
							(OpticalFlashModOffset < AREAControl->VmReal_Module.Module2)
							&&(OpticalFlashModOffset >=OpticalFlashValue_Delta)
							//||(OpticalFlashModOffset >= (System.OffsetCoordinate.Axis17 - OpticalFlashValue_Delta))
							)
						{
							AREAControl->DeltaStep.Axis23 = 1;		
							MonitorBuffer1[MonitorBuffer1Cnt] = OpticalFlashModOffset;
						
							MonitorBuffer1Cnt++;
							if(MonitorBuffer1Cnt > 1000)
							{
								MonitorBuffer1Cnt = 0;
							}
						}
						else
						{
							AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
							return 0;
						}
					}
				}
			}			
		}
		else if((OverallSign.MoveSign.Axis17 == 0)&&(AREAControl->DeltaStep.Axis18 == 0))
		{
			OpticalFlashPointerCnt = (((Int32)(CameraScanStepperTotalNumber_X+0.6))-OpticalFlashDivideStepperX_Int32)+((Int32)(CameraScanStepperTotalNumber_X))*OpticalFlashDivideStepperY_Int32;

			if((OpticalFlashPointerCnt - OpticalFlashPointerCnt_Bak) > 1)
			{
				Error.OpticalFlashTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if(OpticalFlashPointerCnt < 0)
			{
				Error.OpticalFlashTriggerBreakError = 1;
				high_error();		//CH0904
				return 0;
			}

			if(OpticalFlashPointerCnt>0)
			{
				if(OpticalFlashPointerCnt_Bak == OpticalFlashPointerCnt)
				{
					AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
					return 0;
				}
				else				
				{
					if(System.FunctionSelect02 == 1)
					{
						if( 
							(OpticalFlashModOffset <= OpticalFlashValue_Delta_2)
							||
							(OpticalFlashModOffset > (System.OffsetCoordinate.Axis17 - AREAControl->VmReal_Module.Module2))
							)
						{
							AREAControl->DeltaStep.Axis23 = 1;		

							//MonitorBuffer1[MonitorBuffer1Cnt] = OpticalFlashModOffset;
							MonitorBuffer1[MonitorBuffer1Cnt] = CameraAbsCoordinate.Axis17;
							
							MonitorBuffer1Cnt++;
							if(MonitorBuffer1Cnt > 1000)
							{
								MonitorBuffer1Cnt = 0;
							}
						}
						else
						{
							AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
							return 0;
						}
					}
					else
					{
						if( 
							(OpticalFlashModOffset <= (System.OffsetCoordinate.Axis17 - OpticalFlashValue_Delta_2))
							&&
							(OpticalFlashModOffset > (System.OffsetCoordinate.Axis17 - AREAControl->VmReal_Module.Module2))
							)
						{
							AREAControl->DeltaStep.Axis23 = 1;	

							MonitorBuffer1[MonitorBuffer1Cnt] = OpticalFlashModOffset;
						
							MonitorBuffer1Cnt++;
							if(MonitorBuffer1Cnt > 1000)
							{
								MonitorBuffer1Cnt = 0;
							}
						}
						else
						{
							AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23;
							return 0;
						}
					}

				}
			}
		}

		OpticalFlashPointerCnt_Bak = OpticalFlashPointerCnt;
	}		
}


Uint16 NcAreaRunStateCheck(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign)
{
	Uint32 ri;

	ri = (AreaRunNCSign->RealRunCount-1) % STDGCODE_MOD;

	if(0)
	{
		return 0;
	}
	
	return 1;
}


//判断有无变档
Uint16 NCAreaFeedrateOverride(
									AREA_CONTROL *AREAControl, 
									GCODE*AreaGcodeBufferPointer, 
									NC_RUN_SIGN *AreaRunNCSign
									)
{
	//watch_spindle_stop();
	
	AREAControl->VChangeSign = 0;
	if( (OverallSign.LastFeedrateOverride != OverallSign.FeedrateOverride) ||
		(OverallSign.LastRapidFeedrateOverride != OverallSign.RapidFeedrateOverride) )
	{
		OverallSign.LastFeedrateOverride = OverallSign.FeedrateOverride;
		OverallSign.LastRapidFeedrateOverride = OverallSign.RapidFeedrateOverride;
		AREAControl->VChangeSign = 1;
	}
	return 1;
}
//计算Ve,Vm
Uint16 NCAreaCalVeVm(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel)
{
	float64	Ve_a_de,CurrentVm;
	GCODE	*CurrentCodePT,*AfterCodePT;
	float64 	Ve; 				//当前处理线段终点允许速度
	float64 	Vback;				//反推速度限制
	float64 	VeAfter;			//当前处理线段与后续线段相关终点允许速度
	float64	Vestart;			//当前处理线段起始允许速度
	float64 	Ssi;				//计算i 线段向后搜索距离
	float64 	S1; 				//计算i 线段向后搜索累积距离
	float64	Ve_VMin;

	CurrentCodePT = AreaGcodeBufferPointer + (AreaRunNCSign->RealRunCount-1) % STDGCODE_MOD;
	
	if(ModuleSel == 1)
	{
		if(CurrentCodePT->Stotal_Module.Module1 == 0)
		{
			CurrentCodePT->Ve_Module.Module1 = CurrentCodePT->Vm_Module.Module1;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module1;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module1 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module1;
			
			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module1;	
			if( Ve > AfterCodePT->Vm_Module.Module1 )
				Ve = AfterCodePT->Vm_Module.Module1;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module1,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,1 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module1;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module1 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 2)
	{
		if(CurrentCodePT->Stotal_Module.Module2 == 0)
		{
			CurrentCodePT->Ve_Module.Module2 = CurrentCodePT->Vm_Module.Module2;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module2;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module2 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{		
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module2;
			
			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module2;	
			if( Ve > AfterCodePT->Vm_Module.Module2 )
				Ve = AfterCodePT->Vm_Module.Module2;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module2,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,2 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module2;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module2 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}	
	else if(ModuleSel == 3)
	{
		if(CurrentCodePT->Stotal_Module.Module3 == 0)
		{
			CurrentCodePT->Ve_Module.Module3 = CurrentCodePT->Vm_Module.Module3;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module3;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module3 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{			
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module3;
			
			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module3;	
			if( Ve > AfterCodePT->Vm_Module.Module3 )
				Ve = AfterCodePT->Vm_Module.Module3;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module3,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,3 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module3;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module3 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}	
	else if(ModuleSel == 4)
	{
		if(CurrentCodePT->Stotal_Module.Module4 == 0)
		{
			CurrentCodePT->Ve_Module.Module4 = CurrentCodePT->Vm_Module.Module4;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module4;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module4 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module4;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module4;	
			if( Ve > AfterCodePT->Vm_Module.Module4 )
				Ve = AfterCodePT->Vm_Module.Module4;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module4,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,4 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module4;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module4 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}	
	else if(ModuleSel == 5)
	{
		if(CurrentCodePT->Stotal_Module.Module5 == 0)
		{
			CurrentCodePT->Ve_Module.Module5 = CurrentCodePT->Vm_Module.Module5;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module5;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module5 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module5;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module5;	
			if( Ve > AfterCodePT->Vm_Module.Module5 )
				Ve = AfterCodePT->Vm_Module.Module5;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module5,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,5 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module5;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module5 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}	
	else if(ModuleSel == 6)
	{
		if(CurrentCodePT->Stotal_Module.Module6 == 0)
		{
			CurrentCodePT->Ve_Module.Module6 = CurrentCodePT->Vm_Module.Module6;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module6;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module6 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module6;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module6;	
			if( Ve > AfterCodePT->Vm_Module.Module6 )
				Ve = AfterCodePT->Vm_Module.Module6;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module6,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,6 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module6;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module6 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 7)
	{
		if(CurrentCodePT->Stotal_Module.Module7 == 0)
		{
			CurrentCodePT->Ve_Module.Module7 = CurrentCodePT->Vm_Module.Module7;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module7;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module7 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module7;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module7;	
			if( Ve > AfterCodePT->Vm_Module.Module7 )
				Ve = AfterCodePT->Vm_Module.Module7;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module7,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module7;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module7 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 8)
	{
		if(CurrentCodePT->Stotal_Module.Module8 == 0)
		{
			CurrentCodePT->Ve_Module.Module8 = CurrentCodePT->Vm_Module.Module8;
			return;
		}

		Ve_a_de = System.a_A4_SET_Module.Module8;	
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module8 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module8;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module8;	
			if( Ve > AfterCodePT->Vm_Module.Module8 )
				Ve = AfterCodePT->Vm_Module.Module8;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module8,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module8;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module8 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 9)
	{
		if(CurrentCodePT->Stotal_Module.Module9 == 0)
		{
			CurrentCodePT->Ve_Module.Module9 = CurrentCodePT->Vm_Module.Module9;
			return;
		}

		Ve_a_de = System.a_A4_SET_Module.Module9;	
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module9 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module9;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module9;	
			if( Ve > AfterCodePT->Vm_Module.Module9 )
				Ve = AfterCodePT->Vm_Module.Module9;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module9,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module9;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module9 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 10)
	{
		if(CurrentCodePT->Stotal_Module.Module10 == 0)
		{
			CurrentCodePT->Ve_Module.Module10 = CurrentCodePT->Vm_Module.Module10;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module10;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module10 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module10;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module10;	
			if( Ve > AfterCodePT->Vm_Module.Module10 )
				Ve = AfterCodePT->Vm_Module.Module10;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module10,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module10;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module10 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 11)
	{
		if(CurrentCodePT->Stotal_Module.Module11 == 0)
		{
			CurrentCodePT->Ve_Module.Module11 = CurrentCodePT->Vm_Module.Module11;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module11;
		
		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module11 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module11;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module11;	
			if( Ve > AfterCodePT->Vm_Module.Module11 )
				Ve = AfterCodePT->Vm_Module.Module11;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module11,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module11;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module11 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}
	else if(ModuleSel == 12)
	{
		if(CurrentCodePT->Stotal_Module.Module12 == 0)
		{
			CurrentCodePT->Ve_Module.Module12 = CurrentCodePT->Vm_Module.Module12;
			return;
		}

		Ve_a_de = System.a_SET_Module.Module12;

		NCAreaGetEndCodeCount( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign );
		(AreaGcodeBufferPointer+(AreaRunNCSign->EndCodeCount-1)%STDGCODE_MOD)->Ve_Module.Module12 = 0;

		Ve_VMin = System.VeerDeltaV;		
		
		if( AreaRunNCSign->RealRunCount <= AreaRunNCSign->EndCodeCount )
		{
			CurrentCodePT->Feedrate = CurrentCodePT->Feedrate_Module.Module12;

			CurrentVm = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);

			if(CurrentVm <= Ve_VMin)
				Ve_VMin = CurrentVm;
			
			if(AreaRunNCSign->RealRunCount == AreaRunNCSign->EndCodeCount )
			{
				CurrentCodePT->Ve = 0;
				return;
			}
			
			AfterCodePT = AreaGcodeBufferPointer + AreaRunNCSign->RealRunCount % STDGCODE_MOD;

			Ve = CurrentCodePT->Vm_Module.Module12;	
			if( Ve > AfterCodePT->Vm_Module.Module12 )
				Ve = AfterCodePT->Vm_Module.Module12;
					
			Ssi = divdp( powdp(CurrentCodePT->Vm_Module.Module12,2), Ve_a_de );
			S1 = 0;	
			VeAfter = NCArea_Cal_i1_i2_Vveer( Ve_a_de, AreaGcodeBufferPointer, AreaRunNCSign->RealRunCount, (AreaRunNCSign->RealRunCount+1) ,7 );	//当前代码起点(上条代码终点)允许的速度
			Vestart = sqrtdp(powdp(VeAfter,2)+S1*Ve_a_de);

			if(Vestart < Ve)
			{
				Ve = Vestart;
			}
			
			S1 += CurrentCodePT->Stotal_Module.Module12;	 //S1为RealRunCount 线段终点和RealRunCount+1 线段终点间的距离

			if(S1 > Ssi)   
			{			
				CurrentCodePT->VeCalSign = 1;
				if( Ve <= 3)
					Ve = Ve;
			}
			else		
			{	 
				Vback = sqrtdp(S1*Ve_a_de);       //由不够减速的距离反推,从而获得当前的限制速度
				CurrentCodePT->VeCalSign = 2;					
				if( Ve > Vback )
					Ve = Vback;
			}
			if( Ve <= Ve_VMin)
			{
				Ve = Ve_VMin;
				CurrentCodePT->VeCalSign = 3;
			}

			CurrentCodePT->Ve_Module.Module12 = Ve;   //此为每一代码段的终点速度				
		}

		return 1;
	}

}

//得到终点代码条数
Uint16 NCAreaGetEndCodeCount(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign)
{	

	AreaRunNCSign->EndCodeCount = AreaRunNCSign->RealSendCount;

	return AreaRunNCSign->EndCodeCount;
	
}



//STD区域  根据档位计算理论进给速度
//入口: 分解代码位置指针
float64 NCAreaVm(
					AREA_CONTROL *AREAControl,
					GCODE *AreaGcodeBufferPointer, 
					NC_RUN_SIGN *AreaRunNCSign, 
					Uint32 i,
					Uint8 ModuleSel
					) 
{     
	float64 	ARC_MAXFeedrate;			//圆弧最大进给速度,单位:mm/s
	Uint32 ri;
	
	ri = (i-1) % STDGCODE_MOD;
	#if 0
	//old code for reference
	(AreaGcodeBufferPointer+ri)->Vm = ((AreaGcodeBufferPointer+ri)->CMDMain == 0)? 
		(divdp( (AreaGcodeBufferPointer+ri)->Feedrate*OverallSign.LastRapidFeedrateOverride, 10.0 )):
		(divdp( (AreaGcodeBufferPointer+ri)->Feedrate*OverallSign.LastFeedrateOverride, 10.0 ));
	
	//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
	
	return (AreaGcodeBufferPointer+ri)->Vm;
	#endif

	if(ModuleSel == 1)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module1 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module1*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module1;
	}
	else if(ModuleSel == 2)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module2 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module2*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module2;
	}
	else if(ModuleSel == 3)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module3 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module3*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module3;
	}
	else if(ModuleSel == 4)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module4 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module4*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module4;
	}
	else if(ModuleSel == 5)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module5 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module5*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module5;
	}
	else if(ModuleSel == 6)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module6 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module6*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module6;
	}
	else if(ModuleSel == 7)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module7 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module7*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module7;
	}
	else if(ModuleSel == 8)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module8 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module8*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module8;
	}
	else if(ModuleSel == 9)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module9 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module9*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module9;
	}
	else if(ModuleSel == 10)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module10 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module10*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module10;
	}
	else if(ModuleSel == 11)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module11=  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module11*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module11;
	}
	else if(ModuleSel == 12)
	{
		(AreaGcodeBufferPointer+ri)->Vm_Module.Module12 =  divdp( (AreaGcodeBufferPointer+ri)->Feedrate_Module.Module12*OverallSign.LastFeedrateOverride, 10.0 );
		//LimitAxisSpeed( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);			//opened 20090912	
		return (AreaGcodeBufferPointer+ri)->Vm_Module.Module12;
	}
}  


//CH200906
//计算i1线段和i2线段的转角关系
float64	NCArea_Cal_i1_i2_Vveer(float64 a_de, GCODE * AreaGcodeBufferPointer, Uint32 i1,Uint32 i2,Uint8 ModuleSel)
{
	float64	Vveer2;
	VECTOR 	I1EndVector,I2StartVector;
	float64 cos_angle,sin_half_angle;
	GCODE 			*i1CodePT,*i2CodePT;

	i1CodePT = AreaGcodeBufferPointer + (i1-1) % STDGCODE_MOD;
	i2CodePT = AreaGcodeBufferPointer + (i2-1) % STDGCODE_MOD;

	I1EndVector = i1CodePT->EndPointVector;
	I2StartVector = i2CodePT->StPointVector;

	if(ModuleSel == 1)
	{	
		cos_angle = I1EndVector.Axis15*I2StartVector.Axis15+I1EndVector.Axis16*I2StartVector.Axis16+I1EndVector.Axis1*I2StartVector.Axis1;
	}
	else if(ModuleSel == 2)
	{			
		cos_angle = I1EndVector.Axis17*I2StartVector.Axis17+I1EndVector.Axis18*I2StartVector.Axis18;
	}
	else if(ModuleSel == 3)
	{				
		cos_angle = I1EndVector.Axis3*I2StartVector.Axis3+I1EndVector.Axis4*I2StartVector.Axis4+I1EndVector.Axis5*I2StartVector.Axis5;
	}
	else if(ModuleSel == 4)
	{		
		cos_angle = I1EndVector.Axis8*I2StartVector.Axis8+I1EndVector.Axis7*I2StartVector.Axis7;
	}
	else if(ModuleSel == 5)
	{				
		cos_angle = I1EndVector.Axis6*I2StartVector.Axis6+I1EndVector.Axis20*I2StartVector.Axis20;
	}
	else if(ModuleSel == 6)
	{		
		cos_angle = I1EndVector.Axis11*I2StartVector.Axis11+I1EndVector.Axis12*I2StartVector.Axis12;
	}
	else if(ModuleSel == 7)
	{		
		cos_angle = I1EndVector.Axis21*I2StartVector.Axis21+I1EndVector.Axis22*I2StartVector.Axis22+I1EndVector.Axis9*I2StartVector.Axis9;
	}
	else if(ModuleSel == 8)
	{	
		cos_angle = I1EndVector.Axis2*I2StartVector.Axis2;
	}
	else if(ModuleSel == 9)
	{	
		cos_angle = I1EndVector.Axis10*I2StartVector.Axis10;
	}	
	else if(ModuleSel == 10)
	{		
		cos_angle = I1EndVector.Axis13*I2StartVector.Axis13+I1EndVector.Axis14*I2StartVector.Axis14;
	}
	else if(ModuleSel == 11)
	{	
		cos_angle = I1EndVector.Axis19*I2StartVector.Axis19;
	}
	else if(ModuleSel == 12)
	{	
		cos_angle = I1EndVector.Axis23*I2StartVector.Axis23;
	}
			
	sin_half_angle = sqrtdp( divdp( fabs(1-cos_angle), 2) ) ;
	
	Vveer2 = divdp( a_de * System.VeerDeltaT, ( 2*sin_half_angle) );	
	
	return Vveer2;
}

/*
*******************************************************************************
*				STD_SPC 区域曲线: 	时间对插补周期量化
*
* 1. 稳速段: 	不做量化，插补尾巴由区域终点算法处理。

* 2. 加速段: 	做量化，因为实际加速度未知，所以要用S,Vs,Ve
				定出DeltaT1，再对DeltaT1做量化，修正加速度，修
				正加速度时，根据公式s=powdp( Vm,2)/a,必须更改Vm。
				
* 3. 减速段: 	同加速段。

* 4. 单条代码: 重点是按照公式修正Vm；

* 4. 特殊代码: 单独针对于Vs>Vm的情况。

*******************************************************************************
*/
//STD 区域控制预计算
Uint16 NCAreaControl(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign)
{
	//第一步:  计算向后线段搜索距离
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,1);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,2);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,3);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,4);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,5);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,6);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,7);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,8);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,9);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,10);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,11);
	NCAreaCalSs(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,12);

	//第二步:  计算区域类各线段允许终点速度
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,1);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,2);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,3);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,4);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,5);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,6);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,7);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,8);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,9);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,10);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,11);
	NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,12);

	//第三步: 搜索区域一次假定拐点
	NCAreaFirstInflexion(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,1);

	//第四步: 搜索区域二次假定拐点
	AREAControl->Pointer_SecondInflexionCode = AREAControl->Pointer_FirstInflexionCode;

	//第五步: 拐点确定区域运动控制预计算
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,1);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,2);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,3);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,4);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,5);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,6);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,7);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,8);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,9);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,10);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,11);	
	NCAreaRunPreCal(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign,12);	

	return 1;
}

//第一步:  计算STD 区域向后搜索距离
Uint16	NCAreaCalSs(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel)
{
	GCODE *RiCodePT;

	RiCodePT = AreaGcodeBufferPointer + (AreaRunNCSign->RealRunCount - 1)% STDGCODE_MOD;

	//NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount);
	//AREAControl->Vm = RiCodePT->Vm;
	
	if(ModuleSel == 1)
	{
		//RiCodePT->Vm_Module.Module1 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module1 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module1 = System.a_SET_Module.Module1;
		AREAControl->a_de_Module.Module1 = AREAControl->a_ac_Module.Module1;
				
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module1 = AREAControl->VNow_Module.Module1;
		AREAControl->Vm_Module.Module1 = RiCodePT->Vm_Module.Module1;
		AREAControl->Ss_Module.Module1 = divdp( powdp(AREAControl->Vm_Module.Module1,2), AREAControl->a_de_Module.Module1 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module1 <= AREAControl->VMin_Module.Module1)
			AREAControl->VMin_Module.Module1 = AREAControl->Vm_Module.Module1;

		return 1;

	}
	else if(ModuleSel == 2)
	{
		//RiCodePT->Vm_Module.Module2 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module2 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module2 = System.a_SET_Module.Module2;
		AREAControl->a_de_Module.Module2 = AREAControl->a_ac_Module.Module2;				
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;		
		AREAControl->Vs_Module.Module2 = AREAControl->VNow_Module.Module2;
		AREAControl->Vm_Module.Module2 = RiCodePT->Vm_Module.Module2;
		AREAControl->Ss_Module.Module2 = divdp( powdp(AREAControl->Vm_Module.Module2,2), AREAControl->a_de_Module.Module2 );	
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module2 <= AREAControl->VMin_Module.Module2)
			AREAControl->VMin_Module.Module2 = AREAControl->Vm_Module.Module2;

		return 1;

	}
	else if(ModuleSel == 3)
	{
		//RiCodePT->Vm_Module.Module3 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module3 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module3 = System.a_SET_Module.Module3;
		AREAControl->a_de_Module.Module3 = AREAControl->a_ac_Module.Module3;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module3 = AREAControl->VNow_Module.Module3;
		AREAControl->Vm_Module.Module3 = RiCodePT->Vm_Module.Module3;
		AREAControl->Ss_Module.Module3 = divdp( powdp(AREAControl->Vm_Module.Module3,2), AREAControl->a_de_Module.Module3 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module3 <= AREAControl->VMin_Module.Module3)
			AREAControl->VMin_Module.Module3 = AREAControl->Vm_Module.Module3;

		return 1;

	}
	else if(ModuleSel == 4)
	{
		//RiCodePT->Vm_Module.Module4 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module4 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module4 = System.a_SET_Module.Module4;
		AREAControl->a_de_Module.Module4 = AREAControl->a_ac_Module.Module4;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module4 = AREAControl->VNow_Module.Module4;
		AREAControl->Vm_Module.Module4 = RiCodePT->Vm_Module.Module4;
		AREAControl->Ss_Module.Module4 = divdp( powdp(AREAControl->Vm_Module.Module4,2), AREAControl->a_de_Module.Module4 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module4 <= AREAControl->VMin_Module.Module4)
			AREAControl->VMin_Module.Module4 = AREAControl->Vm_Module.Module4;

		return 1;

	}
	else if(ModuleSel == 5)
	{
		//RiCodePT->Vm_Module.Module5 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module5 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module5 = System.a_SET_Module.Module5;
		AREAControl->a_de_Module.Module5 = AREAControl->a_ac_Module.Module5;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module5 = AREAControl->VNow_Module.Module5;
		AREAControl->Vm_Module.Module5 = RiCodePT->Vm_Module.Module5;
		AREAControl->Ss_Module.Module5 = divdp( powdp(AREAControl->Vm_Module.Module5,2), AREAControl->a_de_Module.Module5 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module5 <= AREAControl->VMin_Module.Module5)
			AREAControl->VMin_Module.Module5 = AREAControl->Vm_Module.Module5;

		return 1;

	}
	else if(ModuleSel == 6)
	{
		//RiCodePT->Vm_Module.Module6 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module6 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module6 = System.a_SET_Module.Module6;
		AREAControl->a_de_Module.Module6 = AREAControl->a_ac_Module.Module6;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module6 = AREAControl->VNow_Module.Module6;
		AREAControl->Vm_Module.Module6 = RiCodePT->Vm_Module.Module6;
		AREAControl->Ss_Module.Module6 = divdp( powdp(AREAControl->Vm_Module.Module6,2), AREAControl->a_de_Module.Module6 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module6 <= AREAControl->VMin_Module.Module6)
			AREAControl->VMin_Module.Module6 = AREAControl->Vm_Module.Module6;

		return 1;

	}
	else if(ModuleSel == 7)
	{
		//RiCodePT->Vm_Module.Module7 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module7 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module7 = System.a_SET_Module.Module7;
		AREAControl->a_de_Module.Module7 = AREAControl->a_ac_Module.Module7;
		
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module7 = AREAControl->VNow_Module.Module7;
		AREAControl->Vm_Module.Module7 = RiCodePT->Vm_Module.Module7;
		AREAControl->Ss_Module.Module7 = divdp( powdp(AREAControl->Vm_Module.Module7,2), AREAControl->a_de_Module.Module7 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module7 <= AREAControl->VMin_Module.Module7)
			AREAControl->VMin_Module.Module7 = AREAControl->Vm_Module.Module7;

		return 1;

	}
	else if(ModuleSel == 8)
	{
		//RiCodePT->Vm_Module.Module8 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module8 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module8 = System.a_A4_SET_Module.Module8;
		AREAControl->a_de_Module.Module8 = AREAControl->a_ac_Module.Module8;
				
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module8 = AREAControl->VNow_Module.Module8;
		AREAControl->Vm_Module.Module8 = RiCodePT->Vm_Module.Module8;
		AREAControl->Ss_Module.Module8 = divdp( powdp(AREAControl->Vm_Module.Module8,2), AREAControl->a_de_Module.Module8 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module8 <= AREAControl->VMin_Module.Module8)
			AREAControl->VMin_Module.Module8 = AREAControl->Vm_Module.Module8;

		return 1;

	}
	else if(ModuleSel == 9)
	{
		//RiCodePT->Vm_Module.Module9 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module9 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module9 = System.a_A4_SET_Module.Module9;
		AREAControl->a_de_Module.Module9 = AREAControl->a_ac_Module.Module9;
				
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module9 = AREAControl->VNow_Module.Module9;
		AREAControl->Vm_Module.Module9 = RiCodePT->Vm_Module.Module9;
		AREAControl->Ss_Module.Module9 = divdp( powdp(AREAControl->Vm_Module.Module9,2), AREAControl->a_de_Module.Module9 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module9 <= AREAControl->VMin_Module.Module9)
			AREAControl->VMin_Module.Module9 = AREAControl->Vm_Module.Module9;

		return 1;

	}
	else if(ModuleSel == 10)
	{
		//RiCodePT->Vm_Module.Module10 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module10 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module10 = System.a_SET_Module.Module10;
		AREAControl->a_de_Module.Module10 = AREAControl->a_ac_Module.Module10;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module10 = AREAControl->VNow_Module.Module10;
		AREAControl->Vm_Module.Module10 = RiCodePT->Vm_Module.Module10;
		AREAControl->Ss_Module.Module10 = divdp( powdp(AREAControl->Vm_Module.Module10,2), AREAControl->a_de_Module.Module10 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module10 <= AREAControl->VMin_Module.Module10)
			AREAControl->VMin_Module.Module10 = AREAControl->Vm_Module.Module10;

		return 1;

	}
	else if(ModuleSel == 11)
	{
		//RiCodePT->Vm_Module.Module11 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module11 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module11 = System.a_SET_Module.Module11;
		AREAControl->a_de_Module.Module11 = AREAControl->a_ac_Module.Module11;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module11 = AREAControl->VNow_Module.Module11;
		AREAControl->Vm_Module.Module11 = RiCodePT->Vm_Module.Module11;
		AREAControl->Ss_Module.Module11 = divdp( powdp(AREAControl->Vm_Module.Module11,2), AREAControl->a_de_Module.Module11 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module11 <= AREAControl->VMin_Module.Module11)
			AREAControl->VMin_Module.Module11 = AREAControl->Vm_Module.Module11;

		return 1;

	}
	else if(ModuleSel == 12)
	{
		//RiCodePT->Vm_Module.Module12 = RiCodePT->Vm;
		RiCodePT->Vm_Module.Module12 = NCAreaVm( AREAControl, AreaGcodeBufferPointer, AreaRunNCSign, AreaRunNCSign->RealRunCount,ModuleSel);
		AREAControl->a_ac_Module.Module12 = System.a_SET_Module.Module12;
		AREAControl->a_de_Module.Module12 = AREAControl->a_ac_Module.Module12;			
		AREAControl->Pointer_StartCode = AreaRunNCSign->RealRunCount;
		AREAControl->Pointer_EndCode = AREAControl->Pointer_StartCode;				
		AREAControl->Vs_Module.Module12 = AREAControl->VNow_Module.Module12;
		AREAControl->Vm_Module.Module12 = RiCodePT->Vm_Module.Module12;
		AREAControl->Ss_Module.Module12 = divdp( powdp(AREAControl->Vm_Module.Module12,2), AREAControl->a_de_Module.Module12 );
		AREAControl->VMin = System.VeerDeltaV;
			
		if( AREAControl->Vm_Module.Module12 <= AREAControl->VMin_Module.Module12)
			AREAControl->VMin_Module.Module12 = AREAControl->Vm_Module.Module12;

		return 1;

	}

	
}

//第三步: 搜索STD 区域一次假定拐点
Uint16	NCAreaFirstInflexion(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel)
{
	
	AREAControl->Pointer_FirstInflexionCode = AREAControl->Pointer_EndCode; 
	
	if( AREAControl->Pointer_StartCode == AREAControl->Pointer_EndCode)		//单条区域计算,防止CodeNextPT指乱
	{
		AREAControl->SpeedState.Module1 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module2 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module3 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module4 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module5 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module6 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module7 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module8 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module9 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module10 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module11 = NCAREA_SINGLE_STATE;	//单条加减速
		AREAControl->SpeedState.Module12 = NCAREA_SINGLE_STATE;	//单条加减速


		if( AREAControl->Vs_Module.Module1 > AREAControl->Vm_Module.Module1)				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module1 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module2 > AREAControl->Vm_Module.Module2 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module2 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module3 > AREAControl->Vm_Module.Module3 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module3 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module4 > AREAControl->Vm_Module.Module4 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module4 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module5 > AREAControl->Vm_Module.Module5 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module5 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module6 > AREAControl->Vm_Module.Module6 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module6 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module7 > AREAControl->Vm_Module.Module7 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module7 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module8 > AREAControl->Vm_Module.Module8 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module8 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module9 > AREAControl->Vm_Module.Module9 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module9 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module10 > AREAControl->Vm_Module.Module10 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module10 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module11 > AREAControl->Vm_Module.Module11 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module11 = NCAREA_SPECIAL_STATE;		//特殊区域
		}
		if( AREAControl->Vs_Module.Module12 > AREAControl->Vm_Module.Module12 )				//用于变档等速度急降处理
		{
			AREAControl->SpeedState.Module12 = NCAREA_SPECIAL_STATE;		//特殊区域
		}

	}
	
	return 1;
}

//第五步: 拐点确定STD 区域运动控制预计算
Uint16 NCAreaRunPreCal(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign, Uint8 ModuleSel)
{		
	GCODE *SecondInflexionCodePT;

	SecondInflexionCodePT = AreaGcodeBufferPointer + (AREAControl->Pointer_SecondInflexionCode-1) % STDGCODE_MOD;

	if(ModuleSel == 1)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module1 = 0;	
		AREAControl->Stotal_Module.Module1 = SecondInflexionCodePT->Stotal_Module.Module1;
		AREAControl->Ve_Module.Module1 = SecondInflexionCodePT->Ve_Module.Module1;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module1 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,1);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module1 = 0;//AREAControl->Vm_Module.Module1
			AREAControl->Sac_Module.Module1 = 0;
			AREAControl->Sde_Module.Module1 = 0;
			AREAControl->Sm_Module.Module1 = 0;
			AREAControl->DeltaT1_Module.Module1 = 0;
			AREAControl->DeltaT2_Module.Module1 = 0;
			AREAControl->DeltaT3_Module.Module1 = 0;
			AREAControl->DeltaT4_Module.Module1 = 0;
			AREAControl->DeltaT5_Module.Module1 = 0;
			AREAControl->S1_Module.Module1 = 0;
			AREAControl->S2_Module.Module1 = 0;
			AREAControl->S3_Module.Module1 = 0;
			AREAControl->S4_Module.Module1 = 0;
			AREAControl->S5_Module.Module1 = 0;
			AREAControl->T1_Module.Module1 = 0;
			AREAControl->T2_Module.Module1 = 0;		
			AREAControl->T3_Module.Module1 = 0;	 	
			AREAControl->T4_Module.Module1 = 0;	
			AREAControl->T5_Module.Module1 = 0;			
			AREAControl->TotalN_Module.Module1 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 2)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module2 = 0;	
		AREAControl->Stotal_Module.Module2 = SecondInflexionCodePT->Stotal_Module.Module2;
		AREAControl->Ve_Module.Module2 = SecondInflexionCodePT->Ve_Module.Module2;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module2 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,2);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module2 = 0;//AREAControl->Vm_Module.Module2
			AREAControl->Sac_Module.Module2 = 0;
			AREAControl->Sde_Module.Module2 = 0;
			AREAControl->Sm_Module.Module2 = 0;
			AREAControl->DeltaT1_Module.Module2 = 0;
			AREAControl->DeltaT2_Module.Module2 = 0;
			AREAControl->DeltaT3_Module.Module2 = 0;
			AREAControl->DeltaT4_Module.Module2 = 0;
			AREAControl->DeltaT5_Module.Module2 = 0;
			AREAControl->S1_Module.Module2 = 0;
			AREAControl->S2_Module.Module2 = 0;
			AREAControl->S3_Module.Module2 = 0;
			AREAControl->S4_Module.Module2 = 0;
			AREAControl->S5_Module.Module2 = 0;
			AREAControl->T1_Module.Module2 = 0;
			AREAControl->T2_Module.Module2 = 0;		
			AREAControl->T3_Module.Module2 = 0;	 	
			AREAControl->T4_Module.Module2 = 0;	
			AREAControl->T5_Module.Module2 = 0;			
			AREAControl->TotalN_Module.Module2 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 3)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module3 = 0;	
		AREAControl->Stotal_Module.Module3 = SecondInflexionCodePT->Stotal_Module.Module3;
		AREAControl->Ve_Module.Module3 = SecondInflexionCodePT->Ve_Module.Module3;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module3 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,3);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module3 = 0;//AREAControl->Vm_Module.Module3
			AREAControl->Sac_Module.Module3 = 0;
			AREAControl->Sde_Module.Module3 = 0;
			AREAControl->Sm_Module.Module3 = 0;
			AREAControl->DeltaT1_Module.Module3 = 0;
			AREAControl->DeltaT2_Module.Module3 = 0;
			AREAControl->DeltaT3_Module.Module3 = 0;
			AREAControl->DeltaT4_Module.Module3 = 0;
			AREAControl->DeltaT5_Module.Module3 = 0;
			AREAControl->S1_Module.Module3 = 0;
			AREAControl->S2_Module.Module3 = 0;
			AREAControl->S3_Module.Module3 = 0;
			AREAControl->S4_Module.Module3 = 0;
			AREAControl->S5_Module.Module3 = 0;
			AREAControl->T1_Module.Module3 = 0;
			AREAControl->T2_Module.Module3 = 0;		
			AREAControl->T3_Module.Module3 = 0;	 	
			AREAControl->T4_Module.Module3 = 0;	
			AREAControl->T5_Module.Module3 = 0;			
			AREAControl->TotalN_Module.Module3 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 4)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module4 = 0;	
		AREAControl->Stotal_Module.Module4 = SecondInflexionCodePT->Stotal_Module.Module4;
		AREAControl->Ve_Module.Module4 = SecondInflexionCodePT->Ve_Module.Module4;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module4 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,4);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module4 = 0;//AREAControl->Vm_Module.Module4
			AREAControl->Sac_Module.Module4 = 0;
			AREAControl->Sde_Module.Module4 = 0;
			AREAControl->Sm_Module.Module4 = 0;
			AREAControl->DeltaT1_Module.Module4 = 0;
			AREAControl->DeltaT2_Module.Module4 = 0;
			AREAControl->DeltaT3_Module.Module4 = 0;
			AREAControl->DeltaT4_Module.Module4 = 0;
			AREAControl->DeltaT5_Module.Module4 = 0;
			AREAControl->S1_Module.Module4 = 0;
			AREAControl->S2_Module.Module4 = 0;
			AREAControl->S3_Module.Module4 = 0;
			AREAControl->S4_Module.Module4 = 0;
			AREAControl->S5_Module.Module4 = 0;
			AREAControl->T1_Module.Module4 = 0;
			AREAControl->T2_Module.Module4 = 0;		
			AREAControl->T3_Module.Module4 = 0;	 	
			AREAControl->T4_Module.Module4 = 0;	
			AREAControl->T5_Module.Module4 = 0;			
			AREAControl->TotalN_Module.Module4 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 5)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module5 = 0;	
		AREAControl->Stotal_Module.Module5 = SecondInflexionCodePT->Stotal_Module.Module5;
		AREAControl->Ve_Module.Module5 = SecondInflexionCodePT->Ve_Module.Module5;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module5 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,5);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module5 = 0;//AREAControl->Vm_Module.Module5
			AREAControl->Sac_Module.Module5 = 0;
			AREAControl->Sde_Module.Module5 = 0;
			AREAControl->Sm_Module.Module5 = 0;
			AREAControl->DeltaT1_Module.Module5 = 0;
			AREAControl->DeltaT2_Module.Module5 = 0;
			AREAControl->DeltaT3_Module.Module5 = 0;
			AREAControl->DeltaT4_Module.Module5 = 0;
			AREAControl->DeltaT5_Module.Module5 = 0;
			AREAControl->S1_Module.Module5 = 0;
			AREAControl->S2_Module.Module5 = 0;
			AREAControl->S3_Module.Module5 = 0;
			AREAControl->S4_Module.Module5 = 0;
			AREAControl->S5_Module.Module5 = 0;
			AREAControl->T1_Module.Module5 = 0;
			AREAControl->T2_Module.Module5 = 0;		
			AREAControl->T3_Module.Module5 = 0;	 	
			AREAControl->T4_Module.Module5 = 0;	
			AREAControl->T5_Module.Module5 = 0;			
			AREAControl->TotalN_Module.Module5 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 6)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module6 = 0;	
		AREAControl->Stotal_Module.Module6 = SecondInflexionCodePT->Stotal_Module.Module6;
		AREAControl->Ve_Module.Module6 = SecondInflexionCodePT->Ve_Module.Module6;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module6 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,6);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module6 = 0;//AREAControl->Vm_Module.Module6
			AREAControl->Sac_Module.Module6 = 0;
			AREAControl->Sde_Module.Module6 = 0;
			AREAControl->Sm_Module.Module6 = 0;
			AREAControl->DeltaT1_Module.Module6 = 0;
			AREAControl->DeltaT2_Module.Module6 = 0;
			AREAControl->DeltaT3_Module.Module6 = 0;
			AREAControl->DeltaT4_Module.Module6 = 0;
			AREAControl->DeltaT5_Module.Module6 = 0;
			AREAControl->S1_Module.Module6 = 0;
			AREAControl->S2_Module.Module6 = 0;
			AREAControl->S3_Module.Module6 = 0;
			AREAControl->S4_Module.Module6 = 0;
			AREAControl->S5_Module.Module6 = 0;
			AREAControl->T1_Module.Module6 = 0;
			AREAControl->T2_Module.Module6 = 0;		
			AREAControl->T3_Module.Module6 = 0;	 	
			AREAControl->T4_Module.Module6 = 0;	
			AREAControl->T5_Module.Module6 = 0;			
			AREAControl->TotalN_Module.Module6 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 7)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module7 = 0;	
		AREAControl->Stotal_Module.Module7 = SecondInflexionCodePT->Stotal_Module.Module7;
		AREAControl->Ve_Module.Module7 = SecondInflexionCodePT->Ve_Module.Module7;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module7 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,7);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module7 = 0;//AREAControl->Vm_Module.Module7
			AREAControl->Sac_Module.Module7 = 0;
			AREAControl->Sde_Module.Module7 = 0;
			AREAControl->Sm_Module.Module7 = 0;
			AREAControl->DeltaT1_Module.Module7 = 0;
			AREAControl->DeltaT2_Module.Module7 = 0;
			AREAControl->DeltaT3_Module.Module7 = 0;
			AREAControl->DeltaT4_Module.Module7 = 0;
			AREAControl->DeltaT5_Module.Module7 = 0;
			AREAControl->S1_Module.Module7 = 0;
			AREAControl->S2_Module.Module7 = 0;
			AREAControl->S3_Module.Module7 = 0;
			AREAControl->S4_Module.Module7 = 0;
			AREAControl->S5_Module.Module7 = 0;
			AREAControl->T1_Module.Module7 = 0;
			AREAControl->T2_Module.Module7 = 0;		
			AREAControl->T3_Module.Module7 = 0;	 	
			AREAControl->T4_Module.Module7 = 0;	
			AREAControl->T5_Module.Module7 = 0;			
			AREAControl->TotalN_Module.Module7 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 8)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module8 = 0;	
		AREAControl->Stotal_Module.Module8 = SecondInflexionCodePT->Stotal_Module.Module8;
		AREAControl->Ve_Module.Module8 = SecondInflexionCodePT->Ve_Module.Module8;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module8 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,8);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module8 = 0;//AREAControl->Vm_Module.Module8
			AREAControl->Sac_Module.Module8 = 0;
			AREAControl->Sde_Module.Module8 = 0;
			AREAControl->Sm_Module.Module8 = 0;
			AREAControl->DeltaT1_Module.Module8 = 0;
			AREAControl->DeltaT2_Module.Module8 = 0;
			AREAControl->DeltaT3_Module.Module8 = 0;
			AREAControl->DeltaT4_Module.Module8 = 0;
			AREAControl->DeltaT5_Module.Module8 = 0;
			AREAControl->S1_Module.Module8 = 0;
			AREAControl->S2_Module.Module8 = 0;
			AREAControl->S3_Module.Module8 = 0;
			AREAControl->S4_Module.Module8 = 0;
			AREAControl->S5_Module.Module8 = 0;
			AREAControl->T1_Module.Module8 = 0;
			AREAControl->T2_Module.Module8 = 0;		
			AREAControl->T3_Module.Module8 = 0;	 	
			AREAControl->T4_Module.Module8 = 0;	
			AREAControl->T5_Module.Module8 = 0;			
			AREAControl->TotalN_Module.Module8 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 9)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module9 = 0;	
		AREAControl->Stotal_Module.Module9 = SecondInflexionCodePT->Stotal_Module.Module9;
		AREAControl->Ve_Module.Module9 = SecondInflexionCodePT->Ve_Module.Module9;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module9 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,9);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module9 = 0;//AREAControl->Vm_Module.Module9
			AREAControl->Sac_Module.Module9 = 0;
			AREAControl->Sde_Module.Module9 = 0;
			AREAControl->Sm_Module.Module9 = 0;
			AREAControl->DeltaT1_Module.Module9 = 0;
			AREAControl->DeltaT2_Module.Module9 = 0;
			AREAControl->DeltaT3_Module.Module9 = 0;
			AREAControl->DeltaT4_Module.Module9 = 0;
			AREAControl->DeltaT5_Module.Module9 = 0;
			AREAControl->S1_Module.Module9 = 0;
			AREAControl->S2_Module.Module9 = 0;
			AREAControl->S3_Module.Module9 = 0;
			AREAControl->S4_Module.Module9 = 0;
			AREAControl->S5_Module.Module9 = 0;
			AREAControl->T1_Module.Module9 = 0;
			AREAControl->T2_Module.Module9 = 0;		
			AREAControl->T3_Module.Module9 = 0;	 	
			AREAControl->T4_Module.Module9 = 0;	
			AREAControl->T5_Module.Module9 = 0;			
			AREAControl->TotalN_Module.Module9 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 10)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module10 = 0;	
		AREAControl->Stotal_Module.Module10 = SecondInflexionCodePT->Stotal_Module.Module10;
		AREAControl->Ve_Module.Module10 = SecondInflexionCodePT->Ve_Module.Module10;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module10 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,10);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module10 = 0;//AREAControl->Vm_Module.Module10
			AREAControl->Sac_Module.Module10 = 0;
			AREAControl->Sde_Module.Module10 = 0;
			AREAControl->Sm_Module.Module10 = 0;
			AREAControl->DeltaT1_Module.Module10 = 0;
			AREAControl->DeltaT2_Module.Module10 = 0;
			AREAControl->DeltaT3_Module.Module10 = 0;
			AREAControl->DeltaT4_Module.Module10 = 0;
			AREAControl->DeltaT5_Module.Module10 = 0;
			AREAControl->S1_Module.Module10 = 0;
			AREAControl->S2_Module.Module10 = 0;
			AREAControl->S3_Module.Module10 = 0;
			AREAControl->S4_Module.Module10 = 0;
			AREAControl->S5_Module.Module10 = 0;
			AREAControl->T1_Module.Module10 = 0;
			AREAControl->T2_Module.Module10 = 0;		
			AREAControl->T3_Module.Module10 = 0;	 	
			AREAControl->T4_Module.Module10 = 0;	
			AREAControl->T5_Module.Module10 = 0;			
			AREAControl->TotalN_Module.Module10 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 11)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module11 = 0;	
		AREAControl->Stotal_Module.Module11 = SecondInflexionCodePT->Stotal_Module.Module11;
		AREAControl->Ve_Module.Module11 = SecondInflexionCodePT->Ve_Module.Module11;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module11 > 0.000001)
		{
			//NCAreaHyperbolaPrecalStandardCurve(AREAControl,11);
			AREAControl->Vs_Module.Module11 = 5;//5mm/s	
			AREAControl->Ve_Module.Module11 = 5;//5mm/s	
			
			AREAControl->Sac_Module.Module11 = 0;
			AREAControl->Sde_Module.Module11 = 0;
			AREAControl->Sm_Module.Module11 = AREAControl->Stotal_Module.Module11;
			AREAControl->VmReal_Module.Module11 = 5;//5mm/s		
			AREAControl->DeltaT1_Module.Module11 = 0;
			AREAControl->DeltaT2_Module.Module11 = 0;	
			AREAControl->DeltaT3_Module.Module11 = divdp( AREAControl->Stotal_Module.Module11 , 5);
			AREAControl->DeltaT4_Module.Module11	= 0;
			AREAControl->DeltaT5_Module.Module11	= 0;
			AREAControl->S1_Module.Module11			= 0;
			AREAControl->S2_Module.Module11			= 0;	
			AREAControl->S3_Module.Module11			= AREAControl->Stotal_Module.Module11;	
			AREAControl->S4_Module.Module11			= 0;
			AREAControl->S5_Module.Module11			= 0;   
			
			AREAControl->T1_Module.Module11 	= AREAControl->DeltaT1_Module.Module11;
			AREAControl->T2_Module.Module11 	= AREAControl->T1_Module.Module11+AREAControl->DeltaT2_Module.Module11;
			AREAControl->T3_Module.Module11 	= AREAControl->T2_Module.Module11+AREAControl->DeltaT3_Module.Module11;
			AREAControl->T4_Module.Module11 	= AREAControl->T3_Module.Module11+AREAControl->DeltaT4_Module.Module11;
			AREAControl->T5_Module.Module11 	= AREAControl->T4_Module.Module11+AREAControl->DeltaT5_Module.Module11;
			AREAControl->TotalN_Module.Module11 = divdp(AREAControl->T5_Module.Module11, System.Tsample);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module11 = 0;//AREAControl->Vm_Module.Module11
			AREAControl->Sac_Module.Module11 = 0;
			AREAControl->Sde_Module.Module11 = 0;
			AREAControl->Sm_Module.Module11 = 0;
			AREAControl->DeltaT1_Module.Module11 = 0;
			AREAControl->DeltaT2_Module.Module11 = 0;
			AREAControl->DeltaT3_Module.Module11 = 0;
			AREAControl->DeltaT4_Module.Module11 = 0;
			AREAControl->DeltaT5_Module.Module11 = 0;
			AREAControl->S1_Module.Module11 = 0;
			AREAControl->S2_Module.Module11 = 0;
			AREAControl->S3_Module.Module11 = 0;
			AREAControl->S4_Module.Module11 = 0;
			AREAControl->S5_Module.Module11 = 0;
			AREAControl->T1_Module.Module11 = 0;
			AREAControl->T2_Module.Module11 = 0;		
			AREAControl->T3_Module.Module11 = 0;	 	
			AREAControl->T4_Module.Module11 = 0;	
			AREAControl->T5_Module.Module11 = 0;			
			AREAControl->TotalN_Module.Module11 = 0;
			return 1;
		}
	}
	else if(ModuleSel == 12)
	{
		//计算起点到二次假定拐点的长度
		AREAControl->Stotal_Module.Module12 = 0;	
		AREAControl->Stotal_Module.Module12 = SecondInflexionCodePT->Stotal_Module.Module12;
		AREAControl->Ve_Module.Module12 = SecondInflexionCodePT->Ve_Module.Module12;

		//构造起点到二次假定拐点的区域曲线
		if(AREAControl->Stotal_Module.Module12 > 0.000001)
		{
			NCAreaHyperbolaPrecalStandardCurve(AREAControl,12);
			return 1;
		}
		else
		{
			AREAControl->VmReal_Module.Module12 = 0;//AREAControl->Vm_Module.Module12
			AREAControl->Sac_Module.Module12 = 0;
			AREAControl->Sde_Module.Module12 = 0;
			AREAControl->Sm_Module.Module12 = 0;
			AREAControl->DeltaT1_Module.Module12 = 0;
			AREAControl->DeltaT2_Module.Module12 = 0;
			AREAControl->DeltaT3_Module.Module12 = 0;
			AREAControl->DeltaT4_Module.Module12 = 0;
			AREAControl->DeltaT5_Module.Module12 = 0;
			AREAControl->S1_Module.Module12 = 0;
			AREAControl->S2_Module.Module12 = 0;
			AREAControl->S3_Module.Module12 = 0;
			AREAControl->S4_Module.Module12 = 0;
			AREAControl->S5_Module.Module12 = 0;
			AREAControl->T1_Module.Module12 = 0;
			AREAControl->T2_Module.Module12 = 0;		
			AREAControl->T3_Module.Module12 = 0;	 	
			AREAControl->T4_Module.Module12 = 0;	
			AREAControl->T5_Module.Module12 = 0;			
			AREAControl->TotalN_Module.Module12 = 0;
			return 1;
		}
	}
}

Uint16 NCAreaHyperbolaPrecalStandardCurve(AREA_CONTROL *AREAControl, Uint8 ModuleSel)	//预计算标准曲线参数
{
	float64 acc,a_de_sd;

	if(ModuleSel == 1)
	{
		if( AREAControl->SpeedState.Module1 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module1 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module1,2) - powdp(AREAControl->Vs_Module.Module1,2)), AREAControl->a_ac_Module.Module1) );
			AREAControl->Sde_Module.Module1 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module1,2) - powdp(AREAControl->Ve_Module.Module1,2)), AREAControl->a_de_Module.Module1) );

			if(AREAControl->Stotal_Module.Module1 >= (AREAControl->Sac_Module.Module1+AREAControl->Sde_Module.Module1)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module1 = AREAControl->Vm_Module.Module1;
				AREAControl->Sm_Module.Module1 	= AREAControl->Ss_Module.Module1-AREAControl->Sac_Module.Module1-AREAControl->Sde_Module.Module1;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module1 >= AREAControl->Vs_Module.Module1 ) ? (AREAControl->a_ac_Module.Module1):(AREAControl->a_de_Module.Module1);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module1, 2 ) - powdp( AREAControl->Vs_Module.Module1, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module1) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module1, 2 ) - powdp( AREAControl->Vs_Module.Module1, 2 ) ), AREAControl->Stotal_Module.Module1 );

					if( AREAControl->Ve_Module.Module1 >= AREAControl->Vs_Module.Module1 )
					{
						AREAControl->VmReal_Module.Module1 = AREAControl->Ve_Module.Module1;
						AREAControl->a_ac_Module.Module1 = divdp( fabs( powdp( AREAControl->Ve_Module.Module1, 2 ) - powdp( AREAControl->Vs_Module.Module1, 2 ) ), AREAControl->Stotal_Module.Module1 );
					}
					else
					{
						AREAControl->VmReal_Module.Module1 = AREAControl->Vs_Module.Module1;

						a_de_sd = AREAControl->a_de_Module.Module1;
						AREAControl->a_de_Module.Module1 = divdp( fabs( powdp( AREAControl->Ve_Module.Module1, 2 ) - powdp( AREAControl->Vs_Module.Module1, 2 ) ), AREAControl->Stotal_Module.Module1 );
						if(AREAControl->a_de_Module.Module1 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module1 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module1 = sqrtdp( powdp(AREAControl->Vs_Module.Module1,2) - AREAControl->a_de_Module.Module1*AREAControl->Stotal_Module.Module1 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module1 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module1 * AREAControl->a_de_Module.Module1 * AREAControl->Stotal_Module.Module1 + 
												AREAControl->a_de_Module.Module1 * powdp( AREAControl->Vs_Module.Module1, 2 ) + AREAControl->a_ac_Module.Module1 * 
												powdp( AREAControl->Ve_Module.Module1, 2 ) ),  ( AREAControl->a_ac_Module.Module1 + AREAControl->a_de_Module.Module1 ) ) );
					AREAControl->Sm_Module.Module1		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module1 	= divdp( fabs(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1), AREAControl->a_ac_Module.Module1);
			AREAControl->DeltaT2_Module.Module1 	= AREAControl->DeltaT1_Module.Module1;
			if(AREAControl->DeltaT1_Module.Module1 == 0)
			{
				AREAControl->S1_Module.Module1		= 0;
				AREAControl->S2_Module.Module1		= 0;
				AREAControl->Sac_Module.Module1	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module1		= divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->DeltaT1_Module.Module1,3), 
										(6*(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1))) + 
										AREAControl->Vs_Module.Module1*AREAControl->DeltaT1_Module.Module1;
				AREAControl->S2_Module.Module1 	= divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->DeltaT2_Module.Module1,3), 
										(6*(AREAControl->Vs_Module.Module1-AREAControl->VmReal_Module.Module1))) +
										AREAControl->VmReal_Module.Module1*AREAControl->DeltaT2_Module.Module1;
				AREAControl->Sac_Module.Module1	= AREAControl->S1_Module.Module1 + AREAControl->S2_Module.Module1;
			}		
			
			AREAControl->DeltaT4_Module.Module1 	= divdp( fabs(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1), AREAControl->a_de_Module.Module1 );
			AREAControl->DeltaT5_Module.Module1 	= AREAControl->DeltaT4_Module.Module1;
			if(AREAControl->DeltaT4_Module.Module1 == 0)
			{
				AREAControl->S4_Module.Module1 	= 0;
				AREAControl->S5_Module.Module1 	= 0;
				AREAControl->Sde_Module.Module1 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module1 	= divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp(AREAControl->DeltaT4_Module.Module1,3), 
										(6*(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1))) +
										AREAControl->VmReal_Module.Module1*AREAControl->DeltaT4_Module.Module1;
				AREAControl->S5_Module.Module1 	= divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp(AREAControl->DeltaT5_Module.Module1,3), 
										(6*(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1))) +
										AREAControl->Ve_Module.Module1*AREAControl->DeltaT5_Module.Module1;
				AREAControl->Sde_Module.Module1 	= AREAControl->S4_Module.Module1+AREAControl->S5_Module.Module1;
			}
					
			AREAControl->S3_Module.Module1 		= AREAControl->Stotal_Module.Module1-AREAControl->S1_Module.Module1-AREAControl->S2_Module.Module1-
										AREAControl->S4_Module.Module1-AREAControl->S5_Module.Module1;
			
			if(AREAControl->VmReal_Module.Module1==0) 
				AREAControl->DeltaT3_Module.Module1 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module1 > 0)
				{
					AREAControl->DeltaT3_Module.Module1 = divdp(AREAControl->S3_Module.Module1, AREAControl->VmReal_Module.Module1);
				}
				else
				{
					AREAControl->S3_Module.Module1 = 0;
					AREAControl->DeltaT3_Module.Module1 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module1 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module1 = AREAControl->a_de_Module.Module1;
			AREAControl->Sac_Module.Module1 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module1,2) - powdp(AREAControl->Vs_Module.Module1,2)), AREAControl->a_ac_Module.Module1) );
			AREAControl->Sde_Module.Module1 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module1,2) - powdp(AREAControl->Ve_Module.Module1,2)), AREAControl->a_de_Module.Module1) );

			if(AREAControl->Stotal_Module.Module1 >= (AREAControl->Sac_Module.Module1+AREAControl->Sde_Module.Module1)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module1 = AREAControl->Vm_Module.Module1;
				AREAControl->Sm_Module.Module1 	= AREAControl->Ss_Module.Module1-AREAControl->Sac_Module.Module1-AREAControl->Sde_Module.Module1;
			}
			else if(AREAControl->Stotal_Module.Module1 >= AREAControl->Sac_Module.Module1)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module1	= AREAControl->Vm_Module.Module1;
				AREAControl->Ve_Module.Module1		= sqrtdp( powdp(AREAControl->VmReal_Module.Module1,2) - AREAControl->a_de_Module.Module1*(AREAControl->Stotal_Module.Module1-AREAControl->Sac_Module.Module1) );
				AREAControl->Sm_Module.Module1		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module1 = sqrtdp( powdp(AREAControl->Vs_Module.Module1,2) - AREAControl->a_ac_Module.Module1*AREAControl->Stotal_Module.Module1 );
				AREAControl->Ve_Module.Module1 = AREAControl->VmReal_Module.Module1;
				AREAControl->Sm_Module.Module1		= 0;				
			}

			AREAControl->DeltaT1_Module.Module1 	= divdp( fabs(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1), AREAControl->a_ac_Module.Module1);
			AREAControl->DeltaT2_Module.Module1 	= AREAControl->DeltaT1_Module.Module1;
			if(AREAControl->DeltaT1_Module.Module1 < 0.000001)
			{
				AREAControl->S1_Module.Module1	= 0;
				AREAControl->S2_Module.Module1	= 0;
				AREAControl->Sac_Module.Module1	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module1		= divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->DeltaT1_Module.Module1,3), 
										(6*(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1))) + 
										AREAControl->Vs_Module.Module1*AREAControl->DeltaT1_Module.Module1;
				AREAControl->S2_Module.Module1 	= divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->DeltaT2_Module.Module1,3), 
										(6*(AREAControl->Vs_Module.Module1-AREAControl->VmReal_Module.Module1))) +
										AREAControl->VmReal_Module.Module1*AREAControl->DeltaT2_Module.Module1;
				AREAControl->Sac_Module.Module1	= AREAControl->S1_Module.Module1 + AREAControl->S2_Module.Module1;
			}		
			
			AREAControl->DeltaT4_Module.Module1 	= divdp( fabs(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1), AREAControl->a_de_Module.Module1 );
			AREAControl->DeltaT5_Module.Module1 	= AREAControl->DeltaT4_Module.Module1;
			if(AREAControl->DeltaT4_Module.Module1 < 0.000001)
			{
				AREAControl->S4_Module.Module1 	= 0;
				AREAControl->S5_Module.Module1 	= 0;
				AREAControl->Sde_Module.Module1 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module1 	= divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp(AREAControl->DeltaT4_Module.Module1,3), 
										(6*(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1))) +
										AREAControl->VmReal_Module.Module1*AREAControl->DeltaT4_Module.Module1;
				AREAControl->S5_Module.Module1 	= divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp(AREAControl->DeltaT5_Module.Module1,3), 
										(6*(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1))) +
										AREAControl->Ve_Module.Module1*AREAControl->DeltaT5_Module.Module1;
				AREAControl->Sde_Module.Module1 	= AREAControl->S4_Module.Module1+AREAControl->S5_Module.Module1;
			}
					
			AREAControl->S3_Module.Module1 		= AREAControl->Stotal_Module.Module1-AREAControl->S1_Module.Module1-AREAControl->S2_Module.Module1-
										AREAControl->S4_Module.Module1-AREAControl->S5_Module.Module1;
			
			if(AREAControl->VmReal_Module.Module1 < 0.000001) 
				AREAControl->DeltaT3_Module.Module1 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module1 > 0)
				{
					AREAControl->DeltaT3_Module.Module1 = divdp(AREAControl->S3_Module.Module1, AREAControl->VmReal_Module.Module1);
				}
				else
				{
					AREAControl->S3_Module.Module1 = 0;
					AREAControl->DeltaT3_Module.Module1 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module1	= AREAControl->S1_Module.Module1+AREAControl->S2_Module.Module1;
		AREAControl->Sde_Module.Module1	= AREAControl->S4_Module.Module1+AREAControl->S5_Module.Module1;
		AREAControl->Sm_Module.Module1	= AREAControl->S3_Module.Module1;

		AREAControl->T1_Module.Module1 	= AREAControl->DeltaT1_Module.Module1;
		AREAControl->T2_Module.Module1 	= AREAControl->T1_Module.Module1+AREAControl->DeltaT2_Module.Module1;
		AREAControl->T3_Module.Module1 	= AREAControl->T2_Module.Module1+AREAControl->DeltaT3_Module.Module1;
		AREAControl->T4_Module.Module1 	= AREAControl->T3_Module.Module1+AREAControl->DeltaT4_Module.Module1;
		AREAControl->T5_Module.Module1 	= AREAControl->T4_Module.Module1+AREAControl->DeltaT5_Module.Module1;
		AREAControl->TotalN_Module.Module1 = divdp(AREAControl->T5_Module.Module1, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 2)
	{
		if( AREAControl->SpeedState.Module2 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module2 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module2,2) - powdp(AREAControl->Vs_Module.Module2,2)), AREAControl->a_ac_Module.Module2) );
			AREAControl->Sde_Module.Module2 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module2,2) - powdp(AREAControl->Ve_Module.Module2,2)), AREAControl->a_de_Module.Module2) );

			if(AREAControl->Stotal_Module.Module2 >= (AREAControl->Sac_Module.Module2+AREAControl->Sde_Module.Module2)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module2 = AREAControl->Vm_Module.Module2;
				AREAControl->Sm_Module.Module2 	= AREAControl->Ss_Module.Module2-AREAControl->Sac_Module.Module2-AREAControl->Sde_Module.Module2;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module2 >= AREAControl->Vs_Module.Module2 ) ? (AREAControl->a_ac_Module.Module2):(AREAControl->a_de_Module.Module2);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module2, 2 ) - powdp( AREAControl->Vs_Module.Module2, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module2) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module2, 2 ) - powdp( AREAControl->Vs_Module.Module2, 2 ) ), AREAControl->Stotal_Module.Module2 );

					if( AREAControl->Ve_Module.Module2 >= AREAControl->Vs_Module.Module2 )
					{
						AREAControl->VmReal_Module.Module2 = AREAControl->Ve_Module.Module2;
						AREAControl->a_ac_Module.Module2 = divdp( fabs( powdp( AREAControl->Ve_Module.Module2, 2 ) - powdp( AREAControl->Vs_Module.Module2, 2 ) ), AREAControl->Stotal_Module.Module2 );
					}
					else
					{
						AREAControl->VmReal_Module.Module2 = AREAControl->Vs_Module.Module2;

						a_de_sd = AREAControl->a_de_Module.Module2;
						AREAControl->a_de_Module.Module2 = divdp( fabs( powdp( AREAControl->Ve_Module.Module2, 2 ) - powdp( AREAControl->Vs_Module.Module2, 2 ) ), AREAControl->Stotal_Module.Module2 );
						if(AREAControl->a_de_Module.Module2 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module2 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module2 = sqrtdp( powdp(AREAControl->Vs_Module.Module2,2) - AREAControl->a_de_Module.Module2*AREAControl->Stotal_Module.Module2 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module2 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module2 * AREAControl->a_de_Module.Module2 * AREAControl->Stotal_Module.Module2 + 
												AREAControl->a_de_Module.Module2 * powdp( AREAControl->Vs_Module.Module2, 2 ) + AREAControl->a_ac_Module.Module2 * 
												powdp( AREAControl->Ve_Module.Module2, 2 ) ),  ( AREAControl->a_ac_Module.Module2 + AREAControl->a_de_Module.Module2 ) ) );
					AREAControl->Sm_Module.Module2		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module2 	= divdp( fabs(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2), AREAControl->a_ac_Module.Module2);
			AREAControl->DeltaT2_Module.Module2 	= AREAControl->DeltaT1_Module.Module2;
			if(AREAControl->DeltaT1_Module.Module2 == 0)
			{
				AREAControl->S1_Module.Module2		= 0;
				AREAControl->S2_Module.Module2		= 0;
				AREAControl->Sac_Module.Module2	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module2		= divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->DeltaT1_Module.Module2,3), 
										(6*(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2))) + 
										AREAControl->Vs_Module.Module2*AREAControl->DeltaT1_Module.Module2;
				AREAControl->S2_Module.Module2 	= divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->DeltaT2_Module.Module2,3), 
										(6*(AREAControl->Vs_Module.Module2-AREAControl->VmReal_Module.Module2))) +
										AREAControl->VmReal_Module.Module2*AREAControl->DeltaT2_Module.Module2;
				AREAControl->Sac_Module.Module2	= AREAControl->S1_Module.Module2 + AREAControl->S2_Module.Module2;
			}		
			
			AREAControl->DeltaT4_Module.Module2 	= divdp( fabs(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2), AREAControl->a_de_Module.Module2 );
			AREAControl->DeltaT5_Module.Module2 	= AREAControl->DeltaT4_Module.Module2;
			if(AREAControl->DeltaT4_Module.Module2 == 0)
			{
				AREAControl->S4_Module.Module2 	= 0;
				AREAControl->S5_Module.Module2 	= 0;
				AREAControl->Sde_Module.Module2 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module2 	= divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp(AREAControl->DeltaT4_Module.Module2,3), 
										(6*(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2))) +
										AREAControl->VmReal_Module.Module2*AREAControl->DeltaT4_Module.Module2;
				AREAControl->S5_Module.Module2 	= divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp(AREAControl->DeltaT5_Module.Module2,3), 
										(6*(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2))) +
										AREAControl->Ve_Module.Module2*AREAControl->DeltaT5_Module.Module2;
				AREAControl->Sde_Module.Module2 	= AREAControl->S4_Module.Module2+AREAControl->S5_Module.Module2;
			}
					
			AREAControl->S3_Module.Module2 		= AREAControl->Stotal_Module.Module2-AREAControl->S1_Module.Module2-AREAControl->S2_Module.Module2-
										AREAControl->S4_Module.Module2-AREAControl->S5_Module.Module2;
			
			if(AREAControl->VmReal_Module.Module2==0) 
				AREAControl->DeltaT3_Module.Module2 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module2 > 0)
				{
					AREAControl->DeltaT3_Module.Module2 = divdp(AREAControl->S3_Module.Module2, AREAControl->VmReal_Module.Module2);
				}
				else
				{
					AREAControl->S3_Module.Module2 = 0;
					AREAControl->DeltaT3_Module.Module2 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module2 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module2 = AREAControl->a_de_Module.Module2;
			AREAControl->Sac_Module.Module2 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module2,2) - powdp(AREAControl->Vs_Module.Module2,2)), AREAControl->a_ac_Module.Module2) );
			AREAControl->Sde_Module.Module2 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module2,2) - powdp(AREAControl->Ve_Module.Module2,2)), AREAControl->a_de_Module.Module2) );

			if(AREAControl->Stotal_Module.Module2 >= (AREAControl->Sac_Module.Module2+AREAControl->Sde_Module.Module2)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module2 = AREAControl->Vm_Module.Module2;
				AREAControl->Sm_Module.Module2 	= AREAControl->Ss_Module.Module2-AREAControl->Sac_Module.Module2-AREAControl->Sde_Module.Module2;
			}
			else if(AREAControl->Stotal_Module.Module2 >= AREAControl->Sac_Module.Module2)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module2	= AREAControl->Vm_Module.Module2;
				AREAControl->Ve_Module.Module2		= sqrtdp( powdp(AREAControl->VmReal_Module.Module2,2) - AREAControl->a_de_Module.Module2*(AREAControl->Stotal_Module.Module2-AREAControl->Sac_Module.Module2) );
				AREAControl->Sm_Module.Module2		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module2 = sqrtdp( powdp(AREAControl->Vs_Module.Module2,2) - AREAControl->a_ac_Module.Module2*AREAControl->Stotal_Module.Module2 );
				AREAControl->Ve_Module.Module2 = AREAControl->VmReal_Module.Module2;
				AREAControl->Sm_Module.Module2		= 0;				
			}

			AREAControl->DeltaT1_Module.Module2 	= divdp( fabs(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2), AREAControl->a_ac_Module.Module2);
			AREAControl->DeltaT2_Module.Module2 	= AREAControl->DeltaT1_Module.Module2;
			if(AREAControl->DeltaT1_Module.Module2 < 0.000001)
			{
				AREAControl->S1_Module.Module2	= 0;
				AREAControl->S2_Module.Module2	= 0;
				AREAControl->Sac_Module.Module2	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module2		= divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->DeltaT1_Module.Module2,3), 
										(6*(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2))) + 
										AREAControl->Vs_Module.Module2*AREAControl->DeltaT1_Module.Module2;
				AREAControl->S2_Module.Module2 	= divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->DeltaT2_Module.Module2,3), 
										(6*(AREAControl->Vs_Module.Module2-AREAControl->VmReal_Module.Module2))) +
										AREAControl->VmReal_Module.Module2*AREAControl->DeltaT2_Module.Module2;
				AREAControl->Sac_Module.Module2	= AREAControl->S1_Module.Module2 + AREAControl->S2_Module.Module2;
			}		
			
			AREAControl->DeltaT4_Module.Module2 	= divdp( fabs(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2), AREAControl->a_de_Module.Module2 );
			AREAControl->DeltaT5_Module.Module2 	= AREAControl->DeltaT4_Module.Module2;
			if(AREAControl->DeltaT4_Module.Module2 < 0.000001)
			{
				AREAControl->S4_Module.Module2 	= 0;
				AREAControl->S5_Module.Module2 	= 0;
				AREAControl->Sde_Module.Module2 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module2 	= divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp(AREAControl->DeltaT4_Module.Module2,3), 
										(6*(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2))) +
										AREAControl->VmReal_Module.Module2*AREAControl->DeltaT4_Module.Module2;
				AREAControl->S5_Module.Module2 	= divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp(AREAControl->DeltaT5_Module.Module2,3), 
										(6*(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2))) +
										AREAControl->Ve_Module.Module2*AREAControl->DeltaT5_Module.Module2;
				AREAControl->Sde_Module.Module2 	= AREAControl->S4_Module.Module2+AREAControl->S5_Module.Module2;
			}
					
			AREAControl->S3_Module.Module2 		= AREAControl->Stotal_Module.Module2-AREAControl->S1_Module.Module2-AREAControl->S2_Module.Module2-
										AREAControl->S4_Module.Module2-AREAControl->S5_Module.Module2;
			
			if(AREAControl->VmReal_Module.Module2 < 0.000001) 
				AREAControl->DeltaT3_Module.Module2 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module2 > 0)
				{
					AREAControl->DeltaT3_Module.Module2 = divdp(AREAControl->S3_Module.Module2, AREAControl->VmReal_Module.Module2);
				}
				else
				{
					AREAControl->S3_Module.Module2 = 0;
					AREAControl->DeltaT3_Module.Module2 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module2	= AREAControl->S1_Module.Module2+AREAControl->S2_Module.Module2;
		AREAControl->Sde_Module.Module2	= AREAControl->S4_Module.Module2+AREAControl->S5_Module.Module2;
		AREAControl->Sm_Module.Module2	= AREAControl->S3_Module.Module2;

		AREAControl->T1_Module.Module2 	= AREAControl->DeltaT1_Module.Module2;
		AREAControl->T2_Module.Module2 	= AREAControl->T1_Module.Module2+AREAControl->DeltaT2_Module.Module2;
		AREAControl->T3_Module.Module2 	= AREAControl->T2_Module.Module2+AREAControl->DeltaT3_Module.Module2;
		AREAControl->T4_Module.Module2 	= AREAControl->T3_Module.Module2+AREAControl->DeltaT4_Module.Module2;
		AREAControl->T5_Module.Module2 	= AREAControl->T4_Module.Module2+AREAControl->DeltaT5_Module.Module2;
		AREAControl->TotalN_Module.Module2 = divdp(AREAControl->T5_Module.Module2, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 3)
	{
		if( AREAControl->SpeedState.Module3 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module3 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module3,2) - powdp(AREAControl->Vs_Module.Module3,2)), AREAControl->a_ac_Module.Module3) );
			AREAControl->Sde_Module.Module3 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module3,2) - powdp(AREAControl->Ve_Module.Module3,2)), AREAControl->a_de_Module.Module3) );

			if(AREAControl->Stotal_Module.Module3 >= (AREAControl->Sac_Module.Module3+AREAControl->Sde_Module.Module3)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module3 = AREAControl->Vm_Module.Module3;
				AREAControl->Sm_Module.Module3 	= AREAControl->Ss_Module.Module3-AREAControl->Sac_Module.Module3-AREAControl->Sde_Module.Module3;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module3 >= AREAControl->Vs_Module.Module3 ) ? (AREAControl->a_ac_Module.Module3):(AREAControl->a_de_Module.Module3);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module3, 2 ) - powdp( AREAControl->Vs_Module.Module3, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module3) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module3, 2 ) - powdp( AREAControl->Vs_Module.Module3, 2 ) ), AREAControl->Stotal_Module.Module3 );

					if( AREAControl->Ve_Module.Module3 >= AREAControl->Vs_Module.Module3 )
					{
						AREAControl->VmReal_Module.Module3 = AREAControl->Ve_Module.Module3;
						AREAControl->a_ac_Module.Module3 = divdp( fabs( powdp( AREAControl->Ve_Module.Module3, 2 ) - powdp( AREAControl->Vs_Module.Module3, 2 ) ), AREAControl->Stotal_Module.Module3 );
					}
					else
					{
						AREAControl->VmReal_Module.Module3 = AREAControl->Vs_Module.Module3;

						a_de_sd = AREAControl->a_de_Module.Module3;
						AREAControl->a_de_Module.Module3 = divdp( fabs( powdp( AREAControl->Ve_Module.Module3, 2 ) - powdp( AREAControl->Vs_Module.Module3, 2 ) ), AREAControl->Stotal_Module.Module3 );
						if(AREAControl->a_de_Module.Module3 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module3 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module3 = sqrtdp( powdp(AREAControl->Vs_Module.Module3,2) - AREAControl->a_de_Module.Module3*AREAControl->Stotal_Module.Module3 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module3 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module3 * AREAControl->a_de_Module.Module3 * AREAControl->Stotal_Module.Module3 + 
												AREAControl->a_de_Module.Module3 * powdp( AREAControl->Vs_Module.Module3, 2 ) + AREAControl->a_ac_Module.Module3 * 
												powdp( AREAControl->Ve_Module.Module3, 2 ) ),  ( AREAControl->a_ac_Module.Module3 + AREAControl->a_de_Module.Module3 ) ) );
					AREAControl->Sm_Module.Module3		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module3 	= divdp( fabs(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3), AREAControl->a_ac_Module.Module3);
			AREAControl->DeltaT2_Module.Module3 	= AREAControl->DeltaT1_Module.Module3;
			if(AREAControl->DeltaT1_Module.Module3 == 0)
			{
				AREAControl->S1_Module.Module3		= 0;
				AREAControl->S2_Module.Module3		= 0;
				AREAControl->Sac_Module.Module3	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module3		= divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->DeltaT1_Module.Module3,3), 
										(6*(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3))) + 
										AREAControl->Vs_Module.Module3*AREAControl->DeltaT1_Module.Module3;
				AREAControl->S2_Module.Module3 	= divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->DeltaT2_Module.Module3,3), 
										(6*(AREAControl->Vs_Module.Module3-AREAControl->VmReal_Module.Module3))) +
										AREAControl->VmReal_Module.Module3*AREAControl->DeltaT2_Module.Module3;
				AREAControl->Sac_Module.Module3	= AREAControl->S1_Module.Module3 + AREAControl->S2_Module.Module3;
			}		
			
			AREAControl->DeltaT4_Module.Module3 	= divdp( fabs(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3), AREAControl->a_de_Module.Module3 );
			AREAControl->DeltaT5_Module.Module3 	= AREAControl->DeltaT4_Module.Module3;
			if(AREAControl->DeltaT4_Module.Module3 == 0)
			{
				AREAControl->S4_Module.Module3 	= 0;
				AREAControl->S5_Module.Module3 	= 0;
				AREAControl->Sde_Module.Module3 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module3 	= divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp(AREAControl->DeltaT4_Module.Module3,3), 
										(6*(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3))) +
										AREAControl->VmReal_Module.Module3*AREAControl->DeltaT4_Module.Module3;
				AREAControl->S5_Module.Module3 	= divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp(AREAControl->DeltaT5_Module.Module3,3), 
										(6*(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3))) +
										AREAControl->Ve_Module.Module3*AREAControl->DeltaT5_Module.Module3;
				AREAControl->Sde_Module.Module3 	= AREAControl->S4_Module.Module3+AREAControl->S5_Module.Module3;
			}
					
			AREAControl->S3_Module.Module3 		= AREAControl->Stotal_Module.Module3-AREAControl->S1_Module.Module3-AREAControl->S2_Module.Module3-
										AREAControl->S4_Module.Module3-AREAControl->S5_Module.Module3;
			
			if(AREAControl->VmReal_Module.Module3==0) 
				AREAControl->DeltaT3_Module.Module3 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module3 > 0)
				{
					AREAControl->DeltaT3_Module.Module3 = divdp(AREAControl->S3_Module.Module3, AREAControl->VmReal_Module.Module3);
				}
				else
				{
					AREAControl->S3_Module.Module3 = 0;
					AREAControl->DeltaT3_Module.Module3 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module3 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module3 = AREAControl->a_de_Module.Module3;
			AREAControl->Sac_Module.Module3 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module3,2) - powdp(AREAControl->Vs_Module.Module3,2)), AREAControl->a_ac_Module.Module3) );
			AREAControl->Sde_Module.Module3 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module3,2) - powdp(AREAControl->Ve_Module.Module3,2)), AREAControl->a_de_Module.Module3) );

			if(AREAControl->Stotal_Module.Module3 >= (AREAControl->Sac_Module.Module3+AREAControl->Sde_Module.Module3)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module3 = AREAControl->Vm_Module.Module3;
				AREAControl->Sm_Module.Module3 	= AREAControl->Ss_Module.Module3-AREAControl->Sac_Module.Module3-AREAControl->Sde_Module.Module3;
			}
			else if(AREAControl->Stotal_Module.Module3 >= AREAControl->Sac_Module.Module3)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module3	= AREAControl->Vm_Module.Module3;
				AREAControl->Ve_Module.Module3		= sqrtdp( powdp(AREAControl->VmReal_Module.Module3,2) - AREAControl->a_de_Module.Module3*(AREAControl->Stotal_Module.Module3-AREAControl->Sac_Module.Module3) );
				AREAControl->Sm_Module.Module3		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module3 = sqrtdp( powdp(AREAControl->Vs_Module.Module3,2) - AREAControl->a_ac_Module.Module3*AREAControl->Stotal_Module.Module3 );
				AREAControl->Ve_Module.Module3 = AREAControl->VmReal_Module.Module3;
				AREAControl->Sm_Module.Module3		= 0;				
			}

			AREAControl->DeltaT1_Module.Module3 	= divdp( fabs(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3), AREAControl->a_ac_Module.Module3);
			AREAControl->DeltaT2_Module.Module3 	= AREAControl->DeltaT1_Module.Module3;
			if(AREAControl->DeltaT1_Module.Module3 < 0.000001)
			{
				AREAControl->S1_Module.Module3	= 0;
				AREAControl->S2_Module.Module3	= 0;
				AREAControl->Sac_Module.Module3	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module3		= divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->DeltaT1_Module.Module3,3), 
										(6*(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3))) + 
										AREAControl->Vs_Module.Module3*AREAControl->DeltaT1_Module.Module3;
				AREAControl->S2_Module.Module3 	= divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->DeltaT2_Module.Module3,3), 
										(6*(AREAControl->Vs_Module.Module3-AREAControl->VmReal_Module.Module3))) +
										AREAControl->VmReal_Module.Module3*AREAControl->DeltaT2_Module.Module3;
				AREAControl->Sac_Module.Module3	= AREAControl->S1_Module.Module3 + AREAControl->S2_Module.Module3;
			}		
			
			AREAControl->DeltaT4_Module.Module3 	= divdp( fabs(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3), AREAControl->a_de_Module.Module3 );
			AREAControl->DeltaT5_Module.Module3 	= AREAControl->DeltaT4_Module.Module3;
			if(AREAControl->DeltaT4_Module.Module3 < 0.000001)
			{
				AREAControl->S4_Module.Module3 	= 0;
				AREAControl->S5_Module.Module3 	= 0;
				AREAControl->Sde_Module.Module3 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module3 	= divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp(AREAControl->DeltaT4_Module.Module3,3), 
										(6*(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3))) +
										AREAControl->VmReal_Module.Module3*AREAControl->DeltaT4_Module.Module3;
				AREAControl->S5_Module.Module3 	= divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp(AREAControl->DeltaT5_Module.Module3,3), 
										(6*(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3))) +
										AREAControl->Ve_Module.Module3*AREAControl->DeltaT5_Module.Module3;
				AREAControl->Sde_Module.Module3 	= AREAControl->S4_Module.Module3+AREAControl->S5_Module.Module3;
			}
					
			AREAControl->S3_Module.Module3 		= AREAControl->Stotal_Module.Module3-AREAControl->S1_Module.Module3-AREAControl->S2_Module.Module3-
										AREAControl->S4_Module.Module3-AREAControl->S5_Module.Module3;
			
			if(AREAControl->VmReal_Module.Module3 < 0.000001) 
				AREAControl->DeltaT3_Module.Module3 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module3 > 0)
				{
					AREAControl->DeltaT3_Module.Module3 = divdp(AREAControl->S3_Module.Module3, AREAControl->VmReal_Module.Module3);
				}
				else
				{
					AREAControl->S3_Module.Module3 = 0;
					AREAControl->DeltaT3_Module.Module3 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module3	= AREAControl->S1_Module.Module3+AREAControl->S2_Module.Module3;
		AREAControl->Sde_Module.Module3	= AREAControl->S4_Module.Module3+AREAControl->S5_Module.Module3;
		AREAControl->Sm_Module.Module3	= AREAControl->S3_Module.Module3;

		AREAControl->T1_Module.Module3 	= AREAControl->DeltaT1_Module.Module3;
		AREAControl->T2_Module.Module3 	= AREAControl->T1_Module.Module3+AREAControl->DeltaT2_Module.Module3;
		AREAControl->T3_Module.Module3 	= AREAControl->T2_Module.Module3+AREAControl->DeltaT3_Module.Module3;
		AREAControl->T4_Module.Module3 	= AREAControl->T3_Module.Module3+AREAControl->DeltaT4_Module.Module3;
		AREAControl->T5_Module.Module3 	= AREAControl->T4_Module.Module3+AREAControl->DeltaT5_Module.Module3;
		AREAControl->TotalN_Module.Module3 = divdp(AREAControl->T5_Module.Module3, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 4)
	{
		if( AREAControl->SpeedState.Module4 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module4 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module4,2) - powdp(AREAControl->Vs_Module.Module4,2)), AREAControl->a_ac_Module.Module4) );
			AREAControl->Sde_Module.Module4 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module4,2) - powdp(AREAControl->Ve_Module.Module4,2)), AREAControl->a_de_Module.Module4) );

			if(AREAControl->Stotal_Module.Module4 >= (AREAControl->Sac_Module.Module4+AREAControl->Sde_Module.Module4)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module4 = AREAControl->Vm_Module.Module4;
				AREAControl->Sm_Module.Module4 	= AREAControl->Ss_Module.Module4-AREAControl->Sac_Module.Module4-AREAControl->Sde_Module.Module4;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module4 >= AREAControl->Vs_Module.Module4 ) ? (AREAControl->a_ac_Module.Module4):(AREAControl->a_de_Module.Module4);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module4, 2 ) - powdp( AREAControl->Vs_Module.Module4, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module4) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module4, 2 ) - powdp( AREAControl->Vs_Module.Module4, 2 ) ), AREAControl->Stotal_Module.Module4 );

					if( AREAControl->Ve_Module.Module4 >= AREAControl->Vs_Module.Module4 )
					{
						AREAControl->VmReal_Module.Module4 = AREAControl->Ve_Module.Module4;
						AREAControl->a_ac_Module.Module4 = divdp( fabs( powdp( AREAControl->Ve_Module.Module4, 2 ) - powdp( AREAControl->Vs_Module.Module4, 2 ) ), AREAControl->Stotal_Module.Module4 );
					}
					else
					{
						AREAControl->VmReal_Module.Module4 = AREAControl->Vs_Module.Module4;

						a_de_sd = AREAControl->a_de_Module.Module4;
						AREAControl->a_de_Module.Module4 = divdp( fabs( powdp( AREAControl->Ve_Module.Module4, 2 ) - powdp( AREAControl->Vs_Module.Module4, 2 ) ), AREAControl->Stotal_Module.Module4 );
						if(AREAControl->a_de_Module.Module4 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module4 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module4 = sqrtdp( powdp(AREAControl->Vs_Module.Module4,2) - AREAControl->a_de_Module.Module4*AREAControl->Stotal_Module.Module4 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module4 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module4 * AREAControl->a_de_Module.Module4 * AREAControl->Stotal_Module.Module4 + 
												AREAControl->a_de_Module.Module4 * powdp( AREAControl->Vs_Module.Module4, 2 ) + AREAControl->a_ac_Module.Module4 * 
												powdp( AREAControl->Ve_Module.Module4, 2 ) ),  ( AREAControl->a_ac_Module.Module4 + AREAControl->a_de_Module.Module4 ) ) );
					AREAControl->Sm_Module.Module4		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module4 	= divdp( fabs(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4), AREAControl->a_ac_Module.Module4);
			AREAControl->DeltaT2_Module.Module4 	= AREAControl->DeltaT1_Module.Module4;
			if(AREAControl->DeltaT1_Module.Module4 == 0)
			{
				AREAControl->S1_Module.Module4		= 0;
				AREAControl->S2_Module.Module4		= 0;
				AREAControl->Sac_Module.Module4	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module4		= divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->DeltaT1_Module.Module4,3), 
										(6*(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4))) + 
										AREAControl->Vs_Module.Module4*AREAControl->DeltaT1_Module.Module4;
				AREAControl->S2_Module.Module4 	= divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->DeltaT2_Module.Module4,3), 
										(6*(AREAControl->Vs_Module.Module4-AREAControl->VmReal_Module.Module4))) +
										AREAControl->VmReal_Module.Module4*AREAControl->DeltaT2_Module.Module4;
				AREAControl->Sac_Module.Module4	= AREAControl->S1_Module.Module4 + AREAControl->S2_Module.Module4;
			}		
			
			AREAControl->DeltaT4_Module.Module4 	= divdp( fabs(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4), AREAControl->a_de_Module.Module4 );
			AREAControl->DeltaT5_Module.Module4 	= AREAControl->DeltaT4_Module.Module4;
			if(AREAControl->DeltaT4_Module.Module4 == 0)
			{
				AREAControl->S4_Module.Module4 	= 0;
				AREAControl->S5_Module.Module4 	= 0;
				AREAControl->Sde_Module.Module4 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module4 	= divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp(AREAControl->DeltaT4_Module.Module4,3), 
										(6*(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4))) +
										AREAControl->VmReal_Module.Module4*AREAControl->DeltaT4_Module.Module4;
				AREAControl->S5_Module.Module4 	= divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp(AREAControl->DeltaT5_Module.Module4,3), 
										(6*(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4))) +
										AREAControl->Ve_Module.Module4*AREAControl->DeltaT5_Module.Module4;
				AREAControl->Sde_Module.Module4 	= AREAControl->S4_Module.Module4+AREAControl->S5_Module.Module4;
			}
					
			AREAControl->S3_Module.Module4 		= AREAControl->Stotal_Module.Module4-AREAControl->S1_Module.Module4-AREAControl->S2_Module.Module4-
										AREAControl->S4_Module.Module4-AREAControl->S5_Module.Module4;
			
			if(AREAControl->VmReal_Module.Module4==0) 
				AREAControl->DeltaT3_Module.Module4 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module4 > 0)
				{
					AREAControl->DeltaT3_Module.Module4 = divdp(AREAControl->S3_Module.Module4, AREAControl->VmReal_Module.Module4);
				}
				else
				{
					AREAControl->S3_Module.Module4 = 0;
					AREAControl->DeltaT3_Module.Module4 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module4 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module4 = AREAControl->a_de_Module.Module4;
			AREAControl->Sac_Module.Module4 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module4,2) - powdp(AREAControl->Vs_Module.Module4,2)), AREAControl->a_ac_Module.Module4) );
			AREAControl->Sde_Module.Module4 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module4,2) - powdp(AREAControl->Ve_Module.Module4,2)), AREAControl->a_de_Module.Module4) );

			if(AREAControl->Stotal_Module.Module4 >= (AREAControl->Sac_Module.Module4+AREAControl->Sde_Module.Module4)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module4 = AREAControl->Vm_Module.Module4;
				AREAControl->Sm_Module.Module4 	= AREAControl->Ss_Module.Module4-AREAControl->Sac_Module.Module4-AREAControl->Sde_Module.Module4;
			}
			else if(AREAControl->Stotal_Module.Module4 >= AREAControl->Sac_Module.Module4)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module4	= AREAControl->Vm_Module.Module4;
				AREAControl->Ve_Module.Module4		= sqrtdp( powdp(AREAControl->VmReal_Module.Module4,2) - AREAControl->a_de_Module.Module4*(AREAControl->Stotal_Module.Module4-AREAControl->Sac_Module.Module4) );
				AREAControl->Sm_Module.Module4		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module4 = sqrtdp( powdp(AREAControl->Vs_Module.Module4,2) - AREAControl->a_ac_Module.Module4*AREAControl->Stotal_Module.Module4 );
				AREAControl->Ve_Module.Module4 = AREAControl->VmReal_Module.Module4;
				AREAControl->Sm_Module.Module4		= 0;				
			}

			AREAControl->DeltaT1_Module.Module4 	= divdp( fabs(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4), AREAControl->a_ac_Module.Module4);
			AREAControl->DeltaT2_Module.Module4 	= AREAControl->DeltaT1_Module.Module4;
			if(AREAControl->DeltaT1_Module.Module4 < 0.000001)
			{
				AREAControl->S1_Module.Module4	= 0;
				AREAControl->S2_Module.Module4	= 0;
				AREAControl->Sac_Module.Module4	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module4		= divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->DeltaT1_Module.Module4,3), 
										(6*(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4))) + 
										AREAControl->Vs_Module.Module4*AREAControl->DeltaT1_Module.Module4;
				AREAControl->S2_Module.Module4 	= divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->DeltaT2_Module.Module4,3), 
										(6*(AREAControl->Vs_Module.Module4-AREAControl->VmReal_Module.Module4))) +
										AREAControl->VmReal_Module.Module4*AREAControl->DeltaT2_Module.Module4;
				AREAControl->Sac_Module.Module4	= AREAControl->S1_Module.Module4 + AREAControl->S2_Module.Module4;
			}		
			
			AREAControl->DeltaT4_Module.Module4 	= divdp( fabs(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4), AREAControl->a_de_Module.Module4 );
			AREAControl->DeltaT5_Module.Module4 	= AREAControl->DeltaT4_Module.Module4;
			if(AREAControl->DeltaT4_Module.Module4 < 0.000001)
			{
				AREAControl->S4_Module.Module4 	= 0;
				AREAControl->S5_Module.Module4 	= 0;
				AREAControl->Sde_Module.Module4 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module4 	= divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp(AREAControl->DeltaT4_Module.Module4,3), 
										(6*(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4))) +
										AREAControl->VmReal_Module.Module4*AREAControl->DeltaT4_Module.Module4;
				AREAControl->S5_Module.Module4 	= divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp(AREAControl->DeltaT5_Module.Module4,3), 
										(6*(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4))) +
										AREAControl->Ve_Module.Module4*AREAControl->DeltaT5_Module.Module4;
				AREAControl->Sde_Module.Module4 	= AREAControl->S4_Module.Module4+AREAControl->S5_Module.Module4;
			}
					
			AREAControl->S3_Module.Module4 		= AREAControl->Stotal_Module.Module4-AREAControl->S1_Module.Module4-AREAControl->S2_Module.Module4-
										AREAControl->S4_Module.Module4-AREAControl->S5_Module.Module4;
			
			if(AREAControl->VmReal_Module.Module4 < 0.000001) 
				AREAControl->DeltaT3_Module.Module4 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module4 > 0)
				{
					AREAControl->DeltaT3_Module.Module4 = divdp(AREAControl->S3_Module.Module4, AREAControl->VmReal_Module.Module4);
				}
				else
				{
					AREAControl->S3_Module.Module4 = 0;
					AREAControl->DeltaT3_Module.Module4 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module4	= AREAControl->S1_Module.Module4+AREAControl->S2_Module.Module4;
		AREAControl->Sde_Module.Module4	= AREAControl->S4_Module.Module4+AREAControl->S5_Module.Module4;
		AREAControl->Sm_Module.Module4	= AREAControl->S3_Module.Module4;

		AREAControl->T1_Module.Module4 	= AREAControl->DeltaT1_Module.Module4;
		AREAControl->T2_Module.Module4 	= AREAControl->T1_Module.Module4+AREAControl->DeltaT2_Module.Module4;
		AREAControl->T3_Module.Module4 	= AREAControl->T2_Module.Module4+AREAControl->DeltaT3_Module.Module4;
		AREAControl->T4_Module.Module4 	= AREAControl->T3_Module.Module4+AREAControl->DeltaT4_Module.Module4;
		AREAControl->T5_Module.Module4 	= AREAControl->T4_Module.Module4+AREAControl->DeltaT5_Module.Module4;
		AREAControl->TotalN_Module.Module4 = divdp(AREAControl->T5_Module.Module4, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 5)
	{
		if( AREAControl->SpeedState.Module5 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module5 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module5,2) - powdp(AREAControl->Vs_Module.Module5,2)), AREAControl->a_ac_Module.Module5) );
			AREAControl->Sde_Module.Module5 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module5,2) - powdp(AREAControl->Ve_Module.Module5,2)), AREAControl->a_de_Module.Module5) );

			if(AREAControl->Stotal_Module.Module5 >= (AREAControl->Sac_Module.Module5+AREAControl->Sde_Module.Module5)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module5 = AREAControl->Vm_Module.Module5;
				AREAControl->Sm_Module.Module5 	= AREAControl->Ss_Module.Module5-AREAControl->Sac_Module.Module5-AREAControl->Sde_Module.Module5;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module5 >= AREAControl->Vs_Module.Module5 ) ? (AREAControl->a_ac_Module.Module5):(AREAControl->a_de_Module.Module5);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module5, 2 ) - powdp( AREAControl->Vs_Module.Module5, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module5) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module5, 2 ) - powdp( AREAControl->Vs_Module.Module5, 2 ) ), AREAControl->Stotal_Module.Module5 );

					if( AREAControl->Ve_Module.Module5 >= AREAControl->Vs_Module.Module5 )
					{
						AREAControl->VmReal_Module.Module5 = AREAControl->Ve_Module.Module5;
						AREAControl->a_ac_Module.Module5 = divdp( fabs( powdp( AREAControl->Ve_Module.Module5, 2 ) - powdp( AREAControl->Vs_Module.Module5, 2 ) ), AREAControl->Stotal_Module.Module5 );
					}
					else
					{
						AREAControl->VmReal_Module.Module5 = AREAControl->Vs_Module.Module5;

						a_de_sd = AREAControl->a_de_Module.Module5;
						AREAControl->a_de_Module.Module5 = divdp( fabs( powdp( AREAControl->Ve_Module.Module5, 2 ) - powdp( AREAControl->Vs_Module.Module5, 2 ) ), AREAControl->Stotal_Module.Module5 );
						if(AREAControl->a_de_Module.Module5 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module5 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module5 = sqrtdp( powdp(AREAControl->Vs_Module.Module5,2) - AREAControl->a_de_Module.Module5*AREAControl->Stotal_Module.Module5 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module5 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module5 * AREAControl->a_de_Module.Module5 * AREAControl->Stotal_Module.Module5 + 
												AREAControl->a_de_Module.Module5 * powdp( AREAControl->Vs_Module.Module5, 2 ) + AREAControl->a_ac_Module.Module5 * 
												powdp( AREAControl->Ve_Module.Module5, 2 ) ),  ( AREAControl->a_ac_Module.Module5 + AREAControl->a_de_Module.Module5 ) ) );
					AREAControl->Sm_Module.Module5		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module5 	= divdp( fabs(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5), AREAControl->a_ac_Module.Module5);
			AREAControl->DeltaT2_Module.Module5 	= AREAControl->DeltaT1_Module.Module5;
			if(AREAControl->DeltaT1_Module.Module5 == 0)
			{
				AREAControl->S1_Module.Module5		= 0;
				AREAControl->S2_Module.Module5		= 0;
				AREAControl->Sac_Module.Module5	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module5		= divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->DeltaT1_Module.Module5,3), 
										(6*(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5))) + 
										AREAControl->Vs_Module.Module5*AREAControl->DeltaT1_Module.Module5;
				AREAControl->S2_Module.Module5 	= divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->DeltaT2_Module.Module5,3), 
										(6*(AREAControl->Vs_Module.Module5-AREAControl->VmReal_Module.Module5))) +
										AREAControl->VmReal_Module.Module5*AREAControl->DeltaT2_Module.Module5;
				AREAControl->Sac_Module.Module5	= AREAControl->S1_Module.Module5 + AREAControl->S2_Module.Module5;
			}		
			
			AREAControl->DeltaT4_Module.Module5 	= divdp( fabs(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5), AREAControl->a_de_Module.Module5 );
			AREAControl->DeltaT5_Module.Module5 	= AREAControl->DeltaT4_Module.Module5;
			if(AREAControl->DeltaT4_Module.Module5 == 0)
			{
				AREAControl->S4_Module.Module5 	= 0;
				AREAControl->S5_Module.Module5 	= 0;
				AREAControl->Sde_Module.Module5 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module5 	= divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp(AREAControl->DeltaT4_Module.Module5,3), 
										(6*(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5))) +
										AREAControl->VmReal_Module.Module5*AREAControl->DeltaT4_Module.Module5;
				AREAControl->S5_Module.Module5 	= divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp(AREAControl->DeltaT5_Module.Module5,3), 
										(6*(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5))) +
										AREAControl->Ve_Module.Module5*AREAControl->DeltaT5_Module.Module5;
				AREAControl->Sde_Module.Module5 	= AREAControl->S4_Module.Module5+AREAControl->S5_Module.Module5;
			}
					
			AREAControl->S3_Module.Module5 		= AREAControl->Stotal_Module.Module5-AREAControl->S1_Module.Module5-AREAControl->S2_Module.Module5-
										AREAControl->S4_Module.Module5-AREAControl->S5_Module.Module5;
			
			if(AREAControl->VmReal_Module.Module5==0) 
				AREAControl->DeltaT3_Module.Module5 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module5 > 0)
				{
					AREAControl->DeltaT3_Module.Module5 = divdp(AREAControl->S3_Module.Module5, AREAControl->VmReal_Module.Module5);
				}
				else
				{
					AREAControl->S3_Module.Module5 = 0;
					AREAControl->DeltaT3_Module.Module5 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module5 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module5 = AREAControl->a_de_Module.Module5;
			AREAControl->Sac_Module.Module5 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module5,2) - powdp(AREAControl->Vs_Module.Module5,2)), AREAControl->a_ac_Module.Module5) );
			AREAControl->Sde_Module.Module5 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module5,2) - powdp(AREAControl->Ve_Module.Module5,2)), AREAControl->a_de_Module.Module5) );

			if(AREAControl->Stotal_Module.Module5 >= (AREAControl->Sac_Module.Module5+AREAControl->Sde_Module.Module5)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module5 = AREAControl->Vm_Module.Module5;
				AREAControl->Sm_Module.Module5 	= AREAControl->Ss_Module.Module5-AREAControl->Sac_Module.Module5-AREAControl->Sde_Module.Module5;
			}
			else if(AREAControl->Stotal_Module.Module5 >= AREAControl->Sac_Module.Module5)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module5	= AREAControl->Vm_Module.Module5;
				AREAControl->Ve_Module.Module5		= sqrtdp( powdp(AREAControl->VmReal_Module.Module5,2) - AREAControl->a_de_Module.Module5*(AREAControl->Stotal_Module.Module5-AREAControl->Sac_Module.Module5) );
				AREAControl->Sm_Module.Module5		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module5 = sqrtdp( powdp(AREAControl->Vs_Module.Module5,2) - AREAControl->a_ac_Module.Module5*AREAControl->Stotal_Module.Module5 );
				AREAControl->Ve_Module.Module5 = AREAControl->VmReal_Module.Module5;
				AREAControl->Sm_Module.Module5		= 0;				
			}

			AREAControl->DeltaT1_Module.Module5 	= divdp( fabs(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5), AREAControl->a_ac_Module.Module5);
			AREAControl->DeltaT2_Module.Module5 	= AREAControl->DeltaT1_Module.Module5;
			if(AREAControl->DeltaT1_Module.Module5 < 0.000001)
			{
				AREAControl->S1_Module.Module5	= 0;
				AREAControl->S2_Module.Module5	= 0;
				AREAControl->Sac_Module.Module5	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module5		= divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->DeltaT1_Module.Module5,3), 
										(6*(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5))) + 
										AREAControl->Vs_Module.Module5*AREAControl->DeltaT1_Module.Module5;
				AREAControl->S2_Module.Module5 	= divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->DeltaT2_Module.Module5,3), 
										(6*(AREAControl->Vs_Module.Module5-AREAControl->VmReal_Module.Module5))) +
										AREAControl->VmReal_Module.Module5*AREAControl->DeltaT2_Module.Module5;
				AREAControl->Sac_Module.Module5	= AREAControl->S1_Module.Module5 + AREAControl->S2_Module.Module5;
			}		
			
			AREAControl->DeltaT4_Module.Module5 	= divdp( fabs(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5), AREAControl->a_de_Module.Module5 );
			AREAControl->DeltaT5_Module.Module5 	= AREAControl->DeltaT4_Module.Module5;
			if(AREAControl->DeltaT4_Module.Module5 < 0.000001)
			{
				AREAControl->S4_Module.Module5 	= 0;
				AREAControl->S5_Module.Module5 	= 0;
				AREAControl->Sde_Module.Module5 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module5 	= divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp(AREAControl->DeltaT4_Module.Module5,3), 
										(6*(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5))) +
										AREAControl->VmReal_Module.Module5*AREAControl->DeltaT4_Module.Module5;
				AREAControl->S5_Module.Module5 	= divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp(AREAControl->DeltaT5_Module.Module5,3), 
										(6*(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5))) +
										AREAControl->Ve_Module.Module5*AREAControl->DeltaT5_Module.Module5;
				AREAControl->Sde_Module.Module5 	= AREAControl->S4_Module.Module5+AREAControl->S5_Module.Module5;
			}
					
			AREAControl->S3_Module.Module5 		= AREAControl->Stotal_Module.Module5-AREAControl->S1_Module.Module5-AREAControl->S2_Module.Module5-
										AREAControl->S4_Module.Module5-AREAControl->S5_Module.Module5;
			
			if(AREAControl->VmReal_Module.Module5 < 0.000001) 
				AREAControl->DeltaT3_Module.Module5 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module5 > 0)
				{
					AREAControl->DeltaT3_Module.Module5 = divdp(AREAControl->S3_Module.Module5, AREAControl->VmReal_Module.Module5);
				}
				else
				{
					AREAControl->S3_Module.Module5 = 0;
					AREAControl->DeltaT3_Module.Module5 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module5	= AREAControl->S1_Module.Module5+AREAControl->S2_Module.Module5;
		AREAControl->Sde_Module.Module5	= AREAControl->S4_Module.Module5+AREAControl->S5_Module.Module5;
		AREAControl->Sm_Module.Module5	= AREAControl->S3_Module.Module5;

		AREAControl->T1_Module.Module5 	= AREAControl->DeltaT1_Module.Module5;
		AREAControl->T2_Module.Module5 	= AREAControl->T1_Module.Module5+AREAControl->DeltaT2_Module.Module5;
		AREAControl->T3_Module.Module5 	= AREAControl->T2_Module.Module5+AREAControl->DeltaT3_Module.Module5;
		AREAControl->T4_Module.Module5 	= AREAControl->T3_Module.Module5+AREAControl->DeltaT4_Module.Module5;
		AREAControl->T5_Module.Module5 	= AREAControl->T4_Module.Module5+AREAControl->DeltaT5_Module.Module5;
		AREAControl->TotalN_Module.Module5 = divdp(AREAControl->T5_Module.Module5, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 6)
	{
		if( AREAControl->SpeedState.Module6 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module6 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module6,2) - powdp(AREAControl->Vs_Module.Module6,2)), AREAControl->a_ac_Module.Module6) );
			AREAControl->Sde_Module.Module6 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module6,2) - powdp(AREAControl->Ve_Module.Module6,2)), AREAControl->a_de_Module.Module6) );

			if(AREAControl->Stotal_Module.Module6 >= (AREAControl->Sac_Module.Module6+AREAControl->Sde_Module.Module6)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module6 = AREAControl->Vm_Module.Module6;
				AREAControl->Sm_Module.Module6 	= AREAControl->Ss_Module.Module6-AREAControl->Sac_Module.Module6-AREAControl->Sde_Module.Module6;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module6 >= AREAControl->Vs_Module.Module6 ) ? (AREAControl->a_ac_Module.Module6):(AREAControl->a_de_Module.Module6);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module6, 2 ) - powdp( AREAControl->Vs_Module.Module6, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module6) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module6, 2 ) - powdp( AREAControl->Vs_Module.Module6, 2 ) ), AREAControl->Stotal_Module.Module6 );

					if( AREAControl->Ve_Module.Module6 >= AREAControl->Vs_Module.Module6 )
					{
						AREAControl->VmReal_Module.Module6 = AREAControl->Ve_Module.Module6;
						AREAControl->a_ac_Module.Module6 = divdp( fabs( powdp( AREAControl->Ve_Module.Module6, 2 ) - powdp( AREAControl->Vs_Module.Module6, 2 ) ), AREAControl->Stotal_Module.Module6 );
					}
					else
					{
						AREAControl->VmReal_Module.Module6 = AREAControl->Vs_Module.Module6;

						a_de_sd = AREAControl->a_de_Module.Module6;
						AREAControl->a_de_Module.Module6 = divdp( fabs( powdp( AREAControl->Ve_Module.Module6, 2 ) - powdp( AREAControl->Vs_Module.Module6, 2 ) ), AREAControl->Stotal_Module.Module6 );
						if(AREAControl->a_de_Module.Module6 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module6 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module6 = sqrtdp( powdp(AREAControl->Vs_Module.Module6,2) - AREAControl->a_de_Module.Module6*AREAControl->Stotal_Module.Module6 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module6 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module6 * AREAControl->a_de_Module.Module6 * AREAControl->Stotal_Module.Module6 + 
												AREAControl->a_de_Module.Module6 * powdp( AREAControl->Vs_Module.Module6, 2 ) + AREAControl->a_ac_Module.Module6 * 
												powdp( AREAControl->Ve_Module.Module6, 2 ) ),  ( AREAControl->a_ac_Module.Module6 + AREAControl->a_de_Module.Module6 ) ) );
					AREAControl->Sm_Module.Module6		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module6 	= divdp( fabs(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6), AREAControl->a_ac_Module.Module6);
			AREAControl->DeltaT2_Module.Module6 	= AREAControl->DeltaT1_Module.Module6;
			if(AREAControl->DeltaT1_Module.Module6 == 0)
			{
				AREAControl->S1_Module.Module6		= 0;
				AREAControl->S2_Module.Module6		= 0;
				AREAControl->Sac_Module.Module6	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module6		= divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->DeltaT1_Module.Module6,3), 
										(6*(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6))) + 
										AREAControl->Vs_Module.Module6*AREAControl->DeltaT1_Module.Module6;
				AREAControl->S2_Module.Module6 	= divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->DeltaT2_Module.Module6,3), 
										(6*(AREAControl->Vs_Module.Module6-AREAControl->VmReal_Module.Module6))) +
										AREAControl->VmReal_Module.Module6*AREAControl->DeltaT2_Module.Module6;
				AREAControl->Sac_Module.Module6	= AREAControl->S1_Module.Module6 + AREAControl->S2_Module.Module6;
			}		
			
			AREAControl->DeltaT4_Module.Module6 	= divdp( fabs(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6), AREAControl->a_de_Module.Module6 );
			AREAControl->DeltaT5_Module.Module6 	= AREAControl->DeltaT4_Module.Module6;
			if(AREAControl->DeltaT4_Module.Module6 == 0)
			{
				AREAControl->S4_Module.Module6 	= 0;
				AREAControl->S5_Module.Module6 	= 0;
				AREAControl->Sde_Module.Module6 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module6 	= divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp(AREAControl->DeltaT4_Module.Module6,3), 
										(6*(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6))) +
										AREAControl->VmReal_Module.Module6*AREAControl->DeltaT4_Module.Module6;
				AREAControl->S5_Module.Module6 	= divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp(AREAControl->DeltaT5_Module.Module6,3), 
										(6*(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6))) +
										AREAControl->Ve_Module.Module6*AREAControl->DeltaT5_Module.Module6;
				AREAControl->Sde_Module.Module6 	= AREAControl->S4_Module.Module6+AREAControl->S5_Module.Module6;
			}
					
			AREAControl->S3_Module.Module6 		= AREAControl->Stotal_Module.Module6-AREAControl->S1_Module.Module6-AREAControl->S2_Module.Module6-
										AREAControl->S4_Module.Module6-AREAControl->S5_Module.Module6;
			
			if(AREAControl->VmReal_Module.Module6==0) 
				AREAControl->DeltaT3_Module.Module6 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module6 > 0)
				{
					AREAControl->DeltaT3_Module.Module6 = divdp(AREAControl->S3_Module.Module6, AREAControl->VmReal_Module.Module6);
				}
				else
				{
					AREAControl->S3_Module.Module6 = 0;
					AREAControl->DeltaT3_Module.Module6 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module6 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module6 = AREAControl->a_de_Module.Module6;
			AREAControl->Sac_Module.Module6 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module6,2) - powdp(AREAControl->Vs_Module.Module6,2)), AREAControl->a_ac_Module.Module6) );
			AREAControl->Sde_Module.Module6 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module6,2) - powdp(AREAControl->Ve_Module.Module6,2)), AREAControl->a_de_Module.Module6) );

			if(AREAControl->Stotal_Module.Module6 >= (AREAControl->Sac_Module.Module6+AREAControl->Sde_Module.Module6)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module6 = AREAControl->Vm_Module.Module6;
				AREAControl->Sm_Module.Module6 	= AREAControl->Ss_Module.Module6-AREAControl->Sac_Module.Module6-AREAControl->Sde_Module.Module6;
			}
			else if(AREAControl->Stotal_Module.Module6 >= AREAControl->Sac_Module.Module6)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module6	= AREAControl->Vm_Module.Module6;
				AREAControl->Ve_Module.Module6		= sqrtdp( powdp(AREAControl->VmReal_Module.Module6,2) - AREAControl->a_de_Module.Module6*(AREAControl->Stotal_Module.Module6-AREAControl->Sac_Module.Module6) );
				AREAControl->Sm_Module.Module6		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module6 = sqrtdp( powdp(AREAControl->Vs_Module.Module6,2) - AREAControl->a_ac_Module.Module6*AREAControl->Stotal_Module.Module6 );
				AREAControl->Ve_Module.Module6 = AREAControl->VmReal_Module.Module6;
				AREAControl->Sm_Module.Module6		= 0;				
			}

			AREAControl->DeltaT1_Module.Module6 	= divdp( fabs(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6), AREAControl->a_ac_Module.Module6);
			AREAControl->DeltaT2_Module.Module6 	= AREAControl->DeltaT1_Module.Module6;
			if(AREAControl->DeltaT1_Module.Module6 < 0.000001)
			{
				AREAControl->S1_Module.Module6	= 0;
				AREAControl->S2_Module.Module6	= 0;
				AREAControl->Sac_Module.Module6	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module6		= divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->DeltaT1_Module.Module6,3), 
										(6*(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6))) + 
										AREAControl->Vs_Module.Module6*AREAControl->DeltaT1_Module.Module6;
				AREAControl->S2_Module.Module6 	= divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->DeltaT2_Module.Module6,3), 
										(6*(AREAControl->Vs_Module.Module6-AREAControl->VmReal_Module.Module6))) +
										AREAControl->VmReal_Module.Module6*AREAControl->DeltaT2_Module.Module6;
				AREAControl->Sac_Module.Module6	= AREAControl->S1_Module.Module6 + AREAControl->S2_Module.Module6;
			}		
			
			AREAControl->DeltaT4_Module.Module6 	= divdp( fabs(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6), AREAControl->a_de_Module.Module6 );
			AREAControl->DeltaT5_Module.Module6 	= AREAControl->DeltaT4_Module.Module6;
			if(AREAControl->DeltaT4_Module.Module6 < 0.000001)
			{
				AREAControl->S4_Module.Module6 	= 0;
				AREAControl->S5_Module.Module6 	= 0;
				AREAControl->Sde_Module.Module6 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module6 	= divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp(AREAControl->DeltaT4_Module.Module6,3), 
										(6*(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6))) +
										AREAControl->VmReal_Module.Module6*AREAControl->DeltaT4_Module.Module6;
				AREAControl->S5_Module.Module6 	= divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp(AREAControl->DeltaT5_Module.Module6,3), 
										(6*(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6))) +
										AREAControl->Ve_Module.Module6*AREAControl->DeltaT5_Module.Module6;
				AREAControl->Sde_Module.Module6 	= AREAControl->S4_Module.Module6+AREAControl->S5_Module.Module6;
			}
					
			AREAControl->S3_Module.Module6 		= AREAControl->Stotal_Module.Module6-AREAControl->S1_Module.Module6-AREAControl->S2_Module.Module6-
										AREAControl->S4_Module.Module6-AREAControl->S5_Module.Module6;
			
			if(AREAControl->VmReal_Module.Module6 < 0.000001) 
				AREAControl->DeltaT3_Module.Module6 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module6 > 0)
				{
					AREAControl->DeltaT3_Module.Module6 = divdp(AREAControl->S3_Module.Module6, AREAControl->VmReal_Module.Module6);
				}
				else
				{
					AREAControl->S3_Module.Module6 = 0;
					AREAControl->DeltaT3_Module.Module6 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module6	= AREAControl->S1_Module.Module6+AREAControl->S2_Module.Module6;
		AREAControl->Sde_Module.Module6	= AREAControl->S4_Module.Module6+AREAControl->S5_Module.Module6;
		AREAControl->Sm_Module.Module6	= AREAControl->S3_Module.Module6;

		AREAControl->T1_Module.Module6 	= AREAControl->DeltaT1_Module.Module6;
		AREAControl->T2_Module.Module6 	= AREAControl->T1_Module.Module6+AREAControl->DeltaT2_Module.Module6;
		AREAControl->T3_Module.Module6 	= AREAControl->T2_Module.Module6+AREAControl->DeltaT3_Module.Module6;
		AREAControl->T4_Module.Module6 	= AREAControl->T3_Module.Module6+AREAControl->DeltaT4_Module.Module6;
		AREAControl->T5_Module.Module6 	= AREAControl->T4_Module.Module6+AREAControl->DeltaT5_Module.Module6;
		AREAControl->TotalN_Module.Module6 = divdp(AREAControl->T5_Module.Module6, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 7)
	{
		if( AREAControl->SpeedState.Module7 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module7 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module7,2) - powdp(AREAControl->Vs_Module.Module7,2)), AREAControl->a_ac_Module.Module7) );
			AREAControl->Sde_Module.Module7 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module7,2) - powdp(AREAControl->Ve_Module.Module7,2)), AREAControl->a_de_Module.Module7) );

			if(AREAControl->Stotal_Module.Module7 >= (AREAControl->Sac_Module.Module7+AREAControl->Sde_Module.Module7)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module7 = AREAControl->Vm_Module.Module7;
				AREAControl->Sm_Module.Module7 	= AREAControl->Ss_Module.Module7-AREAControl->Sac_Module.Module7-AREAControl->Sde_Module.Module7;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module7 >= AREAControl->Vs_Module.Module7 ) ? (AREAControl->a_ac_Module.Module7):(AREAControl->a_de_Module.Module7);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module7, 2 ) - powdp( AREAControl->Vs_Module.Module7, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module7) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module7, 2 ) - powdp( AREAControl->Vs_Module.Module7, 2 ) ), AREAControl->Stotal_Module.Module7 );

					if( AREAControl->Ve_Module.Module7 >= AREAControl->Vs_Module.Module7 )
					{
						AREAControl->VmReal_Module.Module7 = AREAControl->Ve_Module.Module7;
						AREAControl->a_ac_Module.Module7 = divdp( fabs( powdp( AREAControl->Ve_Module.Module7, 2 ) - powdp( AREAControl->Vs_Module.Module7, 2 ) ), AREAControl->Stotal_Module.Module7 );
					}
					else
					{
						AREAControl->VmReal_Module.Module7 = AREAControl->Vs_Module.Module7;

						a_de_sd = AREAControl->a_de_Module.Module7;
						AREAControl->a_de_Module.Module7 = divdp( fabs( powdp( AREAControl->Ve_Module.Module7, 2 ) - powdp( AREAControl->Vs_Module.Module7, 2 ) ), AREAControl->Stotal_Module.Module7 );
						if(AREAControl->a_de_Module.Module7 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module7 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module7 = sqrtdp( powdp(AREAControl->Vs_Module.Module7,2) - AREAControl->a_de_Module.Module7*AREAControl->Stotal_Module.Module7 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module7 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module7 * AREAControl->a_de_Module.Module7 * AREAControl->Stotal_Module.Module7 + 
												AREAControl->a_de_Module.Module7 * powdp( AREAControl->Vs_Module.Module7, 2 ) + AREAControl->a_ac_Module.Module7 * 
												powdp( AREAControl->Ve_Module.Module7, 2 ) ),  ( AREAControl->a_ac_Module.Module7 + AREAControl->a_de_Module.Module7 ) ) );
					AREAControl->Sm_Module.Module7		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module7 	= divdp( fabs(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7), AREAControl->a_ac_Module.Module7);
			AREAControl->DeltaT2_Module.Module7 	= AREAControl->DeltaT1_Module.Module7;
			if(AREAControl->DeltaT1_Module.Module7 == 0)
			{
				AREAControl->S1_Module.Module7		= 0;
				AREAControl->S2_Module.Module7		= 0;
				AREAControl->Sac_Module.Module7	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module7		= divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->DeltaT1_Module.Module7,3), 
										(6*(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7))) + 
										AREAControl->Vs_Module.Module7*AREAControl->DeltaT1_Module.Module7;
				AREAControl->S2_Module.Module7 	= divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->DeltaT2_Module.Module7,3), 
										(6*(AREAControl->Vs_Module.Module7-AREAControl->VmReal_Module.Module7))) +
										AREAControl->VmReal_Module.Module7*AREAControl->DeltaT2_Module.Module7;
				AREAControl->Sac_Module.Module7	= AREAControl->S1_Module.Module7 + AREAControl->S2_Module.Module7;
			}		
			
			AREAControl->DeltaT4_Module.Module7 	= divdp( fabs(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7), AREAControl->a_de_Module.Module7 );
			AREAControl->DeltaT5_Module.Module7 	= AREAControl->DeltaT4_Module.Module7;
			if(AREAControl->DeltaT4_Module.Module7 == 0)
			{
				AREAControl->S4_Module.Module7 	= 0;
				AREAControl->S5_Module.Module7 	= 0;
				AREAControl->Sde_Module.Module7 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module7 	= divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp(AREAControl->DeltaT4_Module.Module7,3), 
										(6*(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7))) +
										AREAControl->VmReal_Module.Module7*AREAControl->DeltaT4_Module.Module7;
				AREAControl->S5_Module.Module7 	= divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp(AREAControl->DeltaT5_Module.Module7,3), 
										(6*(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7))) +
										AREAControl->Ve_Module.Module7*AREAControl->DeltaT5_Module.Module7;
				AREAControl->Sde_Module.Module7 	= AREAControl->S4_Module.Module7+AREAControl->S5_Module.Module7;
			}
					
			AREAControl->S3_Module.Module7 		= AREAControl->Stotal_Module.Module7-AREAControl->S1_Module.Module7-AREAControl->S2_Module.Module7-
										AREAControl->S4_Module.Module7-AREAControl->S5_Module.Module7;
			
			if(AREAControl->VmReal_Module.Module7==0) 
				AREAControl->DeltaT3_Module.Module7 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module7 > 0)
				{
					AREAControl->DeltaT3_Module.Module7 = divdp(AREAControl->S3_Module.Module7, AREAControl->VmReal_Module.Module7);
				}
				else
				{
					AREAControl->S3_Module.Module7 = 0;
					AREAControl->DeltaT3_Module.Module7 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module7 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module7 = AREAControl->a_de_Module.Module7;
			AREAControl->Sac_Module.Module7 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module7,2) - powdp(AREAControl->Vs_Module.Module7,2)), AREAControl->a_ac_Module.Module7) );
			AREAControl->Sde_Module.Module7 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module7,2) - powdp(AREAControl->Ve_Module.Module7,2)), AREAControl->a_de_Module.Module7) );

			if(AREAControl->Stotal_Module.Module7 >= (AREAControl->Sac_Module.Module7+AREAControl->Sde_Module.Module7)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module7 = AREAControl->Vm_Module.Module7;
				AREAControl->Sm_Module.Module7 	= AREAControl->Ss_Module.Module7-AREAControl->Sac_Module.Module7-AREAControl->Sde_Module.Module7;
			}
			else if(AREAControl->Stotal_Module.Module7 >= AREAControl->Sac_Module.Module7)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module7	= AREAControl->Vm_Module.Module7;
				AREAControl->Ve_Module.Module7		= sqrtdp( powdp(AREAControl->VmReal_Module.Module7,2) - AREAControl->a_de_Module.Module7*(AREAControl->Stotal_Module.Module7-AREAControl->Sac_Module.Module7) );
				AREAControl->Sm_Module.Module7		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module7 = sqrtdp( powdp(AREAControl->Vs_Module.Module7,2) - AREAControl->a_ac_Module.Module7*AREAControl->Stotal_Module.Module7 );
				AREAControl->Ve_Module.Module7 = AREAControl->VmReal_Module.Module7;
				AREAControl->Sm_Module.Module7		= 0;				
			}

			AREAControl->DeltaT1_Module.Module7 	= divdp( fabs(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7), AREAControl->a_ac_Module.Module7);
			AREAControl->DeltaT2_Module.Module7 	= AREAControl->DeltaT1_Module.Module7;
			if(AREAControl->DeltaT1_Module.Module7 < 0.000001)
			{
				AREAControl->S1_Module.Module7	= 0;
				AREAControl->S2_Module.Module7	= 0;
				AREAControl->Sac_Module.Module7	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module7		= divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->DeltaT1_Module.Module7,3), 
										(6*(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7))) + 
										AREAControl->Vs_Module.Module7*AREAControl->DeltaT1_Module.Module7;
				AREAControl->S2_Module.Module7 	= divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->DeltaT2_Module.Module7,3), 
										(6*(AREAControl->Vs_Module.Module7-AREAControl->VmReal_Module.Module7))) +
										AREAControl->VmReal_Module.Module7*AREAControl->DeltaT2_Module.Module7;
				AREAControl->Sac_Module.Module7	= AREAControl->S1_Module.Module7 + AREAControl->S2_Module.Module7;
			}		
			
			AREAControl->DeltaT4_Module.Module7 	= divdp( fabs(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7), AREAControl->a_de_Module.Module7 );
			AREAControl->DeltaT5_Module.Module7 	= AREAControl->DeltaT4_Module.Module7;
			if(AREAControl->DeltaT4_Module.Module7 < 0.000001)
			{
				AREAControl->S4_Module.Module7 	= 0;
				AREAControl->S5_Module.Module7 	= 0;
				AREAControl->Sde_Module.Module7 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module7 	= divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp(AREAControl->DeltaT4_Module.Module7,3), 
										(6*(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7))) +
										AREAControl->VmReal_Module.Module7*AREAControl->DeltaT4_Module.Module7;
				AREAControl->S5_Module.Module7 	= divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp(AREAControl->DeltaT5_Module.Module7,3), 
										(6*(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7))) +
										AREAControl->Ve_Module.Module7*AREAControl->DeltaT5_Module.Module7;
				AREAControl->Sde_Module.Module7 	= AREAControl->S4_Module.Module7+AREAControl->S5_Module.Module7;
			}
					
			AREAControl->S3_Module.Module7 		= AREAControl->Stotal_Module.Module7-AREAControl->S1_Module.Module7-AREAControl->S2_Module.Module7-
										AREAControl->S4_Module.Module7-AREAControl->S5_Module.Module7;
			
			if(AREAControl->VmReal_Module.Module7 < 0.000001) 
				AREAControl->DeltaT3_Module.Module7 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module7 > 0)
				{
					AREAControl->DeltaT3_Module.Module7 = divdp(AREAControl->S3_Module.Module7, AREAControl->VmReal_Module.Module7);
				}
				else
				{
					AREAControl->S3_Module.Module7 = 0;
					AREAControl->DeltaT3_Module.Module7 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module7	= AREAControl->S1_Module.Module7+AREAControl->S2_Module.Module7;
		AREAControl->Sde_Module.Module7	= AREAControl->S4_Module.Module7+AREAControl->S5_Module.Module7;
		AREAControl->Sm_Module.Module7	= AREAControl->S3_Module.Module7;

		AREAControl->T1_Module.Module7 	= AREAControl->DeltaT1_Module.Module7;
		AREAControl->T2_Module.Module7 	= AREAControl->T1_Module.Module7+AREAControl->DeltaT2_Module.Module7;
		AREAControl->T3_Module.Module7 	= AREAControl->T2_Module.Module7+AREAControl->DeltaT3_Module.Module7;
		AREAControl->T4_Module.Module7 	= AREAControl->T3_Module.Module7+AREAControl->DeltaT4_Module.Module7;
		AREAControl->T5_Module.Module7 	= AREAControl->T4_Module.Module7+AREAControl->DeltaT5_Module.Module7;
		AREAControl->TotalN_Module.Module7 = divdp(AREAControl->T5_Module.Module7, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 8)
	{
		if( AREAControl->SpeedState.Module8 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module8 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module8,2) - powdp(AREAControl->Vs_Module.Module8,2)), AREAControl->a_ac_Module.Module8) );
			AREAControl->Sde_Module.Module8 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module8,2) - powdp(AREAControl->Ve_Module.Module8,2)), AREAControl->a_de_Module.Module8) );

			if(AREAControl->Stotal_Module.Module8 >= (AREAControl->Sac_Module.Module8+AREAControl->Sde_Module.Module8)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module8 = AREAControl->Vm_Module.Module8;
				AREAControl->Sm_Module.Module8 	= AREAControl->Ss_Module.Module8-AREAControl->Sac_Module.Module8-AREAControl->Sde_Module.Module8;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module8 >= AREAControl->Vs_Module.Module8 ) ? (AREAControl->a_ac_Module.Module8):(AREAControl->a_de_Module.Module8);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module8, 2 ) - powdp( AREAControl->Vs_Module.Module8, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module8) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module8, 2 ) - powdp( AREAControl->Vs_Module.Module8, 2 ) ), AREAControl->Stotal_Module.Module8 );

					if( AREAControl->Ve_Module.Module8 >= AREAControl->Vs_Module.Module8 )
					{
						AREAControl->VmReal_Module.Module8 = AREAControl->Ve_Module.Module8;
						AREAControl->a_ac_Module.Module8 = divdp( fabs( powdp( AREAControl->Ve_Module.Module8, 2 ) - powdp( AREAControl->Vs_Module.Module8, 2 ) ), AREAControl->Stotal_Module.Module8 );
					}
					else
					{
						AREAControl->VmReal_Module.Module8 = AREAControl->Vs_Module.Module8;

						a_de_sd = AREAControl->a_de_Module.Module8;
						AREAControl->a_de_Module.Module8 = divdp( fabs( powdp( AREAControl->Ve_Module.Module8, 2 ) - powdp( AREAControl->Vs_Module.Module8, 2 ) ), AREAControl->Stotal_Module.Module8 );
						if(AREAControl->a_de_Module.Module8 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module8 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module8 = sqrtdp( powdp(AREAControl->Vs_Module.Module8,2) - AREAControl->a_de_Module.Module8*AREAControl->Stotal_Module.Module8 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module8 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module8 * AREAControl->a_de_Module.Module8 * AREAControl->Stotal_Module.Module8 + 
												AREAControl->a_de_Module.Module8 * powdp( AREAControl->Vs_Module.Module8, 2 ) + AREAControl->a_ac_Module.Module8 * 
												powdp( AREAControl->Ve_Module.Module8, 2 ) ),  ( AREAControl->a_ac_Module.Module8 + AREAControl->a_de_Module.Module8 ) ) );
					AREAControl->Sm_Module.Module8		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module8 	= divdp( fabs(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8), AREAControl->a_ac_Module.Module8);
			AREAControl->DeltaT2_Module.Module8 	= AREAControl->DeltaT1_Module.Module8;
			if(AREAControl->DeltaT1_Module.Module8 == 0)
			{
				AREAControl->S1_Module.Module8		= 0;
				AREAControl->S2_Module.Module8		= 0;
				AREAControl->Sac_Module.Module8	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module8		= divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->DeltaT1_Module.Module8,3), 
										(6*(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8))) + 
										AREAControl->Vs_Module.Module8*AREAControl->DeltaT1_Module.Module8;
				AREAControl->S2_Module.Module8 	= divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->DeltaT2_Module.Module8,3), 
										(6*(AREAControl->Vs_Module.Module8-AREAControl->VmReal_Module.Module8))) +
										AREAControl->VmReal_Module.Module8*AREAControl->DeltaT2_Module.Module8;
				AREAControl->Sac_Module.Module8	= AREAControl->S1_Module.Module8 + AREAControl->S2_Module.Module8;
			}		
			
			AREAControl->DeltaT4_Module.Module8 	= divdp( fabs(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8), AREAControl->a_de_Module.Module8 );
			AREAControl->DeltaT5_Module.Module8 	= AREAControl->DeltaT4_Module.Module8;
			if(AREAControl->DeltaT4_Module.Module8 == 0)
			{
				AREAControl->S4_Module.Module8 	= 0;
				AREAControl->S5_Module.Module8 	= 0;
				AREAControl->Sde_Module.Module8 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module8 	= divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp(AREAControl->DeltaT4_Module.Module8,3), 
										(6*(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8))) +
										AREAControl->VmReal_Module.Module8*AREAControl->DeltaT4_Module.Module8;
				AREAControl->S5_Module.Module8 	= divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp(AREAControl->DeltaT5_Module.Module8,3), 
										(6*(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8))) +
										AREAControl->Ve_Module.Module8*AREAControl->DeltaT5_Module.Module8;
				AREAControl->Sde_Module.Module8 	= AREAControl->S4_Module.Module8+AREAControl->S5_Module.Module8;
			}
					
			AREAControl->S3_Module.Module8 		= AREAControl->Stotal_Module.Module8-AREAControl->S1_Module.Module8-AREAControl->S2_Module.Module8-
										AREAControl->S4_Module.Module8-AREAControl->S5_Module.Module8;
			
			if(AREAControl->VmReal_Module.Module8==0) 
				AREAControl->DeltaT3_Module.Module8 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module8 > 0)
				{
					AREAControl->DeltaT3_Module.Module8 = divdp(AREAControl->S3_Module.Module8, AREAControl->VmReal_Module.Module8);
				}
				else
				{
					AREAControl->S3_Module.Module8 = 0;
					AREAControl->DeltaT3_Module.Module8 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module8 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module8 = AREAControl->a_de_Module.Module8;
			AREAControl->Sac_Module.Module8 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module8,2) - powdp(AREAControl->Vs_Module.Module8,2)), AREAControl->a_ac_Module.Module8) );
			AREAControl->Sde_Module.Module8 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module8,2) - powdp(AREAControl->Ve_Module.Module8,2)), AREAControl->a_de_Module.Module8) );

			if(AREAControl->Stotal_Module.Module8 >= (AREAControl->Sac_Module.Module8+AREAControl->Sde_Module.Module8)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module8 = AREAControl->Vm_Module.Module8;
				AREAControl->Sm_Module.Module8 	= AREAControl->Ss_Module.Module8-AREAControl->Sac_Module.Module8-AREAControl->Sde_Module.Module8;
			}
			else if(AREAControl->Stotal_Module.Module8 >= AREAControl->Sac_Module.Module8)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module8	= AREAControl->Vm_Module.Module8;
				AREAControl->Ve_Module.Module8		= sqrtdp( powdp(AREAControl->VmReal_Module.Module8,2) - AREAControl->a_de_Module.Module8*(AREAControl->Stotal_Module.Module8-AREAControl->Sac_Module.Module8) );
				AREAControl->Sm_Module.Module8		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module8 = sqrtdp( powdp(AREAControl->Vs_Module.Module8,2) - AREAControl->a_ac_Module.Module8*AREAControl->Stotal_Module.Module8 );
				AREAControl->Ve_Module.Module8 = AREAControl->VmReal_Module.Module8;
				AREAControl->Sm_Module.Module8		= 0;				
			}

			AREAControl->DeltaT1_Module.Module8 	= divdp( fabs(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8), AREAControl->a_ac_Module.Module8);
			AREAControl->DeltaT2_Module.Module8 	= AREAControl->DeltaT1_Module.Module8;
			if(AREAControl->DeltaT1_Module.Module8 < 0.000001)
			{
				AREAControl->S1_Module.Module8	= 0;
				AREAControl->S2_Module.Module8	= 0;
				AREAControl->Sac_Module.Module8	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module8		= divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->DeltaT1_Module.Module8,3), 
										(6*(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8))) + 
										AREAControl->Vs_Module.Module8*AREAControl->DeltaT1_Module.Module8;
				AREAControl->S2_Module.Module8 	= divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->DeltaT2_Module.Module8,3), 
										(6*(AREAControl->Vs_Module.Module8-AREAControl->VmReal_Module.Module8))) +
										AREAControl->VmReal_Module.Module8*AREAControl->DeltaT2_Module.Module8;
				AREAControl->Sac_Module.Module8	= AREAControl->S1_Module.Module8 + AREAControl->S2_Module.Module8;
			}		
			
			AREAControl->DeltaT4_Module.Module8 	= divdp( fabs(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8), AREAControl->a_de_Module.Module8 );
			AREAControl->DeltaT5_Module.Module8 	= AREAControl->DeltaT4_Module.Module8;
			if(AREAControl->DeltaT4_Module.Module8 < 0.000001)
			{
				AREAControl->S4_Module.Module8 	= 0;
				AREAControl->S5_Module.Module8 	= 0;
				AREAControl->Sde_Module.Module8 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module8 	= divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp(AREAControl->DeltaT4_Module.Module8,3), 
										(6*(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8))) +
										AREAControl->VmReal_Module.Module8*AREAControl->DeltaT4_Module.Module8;
				AREAControl->S5_Module.Module8 	= divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp(AREAControl->DeltaT5_Module.Module8,3), 
										(6*(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8))) +
										AREAControl->Ve_Module.Module8*AREAControl->DeltaT5_Module.Module8;
				AREAControl->Sde_Module.Module8 	= AREAControl->S4_Module.Module8+AREAControl->S5_Module.Module8;
			}
					
			AREAControl->S3_Module.Module8 		= AREAControl->Stotal_Module.Module8-AREAControl->S1_Module.Module8-AREAControl->S2_Module.Module8-
										AREAControl->S4_Module.Module8-AREAControl->S5_Module.Module8;
			
			if(AREAControl->VmReal_Module.Module8 < 0.000001) 
				AREAControl->DeltaT3_Module.Module8 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module8 > 0)
				{
					AREAControl->DeltaT3_Module.Module8 = divdp(AREAControl->S3_Module.Module8, AREAControl->VmReal_Module.Module8);
				}
				else
				{
					AREAControl->S3_Module.Module8 = 0;
					AREAControl->DeltaT3_Module.Module8 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module8	= AREAControl->S1_Module.Module8+AREAControl->S2_Module.Module8;
		AREAControl->Sde_Module.Module8	= AREAControl->S4_Module.Module8+AREAControl->S5_Module.Module8;
		AREAControl->Sm_Module.Module8	= AREAControl->S3_Module.Module8;

		AREAControl->T1_Module.Module8 	= AREAControl->DeltaT1_Module.Module8;
		AREAControl->T2_Module.Module8 	= AREAControl->T1_Module.Module8+AREAControl->DeltaT2_Module.Module8;
		AREAControl->T3_Module.Module8 	= AREAControl->T2_Module.Module8+AREAControl->DeltaT3_Module.Module8;
		AREAControl->T4_Module.Module8 	= AREAControl->T3_Module.Module8+AREAControl->DeltaT4_Module.Module8;
		AREAControl->T5_Module.Module8 	= AREAControl->T4_Module.Module8+AREAControl->DeltaT5_Module.Module8;
		AREAControl->TotalN_Module.Module8 = divdp(AREAControl->T5_Module.Module8, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 9)
	{
		if( AREAControl->SpeedState.Module9 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module9 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module9,2) - powdp(AREAControl->Vs_Module.Module9,2)), AREAControl->a_ac_Module.Module9) );
			AREAControl->Sde_Module.Module9 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module9,2) - powdp(AREAControl->Ve_Module.Module9,2)), AREAControl->a_de_Module.Module9) );

			if(AREAControl->Stotal_Module.Module9 >= (AREAControl->Sac_Module.Module9+AREAControl->Sde_Module.Module9)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module9 = AREAControl->Vm_Module.Module9;
				AREAControl->Sm_Module.Module9 	= AREAControl->Ss_Module.Module9-AREAControl->Sac_Module.Module9-AREAControl->Sde_Module.Module9;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module9 >= AREAControl->Vs_Module.Module9 ) ? (AREAControl->a_ac_Module.Module9):(AREAControl->a_de_Module.Module9);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module9, 2 ) - powdp( AREAControl->Vs_Module.Module9, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module9) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module9, 2 ) - powdp( AREAControl->Vs_Module.Module9, 2 ) ), AREAControl->Stotal_Module.Module9 );

					if( AREAControl->Ve_Module.Module9 >= AREAControl->Vs_Module.Module9 )
					{
						AREAControl->VmReal_Module.Module9 = AREAControl->Ve_Module.Module9;
						AREAControl->a_ac_Module.Module9 = divdp( fabs( powdp( AREAControl->Ve_Module.Module9, 2 ) - powdp( AREAControl->Vs_Module.Module9, 2 ) ), AREAControl->Stotal_Module.Module9 );
					}
					else
					{
						AREAControl->VmReal_Module.Module9 = AREAControl->Vs_Module.Module9;

						a_de_sd = AREAControl->a_de_Module.Module9;
						AREAControl->a_de_Module.Module9 = divdp( fabs( powdp( AREAControl->Ve_Module.Module9, 2 ) - powdp( AREAControl->Vs_Module.Module9, 2 ) ), AREAControl->Stotal_Module.Module9 );
						if(AREAControl->a_de_Module.Module9 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module9 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module9 = sqrtdp( powdp(AREAControl->Vs_Module.Module9,2) - AREAControl->a_de_Module.Module9*AREAControl->Stotal_Module.Module9 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module9 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module9 * AREAControl->a_de_Module.Module9 * AREAControl->Stotal_Module.Module9 + 
												AREAControl->a_de_Module.Module9 * powdp( AREAControl->Vs_Module.Module9, 2 ) + AREAControl->a_ac_Module.Module9 * 
												powdp( AREAControl->Ve_Module.Module9, 2 ) ),  ( AREAControl->a_ac_Module.Module9 + AREAControl->a_de_Module.Module9 ) ) );
					AREAControl->Sm_Module.Module9		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module9 	= divdp( fabs(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9), AREAControl->a_ac_Module.Module9);
			AREAControl->DeltaT2_Module.Module9 	= AREAControl->DeltaT1_Module.Module9;
			if(AREAControl->DeltaT1_Module.Module9 == 0)
			{
				AREAControl->S1_Module.Module9		= 0;
				AREAControl->S2_Module.Module9		= 0;
				AREAControl->Sac_Module.Module9	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module9		= divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->DeltaT1_Module.Module9,3), 
										(6*(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9))) + 
										AREAControl->Vs_Module.Module9*AREAControl->DeltaT1_Module.Module9;
				AREAControl->S2_Module.Module9 	= divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->DeltaT2_Module.Module9,3), 
										(6*(AREAControl->Vs_Module.Module9-AREAControl->VmReal_Module.Module9))) +
										AREAControl->VmReal_Module.Module9*AREAControl->DeltaT2_Module.Module9;
				AREAControl->Sac_Module.Module9	= AREAControl->S1_Module.Module9 + AREAControl->S2_Module.Module9;
			}		
			
			AREAControl->DeltaT4_Module.Module9 	= divdp( fabs(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9), AREAControl->a_de_Module.Module9 );
			AREAControl->DeltaT5_Module.Module9 	= AREAControl->DeltaT4_Module.Module9;
			if(AREAControl->DeltaT4_Module.Module9 == 0)
			{
				AREAControl->S4_Module.Module9 	= 0;
				AREAControl->S5_Module.Module9 	= 0;
				AREAControl->Sde_Module.Module9 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module9 	= divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp(AREAControl->DeltaT4_Module.Module9,3), 
										(6*(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9))) +
										AREAControl->VmReal_Module.Module9*AREAControl->DeltaT4_Module.Module9;
				AREAControl->S5_Module.Module9 	= divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp(AREAControl->DeltaT5_Module.Module9,3), 
										(6*(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9))) +
										AREAControl->Ve_Module.Module9*AREAControl->DeltaT5_Module.Module9;
				AREAControl->Sde_Module.Module9 	= AREAControl->S4_Module.Module9+AREAControl->S5_Module.Module9;
			}
					
			AREAControl->S3_Module.Module9 		= AREAControl->Stotal_Module.Module9-AREAControl->S1_Module.Module9-AREAControl->S2_Module.Module9-
										AREAControl->S4_Module.Module9-AREAControl->S5_Module.Module9;
			
			if(AREAControl->VmReal_Module.Module9==0) 
				AREAControl->DeltaT3_Module.Module9 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module9 > 0)
				{
					AREAControl->DeltaT3_Module.Module9 = divdp(AREAControl->S3_Module.Module9, AREAControl->VmReal_Module.Module9);
				}
				else
				{
					AREAControl->S3_Module.Module9 = 0;
					AREAControl->DeltaT3_Module.Module9 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module9 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module9 = AREAControl->a_de_Module.Module9;
			AREAControl->Sac_Module.Module9 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module9,2) - powdp(AREAControl->Vs_Module.Module9,2)), AREAControl->a_ac_Module.Module9) );
			AREAControl->Sde_Module.Module9 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module9,2) - powdp(AREAControl->Ve_Module.Module9,2)), AREAControl->a_de_Module.Module9) );

			if(AREAControl->Stotal_Module.Module9 >= (AREAControl->Sac_Module.Module9+AREAControl->Sde_Module.Module9)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module9 = AREAControl->Vm_Module.Module9;
				AREAControl->Sm_Module.Module9 	= AREAControl->Ss_Module.Module9-AREAControl->Sac_Module.Module9-AREAControl->Sde_Module.Module9;
			}
			else if(AREAControl->Stotal_Module.Module9 >= AREAControl->Sac_Module.Module9)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module9	= AREAControl->Vm_Module.Module9;
				AREAControl->Ve_Module.Module9		= sqrtdp( powdp(AREAControl->VmReal_Module.Module9,2) - AREAControl->a_de_Module.Module9*(AREAControl->Stotal_Module.Module9-AREAControl->Sac_Module.Module9) );
				AREAControl->Sm_Module.Module9		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module9 = sqrtdp( powdp(AREAControl->Vs_Module.Module9,2) - AREAControl->a_ac_Module.Module9*AREAControl->Stotal_Module.Module9 );
				AREAControl->Ve_Module.Module9 = AREAControl->VmReal_Module.Module9;
				AREAControl->Sm_Module.Module9		= 0;				
			}

			AREAControl->DeltaT1_Module.Module9 	= divdp( fabs(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9), AREAControl->a_ac_Module.Module9);
			AREAControl->DeltaT2_Module.Module9 	= AREAControl->DeltaT1_Module.Module9;
			if(AREAControl->DeltaT1_Module.Module9 < 0.000001)
			{
				AREAControl->S1_Module.Module9	= 0;
				AREAControl->S2_Module.Module9	= 0;
				AREAControl->Sac_Module.Module9	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module9		= divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->DeltaT1_Module.Module9,3), 
										(6*(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9))) + 
										AREAControl->Vs_Module.Module9*AREAControl->DeltaT1_Module.Module9;
				AREAControl->S2_Module.Module9 	= divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->DeltaT2_Module.Module9,3), 
										(6*(AREAControl->Vs_Module.Module9-AREAControl->VmReal_Module.Module9))) +
										AREAControl->VmReal_Module.Module9*AREAControl->DeltaT2_Module.Module9;
				AREAControl->Sac_Module.Module9	= AREAControl->S1_Module.Module9 + AREAControl->S2_Module.Module9;
			}		
			
			AREAControl->DeltaT4_Module.Module9 	= divdp( fabs(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9), AREAControl->a_de_Module.Module9 );
			AREAControl->DeltaT5_Module.Module9 	= AREAControl->DeltaT4_Module.Module9;
			if(AREAControl->DeltaT4_Module.Module9 < 0.000001)
			{
				AREAControl->S4_Module.Module9 	= 0;
				AREAControl->S5_Module.Module9 	= 0;
				AREAControl->Sde_Module.Module9 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module9 	= divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp(AREAControl->DeltaT4_Module.Module9,3), 
										(6*(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9))) +
										AREAControl->VmReal_Module.Module9*AREAControl->DeltaT4_Module.Module9;
				AREAControl->S5_Module.Module9 	= divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp(AREAControl->DeltaT5_Module.Module9,3), 
										(6*(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9))) +
										AREAControl->Ve_Module.Module9*AREAControl->DeltaT5_Module.Module9;
				AREAControl->Sde_Module.Module9 	= AREAControl->S4_Module.Module9+AREAControl->S5_Module.Module9;
			}
					
			AREAControl->S3_Module.Module9 		= AREAControl->Stotal_Module.Module9-AREAControl->S1_Module.Module9-AREAControl->S2_Module.Module9-
										AREAControl->S4_Module.Module9-AREAControl->S5_Module.Module9;
			
			if(AREAControl->VmReal_Module.Module9 < 0.000001) 
				AREAControl->DeltaT3_Module.Module9 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module9 > 0)
				{
					AREAControl->DeltaT3_Module.Module9 = divdp(AREAControl->S3_Module.Module9, AREAControl->VmReal_Module.Module9);
				}
				else
				{
					AREAControl->S3_Module.Module9 = 0;
					AREAControl->DeltaT3_Module.Module9 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module9	= AREAControl->S1_Module.Module9+AREAControl->S2_Module.Module9;
		AREAControl->Sde_Module.Module9	= AREAControl->S4_Module.Module9+AREAControl->S5_Module.Module9;
		AREAControl->Sm_Module.Module9	= AREAControl->S3_Module.Module9;

		AREAControl->T1_Module.Module9 	= AREAControl->DeltaT1_Module.Module9;
		AREAControl->T2_Module.Module9 	= AREAControl->T1_Module.Module9+AREAControl->DeltaT2_Module.Module9;
		AREAControl->T3_Module.Module9 	= AREAControl->T2_Module.Module9+AREAControl->DeltaT3_Module.Module9;
		AREAControl->T4_Module.Module9 	= AREAControl->T3_Module.Module9+AREAControl->DeltaT4_Module.Module9;
		AREAControl->T5_Module.Module9 	= AREAControl->T4_Module.Module9+AREAControl->DeltaT5_Module.Module9;
		AREAControl->TotalN_Module.Module9 = divdp(AREAControl->T5_Module.Module9, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 10)
	{
		if( AREAControl->SpeedState.Module10 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module10 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module10,2) - powdp(AREAControl->Vs_Module.Module10,2)), AREAControl->a_ac_Module.Module10) );
			AREAControl->Sde_Module.Module10 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module10,2) - powdp(AREAControl->Ve_Module.Module10,2)), AREAControl->a_de_Module.Module10) );

			if(AREAControl->Stotal_Module.Module10 >= (AREAControl->Sac_Module.Module10+AREAControl->Sde_Module.Module10)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module10 = AREAControl->Vm_Module.Module10;
				AREAControl->Sm_Module.Module10 	= AREAControl->Ss_Module.Module10-AREAControl->Sac_Module.Module10-AREAControl->Sde_Module.Module10;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module10 >= AREAControl->Vs_Module.Module10 ) ? (AREAControl->a_ac_Module.Module10):(AREAControl->a_de_Module.Module10);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module10, 2 ) - powdp( AREAControl->Vs_Module.Module10, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module10) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module10, 2 ) - powdp( AREAControl->Vs_Module.Module10, 2 ) ), AREAControl->Stotal_Module.Module10 );

					if( AREAControl->Ve_Module.Module10 >= AREAControl->Vs_Module.Module10 )
					{
						AREAControl->VmReal_Module.Module10 = AREAControl->Ve_Module.Module10;
						AREAControl->a_ac_Module.Module10 = divdp( fabs( powdp( AREAControl->Ve_Module.Module10, 2 ) - powdp( AREAControl->Vs_Module.Module10, 2 ) ), AREAControl->Stotal_Module.Module10 );
					}
					else
					{
						AREAControl->VmReal_Module.Module10 = AREAControl->Vs_Module.Module10;

						a_de_sd = AREAControl->a_de_Module.Module10;
						AREAControl->a_de_Module.Module10 = divdp( fabs( powdp( AREAControl->Ve_Module.Module10, 2 ) - powdp( AREAControl->Vs_Module.Module10, 2 ) ), AREAControl->Stotal_Module.Module10 );
						if(AREAControl->a_de_Module.Module10 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module10 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module10 = sqrtdp( powdp(AREAControl->Vs_Module.Module10,2) - AREAControl->a_de_Module.Module10*AREAControl->Stotal_Module.Module10 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module10 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module10 * AREAControl->a_de_Module.Module10 * AREAControl->Stotal_Module.Module10 + 
												AREAControl->a_de_Module.Module10 * powdp( AREAControl->Vs_Module.Module10, 2 ) + AREAControl->a_ac_Module.Module10 * 
												powdp( AREAControl->Ve_Module.Module10, 2 ) ),  ( AREAControl->a_ac_Module.Module10 + AREAControl->a_de_Module.Module10 ) ) );
					AREAControl->Sm_Module.Module10		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module10 	= divdp( fabs(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10), AREAControl->a_ac_Module.Module10);
			AREAControl->DeltaT2_Module.Module10 	= AREAControl->DeltaT1_Module.Module10;
			if(AREAControl->DeltaT1_Module.Module10 == 0)
			{
				AREAControl->S1_Module.Module10		= 0;
				AREAControl->S2_Module.Module10		= 0;
				AREAControl->Sac_Module.Module10	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module10		= divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->DeltaT1_Module.Module10,3), 
										(6*(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10))) + 
										AREAControl->Vs_Module.Module10*AREAControl->DeltaT1_Module.Module10;
				AREAControl->S2_Module.Module10 	= divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->DeltaT2_Module.Module10,3), 
										(6*(AREAControl->Vs_Module.Module10-AREAControl->VmReal_Module.Module10))) +
										AREAControl->VmReal_Module.Module10*AREAControl->DeltaT2_Module.Module10;
				AREAControl->Sac_Module.Module10	= AREAControl->S1_Module.Module10 + AREAControl->S2_Module.Module10;
			}		
			
			AREAControl->DeltaT4_Module.Module10 	= divdp( fabs(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10), AREAControl->a_de_Module.Module10 );
			AREAControl->DeltaT5_Module.Module10 	= AREAControl->DeltaT4_Module.Module10;
			if(AREAControl->DeltaT4_Module.Module10 == 0)
			{
				AREAControl->S4_Module.Module10 	= 0;
				AREAControl->S5_Module.Module10 	= 0;
				AREAControl->Sde_Module.Module10 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module10 	= divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp(AREAControl->DeltaT4_Module.Module10,3), 
										(6*(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10))) +
										AREAControl->VmReal_Module.Module10*AREAControl->DeltaT4_Module.Module10;
				AREAControl->S5_Module.Module10 	= divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp(AREAControl->DeltaT5_Module.Module10,3), 
										(6*(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10))) +
										AREAControl->Ve_Module.Module10*AREAControl->DeltaT5_Module.Module10;
				AREAControl->Sde_Module.Module10 	= AREAControl->S4_Module.Module10+AREAControl->S5_Module.Module10;
			}
					
			AREAControl->S3_Module.Module10 		= AREAControl->Stotal_Module.Module10-AREAControl->S1_Module.Module10-AREAControl->S2_Module.Module10-
										AREAControl->S4_Module.Module10-AREAControl->S5_Module.Module10;
			
			if(AREAControl->VmReal_Module.Module10==0) 
				AREAControl->DeltaT3_Module.Module10 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module10 > 0)
				{
					AREAControl->DeltaT3_Module.Module10 = divdp(AREAControl->S3_Module.Module10, AREAControl->VmReal_Module.Module10);
				}
				else
				{
					AREAControl->S3_Module.Module10 = 0;
					AREAControl->DeltaT3_Module.Module10 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module10 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module10 = AREAControl->a_de_Module.Module10;
			AREAControl->Sac_Module.Module10 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module10,2) - powdp(AREAControl->Vs_Module.Module10,2)), AREAControl->a_ac_Module.Module10) );
			AREAControl->Sde_Module.Module10 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module10,2) - powdp(AREAControl->Ve_Module.Module10,2)), AREAControl->a_de_Module.Module10) );

			if(AREAControl->Stotal_Module.Module10 >= (AREAControl->Sac_Module.Module10+AREAControl->Sde_Module.Module10)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module10 = AREAControl->Vm_Module.Module10;
				AREAControl->Sm_Module.Module10 	= AREAControl->Ss_Module.Module10-AREAControl->Sac_Module.Module10-AREAControl->Sde_Module.Module10;
			}
			else if(AREAControl->Stotal_Module.Module10 >= AREAControl->Sac_Module.Module10)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module10	= AREAControl->Vm_Module.Module10;
				AREAControl->Ve_Module.Module10		= sqrtdp( powdp(AREAControl->VmReal_Module.Module10,2) - AREAControl->a_de_Module.Module10*(AREAControl->Stotal_Module.Module10-AREAControl->Sac_Module.Module10) );
				AREAControl->Sm_Module.Module10		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module10 = sqrtdp( powdp(AREAControl->Vs_Module.Module10,2) - AREAControl->a_ac_Module.Module10*AREAControl->Stotal_Module.Module10 );
				AREAControl->Ve_Module.Module10 = AREAControl->VmReal_Module.Module10;
				AREAControl->Sm_Module.Module10		= 0;				
			}

			AREAControl->DeltaT1_Module.Module10 	= divdp( fabs(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10), AREAControl->a_ac_Module.Module10);
			AREAControl->DeltaT2_Module.Module10 	= AREAControl->DeltaT1_Module.Module10;
			if(AREAControl->DeltaT1_Module.Module10 < 0.000001)
			{
				AREAControl->S1_Module.Module10	= 0;
				AREAControl->S2_Module.Module10	= 0;
				AREAControl->Sac_Module.Module10	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module10		= divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->DeltaT1_Module.Module10,3), 
										(6*(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10))) + 
										AREAControl->Vs_Module.Module10*AREAControl->DeltaT1_Module.Module10;
				AREAControl->S2_Module.Module10 	= divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->DeltaT2_Module.Module10,3), 
										(6*(AREAControl->Vs_Module.Module10-AREAControl->VmReal_Module.Module10))) +
										AREAControl->VmReal_Module.Module10*AREAControl->DeltaT2_Module.Module10;
				AREAControl->Sac_Module.Module10	= AREAControl->S1_Module.Module10 + AREAControl->S2_Module.Module10;
			}		
			
			AREAControl->DeltaT4_Module.Module10 	= divdp( fabs(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10), AREAControl->a_de_Module.Module10 );
			AREAControl->DeltaT5_Module.Module10 	= AREAControl->DeltaT4_Module.Module10;
			if(AREAControl->DeltaT4_Module.Module10 < 0.000001)
			{
				AREAControl->S4_Module.Module10 	= 0;
				AREAControl->S5_Module.Module10 	= 0;
				AREAControl->Sde_Module.Module10 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module10 	= divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp(AREAControl->DeltaT4_Module.Module10,3), 
										(6*(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10))) +
										AREAControl->VmReal_Module.Module10*AREAControl->DeltaT4_Module.Module10;
				AREAControl->S5_Module.Module10 	= divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp(AREAControl->DeltaT5_Module.Module10,3), 
										(6*(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10))) +
										AREAControl->Ve_Module.Module10*AREAControl->DeltaT5_Module.Module10;
				AREAControl->Sde_Module.Module10 	= AREAControl->S4_Module.Module10+AREAControl->S5_Module.Module10;
			}
					
			AREAControl->S3_Module.Module10 		= AREAControl->Stotal_Module.Module10-AREAControl->S1_Module.Module10-AREAControl->S2_Module.Module10-
										AREAControl->S4_Module.Module10-AREAControl->S5_Module.Module10;
			
			if(AREAControl->VmReal_Module.Module10 < 0.000001) 
				AREAControl->DeltaT3_Module.Module10 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module10 > 0)
				{
					AREAControl->DeltaT3_Module.Module10 = divdp(AREAControl->S3_Module.Module10, AREAControl->VmReal_Module.Module10);
				}
				else
				{
					AREAControl->S3_Module.Module10 = 0;
					AREAControl->DeltaT3_Module.Module10 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module10	= AREAControl->S1_Module.Module10+AREAControl->S2_Module.Module10;
		AREAControl->Sde_Module.Module10	= AREAControl->S4_Module.Module10+AREAControl->S5_Module.Module10;
		AREAControl->Sm_Module.Module10	= AREAControl->S3_Module.Module10;

		AREAControl->T1_Module.Module10 	= AREAControl->DeltaT1_Module.Module10;
		AREAControl->T2_Module.Module10 	= AREAControl->T1_Module.Module10+AREAControl->DeltaT2_Module.Module10;
		AREAControl->T3_Module.Module10 	= AREAControl->T2_Module.Module10+AREAControl->DeltaT3_Module.Module10;
		AREAControl->T4_Module.Module10 	= AREAControl->T3_Module.Module10+AREAControl->DeltaT4_Module.Module10;
		AREAControl->T5_Module.Module10 	= AREAControl->T4_Module.Module10+AREAControl->DeltaT5_Module.Module10;
		AREAControl->TotalN_Module.Module10 = divdp(AREAControl->T5_Module.Module10, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 11)
	{
		if( AREAControl->SpeedState.Module11 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module11 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module11,2) - powdp(AREAControl->Vs_Module.Module11,2)), AREAControl->a_ac_Module.Module11) );
			AREAControl->Sde_Module.Module11 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module11,2) - powdp(AREAControl->Ve_Module.Module11,2)), AREAControl->a_de_Module.Module11) );

			if(AREAControl->Stotal_Module.Module11 >= (AREAControl->Sac_Module.Module11+AREAControl->Sde_Module.Module11)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module11 = AREAControl->Vm_Module.Module11;
				AREAControl->Sm_Module.Module11 	= AREAControl->Ss_Module.Module11-AREAControl->Sac_Module.Module11-AREAControl->Sde_Module.Module11;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module11 >= AREAControl->Vs_Module.Module11 ) ? (AREAControl->a_ac_Module.Module11):(AREAControl->a_de_Module.Module11);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module11, 2 ) - powdp( AREAControl->Vs_Module.Module11, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module11) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module11, 2 ) - powdp( AREAControl->Vs_Module.Module11, 2 ) ), AREAControl->Stotal_Module.Module11 );

					if( AREAControl->Ve_Module.Module11 >= AREAControl->Vs_Module.Module11 )
					{
						AREAControl->VmReal_Module.Module11 = AREAControl->Ve_Module.Module11;
						AREAControl->a_ac_Module.Module11 = divdp( fabs( powdp( AREAControl->Ve_Module.Module11, 2 ) - powdp( AREAControl->Vs_Module.Module11, 2 ) ), AREAControl->Stotal_Module.Module11 );
					}
					else
					{
						AREAControl->VmReal_Module.Module11 = AREAControl->Vs_Module.Module11;

						a_de_sd = AREAControl->a_de_Module.Module11;
						AREAControl->a_de_Module.Module11 = divdp( fabs( powdp( AREAControl->Ve_Module.Module11, 2 ) - powdp( AREAControl->Vs_Module.Module11, 2 ) ), AREAControl->Stotal_Module.Module11 );
						if(AREAControl->a_de_Module.Module11 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module11 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module11 = sqrtdp( powdp(AREAControl->Vs_Module.Module11,2) - AREAControl->a_de_Module.Module11*AREAControl->Stotal_Module.Module11 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module11 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module11 * AREAControl->a_de_Module.Module11 * AREAControl->Stotal_Module.Module11 + 
												AREAControl->a_de_Module.Module11 * powdp( AREAControl->Vs_Module.Module11, 2 ) + AREAControl->a_ac_Module.Module11 * 
												powdp( AREAControl->Ve_Module.Module11, 2 ) ),  ( AREAControl->a_ac_Module.Module11 + AREAControl->a_de_Module.Module11 ) ) );
					AREAControl->Sm_Module.Module11		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module11 	= divdp( fabs(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11), AREAControl->a_ac_Module.Module11);
			AREAControl->DeltaT2_Module.Module11 	= AREAControl->DeltaT1_Module.Module11;
			if(AREAControl->DeltaT1_Module.Module11 == 0)
			{
				AREAControl->S1_Module.Module11		= 0;
				AREAControl->S2_Module.Module11		= 0;
				AREAControl->Sac_Module.Module11	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module11		= divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->DeltaT1_Module.Module11,3), 
										(6*(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11))) + 
										AREAControl->Vs_Module.Module11*AREAControl->DeltaT1_Module.Module11;
				AREAControl->S2_Module.Module11 	= divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->DeltaT2_Module.Module11,3), 
										(6*(AREAControl->Vs_Module.Module11-AREAControl->VmReal_Module.Module11))) +
										AREAControl->VmReal_Module.Module11*AREAControl->DeltaT2_Module.Module11;
				AREAControl->Sac_Module.Module11	= AREAControl->S1_Module.Module11 + AREAControl->S2_Module.Module11;
			}		
			
			AREAControl->DeltaT4_Module.Module11 	= divdp( fabs(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11), AREAControl->a_de_Module.Module11 );
			AREAControl->DeltaT5_Module.Module11 	= AREAControl->DeltaT4_Module.Module11;
			if(AREAControl->DeltaT4_Module.Module11 == 0)
			{
				AREAControl->S4_Module.Module11 	= 0;
				AREAControl->S5_Module.Module11 	= 0;
				AREAControl->Sde_Module.Module11 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module11 	= divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp(AREAControl->DeltaT4_Module.Module11,3), 
										(6*(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11))) +
										AREAControl->VmReal_Module.Module11*AREAControl->DeltaT4_Module.Module11;
				AREAControl->S5_Module.Module11 	= divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp(AREAControl->DeltaT5_Module.Module11,3), 
										(6*(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11))) +
										AREAControl->Ve_Module.Module11*AREAControl->DeltaT5_Module.Module11;
				AREAControl->Sde_Module.Module11 	= AREAControl->S4_Module.Module11+AREAControl->S5_Module.Module11;
			}
					
			AREAControl->S3_Module.Module11 		= AREAControl->Stotal_Module.Module11-AREAControl->S1_Module.Module11-AREAControl->S2_Module.Module11-
										AREAControl->S4_Module.Module11-AREAControl->S5_Module.Module11;
			
			if(AREAControl->VmReal_Module.Module11==0) 
				AREAControl->DeltaT3_Module.Module11 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module11 > 0)
				{
					AREAControl->DeltaT3_Module.Module11 = divdp(AREAControl->S3_Module.Module11, AREAControl->VmReal_Module.Module11);
				}
				else
				{
					AREAControl->S3_Module.Module11 = 0;
					AREAControl->DeltaT3_Module.Module11 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module11 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module11 = AREAControl->a_de_Module.Module11;
			AREAControl->Sac_Module.Module11 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module11,2) - powdp(AREAControl->Vs_Module.Module11,2)), AREAControl->a_ac_Module.Module11) );
			AREAControl->Sde_Module.Module11 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module11,2) - powdp(AREAControl->Ve_Module.Module11,2)), AREAControl->a_de_Module.Module11) );

			if(AREAControl->Stotal_Module.Module11 >= (AREAControl->Sac_Module.Module11+AREAControl->Sde_Module.Module11)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module11 = AREAControl->Vm_Module.Module11;
				AREAControl->Sm_Module.Module11 	= AREAControl->Ss_Module.Module11-AREAControl->Sac_Module.Module11-AREAControl->Sde_Module.Module11;
			}
			else if(AREAControl->Stotal_Module.Module11 >= AREAControl->Sac_Module.Module11)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module11	= AREAControl->Vm_Module.Module11;
				AREAControl->Ve_Module.Module11		= sqrtdp( powdp(AREAControl->VmReal_Module.Module11,2) - AREAControl->a_de_Module.Module11*(AREAControl->Stotal_Module.Module11-AREAControl->Sac_Module.Module11) );
				AREAControl->Sm_Module.Module11		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module11 = sqrtdp( powdp(AREAControl->Vs_Module.Module11,2) - AREAControl->a_ac_Module.Module11*AREAControl->Stotal_Module.Module11 );
				AREAControl->Ve_Module.Module11 = AREAControl->VmReal_Module.Module11;
				AREAControl->Sm_Module.Module11		= 0;				
			}

			AREAControl->DeltaT1_Module.Module11 	= divdp( fabs(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11), AREAControl->a_ac_Module.Module11);
			AREAControl->DeltaT2_Module.Module11 	= AREAControl->DeltaT1_Module.Module11;
			if(AREAControl->DeltaT1_Module.Module11 < 0.000001)
			{
				AREAControl->S1_Module.Module11	= 0;
				AREAControl->S2_Module.Module11	= 0;
				AREAControl->Sac_Module.Module11	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module11		= divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->DeltaT1_Module.Module11,3), 
										(6*(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11))) + 
										AREAControl->Vs_Module.Module11*AREAControl->DeltaT1_Module.Module11;
				AREAControl->S2_Module.Module11 	= divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->DeltaT2_Module.Module11,3), 
										(6*(AREAControl->Vs_Module.Module11-AREAControl->VmReal_Module.Module11))) +
										AREAControl->VmReal_Module.Module11*AREAControl->DeltaT2_Module.Module11;
				AREAControl->Sac_Module.Module11	= AREAControl->S1_Module.Module11 + AREAControl->S2_Module.Module11;
			}		
			
			AREAControl->DeltaT4_Module.Module11 	= divdp( fabs(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11), AREAControl->a_de_Module.Module11 );
			AREAControl->DeltaT5_Module.Module11 	= AREAControl->DeltaT4_Module.Module11;
			if(AREAControl->DeltaT4_Module.Module11 < 0.000001)
			{
				AREAControl->S4_Module.Module11 	= 0;
				AREAControl->S5_Module.Module11 	= 0;
				AREAControl->Sde_Module.Module11 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module11 	= divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp(AREAControl->DeltaT4_Module.Module11,3), 
										(6*(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11))) +
										AREAControl->VmReal_Module.Module11*AREAControl->DeltaT4_Module.Module11;
				AREAControl->S5_Module.Module11 	= divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp(AREAControl->DeltaT5_Module.Module11,3), 
										(6*(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11))) +
										AREAControl->Ve_Module.Module11*AREAControl->DeltaT5_Module.Module11;
				AREAControl->Sde_Module.Module11 	= AREAControl->S4_Module.Module11+AREAControl->S5_Module.Module11;
			}
					
			AREAControl->S3_Module.Module11 		= AREAControl->Stotal_Module.Module11-AREAControl->S1_Module.Module11-AREAControl->S2_Module.Module11-
										AREAControl->S4_Module.Module11-AREAControl->S5_Module.Module11;
			
			if(AREAControl->VmReal_Module.Module11 < 0.000001) 
				AREAControl->DeltaT3_Module.Module11 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module11 > 0)
				{
					AREAControl->DeltaT3_Module.Module11 = divdp(AREAControl->S3_Module.Module11, AREAControl->VmReal_Module.Module11);
				}
				else
				{
					AREAControl->S3_Module.Module11 = 0;
					AREAControl->DeltaT3_Module.Module11 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module11	= AREAControl->S1_Module.Module11+AREAControl->S2_Module.Module11;
		AREAControl->Sde_Module.Module11	= AREAControl->S4_Module.Module11+AREAControl->S5_Module.Module11;
		AREAControl->Sm_Module.Module11	= AREAControl->S3_Module.Module11;

		AREAControl->T1_Module.Module11 	= AREAControl->DeltaT1_Module.Module11;
		AREAControl->T2_Module.Module11 	= AREAControl->T1_Module.Module11+AREAControl->DeltaT2_Module.Module11;
		AREAControl->T3_Module.Module11 	= AREAControl->T2_Module.Module11+AREAControl->DeltaT3_Module.Module11;
		AREAControl->T4_Module.Module11 	= AREAControl->T3_Module.Module11+AREAControl->DeltaT4_Module.Module11;
		AREAControl->T5_Module.Module11 	= AREAControl->T4_Module.Module11+AREAControl->DeltaT5_Module.Module11;
		AREAControl->TotalN_Module.Module11 = divdp(AREAControl->T5_Module.Module11, System.Tsample);
		return 1;
	}
	else if(ModuleSel == 12)
	{
		if( AREAControl->SpeedState.Module12 == NCAREA_SINGLE_STATE )		//Single Gcode Area
		{
			AREAControl->Sac_Module.Module12 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module12,2) - powdp(AREAControl->Vs_Module.Module12,2)), AREAControl->a_ac_Module.Module12) );
			AREAControl->Sde_Module.Module12 	= fabs( divdp( (powdp(AREAControl->Vm_Module.Module12,2) - powdp(AREAControl->Ve_Module.Module12,2)), AREAControl->a_de_Module.Module12) );

			if(AREAControl->Stotal_Module.Module12 >= (AREAControl->Sac_Module.Module12+AREAControl->Sde_Module.Module12)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module12 = AREAControl->Vm_Module.Module12;
				AREAControl->Sm_Module.Module12 	= AREAControl->Ss_Module.Module12-AREAControl->Sac_Module.Module12-AREAControl->Sde_Module.Module12;
			}
			else
			{	
				acc = ( AREAControl->Ve_Module.Module12 >= AREAControl->Vs_Module.Module12 ) ? (AREAControl->a_ac_Module.Module12):(AREAControl->a_de_Module.Module12);
								
				if( fabs( powdp( AREAControl->Ve_Module.Module12, 2 ) - powdp( AREAControl->Vs_Module.Module12, 2 ) ) >= (acc*AREAControl->Stotal_Module.Module12) )
				{
					acc = divdp( fabs( powdp( AREAControl->Ve_Module.Module12, 2 ) - powdp( AREAControl->Vs_Module.Module12, 2 ) ), AREAControl->Stotal_Module.Module12 );

					if( AREAControl->Ve_Module.Module12 >= AREAControl->Vs_Module.Module12 )
					{
						AREAControl->VmReal_Module.Module12 = AREAControl->Ve_Module.Module12;
						AREAControl->a_ac_Module.Module12 = divdp( fabs( powdp( AREAControl->Ve_Module.Module12, 2 ) - powdp( AREAControl->Vs_Module.Module12, 2 ) ), AREAControl->Stotal_Module.Module12 );
					}
					else
					{
						AREAControl->VmReal_Module.Module12 = AREAControl->Vs_Module.Module12;

						a_de_sd = AREAControl->a_de_Module.Module12;
						AREAControl->a_de_Module.Module12 = divdp( fabs( powdp( AREAControl->Ve_Module.Module12, 2 ) - powdp( AREAControl->Vs_Module.Module12, 2 ) ), AREAControl->Stotal_Module.Module12 );
						if(AREAControl->a_de_Module.Module12 > 1.2*a_de_sd )
						{
							AREAControl->a_de_Module.Module12 = 1.2*a_de_sd;
							AREAControl->Ve_Module.Module12 = sqrtdp( powdp(AREAControl->Vs_Module.Module12,2) - AREAControl->a_de_Module.Module12*AREAControl->Stotal_Module.Module12 );
						}
					}
				}
				else
				{
					AREAControl->VmReal_Module.Module12 = sqrtdp( divdp( ( AREAControl->a_ac_Module.Module12 * AREAControl->a_de_Module.Module12 * AREAControl->Stotal_Module.Module12 + 
												AREAControl->a_de_Module.Module12 * powdp( AREAControl->Vs_Module.Module12, 2 ) + AREAControl->a_ac_Module.Module12 * 
												powdp( AREAControl->Ve_Module.Module12, 2 ) ),  ( AREAControl->a_ac_Module.Module12 + AREAControl->a_de_Module.Module12 ) ) );
					AREAControl->Sm_Module.Module12		= 0;	
				}
			}	

			AREAControl->DeltaT1_Module.Module12 	= divdp( fabs(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12), AREAControl->a_ac_Module.Module12);
			AREAControl->DeltaT2_Module.Module12 	= AREAControl->DeltaT1_Module.Module12;
			if(AREAControl->DeltaT1_Module.Module12 == 0)
			{
				AREAControl->S1_Module.Module12		= 0;
				AREAControl->S2_Module.Module12		= 0;
				AREAControl->Sac_Module.Module12	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module12		= divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->DeltaT1_Module.Module12,3), 
										(6*(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12))) + 
										AREAControl->Vs_Module.Module12*AREAControl->DeltaT1_Module.Module12;
				AREAControl->S2_Module.Module12 	= divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->DeltaT2_Module.Module12,3), 
										(6*(AREAControl->Vs_Module.Module12-AREAControl->VmReal_Module.Module12))) +
										AREAControl->VmReal_Module.Module12*AREAControl->DeltaT2_Module.Module12;
				AREAControl->Sac_Module.Module12	= AREAControl->S1_Module.Module12 + AREAControl->S2_Module.Module12;
			}		
			
			AREAControl->DeltaT4_Module.Module12 	= divdp( fabs(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12), AREAControl->a_de_Module.Module12 );
			AREAControl->DeltaT5_Module.Module12 	= AREAControl->DeltaT4_Module.Module12;
			if(AREAControl->DeltaT4_Module.Module12 == 0)
			{
				AREAControl->S4_Module.Module12 	= 0;
				AREAControl->S5_Module.Module12 	= 0;
				AREAControl->Sde_Module.Module12 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module12 	= divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp(AREAControl->DeltaT4_Module.Module12,3), 
										(6*(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12))) +
										AREAControl->VmReal_Module.Module12*AREAControl->DeltaT4_Module.Module12;
				AREAControl->S5_Module.Module12 	= divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp(AREAControl->DeltaT5_Module.Module12,3), 
										(6*(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12))) +
										AREAControl->Ve_Module.Module12*AREAControl->DeltaT5_Module.Module12;
				AREAControl->Sde_Module.Module12 	= AREAControl->S4_Module.Module12+AREAControl->S5_Module.Module12;
			}
					
			AREAControl->S3_Module.Module12 		= AREAControl->Stotal_Module.Module12-AREAControl->S1_Module.Module12-AREAControl->S2_Module.Module12-
										AREAControl->S4_Module.Module12-AREAControl->S5_Module.Module12;
			
			if(AREAControl->VmReal_Module.Module12==0) 
				AREAControl->DeltaT3_Module.Module12 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module12 > 0)
				{
					AREAControl->DeltaT3_Module.Module12 = divdp(AREAControl->S3_Module.Module12, AREAControl->VmReal_Module.Module12);
				}
				else
				{
					AREAControl->S3_Module.Module12 = 0;
					AREAControl->DeltaT3_Module.Module12 = 0;
				}
			}
		}
		else if( AREAControl->SpeedState.Module12 == NCAREA_SPECIAL_STATE )		//如果Vs>Vm，那么应该让区域速度急速减到Vm
		{
			//这里的Sac和S1,S2都认为是减速段，所以加速度都取a_de
			AREAControl->a_ac_Module.Module12 = AREAControl->a_de_Module.Module12;
			AREAControl->Sac_Module.Module12 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module12,2) - powdp(AREAControl->Vs_Module.Module12,2)), AREAControl->a_ac_Module.Module12) );
			AREAControl->Sde_Module.Module12 		= fabs( divdp( (powdp(AREAControl->Vm_Module.Module12,2) - powdp(AREAControl->Ve_Module.Module12,2)), AREAControl->a_de_Module.Module12) );

			if(AREAControl->Stotal_Module.Module12 >= (AREAControl->Sac_Module.Module12+AREAControl->Sde_Module.Module12)) 		//这里不能用Ss，要用Stotal
			{	// 有加速段和减速段
				AREAControl->VmReal_Module.Module12 = AREAControl->Vm_Module.Module12;
				AREAControl->Sm_Module.Module12 	= AREAControl->Ss_Module.Module12-AREAControl->Sac_Module.Module12-AREAControl->Sde_Module.Module12;
			}
			else if(AREAControl->Stotal_Module.Module12 >= AREAControl->Sac_Module.Module12)
			{	//有加速段，减速段不完整
				AREAControl->VmReal_Module.Module12	= AREAControl->Vm_Module.Module12;
				AREAControl->Ve_Module.Module12		= sqrtdp( powdp(AREAControl->VmReal_Module.Module12,2) - AREAControl->a_de_Module.Module12*(AREAControl->Stotal_Module.Module12-AREAControl->Sac_Module.Module12) );
				AREAControl->Sm_Module.Module12		= 0;
			}
			else
			{	//没有减速段，加速段也不完整
				AREAControl->VmReal_Module.Module12 = sqrtdp( powdp(AREAControl->Vs_Module.Module12,2) - AREAControl->a_ac_Module.Module12*AREAControl->Stotal_Module.Module12 );
				AREAControl->Ve_Module.Module12 = AREAControl->VmReal_Module.Module12;
				AREAControl->Sm_Module.Module12		= 0;				
			}

			AREAControl->DeltaT1_Module.Module12 	= divdp( fabs(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12), AREAControl->a_ac_Module.Module12);
			AREAControl->DeltaT2_Module.Module12 	= AREAControl->DeltaT1_Module.Module12;
			if(AREAControl->DeltaT1_Module.Module12 < 0.000001)
			{
				AREAControl->S1_Module.Module12	= 0;
				AREAControl->S2_Module.Module12	= 0;
				AREAControl->Sac_Module.Module12	= 0;
			}
			else
			{			
				AREAControl->S1_Module.Module12		= divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->DeltaT1_Module.Module12,3), 
										(6*(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12))) + 
										AREAControl->Vs_Module.Module12*AREAControl->DeltaT1_Module.Module12;
				AREAControl->S2_Module.Module12 	= divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->DeltaT2_Module.Module12,3), 
										(6*(AREAControl->Vs_Module.Module12-AREAControl->VmReal_Module.Module12))) +
										AREAControl->VmReal_Module.Module12*AREAControl->DeltaT2_Module.Module12;
				AREAControl->Sac_Module.Module12	= AREAControl->S1_Module.Module12 + AREAControl->S2_Module.Module12;
			}		
			
			AREAControl->DeltaT4_Module.Module12 	= divdp( fabs(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12), AREAControl->a_de_Module.Module12 );
			AREAControl->DeltaT5_Module.Module12 	= AREAControl->DeltaT4_Module.Module12;
			if(AREAControl->DeltaT4_Module.Module12 < 0.000001)
			{
				AREAControl->S4_Module.Module12 	= 0;
				AREAControl->S5_Module.Module12 	= 0;
				AREAControl->Sde_Module.Module12 	= 0;
			}
			else
			{	
				AREAControl->S4_Module.Module12 	= divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp(AREAControl->DeltaT4_Module.Module12,3), 
										(6*(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12))) +
										AREAControl->VmReal_Module.Module12*AREAControl->DeltaT4_Module.Module12;
				AREAControl->S5_Module.Module12 	= divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp(AREAControl->DeltaT5_Module.Module12,3), 
										(6*(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12))) +
										AREAControl->Ve_Module.Module12*AREAControl->DeltaT5_Module.Module12;
				AREAControl->Sde_Module.Module12 	= AREAControl->S4_Module.Module12+AREAControl->S5_Module.Module12;
			}
					
			AREAControl->S3_Module.Module12 		= AREAControl->Stotal_Module.Module12-AREAControl->S1_Module.Module12-AREAControl->S2_Module.Module12-
										AREAControl->S4_Module.Module12-AREAControl->S5_Module.Module12;
			
			if(AREAControl->VmReal_Module.Module12 < 0.000001) 
				AREAControl->DeltaT3_Module.Module12 = 3e38;		//趋于无穷
			else
			{
				if(AREAControl->S3_Module.Module12 > 0)
				{
					AREAControl->DeltaT3_Module.Module12 = divdp(AREAControl->S3_Module.Module12, AREAControl->VmReal_Module.Module12);
				}
				else
				{
					AREAControl->S3_Module.Module12 = 0;
					AREAControl->DeltaT3_Module.Module12 = 0;
				}
			}
		}
		
		AREAControl->Sac_Module.Module12	= AREAControl->S1_Module.Module12+AREAControl->S2_Module.Module12;
		AREAControl->Sde_Module.Module12	= AREAControl->S4_Module.Module12+AREAControl->S5_Module.Module12;
		AREAControl->Sm_Module.Module12	= AREAControl->S3_Module.Module12;

		AREAControl->T1_Module.Module12 	= AREAControl->DeltaT1_Module.Module12;
		AREAControl->T2_Module.Module12 	= AREAControl->T1_Module.Module12+AREAControl->DeltaT2_Module.Module12;
		AREAControl->T3_Module.Module12 	= AREAControl->T2_Module.Module12+AREAControl->DeltaT3_Module.Module12;
		AREAControl->T4_Module.Module12 	= AREAControl->T3_Module.Module12+AREAControl->DeltaT4_Module.Module12;
		AREAControl->T5_Module.Module12 	= AREAControl->T4_Module.Module12+AREAControl->DeltaT5_Module.Module12;
		AREAControl->TotalN_Module.Module12 = divdp(AREAControl->T5_Module.Module12, System.Tsample);
		return 1;
	}

	
}


// 区域曲线运行预处理
Uint16 NCAreaRunPre(AREA_CONTROL *AREAControl, GCODE * AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign) 
{     
	Int32 delta_value;
	INT40AXIS 	EndPointMachValue;
	Uint32 ri;
	static Uint32 add=1;
	INT40AXIS PointMACH;
	
	GCODE *psAreaGcodeBufferPointer;
	
	ri = (AreaRunNCSign->RealRunCount-1) % STDGCODE_MOD;

	psAreaGcodeBufferPointer = (AreaGcodeBufferPointer+ri);
	

	if(AreaRunNCSign->NewAreaSign == 0)
	{	//下一个区域曲线预处理
		AREAControl->Snow_Module.Module1 = 0;
		AREAControl->Snow_Module.Module2 = 0;
		AREAControl->Snow_Module.Module3 = 0;
		AREAControl->Snow_Module.Module4 = 0;
		AREAControl->Snow_Module.Module5 = 0;
		AREAControl->Snow_Module.Module6 = 0;
		AREAControl->Snow_Module.Module7 = 0;
		AREAControl->Snow_Module.Module8 = 0;
		AREAControl->Snow_Module.Module9 = 0;
		AREAControl->Snow_Module.Module10 = 0;
		AREAControl->Snow_Module.Module11 = 0;
		AREAControl->Snow_Module.Module12 = 0;
		
		AREAControl->SnowLast_Module.Module1 = 0;
		AREAControl->SnowLast_Module.Module2 = 0;
		AREAControl->SnowLast_Module.Module3 = 0;
		AREAControl->SnowLast_Module.Module4 = 0;
		AREAControl->SnowLast_Module.Module5 = 0;
		AREAControl->SnowLast_Module.Module6 = 0;
		AREAControl->SnowLast_Module.Module7 = 0;
		AREAControl->SnowLast_Module.Module8 = 0;
		AREAControl->SnowLast_Module.Module9 = 0;
		AREAControl->SnowLast_Module.Module10 = 0;
		AREAControl->SnowLast_Module.Module11 = 0;
		AREAControl->SnowLast_Module.Module12 = 0;
		
		NCAreaControl(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);

		AREAControl->I = 1;
		AREAControl->I_Module.Module1 = 1;
		AREAControl->I_Module.Module2 = 1;
		AREAControl->I_Module.Module3 = 1;
		AREAControl->I_Module.Module4 = 1;
		AREAControl->I_Module.Module5 = 1;
		AREAControl->I_Module.Module6 = 1;
		AREAControl->I_Module.Module7 = 1;
		AREAControl->I_Module.Module8 = 1;
		AREAControl->I_Module.Module9 = 1;
		AREAControl->I_Module.Module10 = 1;
		AREAControl->I_Module.Module11 = 1;
		AREAControl->I_Module.Module12 = 1;
		
		//AREAControl->CurrentStotal = 0;
		AREAControl->CurrentStotal_Module.Module1 = 0;
		AREAControl->CurrentStotal_Module.Module2 = 0;
		AREAControl->CurrentStotal_Module.Module3 = 0;
		AREAControl->CurrentStotal_Module.Module4 = 0;
		AREAControl->CurrentStotal_Module.Module5 = 0;
		AREAControl->CurrentStotal_Module.Module6 = 0;
		AREAControl->CurrentStotal_Module.Module7 = 0;
		AREAControl->CurrentStotal_Module.Module8 = 0;
		AREAControl->CurrentStotal_Module.Module9 = 0;
		AREAControl->CurrentStotal_Module.Module10 = 0;
		AREAControl->CurrentStotal_Module.Module11 = 0;
		AREAControl->CurrentStotal_Module.Module12 = 0;
		
		AreaRunNCSign->NewAreaSign = 1;
	}
	
	//psAreaGcodeBufferPointer->Stotal = psAreaGcodeBufferPointer->Stotal_Module.Module1+psAreaGcodeBufferPointer->Stotal_Module.Module2+psAreaGcodeBufferPointer->Stotal_Module.Module3+
	//							psAreaGcodeBufferPointer->Stotal_Module.Module4+psAreaGcodeBufferPointer->Stotal_Module.Module5+psAreaGcodeBufferPointer->Stotal_Module.Module6+
	//							psAreaGcodeBufferPointer->Stotal_Module.Module7+psAreaGcodeBufferPointer->Stotal_Module.Module8+psAreaGcodeBufferPointer->Stotal_Module.Module9+
	//							psAreaGcodeBufferPointer->Stotal_Module.Module10+psAreaGcodeBufferPointer->Stotal_Module.Module11+psAreaGcodeBufferPointer->Stotal_Module.Module12;
	
	if(psAreaGcodeBufferPointer->Stotal==0) 
	{
		AreaRunNCSign->CodeRun_Over = 1;
		return 0;
	}

	if(psAreaGcodeBufferPointer->Stotal_Module.Module1>0)
	{
		AREAControl->CurrentStotalLast_Module.Module1 = AREAControl->CurrentStotal_Module.Module1;
		AREAControl->CurrentStotal_Module.Module1 += psAreaGcodeBufferPointer->Stotal_Module.Module1;

		AREAControl->Cosine.Axis15 = (psAreaGcodeBufferPointer->EndPoint.Axis15-psAreaGcodeBufferPointer->StartPoint.Axis15)*System.UnitTo_mm;
		AREAControl->Cosine.Axis15 = divdp(AREAControl->Cosine.Axis15,psAreaGcodeBufferPointer->Stotal_Module.Module1);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis16 = (psAreaGcodeBufferPointer->EndPoint.Axis16-psAreaGcodeBufferPointer->StartPoint.Axis16)*System.UnitTo_mm;
		AREAControl->Cosine.Axis16 = divdp(AREAControl->Cosine.Axis16,psAreaGcodeBufferPointer->Stotal_Module.Module1);		//分开,防止浮点运算出错
		AREAControl->Cosine.Axis1 = (psAreaGcodeBufferPointer->EndPoint.Axis1-psAreaGcodeBufferPointer->StartPoint.Axis1)*System.UnitTo_mm;
		AREAControl->Cosine.Axis1 = divdp(AREAControl->Cosine.Axis1,psAreaGcodeBufferPointer->Stotal_Module.Module1);	//分开,防止浮点运算出错
		//AREAControl->Cosine.Axis2= (psAreaGcodeBufferPointer->EndPoint.Axis2-psAreaGcodeBufferPointer->StartPoint.Axis2)*System.UnitTo_mm;
		//AREAControl->Cosine.Axis2= divdp(AREAControl->Cosine.Axis2,psAreaGcodeBufferPointer->Stotal_Module.Module1);	//分开,防止浮点运算出错
	}
	else
	{
		//防止缓冲区数据冲掉
		AREAControl->CurrentStotalLast_Module.Module1 = AREAControl->CurrentStotal_Module.Module1;
		AREAControl->CurrentStotal_Module.Module1 += psAreaGcodeBufferPointer->Stotal_Module.Module1;

		AREAControl->Cosine.Axis15 = 0;
		AREAControl->Cosine.Axis16 = 0;
		AREAControl->Cosine.Axis1 = 0;
		//AREAControl->Cosine.Axis2= 0;
	}

	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module2>0)
	{
		AREAControl->CurrentStotalLast_Module.Module2 = AREAControl->CurrentStotal_Module.Module2;
		AREAControl->CurrentStotal_Module.Module2 += psAreaGcodeBufferPointer->Stotal_Module.Module2;

		AREAControl->Cosine.Axis17 = (psAreaGcodeBufferPointer->EndPoint.Axis17-psAreaGcodeBufferPointer->StartPoint.Axis17)*System.UnitTo_mm;
		AREAControl->Cosine.Axis17 = divdp(AREAControl->Cosine.Axis17,psAreaGcodeBufferPointer->Stotal_Module.Module2);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis18 = (psAreaGcodeBufferPointer->EndPoint.Axis18-psAreaGcodeBufferPointer->StartPoint.Axis18)*System.UnitTo_mm;
		AREAControl->Cosine.Axis18 = divdp(AREAControl->Cosine.Axis18,psAreaGcodeBufferPointer->Stotal_Module.Module2);		//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module2 = AREAControl->CurrentStotal_Module.Module2;
		AREAControl->CurrentStotal_Module.Module2 += psAreaGcodeBufferPointer->Stotal_Module.Module2;

		AREAControl->Cosine.Axis17 = 0;
		AREAControl->Cosine.Axis18 = 0;
	}

	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module3>0)
	{
		AREAControl->CurrentStotalLast_Module.Module3 = AREAControl->CurrentStotal_Module.Module3;
		AREAControl->CurrentStotal_Module.Module3 += psAreaGcodeBufferPointer->Stotal_Module.Module3;

		AREAControl->Cosine.Axis3 = (psAreaGcodeBufferPointer->EndPoint.Axis3-psAreaGcodeBufferPointer->StartPoint.Axis3)*System.UnitTo_mm;
		AREAControl->Cosine.Axis3 = divdp(AREAControl->Cosine.Axis3,psAreaGcodeBufferPointer->Stotal_Module.Module3);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis4 = (psAreaGcodeBufferPointer->EndPoint.Axis4-psAreaGcodeBufferPointer->StartPoint.Axis4)*System.UnitTo_mm;
		AREAControl->Cosine.Axis4 = divdp(AREAControl->Cosine.Axis4,psAreaGcodeBufferPointer->Stotal_Module.Module3);		//分开,防止浮点运算出错
		AREAControl->Cosine.Axis5 = (psAreaGcodeBufferPointer->EndPoint.Axis5-psAreaGcodeBufferPointer->StartPoint.Axis5)*System.UnitTo_mm;
		AREAControl->Cosine.Axis5 = divdp(AREAControl->Cosine.Axis5,psAreaGcodeBufferPointer->Stotal_Module.Module3);	//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module3 = AREAControl->CurrentStotal_Module.Module3;
		AREAControl->CurrentStotal_Module.Module3 += psAreaGcodeBufferPointer->Stotal_Module.Module3;

		AREAControl->Cosine.Axis3 = 0;
		AREAControl->Cosine.Axis4 = 0;
		AREAControl->Cosine.Axis5 = 0;
	}

	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module4>0)
	{
		AREAControl->CurrentStotalLast_Module.Module4 = AREAControl->CurrentStotal_Module.Module4;
		AREAControl->CurrentStotal_Module.Module4 += psAreaGcodeBufferPointer->Stotal_Module.Module4;

		AREAControl->Cosine.Axis8 = (psAreaGcodeBufferPointer->EndPoint.Axis8-psAreaGcodeBufferPointer->StartPoint.Axis8)*System.UnitTo_mm;
		AREAControl->Cosine.Axis8 = divdp(AREAControl->Cosine.Axis8,psAreaGcodeBufferPointer->Stotal_Module.Module4);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis7= (psAreaGcodeBufferPointer->EndPoint.Axis7-psAreaGcodeBufferPointer->StartPoint.Axis7)*System.UnitTo_mm;
		AREAControl->Cosine.Axis7= divdp(AREAControl->Cosine.Axis7,psAreaGcodeBufferPointer->Stotal_Module.Module4);	//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module4 = AREAControl->CurrentStotal_Module.Module4;
		AREAControl->CurrentStotal_Module.Module4 += psAreaGcodeBufferPointer->Stotal_Module.Module4;

		AREAControl->Cosine.Axis8 = 0;
		AREAControl->Cosine.Axis7 = 0;
	}
	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module5>0)
	{	
		AREAControl->CurrentStotalLast_Module.Module5 = AREAControl->CurrentStotal_Module.Module5;
		AREAControl->CurrentStotal_Module.Module5 += psAreaGcodeBufferPointer->Stotal_Module.Module5;

		AREAControl->Cosine.Axis6 = (psAreaGcodeBufferPointer->EndPoint.Axis6-psAreaGcodeBufferPointer->StartPoint.Axis6)*System.UnitTo_mm;
		AREAControl->Cosine.Axis6 = divdp(AREAControl->Cosine.Axis6,psAreaGcodeBufferPointer->Stotal_Module.Module5);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis20 = (psAreaGcodeBufferPointer->EndPoint.Axis20-psAreaGcodeBufferPointer->StartPoint.Axis20)*System.UnitTo_mm;
		AREAControl->Cosine.Axis20 = divdp(AREAControl->Cosine.Axis20,psAreaGcodeBufferPointer->Stotal_Module.Module5);		//分开,防止浮点运算出错
	}
	else
	{	
		AREAControl->CurrentStotalLast_Module.Module5 = AREAControl->CurrentStotal_Module.Module5;
		AREAControl->CurrentStotal_Module.Module5 += psAreaGcodeBufferPointer->Stotal_Module.Module5;

		AREAControl->Cosine.Axis6 = 0;
		AREAControl->Cosine.Axis20 = 0;
	}

	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module6>0)
	{
		AREAControl->CurrentStotalLast_Module.Module6 = AREAControl->CurrentStotal_Module.Module6;
		AREAControl->CurrentStotal_Module.Module6 += psAreaGcodeBufferPointer->Stotal_Module.Module6;

		AREAControl->Cosine.Axis11 = (psAreaGcodeBufferPointer->EndPoint.Axis11-psAreaGcodeBufferPointer->StartPoint.Axis11)*System.UnitTo_mm;
		AREAControl->Cosine.Axis11 = divdp(AREAControl->Cosine.Axis11,psAreaGcodeBufferPointer->Stotal_Module.Module6);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis12= (psAreaGcodeBufferPointer->EndPoint.Axis12-psAreaGcodeBufferPointer->StartPoint.Axis12)*System.UnitTo_mm;
		AREAControl->Cosine.Axis12= divdp(AREAControl->Cosine.Axis12,psAreaGcodeBufferPointer->Stotal_Module.Module6);	//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module6 = AREAControl->CurrentStotal_Module.Module6;
		AREAControl->CurrentStotal_Module.Module6 += psAreaGcodeBufferPointer->Stotal_Module.Module6;

		AREAControl->Cosine.Axis11 = 0;
		AREAControl->Cosine.Axis12 = 0;
	}
	if(psAreaGcodeBufferPointer->Stotal_Module.Module7>0)
	{	
		AREAControl->CurrentStotalLast_Module.Module7 = AREAControl->CurrentStotal_Module.Module7;
		AREAControl->CurrentStotal_Module.Module7 += psAreaGcodeBufferPointer->Stotal_Module.Module7;

		AREAControl->Cosine.Axis21 = (psAreaGcodeBufferPointer->EndPoint.Axis21-psAreaGcodeBufferPointer->StartPoint.Axis21)*System.UnitTo_mm;
		AREAControl->Cosine.Axis21 = divdp(AREAControl->Cosine.Axis21,psAreaGcodeBufferPointer->Stotal_Module.Module7);		//分开,防止浮点运算出错
		AREAControl->Cosine.Axis22 = (psAreaGcodeBufferPointer->EndPoint.Axis22-psAreaGcodeBufferPointer->StartPoint.Axis22)*System.UnitTo_mm;
		AREAControl->Cosine.Axis22 = divdp(AREAControl->Cosine.Axis22,psAreaGcodeBufferPointer->Stotal_Module.Module7);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis9 = (psAreaGcodeBufferPointer->EndPoint.Axis9-psAreaGcodeBufferPointer->StartPoint.Axis9)*System.UnitTo_mm;
		AREAControl->Cosine.Axis9 = divdp(AREAControl->Cosine.Axis9,psAreaGcodeBufferPointer->Stotal_Module.Module7);	//分开,防止浮点运算出错
	}
	else
	{	
		AREAControl->CurrentStotalLast_Module.Module7 = AREAControl->CurrentStotal_Module.Module7;
		AREAControl->CurrentStotal_Module.Module7 += psAreaGcodeBufferPointer->Stotal_Module.Module7;

		AREAControl->Cosine.Axis21 = 0;
		AREAControl->Cosine.Axis22 = 0;
		AREAControl->Cosine.Axis9 = 0;
	}	

	if(psAreaGcodeBufferPointer->Stotal_Module.Module8>0)
	{
		AREAControl->CurrentStotalLast_Module.Module8 = AREAControl->CurrentStotal_Module.Module8;
		AREAControl->CurrentStotal_Module.Module8 += psAreaGcodeBufferPointer->Stotal_Module.Module8;
		AREAControl->Cosine.Axis2= (psAreaGcodeBufferPointer->EndPoint.Axis2-psAreaGcodeBufferPointer->StartPoint.Axis2)*System.UnitTo_mm;
		AREAControl->Cosine.Axis2= divdp(AREAControl->Cosine.Axis2,psAreaGcodeBufferPointer->Stotal_Module.Module8);	//分开,防止浮点运算出错
	}
	else
	{
		//防止缓冲区数据冲掉
		AREAControl->CurrentStotalLast_Module.Module8 = AREAControl->CurrentStotal_Module.Module8;
		AREAControl->CurrentStotal_Module.Module8 += psAreaGcodeBufferPointer->Stotal_Module.Module8;
		AREAControl->Cosine.Axis2= 0;
	}
	
	if(psAreaGcodeBufferPointer->Stotal_Module.Module9>0)
	{	
		AREAControl->CurrentStotalLast_Module.Module9 = AREAControl->CurrentStotal_Module.Module9;
		AREAControl->CurrentStotal_Module.Module9 += psAreaGcodeBufferPointer->Stotal_Module.Module9;

		AREAControl->Cosine.Axis10 = (psAreaGcodeBufferPointer->EndPoint.Axis10-psAreaGcodeBufferPointer->StartPoint.Axis10)*System.UnitTo_mm;
		AREAControl->Cosine.Axis10 = divdp(AREAControl->Cosine.Axis10,psAreaGcodeBufferPointer->Stotal_Module.Module9);	//分开,防止浮点运算出错
	}
	else
	{	
		AREAControl->CurrentStotalLast_Module.Module9 = AREAControl->CurrentStotal_Module.Module9;
		AREAControl->CurrentStotal_Module.Module9 += psAreaGcodeBufferPointer->Stotal_Module.Module9;
	
		AREAControl->Cosine.Axis10 = 0;
		
	}

	if(psAreaGcodeBufferPointer->Stotal_Module.Module10>0)
	{
		AREAControl->CurrentStotalLast_Module.Module10 = AREAControl->CurrentStotal_Module.Module10;
		AREAControl->CurrentStotal_Module.Module10 += psAreaGcodeBufferPointer->Stotal_Module.Module10;

		AREAControl->Cosine.Axis13 = (psAreaGcodeBufferPointer->EndPoint.Axis13-psAreaGcodeBufferPointer->StartPoint.Axis13)*System.UnitTo_mm;
		AREAControl->Cosine.Axis13 = divdp(AREAControl->Cosine.Axis13,psAreaGcodeBufferPointer->Stotal_Module.Module10);	//分开,防止浮点运算出错
		AREAControl->Cosine.Axis14 = (psAreaGcodeBufferPointer->EndPoint.Axis14-psAreaGcodeBufferPointer->StartPoint.Axis14)*System.UnitTo_mm;
		AREAControl->Cosine.Axis14 = divdp(AREAControl->Cosine.Axis14,psAreaGcodeBufferPointer->Stotal_Module.Module10);		//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module10 = AREAControl->CurrentStotal_Module.Module10;
		AREAControl->CurrentStotal_Module.Module10 += psAreaGcodeBufferPointer->Stotal_Module.Module10;

		AREAControl->Cosine.Axis13 = 0;
		AREAControl->Cosine.Axis14 = 0;
	}

	if(psAreaGcodeBufferPointer->Stotal_Module.Module11>0)
	{
		AREAControl->CurrentStotalLast_Module.Module11 = AREAControl->CurrentStotal_Module.Module11;
		AREAControl->CurrentStotal_Module.Module11 += psAreaGcodeBufferPointer->Stotal_Module.Module11;
		AREAControl->Cosine.Axis19 = (psAreaGcodeBufferPointer->EndPoint.Axis19-psAreaGcodeBufferPointer->StartPoint.Axis19)*System.UnitTo_mm;
		AREAControl->Cosine.Axis19 = divdp(AREAControl->Cosine.Axis19,psAreaGcodeBufferPointer->Stotal_Module.Module11);		//分开,防止浮点运算出错
		//OverallSign.LensMotionOverSign = LensMotionCompensationRun;
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module11 = AREAControl->CurrentStotal_Module.Module11;
		AREAControl->CurrentStotal_Module.Module11 += psAreaGcodeBufferPointer->Stotal_Module.Module11;
		AREAControl->Cosine.Axis19 = 0;
		//OverallSign.LensMotionOverSign = LensMotionNormal;
	}

	if(psAreaGcodeBufferPointer->Stotal_Module.Module12>0)
	{
		AREAControl->CurrentStotalLast_Module.Module12 = AREAControl->CurrentStotal_Module.Module12;
		AREAControl->CurrentStotal_Module.Module12 += psAreaGcodeBufferPointer->Stotal_Module.Module12;
		AREAControl->Cosine.Axis23 = (psAreaGcodeBufferPointer->EndPoint.Axis23-psAreaGcodeBufferPointer->StartPoint.Axis23)*System.UnitTo_mm;
		AREAControl->Cosine.Axis23 = divdp(AREAControl->Cosine.Axis23,psAreaGcodeBufferPointer->Stotal_Module.Module12);		//分开,防止浮点运算出错
	}
	else
	{
		AREAControl->CurrentStotalLast_Module.Module12 = AREAControl->CurrentStotal_Module.Module12;
		AREAControl->CurrentStotal_Module.Module12 += psAreaGcodeBufferPointer->Stotal_Module.Module12;
		AREAControl->Cosine.Axis23 = 0;
	}

	AREAControl->Cosine.Axis24 = 0;
	AREAControl->Cosine.Axis25 = 0;

	AREAControl->EndPointMachPulseOut = ABSCoordinate_to_MachineCoordinate(psAreaGcodeBufferPointer->EndPoint);
	//for test Compensation
	AREAControl->EndPointMachPulseOut = nonlinear_pulse_out(AREAControl->EndPointMachPulseOut);

	AREAControl->EndPointMachPulseOut.Axis1 *= Servo_Axis1.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis2 *= Servo_Axis2.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis3 *= Servo_Axis3.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis4 *= Servo_Axis4.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis5 *= Servo_Axis5.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis6 *= Servo_Axis6.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis7 *= Servo_Axis7.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis8 *= Servo_Axis8.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis9 *= Servo_Axis9.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis10 *= Servo_Axis10.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis11 *= Servo_Axis11.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis12 *= Servo_Axis12.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis13 *= Servo_Axis13.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis14 *= Servo_Axis14.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis15 *= Servo_Axis15.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis16 *= Servo_Axis16.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis17 *= Servo_Axis17.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis18 *= Servo_Axis18.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis19 *= Servo_Axis19.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis20 *= Servo_Axis20.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis21 *= Servo_Axis21.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis22 *= Servo_Axis22.AxisOutUnitEQU; //for test
	AREAControl->EndPointMachPulseOut.Axis23 *= Servo_Axis23.AxisOutUnitEQU; //for test

	delta_value = AREAControl->EndPointMachPulseOut.Axis1-AREAControl->OldPointMACHPulseOut.Axis1;
	if((OverallSign.MoveSign.Axis1==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis1 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis1==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis1 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis1 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis1 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis2-AREAControl->OldPointMACHPulseOut.Axis2;
	if((OverallSign.MoveSign.Axis2==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis2 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis2==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis2 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis2= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis2 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis3-AREAControl->OldPointMACHPulseOut.Axis3;
	if((OverallSign.MoveSign.Axis3==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis3 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis3==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis3 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis3 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis3 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis4-AREAControl->OldPointMACHPulseOut.Axis4;
	if((OverallSign.MoveSign.Axis4==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis4 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis4==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis4 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis4= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis4= 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis5-AREAControl->OldPointMACHPulseOut.Axis5;
	if((OverallSign.MoveSign.Axis5==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis5 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis5==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis5 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis5 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis5 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis6-AREAControl->OldPointMACHPulseOut.Axis6;
	if((OverallSign.MoveSign.Axis6==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis6 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis6==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis6 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis6= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis6 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis7-AREAControl->OldPointMACHPulseOut.Axis7;
	if((OverallSign.MoveSign.Axis7==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis7 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis7==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis7 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis7 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis7 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis8-AREAControl->OldPointMACHPulseOut.Axis8;
	if((OverallSign.MoveSign.Axis8==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis8 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis8==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis8 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis8= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis8= 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis9-AREAControl->OldPointMACHPulseOut.Axis9;
	if((OverallSign.MoveSign.Axis9==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis9 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis9==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis9 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis9 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis9 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis10-AREAControl->OldPointMACHPulseOut.Axis10;
	if((OverallSign.MoveSign.Axis10==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis10 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis10==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis10 = 2;		//negtive gap compensation
	}	
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis10= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis10 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis11-AREAControl->OldPointMACHPulseOut.Axis11;
	if((OverallSign.MoveSign.Axis11==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis11 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis11==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis11 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis11 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis11 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis12-AREAControl->OldPointMACHPulseOut.Axis12;
	if((OverallSign.MoveSign.Axis12==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis12 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis12==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis12 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis12= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis12 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis13-AREAControl->OldPointMACHPulseOut.Axis13;
	if((OverallSign.MoveSign.Axis13==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis13 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis13==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis13 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis13 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis13 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis14-AREAControl->OldPointMACHPulseOut.Axis14;
	if((OverallSign.MoveSign.Axis14==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis14 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis14==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis14 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis14= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis14= 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis15-AREAControl->OldPointMACHPulseOut.Axis15;
	if((OverallSign.MoveSign.Axis15==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis15 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis15==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis15 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis15 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis15 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis16-AREAControl->OldPointMACHPulseOut.Axis16;
	if((OverallSign.MoveSign.Axis16==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis16 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis16==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis16 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis16= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis16 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis17-AREAControl->OldPointMACHPulseOut.Axis17;
	if((OverallSign.MoveSign.Axis17==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis17 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis17==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis17 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis17 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis17 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis18-AREAControl->OldPointMACHPulseOut.Axis18;
	if((OverallSign.MoveSign.Axis18==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis18 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis18==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis18 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis18= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis18= 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis19-AREAControl->OldPointMACHPulseOut.Axis19;
	if((OverallSign.MoveSign.Axis19==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis19 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis19==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis19 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis19 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis19 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis20-AREAControl->OldPointMACHPulseOut.Axis20;
	if((OverallSign.MoveSign.Axis20==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis20 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis20==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis20 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis20= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis20 = 0;
	
	delta_value = AREAControl->EndPointMachPulseOut.Axis21-AREAControl->OldPointMACHPulseOut.Axis21;
	if((OverallSign.MoveSign.Axis21==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis21 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis21==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis21 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis21 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis21 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis22-AREAControl->OldPointMACHPulseOut.Axis22;
	if((OverallSign.MoveSign.Axis22==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis22 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis22==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis22 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis22= 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis22 = 0;

	delta_value = AREAControl->EndPointMachPulseOut.Axis23-AREAControl->OldPointMACHPulseOut.Axis23;
	if((OverallSign.MoveSign.Axis23==0)&&(delta_value>=1))
	{
		AreaRunNCSign->NeedBack.Axis23 = 1;		//positive gap compensation
	}	
	if((OverallSign.MoveSign.Axis23==1)&&(delta_value<=-1))
	{
		AreaRunNCSign->NeedBack.Axis23 = 2;		//negtive gap compensation
	}
	if(delta_value>=1)
		OverallSign.TheoryMoveSign.Axis23 = 1;
	if(delta_value<=-1)
		OverallSign.TheoryMoveSign.Axis23 = 0;

	if(System.Gap.Axis1 == 0)
	{
		AreaRunNCSign->NeedBack.Axis1 = 0;
	}
	if(System.Gap.Axis2 == 0)
	{
		AreaRunNCSign->NeedBack.Axis2 = 0;
	}
	if(System.Gap.Axis3 == 0)
	{
		AreaRunNCSign->NeedBack.Axis3 = 0;
	}
	if(System.Gap.Axis4 == 0)
	{
		AreaRunNCSign->NeedBack.Axis4 = 0;
	}
	if(System.Gap.Axis5 == 0)
	{
		AreaRunNCSign->NeedBack.Axis5 = 0;
	}
	if(System.Gap.Axis6 == 0)
	{
		AreaRunNCSign->NeedBack.Axis6 = 0;
	}
	if(System.Gap.Axis7 == 0)
	{
		AreaRunNCSign->NeedBack.Axis7 = 0;
	}
	if(System.Gap.Axis8 == 0)
	{
		AreaRunNCSign->NeedBack.Axis8 = 0;
	}
	if(System.Gap.Axis9 == 0)
	{
		AreaRunNCSign->NeedBack.Axis9 = 0;
	}
	if(System.Gap.Axis10 == 0)
	{
		AreaRunNCSign->NeedBack.Axis10 = 0;
	}
	if(System.Gap.Axis11 == 0)
	{
		AreaRunNCSign->NeedBack.Axis11 = 0;
	}
	if(System.Gap.Axis12 == 0)
	{
		AreaRunNCSign->NeedBack.Axis12 = 0;
	}
	if(System.Gap.Axis13 == 0)
	{
		AreaRunNCSign->NeedBack.Axis13 = 0;
	}
	if(System.Gap.Axis14 == 0)
	{
		AreaRunNCSign->NeedBack.Axis14 = 0;
	}
	if(System.Gap.Axis15 == 0)
	{
		AreaRunNCSign->NeedBack.Axis15 = 0;
	}
	if(System.Gap.Axis16 == 0)
	{
		AreaRunNCSign->NeedBack.Axis16 = 0;
	}
	if(System.Gap.Axis17 == 0)
	{
		AreaRunNCSign->NeedBack.Axis17 = 0;
	}
	if(System.Gap.Axis18 == 0)
	{
		AreaRunNCSign->NeedBack.Axis18 = 0;
	}
	if(System.Gap.Axis19 == 0)
	{
		AreaRunNCSign->NeedBack.Axis19 = 0;
	}
	if(System.Gap.Axis20 == 0)
	{
		AreaRunNCSign->NeedBack.Axis20 = 0;
	}
	if(System.Gap.Axis21 == 0)
	{
		AreaRunNCSign->NeedBack.Axis21 = 0;
	}
	if(System.Gap.Axis22 == 0)
	{
		AreaRunNCSign->NeedBack.Axis22 = 0;
	}
	if(System.Gap.Axis23 == 0)
	{
		AreaRunNCSign->NeedBack.Axis23 = 0;
	}

	return 1;
}         


//区域曲线 运行输出控制
Uint16 NCAreaHyperbolaRun(AREA_CONTROL *AREAControl,Uint8 ModuleSel)			
{
	if(ModuleSel == 1)
	{		
		if( AREAControl->CurrentT_Module.Module1<= AREAControl->T1_Module.Module1 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1) < 0.01)
			{
				AREAControl->VNow_Module.Module1 = AREAControl->Vs_Module.Module1;
				AREAControl->Snow_Module.Module1 = AREAControl->Vs_Module.Module1*AREAControl->CurrentT_Module.Module1;
			}
			else
			{
				AREAControl->VNow_Module.Module1 = divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->CurrentT_Module.Module1,2),
					2*(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1) ) + AREAControl->Vs_Module.Module1;
				AREAControl->Snow_Module.Module1 = divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp(AREAControl->CurrentT_Module.Module1,3), 
					6*(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1))+AREAControl->Vs_Module.Module1*AREAControl->CurrentT_Module.Module1;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module1 > AREAControl->T1_Module.Module1 ) &&
				(AREAControl->CurrentT_Module.Module1 <= AREAControl->T2_Module.Module1 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module1-AREAControl->Vs_Module.Module1) < 0.01)
			{
				AREAControl->VNow_Module.Module1 = AREAControl->VmReal_Module.Module1;
				AREAControl->Snow_Module.Module1 = AREAControl->VmReal_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T1_Module.Module1);
			}
			else
			{
				AREAControl->VNow_Module.Module1 = divdp( powdp(AREAControl->a_ac_Module.Module1,2)*powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T2_Module.Module1),2), 
					2*(AREAControl->Vs_Module.Module1-AREAControl->VmReal_Module.Module1) )+AREAControl->VmReal_Module.Module1;
				AREAControl->Snow_Module.Module1 = divdp( (powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T2_Module.Module1),3)+powdp(AREAControl->T1_Module.Module1,3))*powdp(AREAControl->a_ac_Module.Module1,2), 
					6*(AREAControl->Vs_Module.Module1-AREAControl->VmReal_Module.Module1) ) + AREAControl->VmReal_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T1_Module.Module1);
			}
			AREAControl->Snow_Module.Module1 += AREAControl->S1_Module.Module1;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module1 > AREAControl->T2_Module.Module1 ) &&
				( AREAControl->CurrentT_Module.Module1 <= AREAControl->T3_Module.Module1 ) )
		{
			AREAControl->VNow_Module.Module1 = AREAControl->VmReal_Module.Module1;
			AREAControl->Snow_Module.Module1 = AREAControl->VmReal_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T2_Module.Module1);
			if(AREAControl->Snow_Module.Module1 > AREAControl->S3_Module.Module1)
				AREAControl->Snow_Module.Module1 = AREAControl->S3_Module.Module1;
			AREAControl->Snow_Module.Module1 += AREAControl->S1_Module.Module1+AREAControl->S2_Module.Module1;
		}
			
		else if(AREAControl->T4_Module.Module1 == AREAControl->T3_Module.Module1)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module1 = AREAControl->Stotal_Module.Module1;
		}

		else if( ( AREAControl->CurrentT_Module.Module1 > AREAControl->T3_Module.Module1 ) &&
				(AREAControl->CurrentT_Module.Module1 <= AREAControl->T4_Module.Module1 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1) < 0.01)
			{
				AREAControl->VNow_Module.Module1 = AREAControl->VmReal_Module.Module1;
				AREAControl->Snow_Module.Module1 = AREAControl->VmReal_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T3_Module.Module1);
			}
			else
			{	AREAControl->VNow_Module.Module1 = divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T3_Module.Module1),2), 
									2*(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1))+AREAControl->VmReal_Module.Module1;
				AREAControl->Snow_Module.Module1 = divdp( powdp(AREAControl->a_de_Module.Module1,2)*
									powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T3_Module.Module1),3), 
							  		6*(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1)) + 
							  		AREAControl->VmReal_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T3_Module.Module1);
			}
			AREAControl->Snow_Module.Module1 += AREAControl->S1_Module.Module1+AREAControl->S2_Module.Module1+AREAControl->S3_Module.Module1;
		}
		else if( ( AREAControl->CurrentT_Module.Module1 > AREAControl->T4_Module.Module1 ) &&
				( AREAControl->CurrentT_Module.Module1 <= AREAControl->T5_Module.Module1 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module1-AREAControl->VmReal_Module.Module1) < 0.01)
			{
				AREAControl->VNow_Module.Module1 = AREAControl->Ve_Module.Module1;
				AREAControl->Snow_Module.Module1 = AREAControl->Ve_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T4_Module.Module1);
			}
			else
			{
				AREAControl->VNow_Module.Module1 = divdp( powdp(AREAControl->a_de_Module.Module1,2)*powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T5_Module.Module1),2), 
									2*(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1))+AREAControl->Ve_Module.Module1;
				
				AREAControl->Snow_Module.Module1 = divdp( (powdp((AREAControl->CurrentT_Module.Module1-AREAControl->T5_Module.Module1),3)+powdp((AREAControl->T5_Module.Module1-AREAControl->T4_Module.Module1),3))*powdp(AREAControl->a_de_Module.Module1,2), 
									6*(AREAControl->VmReal_Module.Module1-AREAControl->Ve_Module.Module1))+AREAControl->Ve_Module.Module1*(AREAControl->CurrentT_Module.Module1-AREAControl->T4_Module.Module1);

			}
			AREAControl->Snow_Module.Module1 += AREAControl->S1_Module.Module1+AREAControl->S2_Module.Module1+AREAControl->S3_Module.Module1+AREAControl->S4_Module.Module1;
		}
		else if( AREAControl->CurrentT_Module.Module1 > AREAControl->T5_Module.Module1 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module1 = AREAControl->Ve_Module.Module1;
			AREAControl->Snow_Module.Module1 = AREAControl->Stotal_Module.Module1;
		}
		
		if(AREAControl->VNow_Module.Module1 < AREAControl->VMin_Module.Module1)	
			AREAControl->VNow_Module.Module1 = AREAControl->VMin_Module.Module1;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 2)
	{		
		if( AREAControl->CurrentT_Module.Module2<= AREAControl->T1_Module.Module2 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2) < 0.01)
			{
				AREAControl->VNow_Module.Module2 = AREAControl->Vs_Module.Module2;
				AREAControl->Snow_Module.Module2 = AREAControl->Vs_Module.Module2*AREAControl->CurrentT_Module.Module2;
			}
			else
			{
				AREAControl->VNow_Module.Module2 = divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->CurrentT_Module.Module2,2), 
					2*(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2) ) + AREAControl->Vs_Module.Module2;
				AREAControl->Snow_Module.Module2 = divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp(AREAControl->CurrentT_Module.Module2,3), 
					6*(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2))+AREAControl->Vs_Module.Module2*AREAControl->CurrentT_Module.Module2;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module2 > AREAControl->T1_Module.Module2 ) &&
				(AREAControl->CurrentT_Module.Module2 <= AREAControl->T2_Module.Module2 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module2-AREAControl->Vs_Module.Module2) < 0.01)
			{
				AREAControl->VNow_Module.Module2 = AREAControl->VmReal_Module.Module2;
				AREAControl->Snow_Module.Module2 = AREAControl->VmReal_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T1_Module.Module2);
			}
			else
			{
				AREAControl->VNow_Module.Module2 = divdp( powdp(AREAControl->a_ac_Module.Module2,2)*powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T2_Module.Module2),2), 
					2*(AREAControl->Vs_Module.Module2-AREAControl->VmReal_Module.Module2) )+AREAControl->VmReal_Module.Module2;
				AREAControl->Snow_Module.Module2 = divdp( (powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T2_Module.Module2),3)+powdp(AREAControl->T1_Module.Module2,3))*powdp(AREAControl->a_ac_Module.Module2,2), 
					6*(AREAControl->Vs_Module.Module2-AREAControl->VmReal_Module.Module2) ) + AREAControl->VmReal_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T1_Module.Module2);
			}
			AREAControl->Snow_Module.Module2 += AREAControl->S1_Module.Module2;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module2 > AREAControl->T2_Module.Module2 ) &&
				( AREAControl->CurrentT_Module.Module2 <= AREAControl->T3_Module.Module2 ) )
		{
			AREAControl->VNow_Module.Module2 = AREAControl->VmReal_Module.Module2;
			AREAControl->Snow_Module.Module2 = AREAControl->VmReal_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T2_Module.Module2);
			if(AREAControl->Snow_Module.Module2 > AREAControl->S3_Module.Module2)
				AREAControl->Snow_Module.Module2 = AREAControl->S3_Module.Module2;
			AREAControl->Snow_Module.Module2 += AREAControl->S1_Module.Module2+AREAControl->S2_Module.Module2;
		}
			
		else if(AREAControl->T4_Module.Module2 == AREAControl->T3_Module.Module2)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module2 = AREAControl->Stotal_Module.Module2;
		}

		else if( ( AREAControl->CurrentT_Module.Module2 > AREAControl->T3_Module.Module2 ) &&
				(AREAControl->CurrentT_Module.Module2 <= AREAControl->T4_Module.Module2 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2) < 0.01)
			{
				AREAControl->VNow_Module.Module2 = AREAControl->VmReal_Module.Module2;
				AREAControl->Snow_Module.Module2 = AREAControl->VmReal_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T3_Module.Module2);
			}
			else
			{	AREAControl->VNow_Module.Module2 = divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T3_Module.Module2),2), 
									2*(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2))+AREAControl->VmReal_Module.Module2;
				AREAControl->Snow_Module.Module2 = divdp( powdp(AREAControl->a_de_Module.Module2,2)*
									powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T3_Module.Module2),3), 
							  		6*(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2)) + 
							  		AREAControl->VmReal_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T3_Module.Module2);
			}
			AREAControl->Snow_Module.Module2 += AREAControl->S1_Module.Module2+AREAControl->S2_Module.Module2+AREAControl->S3_Module.Module2;
		}
		else if( ( AREAControl->CurrentT_Module.Module2 > AREAControl->T4_Module.Module2 ) &&
				( AREAControl->CurrentT_Module.Module2 <= AREAControl->T5_Module.Module2 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module2-AREAControl->VmReal_Module.Module2) < 0.01)
			{
				AREAControl->VNow_Module.Module2 = AREAControl->Ve_Module.Module2;
				AREAControl->Snow_Module.Module2 = AREAControl->Ve_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T4_Module.Module2);
			}
			else
			{
				AREAControl->VNow_Module.Module2 = divdp( powdp(AREAControl->a_de_Module.Module2,2)*powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T5_Module.Module2),2), 
									2*(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2))+AREAControl->Ve_Module.Module2;
				
				AREAControl->Snow_Module.Module2 = divdp( (powdp((AREAControl->CurrentT_Module.Module2-AREAControl->T5_Module.Module2),3)+powdp((AREAControl->T5_Module.Module2-AREAControl->T4_Module.Module2),3))*powdp(AREAControl->a_de_Module.Module2,2), 
									6*(AREAControl->VmReal_Module.Module2-AREAControl->Ve_Module.Module2))+AREAControl->Ve_Module.Module2*(AREAControl->CurrentT_Module.Module2-AREAControl->T4_Module.Module2);

			}
			AREAControl->Snow_Module.Module2 += AREAControl->S1_Module.Module2+AREAControl->S2_Module.Module2+AREAControl->S3_Module.Module2+AREAControl->S4_Module.Module2;
		}
		else if( AREAControl->CurrentT_Module.Module2 > AREAControl->T5_Module.Module2 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module2 = AREAControl->Ve_Module.Module2;
			AREAControl->Snow_Module.Module2 = AREAControl->Stotal_Module.Module2;
		}
		
		if(AREAControl->VNow_Module.Module2 < AREAControl->VMin_Module.Module2)	
			AREAControl->VNow_Module.Module2 = AREAControl->VMin_Module.Module2;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 3)
	{		
		if( AREAControl->CurrentT_Module.Module3<= AREAControl->T1_Module.Module3 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3) < 0.01)
			{
				AREAControl->VNow_Module.Module3 = AREAControl->Vs_Module.Module3;
				AREAControl->Snow_Module.Module3 = AREAControl->Vs_Module.Module3*AREAControl->CurrentT_Module.Module3;
			}
			else
			{
				AREAControl->VNow_Module.Module3 = divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->CurrentT_Module.Module3,2), 
					2*(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3) ) + AREAControl->Vs_Module.Module3;
				AREAControl->Snow_Module.Module3 = divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp(AREAControl->CurrentT_Module.Module3,3), 
					6*(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3))+AREAControl->Vs_Module.Module3*AREAControl->CurrentT_Module.Module3;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module3 > AREAControl->T1_Module.Module3 ) &&
				(AREAControl->CurrentT_Module.Module3 <= AREAControl->T2_Module.Module3 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module3-AREAControl->Vs_Module.Module3) < 0.01)
			{
				AREAControl->VNow_Module.Module3 = AREAControl->VmReal_Module.Module3;
				AREAControl->Snow_Module.Module3 = AREAControl->VmReal_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T1_Module.Module3);
			}
			else
			{
				AREAControl->VNow_Module.Module3 = divdp( powdp(AREAControl->a_ac_Module.Module3,2)*powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T2_Module.Module3),2), 
					2*(AREAControl->Vs_Module.Module3-AREAControl->VmReal_Module.Module3) )+AREAControl->VmReal_Module.Module3;
				AREAControl->Snow_Module.Module3 = divdp( (powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T2_Module.Module3),3)+powdp(AREAControl->T1_Module.Module3,3))*powdp(AREAControl->a_ac_Module.Module3,2), 
					6*(AREAControl->Vs_Module.Module3-AREAControl->VmReal_Module.Module3) ) + AREAControl->VmReal_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T1_Module.Module3);
			}
			AREAControl->Snow_Module.Module3 += AREAControl->S1_Module.Module3;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module3 > AREAControl->T2_Module.Module3 ) &&
				( AREAControl->CurrentT_Module.Module3 <= AREAControl->T3_Module.Module3 ) )
		{
			AREAControl->VNow_Module.Module3 = AREAControl->VmReal_Module.Module3;
			AREAControl->Snow_Module.Module3 = AREAControl->VmReal_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T2_Module.Module3);
			if(AREAControl->Snow_Module.Module3 > AREAControl->S3_Module.Module3)
				AREAControl->Snow_Module.Module3 = AREAControl->S3_Module.Module3;
			AREAControl->Snow_Module.Module3 += AREAControl->S1_Module.Module3+AREAControl->S2_Module.Module3;
		}
			
		else if(AREAControl->T4_Module.Module3 == AREAControl->T3_Module.Module3)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module3 = AREAControl->Stotal_Module.Module3;
		}

		else if( ( AREAControl->CurrentT_Module.Module3 > AREAControl->T3_Module.Module3 ) &&
				(AREAControl->CurrentT_Module.Module3 <= AREAControl->T4_Module.Module3 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3) < 0.01)
			{
				AREAControl->VNow_Module.Module3 = AREAControl->VmReal_Module.Module3;
				AREAControl->Snow_Module.Module3 = AREAControl->VmReal_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T3_Module.Module3);
			}
			else
			{	AREAControl->VNow_Module.Module3 = divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T3_Module.Module3),2), 
									2*(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3))+AREAControl->VmReal_Module.Module3;
				AREAControl->Snow_Module.Module3 = divdp( powdp(AREAControl->a_de_Module.Module3,2)*
									powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T3_Module.Module3),3), 
							  		6*(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3)) + 
							  		AREAControl->VmReal_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T3_Module.Module3);
			}
			AREAControl->Snow_Module.Module3 += AREAControl->S1_Module.Module3+AREAControl->S2_Module.Module3+AREAControl->S3_Module.Module3;
		}
		else if( ( AREAControl->CurrentT_Module.Module3 > AREAControl->T4_Module.Module3 ) &&
				( AREAControl->CurrentT_Module.Module3 <= AREAControl->T5_Module.Module3 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module3-AREAControl->VmReal_Module.Module3) < 0.01)
			{
				AREAControl->VNow_Module.Module3 = AREAControl->Ve_Module.Module3;
				AREAControl->Snow_Module.Module3 = AREAControl->Ve_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T4_Module.Module3);
			}
			else
			{
				AREAControl->VNow_Module.Module3 = divdp( powdp(AREAControl->a_de_Module.Module3,2)*powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T5_Module.Module3),2), 
									2*(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3))+AREAControl->Ve_Module.Module3;
				
				AREAControl->Snow_Module.Module3 = divdp( (powdp((AREAControl->CurrentT_Module.Module3-AREAControl->T5_Module.Module3),3)+powdp((AREAControl->T5_Module.Module3-AREAControl->T4_Module.Module3),3))*powdp(AREAControl->a_de_Module.Module3,2), 
									6*(AREAControl->VmReal_Module.Module3-AREAControl->Ve_Module.Module3))+AREAControl->Ve_Module.Module3*(AREAControl->CurrentT_Module.Module3-AREAControl->T4_Module.Module3);

			}
			AREAControl->Snow_Module.Module3 += AREAControl->S1_Module.Module3+AREAControl->S2_Module.Module3+AREAControl->S3_Module.Module3+AREAControl->S4_Module.Module3;
		}
		else if( AREAControl->CurrentT_Module.Module3 > AREAControl->T5_Module.Module3 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module3 = AREAControl->Ve_Module.Module3;
			AREAControl->Snow_Module.Module3 = AREAControl->Stotal_Module.Module3;
		}
		
		if(AREAControl->VNow_Module.Module3 < AREAControl->VMin_Module.Module3)	
			AREAControl->VNow_Module.Module3 = AREAControl->VMin_Module.Module3;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 4)
	{		
		if( AREAControl->CurrentT_Module.Module4<= AREAControl->T1_Module.Module4 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4) < 0.01)
			{
				AREAControl->VNow_Module.Module4 = AREAControl->Vs_Module.Module4;
				AREAControl->Snow_Module.Module4 = AREAControl->Vs_Module.Module4*AREAControl->CurrentT_Module.Module4;
			}
			else
			{
				AREAControl->VNow_Module.Module4 = divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->CurrentT_Module.Module4,2), 
					2*(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4) ) + AREAControl->Vs_Module.Module4;
				AREAControl->Snow_Module.Module4 = divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp(AREAControl->CurrentT_Module.Module4,3), 
					6*(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4))+AREAControl->Vs_Module.Module4*AREAControl->CurrentT_Module.Module4;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module4 > AREAControl->T1_Module.Module4 ) &&
				(AREAControl->CurrentT_Module.Module4 <= AREAControl->T2_Module.Module4 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module4-AREAControl->Vs_Module.Module4) < 0.01)
			{
				AREAControl->VNow_Module.Module4 = AREAControl->VmReal_Module.Module4;
				AREAControl->Snow_Module.Module4 = AREAControl->VmReal_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T1_Module.Module4);
			}
			else
			{
				AREAControl->VNow_Module.Module4 = divdp( powdp(AREAControl->a_ac_Module.Module4,2)*powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T2_Module.Module4),2), 
					2*(AREAControl->Vs_Module.Module4-AREAControl->VmReal_Module.Module4) )+AREAControl->VmReal_Module.Module4;
				AREAControl->Snow_Module.Module4 = divdp( (powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T2_Module.Module4),3)+powdp(AREAControl->T1_Module.Module4,3))*powdp(AREAControl->a_ac_Module.Module4,2), 
					6*(AREAControl->Vs_Module.Module4-AREAControl->VmReal_Module.Module4) ) + AREAControl->VmReal_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T1_Module.Module4);
			}
			AREAControl->Snow_Module.Module4 += AREAControl->S1_Module.Module4;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module4 > AREAControl->T2_Module.Module4 ) &&
				( AREAControl->CurrentT_Module.Module4 <= AREAControl->T3_Module.Module4 ) )
		{
			AREAControl->VNow_Module.Module4 = AREAControl->VmReal_Module.Module4;
			AREAControl->Snow_Module.Module4 = AREAControl->VmReal_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T2_Module.Module4);
			if(AREAControl->Snow_Module.Module4 > AREAControl->S3_Module.Module4)
				AREAControl->Snow_Module.Module4 = AREAControl->S3_Module.Module4;
			AREAControl->Snow_Module.Module4 += AREAControl->S1_Module.Module4+AREAControl->S2_Module.Module4;
		}
			
		else if(AREAControl->T4_Module.Module4 == AREAControl->T3_Module.Module4)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module4 = AREAControl->Stotal_Module.Module4;
		}

		else if( ( AREAControl->CurrentT_Module.Module4 > AREAControl->T3_Module.Module4 ) &&
				(AREAControl->CurrentT_Module.Module4 <= AREAControl->T4_Module.Module4 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4) < 0.01)
			{
				AREAControl->VNow_Module.Module4 = AREAControl->VmReal_Module.Module4;
				AREAControl->Snow_Module.Module4 = AREAControl->VmReal_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T3_Module.Module4);
			}
			else
			{	AREAControl->VNow_Module.Module4 = divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T3_Module.Module4),2), 
									2*(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4))+AREAControl->VmReal_Module.Module4;
				AREAControl->Snow_Module.Module4 = divdp( powdp(AREAControl->a_de_Module.Module4,2)*
									powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T3_Module.Module4),3), 
							  		6*(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4)) + 
							  		AREAControl->VmReal_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T3_Module.Module4);
			}
			AREAControl->Snow_Module.Module4 += AREAControl->S1_Module.Module4+AREAControl->S2_Module.Module4+AREAControl->S3_Module.Module4;
		}
		else if( ( AREAControl->CurrentT_Module.Module4 > AREAControl->T4_Module.Module4 ) &&
				( AREAControl->CurrentT_Module.Module4 <= AREAControl->T5_Module.Module4 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module4-AREAControl->VmReal_Module.Module4) < 0.01)
			{
				AREAControl->VNow_Module.Module4 = AREAControl->Ve_Module.Module4;
				AREAControl->Snow_Module.Module4 = AREAControl->Ve_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T4_Module.Module4);
			}
			else
			{
				AREAControl->VNow_Module.Module4 = divdp( powdp(AREAControl->a_de_Module.Module4,2)*powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T5_Module.Module4),2), 
									2*(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4))+AREAControl->Ve_Module.Module4;
				
				AREAControl->Snow_Module.Module4 = divdp( (powdp((AREAControl->CurrentT_Module.Module4-AREAControl->T5_Module.Module4),3)+powdp((AREAControl->T5_Module.Module4-AREAControl->T4_Module.Module4),3))*powdp(AREAControl->a_de_Module.Module4,2), 
									6*(AREAControl->VmReal_Module.Module4-AREAControl->Ve_Module.Module4))+AREAControl->Ve_Module.Module4*(AREAControl->CurrentT_Module.Module4-AREAControl->T4_Module.Module4);

			}
			AREAControl->Snow_Module.Module4 += AREAControl->S1_Module.Module4+AREAControl->S2_Module.Module4+AREAControl->S3_Module.Module4+AREAControl->S4_Module.Module4;
		}
		else if( AREAControl->CurrentT_Module.Module4 > AREAControl->T5_Module.Module4 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module4 = AREAControl->Ve_Module.Module4;
			AREAControl->Snow_Module.Module4 = AREAControl->Stotal_Module.Module4;
		}
		
		if(AREAControl->VNow_Module.Module4 < AREAControl->VMin_Module.Module4)	
			AREAControl->VNow_Module.Module4 = AREAControl->VMin_Module.Module4;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 5)
	{		
		if( AREAControl->CurrentT_Module.Module5<= AREAControl->T1_Module.Module5 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5) < 0.01)
			{
				AREAControl->VNow_Module.Module5 = AREAControl->Vs_Module.Module5;
				AREAControl->Snow_Module.Module5 = AREAControl->Vs_Module.Module5*AREAControl->CurrentT_Module.Module5;
			}
			else
			{
				AREAControl->VNow_Module.Module5 = divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->CurrentT_Module.Module5,2), 
					2*(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5) ) + AREAControl->Vs_Module.Module5;
				AREAControl->Snow_Module.Module5 = divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp(AREAControl->CurrentT_Module.Module5,3), 
					6*(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5))+AREAControl->Vs_Module.Module5*AREAControl->CurrentT_Module.Module5;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module5 > AREAControl->T1_Module.Module5 ) &&
				(AREAControl->CurrentT_Module.Module5 <= AREAControl->T2_Module.Module5 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module5-AREAControl->Vs_Module.Module5) < 0.01)
			{
				AREAControl->VNow_Module.Module5 = AREAControl->VmReal_Module.Module5;
				AREAControl->Snow_Module.Module5 = AREAControl->VmReal_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T1_Module.Module5);
			}
			else
			{
				AREAControl->VNow_Module.Module5 = divdp( powdp(AREAControl->a_ac_Module.Module5,2)*powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T2_Module.Module5),2), 
					2*(AREAControl->Vs_Module.Module5-AREAControl->VmReal_Module.Module5) )+AREAControl->VmReal_Module.Module5;
				AREAControl->Snow_Module.Module5 = divdp( (powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T2_Module.Module5),3)+powdp(AREAControl->T1_Module.Module5,3))*powdp(AREAControl->a_ac_Module.Module5,2), 
					6*(AREAControl->Vs_Module.Module5-AREAControl->VmReal_Module.Module5) ) + AREAControl->VmReal_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T1_Module.Module5);
			}
			AREAControl->Snow_Module.Module5 += AREAControl->S1_Module.Module5;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module5 > AREAControl->T2_Module.Module5 ) &&
				( AREAControl->CurrentT_Module.Module5 <= AREAControl->T3_Module.Module5 ) )
		{
			AREAControl->VNow_Module.Module5 = AREAControl->VmReal_Module.Module5;
			AREAControl->Snow_Module.Module5 = AREAControl->VmReal_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T2_Module.Module5);
			if(AREAControl->Snow_Module.Module5 > AREAControl->S3_Module.Module5)
				AREAControl->Snow_Module.Module5 = AREAControl->S3_Module.Module5;
			AREAControl->Snow_Module.Module5 += AREAControl->S1_Module.Module5+AREAControl->S2_Module.Module5;
		}
			
		else if(AREAControl->T4_Module.Module5 == AREAControl->T3_Module.Module5)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module5 = AREAControl->Stotal_Module.Module5;
		}

		else if( ( AREAControl->CurrentT_Module.Module5 > AREAControl->T3_Module.Module5 ) &&
				(AREAControl->CurrentT_Module.Module5 <= AREAControl->T4_Module.Module5 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5) < 0.01)
			{
				AREAControl->VNow_Module.Module5 = AREAControl->VmReal_Module.Module5;
				AREAControl->Snow_Module.Module5 = AREAControl->VmReal_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T3_Module.Module5);
			}
			else
			{	AREAControl->VNow_Module.Module5 = divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T3_Module.Module5),2), 
									2*(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5))+AREAControl->VmReal_Module.Module5;
				AREAControl->Snow_Module.Module5 = divdp( powdp(AREAControl->a_de_Module.Module5,2)*
									powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T3_Module.Module5),3), 
							  		6*(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5)) + 
							  		AREAControl->VmReal_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T3_Module.Module5);
			}
			AREAControl->Snow_Module.Module5 += AREAControl->S1_Module.Module5+AREAControl->S2_Module.Module5+AREAControl->S3_Module.Module5;
		}
		else if( ( AREAControl->CurrentT_Module.Module5 > AREAControl->T4_Module.Module5 ) &&
				( AREAControl->CurrentT_Module.Module5 <= AREAControl->T5_Module.Module5 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module5-AREAControl->VmReal_Module.Module5) < 0.01)
			{
				AREAControl->VNow_Module.Module5 = AREAControl->Ve_Module.Module5;
				AREAControl->Snow_Module.Module5 = AREAControl->Ve_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T4_Module.Module5);
			}
			else
			{
				AREAControl->VNow_Module.Module5 = divdp( powdp(AREAControl->a_de_Module.Module5,2)*powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T5_Module.Module5),2), 
									2*(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5))+AREAControl->Ve_Module.Module5;
				
				AREAControl->Snow_Module.Module5 = divdp( (powdp((AREAControl->CurrentT_Module.Module5-AREAControl->T5_Module.Module5),3)+powdp((AREAControl->T5_Module.Module5-AREAControl->T4_Module.Module5),3))*powdp(AREAControl->a_de_Module.Module5,2), 
									6*(AREAControl->VmReal_Module.Module5-AREAControl->Ve_Module.Module5))+AREAControl->Ve_Module.Module5*(AREAControl->CurrentT_Module.Module5-AREAControl->T4_Module.Module5);

			}
			AREAControl->Snow_Module.Module5 += AREAControl->S1_Module.Module5+AREAControl->S2_Module.Module5+AREAControl->S3_Module.Module5+AREAControl->S4_Module.Module5;
		}
		else if( AREAControl->CurrentT_Module.Module5 > AREAControl->T5_Module.Module5 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module5 = AREAControl->Ve_Module.Module5;
			AREAControl->Snow_Module.Module5 = AREAControl->Stotal_Module.Module5;
		}
		
		if(AREAControl->VNow_Module.Module5 < AREAControl->VMin_Module.Module5)	
			AREAControl->VNow_Module.Module5 = AREAControl->VMin_Module.Module5;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 6)
	{		
		if( AREAControl->CurrentT_Module.Module6<= AREAControl->T1_Module.Module6 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6) < 0.01)
			{
				AREAControl->VNow_Module.Module6 = AREAControl->Vs_Module.Module6;
				AREAControl->Snow_Module.Module6 = AREAControl->Vs_Module.Module6*AREAControl->CurrentT_Module.Module6;
			}
			else
			{
				AREAControl->VNow_Module.Module6 = divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->CurrentT_Module.Module6,2), 
					2*(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6) ) + AREAControl->Vs_Module.Module6;
				AREAControl->Snow_Module.Module6 = divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp(AREAControl->CurrentT_Module.Module6,3), 
					6*(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6))+AREAControl->Vs_Module.Module6*AREAControl->CurrentT_Module.Module6;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module6 > AREAControl->T1_Module.Module6 ) &&
				(AREAControl->CurrentT_Module.Module6 <= AREAControl->T2_Module.Module6 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module6-AREAControl->Vs_Module.Module6) < 0.01)
			{
				AREAControl->VNow_Module.Module6 = AREAControl->VmReal_Module.Module6;
				AREAControl->Snow_Module.Module6 = AREAControl->VmReal_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T1_Module.Module6);
			}
			else
			{
				AREAControl->VNow_Module.Module6 = divdp( powdp(AREAControl->a_ac_Module.Module6,2)*powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T2_Module.Module6),2), 
					2*(AREAControl->Vs_Module.Module6-AREAControl->VmReal_Module.Module6) )+AREAControl->VmReal_Module.Module6;
				AREAControl->Snow_Module.Module6 = divdp( (powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T2_Module.Module6),3)+powdp(AREAControl->T1_Module.Module6,3))*powdp(AREAControl->a_ac_Module.Module6,2), 
					6*(AREAControl->Vs_Module.Module6-AREAControl->VmReal_Module.Module6) ) + AREAControl->VmReal_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T1_Module.Module6);
			}
			AREAControl->Snow_Module.Module6 += AREAControl->S1_Module.Module6;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module6 > AREAControl->T2_Module.Module6 ) &&
				( AREAControl->CurrentT_Module.Module6 <= AREAControl->T3_Module.Module6 ) )
		{
			AREAControl->VNow_Module.Module6 = AREAControl->VmReal_Module.Module6;
			AREAControl->Snow_Module.Module6 = AREAControl->VmReal_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T2_Module.Module6);
			if(AREAControl->Snow_Module.Module6 > AREAControl->S3_Module.Module6)
				AREAControl->Snow_Module.Module6 = AREAControl->S3_Module.Module6;
			AREAControl->Snow_Module.Module6 += AREAControl->S1_Module.Module6+AREAControl->S2_Module.Module6;
		}
			
		else if(AREAControl->T4_Module.Module6 == AREAControl->T3_Module.Module6)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module6 = AREAControl->Stotal_Module.Module6;
		}

		else if( ( AREAControl->CurrentT_Module.Module6 > AREAControl->T3_Module.Module6 ) &&
				(AREAControl->CurrentT_Module.Module6 <= AREAControl->T4_Module.Module6 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6) < 0.01)
			{
				AREAControl->VNow_Module.Module6 = AREAControl->VmReal_Module.Module6;
				AREAControl->Snow_Module.Module6 = AREAControl->VmReal_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T3_Module.Module6);
			}
			else
			{	AREAControl->VNow_Module.Module6 = divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T3_Module.Module6),2), 
									2*(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6))+AREAControl->VmReal_Module.Module6;
				AREAControl->Snow_Module.Module6 = divdp( powdp(AREAControl->a_de_Module.Module6,2)*
									powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T3_Module.Module6),3), 
							  		6*(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6)) + 
							  		AREAControl->VmReal_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T3_Module.Module6);
			}
			AREAControl->Snow_Module.Module6 += AREAControl->S1_Module.Module6+AREAControl->S2_Module.Module6+AREAControl->S3_Module.Module6;
		}
		else if( ( AREAControl->CurrentT_Module.Module6 > AREAControl->T4_Module.Module6 ) &&
				( AREAControl->CurrentT_Module.Module6 <= AREAControl->T5_Module.Module6 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module6-AREAControl->VmReal_Module.Module6) < 0.01)
			{
				AREAControl->VNow_Module.Module6 = AREAControl->Ve_Module.Module6;
				AREAControl->Snow_Module.Module6 = AREAControl->Ve_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T4_Module.Module6);
			}
			else
			{
				AREAControl->VNow_Module.Module6 = divdp( powdp(AREAControl->a_de_Module.Module6,2)*powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T5_Module.Module6),2), 
									2*(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6))+AREAControl->Ve_Module.Module6;
				
				AREAControl->Snow_Module.Module6 = divdp( (powdp((AREAControl->CurrentT_Module.Module6-AREAControl->T5_Module.Module6),3)+powdp((AREAControl->T5_Module.Module6-AREAControl->T4_Module.Module6),3))*powdp(AREAControl->a_de_Module.Module6,2), 
									6*(AREAControl->VmReal_Module.Module6-AREAControl->Ve_Module.Module6))+AREAControl->Ve_Module.Module6*(AREAControl->CurrentT_Module.Module6-AREAControl->T4_Module.Module6);

			}
			AREAControl->Snow_Module.Module6 += AREAControl->S1_Module.Module6+AREAControl->S2_Module.Module6+AREAControl->S3_Module.Module6+AREAControl->S4_Module.Module6;
		}
		else if( AREAControl->CurrentT_Module.Module6 > AREAControl->T5_Module.Module6 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module6 = AREAControl->Ve_Module.Module6;
			AREAControl->Snow_Module.Module6 = AREAControl->Stotal_Module.Module6;
		}
		
		if(AREAControl->VNow_Module.Module6 < AREAControl->VMin_Module.Module6)	
			AREAControl->VNow_Module.Module6 = AREAControl->VMin_Module.Module6;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 7)
	{		
		if( AREAControl->CurrentT_Module.Module7<= AREAControl->T1_Module.Module7 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7) < 0.01)
			{
				AREAControl->VNow_Module.Module7 = AREAControl->Vs_Module.Module7;
				AREAControl->Snow_Module.Module7 = AREAControl->Vs_Module.Module7*AREAControl->CurrentT_Module.Module7;
			}
			else
			{
				AREAControl->VNow_Module.Module7 = divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->CurrentT_Module.Module7,2), 
					2*(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7) ) + AREAControl->Vs_Module.Module7;
				AREAControl->Snow_Module.Module7 = divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp(AREAControl->CurrentT_Module.Module7,3), 
					6*(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7))+AREAControl->Vs_Module.Module7*AREAControl->CurrentT_Module.Module7;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module7 > AREAControl->T1_Module.Module7 ) &&
				(AREAControl->CurrentT_Module.Module7 <= AREAControl->T2_Module.Module7 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module7-AREAControl->Vs_Module.Module7) < 0.01)
			{
				AREAControl->VNow_Module.Module7 = AREAControl->VmReal_Module.Module7;
				AREAControl->Snow_Module.Module7 = AREAControl->VmReal_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T1_Module.Module7);
			}
			else
			{
				AREAControl->VNow_Module.Module7 = divdp( powdp(AREAControl->a_ac_Module.Module7,2)*powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T2_Module.Module7),2), 
					2*(AREAControl->Vs_Module.Module7-AREAControl->VmReal_Module.Module7) )+AREAControl->VmReal_Module.Module7;
				AREAControl->Snow_Module.Module7 = divdp( (powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T2_Module.Module7),3)+powdp(AREAControl->T1_Module.Module7,3))*powdp(AREAControl->a_ac_Module.Module7,2), 
					6*(AREAControl->Vs_Module.Module7-AREAControl->VmReal_Module.Module7) ) + AREAControl->VmReal_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T1_Module.Module7);
			}
			AREAControl->Snow_Module.Module7 += AREAControl->S1_Module.Module7;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module7 > AREAControl->T2_Module.Module7 ) &&
				( AREAControl->CurrentT_Module.Module7 <= AREAControl->T3_Module.Module7 ) )
		{
			AREAControl->VNow_Module.Module7 = AREAControl->VmReal_Module.Module7;
			AREAControl->Snow_Module.Module7 = AREAControl->VmReal_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T2_Module.Module7);
			if(AREAControl->Snow_Module.Module7 > AREAControl->S3_Module.Module7)
				AREAControl->Snow_Module.Module7 = AREAControl->S3_Module.Module7;
			AREAControl->Snow_Module.Module7 += AREAControl->S1_Module.Module7+AREAControl->S2_Module.Module7;
		}
			
		else if(AREAControl->T4_Module.Module7 == AREAControl->T3_Module.Module7)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module7 = AREAControl->Stotal_Module.Module7;
		}

		else if( ( AREAControl->CurrentT_Module.Module7 > AREAControl->T3_Module.Module7 ) &&
				(AREAControl->CurrentT_Module.Module7 <= AREAControl->T4_Module.Module7 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7) < 0.01)
			{
				AREAControl->VNow_Module.Module7 = AREAControl->VmReal_Module.Module7;
				AREAControl->Snow_Module.Module7 = AREAControl->VmReal_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T3_Module.Module7);
			}
			else
			{	AREAControl->VNow_Module.Module7 = divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T3_Module.Module7),2), 
									2*(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7))+AREAControl->VmReal_Module.Module7;
				AREAControl->Snow_Module.Module7 = divdp( powdp(AREAControl->a_de_Module.Module7,2)*
									powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T3_Module.Module7),3), 
							  		6*(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7)) + 
							  		AREAControl->VmReal_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T3_Module.Module7);
			}
			AREAControl->Snow_Module.Module7 += AREAControl->S1_Module.Module7+AREAControl->S2_Module.Module7+AREAControl->S3_Module.Module7;
		}
		else if( ( AREAControl->CurrentT_Module.Module7 > AREAControl->T4_Module.Module7 ) &&
				( AREAControl->CurrentT_Module.Module7 <= AREAControl->T5_Module.Module7 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module7-AREAControl->VmReal_Module.Module7) < 0.01)
			{
				AREAControl->VNow_Module.Module7 = AREAControl->Ve_Module.Module7;
				AREAControl->Snow_Module.Module7 = AREAControl->Ve_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T4_Module.Module7);
			}
			else
			{
				AREAControl->VNow_Module.Module7 = divdp( powdp(AREAControl->a_de_Module.Module7,2)*powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T5_Module.Module7),2), 
									2*(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7))+AREAControl->Ve_Module.Module7;
				
				AREAControl->Snow_Module.Module7 = divdp( (powdp((AREAControl->CurrentT_Module.Module7-AREAControl->T5_Module.Module7),3)+powdp((AREAControl->T5_Module.Module7-AREAControl->T4_Module.Module7),3))*powdp(AREAControl->a_de_Module.Module7,2), 
									6*(AREAControl->VmReal_Module.Module7-AREAControl->Ve_Module.Module7))+AREAControl->Ve_Module.Module7*(AREAControl->CurrentT_Module.Module7-AREAControl->T4_Module.Module7);

			}
			AREAControl->Snow_Module.Module7 += AREAControl->S1_Module.Module7+AREAControl->S2_Module.Module7+AREAControl->S3_Module.Module7+AREAControl->S4_Module.Module7;
		}
		else if( AREAControl->CurrentT_Module.Module7 > AREAControl->T5_Module.Module7 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module7 = AREAControl->Ve_Module.Module7;
			AREAControl->Snow_Module.Module7 = AREAControl->Stotal_Module.Module7;
		}
		
		if(AREAControl->VNow_Module.Module7 < AREAControl->VMin_Module.Module7)	
			AREAControl->VNow_Module.Module7 = AREAControl->VMin_Module.Module7;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 8)
	{		
		if( AREAControl->CurrentT_Module.Module8<= AREAControl->T1_Module.Module8 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8) < 0.01)
			{
				AREAControl->VNow_Module.Module8 = AREAControl->Vs_Module.Module8;
				AREAControl->Snow_Module.Module8 = AREAControl->Vs_Module.Module8*AREAControl->CurrentT_Module.Module8;
			}
			else
			{
				AREAControl->VNow_Module.Module8 = divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->CurrentT_Module.Module8,2), 
					2*(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8) ) + AREAControl->Vs_Module.Module8;
				AREAControl->Snow_Module.Module8 = divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp(AREAControl->CurrentT_Module.Module8,3), 
					6*(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8))+AREAControl->Vs_Module.Module8*AREAControl->CurrentT_Module.Module8;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module8 > AREAControl->T1_Module.Module8 ) &&
				(AREAControl->CurrentT_Module.Module8 <= AREAControl->T2_Module.Module8 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module8-AREAControl->Vs_Module.Module8) < 0.01)
			{
				AREAControl->VNow_Module.Module8 = AREAControl->VmReal_Module.Module8;
				AREAControl->Snow_Module.Module8 = AREAControl->VmReal_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T1_Module.Module8);
			}
			else
			{
				AREAControl->VNow_Module.Module8 = divdp( powdp(AREAControl->a_ac_Module.Module8,2)*powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T2_Module.Module8),2), 
					2*(AREAControl->Vs_Module.Module8-AREAControl->VmReal_Module.Module8) )+AREAControl->VmReal_Module.Module8;
				AREAControl->Snow_Module.Module8 = divdp( (powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T2_Module.Module8),3)+powdp(AREAControl->T1_Module.Module8,3))*powdp(AREAControl->a_ac_Module.Module8,2), 
					6*(AREAControl->Vs_Module.Module8-AREAControl->VmReal_Module.Module8) ) + AREAControl->VmReal_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T1_Module.Module8);
			}
			AREAControl->Snow_Module.Module8 += AREAControl->S1_Module.Module8;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module8 > AREAControl->T2_Module.Module8 ) &&
				( AREAControl->CurrentT_Module.Module8 <= AREAControl->T3_Module.Module8 ) )
		{
			AREAControl->VNow_Module.Module8 = AREAControl->VmReal_Module.Module8;
			AREAControl->Snow_Module.Module8 = AREAControl->VmReal_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T2_Module.Module8);
			if(AREAControl->Snow_Module.Module8 > AREAControl->S3_Module.Module8)
				AREAControl->Snow_Module.Module8 = AREAControl->S3_Module.Module8;
			AREAControl->Snow_Module.Module8 += AREAControl->S1_Module.Module8+AREAControl->S2_Module.Module8;
		}
			
		else if(AREAControl->T4_Module.Module8 == AREAControl->T3_Module.Module8)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module8 = AREAControl->Stotal_Module.Module8;
		}

		else if( ( AREAControl->CurrentT_Module.Module8 > AREAControl->T3_Module.Module8 ) &&
				(AREAControl->CurrentT_Module.Module8 <= AREAControl->T4_Module.Module8 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8) < 0.01)
			{
				AREAControl->VNow_Module.Module8 = AREAControl->VmReal_Module.Module8;
				AREAControl->Snow_Module.Module8 = AREAControl->VmReal_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T3_Module.Module8);
			}
			else
			{	AREAControl->VNow_Module.Module8 = divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T3_Module.Module8),2), 
									2*(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8))+AREAControl->VmReal_Module.Module8;
				AREAControl->Snow_Module.Module8 = divdp( powdp(AREAControl->a_de_Module.Module8,2)*
									powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T3_Module.Module8),3), 
							  		6*(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8)) + 
							  		AREAControl->VmReal_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T3_Module.Module8);
			}
			AREAControl->Snow_Module.Module8 += AREAControl->S1_Module.Module8+AREAControl->S2_Module.Module8+AREAControl->S3_Module.Module8;
		}
		else if( ( AREAControl->CurrentT_Module.Module8 > AREAControl->T4_Module.Module8 ) &&
				( AREAControl->CurrentT_Module.Module8 <= AREAControl->T5_Module.Module8 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module8-AREAControl->VmReal_Module.Module8) < 0.01)
			{
				AREAControl->VNow_Module.Module8 = AREAControl->Ve_Module.Module8;
				AREAControl->Snow_Module.Module8 = AREAControl->Ve_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T4_Module.Module8);
			}
			else
			{
				AREAControl->VNow_Module.Module8 = divdp( powdp(AREAControl->a_de_Module.Module8,2)*powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T5_Module.Module8),2), 
									2*(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8))+AREAControl->Ve_Module.Module8;
				
				AREAControl->Snow_Module.Module8 = divdp( (powdp((AREAControl->CurrentT_Module.Module8-AREAControl->T5_Module.Module8),3)+powdp((AREAControl->T5_Module.Module8-AREAControl->T4_Module.Module8),3))*powdp(AREAControl->a_de_Module.Module8,2), 
									6*(AREAControl->VmReal_Module.Module8-AREAControl->Ve_Module.Module8))+AREAControl->Ve_Module.Module8*(AREAControl->CurrentT_Module.Module8-AREAControl->T4_Module.Module8);

			}
			AREAControl->Snow_Module.Module8 += AREAControl->S1_Module.Module8+AREAControl->S2_Module.Module8+AREAControl->S3_Module.Module8+AREAControl->S4_Module.Module8;
		}
		else if( AREAControl->CurrentT_Module.Module8 > AREAControl->T5_Module.Module8 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module8 = AREAControl->Ve_Module.Module8;
			AREAControl->Snow_Module.Module8 = AREAControl->Stotal_Module.Module8;
		}
		
		if(AREAControl->VNow_Module.Module8 < AREAControl->VMin_Module.Module8)	
			AREAControl->VNow_Module.Module8 = AREAControl->VMin_Module.Module8;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 9)
	{		
		if( AREAControl->CurrentT_Module.Module9<= AREAControl->T1_Module.Module9 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9) < 0.01)
			{
				AREAControl->VNow_Module.Module9 = AREAControl->Vs_Module.Module9;
				AREAControl->Snow_Module.Module9 = AREAControl->Vs_Module.Module9*AREAControl->CurrentT_Module.Module9;
			}
			else
			{
				AREAControl->VNow_Module.Module9 = divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->CurrentT_Module.Module9,2), 
					2*(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9) ) + AREAControl->Vs_Module.Module9;
				AREAControl->Snow_Module.Module9 = divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp(AREAControl->CurrentT_Module.Module9,3), 
					6*(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9))+AREAControl->Vs_Module.Module9*AREAControl->CurrentT_Module.Module9;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module9 > AREAControl->T1_Module.Module9 ) &&
				(AREAControl->CurrentT_Module.Module9 <= AREAControl->T2_Module.Module9 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module9-AREAControl->Vs_Module.Module9) < 0.01)
			{
				AREAControl->VNow_Module.Module9 = AREAControl->VmReal_Module.Module9;
				AREAControl->Snow_Module.Module9 = AREAControl->VmReal_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T1_Module.Module9);
			}
			else
			{
				AREAControl->VNow_Module.Module9 = divdp( powdp(AREAControl->a_ac_Module.Module9,2)*powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T2_Module.Module9),2), 
					2*(AREAControl->Vs_Module.Module9-AREAControl->VmReal_Module.Module9) )+AREAControl->VmReal_Module.Module9;
				AREAControl->Snow_Module.Module9 = divdp( (powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T2_Module.Module9),3)+powdp(AREAControl->T1_Module.Module9,3))*powdp(AREAControl->a_ac_Module.Module9,2), 
					6*(AREAControl->Vs_Module.Module9-AREAControl->VmReal_Module.Module9) ) + AREAControl->VmReal_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T1_Module.Module9);
			}
			AREAControl->Snow_Module.Module9 += AREAControl->S1_Module.Module9;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module9 > AREAControl->T2_Module.Module9 ) &&
				( AREAControl->CurrentT_Module.Module9 <= AREAControl->T3_Module.Module9 ) )
		{
			AREAControl->VNow_Module.Module9 = AREAControl->VmReal_Module.Module9;
			AREAControl->Snow_Module.Module9 = AREAControl->VmReal_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T2_Module.Module9);
			if(AREAControl->Snow_Module.Module9 > AREAControl->S3_Module.Module9)
				AREAControl->Snow_Module.Module9 = AREAControl->S3_Module.Module9;
			AREAControl->Snow_Module.Module9 += AREAControl->S1_Module.Module9+AREAControl->S2_Module.Module9;
		}
			
		else if(AREAControl->T4_Module.Module9 == AREAControl->T3_Module.Module9)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module9 = AREAControl->Stotal_Module.Module9;
		}

		else if( ( AREAControl->CurrentT_Module.Module9 > AREAControl->T3_Module.Module9 ) &&
				(AREAControl->CurrentT_Module.Module9 <= AREAControl->T4_Module.Module9 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9) < 0.01)
			{
				AREAControl->VNow_Module.Module9 = AREAControl->VmReal_Module.Module9;
				AREAControl->Snow_Module.Module9 = AREAControl->VmReal_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T3_Module.Module9);
			}
			else
			{	AREAControl->VNow_Module.Module9 = divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T3_Module.Module9),2), 
									2*(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9))+AREAControl->VmReal_Module.Module9;
				AREAControl->Snow_Module.Module9 = divdp( powdp(AREAControl->a_de_Module.Module9,2)*
									powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T3_Module.Module9),3), 
							  		6*(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9)) + 
							  		AREAControl->VmReal_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T3_Module.Module9);
			}
			AREAControl->Snow_Module.Module9 += AREAControl->S1_Module.Module9+AREAControl->S2_Module.Module9+AREAControl->S3_Module.Module9;
		}
		else if( ( AREAControl->CurrentT_Module.Module9 > AREAControl->T4_Module.Module9 ) &&
				( AREAControl->CurrentT_Module.Module9 <= AREAControl->T5_Module.Module9 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module9-AREAControl->VmReal_Module.Module9) < 0.01)
			{
				AREAControl->VNow_Module.Module9 = AREAControl->Ve_Module.Module9;
				AREAControl->Snow_Module.Module9 = AREAControl->Ve_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T4_Module.Module9);
			}
			else
			{
				AREAControl->VNow_Module.Module9 = divdp( powdp(AREAControl->a_de_Module.Module9,2)*powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T5_Module.Module9),2), 
									2*(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9))+AREAControl->Ve_Module.Module9;
				
				AREAControl->Snow_Module.Module9 = divdp( (powdp((AREAControl->CurrentT_Module.Module9-AREAControl->T5_Module.Module9),3)+powdp((AREAControl->T5_Module.Module9-AREAControl->T4_Module.Module9),3))*powdp(AREAControl->a_de_Module.Module9,2), 
									6*(AREAControl->VmReal_Module.Module9-AREAControl->Ve_Module.Module9))+AREAControl->Ve_Module.Module9*(AREAControl->CurrentT_Module.Module9-AREAControl->T4_Module.Module9);

			}
			AREAControl->Snow_Module.Module9 += AREAControl->S1_Module.Module9+AREAControl->S2_Module.Module9+AREAControl->S3_Module.Module9+AREAControl->S4_Module.Module9;
		}
		else if( AREAControl->CurrentT_Module.Module9 > AREAControl->T5_Module.Module9 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module9 = AREAControl->Ve_Module.Module9;
			AREAControl->Snow_Module.Module9 = AREAControl->Stotal_Module.Module9;
		}
		
		if(AREAControl->VNow_Module.Module9 < AREAControl->VMin_Module.Module9)	
			AREAControl->VNow_Module.Module9 = AREAControl->VMin_Module.Module9;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 10)
	{		
		if( AREAControl->CurrentT_Module.Module10<= AREAControl->T1_Module.Module10 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10) < 0.01)
			{
				AREAControl->VNow_Module.Module10 = AREAControl->Vs_Module.Module10;
				AREAControl->Snow_Module.Module10 = AREAControl->Vs_Module.Module10*AREAControl->CurrentT_Module.Module10;
			}
			else
			{
				AREAControl->VNow_Module.Module10 = divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->CurrentT_Module.Module10,2), 
					2*(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10) ) + AREAControl->Vs_Module.Module10;
				AREAControl->Snow_Module.Module10 = divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp(AREAControl->CurrentT_Module.Module10,3), 
					6*(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10))+AREAControl->Vs_Module.Module10*AREAControl->CurrentT_Module.Module10;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module10 > AREAControl->T1_Module.Module10 ) &&
				(AREAControl->CurrentT_Module.Module10 <= AREAControl->T2_Module.Module10 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module10-AREAControl->Vs_Module.Module10) < 0.01)
			{
				AREAControl->VNow_Module.Module10 = AREAControl->VmReal_Module.Module10;
				AREAControl->Snow_Module.Module10 = AREAControl->VmReal_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T1_Module.Module10);
			}
			else
			{
				AREAControl->VNow_Module.Module10 = divdp( powdp(AREAControl->a_ac_Module.Module10,2)*powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T2_Module.Module10),2), 
					2*(AREAControl->Vs_Module.Module10-AREAControl->VmReal_Module.Module10) )+AREAControl->VmReal_Module.Module10;
				AREAControl->Snow_Module.Module10 = divdp( (powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T2_Module.Module10),3)+powdp(AREAControl->T1_Module.Module10,3))*powdp(AREAControl->a_ac_Module.Module10,2), 
					6*(AREAControl->Vs_Module.Module10-AREAControl->VmReal_Module.Module10) ) + AREAControl->VmReal_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T1_Module.Module10);
			}
			AREAControl->Snow_Module.Module10 += AREAControl->S1_Module.Module10;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module10 > AREAControl->T2_Module.Module10 ) &&
				( AREAControl->CurrentT_Module.Module10 <= AREAControl->T3_Module.Module10 ) )
		{
			AREAControl->VNow_Module.Module10 = AREAControl->VmReal_Module.Module10;
			AREAControl->Snow_Module.Module10 = AREAControl->VmReal_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T2_Module.Module10);
			if(AREAControl->Snow_Module.Module10 > AREAControl->S3_Module.Module10)
				AREAControl->Snow_Module.Module10 = AREAControl->S3_Module.Module10;
			AREAControl->Snow_Module.Module10 += AREAControl->S1_Module.Module10+AREAControl->S2_Module.Module10;
		}
			
		else if(AREAControl->T4_Module.Module10 == AREAControl->T3_Module.Module10)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module10 = AREAControl->Stotal_Module.Module10;
		}

		else if( ( AREAControl->CurrentT_Module.Module10 > AREAControl->T3_Module.Module10 ) &&
				(AREAControl->CurrentT_Module.Module10 <= AREAControl->T4_Module.Module10 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10) < 0.01)
			{
				AREAControl->VNow_Module.Module10 = AREAControl->VmReal_Module.Module10;
				AREAControl->Snow_Module.Module10 = AREAControl->VmReal_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T3_Module.Module10);
			}
			else
			{	AREAControl->VNow_Module.Module10 = divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T3_Module.Module10),2), 
									2*(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10))+AREAControl->VmReal_Module.Module10;
				AREAControl->Snow_Module.Module10 = divdp( powdp(AREAControl->a_de_Module.Module10,2)*
									powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T3_Module.Module10),3), 
							  		6*(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10)) + 
							  		AREAControl->VmReal_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T3_Module.Module10);
			}
			AREAControl->Snow_Module.Module10 += AREAControl->S1_Module.Module10+AREAControl->S2_Module.Module10+AREAControl->S3_Module.Module10;
		}
		else if( ( AREAControl->CurrentT_Module.Module10 > AREAControl->T4_Module.Module10 ) &&
				( AREAControl->CurrentT_Module.Module10 <= AREAControl->T5_Module.Module10 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module10-AREAControl->VmReal_Module.Module10) < 0.01)
			{
				AREAControl->VNow_Module.Module10 = AREAControl->Ve_Module.Module10;
				AREAControl->Snow_Module.Module10 = AREAControl->Ve_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T4_Module.Module10);
			}
			else
			{
				AREAControl->VNow_Module.Module10 = divdp( powdp(AREAControl->a_de_Module.Module10,2)*powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T5_Module.Module10),2), 
									2*(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10))+AREAControl->Ve_Module.Module10;
				
				AREAControl->Snow_Module.Module10 = divdp( (powdp((AREAControl->CurrentT_Module.Module10-AREAControl->T5_Module.Module10),3)+powdp((AREAControl->T5_Module.Module10-AREAControl->T4_Module.Module10),3))*powdp(AREAControl->a_de_Module.Module10,2), 
									6*(AREAControl->VmReal_Module.Module10-AREAControl->Ve_Module.Module10))+AREAControl->Ve_Module.Module10*(AREAControl->CurrentT_Module.Module10-AREAControl->T4_Module.Module10);

			}
			AREAControl->Snow_Module.Module10 += AREAControl->S1_Module.Module10+AREAControl->S2_Module.Module10+AREAControl->S3_Module.Module10+AREAControl->S4_Module.Module10;
		}
		else if( AREAControl->CurrentT_Module.Module10 > AREAControl->T5_Module.Module10 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module10 = AREAControl->Ve_Module.Module10;
			AREAControl->Snow_Module.Module10 = AREAControl->Stotal_Module.Module10;
		}
		
		if(AREAControl->VNow_Module.Module10 < AREAControl->VMin_Module.Module10)	
			AREAControl->VNow_Module.Module10 = AREAControl->VMin_Module.Module10;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 11)
	{		
		if( AREAControl->CurrentT_Module.Module11<= AREAControl->T1_Module.Module11 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11) < 0.01)
			{
				AREAControl->VNow_Module.Module11 = AREAControl->Vs_Module.Module11;
				AREAControl->Snow_Module.Module11 = AREAControl->Vs_Module.Module11*AREAControl->CurrentT_Module.Module11;
			}
			else
			{
				AREAControl->VNow_Module.Module11 = divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->CurrentT_Module.Module11,2), 
					2*(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11) ) + AREAControl->Vs_Module.Module11;
				AREAControl->Snow_Module.Module11 = divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp(AREAControl->CurrentT_Module.Module11,3), 
					6*(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11))+AREAControl->Vs_Module.Module11*AREAControl->CurrentT_Module.Module11;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module11 > AREAControl->T1_Module.Module11 ) &&
				(AREAControl->CurrentT_Module.Module11 <= AREAControl->T2_Module.Module11 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module11-AREAControl->Vs_Module.Module11) < 0.01)
			{
				AREAControl->VNow_Module.Module11 = AREAControl->VmReal_Module.Module11;
				AREAControl->Snow_Module.Module11 = AREAControl->VmReal_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T1_Module.Module11);
			}
			else
			{
				AREAControl->VNow_Module.Module11 = divdp( powdp(AREAControl->a_ac_Module.Module11,2)*powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T2_Module.Module11),2), 
					2*(AREAControl->Vs_Module.Module11-AREAControl->VmReal_Module.Module11) )+AREAControl->VmReal_Module.Module11;
				AREAControl->Snow_Module.Module11 = divdp( (powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T2_Module.Module11),3)+powdp(AREAControl->T1_Module.Module11,3))*powdp(AREAControl->a_ac_Module.Module11,2), 
					6*(AREAControl->Vs_Module.Module11-AREAControl->VmReal_Module.Module11) ) + AREAControl->VmReal_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T1_Module.Module11);
			}
			AREAControl->Snow_Module.Module11 += AREAControl->S1_Module.Module11;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module11 > AREAControl->T2_Module.Module11 ) &&
				( AREAControl->CurrentT_Module.Module11 <= AREAControl->T3_Module.Module11 ) )
		{
			AREAControl->VNow_Module.Module11 = AREAControl->VmReal_Module.Module11;
			AREAControl->Snow_Module.Module11 = AREAControl->VmReal_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T2_Module.Module11);
			if(AREAControl->Snow_Module.Module11 > AREAControl->S3_Module.Module11)
				AREAControl->Snow_Module.Module11 = AREAControl->S3_Module.Module11;
			AREAControl->Snow_Module.Module11 += AREAControl->S1_Module.Module11+AREAControl->S2_Module.Module11;
		}
			
		else if(AREAControl->T4_Module.Module11 == AREAControl->T3_Module.Module11)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module11 = AREAControl->Stotal_Module.Module11;
		}

		else if( ( AREAControl->CurrentT_Module.Module11 > AREAControl->T3_Module.Module11 ) &&
				(AREAControl->CurrentT_Module.Module11 <= AREAControl->T4_Module.Module11 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11) < 0.01)
			{
				AREAControl->VNow_Module.Module11 = AREAControl->VmReal_Module.Module11;
				AREAControl->Snow_Module.Module11 = AREAControl->VmReal_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T3_Module.Module11);
			}
			else
			{	AREAControl->VNow_Module.Module11 = divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T3_Module.Module11),2), 
									2*(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11))+AREAControl->VmReal_Module.Module11;
				AREAControl->Snow_Module.Module11 = divdp( powdp(AREAControl->a_de_Module.Module11,2)*
									powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T3_Module.Module11),3), 
							  		6*(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11)) + 
							  		AREAControl->VmReal_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T3_Module.Module11);
			}
			AREAControl->Snow_Module.Module11 += AREAControl->S1_Module.Module11+AREAControl->S2_Module.Module11+AREAControl->S3_Module.Module11;
		}
		else if( ( AREAControl->CurrentT_Module.Module11 > AREAControl->T4_Module.Module11 ) &&
				( AREAControl->CurrentT_Module.Module11 <= AREAControl->T5_Module.Module11 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module11-AREAControl->VmReal_Module.Module11) < 0.01)
			{
				AREAControl->VNow_Module.Module11 = AREAControl->Ve_Module.Module11;
				AREAControl->Snow_Module.Module11 = AREAControl->Ve_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T4_Module.Module11);
			}
			else
			{
				AREAControl->VNow_Module.Module11 = divdp( powdp(AREAControl->a_de_Module.Module11,2)*powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T5_Module.Module11),2), 
									2*(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11))+AREAControl->Ve_Module.Module11;
				
				AREAControl->Snow_Module.Module11 = divdp( (powdp((AREAControl->CurrentT_Module.Module11-AREAControl->T5_Module.Module11),3)+powdp((AREAControl->T5_Module.Module11-AREAControl->T4_Module.Module11),3))*powdp(AREAControl->a_de_Module.Module11,2), 
									6*(AREAControl->VmReal_Module.Module11-AREAControl->Ve_Module.Module11))+AREAControl->Ve_Module.Module11*(AREAControl->CurrentT_Module.Module11-AREAControl->T4_Module.Module11);

			}
			AREAControl->Snow_Module.Module11 += AREAControl->S1_Module.Module11+AREAControl->S2_Module.Module11+AREAControl->S3_Module.Module11+AREAControl->S4_Module.Module11;
		}
		else if( AREAControl->CurrentT_Module.Module11 > AREAControl->T5_Module.Module11 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module11 = AREAControl->Ve_Module.Module11;
			AREAControl->Snow_Module.Module11 = AREAControl->Stotal_Module.Module11;
		}
		
		if(AREAControl->VNow_Module.Module11 < AREAControl->VMin_Module.Module11)	
			AREAControl->VNow_Module.Module11 = AREAControl->VMin_Module.Module11;		//the lowest move speed

		return 1;	
	}
	else if(ModuleSel == 12)
	{		
		if( AREAControl->CurrentT_Module.Module12<= AREAControl->T1_Module.Module12 ) 
		{
			if(fabs(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12) < 0.01)
			{
				AREAControl->VNow_Module.Module12 = AREAControl->Vs_Module.Module12;
				AREAControl->Snow_Module.Module12 = AREAControl->Vs_Module.Module12*AREAControl->CurrentT_Module.Module12;
			}
			else
			{
				AREAControl->VNow_Module.Module12 = divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->CurrentT_Module.Module12,2), 
					2*(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12) ) + AREAControl->Vs_Module.Module12;
				AREAControl->Snow_Module.Module12 = divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp(AREAControl->CurrentT_Module.Module12,3), 
					6*(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12))+AREAControl->Vs_Module.Module12*AREAControl->CurrentT_Module.Module12;
			}
		}

		else if( ( AREAControl->CurrentT_Module.Module12 > AREAControl->T1_Module.Module12 ) &&
				(AREAControl->CurrentT_Module.Module12 <= AREAControl->T2_Module.Module12 ) )
		{
			if(fabs(AREAControl->VmReal_Module.Module12-AREAControl->Vs_Module.Module12) < 0.01)
			{
				AREAControl->VNow_Module.Module12 = AREAControl->VmReal_Module.Module12;
				AREAControl->Snow_Module.Module12 = AREAControl->VmReal_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T1_Module.Module12);
			}
			else
			{
				AREAControl->VNow_Module.Module12 = divdp( powdp(AREAControl->a_ac_Module.Module12,2)*powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T2_Module.Module12),2), 
					2*(AREAControl->Vs_Module.Module12-AREAControl->VmReal_Module.Module12) )+AREAControl->VmReal_Module.Module12;
				AREAControl->Snow_Module.Module12 = divdp( (powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T2_Module.Module12),3)+powdp(AREAControl->T1_Module.Module12,3))*powdp(AREAControl->a_ac_Module.Module12,2), 
					6*(AREAControl->Vs_Module.Module12-AREAControl->VmReal_Module.Module12) ) + AREAControl->VmReal_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T1_Module.Module12);
			}
			AREAControl->Snow_Module.Module12 += AREAControl->S1_Module.Module12;
		}
		
		else if( ( AREAControl->CurrentT_Module.Module12 > AREAControl->T2_Module.Module12 ) &&
				( AREAControl->CurrentT_Module.Module12 <= AREAControl->T3_Module.Module12 ) )
		{
			AREAControl->VNow_Module.Module12 = AREAControl->VmReal_Module.Module12;
			AREAControl->Snow_Module.Module12 = AREAControl->VmReal_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T2_Module.Module12);
			if(AREAControl->Snow_Module.Module12 > AREAControl->S3_Module.Module12)
				AREAControl->Snow_Module.Module12 = AREAControl->S3_Module.Module12;
			AREAControl->Snow_Module.Module12 += AREAControl->S1_Module.Module12+AREAControl->S2_Module.Module12;
		}
			
		else if(AREAControl->T4_Module.Module12 == AREAControl->T3_Module.Module12)
		{//加速段和稳速段的尾巴速度不做Ve修正		
			AREAControl->Snow_Module.Module12 = AREAControl->Stotal_Module.Module12;
		}

		else if( ( AREAControl->CurrentT_Module.Module12 > AREAControl->T3_Module.Module12 ) &&
				(AREAControl->CurrentT_Module.Module12 <= AREAControl->T4_Module.Module12 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12) < 0.01)
			{
				AREAControl->VNow_Module.Module12 = AREAControl->VmReal_Module.Module12;
				AREAControl->Snow_Module.Module12 = AREAControl->VmReal_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T3_Module.Module12);
			}
			else
			{	AREAControl->VNow_Module.Module12 = divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T3_Module.Module12),2), 
									2*(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12))+AREAControl->VmReal_Module.Module12;
				AREAControl->Snow_Module.Module12 = divdp( powdp(AREAControl->a_de_Module.Module12,2)*
									powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T3_Module.Module12),3), 
							  		6*(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12)) + 
							  		AREAControl->VmReal_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T3_Module.Module12);
			}
			AREAControl->Snow_Module.Module12 += AREAControl->S1_Module.Module12+AREAControl->S2_Module.Module12+AREAControl->S3_Module.Module12;
		}
		else if( ( AREAControl->CurrentT_Module.Module12 > AREAControl->T4_Module.Module12 ) &&
				( AREAControl->CurrentT_Module.Module12 <= AREAControl->T5_Module.Module12 ) )
		{
			if(fabs(AREAControl->Ve_Module.Module12-AREAControl->VmReal_Module.Module12) < 0.01)
			{
				AREAControl->VNow_Module.Module12 = AREAControl->Ve_Module.Module12;
				AREAControl->Snow_Module.Module12 = AREAControl->Ve_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T4_Module.Module12);
			}
			else
			{
				AREAControl->VNow_Module.Module12 = divdp( powdp(AREAControl->a_de_Module.Module12,2)*powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T5_Module.Module12),2), 
									2*(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12))+AREAControl->Ve_Module.Module12;
				
				AREAControl->Snow_Module.Module12 = divdp( (powdp((AREAControl->CurrentT_Module.Module12-AREAControl->T5_Module.Module12),3)+powdp((AREAControl->T5_Module.Module12-AREAControl->T4_Module.Module12),3))*powdp(AREAControl->a_de_Module.Module12,2), 
									6*(AREAControl->VmReal_Module.Module12-AREAControl->Ve_Module.Module12))+AREAControl->Ve_Module.Module12*(AREAControl->CurrentT_Module.Module12-AREAControl->T4_Module.Module12);

			}
			AREAControl->Snow_Module.Module12 += AREAControl->S1_Module.Module12+AREAControl->S2_Module.Module12+AREAControl->S3_Module.Module12+AREAControl->S4_Module.Module12;
		}
		else if( AREAControl->CurrentT_Module.Module12 > AREAControl->T5_Module.Module12 )		//尾巴段(余量段)
		{
			AREAControl->VNow_Module.Module12 = AREAControl->Ve_Module.Module12;
			AREAControl->Snow_Module.Module12 = AREAControl->Stotal_Module.Module12;
		}
		
		if(AREAControl->VNow_Module.Module12 < AREAControl->VMin_Module.Module12)	
			AREAControl->VNow_Module.Module12 = AREAControl->VMin_Module.Module12;		//the lowest move speed

		return 1;	
	}

}

Uint16 NCAreaCalNowValue(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign,Uint8 ModuleSel)
{
	Uint32 ri;
	GCODE *psAreaGcodeBufferPointer;
	Int32		DeltaStepAxis19_Compensation;
	Int32		DeltaStepAxis19_Compensation_2;
	static Int32		LensMotionMaxOffset = 35;

	ri = (AreaRunNCSign->RealRunCount-1) % STDGCODE_MOD;
	
	psAreaGcodeBufferPointer = AreaGcodeBufferPointer+ri;

	if(ModuleSel == 1)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module1 >= AREAControl->Stotal_Module.Module1) || fabs(AREAControl->Snow_Module.Module1 - AREAControl->Stotal_Module.Module1) < 0.001 )
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module1 = AREAControl->Stotal_Module.Module1;
			//AREAControl->VNow_Module.Module1 = AREAControl->Ve;
			
			#if 0
			AreaRunNCSign->NewAreaSign = 0;
			AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			
			AREAControl->NowValue.Axis15 = psAreaGcodeBufferPointer->EndPoint.Axis15;
			AREAControl->NowValue.Axis16 = psAreaGcodeBufferPointer->EndPoint.Axis16;
			AREAControl->NowValue.Axis1 = psAreaGcodeBufferPointer->EndPoint.Axis1;
			//AREAControl->NowValue.Axis2 = psAreaGcodeBufferPointer->EndPoint.Axis2;

			AreaRunNCSign->CodeRun_Over_Module.Module1 = 1;
			SPCNCSign.CodeRun_Over_Module.Module1 = 1;
			STDNCSign.CodeRun_Over_Module.Module1 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module1 > AREAControl->CurrentStotal_Module.Module1)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module1 = AREAControl->CurrentStotal_Module.Module1;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis15 = psAreaGcodeBufferPointer->EndPoint.Axis15;
			AREAControl->NowValue.Axis16 = psAreaGcodeBufferPointer->EndPoint.Axis16;
			AREAControl->NowValue.Axis1 = psAreaGcodeBufferPointer->EndPoint.Axis1;
			//AREAControl->NowValue.Axis2 = psAreaGcodeBufferPointer->EndPoint.Axis2;

			AreaRunNCSign->CodeRun_Over_Module.Module1 = 1;
			SPCNCSign.CodeRun_Over_Module.Module1 = 1;
			STDNCSign.CodeRun_Over_Module.Module1 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain ==0) || ( psAreaGcodeBufferPointer->CMDMain ==1))
			{
				AREAControl->DeltaSnow_Module.Module1 = AREAControl->Snow_Module.Module1-AREAControl->CurrentStotalLast_Module.Module1;
				AREAControl->NowValue.Axis15 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module1*AREAControl->Cosine.Axis15), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis15;
				AREAControl->NowValue.Axis16 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module1*AREAControl->Cosine.Axis16), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis16;
				AREAControl->NowValue.Axis1 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module1*AREAControl->Cosine.Axis1), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis1;
				//AREAControl->NowValue.Axis2 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module1*AREAControl->Cosine.Axis2), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis2;
			}
			
			return 0;
		}

	}
	else if(ModuleSel == 2)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module2 >= AREAControl->Stotal_Module.Module2) || fabs(AREAControl->Snow_Module.Module2 - AREAControl->Stotal_Module.Module2) < 0.001 )
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module2 = AREAControl->Stotal_Module.Module2;
			//AREAControl->VNow_Module.Module2 = AREAControl->Ve;

			#if 0
			AreaRunNCSign->NewAreaSign = 0;
			AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			AREAControl->NowValue.Axis17 = psAreaGcodeBufferPointer->EndPoint.Axis17;
			AREAControl->NowValue.Axis18 = psAreaGcodeBufferPointer->EndPoint.Axis18;
			
			AreaRunNCSign->CodeRun_Over_Module.Module2 = 1;
			SPCNCSign.CodeRun_Over_Module.Module2 = 1;
			STDNCSign.CodeRun_Over_Module.Module2 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module2 > AREAControl->CurrentStotal_Module.Module2)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module2 = AREAControl->CurrentStotal_Module.Module2;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis17 = psAreaGcodeBufferPointer->EndPoint.Axis17;
			AREAControl->NowValue.Axis18 = psAreaGcodeBufferPointer->EndPoint.Axis18;
			AreaRunNCSign->CodeRun_Over_Module.Module2 = 1;
			SPCNCSign.CodeRun_Over_Module.Module2 = 1;
			STDNCSign.CodeRun_Over_Module.Module2 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module2 = AREAControl->Snow_Module.Module2-AREAControl->CurrentStotalLast_Module.Module2;
				AREAControl->NowValue.Axis17 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module2*AREAControl->Cosine.Axis17), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis17;
				AREAControl->NowValue.Axis18 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module2*AREAControl->Cosine.Axis18), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis18;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 3)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module3 >= AREAControl->Stotal_Module.Module3) || fabs(AREAControl->Snow_Module.Module3 - AREAControl->Stotal_Module.Module3) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module3 = AREAControl->Stotal_Module.Module3;
			//AREAControl->VNow_Module.Module3 = AREAControl->Ve;


			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			AREAControl->NowValue.Axis3 = psAreaGcodeBufferPointer->EndPoint.Axis3;
			AREAControl->NowValue.Axis4 = psAreaGcodeBufferPointer->EndPoint.Axis4;
			AREAControl->NowValue.Axis5 = psAreaGcodeBufferPointer->EndPoint.Axis5;
			
			AreaRunNCSign->CodeRun_Over_Module.Module3 = 1;
			SPCNCSign.CodeRun_Over_Module.Module3 = 1;
			STDNCSign.CodeRun_Over_Module.Module3 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module3 > AREAControl->CurrentStotal_Module.Module3)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module3 = AREAControl->CurrentStotal_Module.Module3;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis3 = psAreaGcodeBufferPointer->EndPoint.Axis3;
			AREAControl->NowValue.Axis4 = psAreaGcodeBufferPointer->EndPoint.Axis4;
			AREAControl->NowValue.Axis5 = psAreaGcodeBufferPointer->EndPoint.Axis5;

			AreaRunNCSign->CodeRun_Over_Module.Module3 = 1;
			SPCNCSign.CodeRun_Over_Module.Module3 = 1;
			STDNCSign.CodeRun_Over_Module.Module3 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module3 = AREAControl->Snow_Module.Module3-AREAControl->CurrentStotalLast_Module.Module3;
				AREAControl->NowValue.Axis3 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module3*AREAControl->Cosine.Axis3), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis3;
				AREAControl->NowValue.Axis4 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module3*AREAControl->Cosine.Axis4), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis4;
				AREAControl->NowValue.Axis5= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module3*AREAControl->Cosine.Axis5), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis5;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 4)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module4 >= AREAControl->Stotal_Module.Module4) || fabs(AREAControl->Snow_Module.Module4 - AREAControl->Stotal_Module.Module4) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module4 = AREAControl->Stotal_Module.Module4;
			//AREAControl->VNow_Module.Module4 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			
			AREAControl->NowValue.Axis8 = psAreaGcodeBufferPointer->EndPoint.Axis8;		
			AREAControl->NowValue.Axis7 = psAreaGcodeBufferPointer->EndPoint.Axis7;
			
			AreaRunNCSign->CodeRun_Over_Module.Module4 = 1;
			SPCNCSign.CodeRun_Over_Module.Module4 = 1;
			STDNCSign.CodeRun_Over_Module.Module4 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module4 > AREAControl->CurrentStotal_Module.Module4)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module4 = AREAControl->CurrentStotal_Module.Module4;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis8 = psAreaGcodeBufferPointer->EndPoint.Axis8;		
			AREAControl->NowValue.Axis7 = psAreaGcodeBufferPointer->EndPoint.Axis7;
			AreaRunNCSign->CodeRun_Over_Module.Module4 = 1;
			SPCNCSign.CodeRun_Over_Module.Module4 = 1;
			STDNCSign.CodeRun_Over_Module.Module4 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module4 = AREAControl->Snow_Module.Module4-AREAControl->CurrentStotalLast_Module.Module4;
				AREAControl->NowValue.Axis8 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module4*AREAControl->Cosine.Axis8), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis8;
				AREAControl->NowValue.Axis7= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module4*AREAControl->Cosine.Axis7), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis7;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 5)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module5 >= AREAControl->Stotal_Module.Module5) || fabs(AREAControl->Snow_Module.Module5 - AREAControl->Stotal_Module.Module5) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module5 = AREAControl->Stotal_Module.Module5;
			//AREAControl->VNow_Module.Module5 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
			#endif
			
			AREAControl->NowValue.Axis6 = psAreaGcodeBufferPointer->EndPoint.Axis6;		
			AREAControl->NowValue.Axis20 = psAreaGcodeBufferPointer->EndPoint.Axis20;
			
			AreaRunNCSign->CodeRun_Over_Module.Module5 = 1;
			SPCNCSign.CodeRun_Over_Module.Module5 = 1;
			STDNCSign.CodeRun_Over_Module.Module5 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module5 > AREAControl->CurrentStotal_Module.Module5)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module5 = AREAControl->CurrentStotal_Module.Module5;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis6 = psAreaGcodeBufferPointer->EndPoint.Axis6;		
			AREAControl->NowValue.Axis20 = psAreaGcodeBufferPointer->EndPoint.Axis20;

			AreaRunNCSign->CodeRun_Over_Module.Module5 = 1;
			SPCNCSign.CodeRun_Over_Module.Module5 = 1;
			STDNCSign.CodeRun_Over_Module.Module5 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module5 = AREAControl->Snow_Module.Module5-AREAControl->CurrentStotalLast_Module.Module5;
				AREAControl->NowValue.Axis6 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module5*AREAControl->Cosine.Axis6), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis6;
				AREAControl->NowValue.Axis20 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module5*AREAControl->Cosine.Axis20), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis20;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 6)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module6 >= AREAControl->Stotal_Module.Module6) || fabs(AREAControl->Snow_Module.Module6 - AREAControl->Stotal_Module.Module6) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module6 = AREAControl->Stotal_Module.Module6;
			//AREAControl->VNow_Module.Module6 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			
			AREAControl->NowValue.Axis11 = psAreaGcodeBufferPointer->EndPoint.Axis11;		
			AREAControl->NowValue.Axis12 = psAreaGcodeBufferPointer->EndPoint.Axis12;
			
			AreaRunNCSign->CodeRun_Over_Module.Module6 = 1;
			SPCNCSign.CodeRun_Over_Module.Module6 = 1;
			STDNCSign.CodeRun_Over_Module.Module6 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module6 > AREAControl->CurrentStotal_Module.Module6)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module6 = AREAControl->CurrentStotal_Module.Module6;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis11 = psAreaGcodeBufferPointer->EndPoint.Axis11;		
			AREAControl->NowValue.Axis12 = psAreaGcodeBufferPointer->EndPoint.Axis12;
			
			AreaRunNCSign->CodeRun_Over_Module.Module6 = 1;
			SPCNCSign.CodeRun_Over_Module.Module6 = 1;
			STDNCSign.CodeRun_Over_Module.Module6 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module6 = AREAControl->Snow_Module.Module6-AREAControl->CurrentStotalLast_Module.Module6;
				AREAControl->NowValue.Axis11 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module6*AREAControl->Cosine.Axis11), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis11;
				AREAControl->NowValue.Axis12= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module6*AREAControl->Cosine.Axis12), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis12;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 7)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module7 >= AREAControl->Stotal_Module.Module7) || fabs(AREAControl->Snow_Module.Module7 - AREAControl->Stotal_Module.Module7) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module7 = AREAControl->Stotal_Module.Module7;
			//AREAControl->VNow_Module.Module7 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			AREAControl->NowValue.Axis21 = psAreaGcodeBufferPointer->EndPoint.Axis21;
			AREAControl->NowValue.Axis22 = psAreaGcodeBufferPointer->EndPoint.Axis22;
			AREAControl->NowValue.Axis9 = psAreaGcodeBufferPointer->EndPoint.Axis9;		
			
			AreaRunNCSign->CodeRun_Over_Module.Module7 = 1;
			SPCNCSign.CodeRun_Over_Module.Module7 = 1;
			STDNCSign.CodeRun_Over_Module.Module7 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module7 > AREAControl->CurrentStotal_Module.Module7)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module7 = AREAControl->CurrentStotal_Module.Module7;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			AREAControl->NowValue.Axis21 = psAreaGcodeBufferPointer->EndPoint.Axis21;
			AREAControl->NowValue.Axis22 = psAreaGcodeBufferPointer->EndPoint.Axis22;
			AREAControl->NowValue.Axis9 = psAreaGcodeBufferPointer->EndPoint.Axis9;		

			AreaRunNCSign->CodeRun_Over_Module.Module7 = 1;
			SPCNCSign.CodeRun_Over_Module.Module7 = 1;
			STDNCSign.CodeRun_Over_Module.Module7 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module7 = AREAControl->Snow_Module.Module7-AREAControl->CurrentStotalLast_Module.Module7;
				AREAControl->NowValue.Axis21 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module7*AREAControl->Cosine.Axis21), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis21;
				AREAControl->NowValue.Axis22= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module7*AREAControl->Cosine.Axis22), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis22;
				AREAControl->NowValue.Axis9 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module7*AREAControl->Cosine.Axis9), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis9;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 8)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module8 >= AREAControl->Stotal_Module.Module8) || fabs(AREAControl->Snow_Module.Module8 - AREAControl->Stotal_Module.Module8) < 0.001 )
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module8 = AREAControl->Stotal_Module.Module8;
			//AREAControl->VNow_Module.Module8 = AREAControl->Ve;
			
		#if 0
			AreaRunNCSign->NewAreaSign = 0;
			AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
		#endif
			
			AREAControl->NowValue.Axis2 = psAreaGcodeBufferPointer->EndPoint.Axis2;
			AreaRunNCSign->CodeRun_Over_Module.Module8 = 1;
			SPCNCSign.CodeRun_Over_Module.Module8 = 1;
			STDNCSign.CodeRun_Over_Module.Module8 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module8 > AREAControl->CurrentStotal_Module.Module8)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module8 = AREAControl->CurrentStotal_Module.Module8;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint; 			
			#endif
			
			AREAControl->NowValue.Axis2 = psAreaGcodeBufferPointer->EndPoint.Axis2;

			AreaRunNCSign->CodeRun_Over_Module.Module8 = 1;
			SPCNCSign.CodeRun_Over_Module.Module8 = 1;
			STDNCSign.CodeRun_Over_Module.Module8 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{			
				AREAControl->DeltaSnow_Module.Module8 = AREAControl->Snow_Module.Module8-AREAControl->CurrentStotalLast_Module.Module8;
				AREAControl->NowValue.Axis2 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module8*AREAControl->Cosine.Axis2), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis2;
			}			
			
			return 0;
		}

	}
	else if(ModuleSel == 9)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module9 >= AREAControl->Stotal_Module.Module9) || fabs(AREAControl->Snow_Module.Module9 - AREAControl->Stotal_Module.Module9) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module9 = AREAControl->Stotal_Module.Module9;
			//AREAControl->VNow_Module.Module9 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			//AREAControl->NowValue.Axis22 = psAreaGcodeBufferPointer->EndPoint.Axis22;
			AREAControl->NowValue.Axis10 = psAreaGcodeBufferPointer->EndPoint.Axis10;		
			
			AreaRunNCSign->CodeRun_Over_Module.Module9 = 1;
			SPCNCSign.CodeRun_Over_Module.Module9 = 1;
			STDNCSign.CodeRun_Over_Module.Module9 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module9 > AREAControl->CurrentStotal_Module.Module9)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module9 = AREAControl->CurrentStotal_Module.Module9;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			//AREAControl->NowValue.Axis22 = psAreaGcodeBufferPointer->EndPoint.Axis22;
			AREAControl->NowValue.Axis10 = psAreaGcodeBufferPointer->EndPoint.Axis10;		

			AreaRunNCSign->CodeRun_Over_Module.Module9 = 1;
			SPCNCSign.CodeRun_Over_Module.Module9 = 1;
			STDNCSign.CodeRun_Over_Module.Module9 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module9 = AREAControl->Snow_Module.Module9-AREAControl->CurrentStotalLast_Module.Module9;
				//AREAControl->NowValue.Axis22= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module9*AREAControl->Cosine.Axis22), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis22;
				AREAControl->NowValue.Axis10 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module9*AREAControl->Cosine.Axis10), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis10;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 10)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module10 >= AREAControl->Stotal_Module.Module10) || fabs(AREAControl->Snow_Module.Module10 - AREAControl->Stotal_Module.Module10) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module10 = AREAControl->Stotal_Module.Module10;
			//AREAControl->VNow_Module.Module10 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			
			AREAControl->NowValue.Axis13 = psAreaGcodeBufferPointer->EndPoint.Axis13;		
			AREAControl->NowValue.Axis14 = psAreaGcodeBufferPointer->EndPoint.Axis14;
			
			AreaRunNCSign->CodeRun_Over_Module.Module10 = 1;
			SPCNCSign.CodeRun_Over_Module.Module10 = 1;
			STDNCSign.CodeRun_Over_Module.Module10 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module10 > AREAControl->CurrentStotal_Module.Module10)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module10 = AREAControl->CurrentStotal_Module.Module10;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			
			AREAControl->NowValue.Axis13 = psAreaGcodeBufferPointer->EndPoint.Axis13;		
			AREAControl->NowValue.Axis14 = psAreaGcodeBufferPointer->EndPoint.Axis14;
			
			AreaRunNCSign->CodeRun_Over_Module.Module10 = 1;
			SPCNCSign.CodeRun_Over_Module.Module10 = 1;
			STDNCSign.CodeRun_Over_Module.Module10 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module10 = AREAControl->Snow_Module.Module10-AREAControl->CurrentStotalLast_Module.Module10;
				AREAControl->NowValue.Axis13 = (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module10*AREAControl->Cosine.Axis13), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis13;
				AREAControl->NowValue.Axis14= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module10*AREAControl->Cosine.Axis14), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis14;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 11)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module11 >= AREAControl->Stotal_Module.Module11) || fabs(AREAControl->Snow_Module.Module11 - AREAControl->Stotal_Module.Module11) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module11 = AREAControl->Stotal_Module.Module11;
			//AREAControl->VNow_Module.Module11 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			AREAControl->NowValue.Axis19 = psAreaGcodeBufferPointer->EndPoint.Axis19;
			#if 0

			if(OverallSign.LensMotionOverSign != LensMotionCompensationRun)
			{
				AreaRunNCSign->CodeRun_Over_Module.Module11 = 1;
				SPCNCSign.CodeRun_Over_Module.Module11 = 1;
				STDNCSign.CodeRun_Over_Module.Module11 = 1;
				OverallSign.LensMotionOverSign = LensMotionNormal;
				return 1;
			}
			#endif

			#if 1
			if((System.FunctionSelect04 == 1)&&
				(OverallSign.CameraScanSign == 0)&&
				(AREAControl->Stotal_Module.Module11 > 0.0005)
				&&
				(Reference.RefStep >= 4)
				//(AREAControl->Stotal_Module.Module11 > 0.0025)
				)
			{
				if(OverallSign.LensMotionOverSign == LensMotionCompensationOver)
				{
					AreaRunNCSign->CodeRun_Over_Module.Module11 = 1;
					SPCNCSign.CodeRun_Over_Module.Module11 = 1;
					STDNCSign.CodeRun_Over_Module.Module11 = 1;
					OverallSign.LensMotionOverSign = LensMotionNormal;
					return 1;
				}

				DeltaStepAxis19_Compensation_2 = (AREAControl->NowValue.Axis19-OverallSign.ABS_Coordinate.Axis19);
				
				if(fabs(DeltaStepAxis19_Compensation_2) > LensMotionMaxOffset)
				{
					OverallSign.LensMotionOverSign = OverallSign.LensMotionOverSign;
					OverallSign.LensMotionOverSign = LensMotionNormal;
					return 0;
				}
				
				OverallSign.LensMotionOverSign = LensMotionCompensationRun;
				return 0;
			}
			else
			{
				AreaRunNCSign->CodeRun_Over_Module.Module11 = 1;
				SPCNCSign.CodeRun_Over_Module.Module11 = 1;
				STDNCSign.CodeRun_Over_Module.Module11 = 1;
				OverallSign.LensMotionOverSign = LensMotionNormal;
				return 1;
			}			
			#endif
			
			#if 0
			AreaRunNCSign->CodeRun_Over_Module.Module11 = 1;
			SPCNCSign.CodeRun_Over_Module.Module11 = 1;
			STDNCSign.CodeRun_Over_Module.Module11 = 1;		
			return 1;
			#endif
		}
		else if(AREAControl->Snow_Module.Module11 > AREAControl->CurrentStotal_Module.Module11)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module11 = AREAControl->CurrentStotal_Module.Module11;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			AREAControl->NowValue.Axis19 = psAreaGcodeBufferPointer->EndPoint.Axis19;

			AreaRunNCSign->CodeRun_Over_Module.Module11 = 1;
			SPCNCSign.CodeRun_Over_Module.Module11 = 1;
			STDNCSign.CodeRun_Over_Module.Module11 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module11 = AREAControl->Snow_Module.Module11-AREAControl->CurrentStotalLast_Module.Module11;
				AREAControl->NowValue.Axis19= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module11*AREAControl->Cosine.Axis19), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis19;
			}
			
			return 0;
		}
	}
	else if(ModuleSel == 12)
	{		
		//求得snow和vnow后,分三种情况讨论,计算输出!
		if( (AREAControl->Snow_Module.Module12 >= AREAControl->Stotal_Module.Module12) || fabs(AREAControl->Snow_Module.Module12 - AREAControl->Stotal_Module.Module12) < 0.001 )  
		{	//第一种情况:已到达区域终点
			AREAControl->Snow_Module.Module12 = AREAControl->Stotal_Module.Module12;
			//AREAControl->VNow_Module.Module12 = AREAControl->Ve;

			#if 0
				AreaRunNCSign->NewAreaSign = 0;
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;
			#endif
			AREAControl->NowValue.Axis23 = psAreaGcodeBufferPointer->EndPoint.Axis23;
			
			AreaRunNCSign->CodeRun_Over_Module.Module12 = 1;
			SPCNCSign.CodeRun_Over_Module.Module12 = 1;
			STDNCSign.CodeRun_Over_Module.Module12 = 1;
			
			return 1;
		}
		else if(AREAControl->Snow_Module.Module12 > AREAControl->CurrentStotal_Module.Module12)
		{	//第二种情况:已到达当前代码终点
			AREAControl->Snow_Module.Module12 = AREAControl->CurrentStotal_Module.Module12;
			#if 0				
				AREAControl->NowValue = psAreaGcodeBufferPointer->EndPoint;				
			#endif
			AREAControl->NowValue.Axis23 = psAreaGcodeBufferPointer->EndPoint.Axis23;

			AreaRunNCSign->CodeRun_Over_Module.Module12 = 1;
			SPCNCSign.CodeRun_Over_Module.Module12 = 1;
			STDNCSign.CodeRun_Over_Module.Module12 = 1;
			
			return 0;
		}
		else
		{	//第三种情况:既不是区域终点,也不是当前代码终点,计算Nowvalue.
			if( ( psAreaGcodeBufferPointer->CMDMain==0) || ( psAreaGcodeBufferPointer->CMDMain==1))
			{
				AREAControl->DeltaSnow_Module.Module12 = AREAControl->Snow_Module.Module12-AREAControl->CurrentStotalLast_Module.Module12;
				AREAControl->NowValue.Axis23= (Int32)( divdp( (AREAControl->DeltaSnow_Module.Module12*AREAControl->Cosine.Axis23), System.UnitTo_mm ) )+psAreaGcodeBufferPointer->StartPoint.Axis23;
			}
			
			return 0;
		}
	}

}


void NCArea_check_endpoint_value(AREA_CONTROL *AREAControl)	//检查插补点有无超出终点范围
{

	if(OverallSign.TheoryMoveSign.Axis1 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis1 > AREAControl->EndPointMachPulseOut.Axis1)
		{
			AREAControl->NowPointMACHPulseOut.Axis1 = AREAControl->EndPointMachPulseOut.Axis1;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis1 < AREAControl->EndPointMachPulseOut.Axis1)
		{
			AREAControl->NowPointMACHPulseOut.Axis1 = AREAControl->EndPointMachPulseOut.Axis1;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis2 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis2 > AREAControl->EndPointMachPulseOut.Axis2)
		{
			AREAControl->NowPointMACHPulseOut.Axis2 = AREAControl->EndPointMachPulseOut.Axis2;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis2 < AREAControl->EndPointMachPulseOut.Axis2)
		{
			AREAControl->NowPointMACHPulseOut.Axis2 = AREAControl->EndPointMachPulseOut.Axis2;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis3 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis3 > AREAControl->EndPointMachPulseOut.Axis3)
		{
			AREAControl->NowPointMACHPulseOut.Axis3 = AREAControl->EndPointMachPulseOut.Axis3;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis3 < AREAControl->EndPointMachPulseOut.Axis3)
		{
			AREAControl->NowPointMACHPulseOut.Axis3 = AREAControl->EndPointMachPulseOut.Axis3;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis4 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis4 > AREAControl->EndPointMachPulseOut.Axis4)
		{
			AREAControl->NowPointMACHPulseOut.Axis4 = AREAControl->EndPointMachPulseOut.Axis4;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis4 < AREAControl->EndPointMachPulseOut.Axis4)
		{
			AREAControl->NowPointMACHPulseOut.Axis4 = AREAControl->EndPointMachPulseOut.Axis4;
		}
	}	

	if(OverallSign.TheoryMoveSign.Axis5 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis5 > AREAControl->EndPointMachPulseOut.Axis5)
		{
			AREAControl->NowPointMACHPulseOut.Axis5 = AREAControl->EndPointMachPulseOut.Axis5;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis5 < AREAControl->EndPointMachPulseOut.Axis5)
		{
			AREAControl->NowPointMACHPulseOut.Axis5 = AREAControl->EndPointMachPulseOut.Axis5;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis6 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis6 > AREAControl->EndPointMachPulseOut.Axis6)
		{
			AREAControl->NowPointMACHPulseOut.Axis6 = AREAControl->EndPointMachPulseOut.Axis6;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis6 < AREAControl->EndPointMachPulseOut.Axis6)
		{
			AREAControl->NowPointMACHPulseOut.Axis6 = AREAControl->EndPointMachPulseOut.Axis6;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis7 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis7 > AREAControl->EndPointMachPulseOut.Axis7)
		{
			AREAControl->NowPointMACHPulseOut.Axis7 = AREAControl->EndPointMachPulseOut.Axis7;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis7 < AREAControl->EndPointMachPulseOut.Axis7)
		{
			AREAControl->NowPointMACHPulseOut.Axis7 = AREAControl->EndPointMachPulseOut.Axis7;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis8 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis8 > AREAControl->EndPointMachPulseOut.Axis8)
		{
			AREAControl->NowPointMACHPulseOut.Axis8 = AREAControl->EndPointMachPulseOut.Axis8;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis8 < AREAControl->EndPointMachPulseOut.Axis8)
		{
			AREAControl->NowPointMACHPulseOut.Axis8 = AREAControl->EndPointMachPulseOut.Axis8;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis9 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis9 > AREAControl->EndPointMachPulseOut.Axis9)
		{
			AREAControl->NowPointMACHPulseOut.Axis9 = AREAControl->EndPointMachPulseOut.Axis9;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis9 < AREAControl->EndPointMachPulseOut.Axis9)
		{
			AREAControl->NowPointMACHPulseOut.Axis9 = AREAControl->EndPointMachPulseOut.Axis9;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis10 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis10 > AREAControl->EndPointMachPulseOut.Axis10)
		{
			AREAControl->NowPointMACHPulseOut.Axis10 = AREAControl->EndPointMachPulseOut.Axis10;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis10 < AREAControl->EndPointMachPulseOut.Axis10)
		{
			AREAControl->NowPointMACHPulseOut.Axis10 = AREAControl->EndPointMachPulseOut.Axis10;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis11 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis11 > AREAControl->EndPointMachPulseOut.Axis11)
		{
			AREAControl->NowPointMACHPulseOut.Axis11 = AREAControl->EndPointMachPulseOut.Axis11;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis11 < AREAControl->EndPointMachPulseOut.Axis11)
		{
			AREAControl->NowPointMACHPulseOut.Axis11 = AREAControl->EndPointMachPulseOut.Axis11;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis12 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis12 > AREAControl->EndPointMachPulseOut.Axis12)
		{
			AREAControl->NowPointMACHPulseOut.Axis12 = AREAControl->EndPointMachPulseOut.Axis12;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis12 < AREAControl->EndPointMachPulseOut.Axis12)
		{
			AREAControl->NowPointMACHPulseOut.Axis12 = AREAControl->EndPointMachPulseOut.Axis12;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis13 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis13 > AREAControl->EndPointMachPulseOut.Axis13)
		{
			AREAControl->NowPointMACHPulseOut.Axis13 = AREAControl->EndPointMachPulseOut.Axis13;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis13 < AREAControl->EndPointMachPulseOut.Axis13)
		{
			AREAControl->NowPointMACHPulseOut.Axis13 = AREAControl->EndPointMachPulseOut.Axis13;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis14 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis14 > AREAControl->EndPointMachPulseOut.Axis14)
		{
			AREAControl->NowPointMACHPulseOut.Axis14 = AREAControl->EndPointMachPulseOut.Axis14;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis14 < AREAControl->EndPointMachPulseOut.Axis14)
		{
			AREAControl->NowPointMACHPulseOut.Axis14 = AREAControl->EndPointMachPulseOut.Axis14;
		}
	}	

	if(OverallSign.TheoryMoveSign.Axis15 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis15 > AREAControl->EndPointMachPulseOut.Axis15)
		{
			AREAControl->NowPointMACHPulseOut.Axis15 = AREAControl->EndPointMachPulseOut.Axis15;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis15 < AREAControl->EndPointMachPulseOut.Axis15)
		{
			AREAControl->NowPointMACHPulseOut.Axis15 = AREAControl->EndPointMachPulseOut.Axis15;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis16 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis16 > AREAControl->EndPointMachPulseOut.Axis16)
		{
			AREAControl->NowPointMACHPulseOut.Axis16 = AREAControl->EndPointMachPulseOut.Axis16;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis16 < AREAControl->EndPointMachPulseOut.Axis16)
		{
			AREAControl->NowPointMACHPulseOut.Axis16 = AREAControl->EndPointMachPulseOut.Axis16;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis17 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis17 > AREAControl->EndPointMachPulseOut.Axis17)
		{
			AREAControl->NowPointMACHPulseOut.Axis17 = AREAControl->EndPointMachPulseOut.Axis17;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis17 < AREAControl->EndPointMachPulseOut.Axis17)
		{
			AREAControl->NowPointMACHPulseOut.Axis17 = AREAControl->EndPointMachPulseOut.Axis17;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis18 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis18 > AREAControl->EndPointMachPulseOut.Axis18)
		{
			AREAControl->NowPointMACHPulseOut.Axis18 = AREAControl->EndPointMachPulseOut.Axis18;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis18 < AREAControl->EndPointMachPulseOut.Axis18)
		{
			AREAControl->NowPointMACHPulseOut.Axis18 = AREAControl->EndPointMachPulseOut.Axis18;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis19 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis19 > AREAControl->EndPointMachPulseOut.Axis19)
		{
			AREAControl->NowPointMACHPulseOut.Axis19 = AREAControl->EndPointMachPulseOut.Axis19;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis19 < AREAControl->EndPointMachPulseOut.Axis19)
		{
			AREAControl->NowPointMACHPulseOut.Axis19 = AREAControl->EndPointMachPulseOut.Axis19;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis20 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis20 > AREAControl->EndPointMachPulseOut.Axis20)
		{
			AREAControl->NowPointMACHPulseOut.Axis20 = AREAControl->EndPointMachPulseOut.Axis20;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis20 < AREAControl->EndPointMachPulseOut.Axis20)
		{
			AREAControl->NowPointMACHPulseOut.Axis20 = AREAControl->EndPointMachPulseOut.Axis20;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis21 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis21 > AREAControl->EndPointMachPulseOut.Axis21)
		{
			AREAControl->NowPointMACHPulseOut.Axis21 = AREAControl->EndPointMachPulseOut.Axis21;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis21 < AREAControl->EndPointMachPulseOut.Axis21)
		{
			AREAControl->NowPointMACHPulseOut.Axis21 = AREAControl->EndPointMachPulseOut.Axis21;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis22 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis22 > AREAControl->EndPointMachPulseOut.Axis22)
		{
			AREAControl->NowPointMACHPulseOut.Axis22 = AREAControl->EndPointMachPulseOut.Axis22;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis22 < AREAControl->EndPointMachPulseOut.Axis22)
		{
			AREAControl->NowPointMACHPulseOut.Axis22 = AREAControl->EndPointMachPulseOut.Axis22;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis23 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis23 > AREAControl->EndPointMachPulseOut.Axis23)
		{
			AREAControl->NowPointMACHPulseOut.Axis23 = AREAControl->EndPointMachPulseOut.Axis23;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis23 < AREAControl->EndPointMachPulseOut.Axis23)
		{
			AREAControl->NowPointMACHPulseOut.Axis23 = AREAControl->EndPointMachPulseOut.Axis23;
		}
	}

}

//由于浮点计算精度问题，会造成位移很小轴计算位移值轻微抖动
//故需要根据理论方向修正抖动
void NCArea_check_point_direction(AREA_CONTROL *AREAControl)
{
	if(OverallSign.TheoryMoveSign.Axis1 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis1 < AREAControl->OldPointMACHPulseOut.Axis1)
		{
			AREAControl->NowPointMACHPulseOut.Axis1 = AREAControl->OldPointMACHPulseOut.Axis1;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis1 > AREAControl->OldPointMACHPulseOut.Axis1)
		{
			AREAControl->NowPointMACHPulseOut.Axis1 = AREAControl->OldPointMACHPulseOut.Axis1;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis2 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis2 < AREAControl->OldPointMACHPulseOut.Axis2)
		{
			AREAControl->NowPointMACHPulseOut.Axis2 = AREAControl->OldPointMACHPulseOut.Axis2;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis2 > AREAControl->OldPointMACHPulseOut.Axis2)
		{
			AREAControl->NowPointMACHPulseOut.Axis2 = AREAControl->OldPointMACHPulseOut.Axis2;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis3 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis3 < AREAControl->OldPointMACHPulseOut.Axis3)
		{
			AREAControl->NowPointMACHPulseOut.Axis3 = AREAControl->OldPointMACHPulseOut.Axis3;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis3 > AREAControl->OldPointMACHPulseOut.Axis3)
		{
			AREAControl->NowPointMACHPulseOut.Axis3 = AREAControl->OldPointMACHPulseOut.Axis3;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis4 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis4 < AREAControl->OldPointMACHPulseOut.Axis4)
		{
			AREAControl->NowPointMACHPulseOut.Axis4 = AREAControl->OldPointMACHPulseOut.Axis4;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis4 > AREAControl->OldPointMACHPulseOut.Axis4)
		{
			AREAControl->NowPointMACHPulseOut.Axis4 = AREAControl->OldPointMACHPulseOut.Axis4;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis5 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis5 < AREAControl->OldPointMACHPulseOut.Axis5)
		{
			AREAControl->NowPointMACHPulseOut.Axis5 = AREAControl->OldPointMACHPulseOut.Axis5;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis5 > AREAControl->OldPointMACHPulseOut.Axis5)
		{
			AREAControl->NowPointMACHPulseOut.Axis5 = AREAControl->OldPointMACHPulseOut.Axis5;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis6 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis6 < AREAControl->OldPointMACHPulseOut.Axis6)
		{
			AREAControl->NowPointMACHPulseOut.Axis6 = AREAControl->OldPointMACHPulseOut.Axis6;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis6 > AREAControl->OldPointMACHPulseOut.Axis6)
		{
			AREAControl->NowPointMACHPulseOut.Axis6 = AREAControl->OldPointMACHPulseOut.Axis6;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis7 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis7 < AREAControl->OldPointMACHPulseOut.Axis7)
		{
			AREAControl->NowPointMACHPulseOut.Axis7 = AREAControl->OldPointMACHPulseOut.Axis7;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis7 > AREAControl->OldPointMACHPulseOut.Axis7)
		{
			AREAControl->NowPointMACHPulseOut.Axis7 = AREAControl->OldPointMACHPulseOut.Axis7;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis8 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis8 < AREAControl->OldPointMACHPulseOut.Axis8)
		{
			AREAControl->NowPointMACHPulseOut.Axis8 = AREAControl->OldPointMACHPulseOut.Axis8;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis8 > AREAControl->OldPointMACHPulseOut.Axis8)
		{
			AREAControl->NowPointMACHPulseOut.Axis8 = AREAControl->OldPointMACHPulseOut.Axis8;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis9 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis9 < AREAControl->OldPointMACHPulseOut.Axis9)
		{
			AREAControl->NowPointMACHPulseOut.Axis9 = AREAControl->OldPointMACHPulseOut.Axis9;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis9 > AREAControl->OldPointMACHPulseOut.Axis9)
		{
			AREAControl->NowPointMACHPulseOut.Axis9 = AREAControl->OldPointMACHPulseOut.Axis9;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis10 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis10 < AREAControl->OldPointMACHPulseOut.Axis10)
		{
			AREAControl->NowPointMACHPulseOut.Axis10 = AREAControl->OldPointMACHPulseOut.Axis10;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis10 > AREAControl->OldPointMACHPulseOut.Axis10)
		{
			AREAControl->NowPointMACHPulseOut.Axis10 = AREAControl->OldPointMACHPulseOut.Axis10;
		}
	}

	
	if(OverallSign.TheoryMoveSign.Axis11 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis11 < AREAControl->OldPointMACHPulseOut.Axis11)
		{
			AREAControl->NowPointMACHPulseOut.Axis11 = AREAControl->OldPointMACHPulseOut.Axis11;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis11 > AREAControl->OldPointMACHPulseOut.Axis11)
		{
			AREAControl->NowPointMACHPulseOut.Axis11 = AREAControl->OldPointMACHPulseOut.Axis11;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis12 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis12 < AREAControl->OldPointMACHPulseOut.Axis12)
		{
			AREAControl->NowPointMACHPulseOut.Axis12 = AREAControl->OldPointMACHPulseOut.Axis12;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis12 > AREAControl->OldPointMACHPulseOut.Axis12)
		{
			AREAControl->NowPointMACHPulseOut.Axis12 = AREAControl->OldPointMACHPulseOut.Axis12;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis13 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis13 < AREAControl->OldPointMACHPulseOut.Axis13)
		{
			AREAControl->NowPointMACHPulseOut.Axis13 = AREAControl->OldPointMACHPulseOut.Axis13;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis13 > AREAControl->OldPointMACHPulseOut.Axis13)
		{
			AREAControl->NowPointMACHPulseOut.Axis13 = AREAControl->OldPointMACHPulseOut.Axis13;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis14 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis14 < AREAControl->OldPointMACHPulseOut.Axis14)
		{
			AREAControl->NowPointMACHPulseOut.Axis14 = AREAControl->OldPointMACHPulseOut.Axis14;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis14 > AREAControl->OldPointMACHPulseOut.Axis14)
		{
			AREAControl->NowPointMACHPulseOut.Axis14 = AREAControl->OldPointMACHPulseOut.Axis14;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis15 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis15 < AREAControl->OldPointMACHPulseOut.Axis15)
		{
			AREAControl->NowPointMACHPulseOut.Axis15 = AREAControl->OldPointMACHPulseOut.Axis15;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis15 > AREAControl->OldPointMACHPulseOut.Axis15)
		{
			AREAControl->NowPointMACHPulseOut.Axis15 = AREAControl->OldPointMACHPulseOut.Axis15;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis16 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis16 < AREAControl->OldPointMACHPulseOut.Axis16)
		{
			AREAControl->NowPointMACHPulseOut.Axis16 = AREAControl->OldPointMACHPulseOut.Axis16;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis16 > AREAControl->OldPointMACHPulseOut.Axis16)
		{
			AREAControl->NowPointMACHPulseOut.Axis16 = AREAControl->OldPointMACHPulseOut.Axis16;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis17 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis17 < AREAControl->OldPointMACHPulseOut.Axis17)
		{
			AREAControl->NowPointMACHPulseOut.Axis17 = AREAControl->OldPointMACHPulseOut.Axis17;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis17 > AREAControl->OldPointMACHPulseOut.Axis17)
		{
			AREAControl->NowPointMACHPulseOut.Axis17 = AREAControl->OldPointMACHPulseOut.Axis17;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis18 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis18 < AREAControl->OldPointMACHPulseOut.Axis18)
		{
			AREAControl->NowPointMACHPulseOut.Axis18 = AREAControl->OldPointMACHPulseOut.Axis18;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis18 > AREAControl->OldPointMACHPulseOut.Axis18)
		{
			AREAControl->NowPointMACHPulseOut.Axis18 = AREAControl->OldPointMACHPulseOut.Axis18;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis19 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis19 < AREAControl->OldPointMACHPulseOut.Axis19)
		{
			AREAControl->NowPointMACHPulseOut.Axis19 = AREAControl->OldPointMACHPulseOut.Axis19;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis19 > AREAControl->OldPointMACHPulseOut.Axis19)
		{
			AREAControl->NowPointMACHPulseOut.Axis19 = AREAControl->OldPointMACHPulseOut.Axis19;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis20 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis20 < AREAControl->OldPointMACHPulseOut.Axis20)
		{
			AREAControl->NowPointMACHPulseOut.Axis20 = AREAControl->OldPointMACHPulseOut.Axis20;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis20 > AREAControl->OldPointMACHPulseOut.Axis20)
		{
			AREAControl->NowPointMACHPulseOut.Axis20 = AREAControl->OldPointMACHPulseOut.Axis20;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis21 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis21 < AREAControl->OldPointMACHPulseOut.Axis21)
		{
			AREAControl->NowPointMACHPulseOut.Axis21 = AREAControl->OldPointMACHPulseOut.Axis21;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis21 > AREAControl->OldPointMACHPulseOut.Axis21)
		{
			AREAControl->NowPointMACHPulseOut.Axis21 = AREAControl->OldPointMACHPulseOut.Axis21;
		}
	}

	if(OverallSign.TheoryMoveSign.Axis22 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis22 < AREAControl->OldPointMACHPulseOut.Axis22)
		{
			AREAControl->NowPointMACHPulseOut.Axis22 = AREAControl->OldPointMACHPulseOut.Axis22;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis22 > AREAControl->OldPointMACHPulseOut.Axis22)
		{
			AREAControl->NowPointMACHPulseOut.Axis22 = AREAControl->OldPointMACHPulseOut.Axis22;
		}
	}
	
	if(OverallSign.TheoryMoveSign.Axis23 == MovePositive)
	{
		if(AREAControl->NowPointMACHPulseOut.Axis23 < AREAControl->OldPointMACHPulseOut.Axis23)
		{
			AREAControl->NowPointMACHPulseOut.Axis23 = AREAControl->OldPointMACHPulseOut.Axis23;
		}
	}
	else 
	{
		if(AREAControl->NowPointMACHPulseOut.Axis23 > AREAControl->OldPointMACHPulseOut.Axis23)
		{
			AREAControl->NowPointMACHPulseOut.Axis23 = AREAControl->OldPointMACHPulseOut.Axis23;
		}
	}

}


//STD/SPC 区域曲线实际处理GAP消除 
void NCAreaEnsureDirection(AREA_CONTROL *AREAControl, NC_RUN_SIGN *AreaRunNCSign)	
{   
	if(AREAControl->DeltaStep.Axis1>0)
	{
		if(OverallSign.MoveSign.Axis1==0)
		{
			AREAControl->DeltaStep.Axis1 = AREAControl->DeltaStep.Axis1 + System.GapPulse.Axis1;
			AreaRunNCSign->NeedBack.Axis1 = 0;
		}
		OverallSign.MoveSign.Axis1 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis1<0)
	{
		if(OverallSign.MoveSign.Axis1==1) 
		{
			AREAControl->DeltaStep.Axis1 = AREAControl->DeltaStep.Axis1 - System.GapPulse.Axis1;
			AreaRunNCSign->NeedBack.Axis1 = 0;
		}
		OverallSign.MoveSign.Axis1 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis1 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis1==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis1 = System.GapPulse.Axis1;
			OverallSign.MoveSign.Axis1 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis1==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis1 = -System.GapPulse.Axis1;
			OverallSign.MoveSign.Axis1 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis1 = 0;	
	}

	if(AREAControl->DeltaStep.Axis2>0)
	{
		if(OverallSign.MoveSign.Axis2==0)
		{
			AREAControl->DeltaStep.Axis2 = AREAControl->DeltaStep.Axis2 + System.GapPulse.Axis2;
			AreaRunNCSign->NeedBack.Axis2 = 0;
		}
		OverallSign.MoveSign.Axis2 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis2<0)
	{
		if(OverallSign.MoveSign.Axis2==1) 
		{
			AREAControl->DeltaStep.Axis2 = AREAControl->DeltaStep.Axis2 - System.GapPulse.Axis2;
			AreaRunNCSign->NeedBack.Axis2 = 0;
		}
		OverallSign.MoveSign.Axis2 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis2 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis2==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis2 = System.GapPulse.Axis2;
			OverallSign.MoveSign.Axis2 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis2==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis2 = -System.GapPulse.Axis2;
			OverallSign.MoveSign.Axis2 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis2 = 0;	
	}
	
	if(AREAControl->DeltaStep.Axis3>0)
	{
		if(OverallSign.MoveSign.Axis3==0)
		{
			AREAControl->DeltaStep.Axis3 = AREAControl->DeltaStep.Axis3 + System.GapPulse.Axis3;
			AreaRunNCSign->NeedBack.Axis3 = 0;
		}
		OverallSign.MoveSign.Axis3 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis3<0)
	{
		if(OverallSign.MoveSign.Axis3==1) 
		{
			AREAControl->DeltaStep.Axis3 = AREAControl->DeltaStep.Axis3 - System.GapPulse.Axis3;
			AreaRunNCSign->NeedBack.Axis3 = 0;
		}
		OverallSign.MoveSign.Axis3 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis3 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis3==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis3 = System.GapPulse.Axis3;
			OverallSign.MoveSign.Axis3 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis3==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis3 = -System.GapPulse.Axis3;
			OverallSign.MoveSign.Axis3 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis3 = 0;	
	}

	if(AREAControl->DeltaStep.Axis4>0)
	{
		if(OverallSign.MoveSign.Axis4==0)
		{
			AREAControl->DeltaStep.Axis4 = AREAControl->DeltaStep.Axis4 + System.GapPulse.Axis4;
			AreaRunNCSign->NeedBack.Axis4 = 0;
		}
		OverallSign.MoveSign.Axis4 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis4<0)
	{
		if(OverallSign.MoveSign.Axis4==1) 
		{
			AREAControl->DeltaStep.Axis4 = AREAControl->DeltaStep.Axis4 - System.GapPulse.Axis4;
			AreaRunNCSign->NeedBack.Axis4 = 0;
		}
		OverallSign.MoveSign.Axis4 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis4 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis4==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis4 = System.GapPulse.Axis4;
			OverallSign.MoveSign.Axis4 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis4==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis4 = -System.GapPulse.Axis4;
			OverallSign.MoveSign.Axis4 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis4 = 0;	
	}

	if(AREAControl->DeltaStep.Axis5>0)
	{
		if(OverallSign.MoveSign.Axis5==0)
		{
			AREAControl->DeltaStep.Axis5 = AREAControl->DeltaStep.Axis5 + System.GapPulse.Axis5;
			AreaRunNCSign->NeedBack.Axis5 = 0;
		}
		OverallSign.MoveSign.Axis5 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis5<0)
	{
		if(OverallSign.MoveSign.Axis5==1) 
		{
			AREAControl->DeltaStep.Axis5 = AREAControl->DeltaStep.Axis5 - System.GapPulse.Axis5;
			AreaRunNCSign->NeedBack.Axis5 = 0;
		}
		OverallSign.MoveSign.Axis5 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis5 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis5==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis5 = System.GapPulse.Axis5;
			OverallSign.MoveSign.Axis5 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis5==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis5 = -System.GapPulse.Axis5;
			OverallSign.MoveSign.Axis5 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis5 = 0;	
	}

	if(AREAControl->DeltaStep.Axis6>0)
	{
		if(OverallSign.MoveSign.Axis6==0)
		{
			AREAControl->DeltaStep.Axis6 = AREAControl->DeltaStep.Axis6 + System.GapPulse.Axis6;
			AreaRunNCSign->NeedBack.Axis6 = 0;
		}
		OverallSign.MoveSign.Axis6 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis6<0)
	{
		if(OverallSign.MoveSign.Axis6==1) 
		{
			AREAControl->DeltaStep.Axis6 = AREAControl->DeltaStep.Axis6 - System.GapPulse.Axis6;
			AreaRunNCSign->NeedBack.Axis6 = 0;
		}
		OverallSign.MoveSign.Axis6 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis6 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis6==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis6 = System.GapPulse.Axis6;
			OverallSign.MoveSign.Axis6 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis6==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis6 = -System.GapPulse.Axis6;
			OverallSign.MoveSign.Axis6 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis6 = 0;	
	}

	if(AREAControl->DeltaStep.Axis7>0)
	{
		if(OverallSign.MoveSign.Axis7==0)
		{
			AREAControl->DeltaStep.Axis7 = AREAControl->DeltaStep.Axis7 + System.GapPulse.Axis7;
			AreaRunNCSign->NeedBack.Axis7 = 0;
		}
		OverallSign.MoveSign.Axis7 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis7<0)
	{
		if(OverallSign.MoveSign.Axis7==1) 
		{
			AREAControl->DeltaStep.Axis7 = AREAControl->DeltaStep.Axis7 - System.GapPulse.Axis7;
			AreaRunNCSign->NeedBack.Axis7 = 0;
		}
		OverallSign.MoveSign.Axis7 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis7 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis7==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis7 = System.GapPulse.Axis7;
			OverallSign.MoveSign.Axis7 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis7==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis7 = -System.GapPulse.Axis7;
			OverallSign.MoveSign.Axis7 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis7 = 0;	
	}

	if(AREAControl->DeltaStep.Axis8>0)
	{
		if(OverallSign.MoveSign.Axis8==0)
		{
			AREAControl->DeltaStep.Axis8 = AREAControl->DeltaStep.Axis8 + System.GapPulse.Axis8;
			AreaRunNCSign->NeedBack.Axis8 = 0;
		}
		OverallSign.MoveSign.Axis8 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis8<0)
	{
		if(OverallSign.MoveSign.Axis8==1) 
		{
			AREAControl->DeltaStep.Axis8 = AREAControl->DeltaStep.Axis8 - System.GapPulse.Axis8;
			AreaRunNCSign->NeedBack.Axis8 = 0;
		}
		OverallSign.MoveSign.Axis8 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis8 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis8==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis8 = System.GapPulse.Axis8;
			OverallSign.MoveSign.Axis8 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis8==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis8 = -System.GapPulse.Axis8;
			OverallSign.MoveSign.Axis8 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis8 = 0;	
	}

	if(AREAControl->DeltaStep.Axis9>0)
	{
		if(OverallSign.MoveSign.Axis9==0)
		{
			AREAControl->DeltaStep.Axis9 = AREAControl->DeltaStep.Axis9 + System.GapPulse.Axis9;
			AreaRunNCSign->NeedBack.Axis9 = 0;
		}
		OverallSign.MoveSign.Axis9 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis9<0)
	{
		if(OverallSign.MoveSign.Axis9==1) 
		{
			AREAControl->DeltaStep.Axis9 = AREAControl->DeltaStep.Axis9 - System.GapPulse.Axis9;
			AreaRunNCSign->NeedBack.Axis9 = 0;
		}
		OverallSign.MoveSign.Axis9 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis9 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis9==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis9 = System.GapPulse.Axis9;
			OverallSign.MoveSign.Axis9 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis9==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis9 = -System.GapPulse.Axis9;
			OverallSign.MoveSign.Axis9 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis9 = 0;	
	}

	if(AREAControl->DeltaStep.Axis10>0)
	{
		if(OverallSign.MoveSign.Axis10==0)
		{
			AREAControl->DeltaStep.Axis10 = AREAControl->DeltaStep.Axis10 + System.GapPulse.Axis10;
			AreaRunNCSign->NeedBack.Axis10 = 0;
		}
		OverallSign.MoveSign.Axis10 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis10<0)
	{
		if(OverallSign.MoveSign.Axis10==1) 
		{
			AREAControl->DeltaStep.Axis10 = AREAControl->DeltaStep.Axis10 - System.GapPulse.Axis10;
			AreaRunNCSign->NeedBack.Axis10 = 0;
		}
		OverallSign.MoveSign.Axis10 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis10 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis10==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis10 = System.GapPulse.Axis10;
			OverallSign.MoveSign.Axis10 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis10==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis10 = -System.GapPulse.Axis10;
			OverallSign.MoveSign.Axis10 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis10 = 0;	
	}

	if(AREAControl->DeltaStep.Axis11>0)
	{
		if(OverallSign.MoveSign.Axis11==0)
		{
			AREAControl->DeltaStep.Axis11 = AREAControl->DeltaStep.Axis11 + System.GapPulse.Axis11;
			AreaRunNCSign->NeedBack.Axis11 = 0;
		}
		OverallSign.MoveSign.Axis11 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis11<0)
	{
		if(OverallSign.MoveSign.Axis11==1) 
		{
			AREAControl->DeltaStep.Axis11 = AREAControl->DeltaStep.Axis11 - System.GapPulse.Axis11;
			AreaRunNCSign->NeedBack.Axis11 = 0;
		}
		OverallSign.MoveSign.Axis11 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis11 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis11==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis11 = System.GapPulse.Axis11;
			OverallSign.MoveSign.Axis11 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis11==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis11 = -System.GapPulse.Axis11;
			OverallSign.MoveSign.Axis11 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis11 = 0;	
	}


	if(AREAControl->DeltaStep.Axis12>0)
	{
		if(OverallSign.MoveSign.Axis12==0)
		{
			AREAControl->DeltaStep.Axis12 = AREAControl->DeltaStep.Axis12 + System.GapPulse.Axis12;
			AreaRunNCSign->NeedBack.Axis12 = 0;
		}
		OverallSign.MoveSign.Axis12 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis12<0)
	{
		if(OverallSign.MoveSign.Axis12==1) 
		{
			AREAControl->DeltaStep.Axis12 = AREAControl->DeltaStep.Axis12 - System.GapPulse.Axis12;
			AreaRunNCSign->NeedBack.Axis12 = 0;
		}
		OverallSign.MoveSign.Axis12 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis12 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis12==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis12 = System.GapPulse.Axis12;
			OverallSign.MoveSign.Axis12 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis12==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis12 = -System.GapPulse.Axis12;
			OverallSign.MoveSign.Axis12 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis12 = 0;	
	}


	if(AREAControl->DeltaStep.Axis13>0)
	{
		if(OverallSign.MoveSign.Axis13==0)
		{
			AREAControl->DeltaStep.Axis13 = AREAControl->DeltaStep.Axis13 + System.GapPulse.Axis13;
			AreaRunNCSign->NeedBack.Axis13 = 0;
		}
		OverallSign.MoveSign.Axis13 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis13<0)
	{
		if(OverallSign.MoveSign.Axis13==1) 
		{
			AREAControl->DeltaStep.Axis13 = AREAControl->DeltaStep.Axis13 - System.GapPulse.Axis13;
			AreaRunNCSign->NeedBack.Axis13 = 0;
		}
		OverallSign.MoveSign.Axis13 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis13 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis13==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis13 = System.GapPulse.Axis13;
			OverallSign.MoveSign.Axis13 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis13==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis13 = -System.GapPulse.Axis13;
			OverallSign.MoveSign.Axis13 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis13 = 0;	
	}


	if(AREAControl->DeltaStep.Axis14>0)
	{
		if(OverallSign.MoveSign.Axis14==0)
		{
			AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14 + System.GapPulse.Axis14;
			AreaRunNCSign->NeedBack.Axis14 = 0;
		}
		OverallSign.MoveSign.Axis14 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis14<0)
	{
		if(OverallSign.MoveSign.Axis14==1) 
		{
			AREAControl->DeltaStep.Axis14 = AREAControl->DeltaStep.Axis14 - System.GapPulse.Axis14;
			AreaRunNCSign->NeedBack.Axis14 = 0;
		}
		OverallSign.MoveSign.Axis14 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis14 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis14==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis14 = System.GapPulse.Axis14;
			OverallSign.MoveSign.Axis14 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis14==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis14 = -System.GapPulse.Axis14;
			OverallSign.MoveSign.Axis14 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis14 = 0;	
	}


	if(AREAControl->DeltaStep.Axis15>0)
	{
		if(OverallSign.MoveSign.Axis15==0)
		{
			AREAControl->DeltaStep.Axis15 = AREAControl->DeltaStep.Axis15 + System.GapPulse.Axis15;
			AreaRunNCSign->NeedBack.Axis15 = 0;
		}
		OverallSign.MoveSign.Axis15 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis15<0)
	{
		if(OverallSign.MoveSign.Axis15==1) 
		{
			AREAControl->DeltaStep.Axis15 = AREAControl->DeltaStep.Axis15 - System.GapPulse.Axis15;
			AreaRunNCSign->NeedBack.Axis15 = 0;
		}
		OverallSign.MoveSign.Axis15 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis15 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis15==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis15 = System.GapPulse.Axis15;
			OverallSign.MoveSign.Axis15 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis15==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis15 = -System.GapPulse.Axis15;
			OverallSign.MoveSign.Axis15 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis15 = 0;	
	}


	if(AREAControl->DeltaStep.Axis16>0)
	{
		if(OverallSign.MoveSign.Axis16==0)
		{
			AREAControl->DeltaStep.Axis16 = AREAControl->DeltaStep.Axis16 + System.GapPulse.Axis16;
			AreaRunNCSign->NeedBack.Axis16 = 0;
		}
		OverallSign.MoveSign.Axis16 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis16<0)
	{
		if(OverallSign.MoveSign.Axis16==1) 
		{
			AREAControl->DeltaStep.Axis16 = AREAControl->DeltaStep.Axis16 - System.GapPulse.Axis16;
			AreaRunNCSign->NeedBack.Axis16 = 0;
		}
		OverallSign.MoveSign.Axis16 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis16 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis16==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis16 = System.GapPulse.Axis16;
			OverallSign.MoveSign.Axis16 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis16==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis16 = -System.GapPulse.Axis16;
			OverallSign.MoveSign.Axis16 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis16 = 0;	
	}


	if(AREAControl->DeltaStep.Axis17>0)
	{
		if(OverallSign.MoveSign.Axis17==0)
		{
			AREAControl->DeltaStep.Axis17 = AREAControl->DeltaStep.Axis17 + System.GapPulse.Axis17;
			AreaRunNCSign->NeedBack.Axis17 = 0;
		}
		OverallSign.MoveSign.Axis17 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis17<0)
	{
		if(OverallSign.MoveSign.Axis17==1) 
		{
			AREAControl->DeltaStep.Axis17 = AREAControl->DeltaStep.Axis17 - System.GapPulse.Axis17;
			AreaRunNCSign->NeedBack.Axis17 = 0;
		}
		OverallSign.MoveSign.Axis17 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis17 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis17==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis17 = System.GapPulse.Axis17;
			OverallSign.MoveSign.Axis17 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis17==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis17 = -System.GapPulse.Axis17;
			OverallSign.MoveSign.Axis17 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis17 = 0;	
	}


	if(AREAControl->DeltaStep.Axis18>0)
	{
		if(OverallSign.MoveSign.Axis18==0)
		{
			AREAControl->DeltaStep.Axis18 = AREAControl->DeltaStep.Axis18 + System.GapPulse.Axis18;
			AreaRunNCSign->NeedBack.Axis18 = 0;
		}
		OverallSign.MoveSign.Axis18 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis18<0)
	{
		if(OverallSign.MoveSign.Axis18==1) 
		{
			AREAControl->DeltaStep.Axis18 = AREAControl->DeltaStep.Axis18 - System.GapPulse.Axis18;
			AreaRunNCSign->NeedBack.Axis18 = 0;
		}
		OverallSign.MoveSign.Axis18 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis18 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis18==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis18 = System.GapPulse.Axis18;
			OverallSign.MoveSign.Axis18 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis18==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis18 = -System.GapPulse.Axis18;
			OverallSign.MoveSign.Axis18 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis18 = 0;	
	}


	if(AREAControl->DeltaStep.Axis19>0)
	{
		if(OverallSign.MoveSign.Axis19==0)
		{
			AREAControl->DeltaStep.Axis19 = AREAControl->DeltaStep.Axis19 + System.GapPulse.Axis19;
			AreaRunNCSign->NeedBack.Axis19 = 0;
		}
		OverallSign.MoveSign.Axis19 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis19<0)
	{
		if(OverallSign.MoveSign.Axis19==1) 
		{
			AREAControl->DeltaStep.Axis19 = AREAControl->DeltaStep.Axis19 - System.GapPulse.Axis19;
			AreaRunNCSign->NeedBack.Axis19 = 0;
		}
		OverallSign.MoveSign.Axis19 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis19 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis19==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis19 = System.GapPulse.Axis19;
			OverallSign.MoveSign.Axis19 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis19==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis19 = -System.GapPulse.Axis19;
			OverallSign.MoveSign.Axis19 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis19 = 0;	
	}


	if(AREAControl->DeltaStep.Axis20>0)
	{
		if(OverallSign.MoveSign.Axis20==0)
		{
			AREAControl->DeltaStep.Axis20 = AREAControl->DeltaStep.Axis20 + System.GapPulse.Axis20;
			AreaRunNCSign->NeedBack.Axis20 = 0;
		}
		OverallSign.MoveSign.Axis20 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis20<0)
	{
		if(OverallSign.MoveSign.Axis20==1) 
		{
			AREAControl->DeltaStep.Axis20 = AREAControl->DeltaStep.Axis20 - System.GapPulse.Axis20;
			AreaRunNCSign->NeedBack.Axis20 = 0;
		}
		OverallSign.MoveSign.Axis20 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis20 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis20==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis20 = System.GapPulse.Axis20;
			OverallSign.MoveSign.Axis20 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis20==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis20 = -System.GapPulse.Axis20;
			OverallSign.MoveSign.Axis20 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis20 = 0;	
	}


	if(AREAControl->DeltaStep.Axis21>0)
	{
		if(OverallSign.MoveSign.Axis21==0)
		{
			AREAControl->DeltaStep.Axis21 = AREAControl->DeltaStep.Axis21 + System.GapPulse.Axis21;
			AreaRunNCSign->NeedBack.Axis21 = 0;
		}
		OverallSign.MoveSign.Axis21 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis21<0)
	{
		if(OverallSign.MoveSign.Axis21==1) 
		{
			AREAControl->DeltaStep.Axis21 = AREAControl->DeltaStep.Axis21 - System.GapPulse.Axis21;
			AreaRunNCSign->NeedBack.Axis21 = 0;
		}
		OverallSign.MoveSign.Axis21 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis21 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis21==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis21 = System.GapPulse.Axis21;
			OverallSign.MoveSign.Axis21 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis21==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis21 = -System.GapPulse.Axis21;
			OverallSign.MoveSign.Axis21 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis21 = 0;	
	}


	if(AREAControl->DeltaStep.Axis22>0)
	{
		if(OverallSign.MoveSign.Axis22==0)
		{
			AREAControl->DeltaStep.Axis22 = AREAControl->DeltaStep.Axis22 + System.GapPulse.Axis22;
			AreaRunNCSign->NeedBack.Axis22 = 0;
		}
		OverallSign.MoveSign.Axis22 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis22<0)
	{
		if(OverallSign.MoveSign.Axis22==1) 
		{
			AREAControl->DeltaStep.Axis22 = AREAControl->DeltaStep.Axis22 - System.GapPulse.Axis22;
			AreaRunNCSign->NeedBack.Axis22 = 0;
		}
		OverallSign.MoveSign.Axis22 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis22 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis22==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis22 = System.GapPulse.Axis22;
			OverallSign.MoveSign.Axis22 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis22==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis22 = -System.GapPulse.Axis22;
			OverallSign.MoveSign.Axis22 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis22 = 0;	
	}

	if(AREAControl->DeltaStep.Axis23>0)
	{
		if(OverallSign.MoveSign.Axis23==0)
		{
			AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23 + System.GapPulse.Axis23;
			AreaRunNCSign->NeedBack.Axis23 = 0;
		}
		OverallSign.MoveSign.Axis23 = 1;   
	}
	else if(AREAControl->DeltaStep.Axis23<0)
	{
		if(OverallSign.MoveSign.Axis23==1) 
		{
			AREAControl->DeltaStep.Axis23 = AREAControl->DeltaStep.Axis23 - System.GapPulse.Axis23;
			AreaRunNCSign->NeedBack.Axis23 = 0;
		}
		OverallSign.MoveSign.Axis23 = 0;
	}
	else if(AreaRunNCSign->NeedBack.Axis23 != 0) 		
	{
		if(AreaRunNCSign->NeedBack.Axis23==1)		//positive gap compensation
		{
			AREAControl->DeltaStep.Axis23 = System.GapPulse.Axis23;
			OverallSign.MoveSign.Axis23 = 1;   
		}
		else if(AreaRunNCSign->NeedBack.Axis23==2)	//negtive gap compensation
		{
			AREAControl->DeltaStep.Axis23 = -System.GapPulse.Axis23;
			OverallSign.MoveSign.Axis23 = 0;   
		}	
		AreaRunNCSign->NeedBack.Axis23 = 0;	
	}	
}

void NCAreaJudgeLimit(AREA_CONTROL *AREAControl)	//判断有无到极限位，并做相应处理
{
	if(OverallSign.NCSign == STDCODERUN)
	{
		if(((OverallSign.MoveSign.Axis1==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1)) ||
			((OverallSign.MoveSign.Axis1==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1)) ||
			((OverallSign.MoveSign.Axis2==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2))||
			((OverallSign.MoveSign.Axis2==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2))||
			((OverallSign.MoveSign.Axis3==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3))||
			((OverallSign.MoveSign.Axis3==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3))||
			((OverallSign.MoveSign.Axis4==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4)) ||
			((OverallSign.MoveSign.Axis4==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4)) ||
			((OverallSign.MoveSign.Axis5==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5))||
			((OverallSign.MoveSign.Axis5==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5))||
			((OverallSign.MoveSign.Axis6==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6))||
			((OverallSign.MoveSign.Axis6==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6))||
			((OverallSign.MoveSign.Axis7==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7)) ||
			((OverallSign.MoveSign.Axis7==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7)) ||
			((OverallSign.MoveSign.Axis8==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8))||
			((OverallSign.MoveSign.Axis8==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8))||
			((OverallSign.MoveSign.Axis9==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9))||
			((OverallSign.MoveSign.Axis9==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9))||
			((OverallSign.MoveSign.Axis10==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10))||
			((OverallSign.MoveSign.Axis10==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10))||
			
			((OverallSign.MoveSign.Axis11==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11)) ||
			((OverallSign.MoveSign.Axis11==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11)) ||
			((OverallSign.MoveSign.Axis12==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12))||
			((OverallSign.MoveSign.Axis12==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12))||
			((OverallSign.MoveSign.Axis13==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13))||
			((OverallSign.MoveSign.Axis13==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13))||
			((OverallSign.MoveSign.Axis14==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14)) ||
			((OverallSign.MoveSign.Axis14==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14)) ||
			((OverallSign.MoveSign.Axis15==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15))||
			((OverallSign.MoveSign.Axis15==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15))||
			((OverallSign.MoveSign.Axis16==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16))||
			((OverallSign.MoveSign.Axis16==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16))||
			((OverallSign.MoveSign.Axis17==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17)) ||
			((OverallSign.MoveSign.Axis17==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17)) ||
			((OverallSign.MoveSign.Axis18==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18))||
			((OverallSign.MoveSign.Axis18==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18))||
			((OverallSign.MoveSign.Axis19==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19))||
			((OverallSign.MoveSign.Axis19==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))||
			((OverallSign.MoveSign.Axis20==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20))||
			((OverallSign.MoveSign.Axis20==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20))||
			((OverallSign.MoveSign.Axis21==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21))||
			((OverallSign.MoveSign.Axis21==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21))||
			((OverallSign.MoveSign.Axis22==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22))||
			((OverallSign.MoveSign.Axis22==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22))||
			((OverallSign.MoveSign.Axis23==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23))||
			((OverallSign.MoveSign.Axis23==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23))
		)
		{
			AREAControl->DeltaStep.Axis1 = 0;
			AREAControl->DeltaStep.Axis2 = 0;
			AREAControl->DeltaStep.Axis3 = 0;
			AREAControl->DeltaStep.Axis4 = 0;
			AREAControl->DeltaStep.Axis5 = 0;
			AREAControl->DeltaStep.Axis6 = 0;
			AREAControl->DeltaStep.Axis7 = 0;
			AREAControl->DeltaStep.Axis8 = 0;
			AREAControl->DeltaStep.Axis9 = 0;
			AREAControl->DeltaStep.Axis10 = 0;
			AREAControl->DeltaStep.Axis11 = 0;
			AREAControl->DeltaStep.Axis12 = 0;
			AREAControl->DeltaStep.Axis13 = 0;
			AREAControl->DeltaStep.Axis14 = 0;
			AREAControl->DeltaStep.Axis15 = 0;
			AREAControl->DeltaStep.Axis16 = 0;
			AREAControl->DeltaStep.Axis17 = 0;
			AREAControl->DeltaStep.Axis18 = 0;
			AREAControl->DeltaStep.Axis19 = 0;
			AREAControl->DeltaStep.Axis20 = 0;
			AREAControl->DeltaStep.Axis21 = 0;
			AREAControl->DeltaStep.Axis22 = 0;
			AREAControl->DeltaStep.Axis23 = 0;
			return;
		}

		#if 0
		if(((OverallSign.MoveSign.Axis1==1)&&(AREAControl->NowValue.Axis1>System.SLimitPos.Axis1)) ||
			((OverallSign.MoveSign.Axis1==0)&&(AREAControl->NowValue.Axis1<System.SLimitNeg.Axis1)) ||
			((OverallSign.MoveSign.Axis2==1)&&(AREAControl->NowValue.Axis2>System.SLimitPos.Axis2))||
			((OverallSign.MoveSign.Axis2==0)&&(AREAControl->NowValue.Axis2<System.SLimitNeg.Axis2))||
			((OverallSign.MoveSign.Axis3==1)&&(AREAControl->NowValue.Axis3>System.SLimitPos.Axis3))||
			((OverallSign.MoveSign.Axis3==0)&&(AREAControl->NowValue.Axis3<System.SLimitNeg.Axis3))||
			((OverallSign.MoveSign.Axis4==1)&&(AREAControl->NowValue.Axis4>System.SLimitPos.Axis4)) ||
			((OverallSign.MoveSign.Axis4==0)&&(AREAControl->NowValue.Axis4<System.SLimitNeg.Axis4)) ||
			((OverallSign.MoveSign.Axis5==1)&&(AREAControl->NowValue.Axis5>System.SLimitPos.Axis5))||
			((OverallSign.MoveSign.Axis5==0)&&(AREAControl->NowValue.Axis5<System.SLimitNeg.Axis5))||
			((OverallSign.MoveSign.Axis6==1)&&(AREAControl->NowValue.Axis6>System.SLimitPos.Axis6))||
			((OverallSign.MoveSign.Axis6==0)&&(AREAControl->NowValue.Axis6<System.SLimitNeg.Axis6))||
			((OverallSign.MoveSign.Axis7==1)&&(AREAControl->NowValue.Axis7>System.SLimitPos.Axis7)) ||
			((OverallSign.MoveSign.Axis7==0)&&(AREAControl->NowValue.Axis7<System.SLimitNeg.Axis7)) ||
			((OverallSign.MoveSign.Axis8==1)&&(AREAControl->NowValue.Axis8>System.SLimitPos.Axis8))||
			((OverallSign.MoveSign.Axis8==0)&&(AREAControl->NowValue.Axis8<System.SLimitNeg.Axis8))||
			((OverallSign.MoveSign.Axis9==1)&&(AREAControl->NowValue.Axis9>System.SLimitPos.Axis9))||
			((OverallSign.MoveSign.Axis9==0)&&(AREAControl->NowValue.Axis9<System.SLimitNeg.Axis9))||
			((OverallSign.MoveSign.Axis10==1)&&(AREAControl->NowValue.Axis10>System.SLimitPos.Axis10))||
			((OverallSign.MoveSign.Axis10==0)&&(AREAControl->NowValue.Axis10<System.SLimitNeg.Axis10))||
			
			((OverallSign.MoveSign.Axis11==1)&&(AREAControl->NowValue.Axis11>System.SLimitPos.Axis11)) ||
			((OverallSign.MoveSign.Axis11==0)&&(AREAControl->NowValue.Axis11<System.SLimitNeg.Axis11)) ||
			((OverallSign.MoveSign.Axis12==1)&&(AREAControl->NowValue.Axis12>System.SLimitPos.Axis12))||
			((OverallSign.MoveSign.Axis12==0)&&(AREAControl->NowValue.Axis12<System.SLimitNeg.Axis12))||
			((OverallSign.MoveSign.Axis13==1)&&(AREAControl->NowValue.Axis13>System.SLimitPos.Axis13))||
			((OverallSign.MoveSign.Axis13==0)&&(AREAControl->NowValue.Axis13<System.SLimitNeg.Axis13))||
			((OverallSign.MoveSign.Axis14==1)&&(AREAControl->NowValue.Axis14>System.SLimitPos.Axis14)) ||
			((OverallSign.MoveSign.Axis14==0)&&(AREAControl->NowValue.Axis14<System.SLimitNeg.Axis14)) ||
			((OverallSign.MoveSign.Axis15==1)&&(AREAControl->NowValue.Axis15>System.SLimitPos.Axis15))||
			((OverallSign.MoveSign.Axis15==0)&&(AREAControl->NowValue.Axis15<System.SLimitNeg.Axis15))||
			((OverallSign.MoveSign.Axis16==1)&&(AREAControl->NowValue.Axis16>System.SLimitPos.Axis16))||
			((OverallSign.MoveSign.Axis16==0)&&(AREAControl->NowValue.Axis16<System.SLimitNeg.Axis16))||
			((OverallSign.MoveSign.Axis17==1)&&(AREAControl->NowValue.Axis17>System.SLimitPos.Axis17)) ||
			((OverallSign.MoveSign.Axis17==0)&&(AREAControl->NowValue.Axis17<System.SLimitNeg.Axis17)) ||
			((OverallSign.MoveSign.Axis18==1)&&(AREAControl->NowValue.Axis18>System.SLimitPos.Axis18))||
			((OverallSign.MoveSign.Axis18==0)&&(AREAControl->NowValue.Axis18<System.SLimitNeg.Axis18))||
			((OverallSign.MoveSign.Axis19==1)&&(AREAControl->NowValue.Axis19>System.SLimitPos.Axis19))||
			((OverallSign.MoveSign.Axis19==0)&&(AREAControl->NowValue.Axis19<System.SLimitNeg.Axis19))||
			((OverallSign.MoveSign.Axis20==1)&&(AREAControl->NowValue.Axis20>System.SLimitPos.Axis20))||
			((OverallSign.MoveSign.Axis20==0)&&(AREAControl->NowValue.Axis20<System.SLimitNeg.Axis20))||
			((OverallSign.MoveSign.Axis21==1)&&(AREAControl->NowValue.Axis21>System.SLimitPos.Axis21))||
			((OverallSign.MoveSign.Axis21==0)&&(AREAControl->NowValue.Axis21<System.SLimitNeg.Axis21))||
			((OverallSign.MoveSign.Axis22==1)&&(AREAControl->NowValue.Axis22>System.SLimitPos.Axis22))||
			((OverallSign.MoveSign.Axis22==0)&&(AREAControl->NowValue.Axis22<System.SLimitNeg.Axis22))||
			((OverallSign.MoveSign.Axis23==1)&&(AREAControl->NowValue.Axis23>System.SLimitPos.Axis23))||
			((OverallSign.MoveSign.Axis23==0)&&(AREAControl->NowValue.Axis23<System.SLimitNeg.Axis23))
		)
		{
			AREAControl->DeltaStep.Axis1 = 0;
			AREAControl->DeltaStep.Axis2 = 0;
			AREAControl->DeltaStep.Axis3 = 0;
			AREAControl->DeltaStep.Axis4 = 0;
			AREAControl->DeltaStep.Axis5 = 0;
			AREAControl->DeltaStep.Axis6 = 0;
			AREAControl->DeltaStep.Axis7 = 0;
			AREAControl->DeltaStep.Axis8 = 0;
			AREAControl->DeltaStep.Axis9 = 0;
			AREAControl->DeltaStep.Axis10 = 0;
			AREAControl->DeltaStep.Axis11 = 0;
			AREAControl->DeltaStep.Axis12 = 0;
			AREAControl->DeltaStep.Axis13 = 0;
			AREAControl->DeltaStep.Axis14 = 0;
			AREAControl->DeltaStep.Axis15 = 0;
			AREAControl->DeltaStep.Axis16 = 0;
			AREAControl->DeltaStep.Axis17 = 0;
			AREAControl->DeltaStep.Axis18 = 0;
			AREAControl->DeltaStep.Axis19 = 0;
			AREAControl->DeltaStep.Axis20 = 0;
			AREAControl->DeltaStep.Axis21 = 0;
			AREAControl->DeltaStep.Axis22 = 0;
			AREAControl->DeltaStep.Axis23 = 0;
			return;
		}
		#endif
		
		if(((OverallSign.MoveSign.Axis1==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis1)) ||
			((OverallSign.MoveSign.Axis1==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis1)) ||
			((OverallSign.MoveSign.Axis2==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis2))||
			((OverallSign.MoveSign.Axis2==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis2))||
			((OverallSign.MoveSign.Axis3==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis3))||
			((OverallSign.MoveSign.Axis3==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis3))||
			((OverallSign.MoveSign.Axis4==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis4)) ||
			((OverallSign.MoveSign.Axis4==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis4)) ||
			((OverallSign.MoveSign.Axis5==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis5))||
			((OverallSign.MoveSign.Axis5==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis5))||
			((OverallSign.MoveSign.Axis6==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis6))||
			((OverallSign.MoveSign.Axis6==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis6))||
			((OverallSign.MoveSign.Axis7==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis7)) ||
			((OverallSign.MoveSign.Axis7==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis7)) ||
			((OverallSign.MoveSign.Axis8==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis8))||
			((OverallSign.MoveSign.Axis8==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis8))||
			((OverallSign.MoveSign.Axis9==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis9))||
			((OverallSign.MoveSign.Axis9==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis9))||
			((OverallSign.MoveSign.Axis10==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis10))||
			((OverallSign.MoveSign.Axis10==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis10))||
			
			((OverallSign.MoveSign.Axis11==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis11)) ||
			((OverallSign.MoveSign.Axis11==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis11)) ||
			((OverallSign.MoveSign.Axis12==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis12))||
			((OverallSign.MoveSign.Axis12==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis12))||
			((OverallSign.MoveSign.Axis13==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis13))||
			((OverallSign.MoveSign.Axis13==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis13))||
			((OverallSign.MoveSign.Axis14==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis14)) ||
			((OverallSign.MoveSign.Axis14==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis14)) ||
			((OverallSign.MoveSign.Axis15==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis15))||
			((OverallSign.MoveSign.Axis15==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis15))||
			((OverallSign.MoveSign.Axis16==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis16))||
			((OverallSign.MoveSign.Axis16==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis16))||
			((OverallSign.MoveSign.Axis17==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis17)) ||
			((OverallSign.MoveSign.Axis17==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis17)) ||
			((OverallSign.MoveSign.Axis18==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis18))||
			((OverallSign.MoveSign.Axis18==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis18))||
			((OverallSign.MoveSign.Axis19==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis19))||
			((OverallSign.MoveSign.Axis19==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis19))||
			((OverallSign.MoveSign.Axis20==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis20))||
			((OverallSign.MoveSign.Axis20==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis20))||
			((OverallSign.MoveSign.Axis21==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis21))||
			((OverallSign.MoveSign.Axis21==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis21))||
			((OverallSign.MoveSign.Axis22==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis22))||
			((OverallSign.MoveSign.Axis22==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis22))||
			((OverallSign.MoveSign.Axis23==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis23))||
			((OverallSign.MoveSign.Axis23==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis23))
		)
		{
			AREAControl->DeltaStep.Axis1 = 0;
			AREAControl->DeltaStep.Axis2 = 0;
			AREAControl->DeltaStep.Axis3 = 0;
			AREAControl->DeltaStep.Axis4 = 0;
			AREAControl->DeltaStep.Axis5 = 0;
			AREAControl->DeltaStep.Axis6 = 0;
			AREAControl->DeltaStep.Axis7 = 0;
			AREAControl->DeltaStep.Axis8 = 0;
			AREAControl->DeltaStep.Axis9 = 0;
			AREAControl->DeltaStep.Axis10 = 0;
			AREAControl->DeltaStep.Axis11 = 0;
			AREAControl->DeltaStep.Axis12 = 0;
			AREAControl->DeltaStep.Axis13 = 0;
			AREAControl->DeltaStep.Axis14 = 0;
			AREAControl->DeltaStep.Axis15 = 0;
			AREAControl->DeltaStep.Axis16 = 0;
			AREAControl->DeltaStep.Axis17 = 0;
			AREAControl->DeltaStep.Axis18 = 0;
			AREAControl->DeltaStep.Axis19 = 0;
			AREAControl->DeltaStep.Axis20 = 0;
			AREAControl->DeltaStep.Axis21 = 0;
			AREAControl->DeltaStep.Axis22 = 0;
			AREAControl->DeltaStep.Axis23 = 0;
			return;
		}


		
		//if(System.FunctionSelect05 == False)
		{
			if(GPIO_In1_Register.bit.GPIOIN12 == 1)
			{
				if(OverallSign.MoveSign.Axis1 == 0)
				{
					if(abs(AREAControl->DeltaStep.Axis1) > 0)
					{
						AREAControl->DeltaStep.Axis1 = 0;
					}
				}
			}
		}
	}

	else if( OverallSign.NCSign == SPCCODERUN )
	{
		if(((OverallSign.MoveSign.Axis1==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1)) ||
			((OverallSign.MoveSign.Axis1==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1))) 
		{
			AREAControl->DeltaStep.Axis1 = 0;
		}
		if(((OverallSign.MoveSign.Axis2==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2)) ||
			((OverallSign.MoveSign.Axis2==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2))) 
		{
			AREAControl->DeltaStep.Axis2 = 0;
		}
		if(((OverallSign.MoveSign.Axis3==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3)) ||
			((OverallSign.MoveSign.Axis3==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3))) 
		{
			AREAControl->DeltaStep.Axis3 = 0;
		}
		
		if(((OverallSign.MoveSign.Axis4==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4)) ||
			((OverallSign.MoveSign.Axis4==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4))) 
		{
			AREAControl->DeltaStep.Axis4= 0;	
		}

		if(((OverallSign.MoveSign.Axis5==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5))||
			((OverallSign.MoveSign.Axis5==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5)))
		{
			AREAControl->DeltaStep.Axis5= 0;	
		}
		if(((OverallSign.MoveSign.Axis6==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6))||
			((OverallSign.MoveSign.Axis6==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6)))
		{
			AREAControl->DeltaStep.Axis6 = 0;	
		}

		if(((OverallSign.MoveSign.Axis7==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7)) ||
			((OverallSign.MoveSign.Axis7==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7)))
		{
			AREAControl->DeltaStep.Axis7 = 0;	
		}

		if(((OverallSign.MoveSign.Axis8==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8))||
			((OverallSign.MoveSign.Axis8==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8)))
		{
			AREAControl->DeltaStep.Axis8= 0;	
		}

		if(((OverallSign.MoveSign.Axis9==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9))||
			((OverallSign.MoveSign.Axis9==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9)))
		{
			AREAControl->DeltaStep.Axis9= 0;	
		}

		if(((OverallSign.MoveSign.Axis10==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_10))||
			((OverallSign.MoveSign.Axis10==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_10)))
		{
			AREAControl->DeltaStep.Axis10= 0;	
		}		
		if(((OverallSign.MoveSign.Axis11==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11)) ||
			((OverallSign.MoveSign.Axis11==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11)))
		{
			AREAControl->DeltaStep.Axis11= 0;	
		}

		if(((OverallSign.MoveSign.Axis12==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12))||
			((OverallSign.MoveSign.Axis12==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12)))
		{
			AREAControl->DeltaStep.Axis12= 0;	
		}
		if(((OverallSign.MoveSign.Axis13==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13))||
			((OverallSign.MoveSign.Axis13==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13)))
		{
			AREAControl->DeltaStep.Axis13= 0;	
		}
		if(((OverallSign.MoveSign.Axis14==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14)) ||
			((OverallSign.MoveSign.Axis14==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14)))
		{
			AREAControl->DeltaStep.Axis14= 0;	
		}
		if(((OverallSign.MoveSign.Axis15==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15))||
			((OverallSign.MoveSign.Axis15==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15)))
		{
			AREAControl->DeltaStep.Axis15= 0;	
		}
		if(((OverallSign.MoveSign.Axis16==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16))||
			((OverallSign.MoveSign.Axis16==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16)))
		{
			AREAControl->DeltaStep.Axis16= 0;	
		}

		if(((OverallSign.MoveSign.Axis17==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17)) ||
			((OverallSign.MoveSign.Axis17==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17)))
		{
			AREAControl->DeltaStep.Axis17= 0;	
		}
		if(((OverallSign.MoveSign.Axis18==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18))||
			((OverallSign.MoveSign.Axis18==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18)))
		{
			AREAControl->DeltaStep.Axis18= 0;	
		}
		if(((OverallSign.MoveSign.Axis19==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19))||
			((OverallSign.MoveSign.Axis19==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19)))
		{
			AREAControl->DeltaStep.Axis19= 0;	
		}
		if(((OverallSign.MoveSign.Axis20==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20))||
			((OverallSign.MoveSign.Axis20==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20)))
		{
			AREAControl->DeltaStep.Axis20= 0;	
		}
		if(((OverallSign.MoveSign.Axis21==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21))||
			((OverallSign.MoveSign.Axis21==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21)))
		{
			AREAControl->DeltaStep.Axis21= 0;	
		}
		if(((OverallSign.MoveSign.Axis22==1)&&(Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22))||
			((OverallSign.MoveSign.Axis22==0)&&(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22)))
		{
			AREAControl->DeltaStep.Axis22= 0;	
		}
		if(((OverallSign.MoveSign.Axis23==1)&&(Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23))||
			((OverallSign.MoveSign.Axis23==0)&&(Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23)))
		{
			AREAControl->DeltaStep.Axis23= 0;	
		}

		if((Reference.SearchRefSign) && (Reference.RefStep==0))
		{
			if(( Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1) && (Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1))
			{
				AREAControl->DeltaStep.Axis1 = 0;
				
			}
			if( (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_2) && (Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_2))
			{
				AREAControl->DeltaStep.Axis2 = 0;
			}
			if( (Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3) && (Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3))
			{
				AREAControl->DeltaStep.Axis3 = 0;
			}
		}

		if( Reference.RefStep == 20 )
		{
			if(((OverallSign.MoveSign.Axis1==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis1)) ||
				((OverallSign.MoveSign.Axis1==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis1))) 
			{
				AREAControl->DeltaStep.Axis1 = 0;
			}
			if(((OverallSign.MoveSign.Axis2==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis2))||
				((OverallSign.MoveSign.Axis2==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis2))) 
			{
				AREAControl->DeltaStep.Axis2 = 0;
			}
			if(((OverallSign.MoveSign.Axis3==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis3))||
				((OverallSign.MoveSign.Axis3==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis3))) 
			{
				AREAControl->DeltaStep.Axis3 = 0;
			}
			
			if(((OverallSign.MoveSign.Axis4==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis4)) ||
				((OverallSign.MoveSign.Axis4==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis4))) 
			{
				AREAControl->DeltaStep.Axis4= 0;	
			}

			if(((OverallSign.MoveSign.Axis5==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis5))||
				((OverallSign.MoveSign.Axis5==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis5)))
			{
				AREAControl->DeltaStep.Axis5= 0;	
			}
			if(((OverallSign.MoveSign.Axis6==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis6))||
				((OverallSign.MoveSign.Axis6==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis6)))
			{
				AREAControl->DeltaStep.Axis6 = 0;	
			}

			if(((OverallSign.MoveSign.Axis7==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis7)) ||
				((OverallSign.MoveSign.Axis7==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis7)))
			{
				AREAControl->DeltaStep.Axis7 = 0;	
			}

			if(((OverallSign.MoveSign.Axis8==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis8))||
				((OverallSign.MoveSign.Axis8==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis8)))
			{
				AREAControl->DeltaStep.Axis8= 0;	
			}

			if(((OverallSign.MoveSign.Axis9==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis9))||
				((OverallSign.MoveSign.Axis9==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis9)))
			{
				AREAControl->DeltaStep.Axis9= 0;	
			}

			if(((OverallSign.MoveSign.Axis10==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis10))||
				((OverallSign.MoveSign.Axis10==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis10)))
			{
				AREAControl->DeltaStep.Axis10= 0;	
			}		
			if(((OverallSign.MoveSign.Axis11==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis11)) ||
				((OverallSign.MoveSign.Axis11==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis11)))
			{
				AREAControl->DeltaStep.Axis11= 0;	
			}

			if(((OverallSign.MoveSign.Axis12==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis12))||
				((OverallSign.MoveSign.Axis12==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis12)))
			{
				AREAControl->DeltaStep.Axis12= 0;	
			}
			if(((OverallSign.MoveSign.Axis13==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis13))||
				((OverallSign.MoveSign.Axis13==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis13)))
			{
				AREAControl->DeltaStep.Axis13= 0;	
			}
			if(((OverallSign.MoveSign.Axis14==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis14)) ||
				((OverallSign.MoveSign.Axis14==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis14)))
			{
				AREAControl->DeltaStep.Axis14= 0;	
			}
			if(((OverallSign.MoveSign.Axis15==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis15))||
				((OverallSign.MoveSign.Axis15==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis15)))
			{
				AREAControl->DeltaStep.Axis15= 0;	
			}
			if(((OverallSign.MoveSign.Axis16==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis16))||
				((OverallSign.MoveSign.Axis16==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis16)))
			{
				AREAControl->DeltaStep.Axis16= 0;	
			}

			if(((OverallSign.MoveSign.Axis17==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis17)) ||
				((OverallSign.MoveSign.Axis17==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis17)))
			{
				AREAControl->DeltaStep.Axis17= 0;	
			}
			if(((OverallSign.MoveSign.Axis18==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis18))||
				((OverallSign.MoveSign.Axis18==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis18)))
			{
				AREAControl->DeltaStep.Axis18= 0;	
			}
			if(((OverallSign.MoveSign.Axis19==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis19))||
				((OverallSign.MoveSign.Axis19==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis19)))
			{
				AREAControl->DeltaStep.Axis19= 0;	
			}
			if(((OverallSign.MoveSign.Axis20==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis20))||
				((OverallSign.MoveSign.Axis20==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis20)))
			{
				AREAControl->DeltaStep.Axis20= 0;	
			}
			if(((OverallSign.MoveSign.Axis21==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis21))||
				((OverallSign.MoveSign.Axis21==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis21)))
			{
				AREAControl->DeltaStep.Axis21= 0;	
			}
						
			if(((OverallSign.MoveSign.Axis22==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis22))||
				((OverallSign.MoveSign.Axis22==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis22)))
			{
				AREAControl->DeltaStep.Axis22= 0;	
			}
			if(((OverallSign.MoveSign.Axis23==1)&&(psDSPPacketOut_InfoEvent->SLimitPosAxis23))||
				((OverallSign.MoveSign.Axis23==0)&&(psDSPPacketOut_InfoEvent->SLimitNegAxis23)))
			{
				AREAControl->DeltaStep.Axis23= 0;	
			}
		}
		
	}		

	return;
}

//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr(AREA_CONTROL *AREAControl)			
{
	NCAreaCal_V_tpr_Module1(AREAControl);
	NCAreaCal_V_tpr_Module2(AREAControl);
	NCAreaCal_V_tpr_Module3(AREAControl);
	NCAreaCal_V_tpr_Module4(AREAControl);
	NCAreaCal_V_tpr_Module5(AREAControl);
	NCAreaCal_V_tpr_Module6(AREAControl);
	NCAreaCal_V_tpr_Module7(AREAControl);
	NCAreaCal_V_tpr_Module8(AREAControl);
	NCAreaCal_V_tpr_Module9(AREAControl);
	NCAreaCal_V_tpr_Module10(AREAControl);
	NCAreaCal_V_tpr_Module11(AREAControl);
	NCAreaCal_V_tpr_Module12(AREAControl);

}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module1(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        
	
	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis15,Servo_Axis15.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis16,Servo_Axis16.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis1,Servo_Axis1.AxisOutUnitEQU)),2));    

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis15,l1*Servo_Axis15.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis16,l1*Servo_Axis16.AxisOutUnitEQU));
	cos_z1 = fabs(divdp(AREAControl->DeltaStep.Axis1,l1*Servo_Axis1.AxisOutUnitEQU));   

	if( abs(AREAControl->DeltaStep.Axis15)==0 ) 
	{
		Servo_Axis15.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis15.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis15) , System.Tsample ) );
		//Servo_Axis15.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*fabs(AREAControl->Cosine.Axis15)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis15.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis15.PWM_PeriodRegister = divdp( Servo_Axis15.PWM_PeriodRegister, Servo_Axis15.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis15.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis15.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis15.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis15.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis16)==0 ) 
	{
		Servo_Axis16.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis16.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis16) , System.Tsample ) );
		//Servo_Axis16.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*fabs(AREAControl->Cosine.Axis16)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis16.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis16.PWM_PeriodRegister = divdp( Servo_Axis16.PWM_PeriodRegister, Servo_Axis16.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis16.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis16.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis16.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis16.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	if( abs(AREAControl->DeltaStep.Axis1)==0 ) 
	{
		Servo_Axis1.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis1.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis1), System.Tsample) );
		//Servo_Axis1.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*fabs(AREAControl->Cosine.Axis1)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis1.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*cos_z1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis1.PWM_PeriodRegister = divdp( Servo_Axis1.PWM_PeriodRegister, Servo_Axis1.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis1.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis1.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis1.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis1.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	#if 0
	if( abs(AREAControl->DeltaStep.Axis2)==0 ) 
	{
		Servo_Axis2.PWM_PeriodRegister= 10;
	}
	else
	{
		//A_Servo.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( (abs(AREAControl->DeltaStep.Axis2)+1), System.Tsample) );
		Servo_Axis2.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis2.PWM_PeriodRegister = divdp( Servo_Axis2.PWM_PeriodRegister, Servo_Axis2.AxisOutUnitEQU);

		if(Servo_Axis2.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis2.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;
		}
		if(Servo_Axis2.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN)
		{
			Servo_Axis2.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	#endif
	
	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module2(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis17,Servo_Axis17.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis18,Servo_Axis18.AxisOutUnitEQU)),2)+0);    

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis17,l1*Servo_Axis17.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis18,l1*Servo_Axis18.AxisOutUnitEQU));
	cos_z1 = 0;   

	if( abs(AREAControl->DeltaStep.Axis17)==0 ) 
	{
		Servo_Axis17.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis17.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis17) , System.Tsample ) );
		//Servo_Axis17.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module2*fabs(AREAControl->Cosine.Axis17)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis17.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module2*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis17.PWM_PeriodRegister = divdp( Servo_Axis17.PWM_PeriodRegister, Servo_Axis17.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis17.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis17.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis17.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis17.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis18)==0 ) 
	{
		Servo_Axis18.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis18.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis18) , System.Tsample ) );
		//Servo_Axis18.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module2*fabs(AREAControl->Cosine.Axis18)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis18.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module2*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis18.PWM_PeriodRegister = divdp( Servo_Axis18.PWM_PeriodRegister, Servo_Axis18.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis18.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis18.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis18.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis18.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module3(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis3,Servo_Axis3.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis4,Servo_Axis4.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis5,Servo_Axis5.AxisOutUnitEQU)),2));    

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis3,l1*Servo_Axis3.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis4,l1*Servo_Axis4.AxisOutUnitEQU));
	cos_z1 = fabs(divdp(AREAControl->DeltaStep.Axis5,l1*Servo_Axis5.AxisOutUnitEQU));   

	if( abs(AREAControl->DeltaStep.Axis3)==0 ) 
	{
		Servo_Axis3.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis3.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis3) , System.Tsample ) );
		//Servo_Axis3.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*fabs(AREAControl->Cosine.Axis3)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis3.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis3.PWM_PeriodRegister = divdp( Servo_Axis3.PWM_PeriodRegister, Servo_Axis3.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis3.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis3.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis3.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis3.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis4)==0 ) 
	{
		Servo_Axis4.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis4.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis4) , System.Tsample ) );
		//Servo_Axis4.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*fabs(AREAControl->Cosine.Axis4)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis4.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis4.PWM_PeriodRegister = divdp( Servo_Axis4.PWM_PeriodRegister, Servo_Axis4.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis4.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis4.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis4.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis4.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	if( abs(AREAControl->DeltaStep.Axis5)==0 ) 
	{
		Servo_Axis5.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis5.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis5), System.Tsample) );
		//Servo_Axis5.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*fabs(AREAControl->Cosine.Axis5)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis5.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module3*cos_z1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis5.PWM_PeriodRegister = divdp( Servo_Axis5.PWM_PeriodRegister, Servo_Axis5.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis5.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis5.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis5.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis5.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

    return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module4(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis8,Servo_Axis8.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis7,Servo_Axis7.AxisOutUnitEQU)),2)+0);

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis8,l1*Servo_Axis8.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis7,l1*Servo_Axis7.AxisOutUnitEQU));
	cos_z1 = 0;   

	if( abs(AREAControl->DeltaStep.Axis8)==0 ) 
	{
		Servo_Axis8.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis8.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis8) , System.Tsample ) );
		//Servo_Axis8.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module4*fabs(AREAControl->Cosine.Axis8)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis8.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module4*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis8.PWM_PeriodRegister = divdp( Servo_Axis8.PWM_PeriodRegister, Servo_Axis8.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis8.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis8.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis8.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis8.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis7)==0 ) 
	{
		Servo_Axis7.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis7.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis7) , System.Tsample ) );
		//Servo_Axis7.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module4*fabs(AREAControl->Cosine.Axis7)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis7.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module4*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis7.PWM_PeriodRegister = divdp( Servo_Axis7.PWM_PeriodRegister, Servo_Axis7.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis7.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis7.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis7.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis7.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module5(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis6,Servo_Axis6.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis20,Servo_Axis20.AxisOutUnitEQU)),2)+0);

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis6,l1*Servo_Axis6.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis20,l1*Servo_Axis20.AxisOutUnitEQU));
	cos_z1 = 0;   

	if( abs(AREAControl->DeltaStep.Axis6)==0 ) 
	{
		Servo_Axis6.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis6.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis6) , System.Tsample ) );
		//Servo_Axis6.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module5*fabs(AREAControl->Cosine.Axis6)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis6.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module5*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis6.PWM_PeriodRegister = divdp( Servo_Axis6.PWM_PeriodRegister, Servo_Axis6.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis6.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis6.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis6.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis6.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis20)==0 ) 
	{
		Servo_Axis20.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis20.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis20) , System.Tsample ) );
		//Servo_Axis20.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module5*fabs(AREAControl->Cosine.Axis20)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis20.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module5*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis20.PWM_PeriodRegister = divdp( Servo_Axis20.PWM_PeriodRegister, Servo_Axis20.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis20.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis20.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis20.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis20.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module6(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis11,Servo_Axis11.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis12,Servo_Axis12.AxisOutUnitEQU)),2)+0);

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis11,l1*Servo_Axis11.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis12,l1*Servo_Axis12.AxisOutUnitEQU));
	cos_z1 = 0;   

	if( abs(AREAControl->DeltaStep.Axis11)==0 ) 
	{
		Servo_Axis11.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis11.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis11) , System.Tsample ) );
		//Servo_Axis11.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module6*fabs(AREAControl->Cosine.Axis11)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis11.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module6*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis11.PWM_PeriodRegister = divdp( Servo_Axis11.PWM_PeriodRegister, Servo_Axis11.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis11.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis11.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis11.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis11.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis12)==0 ) 
	{
		Servo_Axis12.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis12.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis12) , System.Tsample ) );
		//Servo_Axis12.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module6*fabs(AREAControl->Cosine.Axis12)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis12.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module6*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis12.PWM_PeriodRegister = divdp( Servo_Axis12.PWM_PeriodRegister, Servo_Axis12.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis12.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis12.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis12.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis12.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module7(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	//l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis9,Servo_Axis9.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis21,Servo_Axis21.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis10,Servo_Axis10.AxisOutUnitEQU)),2));    
	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis21,Servo_Axis21.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis22,Servo_Axis22.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis9,Servo_Axis9.AxisOutUnitEQU)),2));    

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis21,l1*Servo_Axis21.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis22,l1*Servo_Axis22.AxisOutUnitEQU));
	cos_z1 = fabs(divdp(AREAControl->DeltaStep.Axis9,l1*Servo_Axis9.AxisOutUnitEQU));   

	if( abs(AREAControl->DeltaStep.Axis21)==0 ) 
	{
		Servo_Axis21.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis21.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis21) , System.Tsample ) );
		//Servo_Axis21.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*fabs(AREAControl->Cosine.Axis21)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis21.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis21.PWM_PeriodRegister = divdp( Servo_Axis21.PWM_PeriodRegister, Servo_Axis21.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis21.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis21.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis21.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis21.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis22)==0 ) 
	{
		Servo_Axis22.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis22.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis22) , System.Tsample ) );
		//Servo_Axis22.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*fabs(AREAControl->Cosine.Axis22)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis22.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis22.PWM_PeriodRegister = divdp( Servo_Axis22.PWM_PeriodRegister, Servo_Axis22.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis22.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis22.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis22.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis22.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	if( abs(AREAControl->DeltaStep.Axis9)==0 ) 
	{
		Servo_Axis9.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis9.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis9), System.Tsample) );
		//Servo_Axis9.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*fabs(AREAControl->Cosine.Axis9)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis9.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*cos_z1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis9.PWM_PeriodRegister = divdp( Servo_Axis9.PWM_PeriodRegister, Servo_Axis9.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis9.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis9.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis9.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis9.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	#if 0
	if( abs(AREAControl->DeltaStep.Axis10)==0 ) 
	{
		Servo_Axis10.PWM_PeriodRegister= 10;
	}
	else
	{
		//A_Servo.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( (abs(AREAControl->DeltaStep.Axis10)+1), System.Tsample) );
		Servo_Axis10.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module7*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis10.PWM_PeriodRegister = divdp( Servo_Axis10.PWM_PeriodRegister, Servo_Axis10.AxisOutUnitEQU);

		if(Servo_Axis10.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis10.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;
		}
		if(Servo_Axis10.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN)
		{
			Servo_Axis10.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}	
	#endif

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module8(AREA_CONTROL *AREAControl)			
{
	if( abs(AREAControl->DeltaStep.Axis2)==0 ) 
	{
		Servo_Axis2.PWM_PeriodRegister=10;
	}
	else
	{	
		Servo_Axis2.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis2) , System.Tsample ) );
		//Servo_Axis2.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module8*fabs(AREAControl->Cosine.Axis2)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis2.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module8*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis2.PWM_PeriodRegister = divdp( Servo_Axis2.PWM_PeriodRegister, Servo_Axis2.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis2.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis2.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX; 	
		}
		if(Servo_Axis2.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis2.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module9(AREA_CONTROL *AREAControl)			
{
	if( abs(AREAControl->DeltaStep.Axis10)==0 ) 
	{
		Servo_Axis10.PWM_PeriodRegister=10;
	}
	else
	{	
		Servo_Axis10.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis10) , System.Tsample ) );
		//Servo_Axis10.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module9*fabs(AREAControl->Cosine.Axis10)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis10.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module9*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis10.PWM_PeriodRegister = divdp( Servo_Axis10.PWM_PeriodRegister, Servo_Axis10.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis10.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis10.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX; 	
		}
		if(Servo_Axis10.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis10.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module10(AREA_CONTROL *AREAControl)			
{
	float64 cos_x1,cos_y1,cos_z1,l1;        

	l1 = sqrtdp(powdp((divdp(AREAControl->DeltaStep.Axis13,Servo_Axis13.AxisOutUnitEQU)),2)+powdp((divdp(AREAControl->DeltaStep.Axis14,Servo_Axis14.AxisOutUnitEQU)),2)+0);

	if(l1<1)
	{
		l1 = 1;
	}

	cos_x1 = fabs(divdp(AREAControl->DeltaStep.Axis13,l1*Servo_Axis13.AxisOutUnitEQU));
	cos_y1 = fabs(divdp(AREAControl->DeltaStep.Axis14,l1*Servo_Axis14.AxisOutUnitEQU));
	cos_z1 = 0;   

	if( abs(AREAControl->DeltaStep.Axis13)==0 ) 
	{
		Servo_Axis13.PWM_PeriodRegister=10;
	}
	else
	{   
		Servo_Axis13.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis13) , System.Tsample ) );
		//Servo_Axis13.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module10*fabs(AREAControl->Cosine.Axis13)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis13.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module10*cos_x1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis13.PWM_PeriodRegister = divdp( Servo_Axis13.PWM_PeriodRegister, Servo_Axis13.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis13.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis13.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis13.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis13.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	if( abs(AREAControl->DeltaStep.Axis14)==0 ) 
	{
		Servo_Axis14.PWM_PeriodRegister= 10;
	}
	else
	{
		Servo_Axis14.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis14) , System.Tsample ) );
		//Servo_Axis14.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module10*fabs(AREAControl->Cosine.Axis14)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis14.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module10*cos_y1*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis14.PWM_PeriodRegister = divdp( Servo_Axis14.PWM_PeriodRegister, Servo_Axis14.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis14.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis14.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX;		
		}
		if(Servo_Axis14.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis14.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}
	
	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module11(AREA_CONTROL *AREAControl)			
{
	if( abs(AREAControl->DeltaStep.Axis19)==0 ) 
	{
		Servo_Axis19.PWM_PeriodRegister=10;
	}
	else
	{	
		Servo_Axis19.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( (fabs(AREAControl->DeltaStep.Axis19)+1) , System.Tsample) );
		//Servo_Axis19.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module11*fabs(AREAControl->Cosine.Axis19)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis19.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module11*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis19.PWM_PeriodRegister = divdp( Servo_Axis19.PWM_PeriodRegister, Servo_Axis19.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis19.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis19.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX; 	
		}
		if(Servo_Axis19.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis19.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


//STD/SPC 区域曲线计算各轴脉冲发生器的PWM PERIOD
void NCAreaCal_V_tpr_Module12(AREA_CONTROL *AREAControl)			
{
	if( abs(AREAControl->DeltaStep.Axis23)==0 ) 
	{
		Servo_Axis23.PWM_PeriodRegister=10;
	}
	else
	{	
		Servo_Axis23.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp( fabs(AREAControl->DeltaStep.Axis23) , System.Tsample ) );
		//Servo_Axis23.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module12*fabs(AREAControl->Cosine.Axis23)*1e6),System.LinearAxisOutUnitEQU));
		#if 0
		Servo_Axis23.PWM_PeriodRegister = divdp( FPGA_MAX_F, divdp((AREAControl->VNow_Module.Module12*1e6),System.LinearAxisOutUnitEQU));
		Servo_Axis23.PWM_PeriodRegister = divdp( Servo_Axis23.PWM_PeriodRegister, Servo_Axis23.AxisOutUnitEQU);
		#endif
		
		if(Servo_Axis23.PWM_PeriodRegister > System.PWM_PeriodRegister_MAX)
		{
			Servo_Axis23.PWM_PeriodRegister = System.PWM_PeriodRegister_MAX; 	
		}
		if(Servo_Axis23.PWM_PeriodRegister < System.PWM_PeriodRegister_MIN) 
		{
			Servo_Axis23.PWM_PeriodRegister = System.PWM_PeriodRegister_MIN;
		}
	}

	return;
}


// 区域曲线写插补缓冲区
Uint16 NCAreaWriteInterpolationBuffer(AREA_CONTROL *AREAControl) 
{		
	INTERPOLATION_BUFFER *psInterpolationBuffer_1,*psInterpolationBuffer_2;
	
	//CH0903
	if(System.RunState == 1)
	{
		AREAControl->I++;
		return 1;
	}						

	IRQ_DISABLE(IRQNUM_PLUSE1);
	IRQ_DISABLE(IRQNUM_PLUSE2);	
	IRQ_DISABLE(IRQNUM_PLUSE3);
	IRQ_DISABLE(IRQNUM_PLUSE4);
	
	psInterpolationBuffer_1=&InterpolationBuffer_1[OverallSign.InterpolationSendCnt_1%INTERPOLATIONBUFLENGTH];
	OverallSign.InterpolationSendCnt_2 = OverallSign.InterpolationSendCnt_1;
	psInterpolationBuffer_2=&InterpolationBuffer_2[OverallSign.InterpolationSendCnt_2%INTERPOLATIONBUFLENGTH];

	psInterpolationBuffer_2->SIGN.Axis1 = (OverallSign.MoveSign.Axis1 != System.MotorChangeDir.Axis1)?1:0;
	psInterpolationBuffer_2->SIGN.Axis2 = (OverallSign.MoveSign.Axis2 != System.MotorChangeDir.Axis2)?1:0;
	psInterpolationBuffer_2->SIGN.Axis3 = (OverallSign.MoveSign.Axis3 != System.MotorChangeDir.Axis3)?1:0;
	psInterpolationBuffer_2->SIGN.Axis4 = (OverallSign.MoveSign.Axis4 != System.MotorChangeDir.Axis4)?1:0;
	psInterpolationBuffer_2->SIGN.Axis5 = (OverallSign.MoveSign.Axis5 != System.MotorChangeDir.Axis5)?1:0;
	psInterpolationBuffer_1->SIGN.Axis6 = (OverallSign.MoveSign.Axis6 != System.MotorChangeDir.Axis6)?1:0;
	psInterpolationBuffer_1->SIGN.Axis7 = (OverallSign.MoveSign.Axis7 != System.MotorChangeDir.Axis7)?1:0;
	psInterpolationBuffer_1->SIGN.Axis8 = (OverallSign.MoveSign.Axis8 != System.MotorChangeDir.Axis8)?1:0;
	psInterpolationBuffer_1->SIGN.Axis9 = (OverallSign.MoveSign.Axis9 != System.MotorChangeDir.Axis9)?1:0;
	psInterpolationBuffer_1->SIGN.Axis10 = (OverallSign.MoveSign.Axis10 != System.MotorChangeDir.Axis10)?1:0;
	psInterpolationBuffer_2->SIGN.Axis11 = (OverallSign.MoveSign.Axis11 != System.MotorChangeDir.Axis11)?1:0;
	psInterpolationBuffer_2->SIGN.Axis12 = (OverallSign.MoveSign.Axis12 != System.MotorChangeDir.Axis12)?1:0;
	psInterpolationBuffer_2->SIGN.Axis13 = (OverallSign.MoveSign.Axis13 != System.MotorChangeDir.Axis13)?1:0;
	psInterpolationBuffer_2->SIGN.Axis14 = (OverallSign.MoveSign.Axis14 != System.MotorChangeDir.Axis14)?1:0;
	psInterpolationBuffer_2->SIGN.Axis15 = (OverallSign.MoveSign.Axis15 != System.MotorChangeDir.Axis15)?1:0;
	psInterpolationBuffer_2->SIGN.Axis16 = (OverallSign.MoveSign.Axis16 != System.MotorChangeDir.Axis16)?1:0;
	psInterpolationBuffer_2->SIGN.Axis17 = (OverallSign.MoveSign.Axis17 != System.MotorChangeDir.Axis17)?1:0;
	psInterpolationBuffer_2->SIGN.Axis18 = (OverallSign.MoveSign.Axis18 != System.MotorChangeDir.Axis18)?1:0;
	psInterpolationBuffer_1->SIGN.Axis19 = (OverallSign.MoveSign.Axis19 != System.MotorChangeDir.Axis19)?1:0;
	psInterpolationBuffer_1->SIGN.Axis20 = (OverallSign.MoveSign.Axis20 != System.MotorChangeDir.Axis20)?1:0;
	psInterpolationBuffer_1->SIGN.Axis21 = (OverallSign.MoveSign.Axis21 != System.MotorChangeDir.Axis21)?1:0;
	psInterpolationBuffer_1->SIGN.Axis22 = (OverallSign.MoveSign.Axis22 != System.MotorChangeDir.Axis22)?1:0;
	psInterpolationBuffer_2->SIGN.Axis23 = (OverallSign.MoveSign.Axis23 != System.MotorChangeDir.Axis23)?1:0;

	psInterpolationBuffer_2->DeltaPulse.Axis1 = AREAControl->DeltaStep.Axis1;
	psInterpolationBuffer_2->DeltaPulse.Axis2 = AREAControl->DeltaStep.Axis2;
	psInterpolationBuffer_2->DeltaPulse.Axis3 = AREAControl->DeltaStep.Axis3;
	psInterpolationBuffer_2->DeltaPulse.Axis4 = AREAControl->DeltaStep.Axis4;
	psInterpolationBuffer_2->DeltaPulse.Axis5 = AREAControl->DeltaStep.Axis5;
	psInterpolationBuffer_1->DeltaPulse.Axis6 = AREAControl->DeltaStep.Axis6;
	psInterpolationBuffer_1->DeltaPulse.Axis7 = AREAControl->DeltaStep.Axis7;
	psInterpolationBuffer_1->DeltaPulse.Axis8 = AREAControl->DeltaStep.Axis8;
	psInterpolationBuffer_1->DeltaPulse.Axis9 = AREAControl->DeltaStep.Axis9;
	psInterpolationBuffer_1->DeltaPulse.Axis10 = AREAControl->DeltaStep.Axis10;
	psInterpolationBuffer_2->DeltaPulse.Axis11 = AREAControl->DeltaStep.Axis11;
	psInterpolationBuffer_2->DeltaPulse.Axis12 = AREAControl->DeltaStep.Axis12;
	psInterpolationBuffer_2->DeltaPulse.Axis13 = AREAControl->DeltaStep.Axis13;
	psInterpolationBuffer_2->DeltaPulse.Axis14 = AREAControl->DeltaStep.Axis14;
	psInterpolationBuffer_2->DeltaPulse.Axis15 = AREAControl->DeltaStep.Axis15;
	psInterpolationBuffer_2->DeltaPulse.Axis16 = AREAControl->DeltaStep.Axis16;
	psInterpolationBuffer_2->DeltaPulse.Axis17 = AREAControl->DeltaStep.Axis17;
	psInterpolationBuffer_2->DeltaPulse.Axis18 = AREAControl->DeltaStep.Axis18;
	psInterpolationBuffer_1->DeltaPulse.Axis19 = AREAControl->DeltaStep.Axis19;
	psInterpolationBuffer_1->DeltaPulse.Axis20 = AREAControl->DeltaStep.Axis20;
	psInterpolationBuffer_1->DeltaPulse.Axis21 = AREAControl->DeltaStep.Axis21;
	psInterpolationBuffer_1->DeltaPulse.Axis22 = AREAControl->DeltaStep.Axis22;
	psInterpolationBuffer_2->DeltaPulse.Axis23 = AREAControl->DeltaStep.Axis23;

	psInterpolationBuffer_2->PWM_PeriodRegister.Axis1 = Servo_Axis1.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis2 = Servo_Axis2.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis3 = Servo_Axis3.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis4 = Servo_Axis4.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis5 = Servo_Axis5.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis6 = Servo_Axis6.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis7 = Servo_Axis7.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis8 = Servo_Axis8.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis9 = Servo_Axis9.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis10 = Servo_Axis10.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis11 = Servo_Axis11.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis12 = Servo_Axis12.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis13 = Servo_Axis13.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis14 = Servo_Axis14.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis15 = Servo_Axis15.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis16 = Servo_Axis16.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis17 = Servo_Axis17.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis18 = Servo_Axis18.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis19 = Servo_Axis19.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis20 = Servo_Axis20.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis21 = Servo_Axis21.PWM_PeriodRegister;
	psInterpolationBuffer_1->PWM_PeriodRegister.Axis22 = Servo_Axis22.PWM_PeriodRegister;
	psInterpolationBuffer_2->PWM_PeriodRegister.Axis23 = Servo_Axis23.PWM_PeriodRegister;

	if(System.RunState == 3)
	{
		psInterpolationBuffer_2->MACHPosition.Axis1 = AREAControl->NowValue.Axis1;
		psInterpolationBuffer_2->MACHPosition.Axis2 = AREAControl->NowValue.Axis2;
		psInterpolationBuffer_2->MACHPosition.Axis3 = AREAControl->NowValue.Axis3;
		psInterpolationBuffer_2->MACHPosition.Axis4 = AREAControl->NowValue.Axis4;
		psInterpolationBuffer_2->MACHPosition.Axis5 = AREAControl->NowValue.Axis5;
		psInterpolationBuffer_1->MACHPosition.Axis6 = AREAControl->NowValue.Axis6;
		psInterpolationBuffer_1->MACHPosition.Axis7 = AREAControl->NowValue.Axis7;
		psInterpolationBuffer_1->MACHPosition.Axis8 = AREAControl->NowValue.Axis8;		
		psInterpolationBuffer_1->MACHPosition.Axis9 = AREAControl->NowValue.Axis9;
		psInterpolationBuffer_1->MACHPosition.Axis10 = AREAControl->NowValue.Axis10;
		psInterpolationBuffer_2->MACHPosition.Axis11 = AREAControl->NowValue.Axis11;
		psInterpolationBuffer_2->MACHPosition.Axis12 = AREAControl->NowValue.Axis12;
		psInterpolationBuffer_2->MACHPosition.Axis13 = AREAControl->NowValue.Axis13;
		psInterpolationBuffer_2->MACHPosition.Axis14 = AREAControl->NowValue.Axis14;
		psInterpolationBuffer_2->MACHPosition.Axis15 = AREAControl->NowValue.Axis15;
		psInterpolationBuffer_2->MACHPosition.Axis16 = AREAControl->NowValue.Axis16;
		psInterpolationBuffer_2->MACHPosition.Axis17 = AREAControl->NowValue.Axis17;
		psInterpolationBuffer_2->MACHPosition.Axis18 = AREAControl->NowValue.Axis18;		
		psInterpolationBuffer_1->MACHPosition.Axis19 = AREAControl->NowValue.Axis19;
		psInterpolationBuffer_1->MACHPosition.Axis20 = AREAControl->NowValue.Axis20;
		psInterpolationBuffer_1->MACHPosition.Axis21 = AREAControl->NowValue.Axis21;
		psInterpolationBuffer_1->MACHPosition.Axis22 = AREAControl->NowValue.Axis22;
		psInterpolationBuffer_2->MACHPosition.Axis23 = AREAControl->NowValue.Axis23;

	}

	OverallSign.InterpolationSendCnt_1++;
	OverallSign.InterpolationSendCnt_2 = OverallSign.InterpolationSendCnt_1;

	AREAControl->I++;

	IRQ_ENABLE(IRQNUM_PLUSE1);
	IRQ_ENABLE(IRQNUM_PLUSE2);	
	IRQ_ENABLE(IRQNUM_PLUSE3);
	IRQ_ENABLE(IRQNUM_PLUSE4);	

	
	return 1;
}

//CH0701
Uint16 NCAreaPrePulseToServo(AREA_CONTROL *AREAControl)  
{	
	if(abs(AREAControl->DeltaStep.Axis1) > (5000.0*Servo_Axis1.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000001;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis2) > (5000.0*Servo_Axis2.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000002;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis3) > (5000.0*Servo_Axis3.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000004;	
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis4) > (5000.0*Servo_Axis4.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000008;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis5) > (5000.0*Servo_Axis5.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000010;
		high_error();		//CH0904
		return 0;
	}
	
	if(abs(AREAControl->DeltaStep.Axis6) > (5000.0*Servo_Axis6.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000020;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis7) > (5000.0*Servo_Axis7.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000040;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis8) > (5000.0*Servo_Axis8.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000080;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis9) > (5000.0*Servo_Axis9.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000100;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis10) > (5000.0*Servo_Axis10.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000200;
		high_error();		//CH0904
		return 0;
	}
	
	if(abs(AREAControl->DeltaStep.Axis11) > (5000.0*Servo_Axis11.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000400;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis12) > (5000.0*Servo_Axis12.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00000800;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis13) > (5000.0*Servo_Axis13.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00001000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis14) > (5000.0*Servo_Axis14.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00002000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis15) > (5000.0*Servo_Axis15.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00004000;
		high_error();		//CH0904
		return 0;
	}
	
	if(abs(AREAControl->DeltaStep.Axis16) > (5000.0*Servo_Axis16.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00008000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis17) > (5000.0*Servo_Axis17.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00010000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis18) > (5000.0*Servo_Axis18.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00020000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis19) > (5000.0*Servo_Axis19.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00040000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis20) > (5000.0*Servo_Axis20.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00080000;
		high_error();		//CH0904
		return 0;
	}
	
	if(abs(AREAControl->DeltaStep.Axis21) > (5000.0*Servo_Axis21.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00100000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis22) > (5000.0*Servo_Axis22.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00200000;
		high_error();		//CH0904
		return 0;
	}
	if(abs(AREAControl->DeltaStep.Axis23) > (5000.0*Servo_Axis23.AxisOutUnitEQU))
	{
		Error.InterpolationOver = 1;
		psDSPPacketOut->InterpolationOver = 0x00400000;
		high_error();		//CH0904
		return 0;
	}
	
	return 1;
}
	
Uint16 NCAreaPulseToServo_LPMEN1(void) 
{	//FPGA_B
	INTERPOLATION_BUFFER *InterpolationBufPT_1;
	
	OverallSign.Int4IdleSign_1 = 0;
	if(OverallSign.InterpolationRunCnt_1 >= OverallSign.InterpolationSendCnt_1)
	{
		if(OverallSign.LensMotionOverSign != LensMotionCompensationRun)
		{
			return 0;
		}
	}

	Read_Servo_Over2_In();
	if(OverallSign.EveryAxisOverValid2 == 0)
	{
		OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
		return 0;
	}
	
	IRQ_DISABLE(IRQNUM_PLUSE1);		
	IRQ_DISABLE(IRQNUM_PLUSE2);
	IRQ_DISABLE(IRQNUM_PLUSE3);		
	IRQ_DISABLE(IRQNUM_PLUSE4);			
	InterpolationBufPT_1 = &InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
	if(OverallSign.InterpolationRunCnt_1 < OverallSign.InterpolationSendCnt_1)
		OverallSign.InterpolationRunCnt_1++;
	
	if(
		(abs(InterpolationBufPT_1->DeltaPulse.Axis6)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis7)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis8)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis9)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis10)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis19)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis20)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis21)+
		abs(InterpolationBufPT_1->DeltaPulse.Axis22))
		== 0
		)
	{	
		if((DeltaStepAxis19_Bak == 0)&&
			(DeltaStepAxis19_CPS == 0)
			&&
			(OverallSign.LensMotionOverSign == LensMotionNormal)
			)
		{
			IRQ_ENABLE(IRQNUM_PLUSE1);		
			IRQ_ENABLE(IRQNUM_PLUSE2);		
			IRQ_ENABLE(IRQNUM_PLUSE3);		
			IRQ_ENABLE(IRQNUM_PLUSE4);		
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<22);
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1<<22);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = (0x1<<17);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0) = (0x1<<17);
			return 0;
		}
	}

	Servo_LPMEN_Register.bit.LPMEN_1 = 0;
	Write_Servo_LPMEN();
	
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_1= InterpolationBufPT_1->SIGN.Axis1;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_2= InterpolationBufPT_1->SIGN.Axis2;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_3= InterpolationBufPT_1->SIGN.Axis3;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_4= InterpolationBufPT_1->SIGN.Axis4;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_5= InterpolationBufPT_1->SIGN.Axis5;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_6= InterpolationBufPT_1->SIGN.Axis6;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_7= InterpolationBufPT_1->SIGN.Axis7;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_8= InterpolationBufPT_1->SIGN.Axis8;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_9= InterpolationBufPT_1->SIGN.Axis9;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_10= InterpolationBufPT_1->SIGN.Axis10;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_11= InterpolationBufPT_1->SIGN.Axis11;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_12= InterpolationBufPT_1->SIGN.Axis12;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_13= InterpolationBufPT_1->SIGN.Axis13;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_14= InterpolationBufPT_1->SIGN.Axis14;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_15= InterpolationBufPT_1->SIGN.Axis15;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_16= InterpolationBufPT_1->SIGN.Axis16;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_17= InterpolationBufPT_1->SIGN.Axis17;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_18= InterpolationBufPT_1->SIGN.Axis18;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_19= InterpolationBufPT_1->SIGN.Axis19;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_20= InterpolationBufPT_1->SIGN.Axis20;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_21= InterpolationBufPT_1->SIGN.Axis21;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_22= InterpolationBufPT_1->SIGN.Axis22;
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= InterpolationBufPT_1->SIGN.Axis23;
	
	if(System.FunctionSelect03 == 1)
	{
		if(DeltaStepAxis19_Bak < 0)
		{
			Servo_Sign2_Out_Register.bit.SERVO_SIGN_19 = 0;
		}
		else if(DeltaStepAxis19_Bak > 0)
		{
			Servo_Sign2_Out_Register.bit.SERVO_SIGN_19 = 1;
		}
	}

	if((System.FunctionSelect04 == 1)&&(OverallSign.CameraScanSign == 0))
	{
		if(OverallSign.LensMotionOverSign == LensMotionCompensationRun)
		{
			if(DeltaStepAxis19_CPS< 0)
			{
				Servo_Sign2_Out_Register.bit.SERVO_SIGN_19 = 0;
			}
			else if(DeltaStepAxis19_CPS > 0)
			{
				Servo_Sign2_Out_Register.bit.SERVO_SIGN_19 = 1;
			}
		}
	}
	
	//Write_Servo_Sign1_Out();
	Write_Servo_Sign2_Out();

	//*(Servo_Axis1.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis1);
	//*(Servo_Axis2.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis2);
	//*(Servo_Axis3.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis3);
	//*(Servo_Axis4.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis4);
	//*(Servo_Axis5.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis5);
	*(Servo_Axis6.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis6);
	*(Servo_Axis7.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis7);
	*(Servo_Axis8.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis8);
	*(Servo_Axis9.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis9);
	*(Servo_Axis10.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis10);
	//*(Servo_Axis11.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis11);
	//*(Servo_Axis12.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis12);
	//*(Servo_Axis13.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis13);
	//*(Servo_Axis14.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis14);
	//*(Servo_Axis15.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis15);
	//*(Servo_Axis16.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis16);
	//*(Servo_Axis17.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis17);
	//*(Servo_Axis18.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis18);
	*(Servo_Axis19.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis19);
	*(Servo_Axis20.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis20);
	*(Servo_Axis21.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis21);
	*(Servo_Axis22.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis22);
	//*(Servo_Axis23.LPM_CounterPort) = abs(InterpolationBufPT_1->DeltaPulse.Axis23);

	//*(Servo_Axis1.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis1;
	//*(Servo_Axis2.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis2;
	//*(Servo_Axis3.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis3;
	//*(Servo_Axis4.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis4;
	//*(Servo_Axis5.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis5;
	*(Servo_Axis6.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis6;
	*(Servo_Axis7.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis7;
	*(Servo_Axis8.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis8;
	*(Servo_Axis9.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis9;
	*(Servo_Axis10.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis10;
	//*(Servo_Axis11.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis11;
	//*(Servo_Axis12.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis12;
	//*(Servo_Axis13.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis13;
	//*(Servo_Axis14.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis14;
	//*(Servo_Axis15.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis15;
	//*(Servo_Axis16.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis16;
	//*(Servo_Axis17.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis17;
	//*(Servo_Axis18.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis18;
	*(Servo_Axis19.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis19;
	*(Servo_Axis20.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis20;
	*(Servo_Axis21.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis21;
	*(Servo_Axis22.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis22;
	//*(Servo_Axis23.PWM_Port) = InterpolationBufPT_1->PWM_PeriodRegister.Axis23;
	
	if(System.FunctionSelect03 == 1)
	{
		if(abs(DeltaStepAxis19_Bak) > 0)
		{
			*(Servo_Axis19.LPM_CounterPort) = abs(DeltaStepAxis19_Bak);
			*(Servo_Axis19.PWM_Port) = divdp( FPGA_MAX_F*System.Tsample, (abs(DeltaStepAxis19_Bak)+1));		
			DeltaStepAxis19_Bak = 0;
		}		
	}

	if((System.FunctionSelect04 == 1)&&(OverallSign.CameraScanSign == 0))
	{
		if(OverallSign.LensMotionOverSign == LensMotionCompensationRun)
		{
			if(abs(DeltaStepAxis19_CPS) > 0)
			{
				*(Servo_Axis19.LPM_CounterPort) = abs(DeltaStepAxis19_CPS);
				*(Servo_Axis19.PWM_Port) = divdp( FPGA_MAX_F*System.Tsample, (abs(DeltaStepAxis19_CPS)+1));		
				DeltaStepAxis19_CPS = 0;
			}
			else
			{
				*(Servo_Axis19.LPM_CounterPort) = 0;
				*(Servo_Axis19.PWM_Port) = 10;		
				DeltaStepAxis19_CPS = 0;
			}
		}
		else if((NCRunSign.CodeRun_Over == 1)&&
			(SPCNCSign.CodeRun_Over == 1)&&
			(STDNCSign.CodeRun_Over == 1)&&
			(NCRunSign.CodeRun_Over_Module.Module11 == 1)&&
			(SPCNCSign.CodeRun_Over_Module.Module11 == 1)&&
			(STDNCSign.CodeRun_Over_Module.Module11 == 1)&&
			(DeltaStepAxis19_Bak == 0)&&
			(DeltaStepAxis19_CPS == 0)&&
			//(OverallSign.InterpolationRunCnt_1>0)&&
			(OverallSign.InterpolationRunCnt_1 == OverallSign.InterpolationSendCnt_1)

		)
		{
			*(Servo_Axis19.LPM_CounterPort) = 0;
			*(Servo_Axis19.PWM_Port) = 10;		
			DeltaStepAxis19_CPS = 0;
		}
	}
	
	#if 0
	if(System.FunctionSelect04 == 1)
	{
		if(OverallSign.CameraScanSign == 0)
		{
			if(abs(DeltaStepAxis19_CPS) > 0)
			{
				*(Servo_Axis19.LPM_CounterPort) = abs(DeltaStepAxis19_CPS);
				*(Servo_Axis19.PWM_Port) = divdp( FPGA_MAX_F*System.Tsample, (abs(DeltaStepAxis19_CPS)+1)); 	
				DeltaStepAxis19_CPS = 0;
			}
		}
	}
	#endif

	Servo_LPMEN_Register.bit.LPMEN_1 = 1;
	Write_Servo_LPMEN();

	#if 0
	MonitorInterPlationBuf[MonitorInterPlationBufi++] = OverallSign.InterpolationRunCnt_1;
	if( MonitorInterPlationBufi >= 1000 )
	{
		MonitorInterPlationBufi = 0;
	}
	#endif
	
	IRQ_ENABLE(IRQNUM_PLUSE1);		
	IRQ_ENABLE(IRQNUM_PLUSE2);
	IRQ_ENABLE(IRQNUM_PLUSE3);		
	IRQ_ENABLE(IRQNUM_PLUSE4);

	return 1;

}

	
Uint16 NCAreaPulseToServo_LPMEN2(void) 
{	//FPGA_A
	
	INTERPOLATION_BUFFER *InterpolationBufPT_2;
	
	OverallSign.Int4IdleSign_2 = 0;
	if(OverallSign.InterpolationRunCnt_2 >= OverallSign.InterpolationSendCnt_2)
		return 0;

	Read_Servo_Over1_In();
	if(OverallSign.EveryAxisOverValid1 == 0)
	{
		OverallSign.Int4IdleSign_2 = OverallSign.Int4IdleSign_2;
		return 0;
	}
	
	IRQ_DISABLE(IRQNUM_PLUSE1);		
	IRQ_DISABLE(IRQNUM_PLUSE2);
	IRQ_DISABLE(IRQNUM_PLUSE3);		
	IRQ_DISABLE(IRQNUM_PLUSE4);
			
	InterpolationBufPT_2 = &InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
	if(OverallSign.InterpolationRunCnt_2 < OverallSign.InterpolationSendCnt_2)
		OverallSign.InterpolationRunCnt_2++;

	if(
		(abs(InterpolationBufPT_2->DeltaPulse.Axis1)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis2)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis3)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis4)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis5)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis11)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis12)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis13)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis14)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis15)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis16)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis17)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis18)+
		abs(InterpolationBufPT_2->DeltaPulse.Axis23))
		== 0
		)
	{			
		IRQ_ENABLE(IRQNUM_PLUSE1);		
		IRQ_ENABLE(IRQNUM_PLUSE2);
		IRQ_ENABLE(IRQNUM_PLUSE3);		
		IRQ_ENABLE(IRQNUM_PLUSE4);
		*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<23);
		*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1<<23);
		*(GPIO_IRQStatus.GPIO0_IRQStatus_1) = (0x1<<23);
		*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_1) = (0x1<<23);
		return 0;
	}

	//Servo_LPMEN_Register.bit.LPMEN_1 = 0;
	Servo_LPMEN_Register.bit.LPMEN_2 = 0;
	Write_Servo_LPMEN();
	
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_1= InterpolationBufPT_2->SIGN.Axis1;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_2= InterpolationBufPT_2->SIGN.Axis2;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_3= InterpolationBufPT_2->SIGN.Axis3;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_4= InterpolationBufPT_2->SIGN.Axis4;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_5= InterpolationBufPT_2->SIGN.Axis5;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_6= InterpolationBufPT_2->SIGN.Axis6;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_7= InterpolationBufPT_2->SIGN.Axis7;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_8= InterpolationBufPT_2->SIGN.Axis8;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_9= InterpolationBufPT_2->SIGN.Axis9;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_10= InterpolationBufPT_2->SIGN.Axis10;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_11= InterpolationBufPT_2->SIGN.Axis11;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_12= InterpolationBufPT_2->SIGN.Axis12;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_13= InterpolationBufPT_2->SIGN.Axis13;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_14= InterpolationBufPT_2->SIGN.Axis14;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_15= InterpolationBufPT_2->SIGN.Axis15;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_16= InterpolationBufPT_2->SIGN.Axis16;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_17= InterpolationBufPT_2->SIGN.Axis17;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_18= InterpolationBufPT_2->SIGN.Axis18;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_19= InterpolationBufPT_2->SIGN.Axis19;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_20= InterpolationBufPT_2->SIGN.Axis20;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_21= InterpolationBufPT_2->SIGN.Axis21;
	//Servo_Sign2_Out_Register.bit.SERVO_SIGN_22= InterpolationBufPT_2->SIGN.Axis22;
	//Close SIGN.Axis23 for trigger camera
	//Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= InterpolationBufPT_2->SIGN.Axis23;

	if(System.FunctionSelect06 == 1)
	{
		if(InterpolationBufPT_2->DeltaPulse.Axis14 == 1)
		{
			Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 0;
		}
		else
		{
			Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 1;
		}
	}
		
	Write_Servo_Sign1_Out();
	//Write_Servo_Sign2_Out();

	//if(System.FunctionSelect05 == False)
	{
		if (GPIO_In1_Register.bit.GPIOIN12 == 1)
		{
			if(InterpolationBufPT_2->SIGN.Axis1 == 1)
			{
				if(abs(InterpolationBufPT_2->DeltaPulse.Axis1) > 0)
				{
					InterpolationBufPT_2->DeltaPulse.Axis1 = 0;
				}
			}
		}
	}

	*(Servo_Axis1.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis1);
	*(Servo_Axis2.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis2);
	*(Servo_Axis3.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis3);
	*(Servo_Axis4.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis4);
	*(Servo_Axis5.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis5);
	//*(Servo_Axis6.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis6);
	//*(Servo_Axis7.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis7);
	//*(Servo_Axis8.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis8);
	//*(Servo_Axis9.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis9);
	//*(Servo_Axis10.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis10);
	*(Servo_Axis11.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis11);
	*(Servo_Axis12.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis12);
	*(Servo_Axis13.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis13);
	*(Servo_Axis14.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis14);
	*(Servo_Axis15.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis15);
	*(Servo_Axis16.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis16);
	*(Servo_Axis17.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis17);
	*(Servo_Axis18.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis18);
	//*(Servo_Axis19.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis19);
	//*(Servo_Axis20.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis20);
	//*(Servo_Axis21.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis21);
	//*(Servo_Axis22.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis22);
	*(Servo_Axis23.LPM_CounterPort) = abs(InterpolationBufPT_2->DeltaPulse.Axis23);
	
	*(Servo_Axis1.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis1;
	*(Servo_Axis2.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis2;
	*(Servo_Axis3.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis3;
	*(Servo_Axis4.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis4;
	*(Servo_Axis5.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis5;
	//*(Servo_Axis6.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis6;
	//*(Servo_Axis7.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis7;
	//*(Servo_Axis8.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis8;
	//*(Servo_Axis9.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis9;
	//*(Servo_Axis10.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis10;
	*(Servo_Axis11.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis11;
	*(Servo_Axis12.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis12;
	*(Servo_Axis13.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis13;
	*(Servo_Axis14.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis14;
	*(Servo_Axis15.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis15;
	*(Servo_Axis16.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis16;
	*(Servo_Axis17.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis17;
	*(Servo_Axis18.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis18;
	//*(Servo_Axis19.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis19;
	//*(Servo_Axis20.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis20;
	//*(Servo_Axis21.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis21;
	//*(Servo_Axis22.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis22;
	*(Servo_Axis23.PWM_Port) = InterpolationBufPT_2->PWM_PeriodRegister.Axis23;

	if(InterpolationBufPT_2->DeltaPulse.Axis23 == 1)
	{
		//Write_LEDFlash_OUT(0x03,psParaArm_Ctrl->LEDFlashTime1,psParaArm_Ctrl->LEDFlashTime2);//flash time
		Write_LEDFlash_OUT(0x03,System.LEDFlashTime1,System.LEDFlashTime2);//flash time
	}

	//Servo_LPMEN_Register.bit.LPMEN_1 = 1;
	Servo_LPMEN_Register.bit.LPMEN_2 = 1;
	Write_Servo_LPMEN();

	#if 0
	MonitorInterPlationBuf[MonitorInterPlationBufi++] = OverallSign.InterpolationRunCnt_2;
	if( MonitorInterPlationBufi >= 1000 )
	{
		MonitorInterPlationBufi = 0;
	}
	#endif
	
	IRQ_ENABLE(IRQNUM_PLUSE1);		
	IRQ_ENABLE(IRQNUM_PLUSE2);
	IRQ_ENABLE(IRQNUM_PLUSE3);		
	IRQ_ENABLE(IRQNUM_PLUSE4);

	return 1;
}

Uint16  NCAreaRun(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign)
{
	Uint32 ri;
	INT40AXIS NowPointMachValue;
	float64 x1,y1,z1;	
	static Uint16 test1 = 0;		
	INT40AXIS PointMACH;

	ri = (AreaRunNCSign->RealRunCount-1) % STDGCODE_MOD;

	if(NcAreaRunStateCheck(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign) == 0)
	{	//for test version 1.1
		//return 0;
		//for test  version 1.2
		AreaRunNCSign->RealRunCount = AreaRunNCSign->RealRunCount;
	}
	
	NCAreaFeedrateOverride(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);	
	
	//NCAreaCalVeVm(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);
		
	if(AREAControl->VChangeSign==1)
	{	//如档位有变，则重新计算插补预处理
		AREAControl->VChangeSign = 0;
		if( AreaRunNCSign->NewAreaSign == 1 )	
		{
			//重新计算插补预处理继承			
			AreaRunNCSign->NewAreaSign = 0;
			AreaRunNCSign->PrecalCode = 0;	
			AREAControl->Pointer_EndCode = AreaRunNCSign->RealRunCount - 1;	//start new area
			(AreaGcodeBufferPointer+ri)->StartPoint = AREAControl->OldValue;

			STD_Deal_DeltaPoint(AreaGcodeBufferPointer+ri);
			STD_Deal_Vveer(AreaGcodeBufferPointer+ri);//重新计算矢量

			AreaRunNCSign->VeVmCalCount = AreaRunNCSign->RealRunCount;
			return 1;
		}		
	}
	
	if(AreaRunNCSign->PrecalCode == 0)	
	{		
		NCAreaRunPre(AREAControl, AreaGcodeBufferPointer, AreaRunNCSign);		
		AreaRunNCSign->PrecalCode = 1;
		SPCNCSign.PrecalCode = 1;
		STDNCSign.PrecalCode = 1;

		
		AREAControl->Stotal = AREAControl->Stotal_Module.Module1+
							AREAControl->Stotal_Module.Module2+
							AREAControl->Stotal_Module.Module3+
							AREAControl->Stotal_Module.Module4+
							AREAControl->Stotal_Module.Module5+
							AREAControl->Stotal_Module.Module6+
							AREAControl->Stotal_Module.Module7+
							AREAControl->Stotal_Module.Module8+
							AREAControl->Stotal_Module.Module9+
							AREAControl->Stotal_Module.Module10+
							AREAControl->Stotal_Module.Module11+
							AREAControl->Stotal_Module.Module12;
		
		if(AREAControl->Stotal == 0)
		{
			AreaRunNCSign->CodeRun_Over = 1;
			SPCNCSign.CodeRun_Over = 1;
			STDNCSign.CodeRun_Over = 1;		
			AreaRunNCSign->NewAreaSign = 0;
			return 1;
		}
		
		if((AreaGcodeBufferPointer+ri)->Stotal == 0)
		{
			AreaRunNCSign->CodeRun_Over = 1;
			SPCNCSign.CodeRun_Over = 1;
			STDNCSign.CodeRun_Over = 1;
			return 1;
		}
	}	 
		
	AREAControl->CurrentT = AREAControl->I*System.Tsample;
	AREAControl->CurrentT_Module.Module1 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module2 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module3 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module4 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module5 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module6 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module7 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module8 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module9 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module10 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module11 = AREAControl->CurrentT;
	AREAControl->CurrentT_Module.Module12 = AREAControl->CurrentT;

	NCAreaHyperbolaRun(AREAControl,1);
	NCAreaHyperbolaRun(AREAControl,2);
	NCAreaHyperbolaRun(AREAControl,3);
	NCAreaHyperbolaRun(AREAControl,4);
	NCAreaHyperbolaRun(AREAControl,5);
	NCAreaHyperbolaRun(AREAControl,6);
	NCAreaHyperbolaRun(AREAControl,7);
	NCAreaHyperbolaRun(AREAControl,8);
	NCAreaHyperbolaRun(AREAControl,9);
	NCAreaHyperbolaRun(AREAControl,10);
	NCAreaHyperbolaRun(AREAControl,11);
	NCAreaHyperbolaRun(AREAControl,12);
	
	AREAControl->Snow_Module.Module1= (AREAControl->Snow_Module.Module1 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module1);
	AREAControl->Snow_Module.Module2= (AREAControl->Snow_Module.Module2 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module2);
	AREAControl->Snow_Module.Module3= (AREAControl->Snow_Module.Module3 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module3);
	AREAControl->Snow_Module.Module4= (AREAControl->Snow_Module.Module4 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module4);
	AREAControl->Snow_Module.Module5= (AREAControl->Snow_Module.Module5 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module5);
	AREAControl->Snow_Module.Module6= (AREAControl->Snow_Module.Module6 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module6);
	AREAControl->Snow_Module.Module7= (AREAControl->Snow_Module.Module7 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module7);
	AREAControl->Snow_Module.Module8= (AREAControl->Snow_Module.Module8 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module8);
	AREAControl->Snow_Module.Module9= (AREAControl->Snow_Module.Module9 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module9);
	AREAControl->Snow_Module.Module10= (AREAControl->Snow_Module.Module10 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module10);
	AREAControl->Snow_Module.Module11= (AREAControl->Snow_Module.Module11 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module11);
	AREAControl->Snow_Module.Module12= (AREAControl->Snow_Module.Module12 < 0.000001) ? (0.000001):(AREAControl->Snow_Module.Module12);

	Int8	ReturnValue;
	ReturnValue = 0;
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,1);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,2);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,3);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,4);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,5);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,6);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,7);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,8);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,9);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,10);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,11);
	ReturnValue += NCAreaCalNowValue(AREAControl, AreaGcodeBufferPointer,AreaRunNCSign,12);

	if(ReturnValue == 12)
	{
		AreaRunNCSign->NewAreaSign = 0;
		AreaRunNCSign->CodeRun_Over = 1;
		SPCNCSign.CodeRun_Over = 1;
		STDNCSign.CodeRun_Over = 1;		
	}
	else if((ReturnValue == 11)&&
			(OverallSign.LensMotionOverSign == LensMotionCompensationRun))
	{
		return;
	}
		
	AREAControl->SnowLast_Module.Module1 = AREAControl->Snow_Module.Module1;
	AREAControl->SnowLast_Module.Module2 = AREAControl->Snow_Module.Module2;
	AREAControl->SnowLast_Module.Module3 = AREAControl->Snow_Module.Module3;
	AREAControl->SnowLast_Module.Module4 = AREAControl->Snow_Module.Module4;
	AREAControl->SnowLast_Module.Module5 = AREAControl->Snow_Module.Module5;
	AREAControl->SnowLast_Module.Module6 = AREAControl->Snow_Module.Module6;
	AREAControl->SnowLast_Module.Module7 = AREAControl->Snow_Module.Module7;
	AREAControl->SnowLast_Module.Module8 = AREAControl->Snow_Module.Module8;
	AREAControl->SnowLast_Module.Module9 = AREAControl->Snow_Module.Module9;
	AREAControl->SnowLast_Module.Module10 = AREAControl->Snow_Module.Module10;
	AREAControl->SnowLast_Module.Module11 = AREAControl->Snow_Module.Module11;
	AREAControl->SnowLast_Module.Module12 = AREAControl->Snow_Module.Module12;
	
	AREAControl->NowPointMACHPulseOut = ABSCoordinate_to_MachineCoordinate(AREAControl->NowValue);
	//for test compensation
	AREAControl->NowPointMACHPulseOut = nonlinear_pulse_out(AREAControl->NowPointMACHPulseOut);

	AREAControl->NowPointMACHPulseOut.Axis1 *= Servo_Axis1.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis2 *= Servo_Axis2.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis3 *= Servo_Axis3.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis4 *= Servo_Axis4.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis5 *= Servo_Axis5.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis6 *= Servo_Axis6.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis7 *= Servo_Axis7.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis8 *= Servo_Axis8.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis9 *= Servo_Axis9.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis10 *= Servo_Axis10.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis11 *= Servo_Axis11.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis12 *= Servo_Axis12.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis13 *= Servo_Axis13.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis14 *= Servo_Axis14.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis15 *= Servo_Axis15.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis16 *= Servo_Axis16.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis17 *= Servo_Axis17.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis18 *= Servo_Axis18.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis19 *= Servo_Axis19.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis20 *= Servo_Axis20.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis21 *= Servo_Axis21.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis22 *= Servo_Axis22.AxisOutUnitEQU; //for test
	AREAControl->NowPointMACHPulseOut.Axis23 *= Servo_Axis23.AxisOutUnitEQU; //for test
		
	NCArea_check_endpoint_value(AREAControl);
	NCArea_check_point_direction(AREAControl);

	AREAControl->DeltaStep.Axis1 = AREAControl->NowPointMACHPulseOut.Axis1-AREAControl->OldPointMACHPulseOut.Axis1; 
	AREAControl->DeltaStep.Axis2 = AREAControl->NowPointMACHPulseOut.Axis2-AREAControl->OldPointMACHPulseOut.Axis2; 
	AREAControl->DeltaStep.Axis3 = AREAControl->NowPointMACHPulseOut.Axis3-AREAControl->OldPointMACHPulseOut.Axis3;  
	AREAControl->DeltaStep.Axis4 = AREAControl->NowPointMACHPulseOut.Axis4-AREAControl->OldPointMACHPulseOut.Axis4;
	AREAControl->DeltaStep.Axis5 = AREAControl->NowPointMACHPulseOut.Axis5-AREAControl->OldPointMACHPulseOut.Axis5; 
	AREAControl->DeltaStep.Axis6 = AREAControl->NowPointMACHPulseOut.Axis6-AREAControl->OldPointMACHPulseOut.Axis6; 
	AREAControl->DeltaStep.Axis7 = AREAControl->NowPointMACHPulseOut.Axis7-AREAControl->OldPointMACHPulseOut.Axis7;  
	AREAControl->DeltaStep.Axis8 = AREAControl->NowPointMACHPulseOut.Axis8-AREAControl->OldPointMACHPulseOut.Axis8;
	AREAControl->DeltaStep.Axis9 = AREAControl->NowPointMACHPulseOut.Axis9-AREAControl->OldPointMACHPulseOut.Axis9; 
	AREAControl->DeltaStep.Axis10 = AREAControl->NowPointMACHPulseOut.Axis10-AREAControl->OldPointMACHPulseOut.Axis10; 
	AREAControl->DeltaStep.Axis11 = AREAControl->NowPointMACHPulseOut.Axis11-AREAControl->OldPointMACHPulseOut.Axis11; 
	AREAControl->DeltaStep.Axis12 = AREAControl->NowPointMACHPulseOut.Axis12-AREAControl->OldPointMACHPulseOut.Axis12; 
	AREAControl->DeltaStep.Axis13 = AREAControl->NowPointMACHPulseOut.Axis13-AREAControl->OldPointMACHPulseOut.Axis13;  
	AREAControl->DeltaStep.Axis14 = AREAControl->NowPointMACHPulseOut.Axis14-AREAControl->OldPointMACHPulseOut.Axis14;
	AREAControl->DeltaStep.Axis15 = AREAControl->NowPointMACHPulseOut.Axis15-AREAControl->OldPointMACHPulseOut.Axis15; 
	AREAControl->DeltaStep.Axis16 = AREAControl->NowPointMACHPulseOut.Axis16-AREAControl->OldPointMACHPulseOut.Axis16; 
	AREAControl->DeltaStep.Axis17 = AREAControl->NowPointMACHPulseOut.Axis17-AREAControl->OldPointMACHPulseOut.Axis17;  
	AREAControl->DeltaStep.Axis18 = AREAControl->NowPointMACHPulseOut.Axis18-AREAControl->OldPointMACHPulseOut.Axis18;
	AREAControl->DeltaStep.Axis19 = AREAControl->NowPointMACHPulseOut.Axis19-AREAControl->OldPointMACHPulseOut.Axis19; 
	//for test
	//AREAControl->DeltaStep.Axis19 = 0;
	AREAControl->DeltaStep.Axis20 = AREAControl->NowPointMACHPulseOut.Axis20-AREAControl->OldPointMACHPulseOut.Axis20; 
	AREAControl->DeltaStep.Axis21 = AREAControl->NowPointMACHPulseOut.Axis21-AREAControl->OldPointMACHPulseOut.Axis21; 
	AREAControl->DeltaStep.Axis22 = AREAControl->NowPointMACHPulseOut.Axis22-AREAControl->OldPointMACHPulseOut.Axis22; 
	AREAControl->DeltaStep.Axis23 = AREAControl->NowPointMACHPulseOut.Axis23-AREAControl->OldPointMACHPulseOut.Axis23;  

	NcAreaCameraShutter(AREAControl,AreaRunNCSign);
	NcAreaOpticalFlash(AREAControl,AreaRunNCSign);

	NCAreaPrePulseToServo(AREAControl);

	AREAControl->OldPointMACHPulseOut.Axis1 = AREAControl->NowPointMACHPulseOut.Axis1; 
	AREAControl->OldPointMACHPulseOut.Axis2 = AREAControl->NowPointMACHPulseOut.Axis2; 
	AREAControl->OldPointMACHPulseOut.Axis3 = AREAControl->NowPointMACHPulseOut.Axis3;	
	AREAControl->OldPointMACHPulseOut.Axis4 = AREAControl->NowPointMACHPulseOut.Axis4; 
	AREAControl->OldPointMACHPulseOut.Axis5 = AREAControl->NowPointMACHPulseOut.Axis5; 
	AREAControl->OldPointMACHPulseOut.Axis6 = AREAControl->NowPointMACHPulseOut.Axis6; 
	AREAControl->OldPointMACHPulseOut.Axis7 = AREAControl->NowPointMACHPulseOut.Axis7;	
	AREAControl->OldPointMACHPulseOut.Axis8 = AREAControl->NowPointMACHPulseOut.Axis8; 
	AREAControl->OldPointMACHPulseOut.Axis9 = AREAControl->NowPointMACHPulseOut.Axis9; 
	AREAControl->OldPointMACHPulseOut.Axis10 = AREAControl->NowPointMACHPulseOut.Axis10; 
	AREAControl->OldPointMACHPulseOut.Axis11 = AREAControl->NowPointMACHPulseOut.Axis11; 
	AREAControl->OldPointMACHPulseOut.Axis12 = AREAControl->NowPointMACHPulseOut.Axis12; 
	AREAControl->OldPointMACHPulseOut.Axis13 = AREAControl->NowPointMACHPulseOut.Axis13;	
	AREAControl->OldPointMACHPulseOut.Axis14 = AREAControl->NowPointMACHPulseOut.Axis14; 
	AREAControl->OldPointMACHPulseOut.Axis15 = AREAControl->NowPointMACHPulseOut.Axis15; 
	AREAControl->OldPointMACHPulseOut.Axis16 = AREAControl->NowPointMACHPulseOut.Axis16; 
	AREAControl->OldPointMACHPulseOut.Axis17 = AREAControl->NowPointMACHPulseOut.Axis17;	
	AREAControl->OldPointMACHPulseOut.Axis18 = AREAControl->NowPointMACHPulseOut.Axis18; 
	AREAControl->OldPointMACHPulseOut.Axis19 = AREAControl->NowPointMACHPulseOut.Axis19; 
	AREAControl->OldPointMACHPulseOut.Axis20 = AREAControl->NowPointMACHPulseOut.Axis20; 
	AREAControl->OldPointMACHPulseOut.Axis21 = AREAControl->NowPointMACHPulseOut.Axis21; 
	AREAControl->OldPointMACHPulseOut.Axis22 = AREAControl->NowPointMACHPulseOut.Axis22; 
	AREAControl->OldPointMACHPulseOut.Axis23 = AREAControl->NowPointMACHPulseOut.Axis23;	
	
	AREAControl->OldValue.Axis1 = AREAControl->NowValue.Axis1;
	AREAControl->OldValue.Axis2 = AREAControl->NowValue.Axis2;
	AREAControl->OldValue.Axis3 = AREAControl->NowValue.Axis3;
	AREAControl->OldValue.Axis4 = AREAControl->NowValue.Axis4;
	AREAControl->OldValue.Axis5 = AREAControl->NowValue.Axis5;
	AREAControl->OldValue.Axis6 = AREAControl->NowValue.Axis6;
	AREAControl->OldValue.Axis7 = AREAControl->NowValue.Axis7;
	AREAControl->OldValue.Axis8 = AREAControl->NowValue.Axis8;
	AREAControl->OldValue.Axis9 = AREAControl->NowValue.Axis9;
	AREAControl->OldValue.Axis10 = AREAControl->NowValue.Axis10;
	AREAControl->OldValue.Axis11 = AREAControl->NowValue.Axis11;
	AREAControl->OldValue.Axis12 = AREAControl->NowValue.Axis12;
	AREAControl->OldValue.Axis13 = AREAControl->NowValue.Axis13;
	AREAControl->OldValue.Axis14 = AREAControl->NowValue.Axis14;
	AREAControl->OldValue.Axis15 = AREAControl->NowValue.Axis15;
	AREAControl->OldValue.Axis16 = AREAControl->NowValue.Axis16;
	AREAControl->OldValue.Axis17 = AREAControl->NowValue.Axis17;
	AREAControl->OldValue.Axis18 = AREAControl->NowValue.Axis18;
	AREAControl->OldValue.Axis19 = AREAControl->NowValue.Axis19;
	AREAControl->OldValue.Axis20 = AREAControl->NowValue.Axis20;
	AREAControl->OldValue.Axis21 = AREAControl->NowValue.Axis21;
	AREAControl->OldValue.Axis22 = AREAControl->NowValue.Axis22;
	AREAControl->OldValue.Axis23 = AREAControl->NowValue.Axis23;

	//NCAreaMonitorBuffer(AREAControl->DeltaStep.Axis2,AREAControl->DeltaStep.Axis1,AREAControl->DeltaStep.Axis15,0);
	
	NCAreaEnsureDirection(AREAControl,AreaRunNCSign);
	NCAreaJudgeLimit(AREAControl);			//如到极限位，相应轴输出脉冲清除			
	NCAreaCal_V_tpr(AREAControl);				
	NCAreaWriteInterpolationBuffer(AREAControl);
	//NCAreaPrePulseToServo(AREAControl);
 	return 1;
}


