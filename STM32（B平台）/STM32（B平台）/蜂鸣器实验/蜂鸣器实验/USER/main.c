#include "sys.h"	
#include "delay.h"	
#include "beep.h" 

 
/************************************************
 ������ʵ�� 
 

************************************************/

 int main(void)
 {
	delay_init();	    	 //��ʱ������ʼ��	  
	BEEP_Init();         	//��ʼ���������˿�
	while(1)
	{

		BEEP=0;		  
		delay_ms(300);//��ʱ300ms
 
		BEEP=1;  
		delay_ms(300);//��ʱ300ms
	}
 }

