#ifndef _DATATYPE_H_
#define _DATATYPE_H_

typedef unsigned char		BOOL; 
typedef unsigned char		INT8U;                    
typedef signed char 		INT8S;
typedef unsigned short		INT16U;                  
typedef signed short		INT16S;   
typedef unsigned int		INT32U;
typedef signed int			INT32S; 
typedef unsigned long		INT40U; 
typedef signed long	    	INT40S;
typedef float				FLOAT32;
typedef double 		    	FLOAT64;

#define BIT0		0x1
#define BIT1		0x2
#define BIT2		0x4
#define BIT3		0x8
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80
#define BIT8		0x100
#define BIT9		0x200
#define BIT10	0x400
#define BIT11	0x800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BIT16	0x10000
#define BIT17	0x20000
#define BIT18	0x40000
#define BIT19	0x80000
#define BIT20	0x100000
#define BIT21	0x200000
#define BIT22	0x400000
#define BIT23	0x800000
#define BIT24	0x1000000
#define BIT25	0x2000000
#define BIT26	0x4000000
#define BIT27	0x8000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#define BIT_DA16	0xFFFF
#define BIT_DA32	0xFFFFFFFF
#define BIT_DA30	0x3FFFFFFF

#define BIT_AXISX	(0x1)
#define BIT_AXISY	(0x2)
#define BIT_AXISZ	(0x4)
#define BIT_AXISIV	(0x8)
#define BIT_AXISV	(0x10)
#define BIT_AXIS3	(0x7)
#define BIT_AXIS5	(0x31)


#define mfBIT(bit) (1<<bit)
#define mfBITGET(var,bit)  ((var>>bit)&1)
#define mfBITSET(var,bit)  {var=var|(1<<bit);}
#define mfBITCLR(var,bit)  {var=var&(~(1<<bit));}
#define mfBITWRITE(var,bit,val)  {var=(var&(~(1<<bit)))|((val&1)<<bit);}
#define mfBITSWRITE(var,mask,val){var=(var&(~mask))|val;}

//#define PI  (3.1415926536)

extern const double PI;   //real define in dec6713_main.c

#define 	e				2.7182818284590452353602874713527

#ifndef OK
#define OK 	  1
#endif

#ifndef FAIL
#define FAIL  0
#endif

#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif
