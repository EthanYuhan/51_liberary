#ifndef __SYS_H
#define __SYS_H	

#include  "stc15f2k60s2.h"
#include <intrins.h>


#define DRIVING_SIMULATION  0  //是否是定时器脉冲模拟计价/码盘计数



#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

void delay2us(u16 us);   //2us倍数延时
void delay4us(u16 us);   //4us倍数延时
void delay1ms(u16 ms);   //毫秒延时
void delay_us(u16 us);   //为兼容源程序
void delay_ms(u16 ms);
void Delay100ms(void);	
	  		 
#endif  
	 
	 



