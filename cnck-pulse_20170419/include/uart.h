#ifndef _UART_H_
#define _UART_H_

#include "datatype.h"

#define mUART1_RXBUFFERSIZE 	(128)
#define mUART1_TXBUFFERSIZE	(128)
#define mUART2_RXBUFFERSIZE 	(128)
#define mUART2_TXBUFFERSIZE	(128)
#define mUART3_RXBUFFERSIZE 	(128)
#define mUART3_TXBUFFERSIZE	(128)

//for spindle module
#define rUARTRX1    	(*(volatile unsigned char *)0X48022000)
#define rUARTTX1    	(*(volatile unsigned char *)0X48022000)
#define rUARTDLL1    	(*(volatile unsigned char *)0X48022000)
#define rUARTDLH1    	(*(volatile unsigned char *)0X48022004)
#define rUARTEFR1    	(*(volatile unsigned char *)0X48022008)
#define rUARTFCR1   	(*(volatile unsigned char *)0X48022008)
#define rUARTIIR1    	(*(volatile unsigned char *)0X48022008)
#define rUARTLCR1    	(*(volatile unsigned char *)0X4802200C)
#define rUARTMCR1    (*(volatile unsigned char *)0X48022010)
#define rUARTLSR1	(*(volatile unsigned char *)0X48022014)
#define rUARTSYSC1	(*(volatile unsigned char *)0X48022054)
#define rUARTSYSS1	(*(volatile unsigned char *)0X48022058)

//for IO module
#define rUARTRX2    	(*(volatile unsigned char *)0X48024000)
#define rUARTTX2    	(*(volatile unsigned char *)0X48024000)
#define rUARTDLL2    	(*(volatile unsigned char *)0X48024000)
#define rUARTDLH2    	(*(volatile unsigned char *)0X48024004)
#define rUARTEFR2    	(*(volatile unsigned char *)0X48024008)
#define rUARTFCR2   	(*(volatile unsigned char *)0X48024008)
#define rUARTIIR2    	(*(volatile unsigned char *)0X48024008)
#define rUARTLCR2    	(*(volatile unsigned char *)0X4802400C)
#define rUARTMCR2    (*(volatile unsigned char *)0X48024010)
#define rUARTLSR2	(*(volatile unsigned char *)0X48024014)
#define rUARTSYSC2	(*(volatile unsigned char *)0X48024054)
#define rUARTSYSS2	(*(volatile unsigned char *)0X48024058)

//for keyboard module
#define rUARTRX3    	(*(volatile unsigned char *)0X481A6000)
#define rUARTTX3    	(*(volatile unsigned char *)0X481A6000)
#define rUARTDLL3    	(*(volatile unsigned char *)0X481A6000)
#define rUARTDLH3    	(*(volatile unsigned char *)0X481A6004)
#define rUARTEFR3    	(*(volatile unsigned char *)0X481A6008)
#define rUARTFCR3   	(*(volatile unsigned char *)0X481A6008)
#define rUARTIIR3    	(*(volatile unsigned char *)0X481A6008)
#define rUARTLCR3    	(*(volatile unsigned char *)0X481A600C)
#define rUARTMCR3    (*(volatile unsigned char *)0X481A6010)
#define rUARTLSR3	(*(volatile unsigned char *)0X481A6014)
#define rUARTSYSC3	(*(volatile unsigned char *)0X481A6054)
#define rUARTSYSS3	(*(volatile unsigned char *)0X481A6058)

#define rUART485EN	(*(volatile unsigned int *)0x4804c13c)

#define mfUARTRX1_EMPTY()			((rUARTLSR1 & 0x3)==0)
#define mfUARTTX1_EMPTY()         		((rUARTLSR1 & 0x20))
#define mfUARTRX1_ERR()              	((rUARTLSR1 & 0x80))
#define mfUARTFIFO1_CLEAR()    		(rUARTFCR1 = (rUARTFCR1 | 0x6))
#define mfUARTTX1_FIFOCLEAR()    	(rUARTFCR1 = (rUARTFCR1 | 0x4))
#define mfUARTRX1_FIFOCLEAR()    	(rUARTFCR1 = (rUARTFCR1 | 0x2))

#define mfUARTRX2_EMPTY()			((rUARTLSR2 & 0x3)==0)
#define mfUARTTX2_EMPTY()         		((rUARTLSR2 & 0x20))
#define mfUARTRX2_ERR()              	((rUARTLSR2 & 0x80))
#define mfUARTFIFO2_CLEAR()    		(rUARTFCR2 = (rUARTFCR2 | 0x6))
#define mfUARTTX2_FIFOCLEAR()    	(rUARTFCR2 = (rUARTFCR2 | 0x4))
#define mfUARTRX2_FIFOCLEAR()    	(rUARTFCR2 = (rUARTFCR2 | 0x2))

#define mfUARTRX3_EMPTY()			((rUARTLSR3 & 0x3)==0)
#define mfUARTTX3_EMPTY()         		((rUARTLSR3 & 0x20))
#define mfUARTRX3_ERR()              	((rUARTLSR3 & 0x80))
#define mfUARTFIFO3_CLEAR()    		(rUARTFCR3 = (rUARTFCR3 | 0x6))
#define mfUARTTX3_FIFOCLEAR()    	(rUARTFCR3 = (rUARTFCR3 | 0x4))
#define mfUARTRX3_FIFOCLEAR()    	(rUARTFCR3 = (rUARTFCR3 | 0x2))
  
extern INT32U Uart_RxCountKEY;
extern INT32U Uart_RxCountIO;
extern INT32U Uart_RxCountSPD;

extern INT32U Uart1_TxIndex;
extern INT32U Uart2_TxIndex;
extern INT32U Uart3_TxIndex;
extern INT32U Uart1_RxIndex;
extern INT32U Uart2_RxIndex;
extern INT32U Uart3_RxIndex;

extern INT32U Uart1_TxTotal;
extern INT32U Uart2_TxTotal;
extern INT32U Uart3_TxTotal;

extern INT32U Uart1_RxTotal;
extern INT32U Uart2_RxTotal;
extern INT32U Uart3_RxTotal;

extern INT8U Uart1_TxBuf[mUART1_TXBUFFERSIZE];
extern INT8U Uart2_TxBuf[mUART2_TXBUFFERSIZE];
extern INT8U Uart3_TxBuf[mUART3_TXBUFFERSIZE];
extern INT8U Uart1_RxBuf[mUART1_RXBUFFERSIZE];
extern INT8U Uart2_RxBuf[mUART2_RXBUFFERSIZE];
extern INT8U Uart3_RxBuf[mUART3_RXBUFFERSIZE];


void Uart_Init(void);

#endif

