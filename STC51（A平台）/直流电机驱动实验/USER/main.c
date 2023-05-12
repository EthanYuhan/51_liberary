/* 
ֱ���������ʵ��
*/


#include "stc15f2k60s2.h"

sbit moto1=P1^5;	
sbit moto2=P1^6;	



//��� STC15W4K56S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   
//ע��: STC15W4K32S4ϵ�е�оƬ,�ϵ��������PWM��ص�IO�ھ�Ϊ
//      ����̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ��
//���IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
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
	
	IO_init();				   //��� STC15W4K56S4  IO�ڳ�ʼ��		
	
	//����STC ��Ƭ����P15 P16Ϊ�������
	P1M1 &= ~(1<<5),P1M0 |=  (1<<5); 
	P1M1 &= ~(1<<6),P1M0 |=  (1<<6); 

	
	while(1)
	{
		moto1 = 1;	
		moto2 = 1;
		
		delay(1000);

		moto1 = 0;	
		moto2 = 0;
		
		delay(1000);	
	
	}
	

}

