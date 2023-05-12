
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "ds1302.h"

/*			
*DS1302����  
*CE-P5^4  I/O-P3^6  SCLK-P3^5  	

DS1302 ʱ��оƬ����
     ˵�� ������DS1302оƬ ʹ�õ�����Ϊ	  
	      
		  ����DS1302�ĳ�ʼ��	Set_RTC();  ��������ǳ�ʼ��DS1302�ĺ���
		  ˵���˾���DS1302�����ϵ��ʱ��һ��Ҫ ��������һ��ʱ�� �����ȡʱ �������������
		  ��Ҳ������һ��ȱ��� 
		  �����һ�γ��� д��ʱ�� Ȼ���´β����ʱ�� ��ô��Ҫ��	Set_RTC();������ ������DS1302��������
		  �ں�����ۺϳ����� ���ǽ���һ�ֶ��������÷��� ��������һ�� �Ժ�Ͳ��������� ��������ڽ��ܰ�

		  DS1302�ľ��幤��ԭ��  ��Ҿ�����ԭ��ͼ ��DS1302�����ֲ� ����  ���ﲻ��������

   
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

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	LCD_Clear(WHITE);//�����׵�	
	Gui_StrCenter(0,0,RED,BLUE,"DS1302 TIME���Գ���",16,1);//������ʾ	

	Set_RTC();				   //DS1302ʱ�ӳ�ʼ��   
	while(1)
	{
	 ds1302_scan(50,60,RED,WHITE);		//������ʾ
    }					 
	
	
}





