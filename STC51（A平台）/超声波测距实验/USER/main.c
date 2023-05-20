
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
//#include "uart.h"

/*
���������ʵ��
���READMEʵ��
*/


sbit Trig = P1^5;
sbit Echo = P3^4;


static unsigned char DisNum = 0; //��ʾ��ָ��				  
unsigned int  time=0;
unsigned long S=0;
bit      flag =0;

	   
void Conut(void)
	{

											  
	 S=(time*1.7)/10;     //�������CM 
		
		time = 0 ;
		
	 if((S>=700)||flag==1) //����������Χ��ʾ��-��
	 {	 
	  flag=0;
	 
	 Show_Str(20,30,BLUE,YELLOW,"out of range!",16,1);
		 
	 }
	 else
	 {	
		 
	  Show_Str(20,30,BLUE,YELLOW,"distance:    CM",16,1);
		 
		LCD_ShowNum(92,30,(u32)S,4,16);

		 
	 }
	}

void zd0() interrupt 1 using 1	 //T0�ж��������������,������෶Χ
  {
    //flag=1;							 //�ж������־
		time ++ ;
		
  }

 void  StartModule() 		         //����ģ��
  {
	  Trig=1;			                     //����һ��ģ��
	  	  
	  delay4us(10); //����10us�ĸߵ�ƽ	  	 
	  	  
	  Trig=0;

  }

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
	
	
	P1M1 &= ~(1<<5),P1M0 |=  (1<<5); //���� P15 Ϊ�������	
	P3M1 |=(1<<4),P3M0 &=~(1<<4);    //���� P34Ϊ����
	
	Trig=0;	//������

}


//������
void main(void)
{ 
	
	IO_init(); //IO�ڳ�ʼ��		
	LCD_Init();//Һ������ʼ��
	delay1ms(10);	
	LCD_Clear(WHITE);//�����׵�	
	
	delay1ms(200);		
	
	Show_Str(20,0,BLUE,YELLOW,"ULTRA Test",16,1);
	 
								    //10΢��@24.000MHz
	
			AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
			TMOD &= 0xF0;		//���ö�ʱ��ģʽ  16�Զ���װ
			TL0 = 0xEC;		//���ö�ʱ��ֵ
	    TH0 = 0xFF;		//���ö�ʱ��ֵ
	   ET0 =1;			 //ʹ�ܶ�ʱ���ж�
	   EA  =1;			 //�����ж�

	
	while(1)
	{
		 StartModule();		
		
	     while(!Echo);		//��RXΪ��ʱ�ȴ�
	     
		   ET0 =1;				//����ʱ���ж�
	    	TR0=1;			    //��������
	     while(Echo);			//��RXΪ1�������ȴ�
	     
			 ET0 =0;				//�ض�ʱ���ж�
	    	TR0=0;				//�رռ���
       Conut();			//����
		
		 delay1ms(80);
	}
}
