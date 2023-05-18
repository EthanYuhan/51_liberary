#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
 
 
/************************************************
�����ʵ��
���README�ĵ�˵��
************************************************/


int main(void)
 {
	  u8 i;
	  delay_init();	    //��ʱ��ʼ������
	  LED_Init();       //����ܶ˿ڳ�ʼ������

	

	while(1)
	{

		for(i=0;i<8;i++)
		{
			switch(i)	 //λѡ��ѡ�����������ܣ�
			{
				case(0):
					LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
				case(1):
					LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
				case(2):
					LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
				case(3):
					LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ      
				case(4):
					LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
				case(5):
					LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
				case(6):
					LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
				case(7):
					LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ	
			}
			Display(i);  //�������ʾ0   
			delay_ms(1); //���һ��ʱ��ɨ��	

			GPIOC->ODR&=0XFFFFFF00;//����

		}
		
	}		

	
 }



 

