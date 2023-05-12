#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
 PWM调光实验  
详见README文档说明
************************************************/

	
 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 //	LED_Init();			     //LED端口初始化
 	TIM4_PWM_Init(899,80);	 //80分频 PWM频率=72000000/900/80=1Khz   
												 
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>900)dir=0;
		if(led0pwmval==0)dir=1;										 
		TIM_SetCompare2(TIM4,led0pwmval);		   
	}	 
 }

