#ifndef __SPI_H__
#define __SPI_H__
#include "datatype.h"

// define registers

#define mSPI_BASE_ADDRESS   	0X481A2000
//SPI2 SUPORT REG  0X481A3000

#define rSPI_CM_ALLOW          	(*(volatile unsigned int *)0X48181590)

#define rSPI_HW_IPVESION   		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X0))
#define rSPI_HW_INFO       		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X4))
#define rSPI_IP_SYSCONFIG  		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X10))

#define rSPI_REVESION     		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X100))
#define rSPI_SYSCONFIG    		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X110))
#define rSPI_SYSSTATUS    		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X114))
#define rSPI_IRQSTATUS	 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X118))
#define rSPI_IRQENABLE    		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X11C)) 
#define rSPI_WAKEUPENABLE 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X120))
#define rSPI_SYST 		 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X124))
#define rSPI_MODULCTRL    		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X128))

#define rSPI_CH1CONF 	 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X140))
#define rSPI_CH1STAT 	 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X144))
#define rSPI_CH1CTRL 	 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X148))
#define rSPI_TX1 		 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X14C))
#define rSPI_RX1 		 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X150))

#define rSPI_XFERLEVEL 	 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X17C))
#define rSPI_DAFTX  		 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X180))
#define rSPI_DAFRX 		 		(*(volatile unsigned int *)(mSPI_BASE_ADDRESS+0X1A0))

// Flash opcodes. 
#define mSPI_OPCODE_WREN			0x06	// Write enable 
#define mSPI_OPCODE_RDSR			0x05	// Read status register 
#define mSPI_OPCODE_WRSR			0x01	// Write status register 1 byte 
#define mSPI_OPCODE_NORM_READ	0x03	// Read data bytes (low frequency) 
#define mSPI_OPCODE_FAST_READ	0x0b	// Read data bytes (high frequency) 
#define mSPI_OPCODE_WRITE			0x02	// Page program (up to 256 bytes) 
#define mSPI_OPCODE_BE_4K			0x20	// Erase 4KiB block 
#define mSPI_OPCODE_BE_32K		0x52	// Erase 32KiB block 
#define mSPI_OPCODE_CHIP_ERASE	0xc7	// Erase whole flash chip 
#define mSPI_OPCODE_SE				0xd8	// Sector erase (usually 64KiB) 
#define mSPI_OPCODE_RDID			0x9f	// Read JEDEC ID 
#define mSPI_OPCODE_SNR    			0XC3   	// Read S/N 

#define mSPI_MAX_WAIT_LOP  1000000


#define mfSPI_START_CHANNEL1  	(rSPI_CH1CTRL|=1)
#define mfSPI_STOP_CHANEL1    	(rSPI_CH1CTRL&=0XFFFFFFFE)
#define mfSPI_RX_IS_FULL      	((rSPI_CH1STAT)&(1<<6))
#define mfSPI_RX_IS_EMPTY     	((rSPI_CH1STAT)&(1<<5))
#define mfSPI_TX_IS_FULL      		((rSPI_CH1STAT)&(1<<4))
#define mfSPI_TX_IS_EMPTY     	((rSPI_CH1STAT)&(1<<3))
#define mfSPI_TRANS_ENDED    	((rSPI_CH1STAT)&(1<<2))
#define mfSPI_TXS_IS_EMPTY     	((rSPI_CH1STAT)&(1<<1))
#define mfSPI_RXS_IS_FULL     	((rSPI_CH1STAT)&(1<<0))

#define mfSPI_FORCE_CHANNEL1_CSL 	(rSPI_CH1CONF|=(1<<20))
#define mfSPI_FORCE_CHANNEL1_CSH 	(rSPI_CH1CONF&=(~(1<<20)))

INT32S SPI_Init(void);
INT8U  SPI_ReadU8(INT32U Address);
INT16U SPI_ReadU16(INT32U Address);
INT32U SPI_ReadU32(INT32U Address);
INT32S SPI_Read(INT32U Address, INT8U * pValue, INT32U Count);

INT32S SPI_WriteU8(INT32U Address, INT8U Value);
INT32S SPI_WriteU16(INT32U Address, INT16U Value);
INT32S SPI_WriteU32(INT32U Address, INT32U Value);
INT32S SPI_Write(INT32U Address, INT8U * pValue, INT32U Count);


#define IIC_Read    	SPI_Read
#define IIC_Read1   	SPI_ReadU8
#define IIC_Read2   	SPI_ReadU16
#define IIC_Read4   	SPI_ReadU32
#define IIC_Write   	SPI_Write
#define IIC_Write1  	SPI_WriteU8
#define IIC_Write2  	SPI_WriteU16
#define IIC_Write4  	SPI_WriteU32

#endif
