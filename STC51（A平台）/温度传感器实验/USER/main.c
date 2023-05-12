
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "ds18b20.h"

/*			
�¶ȴ�����ʵ��
���README�ĵ�			
   
*/

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



//������
void main(void)
{ 
	u8 buf[10];
	
	short temperature;			//�¶ȱ���
	IO_init(); //IO�ڳ�ʼ��
	
	LCD_Init();	//Һ������ʼ��
	LCD_Clear(WHITE);//�����׵�	
	Gui_StrCenter(0,16,RED,BLUE,"DS18B20 read temperature���Գ���",16,1);//������ʾ	

	
	Show_Str(0,56,BLUE,YELLOW,"The temperature",16,0); 	
	Show_Str(120,56,BLUE,YELLOW,":   . ",16,0); 
		
	
	while(1)
	{
	 temperature=read_temp();										   //��ȡ�¶�ֵ
     if(temperature<0) Show_Str(28,56,BLUE,YELLOW,'-',16,0); 
     else Show_Str(128,56,BLUE,YELLOW,'+',16,0); 
	 sprintf((char *)buf,"%d",(temperature/100%10));		
	 Show_Str(136,56,BLUE,YELLOW,buf,16,0); 
		sprintf((char *)buf,"%d",(temperature/10%10));		
	 Show_Str(144,56,BLUE,YELLOW,buf,16,0); 
		sprintf((char *)buf,"%d",(temperature%10));		
	 Show_Str(160,56,BLUE,YELLOW,buf,16,0); 
	
	
	}
	

		
	
}
