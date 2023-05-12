
#include "stc15f2k60s2.h"

																								
/* 
红外对管检测实验
详见README文件
*/



sbit led=P5^3;		// LED灯定义
sbit inf=P1^4;		// 红外检测引脚定义


//针对 STC15W4K32S4 系列 IO口初始化
//io口初始化 P0 P1 P2 P3 P4等为准双向IO口   

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

void main() 
{
  IO_init();				 //针对 STC15W4K32S4  IO口初始化

  while(1)
	{
		if(inf==0)
		{
		led= 0;  
		}
		else
		{
		led= 1;
		}
	
	}
}




 


 



