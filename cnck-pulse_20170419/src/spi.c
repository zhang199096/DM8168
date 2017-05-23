#include "../include/spi.h"
#include <xdc/runtime/Log.h>
#include <xdc/std.h>


//if ok return 1 ,else return 0
inline INT32S SPI_WaitTransEnd(void)
{
	INT32U i;
	for(i=0;i<mSPI_MAX_WAIT_LOP;i++)
	{
		if((rSPI_CH1STAT&0xc)==0xc) 
			return 1; //EOT=1 nad txs=0
		//if(TRANS_ENDED) return 1;
	}
	return 0;
}

#if 0
inline INT32S SPI_EmptyRX(void)
{
	INT32S i,j;
	for(i=0;i<64;i++)
	{
		j=rSPI_RX1;
		if(!mfSPI_RXS_IS_FULL)
			return 1;
	}
	return 0;
}
#endif


/*read status register*/
INT32S SPI_ReadStatuReg(INT8U *pValue)
{
	rSPI_XFERLEVEL=(2<<16);
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=(mSPI_OPCODE_RDSR);
	rSPI_TX1=0;
	if(SPI_WaitTransEnd()==0) goto ERROR;
	*pValue=rSPI_RX1; //ignore the first cmd byte's return
	*pValue=rSPI_RX1;
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
ERROR:
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 0;
}

INT32S SPI_ReadID(INT8U *pValue)
{
	INT32U i;
	//if(emptyRX()==0)goto ERROR;
	rSPI_XFERLEVEL=10<<16;
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=mSPI_OPCODE_RDID;
	for(i=0;i<9;i++)
	{
		rSPI_TX1=0; //9 dumy byte to receive data
	}
	if(SPI_WaitTransEnd()==0) goto ERROR;
	i =rSPI_RX1; //ignore the first cmd byte's return
	for(i=0;i<9;i++)
	{
		*pValue++=rSPI_RX1;
	}
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
ERROR:
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	Log_info0("spi time out error read ID \n");
	return 0;
}

INT32S SPI_WriteStatuReg(INT8U *pValue)
{
	rSPI_XFERLEVEL=(2<<16);
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=(mSPI_OPCODE_RDID);
	rSPI_TX1=*pValue;
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
}
INT32S SPI_WriteEnable(void)
{
	rSPI_XFERLEVEL=(1<<16);
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=mSPI_OPCODE_WREN;
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
}

INT32S SPI_ReadArray(INT32U Address,INT8U *pValue,INT32U Count)
{
	INT32S i,j;
	if((pValue==NULL)||(Count>28)) return 0;
	Address=Address&0x1ffff; //ignore not used address bit
	//if(emptyRX()==0)goto ERROR;
	rSPI_XFERLEVEL=((Count+4)<<16);
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=mSPI_OPCODE_NORM_READ;
	rSPI_TX1=((Address>>16)&0xFF );
	rSPI_TX1=((Address>>8)&0xFF );
	rSPI_TX1=((Address)&0xFF );
	for(i=0;i<Count;i++)
	{
		//if(waitTransEnd()==0)goto ERROR;
		// wait for transmit may be need if SPI clock is slow
		rSPI_TX1=0;
	}
	if(SPI_WaitTransEnd()==0)goto ERROR;
	//waitTransEnd();
	//read rx data
	for(i=0;i<4;i++)
		j=rSPI_RX1;
	for(i=0;i<Count;i++)
		*pValue++=rSPI_RX1;
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
ERROR:
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	Log_info0("spi time out error read \n");
	return 0;
}

INT8U SPI_ReadU8(INT32U Address)
{
	INT8U Value;
	SPI_ReadArray(Address,&Value,1);
	return Value;
}
INT16U SPI_ReadU16(INT32U Address)
{
	INT16U Value;
	SPI_ReadArray(Address,(INT8U *)&Value,2);
	return Value;
}
INT32U SPI_ReadU32(INT32U Address)
{
	INT32U Value;
	SPI_ReadArray(Address,(INT8U *)&Value,4);
	return Value;
}
INT32S SPI_Read(INT32U Address,INT8U *pValue,INT32U Count)
{
	if(Count<28)
	{
		return SPI_ReadArray(Address,pValue,Count);
	}
	else
	{
		INT32S i=0;
		INT32S ct=24;
		do
		{
			if(SPI_ReadArray(Address+i,pValue+i,ct)==0)
				return 0; //error
			i=i+ct;
			if((i+ct)>=Count)
			{
				ct=Count-i;
			}
		}while(ct>0);
	}
	return 1;
}

INT32S SPI_WriteArray(INT32U Address,INT8U *pValue,INT32U Count)
{
	INT32U i;
	if((pValue==NULL)||(Count>28)) return 0;
	Address=Address&0x1ffff; //ignore not used address bit
	SPI_WriteEnable();

	rSPI_XFERLEVEL=((Count+4)<<16);
	mfSPI_FORCE_CHANNEL1_CSL; //case cs low
	mfSPI_START_CHANNEL1;
	rSPI_TX1=(mSPI_OPCODE_WRITE);
	rSPI_TX1=(Address>>16)&0xFF;
	rSPI_TX1=(Address>>8)&0xFF;
	rSPI_TX1=(Address)&0xFF;
	for(i=0;i<Count;i++)
	{
		rSPI_TX1=*pValue++;
	}
	if(SPI_WaitTransEnd()==0)goto ERROR;
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	return 1;
ERROR:
	mfSPI_STOP_CHANEL1;
	mfSPI_FORCE_CHANNEL1_CSH;
	Log_info0("spi time out error write \n");
	return 0;
}
INT32S SPI_WriteU8(INT32U Address,INT8U Value)
{
	return SPI_WriteArray(Address,(INT8U*)&Value,1);
}
INT32S SPI_WriteU16(INT32U Address,INT16U Value)
{
	return SPI_WriteArray(Address,(INT8U*)&Value,2);
}
INT32S SPI_WriteU32(INT32U Address,INT32U Value)
{
	return SPI_WriteArray(Address,(INT8U*)&Value,4);
}

INT32S SPI_Write(INT32U Address,INT8U *pValue,INT32U Count)
{
	if(Count<28)
	{
		return SPI_WriteArray(Address,pValue,Count);
	}
	else
	{
		INT32S i=0;
		INT32S ct=24;
		do
		{
			if(SPI_WriteArray(Address+i,pValue+i,ct)==0)
				return 0; //error
			i=i+ct;
			if((i+ct)>=Count)
			{
				ct=Count-i;
			}
		}while(ct>0);
	}
	return 1;
}

INT32S SPI_Init(void)
{
	//INT32U regval;
	INT8U FM25V10ID[10];
	//Log_info0("spi configuring\n");
	rSPI_CM_ALLOW=0X2; //open thee clock source to spi
	//regval=READ_REG(MCSPI_HW_IPVESION);
	//Log_info1("Configuring spi IP revision = 0x%08x\n", regval);
	//regval=READ_REG(MCSPI_HW_INFO);
	//Log_info1("Configuring spi INFO = 0x%08x\n", regval);
	rSPI_IP_SYSCONFIG=((1<<2)|1);
	//regval=READ_REG(MCSPI_IP_SYSCONFIG);
	//Log_info1("Configuring spi IP config = 0x%08x\n", regval);
	//regval=READ_REG(SPI_REVESION);
	//Log_info1("Configuring spi revision = 0x%08x\n", regval);

	rSPI_SYSCONFIG=(3<<8|1<<3|1<<2|1<<1|0);
	//regval=READ_REG(SPI_SYSCONFIG);
	//Log_info1("Configuring spi sysconfig = 0x%08x\n", regval);
	rSPI_WAKEUPENABLE=1;
	rSPI_MODULCTRL=(0<<8|0<<7|0<<4|0<<3|0<<2|0<<1|1<<0);
	//regval=READ_REG(SPI_MODULCTRL);
	//Log_info1("Configuring spi modlule contrl = 0x%08x\n", regval);

	mfSPI_START_CHANNEL1;
	rSPI_CH1CONF=(0<<29|1<<28|1<<27|1<<25|1<<16|7<<7|1<<6|1<<2);  //d0 for miso d1 fo mosi 64 div clk
	//regval=READ_REG(SPI_CH1CONF);
	//Log_info1("Configuring spi modlule contrl = 0x%08x\n", regval);

	rSPI_CH1CTRL=0;
	rSPI_XFERLEVEL=(2<<8|1);
	
	FM25V10ID[6]=0;
	FM25V10ID[7]=0;
	FM25V10ID[8]=0;
	if(SPI_ReadID(FM25V10ID)) 
	if((FM25V10ID[6]==0xc2)&&(FM25V10ID[7]==0x24)&&(FM25V10ID[8]==0x00))
		return 1;
	//spi_writeEnable();	
	Log_info3("FM25V10 read id fail ID=0x%02x %02x %02x",FM25V10ID[6], FM25V10ID[7], FM25V10ID[8]);
	return 0;
}


////////////////////////////////////////////////
void testspi()
{
    INT8U temp[256];
    INT8U temp2[256];
    int i;
    for(i=0;i<256;i++)
    {
        temp[i]=i;
        temp2[i]=0;
    }
    IIC_Write(10,temp,256);
    IIC_Read(10,temp2,256);
}


