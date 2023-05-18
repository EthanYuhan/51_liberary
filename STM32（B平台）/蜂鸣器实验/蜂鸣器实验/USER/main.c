#include "sys.h"	
#include "delay.h"	
#include "beep.h" 

 
/************************************************
 蜂鸣器实验 
 

************************************************/

 int main(void)
 {
	delay_init();	    	 //延时函数初始化	  
	BEEP_Init();         	//初始化蜂鸣器端口
	while(1)
	{

		BEEP=0;		  
		delay_ms(300);//延时300ms
 
		BEEP=1;  
		delay_ms(300);//延时300ms
	}
 }

