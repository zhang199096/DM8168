/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  NOR Flash Test
 *
 */

 #include "..\\include\\main_dsp.h"

#define FLASH_SKIPSIZE      0x20000  // FLASH_SKIPSIZE is an arbitrarily set size. 
                                     // For now it is set as the sector size
static Uint32 buf_len = 1024;
static Uint16 rx[1024];
static Uint16 tx[1024];

#define rUARTA_BUF 		(*((char *)(0xA0000160)))

void test_all(void)
{
	static Uint16 test_allSign = 0;
		
	 if(test_allSign == 0)
	{
		return 0;
	}
	 
	test_PWM();
	GPMC_test();
	GPMC_test1();
	GPMC_test2();

	Test_Dsp_SPC();
	Test_Dsp_PacketIn();
	//Task_sleep(2);
}

void InitTest(void)	
{}


//测试各进给轴脉冲输出稳定性
Uint16 test_PWM(void)
{	
	static Int8 SIGN = 0;
	static Int32 DeltaPulse = 20;
	static FLOAT64 Time_Sample=0.001;	
	static Uint16 test_PWMSign = 0;	
	static Uint16 LPMEN = 0x3;
	
	if(test_PWMSign == 0)
	{
		return 0;
	}
	
	int PWM_PeriodRegister=divdp(FPGA_MAX_F,divdp((DeltaPulse+1),Time_Sample));
	OverallSign.Int4IdleSign_1 = 0;

	Read_GPIO_IRQSTATUS_RAW();
	if((GPIO0_IRQSTATUS_RAW_0_Register.all != 0)||
		(GPIO0_IRQSTATUS_RAW_1_Register.all != 0)||
		(GPIO1_IRQSTATUS_RAW_0_Register.all != 0)||
		(GPIO1_IRQSTATUS_RAW_1_Register.all != 0)	
	)
	{
		OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
	}

	
	if(test_PWMSign == 1)
	{
		Read_GPIO_IRQSTATUS_RAW();
		if((GPIO0_IRQSTATUS_RAW_0_Register.bit.SIGN23 != 1)||
			(GPIO0_IRQSTATUS_RAW_1_Register.bit.SIGN24 != 1))
		{
			OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
			return 0;
		}
	}
	
	Servo_LPMEN_Register.bit.LPMEN_1 = 0;
	Servo_LPMEN_Register.bit.LPMEN_2 = 0;
	Servo_LPMEN_Register.bit.LPMEN_3 = 0;
	Servo_LPMEN_Register.bit.LPMEN_4 = 0;
	Servo_LPMEN_Register.all = 0;//for test
	Write_Servo_LPMEN();
	
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_1= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_2= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_3= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_4= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_5= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_6= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_7= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_8= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_9= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_10= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_11= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_12= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_13= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_14= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_15= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_16= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_17= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_18= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_19= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_20= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_21= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_22= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= SIGN;
	Write_Servo_Sign1_Out();
	Write_Servo_Sign2_Out();

	*(Servo_Axis1.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis2.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis3.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis4.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis5.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis6.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis7.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis8.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis9.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis10.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis11.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis12.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis13.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis14.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis15.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis16.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis17.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis18.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis19.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis20.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis21.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis22.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis23.LPM_CounterPort) = abs(DeltaPulse);
	
	*(Servo_Axis1.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis2.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis3.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis4.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis5.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis6.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis7.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis8.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis9.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis10.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis11.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis12.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis13.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis14.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis15.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis16.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis17.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis18.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis19.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis20.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis21.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis22.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis23.PWM_Port) = PWM_PeriodRegister;

	Servo_LPMEN_Register.bit.LPMEN_1 = 1;
	Servo_LPMEN_Register.bit.LPMEN_2 = 1;
	Servo_LPMEN_Register.all = LPMEN;//for test
	Write_Servo_LPMEN();
	
	return 1;

}



//测试各进给轴脉冲输出稳定性
Uint16 test_PWM_IO(void)
{}


//测试各进给轴脉冲输出稳定性
Uint16 test_PWM_CS(void)
{	
	static Int8 SIGN = 0;
	static Int32 DeltaPulse = 5;
	static FLOAT64 Time_Sample=0.001;	
	static Uint16 Uint16test_PWM_CSSign = 0; 
	static Int32 TestPWNCounter = 0;
	
	if(Uint16test_PWM_CSSign == 0)
	{
		return 0;
	}
	
	int PWM_PeriodRegister=divdp(FPGA_MAX_F,divdp((DeltaPulse+1),Time_Sample));
	OverallSign.Int4IdleSign_1 = 0;
	
	Read_GPIO_IRQSTATUS_RAW();
	if((GPIO0_IRQSTATUS_RAW_0_Register.bit.SIGN23 != 1)||
		(GPIO0_IRQSTATUS_RAW_1_Register.bit.SIGN24 != 1)	
	)
	{
		OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
		//return 0;
	}
	Read_GPIO_IRQSTATUS();
	if((GPIO0_IRQSTATUS_0_Register.bit.SIGN23 != 1)||
		(GPIO0_IRQSTATUS_1_Register.bit.SIGN24 != 1)	
	)
	{
		OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
		//return 0;
	}

	Read_Servo_Over_In();
	if((OverallSign.EveryAxisOverValid1 == 0)||(OverallSign.EveryAxisOverValid2 == 0))
	{
		OverallSign.Int4IdleSign_1 = OverallSign.Int4IdleSign_1;
		return 0;
	}
	
	Servo_LPMEN_Register.bit.LPMEN_1 = 0;
	Servo_LPMEN_Register.bit.LPMEN_2 = 0;
	Write_Servo_LPMEN();
	
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_1= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_2= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_3= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_4= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_5= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_6= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_7= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_8= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_9= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_10= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_11= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_12= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_13= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_14= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_15= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_16= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_17= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_18= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_19= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_20= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_21= SIGN;
	Servo_Sign2_Out_Register.bit.SERVO_SIGN_22= SIGN;
	Servo_Sign1_Out_Register.bit.SERVO_SIGN_23= SIGN;
	Write_Servo_Sign1_Out();
	Write_Servo_Sign2_Out();

	*(Servo_Axis1.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis2.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis3.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis4.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis5.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis6.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis7.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis8.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis9.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis10.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis11.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis12.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis13.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis14.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis15.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis16.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis17.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis18.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis19.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis20.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis21.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis22.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis23.LPM_CounterPort) = abs(DeltaPulse);
	
	*(Servo_Axis1.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis2.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis3.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis4.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis5.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis6.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis7.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis8.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis9.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis10.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis11.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis12.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis13.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis14.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis15.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis16.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis17.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis18.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis19.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis20.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis21.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis22.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis23.PWM_Port) = PWM_PeriodRegister;

	Servo_LPMEN_Register.bit.LPMEN_1 = 1;
	Servo_LPMEN_Register.bit.LPMEN_2 = 1;
	Write_Servo_LPMEN();
	TestPWNCounter++;
	if(TestPWNCounter >= 1000)
	{
		TestPWNCounter = 0;
		Uint16test_PWM_CSSign = 0;
	}
	return 1;

}


Uint16 test_X_PWM(void)
{}

Uint16 test_Y_PWM(void)
{}

Uint16 test_Z_PWM(void)
{}

Uint16 test_A_PWM(void)
{}

void test_servo_io(void)
{}

Uint16 test_com_ARM(void)
{}

Uint16 test_SPC_code(void)
{
	return 1;
}

Uint16 test_STD_code(void)
{
	return 1;
}

Uint16 test_int4(void)
{}

Uint16 test_IO(void)
{}

Uint16 test_VT(void)
{}
Uint16 test_sci(void)
{}

void delay()
{
	int time;
	for(time=0;time<0x8000;time++){}
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_test( )                                                        *
 *                                                                          *
 *      Write a 1024-byte long pattern every FLASH_SKIPSIZE bytes           *
 * ------------------------------------------------------------------------ */
Int16 norflash_test( )
{
    Uint32 i = 0, j = 0;
    Uint16 *p16, id[4];
    Uint32 addr;
    
    /* Initialize Flash */
/*    norflash_init();
   gpmc_cs1_init();*/
    gpmc_cs1_init();
/*    *(unsigned int *)0x4803213c = 0x00000001;  // GPMC 27 low
    *(unsigned int *)0x48032134 = 0xffeffff0;  // GPMC 27 low
*/
/*
while(1){
	WR_MEM_16(0x8000000, 0xAAAA);
	EVM816X_waitusec(100);
	n=RD_MEM_16(0X8000300);
	m=RD_MEM_16(0X8000302);
	WR_MEM_16(0x8000000, n);
	WR_MEM_16(0x8000000, m);
}*/
/*while(1){
    id[0]=RD_MEM_16(0X2000300);
	EVM816X_waitusec(100);
    id[1]=RD_MEM_16(0X2000310);
}*/
    //id[0]=RD_MEM_16(0X2000300);
    //base_address = 0x2000000;
 while(1)
 	 {
	 	 WR_MEM_16(0x20008B8,0x0000);
	 	 //EVM816X_waitusec(100);
	 	 for(i=0;i<23;i++){
	 		 WR_MEM_16(0x2000800+i*4, 0x00AA);
	 		WR_MEM_16(0x20008c4, 0x0000);
	 		WR_MEM_16(0x20008c8, 0x0000);
	 		 //EVM816X_waitusec(500);
	 		// WR_MEM_16(0x2000804, 0x5555);
	 		 //WR_MEM_16(0x2000808, 0x0AAA);
	 		 WR_MEM_16(0x200085C+i*4, 0x4);
	 		WR_MEM_16(0x20008c4, 0xffff);
	 		WR_MEM_16(0x20008c8, 0xffff);

	 	 }
	 	 WR_MEM_16(0x20008B8,0xffff);
	 	//EVM816X_waitusec(100);
//	 	delay();
 	 }

    /* ---------------------------------------------------------------- *
     *  Erase                                                           *
     * ---------------------------------------------------------------- */
    /* Erasing all Flash Sectors */
    printf( "     Erasing Sectors to be written to.\n");
    for ( i = 0 ; i < 0x200/*1*/ ; i++ )    
        norflash_erase( FLASH_BASE + (i * FLASH_SECTORSIZE), FLASH_SKIPSIZE  );

    /* ---------------------------------------------------------------- *
     *  Write                                                           *
     * ---------------------------------------------------------------- */
     /* Test first sector */
     p16 = ( Uint16* )FLASH_BASE;
     for ( i = 0 ; i < (FLASH_SECTORSIZE ) ; i +=2 )
     {
         /* Program one 16-bit word */
         FLASH_CTL555 = FLASH_CMD_AA;
         FLASH_CTL2AA = FLASH_CMD_55;
         FLASH_CTL555 = FLASH_PROGRAM;
         *p16 = i/2;
         while(*p16 != i/2);  // Wait for operation to complete
         *p16++;
     }
    /* Write a pattern of buf_len length every FLASH_SKIPSIZE bytes till end of Flash */
    printf( "     Writing Flash pattern\n", 0, buf_len );
    i = 0;
    for ( addr = FLASH_BASE + FLASH_SKIPSIZE ; addr < FLASH_END ; addr += FLASH_SKIPSIZE )
    {
        /* Create write pattern */
        p16 = ( Uint16* )tx;
        for ( j = 0 ; j < buf_len ; j ++ )
            *p16++ = ( (addr >> 12) + j + i );

        /* Write the pattern to Flash */
        norflash_write( ( Uint32 )tx, addr, buf_len );
        i++;
    }
    
    /* ---------------------------------------------------------------- *
     *  Read                                                            *
     * ---------------------------------------------------------------- */
     /* Check first sector */
     p16 = ( Uint16* )0x8000000;
     for ( i = 0 ; i < (FLASH_SECTORSIZE ) ; i +=2 )
     {
         /* Program one 16-bit word */

         if(*p16 != i/2)
             printf("First Sector Data Error\n");
         *p16++;
     } 

    printf( "     Reading Flash pattern\n", 0, 1024/*nbytes*/ );
    /* Read the pattern of buf_len length every FLASH_SKIPSIZE bytes till end of Flash */
    i = 0;
    for ( addr = FLASH_BASE + FLASH_SKIPSIZE; addr < FLASH_END ; addr += FLASH_SKIPSIZE )
    {
        /* Read from Flash */
        norflash_read( addr, ( Uint32 )rx, buf_len );

        /* Check pattern */
        p16 = ( Uint16* )rx;
        for ( j = 0 ; j < buf_len ; j ++ )
            if ( *p16++ != ( (addr >> 12) + j + i ) )
                return ( j | 0x8000 );
        i++;
    }

    /* Test Passed */

    return 0;
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_test( )                                                        *
 *                                                                          *
 *      Write a 1024-byte long pattern every FLASH_SKIPSIZE bytes           *
 * ------------------------------------------------------------------------ */
Int16 GPMC_test(void)
{
	Uint32 i = 0, j = 0;
	Uint16 *p16, id[4];
	Uint32 addr;
	static Uint16 GPMCTestSign = 0;
		
	/* Initialize Flash */
	/*	  norflash_init();
	 gpmc_cs1_init();*/
	 if(GPMCTestSign == 0)
	 {
	 	//GPMCTestSign = 1;
		//gpmc_cs1_init();
		return 0;
	}

	WR_MEM_16(0x20008B8,0x0000);
	//EVM816X_waitusec(100);
	for(i=0;i<23;i++)
	{
		WR_MEM_16(0x2000800+i*4, 0x00AA);
		WR_MEM_16(0x20008c4, 0x0000);
		WR_MEM_16(0x20008c8, 0x0000);
		//EVM816X_waitusec(500);
		// WR_MEM_16(0x2000804, 0x5555);
		//WR_MEM_16(0x2000808, 0x0AAA);
		WR_MEM_16(0x200085C+i*4, 0x4);
		WR_MEM_16(0x20008c4, 0xffff);
		WR_MEM_16(0x20008c8, 0xffff);

	}
	
}



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_test( )                                                        *
 *                                                                          *
 *      Write a 1024-byte long pattern every FLASH_SKIPSIZE bytes           *
 * ------------------------------------------------------------------------ */
Int16 GPMC_test1(void)
{
	Uint32 i = 0, j = 0;
	Uint16 *p16, id[4];
	Uint32 addr;
	static Uint16 GPMCTest1Sign = 0;
		
	 if(GPMCTest1Sign == 0)
	{
		return 0;
	}


	WR_MEM_16(0x010008B8,0x0000);
	//EVM816X_waitusec(100);
	for(i=0;i<23;i++)
	{
		WR_MEM_16(0x01000800+i*4, 0x00AA);
		WR_MEM_16(0x010008c4, 0x0000);
		WR_MEM_16(0x010008c8, 0x0000);
		//EVM816X_waitusec(500);
		// WR_MEM_16(0x2000804, 0x5555);
		//WR_MEM_16(0x2000808, 0x0AAA);
		WR_MEM_16(0x0100085C+i*4, 0x4);
		WR_MEM_16(0x010008c4, 0xffff);
		WR_MEM_16(0x010008c8, 0xffff);

	}
	WR_MEM_16(0x010008B8,0xffff);
	//EVM816X_waitusec(100);
	//		delay();
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_test( )                                                        *
 *                                                                          *
 *      Write a 1024-byte long pattern every FLASH_SKIPSIZE bytes           *
 * ------------------------------------------------------------------------ */
Int16 GPMC_test2(void)
{
	Uint32 i = 0, j = 0;
	Uint16 *p16, id[4];
	Uint32 addr;
	int TestValue; 
	static Uint16 GPMCTest2Sign = 0;
		
	 if(GPMCTest2Sign == 0)
	 {
	 	return 0;
	}
	 
    	int *LPMENmemptr=(int*)(0x110008B8); //LPMEN
    	int *Tprmemptr=(int*)(0x11000800); //Tpr
    	int *Sign1memptr=(int*)(0x110008c4); //Sign1
    	int *Sign2memptr=(int*)(0x110008c8); //Tpr2
    	int *Pwmmemptr=(int*)(0x1100085C); //Tpr1

	//WR_MEM_16(0x1100000E,0x3456);	
	//WR_MEM_16((mFPGA_BASE+0x8B8),0x4567);
	//TestValue = RD_MEM_16(mFPGA_BASE+0x8FA);
    	//WR_MEM_16((0x110008B8),0x4567);
    	//TestValue = RD_MEM_16(0x110008FA);
	static FLOAT64 Time_Sample=0.0005;
	WR_MEM_16(0x110008B8,0x0000);
	
	#if 0
	int Tpr=divdp(FPGA_MAX_F,divdp((0x04+1),Time_Sample));
	//delay();
	
	for(i=0;i<23;i++)
	{
		WR_MEM_16(0x11000800+i*4, Tpr);
		WR_MEM_16(0x110008c4, 0x0000);				
		WR_MEM_16(0x110008c8, 0x0000);				
		//EVM816X_waitusec(500);
		//delay();
		// WR_MEM_16(0x2000804, 0x5555);
		//WR_MEM_16(0x2000808, 0x0AAA);
		WR_MEM_16(0x1100085C+i*4, 0x4);
		//WR_MEM_16(0x110008c4, 0xffff);
		//WR_MEM_16(0x110008c8, 0xffff);
	}
	#endif
	
	static Int32 DeltaPulse = 5;
	int PWM_PeriodRegister=divdp(FPGA_MAX_F,divdp((DeltaPulse+1),Time_Sample));

	*(Servo_Axis1.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis2.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis3.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis4.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis5.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis6.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis7.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis8.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis9.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis10.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis11.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis12.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis13.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis14.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis15.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis16.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis17.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis18.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis19.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis20.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis21.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis22.LPM_CounterPort) = abs(DeltaPulse);
	*(Servo_Axis23.LPM_CounterPort) = abs(DeltaPulse);
	
	*(Servo_Axis1.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis2.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis3.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis4.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis5.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis6.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis7.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis8.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis9.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis10.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis11.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis12.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis13.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis14.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis15.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis16.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis17.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis18.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis19.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis20.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis21.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis22.PWM_Port) = PWM_PeriodRegister;
	*(Servo_Axis23.PWM_Port) = PWM_PeriodRegister;
	
	WR_MEM_16(0x110008B8,0xffff);
	//EVM816X_waitusec(100);
	//delay();

	TestValue = RD_MEM_16(0x11000108);
	TestValue = RD_MEM_16(0x1100010C);
	TestValue = RD_MEM_16(0x11000110);
	TestValue = RD_MEM_16(0x11000114);
	
	//TestValue = RD_MEM_16(0x110008FB);
}

void Test_Dsp_SPC(void)
{
	static Uint16 Test_Dsp_SPCSign = 1;
	static INT32S Endpoint = 65000;
		
	if(Test_Dsp_SPCSign == 0)
	{
		return 0;
	}

	INT32S i;

	psDSPCodeSPC->CodeTotal = 1;

	for(i=0;i<psDSPCodeSPC->CodeTotal;i++)
	{	
		psDSPCodeSPCSub=&psDSPCodeSPC->sInfoBuf[i];
			
		psDSPCodeSPCSub->CountID = i+1;		//SPCNCSign.RealSendCount
				
		psDSPCodeSPCSub->PosAxis1 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis2 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis3 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis4 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis5 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis6 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis7 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis8 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis9 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis10 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis11 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis12 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis13 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis14 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis15 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis16 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis17 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis18 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis19 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis20 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis21 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis22 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis23 = Endpoint;	//Endpoint
		psDSPCodeSPCSub->PosAxis24 = 0;
		psDSPCodeSPCSub->PosAxis25 = 0;
		
		psDSPCodeSPCSub->Feed = 30;

	}

	//psSHMEvent->GCodeSPC = 1;		//for test

	Test_Dsp_SPCSign = 0;
	
}


void Test_Dsp_PacketIn(void)
{
	static Uint16 Test_Dsp_PacketInSign = 0;
	
	if(psParaDsp_Ctrl->FunctionSelect01 == 1)
	{
		psParaDsp_Ctrl->FunctionSelect01 = 0;
		Test_Dsp_PacketInSign = 0;
	}
	if(Test_Dsp_PacketInSign == 0)
	{
		psDSPPacketIn_InfoEvent->STDInit = true;
		psDSPPacketIn_InfoEvent->SPCInit = true;
		psDSPPacketIn_InfoEvent->ServoOn = true;
		psDSPPacketIn_InfoEvent->DSPPositionGet = true;
		psDSPPacketIn_InfoEvent->NCModeType = SPCCODERUN;
		Test_Dsp_PacketInSign = 1;
		return 0;
	}
	if(psDSPPacketOut_InfoEvent->DSPInAlarm)
	{
		Error.MainErrorSign = Error.MainErrorSign;
		return 0;
	}
	 
	if(Test_Dsp_PacketInSign == 1)
	{
		//if((SPCNCSign.GetPositionSign == 0)&&(STDNCSign.GetPositionSign == 0)&&(NCRunSign.GetPositionSign == 0))				
		if((psDSPPacketOut_InfoEvent->DSPLocateOver)&&
			(psDSPPacketOut_InfoEvent->SPCSendEnable == OK)&&
			(psDSPPacketOut_InfoEvent->DSPRunOver))
		{
			psSHMEvent->GCodeSPC = 1;
			psDSPPacketIn->GradeG0 = 10;
			psDSPPacketIn->GradeG1 = 10;			
			Test_Dsp_PacketInSign = 2;
		}
	}
}

