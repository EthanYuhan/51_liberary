
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"

u16 count = 0; //���̼���
u16 count_temp = 0; //���̼���
u16 timer_count = 0; //��ʱ������


//���Ŷ���!!!

sbit moto=P1^6;
sbit beep=P5^5;		// ����������




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


void Timer0Init(void)		//10����@24.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xE0;		//���ö�ʱ��ֵ
	TH0 = 0xB1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 =1;			 //ʹ�ܶ�ʱ���ж�
}



//������
void main(void)
{ 
	
	IO_init(); //IO�ڳ�ʼ��		
	LCD_Init();//Һ������ʼ��
	delay1ms(10);	
	LCD_Clear(WHITE);//�����׵�	
	
	
	
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5); //����P55Ϊ������� -->������
	P1M1 &= ~(1<<6),P1M0 |=  (1<<6); //����P16Ϊ�������	
	moto = 1;  //��ֱ�����ת��
	beep = 1; 
	
	IT1 =1;			//����INT1���ж����ͣ�1�����½��� 0�������ػ��½��أ�
  EX1 =1;           //ʹ��INT1�ж�                                            
  
	
	Timer0Init();   //��ʱ��0��ʼ��
	
	count = 0;
	timer_count = 0;
	
	EA  =1;           // �����ж� 	
	
	while(1)
	{		
		if(timer_count >99)
		{
			EA  =0;           // �����ж� 			
			LCD_ShowNum(136,56,count,4,16);			
			count = 0;
			timer_count = 0;			
			EA  =1;           // �����ж� 
		
		}
	}		
	
}	
	

void exint0()  interrupt 2 	//�ⲿ�ж�1�ж�
{
	 count ++ ;	

}

void tm0() interrupt 1 using 1  //��ʱ���ж�
{
 timer_count ++ ;
}
















