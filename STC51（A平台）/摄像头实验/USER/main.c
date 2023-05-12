
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"
#include  "pff.h"				  //�ļ�ϵͳ����.h����
#include  "sram.h"
#include  "ov7670.h"


/*			
*���ź�ԭ��ͼһֱ  �����ֱ����֤ 


				   //LCD_RESET �ĳ� P3^3 ���������������� 
				   //LCD_RESET ֱ�ӽ�3.3V VCC
				   //P1��Ϊ������״̬,Ҫ����P1�ڵĸ��ţ�������
					 
					 
					 //��ֲ����ʷ�汾 ���ԣ�����
	  	                                           		    	  
*/





FATFS fatfs;	//�ļ�ϵͳ�ṹ�嶨��
u8 tbuf[512];			   //512�ֽ� SD�����ݻ���ռ�


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
	SP=0X80;				    //������ջָ��  

	IO_init(); 					//IO�ڳ�ʼ��	
	LCD_Init();					//Һ������ʼ��
	Init_SPI(); 				//SPI��ʼ�� 
	
	LCD_Clear(WHITE);   //�����׵�		

  SD_Init();			     //SD����ʼ��
  pf_mount(&fatfs);	   //��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����
											 //���ǳ���Ҫ������ʹ������Petit Fatfs�ļ�ϵͳ���ܵ�ǰ��

	
	get_font_sector();		  //��ȡTF����  GBK������  ������
													//����tf�ֿ���к�����ʾ

  ov7670_test();			  //ov7670���Ժ���
  while(1);
	

	
	
}





