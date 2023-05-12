
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"

u16 count = 0; //码盘计数
u16 count_temp = 0; //码盘计数
u16 timer_count = 0; //定时器计数


//引脚定义!!!

sbit moto=P1^6;
sbit beep=P5^5;		// 蜂鸣器定义




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


void Timer0Init(void)		//10毫秒@24.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xE0;		//设置定时初值
	TH0 = 0xB1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 =1;			 //使能定时器中断
}



//主函数
void main(void)
{ 
	
	IO_init(); //IO口初始化		
	LCD_Init();//液晶屏初始化
	delay1ms(10);	
	LCD_Clear(WHITE);//清屏白底	
	
	
	
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5); //设置P55为推挽输出 -->蜂鸣器
	P1M1 &= ~(1<<6),P1M0 |=  (1<<6); //设置P16为推挽输出	
	moto = 1;  //打开直流电机转动
	beep = 1; 
	
	IT1 =1;			//设置INT1的中断类型（1：仅下降沿 0：上升沿货下降沿）
  EX1 =1;           //使能INT1中断                                            
  
	
	Timer0Init();   //定时器0初始化
	
	count = 0;
	timer_count = 0;
	
	EA  =1;           // 开总中断 	
	
	while(1)
	{		
		if(timer_count >99)
		{
			EA  =0;           // 关总中断 			
			LCD_ShowNum(136,56,count,4,16);			
			count = 0;
			timer_count = 0;			
			EA  =1;           // 开总中断 
		
		}
	}		
	
}	
	

void exint0()  interrupt 2 	//外部中断1中断
{
	 count ++ ;	

}

void tm0() interrupt 1 using 1  //定时器中断
{
 timer_count ++ ;
}
















