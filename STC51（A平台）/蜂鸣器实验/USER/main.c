														 
#include "stc15f2k60s2.h"


/* 
������ʵ��

���README�ļ�
*/



sbit BEEP_IO=P5^5;

//��� STC15W4K56S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   
void IO_init(void)
{
  P0M0 = 0X00;	P0M1 = 0X00;

	P1M0 = 0X00;  P1M1 = 0X00;

	P2M0 = 0X00;	P2M1 = 0X00;

	P3M0 = 0X00;	P3M1 = 0X00;

	P4M0 = 0X00;	P4M1 = 0X00;  
	
	P5M0 = 0X00;	P5M1 = 0X00;  
	
	P6M0 = 0X00;	P6M1 = 0X00;  
	
	P7M0 = 0X00;	P7M1 = 0X00;  
}



void delay(unsigned int m)			//��ʱ����
     {
	  int  a, b;

	 for(a=0;a<5000;a++)
	 for(b=0;b<m;b++);
	   
	 }

main()
{
	
	IO_init();				   //��� STC15W4K56S4 IO�ڳ�ʼ��
	
	//����STC ��Ƭ����P55 Ϊ�������
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5); 
	
	while(1)
	{		
			
		BEEP_IO=~BEEP_IO;   //
			
		delay(200);		



	}
}

