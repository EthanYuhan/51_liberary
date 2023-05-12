
#include "stc15f2k60s2.h"


/* 
����ܲ���
���README�ļ�
*/


typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit LSA=P2^0;
sbit LSB=P2^1;
sbit LSC=P2^2;


u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ
					
//��� STC15W4K56S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   					
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


/*******************************************************************************
* �� �� ��         : delay
* ��������		   : ��ʱ������i=1ʱ����Լ��ʱ10us     ����֤�滻
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ����ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
*******************************************************************************/
void DigDisplay()
{
	u8 i;
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
		P0=smgduan[i];//���Ͷ���    
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}

	


main()
{

	IO_init();				   //��� STC15W4K56S4  IO�ڳ�ʼ��
	
	while(1)
	{
		DigDisplay();  //�������ʾ����	


	}
}

