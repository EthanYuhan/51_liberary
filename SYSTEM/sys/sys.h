#ifndef __SYS_H
#define __SYS_H	

#include  "stc15f2k60s2.h"
#include <intrins.h>


#define DRIVING_SIMULATION  0  //�Ƿ��Ƕ�ʱ������ģ��Ƽ�/���̼���



#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

void delay2us(u16 us);   //2us������ʱ
void delay4us(u16 us);   //4us������ʱ
void delay1ms(u16 ms);   //������ʱ
void delay_us(u16 us);   //Ϊ����Դ����
void delay_ms(u16 ms);
void Delay100ms(void);	
	  		 
#endif  
	 
	 



