#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "motor.h"
 
 
/************************************************
 �������ʵ�� 
 ���README�ĵ�˵��
************************************************/


 int main(void)
 {	
	delay_init();	    //��ʱ������ʼ��	  
	 
	MOTOR_Init();  //�������IO��ʼ�� 


	while(1)
	{
		int i;
		for(i = 0;i < 1000;i++)      //�����ת
			stepper(Pos,5);    //���������5ms  
		delay_ms(1000);    //��ʱ
		for(i = 0;i < 1000;i++)      //�����ת
			stepper(Neg,8); 
		delay_ms(1000);    //��ʱ
	}

	
	
 }

