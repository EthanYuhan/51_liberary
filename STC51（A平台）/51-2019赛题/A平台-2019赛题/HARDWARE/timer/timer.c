#include "sys.h"
#include "timer.h"

u16 TimerCount = 0;
u8 GoTo5FloorFlag = 0; //�����Ƿ����ȥ5¥��־λ

void Timer0Init(void)		//20����@24.000MHz
{
//	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
//	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0xC0;		//���ö�ʱ��ֵ
//	TH0 = 0x63;		//���ö�ʱ��ֵ
//	TF0 = 0;		//���TF0��־
//	TR0 = 1;		//��ʱ��0��ʼ��ʱ
//	
//	ET0 =1;			 //ʹ�ܶ�ʱ���ж�
//	EA  =1;			 //�����ж�
	
}

void tm0() interrupt 1 using 1
{
	TimerCount ++ ;
	
	if(TimerCount>1000) //��ʱ����3s  !!!!!����20s
	{
		
		GoTo5FloorFlag = 1;
		TimerCount = 0;
	
	}
 

			
}