#include "sys.h"
#include "timer.h"

u16 TimerCount = 0;
u8 GoTo5FloorFlag = 0; //电梯是否空闲去5楼标志位

void Timer0Init(void)		//20毫秒@24.000MHz
{
//	AUXR &= 0x7F;		//定时器时钟12T模式
//	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0xC0;		//设置定时初值
//	TH0 = 0x63;		//设置定时初值
//	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
//	
//	ET0 =1;			 //使能定时器中断
//	EA  =1;			 //开总中断
	
}

void tm0() interrupt 1 using 1
{
	TimerCount ++ ;
	
	if(TimerCount>1000) //定时超过3s  !!!!!测试20s
	{
		
		GoTo5FloorFlag = 1;
		TimerCount = 0;
	
	}
 

			
}