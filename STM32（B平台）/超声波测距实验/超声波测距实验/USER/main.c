
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include "wave.h"
#include "sys.h"

 
/************************************************
���������ʵ��
���README�ĵ�˵��
************************************************/

 int main(void)
 {		
 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200

	 //EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��

	Timer_SRD_Init(5000,7199);
	Wave_SRD_Init();
	 
	while(1)   //����
	{	    
		Wave_SRD_Strat();	

    delay_ms(1000);
		delay_ms(1000);
	
		
		
        	
	}
 }

