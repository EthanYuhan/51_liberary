														 
#include "stc15f2k60s2.h"


/* 
蜂鸣器实验

详见README文件
*/



sbit BEEP_IO=P5^5;

//针对 STC15W4K56S4 系列 IO口初始化
//io口初始化 P0 P1 P2 P3 P4 为准双向IO口   
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



void delay(unsigned int m)			//延时函数
     {
	  int  a, b;

	 for(a=0;a<5000;a++)
	 for(b=0;b<m;b++);
	   
	 }

main()
{
	
	IO_init();				   //针对 STC15W4K56S4 IO口初始化
	
	//设置STC 单片机的P55 为推挽输出
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5); 
	
	while(1)
	{		
			
		BEEP_IO=~BEEP_IO;   //
			
		delay(200);		



	}
}

