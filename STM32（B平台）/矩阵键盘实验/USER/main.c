//#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"

 
/************************************************

 TFTLCD��ʾʵ��  

************************************************/
 	
 int main(void)
 {	 

	u8 key_value;

	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 //	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	KEY_Init(); //���󰴼���ʼ��   

	 
	 
	 POINT_COLOR=RED;	  
	 LCD_ShowString(30,70,200,16,16,"KEY TEST");
	 LCD_ShowString(30,90,200,16,16,"STM32F103ZET6");
	 LCD_ShowString(30,130,200,12,12,"2020/5/4");
	 LCD_ShowString(30,170,200,16,16,"key_value:");
	 
	 while(1) 
	{	
		
		key_value =  keyhandle();
		
		if(key_value > 0)
		{		
			
			LCD_ShowNum(110,170,key_value,4,16);

		}
		
	} 
}
