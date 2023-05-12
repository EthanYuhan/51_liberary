#include "sys.h"
#include "timer.h"
#include "stc15f2k60s2.h"
#include "ad.h"


//�������Ͳ���������ƴ���


u16 TimerCount = 0;
u8  PulseNum = 0;   	//�������
u8  MoterCount = 0; 	//����������Ƽ���
u16 AdcCount = 0 ; 		//adc��������
u16 WaiteTimeCount = 0; //��ֹ״̬�ȴ�ʱ�����

extern u8 MoterSpeed; //������2���Ʋ������ת��
extern u8 AdcFlag ;	  //adc������־
extern u16 ad; 		  //ADC����ֵ




// ���������ʱ����ת�����
u8 code CCW[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};

// ���������������Ŷ���
sbit A1 = P1^1;
sbit B1 = P1^2;
sbit C1 = P1^3;
sbit D1 = P1^4;

// ���������ÿ���ำֵ
void Give(unsigned char dat)
{
	A1 = dat & 0x08;
	B1 = dat & 0x04;
	C1 = dat & 0x02;
	D1 = dat & 0x01;
}



//��ʱ��0��ʼ��
void Timer0Init(void)//10����@24.000MHz
{
	AUXR &= 0x7F;	//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;	//���ö�ʱ��ģʽ
	TL0 = 0xE0;		//���ö�ʱ��ֵ
	TH0 = 0xB1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

//��ʱ��2��ʼ��
void Timer2Init(void)//250΢��@24.000MHz
{
	AUXR &= 0xFB;	//��ʱ��ʱ��12Tģʽ
	T2L = 0x0C;		//���ö�ʱ��ֵ
	T2H = 0xFE;		//���ö�ʱ��ֵ
	AUXR |= 0x10;	//��ʱ��2��ʼ��ʱ
}






void tm0() interrupt 1 using 1 //��ʱ��0�ж�
{
	
  WaiteTimeCount ++ ;          //10�����һ

}


	


void tm2() interrupt 12  //��ʱ��2�ж�
{
#if DRIVING_SIMULATION	
	
	TimerCount ++ ;     //��ʱ��2����ģ��Ч��
	if(TimerCount>4000) //��ʱ���� 1��
	{		
		PulseNum ++ ;
		TimerCount = 0;
	}
	
#else
	
	TimerCount ++ ; 	
	if(TimerCount >MoterSpeed)   //��ʱ����  MoterSpeedΪ��������ٶȿ���
	{
		MoterCount ++ ;  			
		TimerCount = 0;		
		Give(CCW[MoterCount%8]); //���Ƶ��
	}	
	if( MoterCount%64==63 )      //��ʱADC������־λ
	{
		MoterCount = 0;		
		AdcFlag = 1;
	}	
	
#endif	
		
}


