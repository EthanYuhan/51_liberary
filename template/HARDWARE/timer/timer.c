#include "sys.h"
#include "timer.h"
#include "stc15f2k60s2.h"
#include "ad.h"


//定制器和步进电机控制代码


u16 TimerCount = 0;
u8  PulseNum = 0;   	//脉冲计数
u8  MoterCount = 0; 	//步进电机控制计数
u16 AdcCount = 0 ; 		//adc采样计数
u16 WaiteTimeCount = 0; //静止状态等待时间计数

extern u8 MoterSpeed; //定制器2控制步进电机转速
extern u8 AdcFlag ;	  //adc采样标志
extern u16 ad; 		  //ADC采样值




// 步进电机逆时针旋转相序表
u8 code CCW[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};

// 步进电机四相的引脚定义
sbit A1 = P1^1;
sbit B1 = P1^2;
sbit C1 = P1^3;
sbit D1 = P1^4;

// 给步进电机每个相赋值
void Give(unsigned char dat)
{
	A1 = dat & 0x08;
	B1 = dat & 0x04;
	C1 = dat & 0x02;
	D1 = dat & 0x01;
}



//定时器0初始化
void Timer0Init(void)//10毫秒@24.000MHz
{
	AUXR &= 0x7F;	//定时器时钟12T模式
	TMOD &= 0xF0;	//设置定时器模式
	TL0 = 0xE0;		//设置定时初值
	TH0 = 0xB1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

//定时器2初始化
void Timer2Init(void)//250微秒@24.000MHz
{
	AUXR &= 0xFB;	//定时器时钟12T模式
	T2L = 0x0C;		//设置定时初值
	T2H = 0xFE;		//设置定时初值
	AUXR |= 0x10;	//定时器2开始计时
}






void tm0() interrupt 1 using 1 //定时器0中断
{
	
  WaiteTimeCount ++ ;          //10毫秒加一

}


	


void tm2() interrupt 12  //定时器2中断
{
#if DRIVING_SIMULATION	
	
	TimerCount ++ ;     //定时器2脉冲模拟效果
	if(TimerCount>4000) //定时超过 1秒
	{		
		PulseNum ++ ;
		TimerCount = 0;
	}
	
#else
	
	TimerCount ++ ; 	
	if(TimerCount >MoterSpeed)   //定时超过  MoterSpeed为步进电机速度控制
	{
		MoterCount ++ ;  			
		TimerCount = 0;		
		Give(CCW[MoterCount%8]); //控制电机
	}	
	if( MoterCount%64==63 )      //定时ADC采样标志位
	{
		MoterCount = 0;		
		AdcFlag = 1;
	}	
	
#endif	
		
}


