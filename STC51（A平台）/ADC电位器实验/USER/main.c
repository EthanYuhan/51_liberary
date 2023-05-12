
#include "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include <stdio.h>
#include "string.h"
#include "ad.h"


/*
ADC��λ��ʵ��
���README�ĵ�
*/

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
	u16 ad;
	u8 buf[10] ;
	
	IO_init(); //IO�ڳ�ʼ��

	//Һ������ʼ��
	LCD_Init();	
	LCD_Clear(WHITE);//�����׵�	
	Gui_StrCenter(0,16,RED,BLUE,"ADC ���Գ���",16,1);//������ʾ	
	Show_Str(20,56,BLUE,YELLOW,"Voltage",16,0);
	Show_Str(101,56,BLUE,YELLOW,": .  V",16,0);
	
	
	//ѭ��
	while(1)
	{
		
	delay_ms(500);
	
	ad=GetADCResult(0);			 //��ȡ��ѹ����ֵ  P1.0��  �ɼ�AD��ѹֵ
		
    ad=(float)ad*3.3*100/1024;      //3.3v������
								//5/1024 5v��ѹ��1024�� ֵΪÿһ�ݵ�ֵ adΪ�ɼ��ķ���  �������Ϊ�ɼ��ĵ�ѹֵ *100С�������λ 
                                 //10λAD�ɼ� ��2��10�η� ��ֵΪ1024 ������1024��ʾ5���ĵ�ѹ 
					             //��ô�òɼ���������ֵ ����1024 �ڳ���5 �õ���ֵ���ǲɼ��ĵ�ѹ��ֵ

	//�� 3.3v ��ѹΪ��׼������ɼ�ֵ�ĵ�ѹ��������ʾ���� ������
		
	sprintf((char *)buf,"%d",(ad/100%10));		
    Show_Str(109,56,BLUE,YELLOW,buf,16,0);  //��ʾ��ѹֵ
	sprintf((char *)buf,"%d",(ad/10%10));	
	Show_Str(125,56,BLUE,YELLOW,buf,16,0);  //��ʾ��ѹֵ
	sprintf((char *)buf,"%d",(ad%10));	
	Show_Str(133,56,BLUE,YELLOW,buf,16,0);  //��ʾ��ѹֵ
		
		
	delay_ms(500);			
		
		
	}   
}
