//****************************************************************************************
//** 文件名：Uart.c

//** Copyright (c) 2014 HS CNC design team

//** 创建人：REX

//** 日期：2014.11.05

//** 描述：Uart drivers
//**
//** 版本：1.0
//**************************************************************************************
#include "..\\include\\uart.h"
/*
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
*/
 
INT32U Uart_RxCountKEY;  
INT32U Uart_RxCountSPD;
INT32U Uart_RxCountIO;

INT32U Uart1_TxIndex;
INT32U Uart2_TxIndex;
INT32U Uart3_TxIndex;

INT32U Uart1_RxIndex;
INT32U Uart2_RxIndex;
INT32U Uart3_RxIndex;

INT32U Uart1_TxTotal;
INT32U Uart2_TxTotal;
INT32U Uart3_TxTotal;
INT32U Uart1_RxTotal;
INT32U Uart2_RxTotal;
INT32U Uart3_RxTotal;

INT8U Uart1_TxBuf[mUART1_TXBUFFERSIZE];
INT8U Uart2_TxBuf[mUART2_TXBUFFERSIZE];
INT8U Uart3_TxBuf[mUART3_TXBUFFERSIZE];
INT8U Uart1_RxBuf[mUART1_RXBUFFERSIZE];
INT8U Uart2_RxBuf[mUART2_RXBUFFERSIZE];
INT8U Uart3_RxBuf[mUART3_RXBUFFERSIZE];

//*****************************************************************
//** 函 数:	void Uart1_Init(void)
//** 输 入:	Null
//** 输 出:	Null
//** 功 能:	初始化Uart1
//** 全局变量:Null
//*****************************************************************
void Uart1_Init(void)
{
	//init uart1 to 500k  for spindle
	rUARTLCR1=0xBF;
	rUARTEFR1=0x10;	
	// 6:1mhz baud rate
	rUARTDLL1=0x6;
	rUARTDLH1=0x0;
	//n; 1 stop; 8 bit
	rUARTLCR1=((0<<3)|(1<<2)|(3));
	//clear fifo here  
	rUARTFCR1=((1<<6)|(1<<4)|(1<<2)|(1<<1)|(1));	
	//disable moderm
	rUARTMCR1=0;	
}

//*****************************************************************
//** 函 数:	void Uart2_Init(void)
//** 输 入:	Null
//** 输 出:	Null
//** 功 能:	初始化Uart2
//** 全局变量:Null
//*****************************************************************
void Uart2_Init(void)
{	
	//init uart3 to 1m for IO module
	rUARTLCR2=0xBF;
	rUARTEFR2=0x10;	
	// 3:1mhz baud rate
	rUARTDLL2=0x3;
	rUARTDLH2=0x0;
	//n; 1 stop; 8 bit
	rUARTLCR2=((0<<3)|(1<<2)|(3));
	//clear fifo here  
	rUARTFCR2=((1<<6)|(1<<4)|(1<<2)|(1<<1)|(1));	
	//disable moderm
	rUARTMCR2=0;	
}

//*****************************************************************
//** 函 数:	void Uart3_Init(void)
//** 输 入:	Null
//** 输 出:	Null
//** 功 能:	初始化Uart3
//** 全局变量:Null
//*****************************************************************
void Uart3_Init(void)
{
	//init uart3 to 1m for keyboard
	rUARTLCR3=0xBF;
	rUARTEFR3=0x10;	
	// 3:1mhz baud rate
	rUARTDLL3=0x3;
	rUARTDLH3=0x0;
	//n; 1 stop; 8 bit
	rUARTLCR3=((0<<3)|(1<<2)|(3));
	//clear fifo here  
	rUARTFCR3=((1<<6)|(1<<4)|(1<<2)|(1<<1)|(1));	
	//disable moderm
	rUARTMCR3=0;	
}

//*****************************************************************
//** 函 数:	void Uart_init(void)
//** 输 入:	Null
//** 输 出:	Null
//** 功 能:	设置串口的波特率
//** 全局变量:Null
//*****************************************************************
void Uart_Init(void)
{
	Uart1_Init();
	Uart2_Init();
	Uart3_Init();
}

