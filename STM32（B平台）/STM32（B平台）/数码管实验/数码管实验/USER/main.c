#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
 
 
/************************************************
数码管实验
详见README文档说明
************************************************/


int main(void)
 {
	  u8 i;
	  delay_init();	    //延时初始化程序
	  LED_Init();       //数码管端口初始化函数

	

	while(1)
	{

		for(i=0;i<8;i++)
		{
			switch(i)	 //位选，选择点亮的数码管，
			{
				case(0):
					LSA=0;LSB=0;LSC=0; break;//显示第0位
				case(1):
					LSA=1;LSB=0;LSC=0; break;//显示第1位
				case(2):
					LSA=0;LSB=1;LSC=0; break;//显示第2位
				case(3):
					LSA=1;LSB=1;LSC=0; break;//显示第3位      
				case(4):
					LSA=0;LSB=0;LSC=1; break;//显示第4位
				case(5):
					LSA=1;LSB=0;LSC=1; break;//显示第5位
				case(6):
					LSA=0;LSB=1;LSC=1; break;//显示第6位
				case(7):
					LSA=1;LSB=1;LSC=1; break;//显示第7位	
			}
			Display(i);  //数码管显示0   
			delay_ms(1); //间隔一段时间扫描	

			GPIOC->ODR&=0XFFFFFF00;//消隐

		}
		
	}		

	
 }



 

