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
#include "motor.h"
#include "mpu6050.h"

u8 code CharCodeLine1[]="CZCJJQ";       //第一行显示字符
u8 code CharCodeLine2[]="0302";         //第二行显示字符


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
	//P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	//P5M1 &= ~(1<<5),P5M0 |=  (1<<5);//蜂鸣器
	//P5M1 &= ~(1<<2),P5M0 |=  (1<<2);//继电器
}

void main(void){  //主函数
	IO_init();				 //IO口初始化		
	LCD12864_Init();         //12864液晶初始
	motor_init();
	InitMPU6050();
	//LCD12864_ClearScreen();	
	//LCD12864_Roll();

	
	
	delay_ms(1000);
	
	while(1){  //主循环
		print_n(0,0,GetData(ACCEL_XOUT_H),5);
		print_n(1,0,GetData(ACCEL_YOUT_H),5);
		print_n(2,0,GetData(ACCEL_ZOUT_H),5);
		delay_ms(200);
		
	}
}

void exint0()  interrupt 2 //外部中断1中断
{
	
}





