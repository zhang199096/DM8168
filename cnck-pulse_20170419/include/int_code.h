// FILE:	int_code.h

#ifndef DSP6713_INT_CODE_H
#define DSP6713_INT_CODE_H

#define WR_MEM_16(addr, data)    *(unsigned short*)(addr) =(unsigned short)(data)
#define RD_MEM_16(addr)          *(unsigned short*)(addr)

#define WR_MEM_32(addr, data)    *(unsigned int*)(addr) =(unsigned int)(data)
#define RD_MEM_32(addr)          *(unsigned int*)(addr)

extern void interrupt4();
extern void interrupt6();
extern void main_cycle();
extern void Log_info_MACH_Position();

#endif  // end of definition

//===========================================================================
// No more.
//===========================================================================
