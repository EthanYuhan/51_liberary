
#include "stc15f2k60s2.h"

																								
/* 
����Թܼ��ʵ��
���README�ļ�
*/



sbit led=P5^3;		// LED�ƶ���
sbit inf=P1^4;		// ���������Ŷ���


//��� STC15W4K32S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4��Ϊ׼˫��IO��   

void IO_init(void)
{
  P0M0 = 0X00;
	P0M1 = 0X00;

	P1M0 = 0X00;
  P1M1 = 0X00;

	P2M0 = 0X00;
	P2M1 = 0X00;

	P3M0 = 0X00;
	P3M1 = 0X00;

	P4M0 = 0X00;
	P4M1 = 0X00;  
	
	P5M0 = 0X00;
	P5M1 = 0X00;  
	
	P6M0 = 0X00;
	P6M1 = 0X00;  
	
	P7M0 = 0X00;
	P7M1 = 0X00;   
}

void main() 
{
  IO_init();				 //��� STC15W4K32S4  IO�ڳ�ʼ��

  while(1)
	{
		if(inf==0)
		{
		led= 0;  
		}
		else
		{
		led= 1;
		}
	
	}
}




 


 



