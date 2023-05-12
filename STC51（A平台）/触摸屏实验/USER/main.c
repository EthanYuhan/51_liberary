
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


//主函数
void main(void)
{ 
	
	IO_init(); //IO口初始化		
	LCD_Init();//液晶屏初始化
	delay1ms(10);	
	LCD_Clear(BLUE);//清屏白底	
	

	//循环进行各项测试	
	while(1)
	{

		Touch_Test();		//触摸屏手写测试

	}   
}
