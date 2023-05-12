//#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"

 
/************************************************

 TFTLCD显示实验  

************************************************/
 	
 int main(void)
 {	 

	u8 key_value;

	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 //	LED_Init();			     //LED端口初始化
	LCD_Init();
	KEY_Init(); //矩阵按键初始化   

	 
	 
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
