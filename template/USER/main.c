#include "stc15f2k60s2.h"
#include <intrins.h>
#include <stdio.h>
#include "string.h"
#include "sys.h"
#include "lcd12864.h"
#include "key.h"
#include "timer.h"
#include "ad.h"
#include "ds1302.h"
#include "dht11.h"

u8 code CharCodeLine1[]="CZCJJQ";       //第一行显示字符
u8 code CharCodeLine2[]="0302";         //第二行显示字符
u8 code MileValue[]="mile：       km";  //显示行驶里程
u8 code CostValue[]="cost：     yuan";  //显示应收取费用
u8 code WaiteTimeValue[]="delay ：    min"; //显示应收取费用

u8  StartDrive = 0 ;  //0-停车服务结束 1-开始服务计价中 2-静止等待状态
u16 MileNum = 0; 	  //里程计数
u16 CostNum = 0;      //应收费用计数
u16 WaiteTimeNum = 0; //等待时间
u16 StartPrice = 6;   //起步价 默认 6
u8  TimeNum = 1;      //时段1/2 默认为1
u16 ad;               //ADC采样值
u8  MoterSpeed =8;    //通过定时器2控制步进电机转速
u8  AdcFlag = 0 ;     //adc采样标志
u16 InterruptCount = 0 ;  //码盘中断计数

extern u8 KeyCode;      //按键键值
extern u16 TimerCount ; //定时器计数
extern u8 PulseNum;     //脉冲计数
extern u16 WaiteTimeCount; //静止状态等待时间计数

sbit people_dec = P0^1;

void IO_init(void){  //IO口初始化
	//IO配置为准双向IO口 
	P0M0 = 0X00;	P0M1 = 0X00;  
	P1M0 = 0X00;	P1M1 = 0X00;
	P2M0 = 0X00;	P2M1 = 0X00;
	P3M0 = 0X00;	P3M1 = 0X00;
	P4M0 = 0X00;	P4M1 = 0X00;  	
	P5M0 = 0X00;	P5M1 = 0X00;  	
	P6M0 = 0X00;	P6M1 = 0X00;  	
	P7M0 = 0X00;	P7M1 = 0X00;  
	
	//设置推挽输出
	P2M1 &= ~(1<<2),P2M0 |=  (1<<2);//12864LCD设置
	P2M1 &= ~(1<<1),P2M0 |=  (1<<1); 
	P2M1 &= ~(1<<0),P2M0 |=  (1<<0); 
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3);//步进电机
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5);//蜂鸣器
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2);//继电器
}


void main(void){  //主函数
	u8 i;	
	u8 temp;
	IO_init();				 //IO口初始化		
	LCD12864_Init();         //12864液晶初始?
	LCD12864_WriteCmd(0x34);
	LCD12864_ClearScreen();	
	LCD12864_Roll();
	
	while(1){  //主循环
		if(people_dec==0){
			P53=0;
		}else{
			P53=1;
		}	
	}
}




void exint0()  interrupt 2 //外部中断1中断
{
	InterruptCount ++ ;    //码盘一圈20格
}





