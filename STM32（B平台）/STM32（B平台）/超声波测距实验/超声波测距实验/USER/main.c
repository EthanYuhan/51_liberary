
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "wave.h"
#include "sys.h"

 
/************************************************
超声波测距实验
详见README文档说明
************************************************/

 int main(void)
 {		
 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200

	 //EXTIX_Init();		 	//外部中断初始化

	Timer_SRD_Init(5000,7199);
	Wave_SRD_Init();
	 
	while(1)   //测试
	{	    
		Wave_SRD_Strat();	

    delay_ms(1000);
		delay_ms(1000);
	
		
		
        	
	}
 }

