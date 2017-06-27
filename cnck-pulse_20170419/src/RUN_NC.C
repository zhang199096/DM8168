#include "..\\include\\main_dsp.h"

//通用变量和标志
SYSTEM System;			//系统参数结构对象
ERROR Error;			//报警结构对象
OVERALLSIGN OverallSign;	// 总体标志
DACONVERT  DA_Convert1;//DA转换1
DACONVERT  DA_AD5315_G106;//DA_G106

GCODE GCodeBuffer[STDGCODE_MOD];	// G-code 结构缓冲数组
GCODE NCAreaGCodeBuffer[STDGCODE_MOD];	//区域计算代码缓冲数组

GCODE AddLineGCodeBuffer_Incubator[ADDGCODEINCUBATOR_MOD];//增加20条代码用于培养箱定位
GCODE SPCGCodeBuffer[SPCGCODE_MOD];	//SPC G-code 结构缓冲数组
UINT16AXIS LastPrecalMoveSign;	// 以前的移动方向
UINT16AXIS NowPrecalMoveSign;	// 现在的移动方向

NC_RUN_SIGN NCRunSign;	//区域代码各组群状态和通讯状态结构对象
NC_RUN_SIGN STDNCSign;			//NC代码运行标志和变量//用于标准G代码
NC_RUN_SIGN SPCNCSign;//NC代码运行标志和变量//用于HANDLE  JOG  REF 等功能用特殊G代码


REFERENCE Reference;//参考点搜索
AxisCompensation AxisCompBuffer[COMPBUFFERLENGTH];//非线补
extern Int32		DeltaStepAxis19_CPS;

//判断轨迹是否偏离位置  CH0701
//判断轨迹静态时是否偏离位置 各轴判定界限0.01mm 
Uint16 check_cycle = 0, SampleCycle = 0;
INT40AXIS LastNowValue;
UINT16AXIS StaticErrorTimes;
Uint16 STD_check_track_run_over(void) 
{
	double TrackLengthSQR;		//unit:mm
	float64AXIS TrackDelta;
	double TrackDelta_Max=0;
	Uint32 TrackRunOverSign;

	if (OverallSign.GetMachCoordinateRight == 0) 
	{
		return 0;
	}

	if (SampleCycle > 30) 
	{
		SampleCycle = 0;
	} 
	else 
	{
		SampleCycle++;
		return 0;
	}

	if (STDNCSign.GetPositionSign == 1)	 // 需要获得稳定的坐标值
	{
		check_cycle = 0;
		StaticErrorTimes.Axis1 = 0;
		StaticErrorTimes.Axis2 = 0;
		StaticErrorTimes.Axis3 = 0;
		StaticErrorTimes.Axis4 = 0;
		StaticErrorTimes.Axis5 = 0;
		StaticErrorTimes.Axis6 = 0;
		StaticErrorTimes.Axis7 = 0;
		StaticErrorTimes.Axis8 = 0;
		StaticErrorTimes.Axis9 = 0;
		StaticErrorTimes.Axis10 = 0;
		StaticErrorTimes.Axis11 = 0;
		StaticErrorTimes.Axis12 = 0;
		StaticErrorTimes.Axis13 = 0;
		StaticErrorTimes.Axis14 = 0;
		StaticErrorTimes.Axis15 = 0;
		StaticErrorTimes.Axis16 = 0;
		StaticErrorTimes.Axis17 = 0;
		StaticErrorTimes.Axis18 = 0;
		StaticErrorTimes.Axis19 = 0;
		StaticErrorTimes.Axis20 = 0;
		StaticErrorTimes.Axis21 = 0;
		StaticErrorTimes.Axis22 = 0;
		StaticErrorTimes.Axis23 = 0;
		StaticErrorTimes.Axis24 = 0;
		StaticErrorTimes.Axis25 = 0;
		return 0;
	}

	if (STDNCSign.RealRunCount > 0) 
	{
		if (System.TrackRunOutRangeSQR > 10.0) 
		{
			TrackDelta.Axis1 = labs(NC_AREAControl.NowValue.Axis1 - OverallSign.ABS_Coordinate.Axis1) * System.UnitTo_mm;
			TrackDelta_Max = TrackDelta.Axis1;
			TrackRunOverSign = 0x00000001;
			
 			TrackDelta.Axis2 = labs(NC_AREAControl.NowValue.Axis2 - OverallSign.ABS_Coordinate.Axis2) * System.UnitTo_mm;
			if(TrackDelta.Axis2 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis2;
				TrackRunOverSign = 0x00000002;
			}
			
			TrackDelta.Axis3 = labs(NC_AREAControl.NowValue.Axis3 - OverallSign.ABS_Coordinate.Axis3) * System.UnitTo_mm;
			if(TrackDelta.Axis3 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis3;
				TrackRunOverSign = 0x00000004;
			}

			TrackDelta.Axis4 = labs(NC_AREAControl.NowValue.Axis4 - OverallSign.ABS_Coordinate.Axis4) * System.UnitTo_mm;
			if(TrackDelta.Axis4 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis4;
				TrackRunOverSign = 0x00000008;
			}

			TrackDelta.Axis5 = labs(NC_AREAControl.NowValue.Axis5 - OverallSign.ABS_Coordinate.Axis5) * System.UnitTo_mm;
			if(TrackDelta.Axis5 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis5;
				TrackRunOverSign = 0x00000010;
			}

			TrackDelta.Axis6 = labs(NC_AREAControl.NowValue.Axis6 - OverallSign.ABS_Coordinate.Axis6) * System.UnitTo_mm;
			if(TrackDelta.Axis6 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis6;
				TrackRunOverSign = 0x00000020;
			}

			TrackDelta.Axis7 = labs(NC_AREAControl.NowValue.Axis7 - OverallSign.ABS_Coordinate.Axis7) * System.UnitTo_mm;
			if(TrackDelta.Axis7 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis7;
				TrackRunOverSign = 0x0000040;
			}

			TrackDelta.Axis8 = labs(NC_AREAControl.NowValue.Axis8 - OverallSign.ABS_Coordinate.Axis8) * System.UnitTo_mm;
			if(TrackDelta.Axis8 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis8;
				TrackRunOverSign = 0x00000080;
			}

			TrackDelta.Axis9 = labs(NC_AREAControl.NowValue.Axis9 - OverallSign.ABS_Coordinate.Axis9) * System.UnitTo_mm;
			if(TrackDelta.Axis9 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis9;
				TrackRunOverSign = 0x00000100;
			}

			TrackDelta.Axis10 = labs(NC_AREAControl.NowValue.Axis10 - OverallSign.ABS_Coordinate.Axis10) * System.UnitTo_mm;
			if(TrackDelta.Axis10 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis10;
				TrackRunOverSign = 0x00000200;
			}

			TrackDelta.Axis11 = labs(NC_AREAControl.NowValue.Axis11 - OverallSign.ABS_Coordinate.Axis11) * System.UnitTo_mm;
			if(TrackDelta.Axis11 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis11;
				TrackRunOverSign = 0x00000400;
			}

			TrackDelta.Axis12 = labs(NC_AREAControl.NowValue.Axis12 - OverallSign.ABS_Coordinate.Axis12) * System.UnitTo_mm;
			if(TrackDelta.Axis12 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis12;
				TrackRunOverSign = 0x00000800;
			}

			TrackDelta.Axis13 = labs(NC_AREAControl.NowValue.Axis13 - OverallSign.ABS_Coordinate.Axis13) * System.UnitTo_mm;
			if(TrackDelta.Axis13 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis13;
				TrackRunOverSign = 0x00001000;
			}

			TrackDelta.Axis14 = labs(NC_AREAControl.NowValue.Axis14 - OverallSign.ABS_Coordinate.Axis14) * System.UnitTo_mm;
			if(TrackDelta.Axis14 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis14;
				TrackRunOverSign = 0x00002000;
			}

			TrackDelta.Axis15 = labs(NC_AREAControl.NowValue.Axis15 - OverallSign.ABS_Coordinate.Axis15) * System.UnitTo_mm;
			if(TrackDelta.Axis15 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis15;
				TrackRunOverSign = 0x00004000;
			}

			TrackDelta.Axis16 = labs(NC_AREAControl.NowValue.Axis16 - OverallSign.ABS_Coordinate.Axis16) * System.UnitTo_mm;
			if(TrackDelta.Axis16 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis16;
				TrackRunOverSign = 0x00008000;
			}

			TrackDelta.Axis17 = labs(NC_AREAControl.NowValue.Axis17 - OverallSign.ABS_Coordinate.Axis17) * System.UnitTo_mm;
			if(TrackDelta.Axis17 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis17;
				TrackRunOverSign = 0x00010000;
			}

			TrackDelta.Axis18 = labs(NC_AREAControl.NowValue.Axis18 - OverallSign.ABS_Coordinate.Axis18) * System.UnitTo_mm;
			if(TrackDelta.Axis18 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis18;
				TrackRunOverSign = 0x00020000;
			}

			TrackDelta.Axis19 = labs(NC_AREAControl.NowValue.Axis19 - OverallSign.ABS_Coordinate.Axis19) * System.UnitTo_mm;
			if(TrackDelta.Axis19 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis19;
				TrackRunOverSign = 0x00040000;
			}

			TrackDelta.Axis20 = labs(NC_AREAControl.NowValue.Axis20 - OverallSign.ABS_Coordinate.Axis20) * System.UnitTo_mm;
			if(TrackDelta.Axis20 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis20;
				TrackRunOverSign = 0x00080000;
			}

			TrackDelta.Axis21 = labs(NC_AREAControl.NowValue.Axis21 - OverallSign.ABS_Coordinate.Axis21) * System.UnitTo_mm;
			if(TrackDelta.Axis21 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis21;
				TrackRunOverSign = 0x00100000;
			}

			TrackDelta.Axis22 = labs(NC_AREAControl.NowValue.Axis22 - OverallSign.ABS_Coordinate.Axis22) * System.UnitTo_mm;
			if(TrackDelta.Axis22 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis22;
				TrackRunOverSign = 0x00200000;
			}

			TrackDelta.Axis23 = labs(NC_AREAControl.NowValue.Axis23 - OverallSign.ABS_Coordinate.Axis23) * System.UnitTo_mm;
			if(TrackDelta.Axis23 > TrackDelta_Max)
			{
				TrackDelta_Max = TrackDelta.Axis23;
				TrackRunOverSign = 0x00400000;
			}

			TrackDelta.Axis24 = 0;
			TrackDelta.Axis25 = 0;

			TrackLengthSQR = powdp(TrackDelta.Axis1, 2)
					+ powdp(TrackDelta.Axis2, 2) + powdp(TrackDelta.Axis3, 2)
					+ powdp(TrackDelta.Axis4, 2) + powdp(TrackDelta.Axis5, 2)
					+ powdp(TrackDelta.Axis6, 2) + powdp(TrackDelta.Axis7, 2)
					+ powdp(TrackDelta.Axis8, 2) + powdp(TrackDelta.Axis9, 2)
					+ powdp(TrackDelta.Axis10, 2) + powdp(TrackDelta.Axis11, 2)
					+ powdp(TrackDelta.Axis12, 2) + powdp(TrackDelta.Axis13, 2)
					+ powdp(TrackDelta.Axis14, 2) + powdp(TrackDelta.Axis15, 2)
					+ powdp(TrackDelta.Axis16, 2) + powdp(TrackDelta.Axis17, 2)
					+ powdp(TrackDelta.Axis18, 2) + powdp(TrackDelta.Axis19, 2)
					+ powdp(TrackDelta.Axis20, 2) + powdp(TrackDelta.Axis21, 2)
					+ powdp(TrackDelta.Axis22, 2) + powdp(TrackDelta.Axis23, 2)
					+ powdp(TrackDelta.Axis24, 2) + powdp(TrackDelta.Axis25, 2);
			if (TrackLengthSQR > System.TrackRunOutRangeSQR) 
			{
				check_cycle++;
				if (check_cycle > 10) 
				{
					Error.TrackRunOutError = 1;
					psDSPPacketOut->TrackRunOver = TrackRunOverSign;
					high_error();
				}
			} 
			else 
			{
				check_cycle = 0;
			}
		}
		else 
		{		
			return 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_1) 
		{
			if (TrackDelta.Axis1 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis1++;
				if (StaticErrorTimes.Axis1 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis1 == LastNowValue.Axis1) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000001;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis1 = NC_AREAControl.NowValue.Axis1;
						StaticErrorTimes.Axis1 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis1 = 0;
			}
		} 
		else 
		{
			StaticErrorTimes.Axis1 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_2) 
		{
			if (TrackDelta.Axis2 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis2++;
				if (StaticErrorTimes.Axis2 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis2 == LastNowValue.Axis2) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000002;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis2 = NC_AREAControl.NowValue.Axis2;
						StaticErrorTimes.Axis2 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis2 = 0;
			}
		} 
		else 
		{
		
			StaticErrorTimes.Axis2 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_3) 
		{
			if (TrackDelta.Axis3 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis3++;
				if (StaticErrorTimes.Axis3 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis3 == LastNowValue.Axis3) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000004;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis3 = NC_AREAControl.NowValue.Axis3;
						StaticErrorTimes.Axis3 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis3 = 0;
			}
		} 
		else 
		{
		
			StaticErrorTimes.Axis3 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_4) 
		{
			if (TrackDelta.Axis4 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis4++;
				if (StaticErrorTimes.Axis4 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis4 == LastNowValue.Axis4) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000004;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis4 = NC_AREAControl.NowValue.Axis4;
						StaticErrorTimes.Axis4 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis4 = 0;
			}
		} 
		else
		{
			StaticErrorTimes.Axis4 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_5) 
		{
			if (TrackDelta.Axis5 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis5++;
				if (StaticErrorTimes.Axis5 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis5 == LastNowValue.Axis5) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000010;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis5 = NC_AREAControl.NowValue.Axis5;
						StaticErrorTimes.Axis5 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis5 = 0;
			}
		} 
		else 
		{
			StaticErrorTimes.Axis5 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_6) 
		{
			if (TrackDelta.Axis6 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis6++;
				if (StaticErrorTimes.Axis6 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis6 == LastNowValue.Axis6) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000020;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis6 = NC_AREAControl.NowValue.Axis6;
						StaticErrorTimes.Axis6 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis6 = 0;
			}
		} 
		else 
		{
			StaticErrorTimes.Axis6 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_7) 
		{
			if (TrackDelta.Axis7 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis7++;
				if (StaticErrorTimes.Axis7 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis7 == LastNowValue.Axis7) {
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000040;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis7 = NC_AREAControl.NowValue.Axis7;
						StaticErrorTimes.Axis7 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis7 = 0;
			}
		} 
		else 
		{
			StaticErrorTimes.Axis7 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_8) 
		{
			if (TrackDelta.Axis8 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis8++;
				if (StaticErrorTimes.Axis8 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis8 == LastNowValue.Axis8) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000080;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis8 = NC_AREAControl.NowValue.Axis8;
						StaticErrorTimes.Axis8 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis8 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis8 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_9) 
		{
			if (TrackDelta.Axis9 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis9++;
				if (StaticErrorTimes.Axis9 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis9 == LastNowValue.Axis9) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000100;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis9 = NC_AREAControl.NowValue.Axis9;
						StaticErrorTimes.Axis9 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis9 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis9 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_10) 
		{
			if (TrackDelta.Axis10 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis10++;
				if (StaticErrorTimes.Axis10 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis10 == LastNowValue.Axis10) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000200;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis10 = NC_AREAControl.NowValue.Axis10;
						StaticErrorTimes.Axis10 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis10 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis10 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_11) 
		{
			if (TrackDelta.Axis11 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis11++;
				if (StaticErrorTimes.Axis11 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis11 == LastNowValue.Axis11) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000400;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis11 = NC_AREAControl.NowValue.Axis11;
						StaticErrorTimes.Axis11 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis11 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis11 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_12) 
		{
			if (TrackDelta.Axis12 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis12++;
				if (StaticErrorTimes.Axis12 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis12 == LastNowValue.Axis12) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00000800;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis12 = NC_AREAControl.NowValue.Axis12;
						StaticErrorTimes.Axis12 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis12 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis12 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_13) 
		{
			if (TrackDelta.Axis13 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis13++;
				if (StaticErrorTimes.Axis13 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis13 == LastNowValue.Axis13) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00001000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis13 = NC_AREAControl.NowValue.Axis13;
						StaticErrorTimes.Axis13 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis13 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis13 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_14) 
		{
			if (TrackDelta.Axis14 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis14++;
				if (StaticErrorTimes.Axis14 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis14 == LastNowValue.Axis14) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00002000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis14 = NC_AREAControl.NowValue.Axis14;
						StaticErrorTimes.Axis14 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis14 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis14 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_15) 
		{
			if (TrackDelta.Axis15 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis15++;
				if (StaticErrorTimes.Axis15 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis15 == LastNowValue.Axis15) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00004000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis15 = NC_AREAControl.NowValue.Axis15;
						StaticErrorTimes.Axis15 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis15 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis15 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_16) 
		{
			if (TrackDelta.Axis16 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis16++;
				if (StaticErrorTimes.Axis16 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis16 == LastNowValue.Axis16) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00008000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis16 = NC_AREAControl.NowValue.Axis16;
						StaticErrorTimes.Axis16 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis16 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis16 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_17) 
		{
			if (TrackDelta.Axis17 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis17++;
				if (StaticErrorTimes.Axis17 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis17 == LastNowValue.Axis17) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00010000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis17 = NC_AREAControl.NowValue.Axis17;
						StaticErrorTimes.Axis17 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis17 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis17 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_18) 
		{
			if (TrackDelta.Axis18 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis18++;
				if (StaticErrorTimes.Axis18 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis18 == LastNowValue.Axis18) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00020000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis18 = NC_AREAControl.NowValue.Axis18;
						StaticErrorTimes.Axis18 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis18 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis18 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_19) 
		{
			if (TrackDelta.Axis19 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis19++;
				if (StaticErrorTimes.Axis19 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis19 == LastNowValue.Axis19) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00040000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis19 = NC_AREAControl.NowValue.Axis19;
						StaticErrorTimes.Axis19 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis19 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis19 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_20) 
		{
			if (TrackDelta.Axis20 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis20++;
				if (StaticErrorTimes.Axis20 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis20 == LastNowValue.Axis20) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00080000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis20 = NC_AREAControl.NowValue.Axis20;
						StaticErrorTimes.Axis20 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis20 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis20 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_21) 
		{
			if (TrackDelta.Axis21 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis21++;
				if (StaticErrorTimes.Axis21 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis21 == LastNowValue.Axis21) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00100000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis21 = NC_AREAControl.NowValue.Axis21;
						StaticErrorTimes.Axis21 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis21 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis21 = 0;
		}

		if (Servo_Coin2_In_Register.bit.COIN_22) 
		{
			if (TrackDelta.Axis22 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis22++;
				if (StaticErrorTimes.Axis22 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis22 == LastNowValue.Axis22) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00200000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis22 = NC_AREAControl.NowValue.Axis22;
						StaticErrorTimes.Axis22 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis22 = 0;
			}
		}
		else 
		{
			StaticErrorTimes.Axis22 = 0;
		}

		if (Servo_Coin1_In_Register.bit.COIN_23) 
		{
			if (TrackDelta.Axis23 > StaticTrackLimit) 
			{
				StaticErrorTimes.Axis23++;
				if (StaticErrorTimes.Axis23 > Max_StaticErrorTimes) 
				{
					if (NC_AREAControl.NowValue.Axis23 == LastNowValue.Axis23) 
					{
						Error.TrackRunOutError = 1;
						psDSPPacketOut->TrackRunOver = 0x00400000;
						high_error();
					} 
					else 
					{
						LastNowValue.Axis23 = NC_AREAControl.NowValue.Axis23;
						StaticErrorTimes.Axis23 = 0;
					}
				}
			} 
			else 
			{
				StaticErrorTimes.Axis23 = 0;
			}
		} 
		else 
		{
			StaticErrorTimes.Axis23 = 0;
		}

	} 
	else 
	{
		StaticErrorTimes.Axis1 = 0;
		StaticErrorTimes.Axis2 = 0;
		StaticErrorTimes.Axis3 = 0;
		StaticErrorTimes.Axis4 = 0;
		StaticErrorTimes.Axis5 = 0;
		StaticErrorTimes.Axis6 = 0;
		StaticErrorTimes.Axis7 = 0;
		StaticErrorTimes.Axis8 = 0;
		StaticErrorTimes.Axis9 = 0;
		StaticErrorTimes.Axis10 = 0;
		StaticErrorTimes.Axis11 = 0;
		StaticErrorTimes.Axis12 = 0;
		StaticErrorTimes.Axis13 = 0;
		StaticErrorTimes.Axis14 = 0;
		StaticErrorTimes.Axis15 = 0;
		StaticErrorTimes.Axis16 = 0;
		StaticErrorTimes.Axis17 = 0;
		StaticErrorTimes.Axis18 = 0;
		StaticErrorTimes.Axis19 = 0;
		StaticErrorTimes.Axis20 = 0;
		StaticErrorTimes.Axis21 = 0;
		StaticErrorTimes.Axis22 = 0;
		StaticErrorTimes.Axis23 = 0;
	}

	return 1;
}

// 获得定点坐标并多次采样判定
Uint16 get_and_check_coordinate(void) {
	INT40AXIS NowValue1, NowValue2, NowValue3;  //当前绝对坐标位置值 um or ud
	INT40AXIS NowPointMachValue, NowPointMachPulseOut;
	INT40AXIS PointMACH;

	if (1)  
	{
		NowValue1.Axis1 = OverallSign.ABS_Coordinate.Axis1;
		NowValue1.Axis2 = OverallSign.ABS_Coordinate.Axis2;
		NowValue1.Axis3 = OverallSign.ABS_Coordinate.Axis3;
		NowValue1.Axis4 = OverallSign.ABS_Coordinate.Axis4;
		NowValue1.Axis5 = OverallSign.ABS_Coordinate.Axis5;
		NowValue1.Axis6 = OverallSign.ABS_Coordinate.Axis6;
		NowValue1.Axis7 = OverallSign.ABS_Coordinate.Axis7;
		NowValue1.Axis8 = OverallSign.ABS_Coordinate.Axis8;
		NowValue1.Axis9 = OverallSign.ABS_Coordinate.Axis9;
		NowValue1.Axis10 = OverallSign.ABS_Coordinate.Axis10;
		NowValue1.Axis11 = OverallSign.ABS_Coordinate.Axis11;
		NowValue1.Axis12 = OverallSign.ABS_Coordinate.Axis12;
		NowValue1.Axis13 = OverallSign.ABS_Coordinate.Axis13;
		NowValue1.Axis14 = OverallSign.ABS_Coordinate.Axis14;
		NowValue1.Axis15 = OverallSign.ABS_Coordinate.Axis15;
		NowValue1.Axis16 = OverallSign.ABS_Coordinate.Axis16;
		NowValue1.Axis17 = OverallSign.ABS_Coordinate.Axis17;
		NowValue1.Axis18 = OverallSign.ABS_Coordinate.Axis18;
		NowValue1.Axis19 = OverallSign.ABS_Coordinate.Axis19;
		NowValue1.Axis20 = OverallSign.ABS_Coordinate.Axis20;
		NowValue1.Axis21 = OverallSign.ABS_Coordinate.Axis21;
		NowValue1.Axis22 = OverallSign.ABS_Coordinate.Axis22;
		NowValue1.Axis23 = OverallSign.ABS_Coordinate.Axis23;

		do {
			OverallSign.GetMachCoordinateRight = 1;
			if (pre_get_MACH_counter(&Encoder_Axis1))
				get_MACH_value(&Encoder_Axis1);
			#if 0
			if (pre_get_MACH_counter(&Encoder_Axis2))
				get_MACH_value(&Encoder_Axis2);
			#else
				Encoder_Axis2.MACH_PositionValue = NC_AREAControl.OldValue.Axis2;
			#endif
			
			if (pre_get_MACH_counter(&Encoder_Axis3))
				get_MACH_value(&Encoder_Axis3);
			if (pre_get_MACH_counter(&Encoder_Axis4))
				get_MACH_value(&Encoder_Axis4);
			if (pre_get_MACH_counter(&Encoder_Axis5))
				get_MACH_value(&Encoder_Axis5);
			if (pre_get_MACH_counter(&Encoder_Axis6))
				get_MACH_value(&Encoder_Axis6);
			if (pre_get_MACH_counter(&Encoder_Axis7))
				get_MACH_value(&Encoder_Axis7);
			if (pre_get_MACH_counter(&Encoder_Axis8))
				get_MACH_value(&Encoder_Axis8);
			if (pre_get_MACH_counter(&Encoder_Axis9))
				get_MACH_value(&Encoder_Axis9);

			#if 0
			if (pre_get_MACH_counter(&Encoder_Axis10))
				get_MACH_value(&Encoder_Axis10);
			#else
				Encoder_Axis10.MACH_PositionValue = NC_AREAControl.OldValue.Axis10;
			#endif
			
			if (pre_get_MACH_counter(&Encoder_Axis11))
				get_MACH_value(&Encoder_Axis11);
			if (pre_get_MACH_counter(&Encoder_Axis12))
				get_MACH_value(&Encoder_Axis12);
			if (pre_get_MACH_counter(&Encoder_Axis13))
				get_MACH_value(&Encoder_Axis13);
			if (pre_get_MACH_counter(&Encoder_Axis14))
				get_MACH_value(&Encoder_Axis14);
			if (pre_get_MACH_counter(&Encoder_Axis15))
				get_MACH_value(&Encoder_Axis15);
			if (pre_get_MACH_counter(&Encoder_Axis16))
				get_MACH_value(&Encoder_Axis16);
			if (pre_get_MACH_counter(&Encoder_Axis17))
				get_MACH_value(&Encoder_Axis17);
			if (pre_get_MACH_counter(&Encoder_Axis18))
				get_MACH_value(&Encoder_Axis18);
			if (pre_get_MACH_counter(&Encoder_Axis19))
				get_MACH_value(&Encoder_Axis19);
			if(pre_get_MACH_counter(&Encoder_Axis20))
				get_MACH_value(&Encoder_Axis20);	
			if(pre_get_MACH_counter(&Encoder_Axis21))
				get_MACH_value(&Encoder_Axis21);
			if(pre_get_MACH_counter(&Encoder_Axis22))
				get_MACH_value(&Encoder_Axis22);	
			if (pre_get_MACH_counter(&Encoder_Axis23))
				get_MACH_value(&Encoder_Axis23);

		} while (OverallSign.GetMachCoordinateRight == 0);
		get_ABS_value();
		NowValue2.Axis1 = OverallSign.ABS_Coordinate.Axis1;
		NowValue2.Axis2 = OverallSign.ABS_Coordinate.Axis2;
		NowValue2.Axis3 = OverallSign.ABS_Coordinate.Axis3;
		NowValue2.Axis4 = OverallSign.ABS_Coordinate.Axis4;
		NowValue2.Axis5 = OverallSign.ABS_Coordinate.Axis5;
		NowValue2.Axis6 = OverallSign.ABS_Coordinate.Axis6;
		NowValue2.Axis7 = OverallSign.ABS_Coordinate.Axis7;
		NowValue2.Axis8 = OverallSign.ABS_Coordinate.Axis8;
		NowValue2.Axis9 = OverallSign.ABS_Coordinate.Axis9;
		NowValue2.Axis10 = OverallSign.ABS_Coordinate.Axis10;
		NowValue2.Axis11 = OverallSign.ABS_Coordinate.Axis11;
		NowValue2.Axis12 = OverallSign.ABS_Coordinate.Axis12;
		NowValue2.Axis13 = OverallSign.ABS_Coordinate.Axis13;
		NowValue2.Axis14 = OverallSign.ABS_Coordinate.Axis14;
		NowValue2.Axis15 = OverallSign.ABS_Coordinate.Axis15;
		NowValue2.Axis16 = OverallSign.ABS_Coordinate.Axis16;
		NowValue2.Axis17 = OverallSign.ABS_Coordinate.Axis17;
		NowValue2.Axis18 = OverallSign.ABS_Coordinate.Axis18;
		NowValue2.Axis19 = OverallSign.ABS_Coordinate.Axis19;
		NowValue2.Axis20 = OverallSign.ABS_Coordinate.Axis20;
		NowValue2.Axis21 = OverallSign.ABS_Coordinate.Axis21;
		NowValue2.Axis22 = OverallSign.ABS_Coordinate.Axis22;
		NowValue2.Axis23 = OverallSign.ABS_Coordinate.Axis23;

		do {
			OverallSign.GetMachCoordinateRight = 1;
			if (pre_get_MACH_counter(&Encoder_Axis1))
				get_MACH_value(&Encoder_Axis1);

			#if 0
			if (pre_get_MACH_counter(&Encoder_Axis2))
				get_MACH_value(&Encoder_Axis2);
			#else
				Encoder_Axis2.MACH_PositionValue = NC_AREAControl.OldValue.Axis2;
			#endif
			
			if (pre_get_MACH_counter(&Encoder_Axis3))
				get_MACH_value(&Encoder_Axis3);
			if (pre_get_MACH_counter(&Encoder_Axis4))
				get_MACH_value(&Encoder_Axis4);
			if (pre_get_MACH_counter(&Encoder_Axis5))
				get_MACH_value(&Encoder_Axis5);
			if (pre_get_MACH_counter(&Encoder_Axis6))
				get_MACH_value(&Encoder_Axis6);
			if (pre_get_MACH_counter(&Encoder_Axis7))
				get_MACH_value(&Encoder_Axis7);
			if (pre_get_MACH_counter(&Encoder_Axis8))
				get_MACH_value(&Encoder_Axis8);			
			if (pre_get_MACH_counter(&Encoder_Axis9))
				get_MACH_value(&Encoder_Axis9);

			#if 0
			if (pre_get_MACH_counter(&Encoder_Axis10))
				get_MACH_value(&Encoder_Axis10);
			#else
				Encoder_Axis10.MACH_PositionValue = NC_AREAControl.OldValue.Axis10;
			#endif
			
			if (pre_get_MACH_counter(&Encoder_Axis11))
				get_MACH_value(&Encoder_Axis11);
			if (pre_get_MACH_counter(&Encoder_Axis12))
				get_MACH_value(&Encoder_Axis12);
			if (pre_get_MACH_counter(&Encoder_Axis13))
				get_MACH_value(&Encoder_Axis13);
			if (pre_get_MACH_counter(&Encoder_Axis14))
				get_MACH_value(&Encoder_Axis14);
			if (pre_get_MACH_counter(&Encoder_Axis15))
				get_MACH_value(&Encoder_Axis15);
			if (pre_get_MACH_counter(&Encoder_Axis16))
				get_MACH_value(&Encoder_Axis16);
			if (pre_get_MACH_counter(&Encoder_Axis17))
				get_MACH_value(&Encoder_Axis17);
			if (pre_get_MACH_counter(&Encoder_Axis18))
				get_MACH_value(&Encoder_Axis18);
			if (pre_get_MACH_counter(&Encoder_Axis19))
				get_MACH_value(&Encoder_Axis19);
			if(pre_get_MACH_counter(&Encoder_Axis20))
				get_MACH_value(&Encoder_Axis20);	
			if(pre_get_MACH_counter(&Encoder_Axis21))
				get_MACH_value(&Encoder_Axis21);
			if(pre_get_MACH_counter(&Encoder_Axis22))
				get_MACH_value(&Encoder_Axis22);		
			if (pre_get_MACH_counter(&Encoder_Axis23))
				get_MACH_value(&Encoder_Axis23);

		} while (OverallSign.GetMachCoordinateRight == 0);

		get_ABS_value();

		NowValue3.Axis1 = OverallSign.ABS_Coordinate.Axis1;
		NowValue3.Axis2 = OverallSign.ABS_Coordinate.Axis2;
		NowValue3.Axis3 = OverallSign.ABS_Coordinate.Axis3;
		NowValue3.Axis4 = OverallSign.ABS_Coordinate.Axis4;
		NowValue3.Axis5 = OverallSign.ABS_Coordinate.Axis5;
		NowValue3.Axis6 = OverallSign.ABS_Coordinate.Axis6;
		NowValue3.Axis7 = OverallSign.ABS_Coordinate.Axis7;
		NowValue3.Axis8 = OverallSign.ABS_Coordinate.Axis8;
		NowValue3.Axis9 = OverallSign.ABS_Coordinate.Axis9;
		NowValue3.Axis10 = OverallSign.ABS_Coordinate.Axis10;
		NowValue3.Axis11 = OverallSign.ABS_Coordinate.Axis11;
		NowValue3.Axis12 = OverallSign.ABS_Coordinate.Axis12;
		NowValue3.Axis13 = OverallSign.ABS_Coordinate.Axis13;
		NowValue3.Axis14 = OverallSign.ABS_Coordinate.Axis14;
		NowValue3.Axis15 = OverallSign.ABS_Coordinate.Axis15;
		NowValue3.Axis16 = OverallSign.ABS_Coordinate.Axis16;
		NowValue3.Axis17 = OverallSign.ABS_Coordinate.Axis17;
		NowValue3.Axis18 = OverallSign.ABS_Coordinate.Axis18;
		NowValue3.Axis19 = OverallSign.ABS_Coordinate.Axis19;
		NowValue3.Axis20 = OverallSign.ABS_Coordinate.Axis20;
		NowValue3.Axis21 = OverallSign.ABS_Coordinate.Axis21;
		NowValue3.Axis22 = OverallSign.ABS_Coordinate.Axis22;
		NowValue3.Axis23 = OverallSign.ABS_Coordinate.Axis23;

		if ((labs(NowValue1.Axis1 - NowValue2.Axis1) > 5)
				|| (labs(NowValue1.Axis1 - NowValue3.Axis1) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis2 - NowValue2.Axis2) > 5)
				|| (labs(NowValue1.Axis2 - NowValue3.Axis2) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis3 - NowValue2.Axis3) > 5)
				|| (labs(NowValue1.Axis3 - NowValue3.Axis3) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}
		if ((labs(NowValue1.Axis4 - NowValue2.Axis4) > 5)
				|| (labs(NowValue1.Axis4 - NowValue3.Axis4) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis5 - NowValue2.Axis5) > 5)
				|| (labs(NowValue1.Axis5 - NowValue3.Axis5) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis6 - NowValue2.Axis6) > 5)
				|| (labs(NowValue1.Axis6 - NowValue3.Axis6) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis7 - NowValue2.Axis7) > 5)
				|| (labs(NowValue1.Axis7 - NowValue3.Axis7) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis8 - NowValue2.Axis8) > 5)
				|| (labs(NowValue1.Axis8 - NowValue3.Axis8) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis9 - NowValue2.Axis9) > 5)
				|| (labs(NowValue1.Axis9 - NowValue3.Axis9) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis10 - NowValue2.Axis10) > 5)
				|| (labs(NowValue1.Axis10 - NowValue3.Axis10) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis11 - NowValue2.Axis11) > 5)
				|| (labs(NowValue1.Axis11 - NowValue3.Axis11) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis12 - NowValue2.Axis12) > 5)
				|| (labs(NowValue1.Axis12 - NowValue3.Axis12) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis13 - NowValue2.Axis13) > 5)
				|| (labs(NowValue1.Axis13 - NowValue3.Axis13) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis14 - NowValue2.Axis14) > 5)
				|| (labs(NowValue1.Axis14 - NowValue3.Axis14) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis15 - NowValue2.Axis15) > 5)
				|| (labs(NowValue1.Axis15 - NowValue3.Axis15) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis16 - NowValue2.Axis16) > 5)
				|| (labs(NowValue1.Axis16 - NowValue3.Axis16) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis17 - NowValue2.Axis17) > 5)
				|| (labs(NowValue1.Axis17 - NowValue3.Axis17) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis18 - NowValue2.Axis18) > 5)
				|| (labs(NowValue1.Axis18 - NowValue3.Axis18) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis19 - NowValue2.Axis19) > 5)
				|| (labs(NowValue1.Axis19 - NowValue3.Axis19) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis20 - NowValue2.Axis20) > 5)
				|| (labs(NowValue1.Axis20 - NowValue3.Axis20) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis21 - NowValue2.Axis21) > 5)
				|| (labs(NowValue1.Axis21 - NowValue3.Axis21) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis22 - NowValue2.Axis22) > 5)
				|| (labs(NowValue1.Axis22 - NowValue3.Axis22) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		if ((labs(NowValue1.Axis23 - NowValue2.Axis23) > 5)
				|| (labs(NowValue1.Axis23 - NowValue3.Axis23) > 5)) {
			STDNCSign.GetPositionSign = 1;
			SPCNCSign.GetPositionSign = 1;
			NCRunSign.GetPositionSign = 1;
			return 0;
		}

		//获得输出位置机床位置坐标  10nm or 10nd
		NC_AREAControl.NowValue.Axis1 = NowValue3.Axis1;
		NC_AREAControl.NowValue.Axis2 = NowValue3.Axis2;
		NC_AREAControl.NowValue.Axis3 = NowValue3.Axis3;
		NC_AREAControl.NowValue.Axis4 = NowValue3.Axis4;
		NC_AREAControl.NowValue.Axis5 = NowValue3.Axis5;
		NC_AREAControl.NowValue.Axis6 = NowValue3.Axis6;
		NC_AREAControl.NowValue.Axis7 = NowValue3.Axis7;
		NC_AREAControl.NowValue.Axis8 = NowValue3.Axis8;
		NC_AREAControl.NowValue.Axis9 = NowValue3.Axis9;
		NC_AREAControl.NowValue.Axis10 = NowValue3.Axis10;
		NC_AREAControl.NowValue.Axis11 = NowValue3.Axis11;
		NC_AREAControl.NowValue.Axis12 = NowValue3.Axis12;
		NC_AREAControl.NowValue.Axis13 = NowValue3.Axis13;
		NC_AREAControl.NowValue.Axis14 = NowValue3.Axis14;
		NC_AREAControl.NowValue.Axis15 = NowValue3.Axis15;
		NC_AREAControl.NowValue.Axis16 = NowValue3.Axis16;
		NC_AREAControl.NowValue.Axis17 = NowValue3.Axis17;
		NC_AREAControl.NowValue.Axis18 = NowValue3.Axis18;
		NC_AREAControl.NowValue.Axis19 = NowValue3.Axis19;
		NC_AREAControl.NowValue.Axis20 = NowValue3.Axis20;
		NC_AREAControl.NowValue.Axis21 = NowValue3.Axis21;
		NC_AREAControl.NowValue.Axis22 = NowValue3.Axis22;
		NC_AREAControl.NowValue.Axis23 = NowValue3.Axis23;

		NC_AREAControl.OldValue.Axis1 = NC_AREAControl.NowValue.Axis1;
		NC_AREAControl.OldValue.Axis2 = NC_AREAControl.NowValue.Axis2;
		NC_AREAControl.OldValue.Axis3 = NC_AREAControl.NowValue.Axis3;
		NC_AREAControl.OldValue.Axis4 = NC_AREAControl.NowValue.Axis4;
		NC_AREAControl.OldValue.Axis5 = NC_AREAControl.NowValue.Axis5;
		NC_AREAControl.OldValue.Axis6 = NC_AREAControl.NowValue.Axis6;
		NC_AREAControl.OldValue.Axis7 = NC_AREAControl.NowValue.Axis7;
		NC_AREAControl.OldValue.Axis8 = NC_AREAControl.NowValue.Axis8;
		NC_AREAControl.OldValue.Axis9 = NC_AREAControl.NowValue.Axis9;
		NC_AREAControl.OldValue.Axis10 = NC_AREAControl.NowValue.Axis10;
		NC_AREAControl.OldValue.Axis11 = NC_AREAControl.NowValue.Axis11;
		NC_AREAControl.OldValue.Axis12 = NC_AREAControl.NowValue.Axis12;
		NC_AREAControl.OldValue.Axis13 = NC_AREAControl.NowValue.Axis13;
		NC_AREAControl.OldValue.Axis14 = NC_AREAControl.NowValue.Axis14;
		NC_AREAControl.OldValue.Axis15 = NC_AREAControl.NowValue.Axis15;
		NC_AREAControl.OldValue.Axis16 = NC_AREAControl.NowValue.Axis16;
		NC_AREAControl.OldValue.Axis17 = NC_AREAControl.NowValue.Axis17;
		NC_AREAControl.OldValue.Axis18 = NC_AREAControl.NowValue.Axis18;
		NC_AREAControl.OldValue.Axis19 = NC_AREAControl.NowValue.Axis19;
		NC_AREAControl.OldValue.Axis20 = NC_AREAControl.NowValue.Axis20;
		NC_AREAControl.OldValue.Axis21 = NC_AREAControl.NowValue.Axis21;
		NC_AREAControl.OldValue.Axis22 = NC_AREAControl.NowValue.Axis22;
		NC_AREAControl.OldValue.Axis23 = NC_AREAControl.NowValue.Axis23;

		//NowPointMachPulseOut = ABSCoordinate_to_MachineCoordinate(NowValue3);
		NowPointMachPulseOut = NowValue3;
		
		//for test compensation
		NowPointMachPulseOut = nonlinear_pulse_out(NowValue3);

		NowPointMachPulseOut.Axis1 *= Servo_Axis1.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis2 *= Servo_Axis2.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis3 *= Servo_Axis3.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis4 *= Servo_Axis4.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis5 *= Servo_Axis5.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis6 *= Servo_Axis6.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis7 *= Servo_Axis7.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis8 *= Servo_Axis8.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis9 *= Servo_Axis9.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis10 *= Servo_Axis10.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis11 *= Servo_Axis11.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis12 *= Servo_Axis12.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis13 *= Servo_Axis13.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis14 *= Servo_Axis14.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis15 *= Servo_Axis15.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis16 *= Servo_Axis16.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis17 *= Servo_Axis17.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis18 *= Servo_Axis18.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis19 *= Servo_Axis19.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis20 *= Servo_Axis20.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis21 *= Servo_Axis21.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis22 *= Servo_Axis22.AxisOutUnitEQU; //for test
		NowPointMachPulseOut.Axis23 *= Servo_Axis23.AxisOutUnitEQU; //for test

		NC_AREAControl.OldPointMACHPulseOut.Axis1 = NowPointMachPulseOut.Axis1;
		NC_AREAControl.OldPointMACHPulseOut.Axis2 = NowPointMachPulseOut.Axis2;
		NC_AREAControl.OldPointMACHPulseOut.Axis3 = NowPointMachPulseOut.Axis3;
		NC_AREAControl.OldPointMACHPulseOut.Axis4 = NowPointMachPulseOut.Axis4;
		NC_AREAControl.OldPointMACHPulseOut.Axis5 = NowPointMachPulseOut.Axis5;
		NC_AREAControl.OldPointMACHPulseOut.Axis6 = NowPointMachPulseOut.Axis6;
		NC_AREAControl.OldPointMACHPulseOut.Axis7 = NowPointMachPulseOut.Axis7;
		NC_AREAControl.OldPointMACHPulseOut.Axis8 = NowPointMachPulseOut.Axis8;
		NC_AREAControl.OldPointMACHPulseOut.Axis9 = NowPointMachPulseOut.Axis9;
		NC_AREAControl.OldPointMACHPulseOut.Axis10 =
				NowPointMachPulseOut.Axis10;
		NC_AREAControl.OldPointMACHPulseOut.Axis11 =
				NowPointMachPulseOut.Axis11;
		NC_AREAControl.OldPointMACHPulseOut.Axis12 =
				NowPointMachPulseOut.Axis12;
		NC_AREAControl.OldPointMACHPulseOut.Axis13 =
				NowPointMachPulseOut.Axis13;
		NC_AREAControl.OldPointMACHPulseOut.Axis14 =
				NowPointMachPulseOut.Axis14;
		NC_AREAControl.OldPointMACHPulseOut.Axis15 =
				NowPointMachPulseOut.Axis15;
		NC_AREAControl.OldPointMACHPulseOut.Axis16 =
				NowPointMachPulseOut.Axis16;
		NC_AREAControl.OldPointMACHPulseOut.Axis17 =
				NowPointMachPulseOut.Axis17;
		NC_AREAControl.OldPointMACHPulseOut.Axis18 =
				NowPointMachPulseOut.Axis18;
		NC_AREAControl.OldPointMACHPulseOut.Axis19 =
				NowPointMachPulseOut.Axis19;
		NC_AREAControl.OldPointMACHPulseOut.Axis20 =
				NowPointMachPulseOut.Axis20;
		NC_AREAControl.OldPointMACHPulseOut.Axis21 =
				NowPointMachPulseOut.Axis21;
		NC_AREAControl.OldPointMACHPulseOut.Axis22 =
				NowPointMachPulseOut.Axis22;
		NC_AREAControl.OldPointMACHPulseOut.Axis23 =
				NowPointMachPulseOut.Axis23;

	}

	return 1;
}

void STD_run_G100(void) {
	Uint32 ri;
	GCODE *GCodeRIPointer;
	
	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];
	//add code for function
	
	STDNCSign.CodeRun_Over = 1;

}

void STD_run_G101(void) 
{	//LEDflash 控制
	Uint32 ri;
	GCODE *GCodeRIPointer;
	
	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	Write_LEDFlash_OUT(0x03, GCodeRIPointer->SubCMD1, GCodeRIPointer->SubCMD2);

	STDNCSign.CodeRun_Over = 1;

}


void STD_run_G102(void) 
{	//检测输入信号
	Uint32 ri;
	GCODE *GCodeRIPointer;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	if ((GCodeRIPointer->SubCMD1 == 0) && (GCodeRIPointer->SubCMD2 == 0)) 
	{
		STDNCSign.CodeRun_Over = 1;
	}

	CPLD_GPIO_INRead();

	if ((((GCodeRIPointer->SubCMD1 & 0xFFFF) ==0)?(1):((0xFFFF-GPIO_In1_Register.all) & (GCodeRIPointer->SubCMD1 & 0xFFFF)))&& 
		((((GCodeRIPointer->SubCMD1 & 0xFFFF0000)) ==0)?(1):((0xFFFF-GPIO_In2_Register.all) & ((GCodeRIPointer->SubCMD1 & 0xFFFF0000) >> 16)))&&
		(((GCodeRIPointer->SubCMD2 & 0xFFFF) ==0)?(1):((0xFFFF-GPIO_In3_Register.all) & (GCodeRIPointer->SubCMD2 & 0xFFFF)))&&
		((((GCodeRIPointer->SubCMD2 & 0xFFFF0000)) ==0)?(1):((0xFFFF-GPIO_In4_Register.all) & ((GCodeRIPointer->SubCMD2 & 0xFFFF0000) >> 16))))
	{
		STDNCSign.CodeRun_Over = 1;
	}	
}

void STD_run_G103(void) 
{	//输出口控制
	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 GPIO_OUT_CMD,GPIO_OUT_CMD_2;
	static Uint16 GPIO_OUT_CLOSE_All = 0xFFFF;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	GPIO_OUT_CMD = GCodeRIPointer->SubCMD1;
	GPIO_OUT_CMD_2 = GCodeRIPointer->SubCMD2;
	
	if (GPIO_OUT_CMD & 0x01) 
	{
		GPIO_Out1_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD & 0x02) 
	{
		GPIO_Out1_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x04) 
	{
		GPIO_Out1_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x08) 
	{
		GPIO_Out1_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x010) 
	{
		GPIO_Out1_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x020) 
	{
		GPIO_Out1_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x040) 
	{
		GPIO_Out1_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x080) 
	{
		GPIO_Out1_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0100) 
	{
		GPIO_Out1_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0200) 
	{
		GPIO_Out1_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0400) 
	{
		GPIO_Out1_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out1_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0800) 
	{
		GPIO_Out1_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out1_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x1000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x2000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x4000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x8000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD & 0x010000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x020000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x040000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x080000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0100000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0200000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0400000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x0800000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x01000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x02000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x04000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x08000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x10000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x20000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x40000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD & 0x80000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD == 0) 
	{
		GPIO_Out1_Register.all = GPIO_OUT_CLOSE_All;
		GPIO_Out2_Register.all = GPIO_OUT_CLOSE_All;
	}

	//added 20170227
	if (GPIO_OUT_CMD_2 & 0x01) 
	{
		GPIO_Out3_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD_2 & 0x02) 
	{
		GPIO_Out3_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x04) 
	{
		GPIO_Out3_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x08) 
	{
		GPIO_Out3_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x010) 
	{
		GPIO_Out3_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x020) 
	{
		GPIO_Out3_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x040) 
	{
		GPIO_Out3_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x080) 
	{
		GPIO_Out3_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0100) 
	{
		GPIO_Out3_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0200) 
	{
		GPIO_Out3_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0400) 
	{
		GPIO_Out3_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out3_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0800) 
	{
		GPIO_Out3_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out3_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x1000) 
	{
		GPIO_Out3_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x2000) 
	{
		GPIO_Out3_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x4000) 
	{
		GPIO_Out3_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x8000) 
	{
		GPIO_Out3_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out3_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD_2 & 0x010000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x020000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x040000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x080000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0100000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0200000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0400000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x0800000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x01000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x02000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x04000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x08000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x10000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x20000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x40000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (GPIO_OUT_CMD_2 & 0x80000000) 
	{
		GPIO_Out4_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out4_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	if (GPIO_OUT_CMD == 0) 
	{
		GPIO_Out1_Register.all = GPIO_OUT_CLOSE_All;
		GPIO_Out2_Register.all = GPIO_OUT_CLOSE_All;
	}

	if (GPIO_OUT_CMD_2 == 0) 
	{
		GPIO_Out3_Register.all = GPIO_OUT_CLOSE_All;
		GPIO_Out4_Register.all = GPIO_OUT_CLOSE_All;
	}

	Write_GPIO_Out1();
	Write_GPIO_Out2();
	Write_GPIO_Out3();
	Write_GPIO_Out4();

	STDNCSign.CodeRun_Over = 1;
}


void STD_run_G104(void) 
{	//荧光控制
	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 Fluorescence_CMD;
	static Uint16 Fluorescence_CLOSE_All = 0xFF;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	* COM_OUT_FIFO_Clear = 0;
	* COM_OUT_FIFO_Clear = 0xffff;
	* COM_IN_FIFO_Clear = 0;
	* COM_IN_FIFO_Clear = 0xffff;

	//Fluorescence_CMD = (GCodeRIPointer->SubCMD1) | 0xf0;
	Fluorescence_CMD = GCodeRIPointer->SubCMD1;

	if (Fluorescence_CMD == 0) 
	{
		GPIO_Out1_Register.all = Fluorescence_CLOSE_All;
	}

	*WRITE_RS232_1 = 0x4F;
	*WRITE_RS232_1 = Fluorescence_CMD;
	*WRITE_RS232_1 = 0x50;

	STDNCSign.CodeRun_Over = 1;
}

void STD_run_G105(void) 
{	//更新相机扫描起始坐标
	Uint32 ri;
	GCODE *GCodeRIPointer;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	if(GCodeRIPointer->SubCMD1 == 1)
	{	//CMDMain = 105
		//SubCMD1 = 0 时为空代码
		//SubCMD1 = 1 时更新成像扫描起始坐标
		//SubCMD1 = 2 时更新成像扫描步进值(不同镜头图片大小不同)
		System.PositionCoordinate1.Axis17 = 	GCodeRIPointer->StartPoint.Axis17;
		System.PositionCoordinate1.Axis18 = 	GCodeRIPointer->StartPoint.Axis18;
	}
	else if(GCodeRIPointer->SubCMD1 == 2)
	{	//CMDMain = 105
		//SubCMD1 = 1 时更新起始坐标
		//SubCMD1 = 2 时更新成像扫描步进值(不同镜头图片大小不同)
		System.OffsetCoordinate.Axis17 = 	GCodeRIPointer->StartPoint.Axis17;
		System.OffsetCoordinate.Axis18 = 	GCodeRIPointer->StartPoint.Axis18;
	}

	STDNCSign.CodeRun_Over = 1;
}


void STD_run_G106(void) 
{	//LED 电压控制
	//for athena
	//DA total 2 channel(0~5V)
	//Channel 2 for LED flash light
	//Channel 3 for Bright field light

	//for Light sheet
	//DA1 total 4 channel(0~10V)
	//Channel 1 for LED flash light
	//Channel 2 for Bright field light
	//Channel 3 for AOTF
	//Channel 4 for Pockels Cell
	
	//DA2 total 4 channel(0~10V)
	//Channel (5)1 for Galvanometer 1
	//Channel (6)2 for Galvanometer 2
	//Channel (7)3 for Galvanometer 3
	//Channel (8)4 for Galvanometer 4
	
	//DA3 total 1 channel(0~5V)
	//Channel (9)1 for PMT measure

	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 CMD1_DA_CH,CMD2_VOLTAGE_OUT;
	Uint32 VOLTAGE_OUT;
	Uint8 VOLTAGE_OUT_HIGH,VOLTAGE_OUT_LOW;
	Uint16 VOLTAGE_DA_Value;
	
	FLOAT64 RiseTime;
	FLOAT64 FallTime;
	FLOAT64 HKeepTime;
	FLOAT64 LKeepTime;
	
	FLOAT64 HighIncrument;
	FLOAT64 LowIncrument;
	FLOAT64 HighDividedNum;
	FLOAT64 LowDividedNum;

	Uint16 V_Amplitude;

	static float64 DAConvertTime = 1.8;//180us=0.18ms  uint=0.18ms

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	* COM_OUT_FIFO_Clear = 0;
	* COM_OUT_FIFO_Clear = 0xffff;
	* COM_IN_FIFO_Clear = 0;
	* COM_IN_FIFO_Clear = 0xffff;

	CMD1_DA_CH = GCodeRIPointer->SubCMD1;
	CMD2_VOLTAGE_OUT = GCodeRIPointer->SubCMD2;
	if(CMD2_VOLTAGE_OUT > 500)
	{
		CMD2_VOLTAGE_OUT = 500;
	}

	//DA value set
	//for Athena's Alpha
	//Numer 1:Old SCH and BOard version1:ADC Module for AD convert
	//(1000-set)/(10/2048) 			//note:1000=10.0V
	//(10-3.25)/(10/2048)=6.75/0.0048828125=1382.4=05 66
	VOLTAGE_OUT = (1000 - CMD2_VOLTAGE_OUT)*2048/1000;		
	VOLTAGE_OUT_LOW = (VOLTAGE_OUT & 0xff);
	VOLTAGE_OUT_HIGH = ((VOLTAGE_OUT>>8) & 0xff);

	//for Athena's Beta
	//Numer 1:New SCH and BOard version2:Board FPGA control for AD convert
	VOLTAGE_DA_Value = divdp(CMD2_VOLTAGE_OUT,500.0)*1023;

	//for Light sheet project
	DA_AD5315_G106.AutoSign = GCodeRIPointer->StartPoint.Axis1;
	if(CMD1_DA_CH != 5)
	{
		if(DA_AD5315_G106.AutoSign != 0)
		{
			DA_AD5315_G106.AutoSign = 1;
		}
	}
	if(DA_AD5315_G106.AutoSign != False)
	{
		DA_AD5315_G106.Voltage_Max = GCodeRIPointer->StartPoint.Axis2;
		DA_AD5315_G106.Voltage_Start = GCodeRIPointer->StartPoint.Axis3;
		DA_AD5315_G106.CycleNum = GCodeRIPointer->StartPoint.Axis4;
		//DA_AD5315_G106.HIncrument = GCodeRIPointer->StartPoint.Axis5;
		DA_AD5315_G106.PWMHighCount = GCodeRIPointer->StartPoint.Axis6;
		DA_AD5315_G106.PWMLowCount = GCodeRIPointer->StartPoint.Axis7;
		DA_AD5315_G106.PWMPhase = GCodeRIPointer->StartPoint.Axis8;

		RiseTime = (GCodeRIPointer->StartPoint.Axis9 < 10) ? (10):(GCodeRIPointer->StartPoint.Axis9);
		FallTime= (GCodeRIPointer->StartPoint.Axis10 < 10) ? (10):(GCodeRIPointer->StartPoint.Axis10);
		HKeepTime = (GCodeRIPointer->StartPoint.Axis11 < 10) ? (10):(GCodeRIPointer->StartPoint.Axis11);
		LKeepTime = (GCodeRIPointer->StartPoint.Axis12 < 10) ? (10):(GCodeRIPointer->StartPoint.Axis12);

		//HighIncrument 	= (GCodeRIPointer->StartPoint.Axis13 < 1) ? (1):(GCodeRIPointer->StartPoint.Axis13);
		//LowIncrument		= (GCodeRIPointer->StartPoint.Axis14 < 1) ? (1):(GCodeRIPointer->StartPoint.Axis14);
		//HighDividedNum 	= (GCodeRIPointer->StartPoint.Axis15 < 1) ? (1):(GCodeRIPointer->StartPoint.Axis15);
		//LowDividedNum  	= (GCodeRIPointer->StartPoint.Axis16 < 1) ? (1):(GCodeRIPointer->StartPoint.Axis16);

		DA_AD5315_G106.RiseCount = divdp(RiseTime,DAConvertTime);
		DA_AD5315_G106.FallCount = divdp(FallTime,DAConvertTime);
		DA_AD5315_G106.HKeepCount = divdp(HKeepTime,DAConvertTime);
		DA_AD5315_G106.LKeepCount =  divdp(LKeepTime,DAConvertTime);
		
		V_Amplitude = DA_AD5315_G106.Voltage_Max - DA_AD5315_G106.Voltage_Start;
		
		HighIncrument 	= (V_Amplitude > DA_AD5315_G106.RiseCount) ? (divdp(V_Amplitude,DA_AD5315_G106.RiseCount)):(1);
		HighDividedNum 	= (HighIncrument > 1.0) ? (0):(divdp(DA_AD5315_G106.RiseCount,V_Amplitude));		
		LowIncrument 	= (V_Amplitude > DA_AD5315_G106.FallCount) ? (divdp(V_Amplitude,DA_AD5315_G106.FallCount)):(1);
		LowDividedNum 	= (LowIncrument > 1.0) ? (0):(divdp(DA_AD5315_G106.FallCount,V_Amplitude));		
		
		DA_AD5315_G106.HIncrument = (Uint16)HighIncrument;	
		DA_AD5315_G106.LIncrument = (Uint16)LowIncrument;
		DA_AD5315_G106.HDividedNum= (Uint16)HighDividedNum;	
		DA_AD5315_G106.LDividedNum = (Uint16)LowDividedNum;

		//DA_AD5315_G106.RiseCount = (DA_AD5315_G106.HDividedNum > 1) ? (DA_AD5315_G106.HDividedNum*V_Amplitude):(DA_AD5315_G106.RiseCount);
		//DA_AD5315_G106.FallCount  = (DA_AD5315_G106.LDividedNum > 1) ? (DA_AD5315_G106.LDividedNum*V_Amplitude):(DA_AD5315_G106.FallCount);
	}
	else
	{
		DA_AD5315_G106.Voltage_Max = 0;
		DA_AD5315_G106.Voltage_Start = 0;
		DA_AD5315_G106.CycleNum = 0;
		
		DA_AD5315_G106.PWMHighCount = 0;
		DA_AD5315_G106.PWMLowCount = 0;
		DA_AD5315_G106.PWMPhase = 0;

		DA_AD5315_G106.RiseCount = 0;
		DA_AD5315_G106.FallCount = 0;
		DA_AD5315_G106.HKeepCount = 0;
		DA_AD5315_G106.LKeepCount = 0;
		DA_AD5315_G106.HIncrument = 0;
		DA_AD5315_G106.LIncrument = 0;
		DA_AD5315_G106.HDividedNum= 0;
		DA_AD5315_G106.LDividedNum = 0;
	}

	switch (CMD1_DA_CH) 
	{
		case DA_CH1:
			//RS232_2...//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)	
			*WRITE_RS232_2 = VOLTAGE_OUT_HIGH;
			*WRITE_RS232_2 = VOLTAGE_OUT_LOW;
			//DA
			Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x01,0);
			break;
			
		case DA_CH2:
			//RS232_2...//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)		
			*WRITE_RS232_2 = VOLTAGE_OUT_HIGH;
			*WRITE_RS232_2 = VOLTAGE_OUT_LOW;
			//DA
			Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x02,0);
			break;
			
		case DA_CH3:
			//RS232_3
			*WRITE_RS232_3 = VOLTAGE_OUT_HIGH;
			*WRITE_RS232_3 = VOLTAGE_OUT_LOW;
			//DA
			Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x04,0);	
			break;
			
		case DA_CH4:
			//RS232_4
			*WRITE_RS232_4 = VOLTAGE_OUT_HIGH;
			*WRITE_RS232_4 = VOLTAGE_OUT_LOW;
			//DA
			Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x08,0);
			break;
		
		case DA_CH5:
			DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
			DA_AD5315_G106.DA_ADDR = 0x01;
			DA_AD5315_G106.DA_CS = 0;

			Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
			break;
		
		case DA_CH6:
			DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
			DA_AD5315_G106.DA_ADDR = 0x02;
			DA_AD5315_G106.DA_CS = 0;
			Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
			break;
		
		case DA_CH7:
			DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
			DA_AD5315_G106.DA_ADDR = 0x04;
			DA_AD5315_G106.DA_CS = 0;
			Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
			break;
		
		case DA_CH8:
			DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
			DA_AD5315_G106.DA_ADDR = 0x08;
			DA_AD5315_G106.DA_CS = 0;
			Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
			break;
		
		case DA_CH9:
			Write_DA_CONVERT_CONTROL_3(VOLTAGE_DA_Value,0x01,0);
			break;

		default:
			break;
	}

	#if 0
	if(CMD1_DA_CH == 1)
	{	//RS232_2...//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)	
		*WRITE_RS232_2 = VOLTAGE_OUT_HIGH;
		*WRITE_RS232_2 = VOLTAGE_OUT_LOW;
		//DA
		Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x01,0);
	}
	else if(CMD1_DA_CH == 2)
	{	//RS232_2...//Alpha machine Com232+DA10V output(Little Board) for flash light power supply(initial 2.3V)		
		*WRITE_RS232_2 = VOLTAGE_OUT_HIGH;
		*WRITE_RS232_2 = VOLTAGE_OUT_LOW;
		//DA
		Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x02,0);
	}
	else if(CMD1_DA_CH == 3)
	{	//RS232_3
		*WRITE_RS232_3 = VOLTAGE_OUT_HIGH;
		*WRITE_RS232_3 = VOLTAGE_OUT_LOW;
		//DA
		Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x04,0);
	}
	else if(CMD1_DA_CH == 4)
	{	//RS232_4
		*WRITE_RS232_4 = VOLTAGE_OUT_HIGH;
		*WRITE_RS232_4 = VOLTAGE_OUT_LOW;
		//DA
		Write_DA_CONVERT_CONTROL(VOLTAGE_DA_Value,0x08,0);
	}
	else if(CMD1_DA_CH == 5)
	{	
		DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
		DA_AD5315_G106.DA_ADDR = 0x01;
		DA_AD5315_G106.DA_CS = 0;
			
		Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
	}	
	else if(CMD1_DA_CH == 6)
	{	
		DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
		DA_AD5315_G106.DA_ADDR = 0x02;
		DA_AD5315_G106.DA_CS = 0;
		Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
	}
	else if(CMD1_DA_CH == 7)
	{
		DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
		DA_AD5315_G106.DA_ADDR = 0x04;
		DA_AD5315_G106.DA_CS = 0;
		Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
	}
	else if(CMD1_DA_CH == 8)
	{	
		DA_AD5315_G106.DA_Data = VOLTAGE_DA_Value;
		DA_AD5315_G106.DA_ADDR = 0x08;
		DA_AD5315_G106.DA_CS = 0;
		Write_DA_CONVERT_CONTROL_2(&DA_AD5315_G106);
	}
	else if(CMD1_DA_CH == 9)
	{	
		Write_DA_CONVERT_CONTROL_3(VOLTAGE_DA_Value,0x01,0);
	}
	#endif

	STDNCSign.CodeRun_Over = 1;
}

void STD_run_G107(void) 
{	//更新闪光灯时间
	Uint32 ri;
	GCODE *GCodeRIPointer;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	System.LEDFlashTime1 = GCodeRIPointer->SubCMD1;
	System.LEDFlashTime2 = GCodeRIPointer->SubCMD2;

	STDNCSign.CodeRun_Over = 1;

}

/***************************************************************************/
/*  Function name: STD_run_G108()                                   */
/*  Argument:NO     	                                    */
/*  Return value:no/
/*  Function: STD_run_G108                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G108 code   (load /uload tips)                                           */
/*	DSP Run G108 code 		*/
/***************************************************************************/
void STD_run_G108(void) 
{	//装载、卸载 tip 头
	//CMD1 bit0= 1 0x01	 =>load channel 1 tip;0 uload tips
	//CMD1 bit1= 1 0x02	 =>load channel 2 tip;0 uload tips
	//CMD1 bit2= 1 0x04	 =>load channel 3 tip;0 uload tips
	//CMD1 bit3= 1 0x08	 =>load channel 4 tip;0 uload tips	
	//if(CMD1 == 0x0f),then load channel 1~4 tips. 
	
	//reserved for total 8 channel
	//CMD1 bit4= 1 0x10	 =>load channel 5 tip;0 uload tips
	//CMD1 bit5= 1 0x20	 =>load channel 6 tip;0 uload tips	
	//CMD1 bit6= 1 0x40	 =>load channel 7 tip;0 uload tips
	//CMD1 bit7= 1 0x80	 =>load channel 8 tip;0 uload tips
	
	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;
	
	if (SubCMD1 & 0x01) 
	{
		GPIO_Out1_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}

	if (SubCMD1 & 0x02) 
	{
		GPIO_Out1_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x04) 
	{
		GPIO_Out1_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x08) 
	{
		GPIO_Out1_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x010) 
	{
		GPIO_Out1_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x020) 
	{
		GPIO_Out1_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x040) 
	{
		GPIO_Out1_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x080) 
	{
		GPIO_Out1_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	
	if (SubCMD1 & 0x0100) 
	{
		GPIO_Out1_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0200) 
	{
		GPIO_Out1_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0400) 
	{
		GPIO_Out1_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out1_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0800) 
	{
		GPIO_Out1_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else
	{
		GPIO_Out1_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x1000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x2000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x4000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x8000) 
	{
		GPIO_Out1_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out1_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	if (SubCMD1 & 0x010000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT1 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT1 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x020000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT2 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT2 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x040000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT3 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT3 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x080000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT4 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT4 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0100000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT5 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT5 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0200000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT6 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT6 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0400000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT7 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT7 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x0800000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT8 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT8 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x01000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT9 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT9 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x02000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT10 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT10 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x04000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT11 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT11 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x08000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT12 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT12 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x10000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT13 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT13 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x20000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT14 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT14 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x40000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT15 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT15 = GPIO_OUT_CLOSE;
	}
	if (SubCMD1 & 0x80000000) 
	{
		GPIO_Out2_Register.bit.GPIOOUT16 = GPIO_OUT_OPEN;
	} 
	else 
	{
		GPIO_Out2_Register.bit.GPIOOUT16 = GPIO_OUT_CLOSE;
	}

	Write_GPIO_Out1();
	Write_GPIO_Out2();
	//Write_GPIO_Out3();
	//Write_GPIO_Out4();

	STDNCSign.CodeRun_Over = 1;
}

/***************************************************************************/
/*  Function name: STD_run_G109()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G109                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G109 code    (liquid bump liquid displacement )                                         */
/*	DSP Run G109 code 		*/
/***************************************************************************/
void STD_run_G109(void) 
{	//移液泵移液、放液
	//CMD1 bit0= 1 0x01  =>Pumping channel 1 liquid;0 no change
	//CMD1 bit1= 1 0x02  =>Pumping channel 2 liquid;0 no change
	//CMD1 bit2= 1 0x04  =>Pumping channel 3 liquid;0 no change
	//CMD1 bit3= 1 0x08  =>Pumping channel 4 liquid;0 no change	
	//reserved for total 8 channel
	//CMD1 bit4= 1 0x10  =>Pumping channel 5 liquid;0 no change
	//CMD1 bit5= 1 0x20  =>Pumping channel 6 liquid;0 no change	
	//CMD1 bit6= 1 0x40  =>Pumping channel 7 liquid;0 no change
	//CMD1 bit7= 1 0x80  =>Pumping channel 8 liquid;0 no change

	//Feedrate and EndPoint match to every channel

	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;

	if (SubCMD1 & 0x01) 
	{

	}
	if (SubCMD1 & 0x02) 
	{

	}
	if (SubCMD1 & 0x04) 
	{

	}
	if (SubCMD1 & 0x08) 
	{

	}
	
	if (SubCMD1 & 0x010) 
	{

	}
	if (SubCMD1 & 0x020) 
	{

	}
	if (SubCMD1 & 0x040) 
	{

	}
	if (SubCMD1 & 0x080) 
	{

	}

	STDNCSign.CodeRun_Over = 1;

}

/***************************************************************************/
/*  Function name: STD_run_G110()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G110                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G110 code   (Centrifuge control)                                           */
/*	DSP Run G110 code 		*/
/***************************************************************************/
void STD_run_G110(void) 
{	//离心机控制
	//CMD1=1 Centrifuge position mode; CMD2= Centrifuge posiotion angle(unit ud:90000=>90 degree)

	//CMD1=2 Centrifuge rotate mode;CMD2= Centrifuge Rotational speed(unit RPM :2000 RPM);
	//HoldTime = Centrifuge time

	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;
	float64 HoldTime;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;

	if (SubCMD1 & 0x01) 
	{
		//CMD1=1 Centrifuge position mode
		//CMD2= Centrifuge posiotion angle(unit ud:90000=>90 degree)
	}
	else if(SubCMD1 & 0x02)
	{
		//CMD1=2 Centrifuge rotate mode;
		//CMD2= Centrifuge Rotational speed(unit RPM :2000 RPM);
		//HoldTime = Centrifuge time
	}

	STDNCSign.CodeRun_Over = 1;

}

/***************************************************************************/
/*  Function name: STD_run_G111()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G111                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G111 code   (Vibration heating)                                         */
/*	DSP Run G111 code 		*/
/***************************************************************************/
void STD_run_G111(void) 
{	//振动加热模块
	//CMD1 Vibration time;(unit:s   2=2s;120=2min;1800=30min);if(CMD1 == 0),then no Vibration
	//CMD1=2 heating time;(unit:s   2=2s;120=2min;1800=30min);if(CMD2 == 0),then no heating

	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;

	if (SubCMD1 & 0x01) 
	{
		//CMD1 Vibration time;(unit:s	2=2s;120=2min;1800=30min);
		//if(CMD1 == 0),then no Vibration
	}
	else if(SubCMD1 & 0x02)
	{
		//CMD1=2 heating time;(unit:s	2=2s;120=2min;1800=30min);
		//if(CMD2 == 0),then no heating

	}

	
	STDNCSign.CodeRun_Over = 1;
	
}

/***************************************************************************/
/*  Function name: STD_run_G112()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G112                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G112 code   (Electromagnetic pulse oscillation)                                         */
/*	DSP Run G112 code 		*/
/***************************************************************************/
void STD_run_G112(void) 
{	//电磁脉冲振荡
	//CMD1 Electromagnetic pulse oscillation time;(unit:s	2=2s;120=2min;1800=30min);if(CMD1 == 0),then no pulse oscillation
	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;
	
	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;


	STDNCSign.CodeRun_Over = 1;
	
}

/***************************************************************************/
/*  Function name: STD_run_G113()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G113                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G113 code   (AOTF or Pockels control)                                         */
/*	DSP Run G113 code 		*/
/***************************************************************************/
void STD_run_G113(void) 
{	//PWM (AOTF or Pockels) control
	Uint32 ri;
	GCODE *GCodeRIPointer;
	Uint32 SubCMD1,SubCMD2;
	Uint16 aotf_HighCount,aotf_LowCount,aotf_Phase,aotf_Enable;
	Uint16 Pockels_HighCount,Pockels_LowCount,Pockels_Phase,Pockels_Enable;
	Uint16 PWM01_HighCount,PWM01_LowCount,PWM01_Phase,PWM01_Enable;
	Uint16 PWM02_HighCount,PWM02_LowCount,PWM02_Phase,PWM02_Enable;
	
	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	SubCMD1 = GCodeRIPointer->SubCMD1;
	SubCMD2 = GCodeRIPointer->SubCMD2;

	if((SubCMD1&0x01) == 0x01)
	{
		aotf_Enable = 1;
		aotf_HighCount = GCodeRIPointer->StartPoint.Axis1;
		aotf_LowCount = GCodeRIPointer->StartPoint.Axis2;
		aotf_Phase = GCodeRIPointer->StartPoint.Axis3;
		
		Write_AOTF_CONTROL(aotf_HighCount,aotf_LowCount,aotf_Phase,aotf_Enable);	
	}
	else
	{		
		Write_AOTF_CONTROL(0,0,0,0);	
	}

	if((SubCMD1&0x02) == 0x02)
	{
		Pockels_Enable = 1;
		Pockels_HighCount = GCodeRIPointer->StartPoint.Axis1;
		Pockels_LowCount = GCodeRIPointer->StartPoint.Axis2;
		Pockels_Phase = GCodeRIPointer->StartPoint.Axis3;
		
		Write_Pockels_CONTROL(Pockels_HighCount,Pockels_LowCount,Pockels_Phase,Pockels_Enable);	
	}
	else
	{		
		Write_Pockels_CONTROL(0,0,0,0);	
	}

	if((SubCMD1&0x04) == 0x04)
	{
		PWM01_Enable = 1;
		PWM01_HighCount = GCodeRIPointer->StartPoint.Axis1;
		PWM01_LowCount = GCodeRIPointer->StartPoint.Axis2;
		PWM01_Phase = GCodeRIPointer->StartPoint.Axis3;
		
		Write_PWM1_CONTROL(PWM01_HighCount,PWM01_LowCount,PWM01_Phase,PWM01_Enable);	
	}
	else
	{		
		Write_PWM1_CONTROL(0,0,0,0);	
	}

	if((SubCMD1&0x08) == 0x08)
	{
		PWM02_Enable = 1;
		PWM02_HighCount = GCodeRIPointer->StartPoint.Axis1;
		PWM02_LowCount = GCodeRIPointer->StartPoint.Axis2;
		PWM02_Phase = GCodeRIPointer->StartPoint.Axis3;
		
		Write_PWM2_CONTROL(PWM02_HighCount,PWM02_LowCount,PWM02_Phase,PWM02_Enable);	
	}
	else
	{		
		Write_PWM2_CONTROL(0,0,0,0);	
	}

	
	STDNCSign.CodeRun_Over = 1;
	
}

/***************************************************************************/
/*  Function name: STD_run_G114()                                   */
/*  Argument:NO     	                                    */
/*  Return value:true/
/*  Function: STD_run_G114                                           */
/*    Please refer to the following document for more detials.        */                                   
/*								*/
/*  G114 code   (PMT Measure:Measure time=HoldTime(ms))                                         */
/*	DSP Run G114 code 		*/
/***************************************************************************/
void STD_run_G114(void) 
{	//G114 HoldTime 运算单位为ms  
	Uint32 ri;
	GCODE *GCodeRIPointer;
	float64 HoldTime;
	INT16U NowValue_Low;
	INT16U NowValue_High;
	PMT *Pmt_CounterPort1;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	HoldTime = GCodeRIPointer->StartPoint.Axis1;
	if(HoldTime == 0)
	{
		HoldTime = 1;//1ms
	}

	Pmt_CounterPort1 = &Pmt_Counter1;
	NowValue_Low = *Pmt_CounterPort1->PMT_CounterPort_Low;
	NowValue_High = *Pmt_CounterPort1->PMT_CounterPort_High;

	if ((STDNCSign.PrecalCode == 0) && OverallSign.EveryAxisCoinValid) 
	{
		Timer0.G114StartCounter = Timer0.GenericCounter;
		STDNCSign.PrecalCode = 1;
		Pmt_Counter1.NowValue_G114 =(NowValue_Low+NowValue_High * 0x10000);
		Pmt_Counter1.OldValue_G114 = Pmt_Counter1.NowValue_G114;
	}

	if ((OverallSign.InterpolationRunCnt_1 >= OverallSign.InterpolationSendCnt_1)) 
	{
		if (STDNCSign.PrecalCode == 1) 
		{
			if (OverallSign.EveryAxisCoinValid) 
			{
				if (get_delta_time(Timer0.G114StartCounter) >= HoldTime) 
				{									
					Pmt_Counter1.NowValue_G114 =(NowValue_Low+NowValue_High * 0x10000);
					Pmt_Counter1.AverageValue_G114 = divdp((Pmt_Counter1.NowValue_G114 - Pmt_Counter1.OldValue_G114),HoldTime);
					Pmt_Counter1.OldValue_G114 = Pmt_Counter1.NowValue_G114;					
					STDNCSign.CodeRun_Over = 1;
				}
			}
		}
	}
}


void STD_run_G04(void) 
{	//G04 运算单位为ms
	Uint32 ri;
	GCODE *GCodeRIPointer;
	static float64 SwitchTime1 = 10.0;
	static float64 SwitchTime2 = 12.0;
	static float64 SwitchTime3 = 180.0;
	float64 HoldTime;

	ri = (STDNCSign.RealRunCount - 1) % STDGCODE_MOD;
	GCodeRIPointer = &GCodeBuffer[ri];

	//CH0509
	if (GCodeRIPointer->StartPoint.Axis1 == 0) 
	{
		STDNCSign.CodeRun_Over = 1;
		Timer0.G04StartCounter = Timer0.GenericCounter;
		return;	//added 20150515 for test camera capture
	}

	HoldTime = GCodeRIPointer->StartPoint.Axis1;

	if ((STDNCSign.PrecalCode == 0) && OverallSign.EveryAxisCoinValid) 
	{
		Timer0.G04StartCounter = Timer0.GenericCounter;
		STDNCSign.PrecalCode = 1;
	}

	if (OverallSign.CameraStaticCaptureSign == 1) 
	{
		if (STDNCSign.PrecalCode == 1) 
		{
			if(HoldTime > SwitchTime3)
			{
				//if (get_delta_time(Timer0.G04StartCounter) > (HoldTime - SwitchTime2)) //500-300=200
				if (get_delta_time(Timer0.G04StartCounter) > SwitchTime3) 
				{
					Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 1; //for trigger camera capture(open)
					*SERVO_Dir1 = Servo_Sign1_Out_Register.all;
				} 
				//else if (get_delta_time(Timer0.G04StartCounter) > (HoldTime - SwitchTime1)) //500-302=198
				else if (get_delta_time(Timer0.G04StartCounter) > (SwitchTime3-2)) 
				{
					Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 0; //for trigger camera capture(close)
					*SERVO_Dir1 = Servo_Sign1_Out_Register.all;
				}
			}
			else
			{
				if (get_delta_time(Timer0.G04StartCounter) > (HoldTime - SwitchTime1)) 
				{
					Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 1; //for trigger camera capture(open)
					*SERVO_Dir1 = Servo_Sign1_Out_Register.all;
				} 
				else if (get_delta_time(Timer0.G04StartCounter) > (HoldTime - SwitchTime2)) 
				{
					Servo_Sign1_Out_Register.bit.SERVO_SIGN_23 = 0; //for trigger camera capture(close)
					*SERVO_Dir1 = Servo_Sign1_Out_Register.all;
				}
			}
		}
	}

	if ((OverallSign.InterpolationRunCnt_1 >= OverallSign.InterpolationSendCnt_1)) 
	{
		if (OverallSign.EveryAxisCoinValid) 
		{
			if (get_delta_time(Timer0.G04StartCounter) > HoldTime) 
			{
				if (OverallSign.CameraStaticCaptureSign == 1) 
				{
					if(HoldTime < SwitchTime3)
					{			
						//Write_LEDFlash_OUT(0x03,psParaArm_Ctrl->LEDFlashTime1,psParaArm_Ctrl->LEDFlashTime2);//flash time						
						Write_LEDFlash_OUT(0x03,System.LEDFlashTime1,System.LEDFlashTime2);//flash time
					}
				}
				
				STDNCSign.CodeRun_Over = 1;
			}
		}
	}
}

Uint16 STD_run_M03(void)		// barcode scan
{
	GPIO_Out1_Register.bit.GPIOOUT9 = 1;
	Write_GPIO_Out1();
	//read_GPIO_in();
	//if(FPGA_GPIO_InRegister.bit.GPIOIN1 == 1)
	{
		STDNCSign.CodeRun_Over = 1;
	}

	return 1;
}

Uint16 STD_run_M04(void)		// sensor1
{
	Read_GPIO_In1();
	if ((GPIO_In1_Register.bit.GPIOIN1 == 1) && (GPIO_In1_Register.bit.GPIOIN2 == 1)) 
	{
		//if(Clone_Linear_Position.CompleteIncubatorOpenDoorSign == 1)
		{
			STDNCSign.CodeRun_Over = 1;
		}
	} 
	else 
	{
	}

	return 1;
}

Uint16 STD_run_M05(void)		// sensor2
{
	Read_GPIO_In1();
	if (GPIO_In1_Register.bit.GPIOIN2 == 1) 
	{
		STDNCSign.CodeRun_Over = 1;
	}

	return 1;
}

Uint16 STD_run_M06(void)		// sensor3
{
	Read_GPIO_In1();
	if (GPIO_In1_Register.bit.GPIOIN3 == 1) 
	{
		STDNCSign.CodeRun_Over = 1;
	}

	return 1;
}

INT40AXIS IncrementalCoordinate_to_AbsCoordinate(INT40AXIS IncrementalCoordinate) 
{
	INT40AXIS AbsCoordinate;

	AbsCoordinate.Axis1 = IncrementalCoordinate.Axis1
			+ OverallSign.ABSORG_Coordinate.Axis1;
	AbsCoordinate.Axis2 = IncrementalCoordinate.Axis2
			+ OverallSign.ABSORG_Coordinate.Axis2;
	AbsCoordinate.Axis3 = IncrementalCoordinate.Axis3
			+ OverallSign.ABSORG_Coordinate.Axis3;
	AbsCoordinate.Axis4 = IncrementalCoordinate.Axis4
			+ OverallSign.ABSORG_Coordinate.Axis4;
	AbsCoordinate.Axis5 = IncrementalCoordinate.Axis5
			+ OverallSign.ABSORG_Coordinate.Axis5;
	AbsCoordinate.Axis6 = IncrementalCoordinate.Axis6
			+ OverallSign.ABSORG_Coordinate.Axis6;
	AbsCoordinate.Axis7 = IncrementalCoordinate.Axis7
			+ OverallSign.ABSORG_Coordinate.Axis7;
	AbsCoordinate.Axis8 = IncrementalCoordinate.Axis8
			+ OverallSign.ABSORG_Coordinate.Axis8;
	AbsCoordinate.Axis9 = IncrementalCoordinate.Axis9
			+ OverallSign.ABSORG_Coordinate.Axis9;
	AbsCoordinate.Axis10 = IncrementalCoordinate.Axis10
			+ OverallSign.ABSORG_Coordinate.Axis10;
	AbsCoordinate.Axis11 = IncrementalCoordinate.Axis11
			+ OverallSign.ABSORG_Coordinate.Axis11;
	AbsCoordinate.Axis12 = IncrementalCoordinate.Axis12
			+ OverallSign.ABSORG_Coordinate.Axis12;
	AbsCoordinate.Axis13 = IncrementalCoordinate.Axis13
			+ OverallSign.ABSORG_Coordinate.Axis13;
	AbsCoordinate.Axis14 = IncrementalCoordinate.Axis14
			+ OverallSign.ABSORG_Coordinate.Axis14;
	AbsCoordinate.Axis15 = IncrementalCoordinate.Axis15
			+ OverallSign.ABSORG_Coordinate.Axis15;
	AbsCoordinate.Axis16 = IncrementalCoordinate.Axis16
			+ OverallSign.ABSORG_Coordinate.Axis16;
	AbsCoordinate.Axis17 = IncrementalCoordinate.Axis17
			+ OverallSign.ABSORG_Coordinate.Axis17;
	AbsCoordinate.Axis18 = IncrementalCoordinate.Axis18
			+ OverallSign.ABSORG_Coordinate.Axis18;
	AbsCoordinate.Axis19 = IncrementalCoordinate.Axis19
			+ OverallSign.ABSORG_Coordinate.Axis19;
	AbsCoordinate.Axis20 = IncrementalCoordinate.Axis20
			+ OverallSign.ABSORG_Coordinate.Axis20;
	AbsCoordinate.Axis21 = IncrementalCoordinate.Axis21
			+ OverallSign.ABSORG_Coordinate.Axis21;
	AbsCoordinate.Axis22 = IncrementalCoordinate.Axis22
			+ OverallSign.ABSORG_Coordinate.Axis22;
	AbsCoordinate.Axis23 = IncrementalCoordinate.Axis23
			+ OverallSign.ABSORG_Coordinate.Axis23;

	return AbsCoordinate;
}

//工件坐标变换到机床坐标  CH0701
//入口:  工件绝对坐标值 单位	: um or ud
//出口:  机床坐标值 		 单位: um or ud
INT40AXIS ABSCoordinate_to_MachineCoordinate(INT40AXIS AbsCoordinate) 
{
	INT40AXIS MachineCoordinate;

	MachineCoordinate.Axis1 = AbsCoordinate.Axis1
			+ OverallSign.ABSORG_M_Coordinate.Axis1;
	MachineCoordinate.Axis2 = AbsCoordinate.Axis2
			+ OverallSign.ABSORG_M_Coordinate.Axis2;
	MachineCoordinate.Axis3 = AbsCoordinate.Axis3
			+ OverallSign.ABSORG_M_Coordinate.Axis3;
	MachineCoordinate.Axis4 = AbsCoordinate.Axis4
			+ OverallSign.ABSORG_M_Coordinate.Axis4;
	MachineCoordinate.Axis5 = AbsCoordinate.Axis5
			+ OverallSign.ABSORG_M_Coordinate.Axis5;
	MachineCoordinate.Axis6 = AbsCoordinate.Axis6
			+ OverallSign.ABSORG_M_Coordinate.Axis6;
	MachineCoordinate.Axis7 = AbsCoordinate.Axis7
			+ OverallSign.ABSORG_M_Coordinate.Axis7;
	MachineCoordinate.Axis8 = AbsCoordinate.Axis8
			+ OverallSign.ABSORG_M_Coordinate.Axis8;
	MachineCoordinate.Axis9 = AbsCoordinate.Axis9
			+ OverallSign.ABSORG_M_Coordinate.Axis9;
	MachineCoordinate.Axis10 = AbsCoordinate.Axis10
			+ OverallSign.ABSORG_M_Coordinate.Axis10;
	MachineCoordinate.Axis11 = AbsCoordinate.Axis11
			+ OverallSign.ABSORG_M_Coordinate.Axis11;
	MachineCoordinate.Axis12 = AbsCoordinate.Axis12
			+ OverallSign.ABSORG_M_Coordinate.Axis12;
	MachineCoordinate.Axis13 = AbsCoordinate.Axis13
			+ OverallSign.ABSORG_M_Coordinate.Axis13;
	MachineCoordinate.Axis14 = AbsCoordinate.Axis14
			+ OverallSign.ABSORG_M_Coordinate.Axis14;
	MachineCoordinate.Axis15 = AbsCoordinate.Axis15
			+ OverallSign.ABSORG_M_Coordinate.Axis15;
	MachineCoordinate.Axis16 = AbsCoordinate.Axis16
			+ OverallSign.ABSORG_M_Coordinate.Axis16;
	MachineCoordinate.Axis17 = AbsCoordinate.Axis17
			+ OverallSign.ABSORG_M_Coordinate.Axis17;
	MachineCoordinate.Axis18 = AbsCoordinate.Axis18
			+ OverallSign.ABSORG_M_Coordinate.Axis18;
	MachineCoordinate.Axis19 = AbsCoordinate.Axis19
			+ OverallSign.ABSORG_M_Coordinate.Axis19;
	MachineCoordinate.Axis20 = AbsCoordinate.Axis20
			+ OverallSign.ABSORG_M_Coordinate.Axis20;
	MachineCoordinate.Axis21 = AbsCoordinate.Axis21
			+ OverallSign.ABSORG_M_Coordinate.Axis21;
	MachineCoordinate.Axis22 = AbsCoordinate.Axis22
			+ OverallSign.ABSORG_M_Coordinate.Axis22;
	MachineCoordinate.Axis23 = AbsCoordinate.Axis23
			+ OverallSign.ABSORG_M_Coordinate.Axis23;

	return MachineCoordinate;
}

//机床原始坐标值变换到绝对坐标 
//入口: 补偿后的机床坐标值	: 10nm or 10nd
//出口:  绝对坐标值 				: 10nm or 10nd
INT40AXIS MachineCoordinate_to_ABSCoordinate(INT40AXIS MachineCoordinate) {
	INT40AXIS ABSCoordinate;
	ABSCoordinate.Axis1 = MachineCoordinate.Axis1
			- OverallSign.ABSORG_M_Coordinate.Axis1;
	ABSCoordinate.Axis2 = MachineCoordinate.Axis2
			- OverallSign.ABSORG_M_Coordinate.Axis2;
	ABSCoordinate.Axis3 = MachineCoordinate.Axis3
			- OverallSign.ABSORG_M_Coordinate.Axis3;
	ABSCoordinate.Axis4 = MachineCoordinate.Axis4
			- OverallSign.ABSORG_M_Coordinate.Axis4;
	ABSCoordinate.Axis5 = MachineCoordinate.Axis5
			- OverallSign.ABSORG_M_Coordinate.Axis5;
	ABSCoordinate.Axis6 = MachineCoordinate.Axis6
			- OverallSign.ABSORG_M_Coordinate.Axis6;
	ABSCoordinate.Axis7 = MachineCoordinate.Axis7
			- OverallSign.ABSORG_M_Coordinate.Axis7;
	ABSCoordinate.Axis8 = MachineCoordinate.Axis8
			- OverallSign.ABSORG_M_Coordinate.Axis8;
	ABSCoordinate.Axis9 = MachineCoordinate.Axis9
			- OverallSign.ABSORG_M_Coordinate.Axis9;
	ABSCoordinate.Axis10 = MachineCoordinate.Axis10
			- OverallSign.ABSORG_M_Coordinate.Axis10;
	ABSCoordinate.Axis11 = MachineCoordinate.Axis11
			- OverallSign.ABSORG_M_Coordinate.Axis11;
	ABSCoordinate.Axis12 = MachineCoordinate.Axis12
			- OverallSign.ABSORG_M_Coordinate.Axis12;
	ABSCoordinate.Axis13 = MachineCoordinate.Axis13
			- OverallSign.ABSORG_M_Coordinate.Axis13;
	ABSCoordinate.Axis14 = MachineCoordinate.Axis14
			- OverallSign.ABSORG_M_Coordinate.Axis14;
	ABSCoordinate.Axis15 = MachineCoordinate.Axis15
			- OverallSign.ABSORG_M_Coordinate.Axis15;
	ABSCoordinate.Axis16 = MachineCoordinate.Axis16
			- OverallSign.ABSORG_M_Coordinate.Axis16;
	ABSCoordinate.Axis17 = MachineCoordinate.Axis17
			- OverallSign.ABSORG_M_Coordinate.Axis17;
	ABSCoordinate.Axis18 = MachineCoordinate.Axis18
			- OverallSign.ABSORG_M_Coordinate.Axis18;
	ABSCoordinate.Axis19 = MachineCoordinate.Axis19
			- OverallSign.ABSORG_M_Coordinate.Axis19;
	ABSCoordinate.Axis20 = MachineCoordinate.Axis20
			- OverallSign.ABSORG_M_Coordinate.Axis20;
	ABSCoordinate.Axis21 = MachineCoordinate.Axis21
			- OverallSign.ABSORG_M_Coordinate.Axis21;
	ABSCoordinate.Axis22 = MachineCoordinate.Axis22
			- OverallSign.ABSORG_M_Coordinate.Axis22;
	ABSCoordinate.Axis23 = MachineCoordinate.Axis23
			- OverallSign.ABSORG_M_Coordinate.Axis23;

	return ABSCoordinate;
}

//速度限定判定程序 added 090907
Uint16 LimitAxisSpeed(AREA_CONTROL *AREAControl, GCODE *AreaGcodeBufferPointer,
		NC_RUN_SIGN *AreaRunNCSign) 
{
	GCODE *GCodeRIPointer;
	float64 VX, VY, VZ, Va, VTemp;	//由各轴反算的速度值mm/s
	Uint32 ri;

	ri = (AreaRunNCSign->VeVmCalCount - 1) % STDGCODE_MOD;

	if ((AreaGcodeBufferPointer + ri)->Stotal == 0) 
	{
		return 0;
	}
	if ((AreaGcodeBufferPointer + ri)->SubCMD1 == 1)//解决JOG、HAND必须先动XYZ问题
	{
		if ((AreaGcodeBufferPointer + ri)->Vm > System.MAXSpeed.Axis4) 
		{
			(AreaGcodeBufferPointer + ri)->Vm = System.MAXSpeed.Axis4;
		}
		return 0;
	}

	if (((AreaGcodeBufferPointer + ri)->CMDMain == 0)
			|| ((AreaGcodeBufferPointer + ri)->CMDMain == 1)) 
	{
		if (fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis1) < 0.001) 
		{
			VX = 1000;
		} 
		else 
		{
			VX = divdp(System.MAXSpeed.Axis1,
					fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis1));
		}
		if (fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis2) < 0.001) 
		{
			VY = 1000;
		} 
		else 
		{
			VY = divdp(System.MAXSpeed.Axis2,
					fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis2));
		}
		if (fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis3) < 0.001) 
		{
			VZ = 1000;
		} 
		else 
		{
			VZ = divdp(System.MAXSpeed.Axis3,
					fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis3));
		}

		if (fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis4) < 0.001) 
		{
			Va = 1000;
		} 
		else 
		{
			Va = divdp(System.MAXSpeed.Axis4,
					fabs((AreaGcodeBufferPointer + ri)->EndPointVector.Axis4));
		}

		//取最小值加入了A4轴的速度比较CH20100311 Qing
		VTemp = VX;
		if (VTemp > VY) 
		{
			VTemp = VY;
		}
		if (VTemp > VZ) 
		{
			VTemp = VZ;
		}
	}

	if ((AreaGcodeBufferPointer + ri)->Vm > VTemp) 
	{
		(AreaGcodeBufferPointer + ri)->Vm = VTemp;
	}

	return 1;
}

//参考点搜索开始区
Uint16 SearchReference(void) 
{
	static int TestA4Time = 100;

	if(System.FunctionSelect05 == True)
	{
		return 0;
	}

	if( Reference.SearchRefSign == 0 )
	{
		return 0;
	}

	OverallSign.NCSign = SPCCODERUN;

	if( Reference.RefStep == 0)
	{
		if(Reference.NeedFindAxisSign.Axis19 == 0)
		{
			Reference.RefStep = 2;
			return;
		}
			
		if( (SPCNCSign.GetPositionSign == 0) &&
		(STDNCSign.GetPositionSign == 0) &&
		(NCRunSign.GetPositionSign == 0) )
		{
			GCODE *GCodeRiPointer;
			OverallSign.LensMotionOverSign = LensMotionNormal;
			DeltaStepAxis19_CPS = 0;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;
			
			NC_AREAControl.Pointer_StartCode = 0;
			NC_AREAControl.Pointer_SecondInflexionCode = 0;
			NC_AREAControl.Pointer_EndCode = 0;
			NC_AREAControl.DeltaStep.Axis1 =0;
			NC_AREAControl.DeltaStep.Axis2 =0;
			NC_AREAControl.DeltaStep.Axis3 =0;
			NC_AREAControl.DeltaStep.Axis4=0;
			NC_AREAControl.DeltaStep.Axis5 =0;
			NC_AREAControl.DeltaStep.Axis6 =0;
			NC_AREAControl.DeltaStep.Axis7 =0;
			NC_AREAControl.DeltaStep.Axis8=0;
			NC_AREAControl.DeltaStep.Axis9 =0;
			NC_AREAControl.DeltaStep.Axis10 =0;
			NC_AREAControl.DeltaStep.Axis11 =0;
			NC_AREAControl.DeltaStep.Axis12 =0;
			NC_AREAControl.DeltaStep.Axis13 =0;
			NC_AREAControl.DeltaStep.Axis14=0;
			NC_AREAControl.DeltaStep.Axis15 =0;
			NC_AREAControl.DeltaStep.Axis16 =0;
			NC_AREAControl.DeltaStep.Axis17 =0;
			NC_AREAControl.DeltaStep.Axis18=0;
			NC_AREAControl.DeltaStep.Axis19 =0;
			NC_AREAControl.DeltaStep.Axis20 =0;
			NC_AREAControl.DeltaStep.Axis21 =0;
			NC_AREAControl.DeltaStep.Axis22 =0;
			NC_AREAControl.DeltaStep.Axis23 =0;
			NC_AREAControl.DeltaStep.Axis24=0;
			NC_AREAControl.DeltaStep.Axis25 =0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			SPCNCSign.RealSendCount++;
			SPCNCSign.RealRunCount = SPCNCSign.RealSendCount;
			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->SendCount = 1;
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;
			GCodeRiPointer->SubCMD2 = 0;
			
			GCodeRiPointer->Feedrate = (System.SRefSpeed>10) ?  10: System.SRefSpeed;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;
	
			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;
			
			if(Reference.NeedFindAxisSign.Axis19)
			{
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 + 1000000*System.RefDir.Axis19;
			}

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			Reference.RefStep = 1;
		}

	}
	else if( Reference.RefStep == 1)
	{
		if(Reference.NeedFindAxisSign.Axis19)
		{
			//if(!((System.RefDir.Axis19==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))
			if(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19 == 0)
			{
				Reference.RefStep = Reference.RefStep;
				return;
			}
		}

		OverallSign.LensMotionOverSign = LensMotionNormal;
		Reference.RefStep = 2;
		SPCNCSign.GetPositionSign = 1;
		return;

	}
	else if( Reference.RefStep == 2)
	{
		if( (SPCNCSign.GetPositionSign == 0) &&
		(STDNCSign.GetPositionSign == 0) &&
		(NCRunSign.GetPositionSign == 0) )
		{
			GCODE *GCodeRiPointer;

			if(Reference.NeedFindAxisSign.Axis2)
			{
				//A4 search reference
				GPIO_Out2_Register.bit.GPIOOUT16 = 1-GPIO_Out2_Register.bit.GPIOOUT16;
				Write_GPIO_Out2();
				Timer0.SMC1_Homing_Counter = Timer0.GenericCounter;
			}

			if(Reference.NeedFindAxisSign.Axis10)
			{
				//A4 search reference
				GPIO_Out2_Register.bit.GPIOOUT14 = 1-GPIO_Out2_Register.bit.GPIOOUT14;
				Write_GPIO_Out2();
				Timer0.SMC1_Homing_Counter = Timer0.GenericCounter;
			}

			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;
			
			NC_AREAControl.Pointer_StartCode = 0;
			NC_AREAControl.Pointer_SecondInflexionCode = 0;
			NC_AREAControl.Pointer_EndCode = 0;
			NC_AREAControl.DeltaStep.Axis1 =0;
			NC_AREAControl.DeltaStep.Axis2 =0;
			NC_AREAControl.DeltaStep.Axis3 =0;
			NC_AREAControl.DeltaStep.Axis4=0;
			NC_AREAControl.DeltaStep.Axis5 =0;
			NC_AREAControl.DeltaStep.Axis6 =0;
			NC_AREAControl.DeltaStep.Axis7 =0;
			NC_AREAControl.DeltaStep.Axis8=0;
			NC_AREAControl.DeltaStep.Axis9 =0;
			NC_AREAControl.DeltaStep.Axis10 =0;
			NC_AREAControl.DeltaStep.Axis11 =0;
			NC_AREAControl.DeltaStep.Axis12 =0;
			NC_AREAControl.DeltaStep.Axis13 =0;
			NC_AREAControl.DeltaStep.Axis14=0;
			NC_AREAControl.DeltaStep.Axis15 =0;
			NC_AREAControl.DeltaStep.Axis16 =0;
			NC_AREAControl.DeltaStep.Axis17 =0;
			NC_AREAControl.DeltaStep.Axis18=0;
			NC_AREAControl.DeltaStep.Axis19 =0;
			NC_AREAControl.DeltaStep.Axis20 =0;
			NC_AREAControl.DeltaStep.Axis21 =0;
			NC_AREAControl.DeltaStep.Axis22 =0;
			NC_AREAControl.DeltaStep.Axis23 =0;
			NC_AREAControl.DeltaStep.Axis24=0;
			NC_AREAControl.DeltaStep.Axis25 =0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			SPCNCSign.RealSendCount++;
			SPCNCSign.RealRunCount = SPCNCSign.RealSendCount;
			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->SendCount = 1;
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;
			GCodeRiPointer->SubCMD2 = 0;
			
			GCodeRiPointer->Feedrate = (System.SRefSpeed>10) ?  10: System.SRefSpeed;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;
	
			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;
			
			if(Reference.NeedFindAxisSign.Axis1)
			{
				GCodeRiPointer->EndPoint.Axis1 = GCodeRiPointer->StartPoint.Axis1 + 1000000*System.RefDir.Axis1;
			}
			#if 0
			if(Reference.NeedFindAxisSign.Axis3)
			{
				GCodeRiPointer->EndPoint.Axis3 = GCodeRiPointer->StartPoint.Axis3 + 1000000*System.RefDir.Axis3;
			}	
			if(Reference.NeedFindAxisSign.Axis4)
			{
				GCodeRiPointer->EndPoint.Axis4 = GCodeRiPointer->StartPoint.Axis4 + 1000000*System.RefDir.Axis4;
			}
			if(Reference.NeedFindAxisSign.Axis5)
			{
				GCodeRiPointer->EndPoint.Axis5 = GCodeRiPointer->StartPoint.Axis5 + 1000000*System.RefDir.Axis5;
			}
			if(Reference.NeedFindAxisSign.Axis6)
			{
				GCodeRiPointer->EndPoint.Axis6 = GCodeRiPointer->StartPoint.Axis6 + 1000000*System.RefDir.Axis6;
			}
			if(Reference.NeedFindAxisSign.Axis7)
			{
				GCodeRiPointer->EndPoint.Axis7 = GCodeRiPointer->StartPoint.Axis7 + 1000000*System.RefDir.Axis7;
			}	
			if(Reference.NeedFindAxisSign.Axis8)
			{
				GCodeRiPointer->EndPoint.Axis8 = GCodeRiPointer->StartPoint.Axis8 + 1000000*System.RefDir.Axis8;
			}
			if(Reference.NeedFindAxisSign.Axis9)
			{
				GCodeRiPointer->EndPoint.Axis9 = GCodeRiPointer->StartPoint.Axis9 + 1000000*System.RefDir.Axis9;
			}
			if(Reference.NeedFindAxisSign.Axis11)
			{
				GCodeRiPointer->EndPoint.Axis11 = GCodeRiPointer->StartPoint.Axis11 + 1000000*System.RefDir.Axis11;
			}
			if(Reference.NeedFindAxisSign.Axis12)
			{
				GCodeRiPointer->EndPoint.Axis12 = GCodeRiPointer->StartPoint.Axis12 + 1000000*System.RefDir.Axis12;
			}
			if(Reference.NeedFindAxisSign.Axis13)
			{
				GCodeRiPointer->EndPoint.Axis13 = GCodeRiPointer->StartPoint.Axis13 + 1000000*System.RefDir.Axis13;
			}	
			if(Reference.NeedFindAxisSign.Axis14)
			{
				GCodeRiPointer->EndPoint.Axis14 = GCodeRiPointer->StartPoint.Axis14 + 1000000*System.RefDir.Axis14;
			}
			if(Reference.NeedFindAxisSign.Axis15)
			{
				GCodeRiPointer->EndPoint.Axis15 = GCodeRiPointer->StartPoint.Axis15 + 1000000*System.RefDir.Axis15;
			}
			if(Reference.NeedFindAxisSign.Axis16)
			{
				GCodeRiPointer->EndPoint.Axis16 = GCodeRiPointer->StartPoint.Axis16 + 1000000*System.RefDir.Axis16;
			}
			if(Reference.NeedFindAxisSign.Axis17)
			{
				GCodeRiPointer->EndPoint.Axis17 = GCodeRiPointer->StartPoint.Axis17 + 1000000*System.RefDir.Axis17;
			}	
			if(Reference.NeedFindAxisSign.Axis18)
			{
				GCodeRiPointer->EndPoint.Axis18 = GCodeRiPointer->StartPoint.Axis18 + 1000000*System.RefDir.Axis18;
			}
			#endif
			#if 0
			if(Reference.NeedFindAxisSign.Axis19)
			{
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 + 1000000*System.RefDir.Axis19;
			}
			#endif
			#if 0
			if(Reference.NeedFindAxisSign.Axis20)
			{
				GCodeRiPointer->EndPoint.Axis20 = GCodeRiPointer->StartPoint.Axis20 + 1000000*System.RefDir.Axis20;
			}
			if(Reference.NeedFindAxisSign.Axis21)
			{
				GCodeRiPointer->EndPoint.Axis21 = GCodeRiPointer->StartPoint.Axis21 + 1000000*System.RefDir.Axis21;
			}
			if(Reference.NeedFindAxisSign.Axis22)
			{
				GCodeRiPointer->EndPoint.Axis22 = GCodeRiPointer->StartPoint.Axis22 + 1000000*System.RefDir.Axis22;
			}
			if(Reference.NeedFindAxisSign.Axis23)
			{
				GCodeRiPointer->EndPoint.Axis23 = GCodeRiPointer->StartPoint.Axis23 + 1000000*System.RefDir.Axis23;
			}
			#endif
			#if 0
			if(Reference.NeedFindAxisSign.Axis24)
			{
				GCodeRiPointer->EndPoint.Axis24 = GCodeRiPointer->StartPoint.Axis24 + 1000000*System.RefDir.Axis24;
			}
			if(Reference.NeedFindAxisSign.Axis25)
			{
				GCodeRiPointer->EndPoint.Axis25 = GCodeRiPointer->StartPoint.Axis25 + 1000000*System.RefDir.Axis25;
			}	
			#endif

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			Reference.RefStep = 3;
		}

	}
	else if( Reference.RefStep == 3)
	{
		if(Reference.NeedFindAxisSign.Axis2)
		{
			if((get_delta_time(Timer0.SMC1_Homing_Counter) >  TestA4Time))
			{
				GPIO_Out2_Register.bit.GPIOOUT16 = 1-GPIO_Out2_Register.bit.GPIOOUT16;
				Write_GPIO_Out2();
			}
			else
			{	//added 20160304
				return;
			}

			if(GPIO_In1_Register.bit.GPIOIN1 == 1) // 0:valid;1:invalid
			{
				NC_AREAControl.OldValue.Axis2 = NC_AREAControl.OldValue.Axis2;
				return;
			}
			else
			{	//A4 find reference and clear zero
				NC_AREAControl.OldValue.Axis2= 0;
				Reference.FindRefSign.Axis2= 1;
			}
		}

		
		if(Reference.NeedFindAxisSign.Axis10)
		{
			if((get_delta_time(Timer0.SMC2_Homing_Counter) >  TestA4Time))
			{
				GPIO_Out2_Register.bit.GPIOOUT14 = 1-GPIO_Out2_Register.bit.GPIOOUT14;
				Write_GPIO_Out2();
			}
			else
			{	//added 20160304
				return;
			}

			if(GPIO_In1_Register.bit.GPIOIN2 == 1)  // 0:valid;1:invalid
			{
				NC_AREAControl.OldValue.Axis10 = NC_AREAControl.OldValue.Axis10;
				return;
			}
			else
			{	//find reference and clear zero
				NC_AREAControl.OldValue.Axis10= 0;
				Reference.FindRefSign.Axis10= 1;
			}
		}

		if(Reference.NeedFindAxisSign.Axis1)
		{
			if(!((System.RefDir.Axis1==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1))
			{
				return;
			}
		}
		
		if(Reference.NeedFindAxisSign.Axis3)
		{
			if(!((System.RefDir.Axis3==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis4)
		{
			if(!((System.RefDir.Axis4==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis5)
		{
			if(!((System.RefDir.Axis5==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis6)
		{
			if(!((System.RefDir.Axis6==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis7)
		{
			if(!((System.RefDir.Axis7==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis8)
		{
			if(!((System.RefDir.Axis8==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis9)
		{
			if(!((System.RefDir.Axis9==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis11)
		{
			if(!((System.RefDir.Axis11==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis12)
		{
			if(!((System.RefDir.Axis12==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis13)
		{
			if(!((System.RefDir.Axis13==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis14)
		{
			if(!((System.RefDir.Axis14==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis15)
		{
			if(!((System.RefDir.Axis15==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis16)
		{
			if(!((System.RefDir.Axis16==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis17)
		{
			if(!((System.RefDir.Axis17==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis18)
		{
			if(!((System.RefDir.Axis18==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis19)
		{
			if(!((System.RefDir.Axis19==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))
			{
				return;
			}	
			OverallSign.LensMotionOverSign = LensMotionNormal;
			DeltaStepAxis19_CPS = 0;
		}
		if(Reference.NeedFindAxisSign.Axis20)
		{
			if(!((System.RefDir.Axis20==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis21)
		{
			if(!((System.RefDir.Axis21==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis22)
		{
			if(!((System.RefDir.Axis22==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis23)
		{
			if(!((System.RefDir.Axis23==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23))
			{
				return;
			}
		}	

		Reference.RefStep = 4;
		SPCNCSign.GetPositionSign = 1;
		return;

	}
	else if( Reference.RefStep == 4)
	{
		if( SPCNCSign.GetPositionSign == 0 )
		{
			GCODE *GCodeRiPointer;
			OverallSign.LensMotionOverSign = LensMotionNormal;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;
			GCodeRiPointer->Feedrate = (System.SRefSpeedBack>3) ?  3: System.SRefSpeedBack;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;
			
			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;

			if(Reference.NeedFindAxisSign.Axis1)
			{
				GCodeRiPointer->EndPoint.Axis1 = GCodeRiPointer->StartPoint.Axis1 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis1;
			}

			#if 0
			if(Reference.NeedFindAxisSign.Axis3)
			{
				GCodeRiPointer->EndPoint.Axis3 = GCodeRiPointer->StartPoint.Axis3 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis3;
			}	
			if(Reference.NeedFindAxisSign.Axis4)
			{
				GCodeRiPointer->EndPoint.Axis4 = GCodeRiPointer->StartPoint.Axis4 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis4;
			}
			if(Reference.NeedFindAxisSign.Axis5)
			{
				GCodeRiPointer->EndPoint.Axis5 = GCodeRiPointer->StartPoint.Axis5 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis5;
			}
			if(Reference.NeedFindAxisSign.Axis6)
			{
				GCodeRiPointer->EndPoint.Axis6 = GCodeRiPointer->StartPoint.Axis6 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis6;
			}
			if(Reference.NeedFindAxisSign.Axis7)
			{
				GCodeRiPointer->EndPoint.Axis7 = GCodeRiPointer->StartPoint.Axis7 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis7;
			}	
			if(Reference.NeedFindAxisSign.Axis8)
			{
				GCodeRiPointer->EndPoint.Axis8 = GCodeRiPointer->StartPoint.Axis8 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis8;
			}
			if(Reference.NeedFindAxisSign.Axis9)
			{
				GCodeRiPointer->EndPoint.Axis9 = GCodeRiPointer->StartPoint.Axis9 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis9;
			}
			if(Reference.NeedFindAxisSign.Axis11)
			{
				GCodeRiPointer->EndPoint.Axis11 = GCodeRiPointer->StartPoint.Axis11 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis11;
			}
			if(Reference.NeedFindAxisSign.Axis12)
			{
				GCodeRiPointer->EndPoint.Axis12 = GCodeRiPointer->StartPoint.Axis12 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis12;
			}
			if(Reference.NeedFindAxisSign.Axis13)
			{
				GCodeRiPointer->EndPoint.Axis13 = GCodeRiPointer->StartPoint.Axis13 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis13;
			}	
			if(Reference.NeedFindAxisSign.Axis14)
			{
				GCodeRiPointer->EndPoint.Axis14 = GCodeRiPointer->StartPoint.Axis14 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis14;
			}
			if(Reference.NeedFindAxisSign.Axis15)
			{
				GCodeRiPointer->EndPoint.Axis15 = GCodeRiPointer->StartPoint.Axis15 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis15;
			}
			if(Reference.NeedFindAxisSign.Axis16)
			{
				GCodeRiPointer->EndPoint.Axis16 = GCodeRiPointer->StartPoint.Axis16 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis16;
			}
			if(Reference.NeedFindAxisSign.Axis17)
			{
				GCodeRiPointer->EndPoint.Axis17 = GCodeRiPointer->StartPoint.Axis17 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis17;
			}	
			if(Reference.NeedFindAxisSign.Axis18)
			{
				GCodeRiPointer->EndPoint.Axis18 = GCodeRiPointer->StartPoint.Axis18 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis18;
			}
			#endif
			if(Reference.NeedFindAxisSign.Axis19)
			{
				//GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis19;
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(2,System.UnitTo_mm)*System.RefDir.Axis19;
			}
			#if 0
			if(Reference.NeedFindAxisSign.Axis20)
			{
				GCodeRiPointer->EndPoint.Axis20 = GCodeRiPointer->StartPoint.Axis20 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis20;
			}
			if(Reference.NeedFindAxisSign.Axis21)
			{
				GCodeRiPointer->EndPoint.Axis21 = GCodeRiPointer->StartPoint.Axis21 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis21;
			}
			if(Reference.NeedFindAxisSign.Axis22)
			{
				GCodeRiPointer->EndPoint.Axis22 = GCodeRiPointer->StartPoint.Axis22 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis22;
			}
			if(Reference.NeedFindAxisSign.Axis23)
			{
				GCodeRiPointer->EndPoint.Axis23 = GCodeRiPointer->StartPoint.Axis23 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis23;
			}
			#endif

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			SPCNCSign.CodeRun_Over = 0;
			Reference.RefStep = 5;
		}
		
		return 0;
	}
	else if( Reference.RefStep == 5 )
	{
		Read_Servo_HardLimit_Positive1_In();
		Read_Servo_HardLimit_Negative1_In();
		Read_Servo_HardLimit_Positive2_In();
		Read_Servo_HardLimit_Negative2_In();

		if(Reference.NeedFindAxisSign.Axis1)
		{
			if(((System.RefDir.Axis1==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_1 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_1))
			{
				return;
			}
		}
		
		if(Reference.NeedFindAxisSign.Axis3)
		{
			if(((System.RefDir.Axis3==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_3 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_3))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis4)
		{
			if(((System.RefDir.Axis4==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_4 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_4))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis5)
		{
			if(((System.RefDir.Axis5==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_5 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_5))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis6)
		{
			if(((System.RefDir.Axis6==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_6 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_6))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis7)
		{
			if(((System.RefDir.Axis7==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_7 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_7))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis8)
		{
			if(((System.RefDir.Axis8==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_8 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_8))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis9)
		{
			if(((System.RefDir.Axis9==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_9 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_9))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis11)
		{
			if(((System.RefDir.Axis11==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_11 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_11))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis12)
		{
			if(((System.RefDir.Axis12==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_12 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_12))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis13)
		{
			if(((System.RefDir.Axis13==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_13 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_13))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis14)
		{
			if(((System.RefDir.Axis14==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_14 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_14))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis15)
		{
			if(((System.RefDir.Axis15==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_15 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_15))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis16)
		{
			if(((System.RefDir.Axis16==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_16 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_16))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis17)
		{
			if(((System.RefDir.Axis17==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_17 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_17))
			{
				return;
			}
		}	
		if(Reference.NeedFindAxisSign.Axis18)
		{
			if(((System.RefDir.Axis18==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_18 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_18))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis19)
		{
			if(((System.RefDir.Axis19==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))
			{
				return;
			}			
			OverallSign.LensMotionOverSign = LensMotionNormal;		
			DeltaStepAxis19_CPS = 0;
			//for test
			if(Reference.NeedFindAxisSign.Axis19)
			{
				CPLD_EncoderResetAxis19();
				Reference.FindRefSign.Axis19 = 2;
			}
		}
		if(Reference.NeedFindAxisSign.Axis20)
		{
			if(((System.RefDir.Axis20==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_20 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_20))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis21)
		{
			if(((System.RefDir.Axis21==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_21 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_21))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis22)
		{
			if(((System.RefDir.Axis22==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_22 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_22))
			{
				return;
			}
		}
		if(Reference.NeedFindAxisSign.Axis23)
		{
			if(((System.RefDir.Axis23==1)?Servo_HardLimit_Positive1_In_Register.bit.HLIMIT_POS_23 : Servo_HardLimit_Negative1_In_Register.bit.HLIMIT_NEG_23))
			{
				return;
			}
		}

		Reference.RefStep = 6;
		SPCNCSign.GetPositionSign = 1;
		return;
	}
	else if( Reference.RefStep == 6 )
	{
		if( SPCNCSign.GetPositionSign == 0 )
		{
			GCODE *GCodeRiPointer;			
			OverallSign.LensMotionOverSign = LensMotionNormal;
			DeltaStepAxis19_CPS = 0;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;

			GCodeRiPointer->Feedrate = (System.SRefSpeedBack>3) ?  3: System.SRefSpeedBack;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;

			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;

			if(Reference.NeedFindAxisSign.Axis1)
			{
				GCodeRiPointer->EndPoint.Axis1 = GCodeRiPointer->StartPoint.Axis1 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis1;
			}

			#if 0
			if(Reference.NeedFindAxisSign.Axis3)
			{
				GCodeRiPointer->EndPoint.Axis3 = GCodeRiPointer->StartPoint.Axis3 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis3;
			}	
			if(Reference.NeedFindAxisSign.Axis4)
			{
				GCodeRiPointer->EndPoint.Axis4 = GCodeRiPointer->StartPoint.Axis4 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis4;
			}
			if(Reference.NeedFindAxisSign.Axis5)
			{
				GCodeRiPointer->EndPoint.Axis5 = GCodeRiPointer->StartPoint.Axis5 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis5;
			}
			if(Reference.NeedFindAxisSign.Axis6)
			{
				GCodeRiPointer->EndPoint.Axis6 = GCodeRiPointer->StartPoint.Axis6 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis6;
			}
			if(Reference.NeedFindAxisSign.Axis7)
			{
				GCodeRiPointer->EndPoint.Axis7 = GCodeRiPointer->StartPoint.Axis7 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis7;
			}	
			if(Reference.NeedFindAxisSign.Axis8)
			{
				GCodeRiPointer->EndPoint.Axis8 = GCodeRiPointer->StartPoint.Axis8 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis8;
			}
			if(Reference.NeedFindAxisSign.Axis9)
			{
				GCodeRiPointer->EndPoint.Axis9 = GCodeRiPointer->StartPoint.Axis9 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis9;
			}
			if(Reference.NeedFindAxisSign.Axis11)
			{
				GCodeRiPointer->EndPoint.Axis11 = GCodeRiPointer->StartPoint.Axis11 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis11;
			}
			if(Reference.NeedFindAxisSign.Axis12)
			{
				GCodeRiPointer->EndPoint.Axis12 = GCodeRiPointer->StartPoint.Axis12 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis12;
			}
			if(Reference.NeedFindAxisSign.Axis13)
			{
				GCodeRiPointer->EndPoint.Axis13 = GCodeRiPointer->StartPoint.Axis13 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis13;
			}	
			if(Reference.NeedFindAxisSign.Axis14)
			{
				GCodeRiPointer->EndPoint.Axis14 = GCodeRiPointer->StartPoint.Axis14 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis14;
			}
			if(Reference.NeedFindAxisSign.Axis15)
			{
				GCodeRiPointer->EndPoint.Axis15 = GCodeRiPointer->StartPoint.Axis15 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis15;
			}
			if(Reference.NeedFindAxisSign.Axis16)
			{
				GCodeRiPointer->EndPoint.Axis16 = GCodeRiPointer->StartPoint.Axis16 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis16;
			}
			if(Reference.NeedFindAxisSign.Axis17)
			{
				GCodeRiPointer->EndPoint.Axis17 = GCodeRiPointer->StartPoint.Axis17 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis17;
			}	
			if(Reference.NeedFindAxisSign.Axis18)
			{
				GCodeRiPointer->EndPoint.Axis18 = GCodeRiPointer->StartPoint.Axis18 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis18;
			}
			#endif
			
			if(Reference.NeedFindAxisSign.Axis19)
			{
				//GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis19;
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(1,System.UnitTo_mm)*System.RefDir.Axis19;
			}
			#if 0
			if(Reference.NeedFindAxisSign.Axis20)
			{
				GCodeRiPointer->EndPoint.Axis20 = GCodeRiPointer->StartPoint.Axis20 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis20;
			}
			if(Reference.NeedFindAxisSign.Axis21)
			{
				GCodeRiPointer->EndPoint.Axis21 = GCodeRiPointer->StartPoint.Axis21 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis21;
			}
			if(Reference.NeedFindAxisSign.Axis22)
			{
				GCodeRiPointer->EndPoint.Axis22 = GCodeRiPointer->StartPoint.Axis22 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis22;
			}
			if(Reference.NeedFindAxisSign.Axis23)
			{
				GCodeRiPointer->EndPoint.Axis23 = GCodeRiPointer->StartPoint.Axis23 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis23;
			}
			#endif

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			SPCNCSign.CodeRun_Over = 0;
			Reference.RefStep = 7;

			//for test
			if(Reference.NeedFindAxisSign.Axis2)
			{
				CPLD_EncoderResetAxis2();
				Reference.FindRefSign.Axis2 = 2;
			}
			if(Reference.NeedFindAxisSign.Axis10)
			{
				CPLD_EncoderResetAxis10();
				Reference.FindRefSign.Axis10 = 2;
			}
		}
		return 0;

	}
	else if( Reference.RefStep == 7 )
	{
		Read_Servo_Encoder_Reference1_In();
		Read_Servo_Encoder_Reference2_In();

		if(Reference.NeedFindAxisSign.Axis1)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_1)
			{
				if(Reference.FindRefSign.Axis1 == 0)
				{
					CPLD_EncoderResetAxis1();
					Reference.FindRefSign.Axis1 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis3)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_3)
			{
				if(Reference.FindRefSign.Axis3 == 0)
				{
					CPLD_EncoderResetAxis3();
					Reference.FindRefSign.Axis3 = 2;
				}
			}
		}
		
		if(Reference.NeedFindAxisSign.Axis4)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_4)
			{
				if(Reference.FindRefSign.Axis4 == 0)
				{
					CPLD_EncoderResetAxis4();
					Reference.FindRefSign.Axis4 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis5)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_5)
			{
				if(Reference.FindRefSign.Axis5 == 0)
				{
					CPLD_EncoderResetAxis5();
					Reference.FindRefSign.Axis5 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis6)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_6)
			{
				if(Reference.FindRefSign.Axis6 == 0)
				{
					CPLD_EncoderResetAxis6();
					Reference.FindRefSign.Axis6 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis7)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_7)
			{
				if(Reference.FindRefSign.Axis7 == 0)
				{
					CPLD_EncoderResetAxis7();
					Reference.FindRefSign.Axis7 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis8)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_8)
			{
				if(Reference.FindRefSign.Axis8 == 0)
				{
					CPLD_EncoderResetAxis8();
					Reference.FindRefSign.Axis8 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis9)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_9)
			{
				if(Reference.FindRefSign.Axis9 == 0)
				{
					CPLD_EncoderResetAxis9();
					Reference.FindRefSign.Axis9 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis11)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_11)
			{
				if(Reference.FindRefSign.Axis11 == 0)
				{
					CPLD_EncoderResetAxis11();
					Reference.FindRefSign.Axis11 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis12)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_12)
			{
				if(Reference.FindRefSign.Axis12 == 0)
				{
					CPLD_EncoderResetAxis12();
					Reference.FindRefSign.Axis12 = 2;
				}
			}
		}	

		if(Reference.NeedFindAxisSign.Axis13)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_13)
			{
				if(Reference.FindRefSign.Axis13 == 0)
				{
					CPLD_EncoderResetAxis13();
					Reference.FindRefSign.Axis13 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis14)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_14)
			{
				if(Reference.FindRefSign.Axis14 == 0)
				{
					CPLD_EncoderResetAxis14();
					Reference.FindRefSign.Axis14 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis15)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_15)
			{
				if(Reference.FindRefSign.Axis15 == 0)
				{
					CPLD_EncoderResetAxis15();
					Reference.FindRefSign.Axis15 = 2;
				}
			}
		}

	
		if(Reference.NeedFindAxisSign.Axis16)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_16)
			{
				if(Reference.FindRefSign.Axis16 == 0)
				{
					CPLD_EncoderResetAxis16();
					Reference.FindRefSign.Axis16 = 2;
				}
			}
		}	

		if(Reference.NeedFindAxisSign.Axis17)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_17)
			{
				if(Reference.FindRefSign.Axis17 == 0)
				{
					CPLD_EncoderResetAxis17();
					Reference.FindRefSign.Axis17 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis18)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_18)
			{
				if(Reference.FindRefSign.Axis18 == 0)
				{
					CPLD_EncoderResetAxis18();
					Reference.FindRefSign.Axis18 = 2;
				}
			}
		}

		#if 0
		if(Reference.NeedFindAxisSign.Axis19)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_19)
			{
				if(Reference.FindRefSign.Axis19 == 0)
				{
					CPLD_EncoderResetAxis19();
					Reference.FindRefSign.Axis19 = 2;
				}
			}
		}
		#endif

		if(Reference.NeedFindAxisSign.Axis20)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_20)
			{
				if(Reference.FindRefSign.Axis20 == 0)
				{
					CPLD_EncoderResetAxis20();
					Reference.FindRefSign.Axis20 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis21)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_21)
			{
				if(Reference.FindRefSign.Axis21 == 0)
				{
					CPLD_EncoderResetAxis21();
					Reference.FindRefSign.Axis21 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis22)
		{
			if(Servo_Encoder_Reference2_In_Register.bit.ENCODER_R_22)
			{
				if(Reference.FindRefSign.Axis22 == 0)
				{
					CPLD_EncoderResetAxis22();
					Reference.FindRefSign.Axis22 = 2;
				}
			}
		}

		if(Reference.NeedFindAxisSign.Axis23)
		{
			if(Servo_Encoder_Reference1_In_Register.bit.ENCODER_R_23)
			{
				if(Reference.FindRefSign.Axis23 == 0)
				{
					CPLD_EncoderResetAxis23();
					Reference.FindRefSign.Axis23 = 2;
				}
			}
		}

		if( (((Reference.NeedFindAxisSign.Axis1)? ( (Reference.FindRefSign.Axis1 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis2)? ( (Reference.FindRefSign.Axis2 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis3)? ( (Reference.FindRefSign.Axis3 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis4)? ( (Reference.FindRefSign.Axis4 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis5)? ( (Reference.FindRefSign.Axis5 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis6)? ( (Reference.FindRefSign.Axis6 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis7)? ( (Reference.FindRefSign.Axis7 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis8)? ( (Reference.FindRefSign.Axis8 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis9)? ( (Reference.FindRefSign.Axis9 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis10)? ( (Reference.FindRefSign.Axis10 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis11)? ( (Reference.FindRefSign.Axis11 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis12)? ( (Reference.FindRefSign.Axis12 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis13)? ( (Reference.FindRefSign.Axis13 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis14)? ( (Reference.FindRefSign.Axis14 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis15)? ( (Reference.FindRefSign.Axis15 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis16)? ( (Reference.FindRefSign.Axis16 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis17)? ( (Reference.FindRefSign.Axis17 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis18)? ( (Reference.FindRefSign.Axis18 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis19)? ( (Reference.FindRefSign.Axis19 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis20)? ( (Reference.FindRefSign.Axis20 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis21)? ( (Reference.FindRefSign.Axis21 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis22)? ( (Reference.FindRefSign.Axis22 == 2) ? 1:0): 1)) &&
			(((Reference.NeedFindAxisSign.Axis23)? ( (Reference.FindRefSign.Axis23 == 2) ? 1:0): 1)) 
		)
		{
			if((OverallSign.EveryAxisCoinValid == 1)&&(OverallSign.LastCodeOverSign == 1)&&(SPCNCSign.CodeRun_Over==1))
			{
				if(Reference.NeedFindAxisSign.Axis1)
				{
					Reference.FindRefSign.Axis1 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis2)
				{
					Reference.FindRefSign.Axis2 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis3)
				{
					Reference.FindRefSign.Axis3 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis4)
				{
					Reference.FindRefSign.Axis4 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis5)
				{
					Reference.FindRefSign.Axis5 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis6)
				{
					Reference.FindRefSign.Axis6 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis7)
				{
					Reference.FindRefSign.Axis7 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis8)
				{
					Reference.FindRefSign.Axis8 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis9)
				{
					Reference.FindRefSign.Axis9 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis10)
				{
					Reference.FindRefSign.Axis10 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis11)
				{
					Reference.FindRefSign.Axis11 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis12)
				{
					Reference.FindRefSign.Axis12 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis13)
				{
					Reference.FindRefSign.Axis13 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis14)
				{
					Reference.FindRefSign.Axis14 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis15)
				{
					Reference.FindRefSign.Axis15 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis16)
				{
					Reference.FindRefSign.Axis16 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis17)
				{
					Reference.FindRefSign.Axis17 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis18)
				{
					Reference.FindRefSign.Axis18 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis19)
				{
					Reference.FindRefSign.Axis19 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis20)
				{
					Reference.FindRefSign.Axis20 = 1;
				}				

				Reference.RefStep = 20;
				//Reference.RefStep = 8;
				//SPCNCSign.GetPositionSign = 1;
			}

		}

		return 1;
	}
	else if( Reference.RefStep == 8 )
	{
		if( SPCNCSign.GetPositionSign == 0 )
		{
			Reference.RefStep = 20;
		}
		return 0;
	}

	return 1;
}												//参考点搜索结束

//参考点搜索开始区
Uint16 SearchReference_Beta(void) 
{
	if(System.FunctionSelect05 == False)
	{
		return 0;
	}

	if( Reference.SearchRefSign == 0 )
	{
		return 0;
	}

	OverallSign.NCSign = SPCCODERUN;

	if( Reference.RefStep == 0)
	{
		if( (SPCNCSign.GetPositionSign == 0) &&
		(STDNCSign.GetPositionSign == 0) &&
		(NCRunSign.GetPositionSign == 0) )
		{
			GCODE *GCodeRiPointer;
			OverallSign.LensMotionOverSign = LensMotionNormal;
			DeltaStepAxis19_CPS = 0;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;
			
			NC_AREAControl.Pointer_StartCode = 0;
			NC_AREAControl.Pointer_SecondInflexionCode = 0;
			NC_AREAControl.Pointer_EndCode = 0;
			NC_AREAControl.DeltaStep.Axis1 =0;
			NC_AREAControl.DeltaStep.Axis2 =0;
			NC_AREAControl.DeltaStep.Axis3 =0;
			NC_AREAControl.DeltaStep.Axis4=0;
			NC_AREAControl.DeltaStep.Axis5 =0;
			NC_AREAControl.DeltaStep.Axis6 =0;
			NC_AREAControl.DeltaStep.Axis7 =0;
			NC_AREAControl.DeltaStep.Axis8=0;
			NC_AREAControl.DeltaStep.Axis9 =0;
			NC_AREAControl.DeltaStep.Axis10 =0;
			NC_AREAControl.DeltaStep.Axis11 =0;
			NC_AREAControl.DeltaStep.Axis12 =0;
			NC_AREAControl.DeltaStep.Axis13 =0;
			NC_AREAControl.DeltaStep.Axis14=0;
			NC_AREAControl.DeltaStep.Axis15 =0;
			NC_AREAControl.DeltaStep.Axis16 =0;
			NC_AREAControl.DeltaStep.Axis17 =0;
			NC_AREAControl.DeltaStep.Axis18=0;
			NC_AREAControl.DeltaStep.Axis19 =0;
			NC_AREAControl.DeltaStep.Axis20 =0;
			NC_AREAControl.DeltaStep.Axis21 =0;
			NC_AREAControl.DeltaStep.Axis22 =0;
			NC_AREAControl.DeltaStep.Axis23 =0;
			NC_AREAControl.DeltaStep.Axis24=0;
			NC_AREAControl.DeltaStep.Axis25 =0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			SPCNCSign.RealSendCount++;
			SPCNCSign.RealRunCount = SPCNCSign.RealSendCount;
			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->SendCount = 1;
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;
			GCodeRiPointer->SubCMD2 = 0;
			
			GCodeRiPointer->Feedrate = (System.SRefSpeed>10) ?  10: System.SRefSpeed;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;
	
			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;
			
			if(Reference.NeedFindAxisSign.Axis19)
			{
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 + 1000000*System.RefDir.Axis19;
			}

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			Reference.RefStep = 1;
		}

	}
	else if( Reference.RefStep == 1)
	{
		if(Reference.NeedFindAxisSign.Axis19)
		{
			//if(!((System.RefDir.Axis19==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))
			if(Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19 == 0)
			{
				Reference.RefStep = Reference.RefStep;
				return;
			}
		}
		OverallSign.LensMotionOverSign = LensMotionNormal;
		Reference.RefStep = 2;
		SPCNCSign.GetPositionSign = 1;
		return;

	}
	else if( Reference.RefStep == 2)
	{
		if( SPCNCSign.GetPositionSign == 0 )
		{
			GCODE *GCodeRiPointer;
			OverallSign.LensMotionOverSign = LensMotionNormal;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;
			GCodeRiPointer->Feedrate = (System.SRefSpeedBack>3) ?  3: System.SRefSpeedBack;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;
			
			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;

			if(Reference.NeedFindAxisSign.Axis19)
			{
				//GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis19;
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(2,System.UnitTo_mm)*System.RefDir.Axis19;
			}

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			SPCNCSign.CodeRun_Over = 0;
			Reference.RefStep = 3;
		}
		
		return 0;
	}	
	else if( Reference.RefStep == 3)
	{
		Read_Servo_HardLimit_Positive2_In();
		Read_Servo_HardLimit_Negative2_In();

		if(Reference.NeedFindAxisSign.Axis19)
		{
			if(((System.RefDir.Axis19==1)?Servo_HardLimit_Positive2_In_Register.bit.HLIMIT_POS_19 : Servo_HardLimit_Negative2_In_Register.bit.HLIMIT_NEG_19))
			{
				return;
			}
		}
		OverallSign.LensMotionOverSign = LensMotionNormal;
		DeltaStepAxis19_CPS = 0;
		Reference.RefStep = 4;
		SPCNCSign.GetPositionSign = 1;
		//for test
		if(Reference.NeedFindAxisSign.Axis19)
		{
			CPLD_EncoderResetAxis19();
			Reference.FindRefSign.Axis19 = 2;
		}
		return;
	}	
	else if( Reference.RefStep == 4)
	{
		if( SPCNCSign.GetPositionSign == 0 )
		{
			GCODE *GCodeRiPointer;			
			OverallSign.LensMotionOverSign = LensMotionNormal;
			DeltaStepAxis19_CPS = 0;
			NCRunSign.RealSendCount = 1;
			NCRunSign.RealRunCount	= 1;
			NCRunSign.VeVmCalCount = 0;
			NCRunSign.NewAreaSign	= 0;
			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			NCRunSign.CodeRun_Over = 0;
			SPCNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over = 0;
			NC_AREAControl.VNow_Module.Module1 = 0;
			NC_AREAControl.VNow_Module.Module2 = 0;
			NC_AREAControl.VNow_Module.Module3 = 0;
			NC_AREAControl.VNow_Module.Module4 = 0;
			NC_AREAControl.VNow_Module.Module5 = 0;
			NC_AREAControl.VNow_Module.Module6 = 0;
			NC_AREAControl.VNow_Module.Module7 = 0;
			NC_AREAControl.VNow_Module.Module8 = 0;
			NC_AREAControl.VNow_Module.Module9 = 0;
			NC_AREAControl.VNow_Module.Module10 = 0;
			NC_AREAControl.VNow_Module.Module11 = 0;
			NC_AREAControl.VNow_Module.Module12 = 0;
			NC_AREAControl.Snow_Module.Module1 = 0;
			NC_AREAControl.Snow_Module.Module2 = 0;
			NC_AREAControl.Snow_Module.Module3 = 0;
			NC_AREAControl.Snow_Module.Module4 = 0;
			NC_AREAControl.Snow_Module.Module5 = 0;
			NC_AREAControl.Snow_Module.Module6 = 0;
			NC_AREAControl.Snow_Module.Module7 = 0;
			NC_AREAControl.Snow_Module.Module8 = 0;
			NC_AREAControl.Snow_Module.Module9 = 0;
			NC_AREAControl.Snow_Module.Module10 = 0;
			NC_AREAControl.Snow_Module.Module11 = 0;
			NC_AREAControl.Snow_Module.Module12 = 0;

			//清插补缓冲区
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;

			GCodeRiPointer = &SPCGCodeBuffer[(SPCNCSign.RealRunCount-1)%SPCGCODE_MOD];
			GCodeRiPointer->CMDMain = 1;
			GCodeRiPointer->SubCMD1 = 0;

			GCodeRiPointer->Feedrate = (System.SRefSpeedBack>3) ?  3: System.SRefSpeedBack;
			GCodeRiPointer->Feedrate_Module.Module1 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module2 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module3 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module4 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module5 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module6 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module7 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module8 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module9 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module10 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module11 = GCodeRiPointer->Feedrate;
			GCodeRiPointer->Feedrate_Module.Module12 = GCodeRiPointer->Feedrate;

			GCodeRiPointer->StartPoint = OverallSign.ABS_Coordinate;
			GCodeRiPointer->EndPoint = GCodeRiPointer->StartPoint;
			
			if(Reference.NeedFindAxisSign.Axis19)
			{
				//GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(System.SRefBack,System.UnitTo_mm)*System.RefDir.Axis19;
				GCodeRiPointer->EndPoint.Axis19 = GCodeRiPointer->StartPoint.Axis19 - divdp(1,System.UnitTo_mm)*System.RefDir.Axis19;
			}

			SPC_Deal_Line(GCodeRiPointer);

			NCAreaGCodeBuffer[ (NCRunSign.RealRunCount-1)%STDGCODE_MOD ]
			= SPCGCodeBuffer[ (SPCNCSign.RealRunCount-1)%SPCGCODE_MOD ];

			NCRunSign.NewAreaSign = 0;
			NCRunSign.PrecalCode = 0;
			SPCNCSign.CodeRun_Over = 0;
			Reference.RefStep = 5;
		}
		return 0;

	}	
	else if( Reference.RefStep == 5 )
	{
		if(((Reference.NeedFindAxisSign.Axis19)? ( (Reference.FindRefSign.Axis19 == 2) ? 1:0): 1))
		{
			if((OverallSign.EveryAxisCoinValid == 1)&&(OverallSign.LastCodeOverSign == 1)&&(SPCNCSign.CodeRun_Over==1))
			{
				if(Reference.NeedFindAxisSign.Axis1)
				{
					Reference.FindRefSign.Axis1 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis2)
				{
					Reference.FindRefSign.Axis2 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis3)
				{
					Reference.FindRefSign.Axis3 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis4)
				{
					Reference.FindRefSign.Axis4 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis5)
				{
					Reference.FindRefSign.Axis5 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis6)
				{
					Reference.FindRefSign.Axis6 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis7)
				{
					Reference.FindRefSign.Axis7 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis8)
				{
					Reference.FindRefSign.Axis8 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis9)
				{
					Reference.FindRefSign.Axis9 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis10)
				{
					Reference.FindRefSign.Axis10 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis11)
				{
					Reference.FindRefSign.Axis11 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis12)
				{
					Reference.FindRefSign.Axis12 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis13)
				{
					Reference.FindRefSign.Axis13 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis14)
				{
					Reference.FindRefSign.Axis14 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis15)
				{
					Reference.FindRefSign.Axis15 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis16)
				{
					Reference.FindRefSign.Axis16 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis17)
				{
					Reference.FindRefSign.Axis17 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis18)
				{
					Reference.FindRefSign.Axis18 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis19)
				{
					Reference.FindRefSign.Axis19 = 1;
				}
				if(Reference.NeedFindAxisSign.Axis20)
				{
					Reference.FindRefSign.Axis20 = 1;
				}				

				Reference.RefStep = 20;
				//Reference.RefStep = 8;
				//SPCNCSign.GetPositionSign = 1;
			}

		}

		return 1;
	}		

	return 1;
}												//参考点搜索结束


// 错误判断程续
Uint16 get_error_data(void)	//获得出错时的数据
{
	return 1;
}

//代码运行开始区CH0701
Uint16 NC_Run_Gcode(void) 
{
	Uint16 i = 0;
	static Uint16 idlecnt = 30;
	GCODE *iGcode;

	//NCAreaPulseToServo_LPMEN1();

	if ((SPCNCSign.GetPositionSign == 1) || (STDNCSign.GetPositionSign == 1)
			|| (NCRunSign.GetPositionSign == 1))	// 需要获得稳定的坐标值
	{
		if (OverallSign.EveryAxisCoinValid) 
		{
			if (get_delta_time(Timer0.GetPositionCounter) > 100)		//500ms
			{
				SPCNCSign.CodeRun_Over = 1;
				STDNCSign.CodeRun_Over = 1;
				NCRunSign.CodeRun_Over = 1;
				if (get_and_check_coordinate() == 0) 
				{//未获得正确坐标, 重新获取
					Timer0.GetPositionCounter = Timer0.GenericCounter;
					return 0;
				}
				SPCNCSign.GetPositionSign = 0;
				STDNCSign.GetPositionSign = 0;
				NCRunSign.GetPositionSign = 0;
				OverallSign.SPCLastMoveGCode.EndPoint =
						OverallSign.ABS_Coordinate;		//um

			}
		} 
		else
			Timer0.GetPositionCounter = Timer0.GenericCounter;
		
		return ((Uint16) 1);
	}

	if (1)	//脉冲控制型
	{//如果是脉冲控制类型，则直接使能中断执行写脉冲操作
		if ((OverallSign.Int4IdleSign_1 > idlecnt)
				& (OverallSign.InterpolationRunCnt_1
						< OverallSign.InterpolationSendCnt_1)) 
		{
			IRQ_ENABLE(IRQNUM_PLUSE1);
			IRQ_ENABLE(IRQNUM_PLUSE3);
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<22);
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1<<22);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = (0x1 << 17);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0) = (0x1 << 17);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_1) = (0x1 << 17);
			*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_1) = (0x1 << 17);
		}
		if ((OverallSign.Int4IdleSign_2 > idlecnt)
				& (OverallSign.InterpolationRunCnt_2
						< OverallSign.InterpolationSendCnt_2)) 
		{
			IRQ_ENABLE(IRQNUM_PLUSE2);
			IRQ_ENABLE(IRQNUM_PLUSE4);
			*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1 << 23);
			*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1 << 23);
			*(GPIO_IRQStatus.GPIO0_IRQStatus_1) = (0x1 << 23);
			*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_1) = (0x1 << 23);
		}
	}

	if (OverallSign.NCSign == STDCODERUN) 
	{//确认STD中断脉冲代码条数不过载
		if ((OverallSign.InterpolationSendCnt_1
				- OverallSign.InterpolationRunCnt_1)
				> (INTERPOLATIONBUFLENGTH - 5)) 
		{
			OverallSign.Int4IdleSign_2 = OverallSign.Int4IdleSign_2;
			return 0;
		}
		if ((OverallSign.InterpolationSendCnt_2
				- OverallSign.InterpolationRunCnt_2)
				> (INTERPOLATIONBUFLENGTH - 5)) 
		{
			OverallSign.Int4IdleSign_2 = OverallSign.Int4IdleSign_2;
			return 0;
		}
	}
	else if (OverallSign.NCSign == SPCCODERUN) 
	{//确认SPC中断脉冲代码条数不过载
		if ((OverallSign.InterpolationSendCnt_1
				- OverallSign.InterpolationRunCnt_1)
				> SPC_INTERPOLATIONBUFLENGTH) 
		{
			OverallSign.Int4IdleSign_2 = OverallSign.Int4IdleSign_2;
			return 0;
		}
		if ((OverallSign.InterpolationSendCnt_2
				- OverallSign.InterpolationRunCnt_2)
				> SPC_INTERPOLATIONBUFLENGTH) 
		{
			OverallSign.Int4IdleSign_2 = OverallSign.Int4IdleSign_2;
			return 0;
		}
	}

#if 0
	if( OverallSign.NCSign == SPCCODERUN )//SPC功能启动等待标志,防止启动时频繁加减速，造成震动
	{
		if(get_delta_time(Timer0.SPCStartCounter) < 60)		//60ms
		return 0;
	}
#endif
//STDNCSign.CodeRun_Over STD代码执行完成标志位 1：完成 0：可执行
	if ((OverallSign.NCSign == STDCODERUN) && (STDNCSign.CodeRun_Over))	//STD代码完成
	{
		OverallSign.LastCodeOverSign = 0;

		if (STDNCSign.RealRunCount == STDNCSign.RealSendCount) //执行完STD G代码 条数暂时相等；返回错误
		{
			if (NCRunSign.RealRunCount != NCRunSign.RealSendCount) 
			{
				NCRunSign.RealRunCount = NCRunSign.RealRunCount;
				NCRunSign.RealSendCount = NCRunSign.RealSendCount;
				Error.InterpolationOver = 1;
				high_error();
			}

			if (OverallSign.EveryAxisCoinValid) 
			{
				NC_AREAControl.VNow_Module.Module1 = 0;
				NC_AREAControl.VNow_Module.Module2 = 0;
				NC_AREAControl.VNow_Module.Module3 = 0;
				NC_AREAControl.VNow_Module.Module4 = 0;
				NC_AREAControl.VNow_Module.Module5 = 0;
				NC_AREAControl.VNow_Module.Module6 = 0;
				NC_AREAControl.VNow_Module.Module7 = 0;
				NC_AREAControl.VNow_Module.Module8 = 0;
				NC_AREAControl.VNow_Module.Module9 = 0;
				NC_AREAControl.VNow_Module.Module10 = 0;
				NC_AREAControl.VNow_Module.Module11 = 0;
				NC_AREAControl.VNow_Module.Module12 = 0;
				OverallSign.LastCodeOverSign = 1; 	//CH0507
			}

			return FALSE;
		} 
		else if (STDNCSign.RealSendCount < STDNCSign.RealRunCount) 
		{
			STDNCSign.RealRunCount = STDNCSign.RealRunCount;
			//return 0;
		} 
		else if (STDNCSign.RealSendCount > STDNCSign.RealRunCount) //有新代码进来，在B3包传入参数带有STDNCSign.RealSendCount 每次递增
		{
			STDNCSign.RealRunCount++;
			STDNCSign.PrecalCode = 0;	//执行新代码,需重新计算
			STDNCSign.CodeRun_Over = 0;
			STDNCSign.CodeRun_Over_Module.Module1 = 0;
			STDNCSign.CodeRun_Over_Module.Module2 = 0;
			STDNCSign.CodeRun_Over_Module.Module3 = 0;
			STDNCSign.CodeRun_Over_Module.Module4 = 0;
			STDNCSign.CodeRun_Over_Module.Module5 = 0;
			STDNCSign.CodeRun_Over_Module.Module6 = 0;
			STDNCSign.CodeRun_Over_Module.Module7 = 0;
			STDNCSign.CodeRun_Over_Module.Module8 = 0;
			STDNCSign.CodeRun_Over_Module.Module9 = 0;
			STDNCSign.CodeRun_Over_Module.Module10 = 0;
			STDNCSign.CodeRun_Over_Module.Module11 = 0;
			STDNCSign.CodeRun_Over_Module.Module12 = 0;
			

			if (GCodeBuffer[(STDNCSign.RealRunCount - 1) % STDGCODE_MOD].MoveCodeSign
					!= 0) 
			{
				NCRunSign.RealRunCount++;
				NCRunSign.PrecalCode = 0;
				NCRunSign.CodeRun_Over = 0;
				NCRunSign.CodeRun_Over_Module.Module1 = 0;
				NCRunSign.CodeRun_Over_Module.Module2 = 0;
				NCRunSign.CodeRun_Over_Module.Module3 = 0;
				NCRunSign.CodeRun_Over_Module.Module4 = 0;
				NCRunSign.CodeRun_Over_Module.Module5 = 0;
				NCRunSign.CodeRun_Over_Module.Module6 = 0;
				NCRunSign.CodeRun_Over_Module.Module7 = 0;
				NCRunSign.CodeRun_Over_Module.Module8 = 0;
				NCRunSign.CodeRun_Over_Module.Module9 = 0;
				NCRunSign.CodeRun_Over_Module.Module10 = 0;
				NCRunSign.CodeRun_Over_Module.Module11 = 0;
				NCRunSign.CodeRun_Over_Module.Module12 = 0;
			}

		}

		return TRUE;
	}
	else if ((OverallSign.NCSign == SPCCODERUN) && (SPCNCSign.CodeRun_Over))//SPC代码完成
	{
		OverallSign.LastCodeOverSign = 0;
		if (SPCNCSign.RealRunCount == SPCNCSign.RealSendCount) 
		{
			if (OverallSign.EveryAxisCoinValid) 
			{
				NC_AREAControl.VNow_Module.Module1 = 0;
				NC_AREAControl.VNow_Module.Module2 = 0;
				NC_AREAControl.VNow_Module.Module3 = 0;
				NC_AREAControl.VNow_Module.Module4 = 0;
				NC_AREAControl.VNow_Module.Module5 = 0;
				NC_AREAControl.VNow_Module.Module6 = 0;
				NC_AREAControl.VNow_Module.Module7 = 0;
				NC_AREAControl.VNow_Module.Module8 = 0;
				NC_AREAControl.VNow_Module.Module9 = 0;
				NC_AREAControl.VNow_Module.Module10 = 0;
				NC_AREAControl.VNow_Module.Module11 = 0;
				NC_AREAControl.VNow_Module.Module12 = 0;
				OverallSign.LastCodeOverSign = 1;
			}
			return ((Uint16) 0);
		}
		SPCNCSign.RealRunCount++;
		SPCNCSign.PrecalCode = 0;	//执行新代码,需重新计算
		SPCNCSign.CodeRun_Over = 0; //新代码进入，在确定上一条G代码执行完毕之后将标志位置回0
		SPCNCSign.CodeRun_Over_Module.Module1 = 0;
		SPCNCSign.CodeRun_Over_Module.Module2 = 0;
		SPCNCSign.CodeRun_Over_Module.Module3 = 0;
		SPCNCSign.CodeRun_Over_Module.Module4 = 0;
		SPCNCSign.CodeRun_Over_Module.Module5 = 0;
		SPCNCSign.CodeRun_Over_Module.Module6 = 0;
		SPCNCSign.CodeRun_Over_Module.Module7 = 0;
		SPCNCSign.CodeRun_Over_Module.Module8 = 0;
		SPCNCSign.CodeRun_Over_Module.Module9 = 0;
		SPCNCSign.CodeRun_Over_Module.Module10 = 0;
		SPCNCSign.CodeRun_Over_Module.Module11 = 0;
		SPCNCSign.CodeRun_Over_Module.Module12 = 0;
		NCRunSign.RealRunCount++;
		NCRunSign.PrecalCode = 0;
		NCRunSign.CodeRun_Over = 0;
		NCRunSign.CodeRun_Over_Module.Module1 = 0;
		NCRunSign.CodeRun_Over_Module.Module2 = 0;
		NCRunSign.CodeRun_Over_Module.Module3 = 0;
		NCRunSign.CodeRun_Over_Module.Module4 = 0;
		NCRunSign.CodeRun_Over_Module.Module5 = 0;
		NCRunSign.CodeRun_Over_Module.Module6 = 0;
		NCRunSign.CodeRun_Over_Module.Module7 = 0;
		NCRunSign.CodeRun_Over_Module.Module8 = 0;
		NCRunSign.CodeRun_Over_Module.Module9 = 0;
		NCRunSign.CodeRun_Over_Module.Module10 = 0;
		NCRunSign.CodeRun_Over_Module.Module11 = 0;
		NCRunSign.CodeRun_Over_Module.Module12 = 0;
	}

	if ((SPCNCSign.CodeRun_Over == 0) || (STDNCSign.CodeRun_Over == 0))	//新代码执行区
	{
		if (OverallSign.NCSign == STDCODERUN) 
		{
			GCODE *iGcode;
			iGcode = &GCodeBuffer[(STDNCSign.RealRunCount - 1) % STDGCODE_MOD];

			switch (iGcode->CMDMain) 
			{

				case 0:
					goto INTO_NCAREA;

				case 1:
					goto INTO_NCAREA;

				case 2:
					goto INTO_NCAREA;
				case 3:
					goto INTO_NCAREA;
				case 4:
					STD_run_G04();
					break;
				case 9:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 17:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 18:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 19:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 30:
					goto INTO_NCAREA;
				case 31:
					goto INTO_NCAREA;
				case 41:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 42:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 43:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 51:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 54:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 55:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 56:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 57:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 58:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 59:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 61:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 64:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 80:
					STDNCSign.CodeRun_Over = 1;
					break;
				case 100:
					STD_run_G100();
					break;
				case 101:
					STD_run_G101();
					break;
				case 102:
					STD_run_G102();
					break;
				case 103:
					STD_run_G103();
					break;
				case 104:
					STD_run_G104();
					break;
				case 105:
					STD_run_G105();
					break;
				case 106:
					STD_run_G106();
					break;
				case 107:
					STD_run_G107();
					break;
				case 108:
					STD_run_G108();
					break;
				case 109:
					STD_run_G109();
					break;
				case 110:
					STD_run_G110();
					break;
				case 111:
					STD_run_G111();
					break;
				case 112:
					STD_run_G112();
					break;
				case 113:
					STD_run_G113();
					break;					
				case 114:
					STD_run_G114();
					break;	

				default:
					STDNCSign.CodeRun_Over = 1;
					break;
				}

			return TRUE;
		}
		else if (OverallSign.NCSign == SPCCODERUN) 
		{
			//把代码打入代码缓冲区
			goto INTO_NCAREA;
		}

		INTO_NCAREA: NCAreaRun(&NC_AREAControl, NCAreaGCodeBuffer, &NCRunSign);

	}

	return 1;
}

//其它开始区

//其它结束区

