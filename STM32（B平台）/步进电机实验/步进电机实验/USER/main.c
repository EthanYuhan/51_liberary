#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "motor.h"
 
 
/************************************************
 步进电机实验 
 详见README文档说明
************************************************/


 int main(void)
 {	
	delay_init();	    //延时函数初始化	  
	 
	MOTOR_Init();  //步进电机IO初始化 


	while(1)
	{
		int i;
		for(i = 0;i < 1000;i++)      //电机正转
			stepper(Pos,5);    //间隔最少是5ms  
		delay_ms(1000);    //延时
		for(i = 0;i < 1000;i++)      //电机反转
			stepper(Neg,8); 
		delay_ms(1000);    //延时
	}

	
	
 }

