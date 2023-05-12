#ifndef __LD3320_H
#define __LD3320_H

#include "sys.h"
#include "ld3320_test.h"



#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		    0x08
#define LD_MODE_MP3		 		0x40

void LD_WriteReg(u8 data1,u8 data2);
u8 LD_ReadReg(u8 reg_add);
void LD_Init_Common(void);
void LD_Init_ASR(void);
u8 LD_Run(void);
void LD_reset(void);
void LD_AsrStart(void);
u8 LD_Check_ASRBusyFlag_b2(void);
u8 spi_send_byte(u8 byte);
u8 LD_AsrRun(void);
u8 LD_AsrAddFixed(void);

void LD3320_Config(void);
  
#endif /* __LD3320_H */
