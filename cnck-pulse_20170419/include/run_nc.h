#ifndef DSP6713_RUN_NC_H
#define DSP6713_RUN_NC_H

#define IRQNUM_PLUSE   10   // pluse 
#define IRQNUM_MBUS    11   //M2 M3 BUS
#define IRQNUM_PWR     12   //power down

#define IRQNUM_PLUSE1   	10   // LPMEN1 
#define IRQNUM_PLUSE2    	11   // LPMEN2
#define IRQNUM_PLUSE3     	12   // LPMEN3
#define IRQNUM_PLUSE4     	13   // LPMEN4

//#define IRQ_DISABLE(num)  Hwi_disableInterrupt(num)
//#define IRQ_ENABLE(num)   Hwi_enableInterrupt(num)

#define	  FPGA_MAX_F		64000000/2		//FPGA鏃堕挓棰戠巼

//通用变量和标志
extern SYSTEM			System;			//系统参数结构对象
extern ERROR				Error;			//报警结构对象
extern OVERALLSIGN		OverallSign;		// 总体标志
extern DACONVERT  		DA_Convert1;

extern GCODE			GCodeBuffer[STDGCODE_MOD];	// G-code 结构缓冲数组
extern GCODE			SPCGCodeBuffer[SPCGCODE_MOD];	//SPC G-code 结构缓冲数组
extern GCODE			NCAreaGCodeBuffer[STDGCODE_MOD];	//区域计算代码缓冲数组
extern GCODE			AddLineGCodeBuffer_Incubator[ADDGCODEINCUBATOR_MOD];			//增加20条代码用于培养箱定位
extern UINT16AXIS 	LastPrecalMoveSign;	// 以前的移动方向
extern UINT16AXIS 	NowPrecalMoveSign;	// 现在的移动方向

extern NC_RUN_SIGN		NCRunSign;	//区域代码各组群状态和通讯状态结构对象
extern NC_RUN_SIGN 		AddGCodePositionSign;		//用于incubator position加代码;

//用于标准G代码
extern NC_RUN_SIGN		STDNCSign;			//NC代码运行标志和变量
extern NC_RUN_SIGN		SPCNCSign;			//NC代码运行标志和变量

//用于主轴
//参考点搜索
extern REFERENCE	Reference;		

extern AxisCompensation AxisCompBuffer[COMPBUFFERLENGTH];


//用于标准 STD G代码
void 	STD_run_G04(void);
void 	STD_run_G100(void);		//处理F AND S 代码
void 	STD_run_G101(void);		//处理M代码
void 	STD_run_G102(void);		//输入口检测
void 	STD_run_G103(void);		//输出口控制
void 	STD_run_G104(void);		//荧光控制
void 	STD_run_G105(void);		//成相扫描起始点坐标更新
void 	STD_run_G106(void);		//电压控制
void 	STD_run_G107(void);		//更新闪光灯时间
void 	STD_run_G108(void);		//装载、卸载tip头
void 	STD_run_G109(void);		//移液泵移液、放液
void 	STD_run_G110(void);		//离心机控制
void 	STD_run_G111(void);		//振动加热模块
void 	STD_run_G112(void);		//电磁脉冲振荡
void 	STD_run_G113(void) ;		//AOTF 控制
void 	STD_run_G114(void) ;		//PMT


Uint16 STD_run_M03(void);		// 条码扫描
Uint16 STD_run_M04(void);		// sensor1
Uint16 STD_run_M05(void);		// sensor2
Uint16 STD_run_M06(void);		// sensor3

Uint16 get_and_check_coordinate(void);//CH0701  获得定点坐标并多次采样判定
extern INT40AXIS ABSCoordinate_to_MachineCoordinate(INT40AXIS AbsCoordinate);		//CH0701
extern INT40AXIS MachineCoordinate_to_ABSCoordinate(INT40AXIS MachineCoordinate);			//CH0701


//运动控制子程序区
Uint16 LimitAxisSpeed(AREA_CONTROL *AREAControl,GCODE *AreaGcodeBufferPointer, NC_RUN_SIGN *AreaRunNCSign);		//速度限定判定程序  CH0605

//参考点搜索开始区
extern Uint16 SearchReference(void);
extern Uint16 SearchReference_Beta(void);


// 错误判断程序开始区
extern Uint16 STD_check_track_run_over(void);		//判断轨迹是否偏离位置  
extern Uint16 SPC_check_track_run_over(void);		//判断轨迹是否偏离位置  
extern Uint16 get_error_data(void);	//获得出错时的数据

extern Uint16 NC_Run_Gcode(void);	//运行代码


#endif  // end of DSP6713_RUN_NC_H definition


