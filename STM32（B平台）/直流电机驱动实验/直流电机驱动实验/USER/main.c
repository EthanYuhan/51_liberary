#include "sys.h"	
#include "delay.h"	
#include "dcmotor.h" 

 
/************************************************
 ������ʵ�� 
 

************************************************/

 int main(void)
 {
	delay_init();	    	 //��ʱ������ʼ��	  
	DCMOTOR_Init();         	//��ʼ��ֱ������˿�
	while(1)
	{

		DCMOTOR1=1;
    	
		delay_ms(1800);//��ʱ1800ms
 
		DCMOTOR1=0;  
		
		delay_ms(1800);//��ʱ1800ms		

	}
 }

