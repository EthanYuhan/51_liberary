
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"


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


//������
void main(void)
{ 
	
	IO_init(); //IO�ڳ�ʼ��		
	LCD_Init();//Һ������ʼ��
	delay1ms(10);	
	LCD_Clear(BLUE);//�����׵�	
	

	//ѭ�����и������	
	while(1)
	{

		Touch_Test();		//��������д����

	}   
}
