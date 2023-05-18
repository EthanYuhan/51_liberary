#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "dht11.h"

/*			
��ʪ�ȴ�����ʵ��
���README�ļ�			
   
*/

void IO_init(void){
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

	IO_init(); //IO�ڳ�ʼ��
	
	LCD_Init();	//Һ������ʼ��
	LCD_Clear(WHITE);//�����׵�	
	Gui_StrCenter(0,16,RED,BLUE,"DHT11  ���Գ���",16,1);//������ʾ

	Show_Str(0,56,BLUE,WHITE,"The temperature",16,0); 	
	Show_Str(120,56,BLUE,WHITE,":   . ",16,0);
	
	Show_Str(0,96,BLUE,WHITE,"The humidity",16,0); 	
	Show_Str(120,96,BLUE,WHITE,":   . ",16,0);

	
	while(1)
	{	

		RH();
	LCD_ShowNum(136,56,U8T_data_H,2,16);
	LCD_ShowNum(154,56,U8T_data_L,2,16);
	LCD_ShowNum(136,96,U8RH_data_H,2,16);
	LCD_ShowNum(154,96,U8RH_data_L,2,16);
	delay_ms(2000); //��ʱ����	
	}
	

		
	
}
