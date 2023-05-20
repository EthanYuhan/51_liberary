
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
//#include "uart.h"

/*
超声波测距实验
详见README实验
*/


sbit Trig = P1^5;
sbit Echo = P3^4;


static unsigned char DisNum = 0; //显示用指针				  
unsigned int  time=0;
unsigned long S=0;
bit      flag =0;

	   
void Conut(void)
	{

											  
	 S=(time*1.7)/10;     //算出来是CM 
		
		time = 0 ;
		
	 if((S>=700)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
	 
	 Show_Str(20,30,BLUE,YELLOW,"out of range!",16,1);
		 
	 }
	 else
	 {	
		 
	  Show_Str(20,30,BLUE,YELLOW,"distance:    CM",16,1);
		 
		LCD_ShowNum(92,30,(u32)S,4,16);

		 
	 }
	}

void zd0() interrupt 1 using 1	 //T0中断用来计数器溢出,超过测距范围
  {
    //flag=1;							 //中断溢出标志
		time ++ ;
		
  }

 void  StartModule() 		         //启动模块
  {
	  Trig=1;			                     //启动一次模块
	  	  
	  delay4us(10); //大于10us的高电平	  	 
	  	  
	  Trig=0;

  }

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
	
	
	P1M1 &= ~(1<<5),P1M0 |=  (1<<5); //设置 P15 为推挽输出	
	P3M1 |=(1<<4),P3M0 &=~(1<<4);    //配置 P34为输入
	
	Trig=0;	//先拉高

}


//主函数
void main(void)
{ 
	
	IO_init(); //IO口初始化		
	LCD_Init();//液晶屏初始化
	delay1ms(10);	
	LCD_Clear(WHITE);//清屏白底	
	
	delay1ms(200);		
	
	Show_Str(20,0,BLUE,YELLOW,"ULTRA Test",16,1);
	 
								    //10微秒@24.000MHz
	
			AUXR &= 0x7F;		//定时器时钟12T模式
			TMOD &= 0xF0;		//设置定时器模式  16自动重装
			TL0 = 0xEC;		//设置定时初值
	    TH0 = 0xFF;		//设置定时初值
	   ET0 =1;			 //使能定时器中断
	   EA  =1;			 //开总中断

	
	while(1)
	{
		 StartModule();		
		
	     while(!Echo);		//当RX为零时等待
	     
		   ET0 =1;				//开定时器中断
	    	TR0=1;			    //开启计数
	     while(Echo);			//当RX为1计数并等待
	     
			 ET0 =0;				//关定时器中断
	    	TR0=0;				//关闭计数
       Conut();			//计算
		
		 delay1ms(80);
	}
}
