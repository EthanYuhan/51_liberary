#include "sys.h"	
#include "delay.h"	
#include "dcmotor.h" 

 
/************************************************
 蜂鸣器实验 
 

************************************************/

 int main(void)
 {
	delay_init();	    	 //延时函数初始化	  
	DCMOTOR_Init();         	//初始化直流电机端口
	while(1)
	{

		DCMOTOR1=1;
    	
		delay_ms(1800);//延时1800ms
 
		DCMOTOR1=0;  
		
		delay_ms(1800);//延时1800ms		

	}
 }

