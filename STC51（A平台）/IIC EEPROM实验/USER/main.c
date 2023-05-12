
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "24cxx.h"

/*			
	*ʵ����24C256 ����֤�����Ƿ����
	*�����Ӿ�����̳���
	
	
			
I2C��24C02�� д���� 
     ˵�� �˴�ʵ����Ҫ���� P1.5 P1.6 IO��ģ��I2C����  
	 
	 

   
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
	u8 pd;
	u8 buf[10] ;

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	LCD_Clear(WHITE);//�����׵�	
	Gui_StrCenter(0,16,RED,BLUE,"24C02 read and write���Գ���",16,1);//������ʾ	
	

	pd=AT24C_Rcvone(0);    //ѡ��Ҫ��ȡ�ĵ�ַ  Ȼ�󽫶�ȡ�����ݴ������pd��
							//1����1���ֽڵ������ָ������ 
	sprintf((char *)buf,"AT24C: %c",pd);	
	Show_Str(109,56,BLUE,YELLOW,buf,16,0); 
	
	pd='F';		    //��Ԥ�洢Count1�е�ֵ������pd
	AT24C_Sendone(0,pd);	//��pd��ֵ����  0��ַλ�� ������Ϊ1�ֽ�
	
	
	
	while(1); //��ֹ�����ܷ�
	

		
	
}
