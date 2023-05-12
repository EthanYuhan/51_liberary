			
#ifndef __SYS_H
#define __SYS_H	

#include  "stc15f2k60s2.h"
#include <intrins.h>

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long


void delaylow(u8 time);	 //红外专用 延时
void delay2us(u16 us);   //2us倍数延时
void delay4us(u16 us);   //4us倍数延时
void delay1ms(u16 ms);   //毫秒延时

void delay_us(u16 us);  //为兼容源程序
void delay_ms(u16 ms);

void Delay100ms(void);	

	  		 
#endif  
	 
	 



