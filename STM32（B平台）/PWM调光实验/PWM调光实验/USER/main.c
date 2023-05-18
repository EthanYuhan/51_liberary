#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
 PWM����ʵ��  
���README�ĵ�˵��
************************************************/

	
 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 //	LED_Init();			     //LED�˿ڳ�ʼ��
 	TIM4_PWM_Init(899,80);	 //80��Ƶ PWMƵ��=72000000/900/80=1Khz   
												 
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

